#pragma once

#include "Common.h"

// 设计成单例模式
class CentralCache
{
public:
	static CentralCache* GetInstance()
	{
		return &_inst;
	}

	// 从page cache获取一个span
	Span* GetOneSpan(SpanList& spanlist, size_t byte_size);

	// 从中心缓存获取一定数量的对象给thread cache
	size_t FetchRangeObj(void*& start, void*& end, size_t n, size_t byte_size);

	// 将一定数量的对象释放到span跨度
	void ReleaseListToSpans(void* start, size_t size);
private:
	SpanList _spanlist[NLISTS];

private:
	CentralCache()
	{}

	CentralCache(CentralCache&) = delete;

	static CentralCache _inst;
};