#include "unity.h"
#include "pool.h"
#include "alloc_common.h"
#include "unity_internals.h"

void setUp(void) {}
void tearDown(void) {}

void test_ok(void)
{
	pool_t *pool;
	AllocError err;
	err = poolCreate(8, 4, &pool);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);
	TEST_ASSERT_NOT_NULL(pool);
}

void test_null_out(void) {
	AllocError err;
	err = poolCreate(8, 4, NULL);
	TEST_ASSERT_EQUAL(ALLOC_ERROR_NULL, err);

}

void test_size_zero(void) {
	pool_t *pool;
	AllocError err;
	err = poolCreate(8, 0, &pool);
	TEST_ASSERT_EQUAL(ALLOC_ERROR_SIZE, err);
}

void test_count_zero(void) {
	pool_t *pool;
	AllocError err;
	err = poolCreate(0, 4, &pool);
	TEST_ASSERT_EQUAL(ALLOC_ERROR_SIZE, err);
}

void test_chunk_alignment(void) {
	pool_t *pool;
	AllocError err;
	err = poolCreate(8, 4, &pool);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);
	TEST_ASSERT_EQUAL(ALIGN(4), pool->chunk_size);
}

void test_size(void) {
	pool_t *pool;
	AllocError err;
	err = poolCreate(9, 4, &pool);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);
	TEST_ASSERT_EQUAL(9, pool->count);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_ok);
	RUN_TEST(test_null_out);
	RUN_TEST(test_size_zero);
	RUN_TEST(test_count_zero);
	RUN_TEST(test_chunk_alignment);
	RUN_TEST(test_size);
	return UNITY_END();
}
