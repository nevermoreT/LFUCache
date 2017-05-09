/*
*file name:LFUCache.h   
*desp:LFU Implement
*
*/
#ifndef __LFUCACHE_H__
#define __LFUCACHE_H__


/*create the LFUCache*/
int LFUCacheCreate(int capacity,void **lfuCache);

/*Destory the LFUCache*/
int LFUCahceFree(void *lfuCache);

/*Set value by key*/
int LFUCacheSet(void *lfuCache, int key, int value);

/*Get value by key*/
int LFUCacheGet(void *lfuCache, int key);

/*print all value by Frequen Recent*/
void LfUCachePrint(void *lfuCache);

#endif //_LFUCACHE_H__
