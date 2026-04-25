#ifndef SBRKALLOC_H
#define SBRKALLOC_H

#include <stddef.h>

void *sbrkMalloc(size_t size);
void sbrkFree(void *ptr);
void *sbrkRealloc(void *ptr, size_t size);
size_t heapSize(void);

#endif
