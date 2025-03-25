#include "arena.h"
#include "unity.h"

#include <stdlib.h>

void test_init_release(void)
{
	size_t cap = 64 * (1 << 10); // 64 KB
	Arena a = arena_init(cap);

	TEST_ASSERT_MESSAGE(a != NULL, "Failed to create arena.");

	TEST_ASSERT_MESSAGE(arena_capacity(a) == cap,
			    "Expected arena capacity to be 64 KB.");

	TEST_ASSERT_MESSAGE(arena_size(a) == 0,
			    "Expected arena size to be 0 KB.");

	arena_release(&a);

	TEST_ASSERT_MESSAGE(
		a == NULL,
		"Arena should be set to NULL after calling arena_release.");
}

void test_alignment(void)
{
	size_t cap = 64 * (1 << 10); // 64 KB
	Arena a = arena_init(cap);

	char *p1 = arena_alloc(a, 12);
	char *p2 = arena_alloc(a, 18);
	ptrdiff_t delta = p2 - p1;

	TEST_ASSERT_MESSAGE(
		delta == 16,
		"Expected alignment to lead to a difference of 16 bytes.");

	char *p3 = arena_alloc(a, 33);
	delta = p3 - p2;

	TEST_ASSERT_MESSAGE(
		delta == 32,
		"Expected alignment to lead to a difference of 32 bytes.");

	char *p4 = arena_alloc(a, 1);
	delta = p4 - p3;

	TEST_ASSERT_MESSAGE(
		delta == 48,
		"Expected alignment to lead to a difference of 48 bytes.");

	arena_release(&a);
}

void test_pop(void)
{
	size_t cap = 64 * (1 << 10); // 64 KB
	Arena a = arena_init(cap);

	arena_alloc(a, 64);
	TEST_ASSERT_MESSAGE(arena_size(a) == 64,
			    "Expected arena size of 64 bytes.");

	arena_pop(a, 15);
	TEST_ASSERT_MESSAGE(arena_size(a) == 48,
			    "Expected arena size of 48 bytes.");

	arena_pop(a, 33);
	TEST_ASSERT_MESSAGE(arena_size(a) == 0,
			    "Expected arena size of 0 bytes.");

	// test superfluous pop
	arena_pop(a, 55);
	TEST_ASSERT_MESSAGE(arena_size(a) == 0,
			    "Expected arena size of 0 bytes.");

	arena_release(&a);
}

void test_clear(void)
{
	size_t cap = 64 * (1 << 10); // 64 KB
	Arena a = arena_init(cap);

	TEST_ASSERT_MESSAGE(arena_size(a) == 0,
			    "Expected arena size of 0 bytes.");

	(void)arena_alloc(a, 256);
	TEST_ASSERT_MESSAGE(arena_size(a) == 256,
			    "Expected arena size of 256 bytes.");

	arena_clear(a);
	TEST_ASSERT_MESSAGE(arena_size(a) == 0,
			    "Expected arena size of 0 bytes.");

	arena_release(&a);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_init_release);
	RUN_TEST(test_alignment);
	RUN_TEST(test_pop);
	RUN_TEST(test_clear);
	return UNITY_END();
}
