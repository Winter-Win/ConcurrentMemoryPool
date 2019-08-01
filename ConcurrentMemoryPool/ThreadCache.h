#pragma once

#include "Common.h"

class ThreadCache
{
private:
	Freelist _freelist[NLISTS];//自由链表

public:
	//申请和释放内存对象
	void* Allocate(size_t size);
	void Deallocate(void* ptr, size_t size);

	//从中心缓存获取对象
	void* FetchFromCentralCache(size_t index, size_t size);

	//释放对象时，链表过长时，回收内存回到中心堆
	void ListTooLong(Freelist* list, size_t size);
};

//静态的，不是所有可见
//每个线程有个自己的指针, 用(_declspec (thread))，我们在使用时，每次来都是自己的，就不用加锁了
//每个线程都有自己的tlslist
_declspec (thread) static ThreadCache* tlslist = nullptr;