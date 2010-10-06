#include <string.h>
#include <stdlib.h>
#include "UnitTest.h"
#include "TestSuite.h"
#include "TestCase.h"
#include "LibcImpl.h"

static char input_buf[64];
static jmp_buf quit_jmp;

static size_t suites_total, suites_ran, suites_failed;
static size_t tests_total, tests_ran, tests_failed;
static size_t asserts_total, asserts_ran, asserts_failed;
static size_t setup_failed, teardown_failed;
static TestResult result;

static void clear_values(void)
{
	suites_total = suites_ran = suites_failed = 0;
	tests_total = tests_ran = tests_failed = 0;
	asserts_total = asserts_ran = asserts_failed = 0;
	setup_failed = teardown_failed = 0;
	memset(&result, 0, sizeof(result));
}

static void init(TestSuite *suites)
{
	size_t i;
	clear_values();
	for (i = 0; suites[i].name != 0; i++) {
		size_t ncases;
		TestSuiteResult suite_result;
		ncases = TestSuite_init(&suites[i]);
		TestSuite_get_result(&suites[i], &suite_result);
		tests_total += ncases;
		result.num_suites++;
		result.suite_result.num_tests += suite_result.num_tests;
	}
	suites_total += i;
}

static void cleanup(TestSuite *suites)
{
	size_t i;
	clear_values();
	for (i = 0; suites[i].name != 0; i++) {
		size_t ncases;
		TestSuiteResult suite_result;
		ncases = TestSuite_cleanup(&suites[i]);
		TestSuite_get_result(&suites[i], &suite_result);
		tests_total += ncases;
		result.num_suites++;
		result.suite_result.num_tests += suite_result.num_tests;
	}
	suites_total += i;
}

static void print_result(void)
{
	PRINTF0("Type       Total      Ran   Passed   Failed\n");
	PRINTF3("suites  %8d %8d      n/a %8d", suites_total, suites_ran, suites_failed);
	if (setup_failed || teardown_failed) {
		PRINTF2("(setup:%d/teardown:%d)\n", setup_failed, teardown_failed);
	} else {
		PRINTF0("\n");
	}
	PRINTF4("tests   %8d %8d %8d %8d\n", tests_total, tests_ran, tests_ran - tests_failed, tests_failed);
	PRINTF4("asserts %8d %8d %8d %8d\n", asserts_total, asserts_ran, asserts_ran - asserts_failed, asserts_failed);
	PRINTF0("\n");


	PRINTF0("Result\n");
	PRINTF0("Type       Total      Ran   Passed   Failed    Error\n");
	PRINTF3("suites  %8d %8d      n/a %8d %8d\n", 
			result.num_suites, result.num_suites_ran, result.num_suites_failed, 
			result.suite_result.num_errors_setup + result.suite_result.num_errors_teardown);
	PRINTF4("tests   %8d %8d %8d %8d %8d\n", 
			result.suite_result.num_tests, 
			result.suite_result.num_tests_ran, 
			result.suite_result.num_tests_ran - result.suite_result.num_tests_failed, 
			result.suite_result.num_tests_failed, 
			result.suite_result.case_result.num_errors_setup + result.suite_result.case_result.num_errors_teardown);
	PRINTF4("asserts %8d %8d %8d %8d        -\n", 
			result.suite_result.case_result.num_asserts, 
			result.suite_result.case_result.num_asserts_ran, 
			result.suite_result.case_result.num_asserts_ran - result.suite_result.case_result.num_asserts_failed, 
			result.suite_result.case_result.num_asserts_failed);
	PRINTF0("\n");

	if (result.suite_result.num_errors_setup + result.suite_result.num_errors_teardown > 0) {
	}
	if (result.num_suites_failed) {
	}
}

static void get_result(TestSuite *suites)
{
	size_t i;
	size_t j;
	for (i = 0; suites[i].name != 0; i++) {
		TestSuite *suite = &suites[i];
		PRINTF1("Suite: %s\n", suite->name);
		if (suite->setup_error) {
		}
		if (suite->teardown_error) {
		}
		for (j = 0; suite->test_cases[j].name != 0; j++) {
			TestCase *tc = &suite->test_cases[j];
			if (tc->setup_error || tc->teardown_error) {
				PRINTF0("E");
			} else if (tc->result.num_asserts_failed == 0) {
				PRINTF0(".");
			} else {
				PRINTF0("F");
			}
		}
		PRINTF0("\n");
		if (suite->result.num_tests_failed == 0) {
			PRINTF1("OK (%d Tests)\n", suite->result.num_tests_ran);
		} else {
			PRINTF2("NG (%d Tests, %d Failures)\n", suite->result.num_tests_ran, suite->result.num_tests_failed);
			for (j = 0; suite->test_cases[j].name != 0; j++) {
				TestCase *tc = &suite->test_cases[j];
				TestAssertion *pos;
				TestAssertion *list = &tc->assertion_list;
				int name_printed = 0;
				for (pos = LIST_BEGIN(list); pos != LIST_END(list); pos = pos->next) {
					if (!pos->passed_flag) {
						if (!name_printed) {
							PRINTF1("\nTest: %s\n", tc->name);
							name_printed = 1;
						}
						PRINTF3("  %s(%d) %s\n", pos->file, pos->line, pos->expr);
					}
				}
			}
		}
		PRINTF0("\n");

/*            PRINTF1("Test: %s\n", tc->name);*/
/*            for (pos = LIST_BEGIN(list); pos != LIST_END(list); pos = pos->next) {*/
/*                if (!pos->passed_flag) {*/
/*                    PRINTF3("  %s(%d) %s\n", pos->file, pos->line, pos->expr);*/
/*                }*/
/*            }*/
/*            PRINTF0("\n");*/
/*            if (tc->result.num_asserts_failed == 0) {*/
/*                PRINTF1("OK (%d Tests)\n", tc->result.num_asserts_ran);*/
/*            } else {*/
/*                PRINTF2("NG (%d Failures / %d Tests)\n", tc->result.num_asserts_failed, tc->result.num_asserts_ran);*/
/*                for (pos = LIST_BEGIN(list); pos != LIST_END(list); pos = pos->next) {*/
/*                    if (!pos->passed_flag) {*/
/*                        PRINTF3("  %s(%d) %s\n", pos->file, pos->line, pos->expr);*/
/*                    }*/
/*                }*/
/*            }*/
/*            PRINTF0("\n");*/
	}
}

static void add_result(const TestSuiteResult *s_result)
{
	result.suite_result.num_tests_ran       += s_result->num_tests_ran;
	result.suite_result.num_tests_failed    += s_result->num_tests_failed;
	result.suite_result.num_errors_setup    += s_result->num_errors_setup;
	result.suite_result.num_errors_teardown += s_result->num_errors_teardown;
	result.suite_result.case_result.num_asserts         += s_result->case_result.num_asserts;
	result.suite_result.case_result.num_asserts_ran     += s_result->case_result.num_asserts_ran;
	result.suite_result.case_result.num_asserts_failed  += s_result->case_result.num_asserts_failed;
	result.suite_result.case_result.num_errors_setup    += s_result->case_result.num_errors_setup;
	result.suite_result.case_result.num_errors_teardown += s_result->case_result.num_errors_teardown;
	result.num_suites_ran++;
	if (s_result->num_tests_failed > 0) {
		result.num_suites_failed++;
	}
}

static void run_all(TestSuite *suites)
{
	size_t i;
	for (i = 0; suites[i].name != 0; i++) {
		size_t nc, ncf, na, naf;
		enum TestSuiteErr ret;
		TestSuiteResult suite_result;
		ret = TestSuite_test(&suites[i], &nc, &ncf, &na, &naf);
		TestSuite_get_result(&suites[i], &suite_result);
		add_result(&suite_result);

		asserts_total += na;
		asserts_ran += na;
		asserts_failed += naf;
		tests_ran += nc;
		tests_failed += ncf;
		if (ncf > 0 || ret != SUITE_OK) {
			suites_failed++;
			if (ret == SETUP_NG) {
				setup_failed++;
			} else if (ret == TEARDOWN_NG) {
				teardown_failed++;
			}
		}
	}
	suites_ran = i;
	print_result();
	get_result(suites);
	cleanup(suites);
}

static void run_suite_selected(TestSuite *suites, int suite_idx, int case_idx)
{
	size_t nc, ncf, na, naf;
	enum TestSuiteErr ret;
	TestSuiteResult suite_result;
	ret = TestSuite_test_selected(&suites[suite_idx], case_idx, &nc, &ncf, &na, &naf);
	TestSuite_get_result(&suites[suite_idx], &suite_result);
	add_result(&suite_result);

	asserts_total = na;
	asserts_ran = na;
	asserts_failed = naf;
	tests_ran = nc;
	tests_failed = ncf;
	if (ncf > 0 || ret != SUITE_OK) {
		suites_failed = 1;
		if (ret == SETUP_NG) {
			setup_failed++;
		} else if (ret == TEARDOWN_NG) {
			teardown_failed++;
		}
	}
	suites_ran = 1;
	print_result();
	get_result(suites);
	cleanup(suites);
}

static void run_suite(TestSuite *suites, int suite_idx)
{
	size_t nc, ncf, na, naf;
	enum TestSuiteErr ret;
	TestSuiteResult suite_result;
	ret = TestSuite_test(&suites[suite_idx], &nc, &ncf, &na, &naf);
	TestSuite_get_result(&suites[suite_idx], &suite_result);
	add_result(&suite_result);

	asserts_total = na;
	asserts_ran = na;
	asserts_failed = naf;
	tests_ran = nc;
	tests_failed = ncf;
	if (ncf > 0 || ret != SUITE_OK) {
		suites_failed = 1;
		if (ret == SETUP_NG) {
			setup_failed++;
		} else if (ret == TEARDOWN_NG) {
			teardown_failed++;
		}
	}
	suites_ran = 1;
	print_result();
	get_result(suites);
	cleanup(suites);
}

static void show_list_tests(const TestCase *cases)
{
	size_t i;
	PRINTF0("List Tests\n");
	PRINTF0("  Number  Name\n");
	for (i = 0; cases[i].name != 0; i++) {
		PRINTF2("  %-6d  %s\n", i + 1, cases[i].name);
	}
	PRINTF0("\n");
}

static void show_list_suites(const TestSuite *suites)
{
	size_t i;
	PRINTF0("List Suites\n");
	PRINTF0("  Number  Name\n");
	for (i = 0; suites[i].name != 0; i++) {
		PRINTF2("  %-6d  %s\n", i + 1, suites[i].name);
	}
	PRINTF0("\n");
}

static int find_test_name(const TestCase *cases, const char *input_str)
{
	int i;
	for (i = 0; cases[i].name != 0; i++) {
		if (strcmp(cases[i].name, input_str) == 0) {
			return i;
		}
	}
	return -1;
}

static int get_ncases(const TestCase *cases)
{
	int i;
	for (i = 0; cases[i].name != 0; i++) ;
	return i;
}

static int find_test_number(const TestCase *cases, const char *input_str)
{
	int n;
	n = atoi(input_str);
	if (n <= 0 || get_ncases(cases) + 1 <= n) {
		return -1;
	}
	return n - 1;
}

static void select_test(TestSuite *suites, int suite_idx)
{
	int idx;
	char *p;
	TestSuite *suite = &suites[suite_idx];
	PRINTF0("Enter Test's Number or Name : ");
	FGETS(input_buf, sizeof input_buf, stdin);
	p = strpbrk(input_buf, "\r\n");
	if (p) *p = '\0';

	idx = find_test_number(suite->test_cases, input_buf);
	if (idx == -1) {
		idx = find_test_name(suite->test_cases, input_buf);
		if (idx == -1) {
			PRINTF0("\nTest not found.\n");
			return;
		}
	}
	PRINTF0("\n");

	run_suite_selected(suites, suite_idx, idx);
}

static int find_suite_name(const TestSuite *suites, const char *input_str)
{
	int i;
	for (i = 0; suites[i].name != 0; i++) {
		if (strcmp(suites[i].name, input_str) == 0) {
			return i;
		}
	}
	return -1;
}

static int get_nsuites(const TestSuite *suites)
{
	int i;
	for (i = 0; suites[i].name != 0; i++) ;
	return i;
}

static int find_suite_number(const TestSuite *suites, const char *input_str)
{
	int n;
	n = atoi(input_str);
	if (n <= 0 || get_nsuites(suites) + 1 <= n) {
		return -1;
	}
	return n - 1;
}

static void select_suite(TestSuite *suites)
{
	int idx;
	char *p;
	TestSuite *selected_suite;
	PRINTF0("Enter Suite's Number or Name : ");
	FGETS(input_buf, sizeof input_buf, stdin);
	p = strpbrk(input_buf, "\r\n");
	if (p) *p = '\0';

	idx = find_suite_number(suites, input_buf);
	if (idx == -1) {
		idx = find_suite_name(suites, input_buf);
		if (idx == -1) {
			PRINTF0("\nSuite not found.\n");
			return;
		}
	}

	selected_suite = &suites[idx];

	PRINTF0("\n");
	while (1) {
		PRINTF1("================== Suite : %s ==================\n", selected_suite->name);
		PRINTF0("(R)un all, (S)elect test, (L)ist tests, (M)ove up, (Q)uit\n");
		PRINTF0("Enter Command : ");
		FGETS(input_buf, sizeof input_buf, stdin);
		PRINTF0("\n");
		switch (input_buf[0]) {
		case 'r':
		case 'R':
			run_suite(suites, idx);
			break;
		case 's':
		case 'S':
			select_test(suites, idx);
			break;
		case 'l':
		case 'L':
			show_list_tests(selected_suite->test_cases);
			break;
		case 'm':
		case 'M':
			return;
		case 'q':
		case 'Q':
			LONGJMP(quit_jmp, 1);
			break;
		default:
			break;
		}
		PRINTF0("\n");
	}
}

void unittest_run_interactive(TestSuite *suites)
{
	init(suites);
	if (SETJMP(quit_jmp)) {
		/* Suite Menu's Quit */
		return;
	}
	while (1) {
		PRINTF0("****************** Unit Test ******************\n");
		PRINTF0("(R)un all, (S)elect suite, (L)ist suites, (Q)uit\n");
		PRINTF0("Enter Command : ");
		FGETS(input_buf, sizeof input_buf, stdin);
		PRINTF0("\n");
		switch (input_buf[0]) {
		case 'r':
		case 'R':
			run_all(suites);
			break;
		case 's':
		case 'S':
			select_suite(suites);
			break;
		case 'l':
		case 'L':
			show_list_suites(suites);
			break;
		case 'q':
		case 'Q':
			return;
		default:
			break;
		}
		PRINTF0("\n");
	}
}


void unittest_run_all(TestSuite *suites)
{
	PRINTF0("****************** Unit Test ******************\n");
	PRINTF0("\n");
	init(suites);
	run_all(suites);
}

