#ifndef MMAPALLOC_H
#define MMAPALLOC_H

#include <stddef.h>
#include "alloc_error.h"

AllocError mmapMalloc(size_t size);
void mmapFree(void *ptr);
AllocError mmapRealloc(void *ptr, size_t size);

#endif
