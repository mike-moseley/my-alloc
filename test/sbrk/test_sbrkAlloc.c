#include "unity.h"
#include "sbrkAlloc.h"
#include "unity_internals.h"
void setUp(void) {}
void tearDown(void) {}

void test_ok(void) {
	void *ptr;
	ptr = sbrkMalloc(16);
	TEST_ASSERT_NOT_NULL(ptr);
	sbrkFree(ptr);
}

void test_returns_null_0_size(void) {
	void *ptr;
	ptr = sbrkMalloc(0);
	TEST_ASSERT_NULL(ptr);
	sbrkFree(ptr);
}

void test_alignment(void) {
	void *ptr;
	ptr = sbrkMalloc(12);
	TEST_ASSERT_EQUAL(0, (size_t)ptr % 8);
}

void test_allocations_unique(void) {
	void *ptr;
	void *ptr2;
	void *ptr3;
	ptr = sbrkMalloc(8);
	ptr2 = sbrkMalloc(8);
	ptr3 = sbrkMalloc(8);
	TEST_ASSERT_NOT_EQUAL(ptr, ptr2);
	TEST_ASSERT_NOT_EQUAL(ptr, ptr3);
	TEST_ASSERT_NOT_EQUAL(ptr3, ptr2);
	sbrkFree(ptr);
	sbrkFree(ptr2);
	sbrkFree(ptr3);
}

void test_allocations_dont_corrupt_adjacent(void) {
	void *ptr;
	void *ptr2;
	void *ptr3;

	ptr = sbrkMalloc(8);
	ptr2 = sbrkMalloc(8);
	ptr3 = sbrkMalloc(8);

	*(long *)ptr = 42;
	*(long *)ptr3 = 42;
	*(long *)ptr2 = 1337;

	TEST_ASSERT_EQUAL(*(long *)ptr, *(long *)ptr3);
	TEST_ASSERT_NOT_EQUAL(*(long *)ptr, *(long *)ptr2);
	TEST_ASSERT_NOT_EQUAL(*(long *)ptr3, *(long *)ptr2);

	sbrkFree(ptr);
	sbrkFree(ptr2);
	sbrkFree(ptr3);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_ok);
	RUN_TEST(test_returns_null_0_size);
	RUN_TEST(test_alignment);
	RUN_TEST(test_allocations_unique);
	RUN_TEST(test_allocations_dont_corrupt_adjacent);
	return UNITY_END();
}
