#include "alloc_error.h"
#include "mmapAlloc.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_null_no_crash(void)
{
	AllocError err;
	err = mmapFree(NULL);
	printf("`mmapFree(NULL)` did not crash: %d\n", err);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_null_no_crash);
	return UNITY_END();
}
