#include "alloc_error.h"
#include "sys/mman.h"
#include "arena.h"
#include "alloc_common.h"

AllocError arenaCreate(size_t capacity, arena_t **out) {
	arena_t *arena;

	if(out == NULL) return ALLOC_ERROR_NULL;
	if(capacity == 0) return ALLOC_ERROR_SIZE;

	arena = (arena_t *)mmap(NULL, sizeof(arena_t) + capacity, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if(arena == MAP_FAILED) return ALLOC_ERROR_OOM;

	/* buf is the data in the arena, must offset by the size of the header*/
	arena->buf = (char *)arena + sizeof(arena_t);
	arena->offset = 0;
	arena->capacity = capacity;
	*out = arena;

	return ALLOC_OK;
}

AllocError arenaAlloc(arena_t *arena, size_t size, void **out) {
	
	return ALLOC_OK;
}
