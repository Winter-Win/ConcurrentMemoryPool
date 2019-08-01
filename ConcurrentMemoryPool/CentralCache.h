#pragma once

#include "Common.h"

//上面的ThreadCache里面没有的话，要从中心获取

/*
进行资源的均衡，对于ThreadCache的某个资源过剩的时候，可以回收ThreadCache内部的的内存
从而可以分配给其他的ThreadCache
只有一个中心缓存，对于所有的线程来获取内存的时候都应该是一个中心缓存
所以对于中心缓存可以使用单例模式来进行创建中心缓存的类
对于中心缓存来说要加锁
*/

//设计成单例模式
class CentralCache
{
public:
	static CentralCache* Getinstence()
	{
		return &_inst;
	}

	//从page cache获取一个span
	Span* GetOneSpan(SpanList& spanlist, size_t byte_size);

	//从中心缓存获取一定数量的对象给threa cache
	size_t FetchRangeObj(void*& start, void*& end, size_t n, size_t byte_size);

	//将一定数量的对象释放给span跨度
	void ReleaseListToSpans(void* start, size_t size);

private:
	SpanList _spanlist[NLISTS];

private:
	CentralCache(){}//声明不实现，防止默认构造，自己创建

	CentralCache(CentralCache&) = delete;
	static CentralCache _inst;
};