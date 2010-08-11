#include <string.h>
#include <stdlib.h>
#include "TestSuite.h"
#include "LibcImpl.h"

static char input_buf[64];
static jmp_buf quit_jmp;

static size_t suites_total, suites_ran, suites_failed;
static size_t tests_total, tests_ran, tests_failed;
static size_t asserts_total, asserts_ran, asserts_failed;
static size_t setup_failed, teardown_failed;

static void clear_values(void)
{
	suites_total = suites_ran = suites_failed = 0;
	tests_total = tests_ran = tests_failed = 0;
	asserts_total = asserts_ran = asserts_failed = 0;
	setup_failed = teardown_failed = 0;
}

static void init(const TestSuite *suites)
{
	size_t i;
	clear_values();
	for (i = 0; suites[i].name != 0; i++) {
		size_t ncases;
		ncases = TestSuite_init(&suites[i]);
		tests_total += ncases;
	}
	suites_total += i;
}

static void cleanup(const TestSuite *suites)
{
	size_t i;
	clear_values();
	for (i = 0; suites[i].name != 0; i++) {
		size_t ncases;
		ncases = TestSuite_cleanup(&suites[i]);
		tests_total += ncases;
	}
	suites_total += i;
}

static void print_result(void)
{
	PRINTF("Type       Total      Ran   Passed   Failed\n");
	PRINTF("suites  %8d %8d      n/a %8d", suites_total, suites_ran, suites_failed);
	if (setup_failed || teardown_failed) {
		PRINTF("(setup:%d/teardown:%d)\n", setup_failed, teardown_failed);
	} else {
		PRINTF("\n");
	}
	PRINTF("tests   %8d %8d %8d %8d\n", tests_total, tests_ran, tests_ran - tests_failed, tests_failed);
	PRINTF("asserts %8d %8d %8d %8d\n", asserts_total, asserts_ran, asserts_ran - asserts_failed, asserts_failed);
	PRINTF("\n");
}

static void run_all(const TestSuite *suites)
{
	size_t i;
	for (i = 0; suites[i].name != 0; i++) {
		size_t nc, ncf, na, naf;
		enum TestSuiteErr ret;
		ret = TestSuite_test(&suites[i], &nc, &ncf, &na, &naf);
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
	cleanup(suites);
}

static void run_suite_selected(const TestSuite *suites, int suite_idx, int case_idx)
{
	size_t nc, ncf, na, naf;
	enum TestSuiteErr ret;
	ret = TestSuite_test_selected(&suites[suite_idx], case_idx, &nc, &ncf, &na, &naf);
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
	cleanup(suites);
}

static void run_suite(const TestSuite *suites, int suite_idx)
{
	size_t nc, ncf, na, naf;
	enum TestSuiteErr ret;
	ret = TestSuite_test(&suites[suite_idx], &nc, &ncf, &na, &naf);
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
	cleanup(suites);
}

static void show_list_tests(const TestCase *cases)
{
	size_t i;
	PRINTF("List Tests\n");
	PRINTF("  Number  Name\n");
	for (i = 0; cases[i].name != 0; i++) {
		PRINTF("  %-6d  %s\n", i + 1, cases[i].name);
	}
	PRINTF("\n");
}

static void show_list_suites(const TestSuite *suites)
{
	size_t i;
	PRINTF("List Suites\n");
	PRINTF("  Number  Name\n");
	for (i = 0; suites[i].name != 0; i++) {
		PRINTF("  %-6d  %s\n", i + 1, suites[i].name);
	}
	PRINTF("\n");
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

static void select_test(const TestSuite *suites, int suite_idx)
{
	int idx;
	char *p;
	const TestSuite *suite = &suites[suite_idx];
	PRINTF("Enter Test's Number or Name : ");
	FGETS(input_buf, sizeof input_buf, stdin);
	p = strpbrk(input_buf, "\r\n");
	if (p) *p = '\0';

	idx = find_test_number(suite->test_cases, input_buf);
	if (idx == -1) {
		idx = find_test_name(suite->test_cases, input_buf);
		if (idx == -1) {
			PRINTF("\nTest not found.\n");
			return;
		}
	}
	PRINTF("\n");

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

static void select_suite(const TestSuite *suites)
{
	int idx;
	char *p;
	const TestSuite *selected_suite;
	PRINTF("Enter Suite's Number or Name : ");
	FGETS(input_buf, sizeof input_buf, stdin);
	p = strpbrk(input_buf, "\r\n");
	if (p) *p = '\0';

	idx = find_suite_number(suites, input_buf);
	if (idx == -1) {
		idx = find_suite_name(suites, input_buf);
		if (idx == -1) {
			PRINTF("\nSuite not found.\n");
			return;
		}
	}

	selected_suite = &suites[idx];

	PRINTF("\n");
	while (1) {
		PRINTF("================== Suite : %s ==================\n", selected_suite->name);
		PRINTF("(R)un all, (S)elect test, (L)ist tests, (M)ove up, (Q)uit\n");
		PRINTF("Enter Command : ");
		FGETS(input_buf, sizeof input_buf, stdin);
		PRINTF("\n");
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
		PRINTF("\n");
	}
}

void unittest_run_interactive(const TestSuite *suites)
{
	init(suites);
	if (SETJMP(quit_jmp)) {
		/* Suite Menu's Quit */
		return;
	}
	while (1) {
		PRINTF("****************** Unit Test ******************\n");
		PRINTF("(R)un all, (S)elect suite, (L)ist suites, (Q)uit\n");
		PRINTF("Enter Command : ");
		FGETS(input_buf, sizeof input_buf, stdin);
		PRINTF("\n");
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
		PRINTF("\n");
	}
}


void unittest_run_all(const TestSuite *suites)
{
	PRINTF("****************** Unit Test ******************\n");
	PRINTF("\n");
	init(suites);
	run_all(suites);
}

