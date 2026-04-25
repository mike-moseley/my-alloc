#ifndef ALLOC_H
#define ALLOC_H

#include <stddef.h>
#include "alloc_error.h"

void *sbrkMalloc(size_t size);
void sbrkFree(void *ptr);
void *sbrkRealloc(void *ptr, size_t size);
size_t heapSize(void);

#endif
