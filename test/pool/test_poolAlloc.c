#include "alloc_error.h"
#include "unity.h"
#include "pool.h"
#include "unity_internals.h"

void setUp(void) {}
void tearDown(void) {}

void test_ok(void)
{
	pool_t *pool;
	AllocError err;
	void *ptr;

	err = poolCreate(8, 4, &pool);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = poolAlloc(pool, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	poolDestroy(pool);
}

void test_pool_null(void){
	AllocError err;
	void *ptr;

	err = poolAlloc(NULL, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_ERROR_NULL, err);
}

void test_out_null(void){
	pool_t *pool;
	AllocError err;
	
	err = poolCreate(8, 4, &pool);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = poolAlloc(pool, NULL);
	TEST_ASSERT_EQUAL(ALLOC_ERROR_NULL, err);
}

void test_multiple_ptrs_unique(void) {
	pool_t *pool;
	AllocError err;
	void *ptr;
	void *ptr2;
	void *ptr3;

	err = poolCreate(12, 4, &pool);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = poolAlloc(pool, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);
	err = poolAlloc(pool, &ptr2);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);
	err = poolAlloc(pool, &ptr3);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	TEST_ASSERT_NOT_EQUAL(ptr, ptr2);
	TEST_ASSERT_NOT_EQUAL(ptr, ptr3);
	TEST_ASSERT_NOT_EQUAL(ptr2, ptr3);
}

void test_out_of_bounds(void) {
	pool_t *pool;
	AllocError err;
	void *ptr;
	void *ptr2;
	void *ptr3;

	err = poolCreate(2, 8, &pool);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = poolAlloc(pool, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);
	err = poolAlloc(pool, &ptr2);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);
	err = poolAlloc(pool, &ptr3);
	TEST_ASSERT_EQUAL(ALLOC_ERROR_OOM, err);
}

void test_same_chunk_after_free(void) {
	pool_t *pool;
	AllocError err;
	void *ptr;
	void *ptr_pos;

	err = poolCreate(2, 8, &pool);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	err = poolAlloc(pool, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	ptr_pos = ptr;

	poolFree(pool, ptr);

	err = poolAlloc(pool, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	TEST_ASSERT_EQUAL(ptr_pos, ptr);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_ok);
	RUN_TEST(test_out_null);
	RUN_TEST(test_pool_null);
	RUN_TEST(test_out_of_bounds);
	RUN_TEST(test_multiple_ptrs_unique);
	RUN_TEST(test_same_chunk_after_free);
	return UNITY_END();
}
