#ifndef UNITTEST_H_INCLUDED
#define UNITTEST_H_INCLUDED

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * private
 */
typedef struct TestAssertion {
	int passed_flag;
	const char *expr;
	const char *file;
	size_t line;
	struct TestAssertion *next;
	struct TestAssertion *prev;
} TestAssertion;


/* 
 * public
 */
#define TEST_SUITE_NULL	{0, 0, 0, 0}
#define TEST_CASE_NULL	{0, 0}

typedef struct {
	/* public */
	const char *name;
	void (*test)(void);
	/* private */
	TestAssertion assertion_list;
} TestCase;

typedef struct {
	/* public */
	const char *name;
	int (*setup)(void);
	int (*teardown)(void);
	TestCase *test_cases;
} TestSuite;


/* 
 * Unit Test API
 */
void unittest_run_interactive(const TestSuite *suites);
void unittest_run_all(const TestSuite *suites);


/* 
 * Assert Macros
 */
#define ASSERT(expr)		assert_impl(((expr) != 0), "ASSERT(" #expr ")", __FILE__, __LINE__, 0)
#define ASSERT_FATAL(expr)	assert_impl(((expr) != 0), "ASSERT_FATAL(" #expr ")", __FILE__, __LINE__, 1)
#define ASSERT_TRUE(expr)		assert_impl(((expr) != 0), "ASSERT_TRUE(" #expr ")", __FILE__, __LINE__, 0)
#define ASSERT_TRUE_FATAL(expr)	assert_impl(((expr) != 0), "ASSERT_TRUE_FATAL(" #expr ")", __FILE__, __LINE__, 1)
#define ASSERT_FALSE(expr)		assert_impl(((expr) == 0), "ASSERT_FALSE(" #expr ")", __FILE__, __LINE__, 0)
#define ASSERT_FALSE_FATAL(expr)	assert_impl(((expr) == 0), "ASSERT_FALSE_FATAL(" #expr ")", __FILE__, __LINE__, 1)
#define ASSERT_EQUAL(actual, expected)				assert_impl(((actual) == (expected)), "ASSERT_EQUAL(" #actual ", " #expected ")", __FILE__, __LINE__, 0)
#define ASSERT_NOT_EQUAL(actual, expected)			assert_impl(((actual) != (expected)), "ASSERT_NOT_EQUAL(" #actual ", " #expected ")", __FILE__, __LINE__, 0)
#define ASSERT_EQUAL_FATAL(actual, expected)		assert_impl(((actual) == (expected)), "ASSERT_EQUAL_FATAL(" #actual ", " #expected ")", __FILE__, __LINE__, 1)
#define ASSERT_NOT_EQUAL_FATAL(actual, expected)	assert_impl(((actual) != (expected)), "ASSERT_NOT_EQUAL_FATAL(" #actual ", " #expected ")", __FILE__, __LINE__, 1)
#define ASSERT_STRING_EQUAL(actual, expected)		assert_impl((strcmp((actual), (expected)) == 0), "ASSERT_STRING_EQUAL(" #actual ", " #expected ")", __FILE__, __LINE__, 0)
#define ASSERT_STRING_NOT_EQUAL(actual, expected)	assert_impl((strcmp((actual), (expected)) != 0), "ASSERT_STRING_NOT_EQUAL(" #actual ", " #expected ")", __FILE__, __LINE__, 0)
#define ASSERT_STRING_EQUAL_FATAL(actual, expected)		assert_impl((strcmp((actual), (expected)) == 0), "ASSERT_STRING_EQUAL_FATAL(" #actual ", " #expected ")", __FILE__, __LINE__, 1)
#define ASSERT_STRING_NOT_EQUAL_FATAL(actual, expected)	assert_impl((strcmp((actual), (expected)) != 0), "ASSERT_STRING_NOT_EQUAL_FATAL(" #actual ", " #expected ")", __FILE__, __LINE__, 1)
#define ASSERT_FAIL(msg)			assert_impl(0, "ASSERT_FAIL(" #msg ")", __FILE__, __LINE__, 0)
#define ASSERT_FAIL_FATAL(msg)		assert_impl(0, "ASSERT_FAIL_FATAL(" #msg ")", __FILE__, __LINE__, 1)

void assert_impl(int passed_flag, const char *expr, const char *file, size_t line, int fatal_flag);


#ifdef __cplusplus
}
#endif

#endif /* UNITTEST_H_INCLUDED */
