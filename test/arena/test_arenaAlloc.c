#include "alloc_error.h"
#include "unity.h"
#include "arena.h"
#include "unity_internals.h"

void setUp(void) {}
void tearDown(void) {}

void test_alloc_ok(void) {
	arena_t *arena;
	AllocError err;
	void *ptr;

	err = arenaCreate(16, &arena);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = arenaAlloc(arena, 4, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);
	arenaDestroy(arena);
}

void test_arena_null(void) {
	AllocError err;
	void *ptr;
	err = arenaAlloc(NULL, 16, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_ERROR_NULL, err);
}

void test_out_null(void) {
	arena_t *arena;
	AllocError err;

	err = arenaCreate(16, &arena);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = arenaAlloc(arena, 4, NULL);
	TEST_ASSERT_EQUAL(ALLOC_ERROR_NULL, err);

	arenaDestroy(arena);
}

void test_size_zero(void) {
	arena_t *arena;
	AllocError err;
	void *ptr;

	err = arenaCreate(16, &arena);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = arenaAlloc(arena, 0, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_ERROR_SIZE, err);

	arenaDestroy(arena);
}

void test_size_exceeds(void) {
	arena_t *arena;
	AllocError err;
	void *ptr;

	err = arenaCreate(16, &arena);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = arenaAlloc(arena, 32, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_ERROR_SIZE, err);

	arenaDestroy(arena);
}

void test_ptr_alignment(void) {
	arena_t *arena;
	AllocError err;
	void *ptr;

	err = arenaCreate(16, &arena);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = arenaAlloc(arena, 6, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	TEST_ASSERT_EQUAL(0, (size_t)ptr % 8);
	TEST_ASSERT_EQUAL(arena->offset, 8);

	arenaDestroy(arena);
}

void test_multiple_ptrs_unique(void) {
	arena_t *arena;
	AllocError err;
	void *ptr;
	void *ptr2;
	void *ptr3;
	void *ptr4;
	
	err = arenaCreate(32, &arena);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = arenaAlloc(arena, 2, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = arenaAlloc(arena, 2, &ptr2);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = arenaAlloc(arena, 2, &ptr3);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = arenaAlloc(arena, 2, &ptr4);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	TEST_ASSERT_NOT_EQUAL(ptr, ptr2);
	TEST_ASSERT_NOT_EQUAL(ptr, ptr3);
	TEST_ASSERT_NOT_EQUAL(ptr, ptr4);

	TEST_ASSERT_NOT_EQUAL(ptr2, ptr3);
	TEST_ASSERT_NOT_EQUAL(ptr2, ptr4);

	TEST_ASSERT_NOT_EQUAL(ptr3, ptr4);

	arenaDestroy(arena);
}

void test_alloc_not_corrupt_neighbors(void) {
	arena_t *arena;
	AllocError err;
	void *ptr;
	void *ptr2;
	void *ptr3;

	err = arenaCreate(24, &arena);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = arenaAlloc(arena, 8, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = arenaAlloc(arena, 8, &ptr2);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = arenaAlloc(arena, 8, &ptr3);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	*(int *)ptr = 100;
	*(int *)ptr2 = 200;
	*(int *)ptr3 = 300;

	TEST_ASSERT_EQUAL(*(int *)ptr, 100);
	TEST_ASSERT_EQUAL(*(int *)ptr2, 200);
	TEST_ASSERT_EQUAL(*(int *)ptr3, 300);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_alloc_ok);
	RUN_TEST(test_arena_null);
	RUN_TEST(test_out_null);
	RUN_TEST(test_size_zero);
	RUN_TEST(test_size_exceeds);
	RUN_TEST(test_ptr_alignment);
	RUN_TEST(test_multiple_ptrs_unique);
	RUN_TEST(test_alloc_not_corrupt_neighbors);
	return UNITY_END();
}
