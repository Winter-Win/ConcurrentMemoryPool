#pragma once
#include "Common.h"
#include "ThreadCache.h"


void* ConcurrentAlloc(size_t size)
{
	if (size > MAX_BYTES)
	{
		return malloc(size);
	}
	else
	{
		if (tlslist == nullptr)
		{
			//cout << std::this_thread::get_id() << endl;
			tlslist = new ThreadCache;
			//cout << tlslist << endl;
		}

		//cout << tlslist << endl;

		return tlslist->Allocate(size);
	}
}

void ConcurrentFree(void* ptr, size_t size)
{
	if (size > MAX_BYTES)
	{
		free(ptr);
	}
	else
	{
		tlslist->Deallocate(ptr, size);
	}
}