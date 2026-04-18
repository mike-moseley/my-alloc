#ifndef ALLOC_H
#define ALLOC_H

#include <stddef.h>
#include "alloc_error.h"

void *myMalloc(size_t size);
void myFree(void *ptr);
void *myRealloc(void *ptr, size_t size);
size_t myHeapSize(void);

#endif
