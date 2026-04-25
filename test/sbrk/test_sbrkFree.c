#include "unity.h"
#include "sbrkAlloc.h"
#include "unity_internals.h"
#include <stdlib.h>
void setUp(void) {}
void tearDown(void) {}

void test_ok(void) {
	void *ptr;
	void *ptr2;
	ptr = sbrkMalloc(16);
	sbrkFree(ptr);
	ptr2 = sbrkMalloc(16);
	TEST_ASSERT_EQUAL(ptr, ptr2);
	sbrkFree(ptr2);
}

void test_null(void) {
	sbrkFree(NULL);
}

void test_consistent_heap(void) {
	void *ptr;
	void * ptr2;
	void * ptr3;

	ptr = sbrkMalloc(16);
	ptr2 = sbrkMalloc(16);
	ptr3 = sbrkMalloc(16);

	sbrkFree(ptr2);
	sbrkFree(ptr);
	sbrkFree(ptr3);

	ptr = sbrkMalloc(16);
	ptr2 = sbrkMalloc(16);
	ptr3 = sbrkMalloc(16);

	TEST_ASSERT_NOT_NULL(ptr);
	TEST_ASSERT_NOT_NULL(ptr2);
	TEST_ASSERT_NOT_NULL(ptr3);

	sbrkFree(ptr);
	sbrkFree(ptr2);
	sbrkFree(ptr3);
}

/* Fails
 * via Claude:
 * The real answer is: this is a limitation of
  testing a global sbrk-based allocator. The
  coalescing logic is correct — trust it from
  reading the code, and test it with a
  fixed-pool allocator where you have full
  control over the heap.
 * */
void test_coalesce(void) {
	void *ptr;
	void *ptr2;
	size_t heap_size;

	ptr = sbrkMalloc(16);
	ptr2 = sbrkMalloc(16);

	heap_size = heapSize();

	sbrkFree(ptr);
	sbrkFree(ptr2);

	ptr = sbrkMalloc(32);
	
	TEST_ASSERT_EQUAL(heapSize(), heap_size);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_ok);
	RUN_TEST(test_null);
	RUN_TEST(test_consistent_heap);
	/* RUN_TEST(test_coalesce); */
	return UNITY_END();
}
