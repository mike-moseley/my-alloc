#include "unity.h"
#include "alloc.h"
#include "unity_internals.h"
#include <stdlib.h>
void setUp(void) {}
void tearDown(void) {}

void test_myFree(void) {
	void *ptr;
	void *ptr2;
	ptr = myMalloc(16);
	myFree(ptr);
	ptr2 = myMalloc(16);
	TEST_ASSERT_EQUAL(ptr, ptr2);
	myFree(ptr2);
}

void test_myFree_null(void) {
	myFree(NULL);
}

void test_consistent_heap(void) {
	void *ptr;
	void * ptr2;
	void * ptr3;

	ptr = myMalloc(16);
	ptr2 = myMalloc(16);
	ptr3 = myMalloc(16);

	myFree(ptr2);
	myFree(ptr);
	myFree(ptr3);

	ptr = myMalloc(16);
	ptr2 = myMalloc(16);
	ptr3 = myMalloc(16);

	TEST_ASSERT_NOT_NULL(ptr);
	TEST_ASSERT_NOT_NULL(ptr2);
	TEST_ASSERT_NOT_NULL(ptr3);

	myFree(ptr);
	myFree(ptr2);
	myFree(ptr3);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_myFree);
	RUN_TEST(test_myFree_null);
	RUN_TEST(test_consistent_heap);
	return UNITY_END();
}
