#pragma once

#include "Common.h"

//对于Page Cache也要设置为单例，对于Central Cache获取span的时候
//每次都是从同一个page数组中获取span
//单例模式
class PageCache
{
public:
	static PageCache* GetInstence()
	{
		return &_inst;
	}

	Span* AllocBigPageObj(size_t size);
	void FreeBigPageObj(void* ptr, Span* span);

	Span* _NewSpan(size_t n);
	Span* NewSpan(size_t n);//获取的是以页为单位

	//获取从对象到span的映射
	Span* MapObjectToSpan(void* obj);

	//释放空间span回到PageCache，并合并相邻的span
	void ReleaseSpanToPageCache(Span* span);

private:
	SpanList _spanlist[NPAGES];
	//std::map<PageID, Span*> _idspanmap;
	std::unordered_map<PageID, Span*> _idspanmap;

	std::mutex _mutex;
private:
	PageCache(){}

	PageCache(const PageCache&) = delete;
	static PageCache _inst;
};