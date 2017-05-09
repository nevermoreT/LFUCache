/*
*file name: main.c
* desp: LFU 缓存测试用例
* */
#include <stdio.h>
#include <stdlib.h>
#include "LFUCache.h"

/*错误处理宏*/
#define HANDLE_ERROR(msg) \
	        do{ fprintf(stderr, "%s fail.\n", msg);exit(-1); }while(0)

/*封装缓存数据储存接口，此处我们让data同时充当key的角色*/
#define LFUCACHE_PUTDATA(cache, data) \
	do {\
	    if (0 != LFUCacheSet(cache, data, data)) \
	        fprintf(stderr, "put (%d,%d) to cache fail.\n", data, data);\
	    else \
	        fprintf(stdout, "put (%d,%d) to cache success.\n", data, data); \
	}while(0)

/*封装缓存数据储存接口*/
#define LFUCACHE_GETDATA(cache, key) \
	do{\
	    int data = LFUCacheGet(cache, key);\
	    if (-1== data) \
	        fprintf(stderr, "get data (Key:%d) from cache fail.\n", key);\
	    else if (key == data) \
	        fprintf(stdout, "got (%d,%d) from cache\n", key, data);\
	}while(0)

/*测试用例1*/    
void testcase1(void)
{
	fprintf(stdout, "=========================\n");
	fprintf(stdout, "In testcase1....\n");
	fprintf(stdout, "=========================\n");
	void *lfuCache;
	if (0 != LFUCacheCreate(5, &lfuCache)) 
	HANDLE_ERROR("LRUCacheCreate");
	//printf("%d\n",lfuCache->cacheCapacity);
	fprintf(stdout,"Begin....\n");
	/*ABC!*/
	LFUCACHE_PUTDATA(lfuCache, 1);
	LFUCachePrint(lfuCache);
	//LFUCACHE_GETDATA(lfuCache, 1);
	LFUCACHE_GETDATA(lfuCache, 1);
	LFUCACHE_PUTDATA(lfuCache, 2);
	LFUCachePrint(lfuCache);
	LFUCACHE_GETDATA(lfuCache, 2);
	LFUCachePrint(lfuCache);
	LFUCACHE_PUTDATA(lfuCache, 3);
	LFUCACHE_GETDATA(lfuCache, 3);
	LFUCachePrint(lfuCache);/*CBA*/

													    /*DEAF!*/
	LFUCACHE_PUTDATA(lfuCache, 4);
	LFUCACHE_GETDATA(lfuCache, 4);
	LFUCACHE_PUTDATA(lfuCache, 4);
	/*															    
	LFUCACHE_GETDATA(lruCache, 'E');
																	    LFUCACHE_PUTDATA(lruCache, 'A');
																		    LFUCACHE_GETDATA(lruCache, 'A');
																			    LFUCACHE_PUTDATA(lruCache, 'F');
																				    LFUCACHE_GETDATA(lruCache, 'F');
																					*/
	LFUCachePrint(lfuCache); /*FAEDC*/

	/*B!*/
    LFUCACHE_PUTDATA(lfuCache, 5);
	LFUCACHE_GETDATA(lfuCache, 5);
	LFUCachePrint(lfuCache);
	LFUCACHE_PUTDATA(lfuCache, 6);
	LFUCACHE_GETDATA(lfuCache, 6);
	LFUCachePrint(lfuCache);
	LFUCACHE_PUTDATA(lfuCache, 7);
	LFUCachePrint(lfuCache); /*BFAED*/
	if (0 != LFUCacheFree(lfuCache))
		HANDLE_ERROR("LRUCacheDestory");
	fprintf(stdout, "\n\ntestcase1 finished\n");
	fprintf(stdout, "=========================\n\n");

}

int main()
{
	testcase1();
	return 0;
}
