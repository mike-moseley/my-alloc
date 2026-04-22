#include "pool.h"

typedef struct pool_node_t pool_node_t;
struct pool_node_t {
	pool_node_t *next;
};

typedef struct chunk_t chunk_t;
struct chunk_t {
	chunk_t *next;
};
