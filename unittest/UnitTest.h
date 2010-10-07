#ifndef UNITTEST_H_INCLUDED
#define UNITTEST_H_INCLUDED

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * private
 */
#define T_NONE	0x01000000
#define T_BOOL	0x02000000
#define T_NUM	0x04000000
#define T_PTR	0x08000000
#define T_STR	0x10000000
#define T_NSTR	0x40000000
#define T_NOT	0x80000000

#define GET_ASSERT_TYPE(type)		(((type) & T_NSTR) ? T_NSTR : ((type) & ~(T_NSTR | T_NOT)))
#define GET_NSTR_LEN(type)			((type) & ~(T_NSTR | T_NOT))
#define GET_NOT_FLAG(type)			((type) & T_NOT)

typedef struct TestAssertion {
	const char *expr;
	union {
		unsigned long num;
		char *str;
		const void *ptr;
	} expected;
	union {
		unsigned long num;
		char *str;
		const void *ptr;
	} actual;
	unsigned long type;
	const char *file;
	size_t line;
	struct TestAssertion *next;
	struct TestAssertion *prev;
} TestAssertion;

typedef struct {
	size_t num_asserts;
	size_t num_asserts_ran;
	size_t num_asserts_failed;
	size_t num_errors_setup;
	size_t num_errors_teardown;
} TestCaseResult;

typedef struct {
	size_t num_tests;
	size_t num_tests_ran;
	size_t num_tests_failed;
	size_t num_errors_setup;
	size_t num_errors_teardown;
	TestCaseResult case_result;
} TestSuiteResult;

typedef struct {
	size_t num_suites;
	size_t num_suites_ran;
	size_t num_suites_failed;
	TestSuiteResult suite_result;
} TestResult;

/* 
 * public
 */
#define TEST_SUITE_NULL	{0, 0, 0, 0}
#define TEST_CASE_NULL	{0, 0, 0, 0}

typedef struct {
	/* public */
	const char *name;
	void (*test)(void);
	int (*setup)(void);
	int (*teardown)(void);
	/* private */
	int setup_error;
	int teardown_error;
	TestCaseResult result;
	TestAssertion assertion_list;
} TestCase;

typedef struct {
	/* public */
	const char *name;
	TestCase *test_cases;
	int (*setup)(void);
	int (*teardown)(void);
	/* private */
	int setup_error;
	int teardown_error;
	TestSuiteResult result;
} TestSuite;


/* 
 * Unit Test API
 */
void unittest_run_interactive(TestSuite *suites);
void unittest_run_all(TestSuite *suites);


/* 
 * Assert Macros
 */
#define ASSERT(expr)		assert_impl(((expr) != 0), 0, 0, T_NONE, "ASSERT(" #expr ")", __FILE__, __LINE__, 0)
#define ASSERT_FATAL(expr)	assert_impl(((expr) != 0), 0, 0, T_NONE, "ASSERT_FATAL(" #expr ")", __FILE__, __LINE__, 1)
#define ASSERT_FAIL(msg)			assert_impl(0, 0, 0, T_NONE, "ASSERT_FAIL(" #msg ")", __FILE__, __LINE__, 0)
#define ASSERT_FAIL_FATAL(msg)		assert_impl(0, 0, 0, T_NONE, "ASSERT_FAIL_FATAL(" #msg ")", __FILE__, __LINE__, 1)

#define ASSERT_TRUE(expr)			assert_impl(((expr) != 0), 1, (unsigned long)(expr), T_BOOL, "ASSERT_TRUE(" #expr ")", __FILE__, __LINE__, 0)
#define ASSERT_TRUE_FATAL(expr)		assert_impl(((expr) != 0), 1, (unsigned long)(expr), T_BOOL, "ASSERT_TRUE_FATAL(" #expr ")", __FILE__, __LINE__, 1)
#define ASSERT_FALSE(expr)			assert_impl(((expr) == 0), 0, (unsigned long)(expr), T_BOOL, "ASSERT_FALSE(" #expr ")", __FILE__, __LINE__, 0)
#define ASSERT_FALSE_FATAL(expr)	assert_impl(((expr) == 0), 0, (unsigned long)(expr), T_BOOL, "ASSERT_FALSE_FATAL(" #expr ")", __FILE__, __LINE__, 1)

#define ASSERT_EQUAL(expected, actual)				assert_impl(((expected) == (actual)), (unsigned long)(expected), (unsigned long)(actual), T_NUM, "ASSERT_EQUAL(" #expected ", " #actual ")", __FILE__, __LINE__, 0)
#define ASSERT_NOT_EQUAL(expected, actual)			assert_impl(((expected) != (actual)), (unsigned long)(expected), (unsigned long)(actual), T_NUM | T_NOT, "ASSERT_NOT_EQUAL(" #expected ", " #actual ")", __FILE__, __LINE__, 0)
#define ASSERT_EQUAL_FATAL(expected, actual)		assert_impl(((expected) == (actual)), (unsigned long)(expected), (unsigned long)(actual), T_NUM, "ASSERT_EQUAL_FATAL(" #expected ", " #actual ")", __FILE__, __LINE__, 1)
#define ASSERT_NOT_EQUAL_FATAL(expected, actual)	assert_impl(((expected) != (actual)), (unsigned long)(expected), (unsigned long)(actual), T_NUM | T_NOT, "ASSERT_NOT_EQUAL_FATAL(" #expected ", " #actual ")", __FILE__, __LINE__, 1)

#define ASSERT_PTR_EQUAL(expected, actual)				assert_impl(((expected) == (actual)), (unsigned long)(expected), (unsigned long)(actual), T_PTR, "ASSERT_PTR_EQUAL(" #expected ", " #actual ")", __FILE__, __LINE__, 0)
#define ASSERT_PTR_NOT_EQUAL(expected, actual)			assert_impl(((expected) != (actual)), (unsigned long)(expected), (unsigned long)(actual), T_PTR | T_NOT, "ASSERT_PTR_NOT_EQUAL(" #expected ", " #actual ")", __FILE__, __LINE__, 0)
#define ASSERT_PTR_EQUAL_FATAL(expected, actual)		assert_impl(((expected) == (actual)), (unsigned long)(expected), (unsigned long)(actual), T_PTR, "ASSERT_PTR_EQUAL_FATAL(" #expected ", " #actual ")", __FILE__, __LINE__, 1)
#define ASSERT_PTR_NOT_EQUAL_FATAL(expected, actual)	assert_impl(((expected) != (actual)), (unsigned long)(expected), (unsigned long)(actual), T_PTR | T_NOT, "ASSERT_PTR_NOT_EQUAL_FATAL(" #expected ", " #actual ")", __FILE__, __LINE__, 1)

#define ASSERT_STRING_EQUAL(expected, actual)			assert_impl((strcmp((expected), (actual)) == 0), (unsigned long)(expected), (unsigned long)(actual), T_STR, "ASSERT_STRING_EQUAL(" #expected ", " #actual ")", __FILE__, __LINE__, 0)
#define ASSERT_STRING_NOT_EQUAL(expected, actual)		assert_impl((strcmp((expected), (actual)) != 0), (unsigned long)(expected), (unsigned long)(actual), T_STR | T_NOT, "ASSERT_STRING_NOT_EQUAL(" #expected ", " #actual ")", __FILE__, __LINE__, 0)
#define ASSERT_STRING_EQUAL_FATAL(expected, actual)		assert_impl((strcmp((expected), (actual)) == 0), (unsigned long)(expected), (unsigned long)(actual), T_STR, "ASSERT_STRING_EQUAL_FATAL(" #expected ", " #actual ")", __FILE__, __LINE__, 1)
#define ASSERT_STRING_NOT_EQUAL_FATAL(expected, actual)	assert_impl((strcmp((expected), (actual)) != 0), (unsigned long)(expected), (unsigned long)(actual), T_STR | T_NOT, "ASSERT_STRING_NOT_EQUAL_FATAL(" #expected ", " #actual ")", __FILE__, __LINE__, 1)

#define ASSERT_NSTRING_EQUAL(expected, actual, n)			assert_impl((strncmp((expected), (actual), (n)) == 0), (unsigned long)(expected), (unsigned long)(actual), (T_NSTR | (n)), "ASSERT_NSTRING_EQUAL(" #expected ", " #actual ", " #n ")", __FILE__, __LINE__, 0)
#define ASSERT_NSTRING_NOT_EQUAL(expected, actual, n)		assert_impl((strncmp((expected), (actual), (n)) != 0), (unsigned long)(expected), (unsigned long)(actual), (T_NSTR | T_NOT | (n)), "ASSERT_NSTRING_NOT_EQUAL(" #expected ", " #actual ", " #n ")", __FILE__, __LINE__, 0)
#define ASSERT_NSTRING_EQUAL_FATAL(expected, actual, n)		assert_impl((strncmp((expected), (actual), (n)) == 0), (unsigned long)(expected), (unsigned long)(actual), (T_NSTR | (n)), "ASSERT_NSTRING_EQUAL_FATAL(" #expected ", " #actual ", " #n ")", __FILE__, __LINE__, 1)
#define ASSERT_NSTRING_NOT_EQUAL_FATAL(expected, actual, n)	assert_impl((strncmp((expected), (actual), (n)) != 0), (unsigned long)(expected), (unsigned long)(actual), (T_NSTR | T_NOT | (n)), "ASSERT_NSTRING_NOT_EQUAL_FATAL(" #expected ", " #actual ", " #n ")", __FILE__, __LINE__, 1)


void assert_impl(int passed_flag, unsigned long expected, unsigned long actual, unsigned long type, const char *expr, const char *file, size_t line, int fatal_flag);


#ifdef __cplusplus
}
#endif

#endif /* UNITTEST_H_INCLUDED */
