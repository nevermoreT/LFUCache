/*
Filename LFUCacheImpl.h

*/
#ifndef __LFUCACHEIMPL_H__
#define __LFUCACHEIMPL_H__

/*Define the storage unit of LFUcache*/
typedef struct cacheEntryS{
	int key;
	int value;
	int freq;
	struct cacheEntryS *hashListPrev;
	struct cacheEntryS *hashListNext;

	struct cacheEntryS *lfuListPrev;
	struct cacheEntryS *lfuListNext;

}cacheEntryS;

typedef struct LFUCache{
	int cacheCapacity;
	cacheEntryS **hashmap;
	cacheEntryS *lfuListHead;
	cacheEntryS *lfuListTail;

	int lfuListSize;
}LFUCache;
#endif
