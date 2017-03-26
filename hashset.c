#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
// for debugging purposes
#include <stdio.h>

void HashSetNew(hashset *h, int elemSize, int numBuckets
                , HashSetHashFunction hashfn
                , HashSetCompareFunction comparefn
                , HashSetFreeFunction freefn) {

    assert(h != NULL);
    assert(hashfn != NULL);
    assert(comparefn != NULL);
    assert(elemSize > 0);
    assert(numBuckets > 0);

    // malloc could also be used
    h->buckets = (char*)calloc(numBuckets, elemSize);
    assert( h->buckets != NULL);

    h->elemSize = elemSize;
    h->numBuckets = numBuckets;
    h->numElements = 0;
    h->hashfn = hashfn;
    h->comparefn = comparefn;
    h->freefn = freefn;
}

void HashSetDispose(hashset *h) {
    assert(h != NULL);

    h->elemSize = 0;
    h->numBuckets = 0;
    h->numElements = 0;
    h->hashfn = NULL;
    h->comparefn = NULL;
    h->freefn = NULL;
    // Destroying buckets structure
    char* elemAddr = h->buckets;
    // TODO: numBuckets or numElements?
    for (int i=0; i<h->numBuckets; i++) {
        h->freefn(elemAddr);
        elemAddr += h->elemSize;
    }
    free(h->buckets);
}

int HashSetCount(const hashset *h) {
    assert(h != NULL);
    // printf's only for debug
    printf("number of buckets: %d\n", h->numBuckets);
    printf("number of elements %d\n", h->numElements);
    return h->numElements;
}


void HashSetEnter(hashset *h, const void *elemAddr) {
    HashSetLookup(h, elemAddr);
    int hash = h->hashfn(elemAddr, h->numBuckets);
    void* targetAddr = (char*)h->buckets + hash*h->elemSize;
    memcpy(targetAddr, elemAddr, h->elemSize);
}

void *HashSetLookup(const hashset *h, const void *elemAddr) {
    assert(elemAddr != NULL);
    int hash = h->hashfn(elemAddr, h->numBuckets);
    assert(hash >= 0 && hash < h->numBuckets);
    void* targetAddr = (char*)h->buckets + hash*h->elemSize;
    if (h->comparefn(elemAddr, targetAddr) == 0) {
        return targetAddr;
    }
    return NULL;
}

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData) {
    char *targetAddr = h->buckets;
    // TODO: new C++11 foreach/for loop
    for (int i = 0; i < h->numBuckets; i++) {
        mapfn(targetAddr, auxData);
        targetAddr += h->elemSize;
    }
}
