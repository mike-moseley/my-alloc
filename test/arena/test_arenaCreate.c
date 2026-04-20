#include "unity.h"
#include "arena.h"

void setUp(void) {}
void tearDown(void) {}

void test_arena_ok(void)
{
	arena_t *arena;
	AllocError err;
	err = arenaCreate(16, &arena);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);
	TEST_ASSERT_NOT_NULL(arena);
}

void test_arena_null_out(void) {
	AllocError err;
	err = arenaCreate(16, NULL);
	TEST_ASSERT_EQUAL(ALLOC_ERROR_NULL, err);
}

void test_arena_zero_cap(void) {
	arena_t *arena;
	AllocError err;
	err = arenaCreate(0, &arena);
	TEST_ASSERT_EQUAL(ALLOC_ERROR_SIZE, err);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_arena_ok);
	RUN_TEST(test_arena_null_out);
	RUN_TEST(test_arena_zero_cap);
	return UNITY_END();
}
