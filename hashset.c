#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define VALIDATE(c)     assert(NULL != c)

// #define ALLOW_COLLISION

/**
 * Function:  HashSetNew
 * ---------------------
 * TC: or one previously initialized but later destroyed using HastSetDispose
 * TC: numBuckets parameter once a hashset is created, this number does
 * not change.
 * An assert is raised unless all of the following conditions are met:
 *    - elemSize is greater than 0.
 *    - numBuckets is greater than 0.
 *    - hashfn is non-NULL
 *    - comparefn is non-NULL
 */



void HashSetNew(hashset *h,
				int elemSize,
				int numBuckets,
				HashSetHashFunction hashfn,
				HashSetCompareFunction comparefn,
				HashSetFreeFunction freefn)
{
	assert(elemSize > 0);
	assert(numBuckets > 0);
	assert(comparefn && hashfn); // these functions must exist
	VALIDATE(h);

	h->sz = 0;
	h->elemSz = elemSize;
	h->numBuckets = numBuckets;
	h->compareFn = comparefn;
	h->freeFn = freefn;
	h->hashFn = hashfn;

	h->data = (uint8_t*)malloc(numBuckets * elemSize);
	assert(h->data);
	memset(h->data, 0, elemSize);
}

void HashSetDispose(hashset *h)
{
	VALIDATE(h);

	if (h->freeFn) {
		for (int i = 0; i < h->numBuckets; i++) {
			h->freeFn(h->data + i * h->elemSz);
		}
	}

	free(h->data);
	h = NULL;
}

int HashSetCount(const hashset *h)
{
	VALIDATE(h);
	return h->sz;
}

void HashSetMap(hashset *h, HashSetMapFunction mapFn, void *auxData)
{
	VALIDATE(h);
	assert(mapFn);

	for (int i = 0; i < h->numBuckets; i++) {
		mapFn(h->data + i * h->elemSz, auxData);
	}
}

void HashSetEnter(hashset *h, const void *elemAddr)
{
/*
 If the specified element matches an element previously inserted (as far as the hash and compare functions are concerned), the the old element is replaced by this new element.
 */
	VALIDATE(h);
	assert(elemAddr);

	uint8_t* pDst;
	int hash = h->hashFn(elemAddr, h->numBuckets);
	assert(hash >= 0 && hash <  h->numBuckets);

	pDst = h->data + hash * h->elemSz;

#ifdef ALLOW_COLLISION
	if (pDst == NULL || h->compareFn(pDst, elemAddr) == 0) {
		uint8_t* pElem = malloc(h->elemSz);
		assert(pElem);
		memcpy(pElem, elemAddr, h->elemSz);
		pDst = pElem;
	}
#else
	if (h->compareFn(pDst, elemAddr) != 0) {
		h->sz++;
	}
	memcpy(pDst, elemAddr, h->elemSz);
#endif
}

void *HashSetLookup(const hashset *h, const void *elemAddr)
{
	VALIDATE(h);
	assert(elemAddr);

	int hash = h->hashFn(elemAddr, h->numBuckets);
	assert(hash >= 0 && hash <  h->numBuckets);

	uint8_t* pDst = h->data + hash * h->elemSz;

	if (h->compareFn(pDst, elemAddr) == 0) {
		return pDst;
	}
	return NULL;
}
