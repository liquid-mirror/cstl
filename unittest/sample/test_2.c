#include "test.h"


void test_2_1(void)
{
	ASSERT(1 == 1);
	ASSERT_FATAL(1 == 1);
	ASSERT(2 == 2);
}

void test_2_2(void)
{
	ASSERT_TRUE(1);
	ASSERT(1 == 1);
	ASSERT(1 == 1);
}

void test_2_3(void)
{
	ASSERT(0 == 0);
	ASSERT(1 == 1);
	ASSERT(0 == 0);
	ASSERT(1 == 1);
}

TestCase test_2_cases[] = {
	{ "test_2_1", test_2_1 },
	{ "test_2_2", test_2_2 },
	{ "test_2_3", test_2_3 },
	TEST_CASE_NULL,
};

