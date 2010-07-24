#ifndef TESTSUITE_H_INCLUDED
#define TESTSUITE_H_INCLUDED

#include "UnitTest.h"

enum TestSuiteErr {
	SUITE_OK,
	SETUP_NG,
	TEARDOWN_NG
};

size_t TestSuite_init(const TestSuite *self);
size_t TestSuite_cleanup(const TestSuite *self);
int TestSuite_setup(const TestSuite *self);
int TestSuite_teardown(const TestSuite *self);
enum TestSuiteErr TestSuite_test(const TestSuite *self, size_t *ncases, size_t *ncases_failed, 
		size_t *nasserts, size_t *nasserts_failed);
enum TestSuiteErr TestSuite_test_selected(const TestSuite *self, int case_idx, size_t *ncases, size_t *ncases_failed, 
		size_t *nasserts, size_t *nasserts_failed);


#endif /* TESTSUITE_H_INCLUDED */
