# my-alloc

Outline made with Claude.
A memory allocator implementing four allocator designs in C90:

- **Free-list allocator** — general-purpose `malloc`/`free`/`realloc` over `sbrk`
- **Arena allocator** — bump-pointer allocator for grouped, same-lifetime allocations
- **Pool allocator** — fixed-size block allocator for many objects of the same type
- **mmap allocator** — free-list allocator backed by `mmap`/`munmap` instead of `sbrk`

---

## Free-List Allocator

### Memory Layout

Each allocation is preceded by a header embedded directly in the heap:

```
+------------------+----------------------------+------------------+
|  block_t header  |     user payload           |  boundary tag    |
|  (size, free,    |     (size bytes)            |  (copy of size)  |
|   next, prev)    |                             |                  |
+------------------+----------------------------+------------------+
^                  ^
block pointer      pointer returned to caller
```

`my_malloc` returns `header + 1` — the address immediately after the header.
`my_free` recovers the header via `(block_t *)ptr - 1`.

Boundary tags (a copy of `size` stored at the end of each block) allow `my_free`
to find the preceding block in memory without a separate backward pointer,
enabling O(1) coalescing in both directions.

### Free List

Free blocks are threaded into an explicit doubly-linked list. The list is
unsorted (insertion order). Allocated blocks are removed from the list;
freed blocks are inserted at the head.

### Allocation: First-Fit

`my_malloc` walks the free list from the head and returns the first block whose
size is sufficient. If no block fits, the heap is extended via `sbrk`.

After selecting a block, it is **split** if the remainder is large enough to
hold a header plus a minimum payload — preventing the free list from filling
with unusably small fragments.

### Freeing: Immediate Coalescing

`my_free` merges adjacent free blocks immediately on every call:

1. Check the block after this one in memory (using `size` to find it) — if
   free, merge.
2. Check the block before this one in memory (using the boundary tag) — if
   free, merge.

This keeps fragmentation low at the cost of slightly more work per `my_free`.

### Alignment

All sizes are rounded up to a multiple of `ALIGNMENT` (16 bytes) before
allocation. `sizeof(block_t)` is also a multiple of `ALIGNMENT` so that the
user pointer is always suitably aligned for any type.

### API

```c
void *my_malloc(size_t size);
void  my_free(void *ptr);
void *my_realloc(void *ptr, size_t size);
```

---

## Arena Allocator

An arena (also called a region or zone) allocates from a single contiguous
block of memory using a bump pointer. Individual frees are a no-op — the
entire arena is released at once. This makes it extremely fast and
fragmentation-free for allocations that share a lifetime.

### Memory Layout

```
+----------------+--------+--------+--------+-----+----------+
|  arena_t       | alloc1 | alloc2 | alloc3 | ... | (unused) |
|  (buf, offset, |        |        |        |     |          |
|   capacity)    |        |        |        |     |          |
+----------------+--------+--------+--------+-----+----------+
                 ^                                ^           ^
                 buf                           buf+offset  buf+capacity
```

The arena header can live on the stack or be `mmap`'d alongside its buffer.
`offset` advances on each allocation; freeing the arena resets it to zero.

### Operations

- **`arena_alloc(arena, size)`** — bump `offset` forward by the aligned size;
  return `buf + old_offset`. Fails if capacity is exceeded.
- **`arena_reset(arena)`** — set `offset = 0`; all memory is logically freed
  and available for reuse. No system call needed.
- **`arena_destroy(arena)`** — release the backing buffer (`munmap` or `free`
  depending on how it was created).

No splitting, no coalescing, no free list — just pointer arithmetic.

### When to use an arena

| Situation | Good fit? |
|-----------|-----------|
| Allocations with a shared, bounded lifetime | Yes |
| Per-request or per-frame scratch space | Yes |
| Objects that are freed individually at unpredictable times | No |
| Long-lived objects of varying sizes | No |

In the shell project, an arena per-command makes sense: allocate tokens,
argv, redirections — then reset the arena after `waitpid` returns.

### API

```c
arena_t *arena_create(size_t capacity);
void    *arena_alloc(arena_t *arena, size_t size);
void     arena_reset(arena_t *arena);
void     arena_destroy(arena_t *arena);
```

---

## Pool Allocator

A pool allocator manages a fixed number of fixed-size blocks carved from a
single buffer. Because every block is the same size, there are no headers,
no splitting, and no coalescing — just a free list of uniform slots.

### Memory Layout

```
+-------------+-------+-------+-------+-------+-------+
|  pool_t     | slot0 | slot1 | slot2 | slot3 | slot4 |
|  (buf,      |       |  -->  |       | NULL  |       |
|   free,     |       | slot3 |       |       |       |
|   capacity) |       |       |       |       |       |
+-------------+-------+-------+-------+-------+-------+
               ^
               free (points to slot1, which points to slot3 — the free list)
```

Free slots are threaded into a singly-linked list using the slot memory
itself: each free slot holds a pointer to the next free slot. No separate
metadata needed. `pool_alloc` pops the head; `pool_free` pushes back.

### Operations

- **`pool_create(block_size, count)`** — `mmap` a buffer of `block_size * count`
  bytes, thread all slots into the free list.
- **`pool_alloc(pool)`** — pop the head of the free list; return that slot.
  O(1), no search.
- **`pool_free(pool, ptr)`** — push the slot back onto the free list. O(1).
- **`pool_destroy(pool)`** — release the backing buffer.

`block_size` must be at least `sizeof(void *)` so each free slot can hold
the next pointer.

### When to use a pool

| Situation | Good fit? |
|-----------|-----------|
| Many objects of identical size and type | Yes |
| Objects allocated and freed individually | Yes |
| Variable-size allocations | No |
| Objects with very different lifetimes | Possible, but no benefit over free-list |

In the shell project, a pool per object type makes sense: one pool for
tokens, one for AST nodes, one for job structs.

### API

```c
pool_t *pool_create(size_t block_size, size_t count);
void   *pool_alloc(pool_t *pool);
void    pool_free(pool_t *pool, void *ptr);
void    pool_destroy(pool_t *pool);
```

---

## Build & Run

Configure (needed after adding/removing files or editing `CMakeLists.txt`):

```bash
./update_cmake.sh
```

Build and run:

```bash
cmake --build cmake
./bin/app

# or
./build_run.sh
```

Debug build with gdb:

```bash
./gdb_build_run.sh
```

Run all tests:

```bash
./tests_build_run.sh
```

## Project Structure

```
.
├── src/
│   ├── main.c          # Driver / manual tests
│   ├── alloc.c         # Free-list allocator implementation
│   ├── arena.c         # Arena allocator implementation
│   └── pool.c          # Pool allocator implementation
├── include/
│   ├── alloc.h         # Free-list API and block_t definition
│   ├── arena.h         # Arena API and arena_t definition
│   └── pool.h          # Pool API and pool_t definition
├── test/
│   ├── unity/          # Unity test framework
│   ├── test_alloc.c    # Free-list unit tests
│   ├── test_arena.c    # Arena unit tests
│   └── test_pool.c     # Pool unit tests
├── bin/                # Compiled binaries (git-ignored)
│   └── test/
└── cmake/              # CMake build directory (git-ignored)
```

## References

- CS:APP 3e — Chapter 9.9 (dynamic memory allocation, explicit free lists, boundary tags)
- K&R C — Chapter 8.7 (minimal storage allocator)
- `man 2 sbrk`, `man 2 mmap`
