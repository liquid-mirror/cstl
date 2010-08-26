#include "TestCase.h"
#include "TestSuite.h"
#include "LibcImpl.h"


size_t TestSuite_init(const TestSuite *self)
{
	size_t i;
	for (i = 0; self->test_cases[i].name != 0; i++) {
		TestCase_init(&self->test_cases[i]);
	}
	return i;
}

size_t TestSuite_cleanup(const TestSuite *self)
{
	size_t i;
	for (i = 0; self->test_cases[i].name != 0; i++) {
		TestCase_cleanup(&self->test_cases[i]);
	}
	return i;
}

int TestSuite_setup(const TestSuite *self)
{
	int ret = 0;
	if (self->setup) {
		ret = self->setup();
	}
	return ret;
}

int TestSuite_teardown(const TestSuite *self)
{
	int ret = 0;
	if (self->teardown) {
		ret = self->teardown();
	}
	return ret;
}

enum TestSuiteErr TestSuite_test(const TestSuite *self, size_t *ncases, size_t *ncases_failed, 
		size_t *nasserts, size_t *nasserts_failed)
{
	size_t i;
	int err;
	*ncases = *ncases_failed = *nasserts = *nasserts_failed = 0;
	PRINTF1("Suite: %s\n", self->name);
	err = TestSuite_setup(self);
	if (err) {
		PRINTF1("  SETUP FAILED: error[%d]\n", err);
		PRINTF0("\n");
		return SETUP_NG;
	}
	for (i = 0; self->test_cases[i].name != 0; i++) {
		size_t na, naf;
		TestCase_test(&self->test_cases[i], &na, &naf);
		(*nasserts) += na;
		(*nasserts_failed) += naf;
		if (naf > 0) {
			(*ncases_failed)++;
		}
	}
	(*ncases) += i;
	err = TestSuite_teardown(self);
	if (err) {
		PRINTF1("  TEARDOWN FAILED: error[%d]\n", err);
		PRINTF0("\n");
		return TEARDOWN_NG;
	}
	PRINTF0("\n");
	return SUITE_OK;
}

enum TestSuiteErr TestSuite_test_selected(const TestSuite *self, int case_idx, size_t *ncases, size_t *ncases_failed, 
		size_t *nasserts, size_t *nasserts_failed)
{
	int err;
	*ncases = *ncases_failed = *nasserts = *nasserts_failed = 0;
	PRINTF1("Suite: %s\n", self->name);
	err = TestSuite_setup(self);
	if (err) {
		PRINTF1("  SETUP FAILED: error[%d]\n", err);
		PRINTF0("\n");
		return SETUP_NG;
	}
	TestCase_test(&self->test_cases[case_idx], nasserts, nasserts_failed);
	if (*nasserts_failed > 0) {
		*ncases_failed = 1;
	}
	*ncases = 1;
	err = TestSuite_teardown(self);
	if (err) {
		PRINTF1("  TEARDOWN FAILED: error[%d]\n", err);
		PRINTF0("\n");
		return TEARDOWN_NG;
	}
	PRINTF0("\n");
	return SUITE_OK;
}

