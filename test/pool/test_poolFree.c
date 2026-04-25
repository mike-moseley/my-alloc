#include "alloc_error.h"
#include "unity.h"
#include "pool.h"

void setUp(void) {}
void tearDown(void) {}

void test_pool_null(void)
{
	pool_t *pool;
	AllocError err;
	void *ptr;
	
	err = poolCreate(2, 8, &pool);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = poolAlloc(pool, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	poolFree(NULL, ptr);
	printf("`poolFree(NULL, ptr)` did not crash.\n");
}

void test_chunk_null(void)
{
	pool_t *pool;
	AllocError err;
	void *ptr;
	
	err = poolCreate(2, 8, &pool);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = poolAlloc(pool, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	poolFree(pool, NULL);
	printf("`poolFree(pool, NULL)` did not crash.\n");
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_pool_null);
	RUN_TEST(test_chunk_null);
	return UNITY_END();
}
