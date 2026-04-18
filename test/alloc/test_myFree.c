#include "unity.h"
#include "alloc.h"
#include "unity_internals.h"
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

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_myFree);
	RUN_TEST(test_myFree_null);
	return UNITY_END();
}
