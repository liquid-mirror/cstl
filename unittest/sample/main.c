#include "../UnitTest.h"
#include "test.h"

TestSuite suites[] = {
	{ "test_1", test_1_setup, test_1_teardown, test_1_cases },
	{ "test_2", 0, 0, test_2_cases },
	TEST_SUITE_NULL,
};


int main(int argc, char **argv)
{
	if (argc >= 2) {
		unittest_run_interactive(suites);
	} else {
		unittest_run_all(suites);
	}
	return 0;
}

