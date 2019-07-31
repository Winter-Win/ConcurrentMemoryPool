//#include "Common.h"
//#include "PageCache.h"
//#include "ConcurrentAlloc.h"
//
//void TestSize()
//{
//	/*cout << SizeClass::Index(10) << endl;
//	cout << SizeClass::Index(16) << endl;
//	cout << SizeClass::Index(128) << endl;
//	cout << SizeClass::Index(129) << endl;
//	cout << SizeClass::Index(128 + 17) << endl;
//	cout << SizeClass::Index(1025) << endl;
//	cout << SizeClass::Index(1024 + 129) << endl;
//	cout << SizeClass::Index(8*1024+1) << endl;
//	cout << SizeClass::Index(8*1024 + 1024) << endl;
//	*/
//
//	cout << SizeClass::Roundup(10) << endl;
//	cout << SizeClass::Roundup(1025) << endl;
//	cout << SizeClass::Roundup(1024*8+1) << endl;
//
//	cout << SizeClass::NumMovePage(16) << endl;
//	cout << SizeClass::NumMovePage(1024) << endl;
//	cout << SizeClass::NumMovePage(1024*8) << endl;
//	cout << SizeClass::NumMovePage(1024*64) << endl;
//
//}
//
//void Alloc(size_t n)
//{
//	size_t begin1 = clock();
//	std::vector<void*> v;
//	for (size_t i = 0; i < n; ++i)
//	{
//		v.push_back(ConcurrentAlloc(10));
//	}
//
//	//v.push_back(ConcurrentAlloc(10));
//
//	for (size_t i = 0; i < n; ++i)
//	{
//		ConcurrentFree(v[i], 10);
//		cout << v[i] << endl;
//	}
//	v.clear();
//	size_t end1 = clock();
//
//	size_t begin2 = clock();
//	cout << endl << endl;
//	for (size_t i = 0; i < n; ++i)
//	{
//		v.push_back(ConcurrentAlloc(10));
//	}
//
//	for (size_t i = 0; i < n; ++i)
//	{
//		ConcurrentFree(v[i], 10);
//		cout << v[i] << endl;
//	}
//	v.clear();
//	size_t end2 = clock();
//
//	cout << end1 - begin1 << endl;
//	cout << end2 - begin2 << endl;
//}
//
//void TestThreadCache()
//{
//	std::thread t1(Alloc, 1000000);
//	//std::thread t2(Alloc, 5);
//	//std::thread t3(Alloc, 5);
//	//std::thread t4(Alloc, 5);
//
//	t1.join();
//	//t2.join();
//	//t3.join();
//	//t4.join();
//
//}
//
//void TestCentralCache()
//{
//	std::vector<void*> v;
//	for (size_t i = 0; i < 8; ++i)
//	{
//		v.push_back(ConcurrentAlloc(10));
//	}
//
//	for (size_t i = 0; i < 8; ++i)
//	{
//		//ConcurrentFree(v[i], 10);
//		cout << v[i] << endl;
//	}
//}
//
//void TestPageCache()
//{
//	PageCache::GetInstance()->NewSpan(2);
//}
//
//int main()
//{
//	//TestSize();
//	//TestThreadCache();
//	//TestCentralCache();
//	TestPageCache();
//
//	//void* ptr = VirtualAlloc(0, (2)*(1 << PAGE_SHIFT),
//	//	MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
//	//PageID id = (PageID)ptr >> PAGE_SHIFT;
//	//cout << id << endl;
//
//	//char* obj = (char*)ptr + 100;
//	//PageID objid = (PageID)obj >> PAGE_SHIFT;
//	//void* pageptr = (void*)(id << PAGE_SHIFT);
//	//cout << pageptr << endl;
//	//cout << ptr << endl;
//
//	system("pause");
//	return 0;
//}



#include "Common.h"
#include "PageCache.h"
#include "ConcurrentAlloc.h"

void TestSize()
{
	/*cout << SizeClass::Index(10) << endl;
	cout << SizeClass::Index(16) << endl;
	cout << SizeClass::Index(128) << endl;
	cout << SizeClass::Index(129) << endl;
	cout << SizeClass::Index(128 + 17) << endl;
	cout << SizeClass::Index(1025) << endl;
	cout << SizeClass::Index(1024 + 129) << endl;
	cout << SizeClass::Index(8*1024+1) << endl;
	cout << SizeClass::Index(8*1024 + 1024) << endl;
	*/

	cout << SizeClass::Roundup(10) << endl;
	cout << SizeClass::Roundup(1025) << endl;
	cout << SizeClass::Roundup(1024 * 8 + 1) << endl;

	cout << SizeClass::NumMovePage(16) << endl;
	cout << SizeClass::NumMovePage(1024) << endl;
	cout << SizeClass::NumMovePage(1024 * 8) << endl;
	cout << SizeClass::NumMovePage(1024 * 64) << endl;

}

void Alloc(size_t n)
{
	size_t begin1 = clock();
	std::vector<void*> v;
	for (size_t i = 0; i < n; ++i)
	{
		v.push_back(ConcurrentAlloc(10));
	}

	//v.push_back(ConcurrentAlloc(10));

	for (size_t i = 0; i < n; ++i)
	{
		ConcurrentFree(v[i], 10);
		cout << v[i] << endl;
	}
	v.clear();
	size_t end1 = clock();

	size_t begin2 = clock();
	cout << endl << endl;
	for (size_t i = 0; i < n; ++i)
	{
		v.push_back(ConcurrentAlloc(10));
	}

	for (size_t i = 0; i < n; ++i)
	{
		ConcurrentFree(v[i], 10);
		cout << v[i] << endl;
	}
	v.clear();
	size_t end2 = clock();

	cout << end1 - begin1 << endl;
	cout << end2 - begin2 << endl;
}

void TestThreadCache()
{
	std::thread t1(Alloc, 1000000);
	//std::thread t2(Alloc, 5);
	//std::thread t3(Alloc, 5);
	//std::thread t4(Alloc, 5);

	t1.join();
	//t2.join();
	//t3.join();
	//t4.join();

}

void TestCentralCache()
{
	std::vector<void*> v;
	for (size_t i = 0; i < 8; ++i)
	{
		v.push_back(ConcurrentAlloc(10));
	}

	for (size_t i = 0; i < 8; ++i)
	{
		//ConcurrentFree(v[i], 10);
		cout << v[i] << endl;
	}
}

void TestPageCache()
{
	PageCache::GetInstance()->NewSpan(2);
}

void TestConcurrentAllocFree()
{
	size_t n = 100;
	std::vector<void*> v;
	for (size_t i = 0; i < n; ++i)
	{
		void* ptr = ConcurrentAlloc(12);
		v.push_back(ptr);
		printf("obj:%d->%p\n", i, ptr);
	}
}

int main()
{
	//TestSize();
	//TestThreadCache();
	//TestCentralCache();
	//TestPageCache();
	//TestConcurrentAllocFree();


	system("pause");
	return 0;
}