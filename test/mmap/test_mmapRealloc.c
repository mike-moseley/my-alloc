#include "alloc_error.h"
#include "mmapAlloc.h"
#include "unity.h"
#include "unity_internals.h"

void setUp(void) {}
void tearDown(void) {}

void test_null_ptr(void)
{
	void *new_ptr;
	AllocError err;
	err = mmapRealloc(NULL, 8, &new_ptr);
	TEST_ASSERT_EQUAL(ALLOC_ERROR_NULL, err);
}
void test_null_out(void) {
	void *ptr;
	AllocError err;
	err = mmapAlloc(4, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);
	err = mmapRealloc(ptr, 8, NULL);
	TEST_ASSERT_EQUAL(ALLOC_ERROR_NULL, err);
}

void test_smaller_size_same_ptr(void) {
	void *ptr;
	void *new_ptr;
	void *old_ptr;
	AllocError err;

	err = mmapAlloc(12, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	*(int *)ptr = 4;

	old_ptr = ptr;
	err = mmapRealloc(ptr, 8, &new_ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);
	TEST_ASSERT_EQUAL(old_ptr, new_ptr);
	TEST_ASSERT_EQUAL(4, *(int *)new_ptr);
}

void test_same_size_same_ptr(void) {
	void *ptr;
	void *new_ptr;
	void *old_ptr;
	AllocError err;

	err = mmapAlloc(8, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	*(int *)ptr = 4;

	old_ptr = ptr;
	err = mmapRealloc(ptr, 8, &new_ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);
	TEST_ASSERT_EQUAL(old_ptr, new_ptr);
	TEST_ASSERT_EQUAL(4, *(int *)new_ptr);
}

void test_larger_size_new_ptr(void) {
	void *ptr;
	void *new_ptr;
	void *old_ptr;
	AllocError err;

	err = mmapAlloc(8, &ptr);
	TEST_ASSERT_EQUAL(ALLOC_OK, err);

	*(int *)ptr = 4;

	old_ptr = ptr;
	err = mmapRealloc(ptr, 12, &new_ptr);
	TEST_ASSERT_NOT_EQUAL(old_ptr, new_ptr);
	TEST_ASSERT_EQUAL(4, *(int *)new_ptr);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_null_ptr);
	RUN_TEST(test_null_out);
	RUN_TEST(test_larger_size_new_ptr);
	RUN_TEST(test_same_size_same_ptr);
	RUN_TEST(test_smaller_size_same_ptr);
	return UNITY_END();
}
