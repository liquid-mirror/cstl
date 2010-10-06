#include <string.h>
#include "TestCase.h"
#include "TestSuite.h"
#include "LibcImpl.h"


static void TestSuite_clear_result(TestSuite *self)
{
	self->setup_error = 0;
	self->teardown_error = 0;
	memset(&self->result, 0, sizeof(self->result));
}

size_t TestSuite_init(TestSuite *self)
{
	size_t i;
	for (i = 0; self->test_cases[i].name != 0; i++) {
		TestCase_init(&self->test_cases[i]);
	}
	TestSuite_clear_result(self);
	self->result.num_tests = i;
	return i;
}

size_t TestSuite_cleanup(TestSuite *self)
{
	size_t i;
	for (i = 0; self->test_cases[i].name != 0; i++) {
		TestCase_cleanup(&self->test_cases[i]);
	}
	TestSuite_clear_result(self);
	self->result.num_tests = i;
	return i;
}

static int TestSuite_setup(const TestSuite *self)
{
	int ret = 0;
	if (self->setup) {
		ret = self->setup();
	}
	return ret;
}

static int TestSuite_teardown(const TestSuite *self)
{
	int ret = 0;
	if (self->teardown) {
		ret = self->teardown();
	}
	return ret;
}

enum TestSuiteErr TestSuite_test(TestSuite *self, size_t *ncases, size_t *ncases_failed, 
		size_t *nasserts, size_t *nasserts_failed)
{
	size_t i;
	*ncases = *ncases_failed = *nasserts = *nasserts_failed = 0;
/*    PRINTF1("Suite: %s\n", self->name);*/
	self->setup_error = TestSuite_setup(self);
	if (self->setup_error) {
/*        PRINTF1("  SETUP ERROR[%d]\n", self->setup_error);*/
/*        PRINTF0("\n");*/
		self->result.num_errors_setup++;
		return SETUP_NG;
	}
	for (i = 0; self->test_cases[i].name != 0; i++) {
		size_t na, naf;
		TestCaseResult case_result;
		TestCase_test(&self->test_cases[i], &na, &naf);
		TestCase_get_result(&self->test_cases[i], &case_result);
		self->result.case_result.num_asserts         += case_result.num_asserts;
		self->result.case_result.num_asserts_ran     += case_result.num_asserts_ran;
		self->result.case_result.num_asserts_failed  += case_result.num_asserts_failed;
		self->result.case_result.num_errors_setup    += case_result.num_errors_setup;
		self->result.case_result.num_errors_teardown += case_result.num_errors_teardown;
		self->result.num_tests_ran++;
		if (case_result.num_asserts_failed > 0) {
			self->result.num_tests_failed++;
		}
		(*nasserts) += na;
		(*nasserts_failed) += naf;
		if (naf > 0) {
			(*ncases_failed)++;
		}
	}
	(*ncases) += i;
	self->teardown_error = TestSuite_teardown(self);
	if (self->teardown_error) {
/*        PRINTF1("  TEARDOWN ERROR[%d]\n", self->teardown_error);*/
/*        PRINTF0("\n");*/
		self->result.num_errors_teardown++;
		return TEARDOWN_NG;
	}
/*    PRINTF0("\n");*/
	return SUITE_OK;
}

enum TestSuiteErr TestSuite_test_selected(TestSuite *self, int case_idx, size_t *ncases, size_t *ncases_failed, 
		size_t *nasserts, size_t *nasserts_failed)
{
	TestCaseResult case_result;
	*ncases = *ncases_failed = *nasserts = *nasserts_failed = 0;
/*    PRINTF1("Suite: %s\n", self->name);*/
	self->setup_error = TestSuite_setup(self);
	if (self->setup_error) {
/*        PRINTF1("  SETUP ERROR[%d]\n", self->setup_error);*/
/*        PRINTF0("\n");*/
		self->result.num_errors_setup++;
		return SETUP_NG;
	}
	TestCase_test(&self->test_cases[case_idx], nasserts, nasserts_failed);
	TestCase_get_result(&self->test_cases[case_idx], &case_result);
	self->result.case_result.num_asserts         += case_result.num_asserts;
	self->result.case_result.num_asserts_ran     += case_result.num_asserts_ran;
	self->result.case_result.num_asserts_failed  += case_result.num_asserts_failed;
	self->result.case_result.num_errors_setup    += case_result.num_errors_setup;
	self->result.case_result.num_errors_teardown += case_result.num_errors_teardown;
	self->result.num_tests_ran++;
	if (case_result.num_asserts_failed > 0) {
		self->result.num_tests_failed++;
	}
	if (*nasserts_failed > 0) {
		*ncases_failed = 1;
	}
	*ncases = 1;
	self->teardown_error = TestSuite_teardown(self);
	if (self->teardown_error) {
/*        PRINTF1("  TEARDOWN ERROR[%d]\n", self->teardown_error);*/
/*        PRINTF0("\n");*/
		self->result.num_errors_teardown++;
		return TEARDOWN_NG;
	}
/*    PRINTF0("\n");*/
	return SUITE_OK;
}

void TestSuite_get_result(TestSuite *self, TestSuiteResult *result)
{
	*result = self->result;
}

