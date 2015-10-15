#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <search.h>

#define kInitialAllocationSize 10

static const int kNotFound = -1;

void VectorNew(vector *v, int elemSize, VectorFreeFunction freefn, int initialAllocation) {
    assert ( initialAllocation >= 0 );
    if ( initialAllocation == 0 ) {
        initialAllocation = kInitialAllocationSize;
    }
    v->elements = malloc(initialAllocation * elemSize);
    assert ( v->elements != NULL );
    v->freefn = freefn;
    v->elementSize = elemSize;
    v->size = initialAllocation;
    v->logSize = 0;
}

void VectorDispose(vector *v) {
    if( v->freefn != NULL ) {
        void *addr = (char*)v->elements + i * v->elementSize;
        for (int i = 0; i < v->logSize; i++) {
            v->freefn(addr);
            adddr += v->elemSize;
        }
    }
}

int VectorLength(const vector *v) {
    assert(v != NULL);
    return v->logSize;
}

void *VectorNth(const vector *v, int position) {
    void *targetAddress;
    int maxIndex = v->logSize - 1;
    
    assert(position >= 0 && position <= maxIndex);
    targetAddress = (char*)v->elements + position * v->elementSize;  //works without cast to (char *)?
    
    return targetAddress;
}

void VectorReplace(vector *v, const void *elemAddr, int position)
{}

void VectorInsert(vector *v, const void *elemAddr, int position)
{}

void VectorAppend(vector *v, const void *elemAddr)
{}

void VectorDelete(vector *v, int position)
{}

void VectorSort(vector *v, VectorCompareFunction compare)
{}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)
{}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{ return -1; } 
