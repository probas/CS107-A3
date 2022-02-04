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
    assert(v->freeFn);

    for(int i = 0; i < v->sz; i++) {
        // NOTE: It is a responsibility of freeFn to cast from void* to elem*
        v->freeFn((int*)(v->data + i * v->elemSz));
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

    if (position == v->sz) {
        VectorAppend(v, elemAddr);
        return;
    }

    // re-alloc if needed
    if (v->sz + 1 == v->maxSz) {
        VectorExpand(v, 0);
    }

    // make room for an element to be inserted
    void* pDst = v->data + v->elemSz*(position + 1);
    void* pIns = v->data + v->elemSz*(position);
    memmove(pDst, pIns, v->sz - position);

    memcpy(pIns, elemAddr, v->elemSz);

    v->sz += 1;
}

void VectorAppend(vector *v, const void *elemAddr)
{
    VALIDATE(v);

    if (v->sz == v->maxSz) {
        VectorExpand(v, 0);
    }
    memmove((void*)(v->data + v->sz * v->elemSz), elemAddr, v->elemSz);
    v->sz += 1;
}

void VectorReplace(vector *v, const void *elemAddr, int position)
{
    // TODO: implement
}

void VectorDelete(vector *v, int position)
{
    // TODO: implement
    v->data = NULL;
    v->sz = 0;
    v->maxSz = 0;
    v->elemSz = 0;
}

void VectorSort(vector *v, VectorCompareFunction compare)
{
    // TODO: implement
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)
{
    // TODO: implement
}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{
    return -1;
}
