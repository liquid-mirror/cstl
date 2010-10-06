#ifndef TESTSUITE_H_INCLUDED
#define TESTSUITE_H_INCLUDED

#include "UnitTest.h"

enum TestSuiteErr {
	SUITE_OK,
	SETUP_NG,
	TEARDOWN_NG
};

size_t TestSuite_init(TestSuite *self);
size_t TestSuite_cleanup(TestSuite *self);
enum TestSuiteErr TestSuite_test(TestSuite *self, size_t *ncases, size_t *ncases_failed, 
		size_t *nasserts, size_t *nasserts_failed);
enum TestSuiteErr TestSuite_test_selected(TestSuite *self, int case_idx, size_t *ncases, size_t *ncases_failed, 
		size_t *nasserts, size_t *nasserts_failed);
void TestSuite_get_result(TestSuite *self, TestSuiteResult *result);


#endif /* TESTSUITE_H_INCLUDED */
