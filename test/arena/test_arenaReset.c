#include "alloc_error.h"
#include "arena.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_reset_same_ptr(void)
{
	arena_t *arena;
	AllocError err;
	void *addr;
	void *ptr;

	err = arenaCreate(16, &arena);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = arenaAlloc(arena, 8, &addr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	arenaReset(arena);

	err = arenaAlloc(arena, 16, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	TEST_ASSERT_EQUAL(addr, ptr);
}

void test_null_err(void) {
	arenaReset(NULL);
	printf("`arenaReset(NULL)` did not crash.\n");
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_reset_same_ptr);
	RUN_TEST(test_null_err);
	return UNITY_END();
}
