#include "CentralCache.h"
#include "PageCache.h"

CentralCache CentralCache::_inst;

Span* CentralCache::GetOneSpan(SpanList& spanlist, size_t byte_size)
{
	Span* span = spanlist.Begin();
	while (span != spanlist.End())//当前找到一个span
	{
		if (span->_list != nullptr)
			return span;
		else
			span = span->_next;
	}


	////测试打桩
	//Span* newspan = new Span;
	//newspan->_objsize = 16;
	//void* ptr = malloc(16 * 8);
	//void* cur = ptr;
	//for (size_t i = 0; i < 7; ++i)
	//{
	//	void* next = (char*)cur + 16;
	//	NEXT_OBJ(cur) = next;
	//	cur = next;
	//}
	//NEXT_OBJ(cur) = nullptr;
	//newspan->_list = ptr;



	// 走到这儿，说明前面没有获取到span,都是空的，到下一层pagecache获取span
	Span* newspan = PageCache::GetInstence()->NewSpan(SizeClass::NumMovePage(byte_size));
	// 将span页切分成需要的对象并链接起来
	char* cur = (char*)(newspan->_pageid << PAGE_SHIFT);
	char* end = cur + (newspan->_npage << PAGE_SHIFT);
	newspan->_list = cur;
	newspan->_objsize = byte_size;

	while (cur + byte_size < end)
	{
		char* next = cur + byte_size;
		NEXT_OBJ(cur) = next;
		cur = next;
	}
	NEXT_OBJ(cur) = nullptr;

	spanlist.PushFront(newspan);

	return newspan;
}


//获取一个批量的内存对象
size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t n, size_t byte_size)
{
	size_t index = SizeClass::Index(byte_size);
	SpanList& spanlist = _spanlist[index];//赋值->拷贝构造

	////到时候记得加锁
	//spanlist.Lock();
	std::unique_lock<std::mutex> lock(spanlist._mutex);


	Span* span = GetOneSpan(spanlist, byte_size);
	//到这儿已经获取到一个newspan

	//从span中获取range对象
	size_t batchsize = 0;
	void* prev = nullptr;//提前保存前一个
	void* cur = span->_list;//用cur来遍历，往后走
	for (size_t i = 0; i < n; ++i)
	{
		prev = cur;
		cur = NEXT_OBJ(cur);
		++batchsize;
		if (cur == nullptr)//随时判断cur是否为空，为空的话，提前停止
			break;
	}

	start = span->_list;
	end = prev;

	span->_list = cur;
	span->_usecount += batchsize;

	//将空的span移到最后，保持非空的span在前面
	if (span->_list == nullptr)
	{
		spanlist.Erase(span);
		spanlist.PushBack(span);
	}

	//spanlist.Unlock();

	return batchsize;
}

void CentralCache::ReleaseListToSpans(void* start, size_t size)
{
	size_t index = SizeClass::Index(size);
	SpanList& spanlist = _spanlist[index];

	//将锁放在循环外面
	// CentralCache:对当前桶进行加锁(桶锁)，减小锁的粒度
	// PageCache:必须对整个SpanList全局加锁
	// 因为可能存在多个线程同时去系统申请内存的情况
	//spanlist.Lock();
	std::unique_lock<std::mutex> lock(spanlist._mutex);

	while (start)
	{
		void* next = NEXT_OBJ(start);

		////到时候记得加锁
		//spanlist.Lock(); // 构成了很多的锁竞争

		Span* span = PageCache::GetInstence()->MapObjectToSpan(start);
		NEXT_OBJ(start) = span->_list;
		span->_list = start;
		//当一个span的对象全部释放回来的时候，将span还给pagecache,并且做页合并
		if (--span->_usecount == 0)
		{
			spanlist.Erase(span);
			PageCache::GetInstence()->ReleaseSpanToPageCache(span);
		}

		//spanlist.Unlock();

		start = next;
	}

	//spanlist.Unlock();
}