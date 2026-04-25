#include "unity.h"
#include "pool.h"

void setUp(void) {}
void tearDown(void) {}

void test_null(void)
{
	poolDestroy(NULL);
	printf("`poolDestroy(pool, NULL)` did not crash.\n");
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_null);
	return UNITY_END();
}
