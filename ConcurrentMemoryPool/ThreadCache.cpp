#include "ThreadCache.h"
#include "CentralCache.h"


//从中心缓存获取对象
// 每一次取批量的数据，因为每次到CentralCache申请内存的时候是需要加锁的
// 所以一次就多申请一些内存块，防止每次到CentralCache去内存块的时候,多次加锁造成效率问题
void* ThreadCache::FetchFromCentralCache(size_t index, size_t size)
{
	//打桩
	//return malloc(size);

	//打桩
	//size_t numtomove = 5;
	//void* start = nullptr, *end = nullptr;
	//size_t batchsize = CentralCache::Getinstance()->FetchRangeObj(start, end, numtomove, size);

	//if (batchsize > 1)
	//{
	//	Freelist* freelist = &_freelist[index];
	//	freelist->PushRange(NEXT_OBJ(start), end, batchsize - 1);
	//}
	//return start;

	Freelist* freelist = &_freelist[index];
	// 不是每次申请10个，而是进行慢增长的过程
	// 单个对象越小，申请内存块的数量越多
	// 单个对象越大，申请内存块的数量越小
	// 申请次数越多，数量多
	// 次数少,数量少
	size_t maxsize = freelist->MaxSize();
	size_t numtomove = min(SizeClass::NumMoveSize(size), maxsize);

	void* start = nullptr, *end = nullptr;
	// start，end分别表示取出来的内存的开始地址和结束地址
	// 取出来的内存是一个链在一起的内存对象，需要首尾标识

	// batchsize表示实际取出来的内存的个数
	// batchsize有可能小于num，表示中心缓存没有那么多大小的内存块
	size_t batchsize = CentralCache::Getinstence()->FetchRangeObj(start, end, numtomove, size);

	if (batchsize > 1)
	{
		freelist->PushRange(NEXT_OBJ(start), end, batchsize - 1);
	}

	if (batchsize >= freelist->MaxSize())
	{
		freelist->SetMaxSize(maxsize + 1);
	}

	return start;
}

//释放对象时，链表过长时，回收内存回到中心缓存
void ThreadCache::ListTooLong(Freelist* freelist, size_t size)
{
	//打桩
	//return nullptr;

	void* start = freelist->PopRange();
	CentralCache::Getinstence()->ReleaseListToSpans(start, size);
}

//申请和释放内存对象
void* ThreadCache::Allocate(size_t size)
{
	size_t index = SizeClass::Index(size);//获取到相对应的位置
	Freelist* freelist = &_freelist[index];
	if (!freelist->Empty())//在ThreadCache处不为空的话，直接取
	{
		return freelist->Pop();
	}
	// 自由链表为空的要去中心缓存中拿取内存对象，一次取多个防止多次去取而加锁带来的开销 
	// 均衡策略:每次中心堆分配给ThreadCache对象的个数是个慢启动策略
	//          随着取的次数增加而内存对象个数增加,防止一次给其他线程分配太多，而另一些线程申请
	//          内存对象的时候必须去PageCache去取，带来效率问题
	else
	{
		// 否则的话，从中心缓存处获取
		return FetchFromCentralCache(index, SizeClass::Roundup(size));
	}
}

void ThreadCache::Deallocate(void* ptr, size_t size)
{
	size_t index = SizeClass::Index(size);
	Freelist* freelist = &_freelist[index];
	freelist->Push(ptr);

	//满足某个条件时(释放回一个批量的对象)，释放回中心缓存
	if (freelist->Size() >= freelist->MaxSize())
	{
		ListTooLong(freelist, size);
	}
}


