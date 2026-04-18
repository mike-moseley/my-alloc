#include "alloc.h"
#include <stddef.h>
#include <unistd.h>
#include <string.h>

/* See examples of what's happening in notes.md */
#define ALIGN(size) (((size) + 7) & ~7)

typedef struct block_t {
	size_t size;
	int free;
	struct block_t *next;
	struct block_t *prev;
} block_t;

static block_t *free_list = NULL;
static void *heap_start = NULL;

static size_t *getBoundaryTag(block_t *block) {
	return (size_t *)((char *)block + sizeof(block_t) + block->size) - 1;
}

static void setBoundaryTag(block_t *block) {
	*getBoundaryTag(block) = block->size;
}

static block_t *findFreeBlock(size_t block_size) {
	block_t *block;
	if(free_list == NULL) {
		return NULL;
	}
	block = free_list;

	while(!block->free || block->size < block_size) {
		if(block->next == NULL) {
			return NULL;
		}
		block = block->next;
	}

	return block;
}

static void removeFreeBlock(block_t *block) {
	block_t *temp_block;

	if(block->prev != NULL) {
		block->prev->next = block->next;
	} else {
		free_list = block->next;
	}
	temp_block = block->next;
	if(temp_block != NULL) {
		temp_block->prev = block->prev;
	}
}

static void coalesce(block_t *block) {
	block_t *next_block;
	block_t *prev_block;
	size_t prev_block_size;

	next_block = NULL;
	/* check that we aren't going passed the heap bounds */
	if(((char *)block + sizeof(block_t) + block->size) < (char *)sbrk(0)) {
		next_block = (block_t *)((char *)block + sizeof(block_t) + block->size);
	}
	prev_block = NULL;
	/* check that we aren't going before the heap bounds */
	if((char *)block > (char *)heap_start) {
		prev_block_size = *((size_t *)((char *)block - sizeof(size_t)));
		prev_block =
		    (block_t *)((char *)block - prev_block_size - sizeof(block_t));
	}

	if(next_block != NULL && next_block->free) {
		block->size += sizeof(block_t) + next_block->size;
		removeFreeBlock(next_block);
		setBoundaryTag(block);
	}

	if(prev_block != NULL && prev_block->free) {
		prev_block->size += sizeof(block_t) + block->size;
		removeFreeBlock(block);
		setBoundaryTag(prev_block);
	}
}

size_t myHeapSize(void) {
	return (char *)sbrk(0) - (char *)heap_start;
}

void *myMalloc(size_t size) {
	block_t *block;
	size_t block_size;

	if(size == 0) return NULL;

	if(heap_start == NULL) {
		heap_start = (void *)sbrk(0);
	}
	/*                        | Boundary tag size | */
	block_size = ALIGN(size) + sizeof(size_t);
	block = findFreeBlock(block_size);
	if(block == NULL) {
		/*                      | Header size |  */
		block = (block_t *)sbrk(sizeof(block_t) + block_size);
		if(block == (void *)-1) {
			return NULL;
		}
	} else {
		removeFreeBlock(block);
	}
	block->size = block_size;
	block->free = 0;
	block->next = NULL;
	block->prev = NULL;
	setBoundaryTag(block);

	return (char *)block + sizeof(block_t);
}

void myFree(void *ptr) {
	block_t *block;

	if (ptr == NULL) return;
	/* ptr points to the data, not the header, so we need to step
	 * back to the header */
	block = (block_t *)((char *)ptr - sizeof(block_t));
	block->free = 1;
	if(free_list != NULL) {
		free_list->prev = block;
	}
	block->next = free_list;
	block->prev = NULL;
	free_list = block;
	coalesce(block);
}

void *myRealloc(void *ptr, size_t size) {
	block_t *block;
	block_t *new_block;

	if(ptr == NULL) {
		return myMalloc(size);
	}

	if(size == 0) {
		myFree(ptr);
		return NULL;
	}
	block = (block_t *)((char *)ptr - sizeof(block_t));

	if(block->size - sizeof(size_t) >= ALIGN(size)) {
		return ptr;
	}

	new_block = myMalloc(size);
	if (new_block == NULL) return NULL;
	
	/*									| boundary tag size | */
	memcpy(new_block, ptr, block->size - sizeof(size_t));
	myFree(ptr);
	return new_block;
}
