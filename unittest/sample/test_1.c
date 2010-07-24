#include "test.h"

int test_1_setup(void)
{
	return 0;
}

int test_1_teardown(void)
{
	return 0;
}


void test_1_1(void)
{
	ASSERT_EQUAL(1, 0);
	ASSERT_NOT_EQUAL(1, 1);
	ASSERT_EQUAL_FATAL(0, 1);
	ASSERT(2 == 1);
}

void test_1_2(void)
{
	ASSERT(0 == 3);
	ASSERT(1 == 1);
	ASSERT_STRING_NOT_EQUAL_FATAL("hoge", "hoge");
}

void test_1_3(void)
{
	ASSERT_FAIL("hoge");
	ASSERT(0 == 0);
	ASSERT(1 == 1);
	ASSERT(0 == 0);
	ASSERT(1 == 1);
}

TestCase test_1_cases[] = {
	{ "test_1_1", test_1_1 },
	{ "test_1_2", test_1_2 },
	{ "test_1_3", test_1_3 },
	TEST_CASE_NULL,
};
