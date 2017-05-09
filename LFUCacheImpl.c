#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"LFUCache.h"
#include"LFUCacheImpl.h"
/**********************************************
*Achievement of LFUCache Implement
**********************************************/

static void freeList(LFUCache *cache);
void LFUCachePrint(void *lfuCache);
/*Create a new cache unit*/
static cacheEntryS* newCacheEntry(int key,int value)
{
	cacheEntryS *entry=NULL;
	if (NULL==(entry=malloc(sizeof(*entry))))
	{
		perror("malloc");
		return NULL;
	}

	memset(entry,0,sizeof(*entry));
	entry->key=key;
	entry->value=value;
	
	return entry;
}

/*free a cache unit*/
static void freeCacheEentry(cacheEntryS *entry)
{
	if (entry!=NULL)
		free(entry);
	else return;
}

/*Create a LFUCache*/
int LFUCacheCreate(int capacity,void **lfuCache)
{
	LFUCache *cache=NULL;
	if (NULL==(cache=malloc(sizeof(*cache))))
	{
		perror("malloc");
		return -1;
	}
	memset(cache,0,sizeof(*cache));
	cache->cacheCapacity=capacity;
	cache->hashmap=malloc(sizeof(cacheEntryS)*capacity);
	if (NULL==cache->hashmap)
	{
		free(cache);
		perror("malloc");
		return -1;
	}
	memset(cache->hashmap,0,sizeof(cacheEntryS)*capacity);
	*lfuCache=cache;
	return 0;
}

/*Free a LFUCache*/
int LFUCacheFree(void *lfuCache)
{
	LFUCache *cache=(LFUCache*) lfuCache;
	if (cache==NULL) return 0;
	else 
	{
		if (cache->hashmap)
			free(cache->hashmap);
		freeList(lfuCache);
		free(lfuCache);
		return 0;
	}
}

static void removeFromList(LFUCache *cache, cacheEntryS *entry)
{
	    /*链表为空*/
	    if (cache->lfuListSize<=0)
			        return ;

		if (entry==cache->lfuListHead && entry==cache->lfuListTail) {
			    /*链表中仅剩当前一个节点*/
			        cache->lfuListHead = cache->lfuListTail = NULL;
					    
		} else if (entry == cache->lfuListHead) {
			    /*欲删除节点位于表头*/
			        cache->lfuListHead = entry->lfuListNext;
					        cache->lfuListHead->lfuListPrev = NULL;
					
		} else if (entry == cache->lfuListTail) {
			    /*欲删除节点位于表尾*/
			        cache->lfuListTail = entry->lfuListPrev;
					cache->lfuListTail->lfuListNext = NULL;
						    
		} else {
			    /*其他非表头表尾情况，直接摘抄节点*/
			        entry->lfuListPrev->lfuListNext = entry->lfuListNext;
					entry->lfuListNext->lfuListPrev = entry->lfuListPrev;
						    
		}
		    /*删除成功，链表节点数减1*/
		    cache->lfuListSize--;
}
/*add a node to the head*/
static cacheEntryS * insertToListHead(LFUCache *cache, cacheEntryS *entry) 
{
	    cacheEntryS *removedEntry = NULL;
		//printf("%d %d\n",cache->cacheCapacity,cache->lfuListSize);
		
		if (++cache->lfuListSize > cache->cacheCapacity) {
			    /* */
			removedEntry = cache->lfuListHead;
			removeFromList(cache, cache->lfuListHead);   
			//LFUCachePrint(cache);					    
		}

		if (cache->lfuListHead==NULL&&cache->lfuListTail==NULL) {
			    /*如果当前链表为空链表*/
			cache->lfuListHead = cache->lfuListTail = entry;
						    
		} else {
			    /*当前链表非空，插入表头*/
			entry->lfuListNext = cache->lfuListHead;
			entry->lfuListPrev = NULL;
			cache->lfuListHead->lfuListPrev = entry;
			cache->lfuListHead = entry;
															    
		}

		    return removedEntry;

}


static void freeList(LFUCache *cache)
{
	if (0==cache->lfuListSize) return;
	cacheEntryS * entry=cache->lfuListHead;
	while (entry)
	{
		cacheEntryS * temp=entry->lfuListNext;
		freeCacheEentry(entry);
		entry=temp;
	}
	cache->lfuListSize=0;
}
/*update the lfulist by freq*/
static void updateLFUlist(LFUCache *cache,cacheEntryS * entry)
{
	cacheEntryS* temp=entry->lfuListNext;
	/*list has one entry*/
	if (NULL==temp) return;
	if (temp->freq>entry->freq) return;
	while (temp)
	{
		if (temp->freq>entry->freq) break;
		temp=temp->lfuListNext;
		
	}
	/*LIST tail*/
	removeFromList(cache,entry);//dont forget to add the lfusize!
	cache->lfuListSize++;
	if (temp==NULL)
	{
		cache->lfuListTail->lfuListNext=entry;
		entry->lfuListPrev=cache->lfuListTail;
		cache->lfuListTail=entry;
		entry->lfuListNext=NULL;
	}
	/*
	else
	if (temp->lfuListPrev==NULL)
	{
		insertToListHead(cache,entry);
	}*/
    else
	{
		temp->lfuListPrev->lfuListNext=entry;
		entry->lfuListPrev=temp->lfuListPrev;
		entry->lfuListNext=temp;
		temp->lfuListPrev=entry;

	}

}


/*get the hashKey*/
int hashKey(LFUCache *cache,int key)
{
	return key%cache->cacheCapacity;
}

static cacheEntryS *getValueFromHashMap(LFUCache *cache, int key) {
	/*1.使用函数函数定位数据存放在哪个槽*/
	cacheEntryS *entry = cache->hashmap[hashKey(cache,key)];
if (entry==NULL)
	
		    /*2.遍历查询槽内链表，找到准确的数据项*/
		while (entry) {
			if (entry->key == key)
			{printf("key=%d value=%d\n",entry->key,entry->value);
			break;}
			entry = entry->hashListNext;
							    
		}
			//printf("key=%d value=%d freq=%d\n",entry->key,entry->value,entry->freq);
		    return entry;

}

static void insertentryToHashMap(LFUCache* cache,cacheEntryS* entry )
{
	cacheEntryS* n=cache->hashmap[hashKey(cache,entry->key)];
	if (n!=NULL)
	{
		entry->hashListNext=n;
		n->lfuListPrev=entry;

	}
	cache->hashmap[hashKey(cache,entry->key)]=entry;
}

static void removeEntryFromHashMap(LFUCache*cache,cacheEntryS *entry )
{
	if (NULL==entry||NULL==cache->hashmap[hashKey(cache,entry->key)])
		return;
	cacheEntryS *n=cache->hashmap[hashKey(cache,entry->key)];
	while (n)
	{
		if (n->key==entry->key)
		{
			if (n->hashListPrev){/*process the head*/
					n->hashListPrev->hashListNext=n->hashListNext;

					}else
					{
						cache->hashmap[hashKey(cache, entry->key)] = n->hashListNext;
					}
		

		if (n->hashListNext)
		{
			n->hashListNext->hashListPrev=n->hashListPrev;
		}

	}
	n=n->hashListNext;
}
}

int LFUCacheSet(void *lfuCache,int key,int value)
{
	LFUCache *cache=(LFUCache*) lfuCache;
	//fprintf(stdout,"create cache success!\n");
	cacheEntryS *entry=getValueFromHashMap(cache,key);
	//fprintf(stdout,"create cacheEntryS success!\n");
	if (entry==NULL)
	{
		entry=newCacheEntry(key,value);
		entry->freq=1;
		cacheEntryS *removedEntry=insertToListHead(cache,entry);
		if (NULL!=removedEntry)
		{
			removeEntryFromHashMap(cache,removedEntry);
			freeCacheEentry(removedEntry);
			
		}
		insertentryToHashMap(cache,entry);
	}
	else
	{
		entry->value=value;
		entry->freq++;
		updateLFUlist(cache,entry);
	}
	return 0;
		
}

int LFUCacheGet(void *lfuCache,int key)
{
	LFUCache *cache=(LFUCache*) lfuCache;
	cacheEntryS *entry=getValueFromHashMap(cache,key);
	
	if (entry)
	{
		entry->freq++;
		updateLFUlist(cache,entry);
		return entry->value;
	}
	else return -1;
}

void LFUCachePrint(void *lfuCache)
{
	    LFUCache *cache = (LFUCache *)lfuCache;
		    if (NULL==cache||0 == cache->lfuListSize) return ;
			    fprintf(stdout, "\n>>>>>>>>>>\n");
				    fprintf(stdout, "cache  (key  data):\n");
					    cacheEntryS *entry = cache->lfuListHead;
						while(entry) {
							        fprintf(stdout, "(%d, %d, %d) ", entry->key, entry->value,entry->freq);
									        entry = entry->lfuListNext;
											    
						}
						    fprintf(stdout, "\n<<<<<<<<<<\n\n");
}
