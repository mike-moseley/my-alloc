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

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_myRealloc_returns_non_null);
	return UNITY_END();
}
