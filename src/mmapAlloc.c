#include "mmapAlloc.h"
#include "alloc_common.h"
#include "alloc_error.h"
#include <sys/mman.h>

typedef struct block_t {
	size_t size;
} block_t;

AllocError mmapAlloc(size_t size, void **out) {
	block_t *block;
	size_t block_size;

	if(size == 0) return ALLOC_ERROR_SIZE;
	if(out == NULL) return ALLOC_ERROR_NULL;

	block_size = ALIGN(size) + sizeof(block_t);
	block = (block_t *)mmap(NULL, block_size, PROT_READ | PROT_WRITE,
	                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if(block == MAP_FAILED) return ALLOC_ERROR_OOM;

	block->size = block_size;

	/* Point to data, not to header */
	*out = (char *)block + sizeof(block_t);

	return ALLOC_OK;
}
