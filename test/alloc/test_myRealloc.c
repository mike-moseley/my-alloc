#include "unity.h"
#include "alloc.h"
#include "unity_internals.h"
void setUp(void) {}
void tearDown(void) {}

void test_myRealloc_returns_non_null(void) {
	void *ptr;
	ptr = myMalloc(16);
	ptr = myRealloc(ptr, 16);
	TEST_ASSERT_NOT_NULL(ptr);
	myFree(ptr);
}

void test_returns_null_0_size(void) {
	void *ptr;
	ptr = myMalloc(16);
	ptr = myRealloc(ptr, 0);

	TEST_ASSERT_NULL(ptr);
}

void test_shrink_same_ptr(void) {
	void *ptr;
	char *ptr_addr_pre_shrink;
	ptr = myMalloc(16);
	ptr_addr_pre_shrink = ptr;
	ptr = myRealloc(ptr, 8);
	TEST_ASSERT_EQUAL_PTR(ptr_addr_pre_shrink, ptr);
}

void test_grow_preserves_data(void) {
	void *ptr;
	long *ptr_data_pre_grow;
	ptr = myMalloc(8);
	*(long *)ptr = 1337;
	ptr_data_pre_grow = (long *)ptr;
	ptr = myRealloc(ptr, 16);
	TEST_ASSERT_EQUAL(*ptr_data_pre_grow, *(long *)ptr);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_myRealloc_returns_non_null);
	RUN_TEST(test_returns_null_0_size);
	RUN_TEST(test_shrink_same_ptr);
	RUN_TEST(test_grow_preserves_data);
	return UNITY_END();
}
