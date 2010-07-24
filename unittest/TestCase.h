#ifndef TESTCASE_H_INCLUDED
#define TESTCASE_H_INCLUDED

#include "UnitTest.h"


void TestCase_init(TestCase *self);
void TestCase_cleanup(TestCase *self);
void TestCase_test(TestCase *self, size_t *nasserts, size_t *nasserts_failed);


#endif /* TESTCASE_H_INCLUDED */
