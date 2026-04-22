#include "unity.h"
#include "arena.h"
#include "alloc_error.h"

void setUp(void) {}
void tearDown(void) {}

/* ??? */
void test_arenaDestroy_ok(void)
{
	arena_t *arena;
	AllocError err;
	void *ptr;

	err = arenaCreate(16, &arena);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);
	
	err = arenaAlloc(arena, 8, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	arenaDestroy(arena);
}

void test_arenaDestroy_null(void) {
	arenaDestroy(NULL);
	printf("`arenaDestroy(NULL)` did not crash.\n");
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_arenaDestroy_null);
	return UNITY_END();
}
