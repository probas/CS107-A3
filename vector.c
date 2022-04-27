#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define VALIDATE(v)     assert(NULL != v)
// #define VALIDATE(v)     if(!v) printf("%s:%d: Bad vector\n", __FUNCTION__, __LINE__);

static void VectorExpand(vector* v, float k) {
    VALIDATE(v);

    double coef = k ? k : 2;
    size_t new_sz = (size_t)(v->maxSz * coef);

    v->data = (uint8_t*)realloc((void*)v->data, new_sz);
    assert(v->data);
    v->maxSz = new_sz;
}

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation)
{
    assert(initialAllocation >= 0);
    assert(elemSize > 0);
    VALIDATE(v);

    v->maxSz = initialAllocation ? initialAllocation : DEFAULT_ALLOCATION_NUM;
    v->elemSz = elemSize;
    v->sz = 0;

    v->data = (void*)malloc(v->maxSz * v->elemSz);
    assert(v->data);

    v->freeFn = freeFn;
}

void VectorDispose(vector *v)
{
    VALIDATE(v);
    assert(v->data);

    if (v->freeFn && v->data) {
        for(int i = 0; i < v->sz; i++) {
            // NOTE: It is a responsibility of freeFn to cast from void* to elem*
            v->freeFn((uint8_t*)(v->data + i * v->elemSz));
        }
    }

    free(v->data);
    v->data = NULL;
    v->sz = 0;
    v->maxSz = 0;
    v->elemSz = 0;
}

int VectorLength(const vector *v)
{
    VALIDATE(v);

    return v->sz;
}

void *VectorNth(const vector *v, int position)
{
    VALIDATE(v);
    // validate index
    assert(position >= 0 && position <= v->sz - 1);

    // Note to caller: after deletion, insertion, etc the return pointer value becomes wrong
    return (void*)(v->data + position * v->elemSz);
}

void VectorInsert(vector *v, const void *elemAddr, int position)
{
    VALIDATE(v);
    // validate index
    assert(position >= 0 && position <= v->sz); // allow insert to the end <==> append
    assert(elemAddr);

    // re-alloc if needed
    if (v->sz == v->maxSz) {
        VectorExpand(v, 0);
    }

    void* pIns = v->data + v->elemSz * position;
    int size = v->sz - position;
    assert(size >= 0);

    if (size > 0) {
        void* pDst = v->data + v->elemSz*(position + 1);
        memmove(pDst, pIns, size);
    }

    memcpy(pIns, elemAddr, v->elemSz);

    v->sz += 1;
}

void VectorAppend(vector *v, const void *elemAddr)
{
    VALIDATE(v);

    VectorInsert(v, elemAddr, v->sz);
}

void VectorReplace(vector *v, const void *elemAddr, int position)
{
    VALIDATE(v);
    assert(position >= 0 && position < v->sz);

    void* pRep = v->data + v->elemSz * position;
    if (v->freeFn) {
        v->freeFn(pRep);
    }
    memcpy(pRep, elemAddr, v->elemSz);
}

void VectorDelete(vector *v, int position)
{
    VALIDATE(v);
    assert(position >= 0 && position < v->sz);

    void* pDel = v->data + v->elemSz * position;
    void* pSrc = v->data + v->elemSz * (position + 1);
    int size = v->sz - position - 1;
    assert(size >= 0);

    if (v->freeFn) {
        v->freeFn(pDel);
    }
    if (size > 0) {
        memmove(pDel, pSrc, size);
    }

    v->sz -= 1;
}

void VectorSort(vector *v, VectorCompareFunction compare)
{
    VALIDATE(v);
    assert(compare);

    // WARNING: This invalidates all previous vectorN calls
    qsort((void*)v->data, v->sz, v->elemSz, compare);
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)
{
    VALIDATE(v);
    assert(mapFn);
    void* pElem = NULL;

    for (int i = 0; i < v->sz; i++) {
        pElem = v->data + i * v->elemSz;
        mapFn(pElem, auxData);
    }
}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{
    VALIDATE(v);
    assert(key);
    assert(searchFn);
    assert(startIndex >= 0 && startIndex < v->sz);

    int pos = kNotFound;
    void* pStart = v->data + v->elemSz * startIndex;
    void* pElem = NULL;

    if (isSorted) {
        pElem = bsearch(key, pStart, v->sz, v->elemSz, searchFn);
        if (pElem) {
            if((((uint8_t*)pElem - (uint8_t*)v->data) % v->elemSz) == 0) {
                pos = ((uint8_t*)pElem - (uint8_t*)v->data) / v->elemSz;
            }
            if (pos < startIndex || pos >= v->sz) {
                pos = kNotFound;
            }
        }
    } else {
        // alternatively, use lfind()
        for (int i = startIndex; i < v->sz; i++) {
            pElem = v->data + v->elemSz * startIndex;
            if (0 == searchFn(key, pElem)) {
                return i;
            }
        }
    }

    return pos;
}
