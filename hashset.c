#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void HashSetNew(hashset *h, int elemSize, int numBuckets,
        HashSetHashFunction hashfn,
        HashSetCompareFunction comparefn,
        HashSetFreeFunction freefn) {
    
    assert(h->elemSize > 0);
    assert(h->numBuckets > 0);
    assert(hashfn!= NULL);
    assert(comparefn!= NULL);

    h->numElements = 0;
    h->elemSize = elemSize;
    h->numBuckets = numBuckets;

    h->buckets = malloc(sizeof(vector)*numBuckets);
    assert(h->buckets != NULL);
    for (int i = 0; i < numBuckets; i++) {
        VectorNew(h->buckets + i, elemSize, NULL, 0);
    }

    h->hashfn = hashfn;
    h->comparefn = comparefn;
    h->freefn = freefn;
}

void HashSetDispose(hashset *h) {
    for (int i = 0; i < h->numBuckets; i++) {
        VectorDispose(h->buckets + i);
    }
}

int HashSetCount(const hashset *h) {
    return h->numElements;
}

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData)
{}

void hashSetEnter(hashset *h, const void *elemAddr) {
    void *lookup = HashSetLookup(h, elemAddr);

    if( lookup != NULL ) {
        
    }
    
    /*
    If the specified element matches an
    element previously inserted (as far as the hash
    and compare functions are concerned), then the
    old element is replaced by this new element
    */
    if( h->comparefn(elemAddr, bucketAddress->elements) != 0) {
        h->numElements += 1;
        VectorAppend(bucketAddress, elemAddr);
    } else {
        VectorReplace(bucketAddress, elemAddr, 0); //not correct
    }
}

void *HashSetLookup(const hashset *h, const void *elemAddr) {
    int hashcode;
    hashcode = h->hashfn(elemAddr, h->numBuckets);   
    assert(hashcode >= 0 && hashcode < h->numBuckets - 1);
    vector *bucketAddress = h->buckets + hashcode;

    // HashSetCompareFunction here

    return NULL;
}
