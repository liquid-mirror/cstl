#ifndef TESTCASE_H_INCLUDED
#define TESTCASE_H_INCLUDED

#include "UnitTest.h"

#define LIST_BEGIN(list)	(list)->next
#define LIST_END(list)		(list)


void TestCase_init(TestCase *self);
void TestCase_cleanup(TestCase *self);
void TestCase_test(TestCase *self, size_t *nasserts, size_t *nasserts_failed);
void TestCase_get_result(TestCase *self, TestCaseResult *result);


#endif /* TESTCASE_H_INCLUDED */
