#include "alloc_error.h"
#include "unity.h"
#include "mmapAlloc.h"

void setUp(void) {}
void tearDown(void) {}

void test_ok(void)
{
	void *ptr;
	AllocError err;
	err = mmapAlloc(8, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);
	*(int *)ptr = 4;
	TEST_ASSERT_EQUAL(*(int *)ptr, 4);
	mmapFree(ptr);
}

void test_zero_size(void) {
	void *ptr;
	AllocError err;
	err = mmapAlloc(0, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_ERROR_SIZE, err);
}

void test_null_ptr(void) {
	AllocError err;
	err = mmapAlloc(8, NULL);
	TEST_ASSERT_EQUAL(ALLOC_ERROR_NULL, err);
}

void test_rewrite(void) {
	void *ptr;
	AllocError err;
	err = mmapAlloc(8, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);
	*(int *)ptr = 4;
	TEST_ASSERT_EQUAL(*(int *)ptr, 4);
	mmapFree(ptr);
	err = mmapAlloc(8, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);
	*(int *)ptr = 4;
	printf("Rewrite did not crash\n");
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_ok);
	RUN_TEST(test_null_ptr);
	RUN_TEST(test_rewrite);
	RUN_TEST(test_zero_size);
	return UNITY_END();
}
