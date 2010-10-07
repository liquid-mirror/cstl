#include <string.h>
#include "TestCase.h"
#include "LibcImpl.h"

static TestCase *current_case;
static jmp_buf fatal_jmp;

static int copy_string(char **dst1, char **dst2, const char *src1, const char *src2, long len);
static TestAssertion *TestAssertion_new(unsigned long expected, unsigned long actual, unsigned long type, const char *expr, const char *file, size_t line);
static void TestAssertion_delete(TestAssertion *self);
static void list_push(TestAssertion *list, TestAssertion *node);
static TestAssertion *list_pop(TestAssertion *list);
static int list_empty(TestAssertion *list);

static void TestCase_clear_result(TestCase *self)
{
	self->setup_error = 0;
	self->teardown_error = 0;
	memset(&self->result, 0, sizeof(self->result));
}

void TestCase_init(TestCase *self)
{
	self->assertion_list.next = &self->assertion_list;
	self->assertion_list.prev = &self->assertion_list;
	TestCase_clear_result(self);
}

void TestCase_cleanup(TestCase *self)
{
	TestAssertion *list = &self->assertion_list;
	while (!list_empty(list)) {
		TestAssertion *tmp = list_pop(list);
		TestAssertion_delete(tmp);
	}
	TestCase_clear_result(self);
}

static int TestCase_setup(const TestCase *self)
{
	int ret = 0;
	if (self->setup) {
		ret = self->setup();
	}
	return ret;
}

static int TestCase_teardown(const TestCase *self)
{
	int ret = 0;
	if (self->teardown) {
		ret = self->teardown();
	}
	return ret;
}

void TestCase_test(TestCase *self, size_t *nasserts, size_t *nasserts_failed)
{
	current_case = self;

	self->setup_error = TestCase_setup(self);
	if (self->setup_error) {
		self->result.num_errors_setup++;
		return;
	}
	if (SETJMP(fatal_jmp) == 0) {
		self->test();
	}
	self->teardown_error = TestCase_teardown(self);
	if (self->teardown_error) {
		self->result.num_errors_teardown++;
		return;
	}
}

void TestCase_get_result(TestCase *self, TestCaseResult *result)
{
	*result = self->result;
}

void assert_impl(int passed_flag, unsigned long expected, unsigned long actual, unsigned long type, const char *expr, const char *file, size_t line, int fatal_flag)
{
	TestAssertion *node;
	current_case->result.num_asserts++;
	current_case->result.num_asserts_ran++;

	if (passed_flag) {
		return;
	}

	current_case->result.num_asserts_failed++;

	node = TestAssertion_new(expected, actual, type, expr, file, line);
	if (node) {
		list_push(&current_case->assertion_list, node);
	}
	if (fatal_flag) {
		/* FATAL */
		LONGJMP(fatal_jmp, 1);
	}
}

static TestAssertion *TestAssertion_new(unsigned long expected, unsigned long actual, unsigned long type, const char *expr, const char *file, size_t line)
{
	unsigned long t;
	long len = 0;
	TestAssertion *self = (TestAssertion *) MALLOC(sizeof(TestAssertion));
	if (!self) {
		PRINTF0("malloc failed!!\n");
		return 0;
	}

	self->type = type;
	t = GET_ASSERT_TYPE(type);
	switch (t) {
	case T_NONE:
		break;
	case T_BOOL:
	case T_NUM:
		self->expected.num = expected;
		self->actual.num = actual;
		break;
	case T_PTR:
		self->expected.ptr = (const void *) expected;
		self->actual.ptr = (const void *) actual;
		break;
	case T_NSTR:
		len = GET_NSTR_LEN(type);
	case T_STR:
		if (!copy_string(&self->expected.str, &self->actual.str, (const char *) expected, (const char *) actual, len)) {
			FREE(self);
			return 0;
		}
		break;
	default:
		self->type = T_NONE;
		break;
	}

	self->expr = expr;
	self->file = file;
	self->line = line;
	self->next = self->prev = 0;
	return self;
}

static int copy_string(char **dst1, char **dst2, const char *src1, const char *src2, long len)
{
	char *p1;
	char *p2;
	size_t src1_len;
	size_t src2_len;
	if (len > 0) {
		src1_len = len + 1;
		src2_len = len + 1;
	} else {
		src1_len = strlen(src1) + 1;
		src2_len = strlen(src2) + 1;
	}
	p1 = (char *) STR_MALLOC(sizeof(char) * (src1_len + src2_len));
	if (!p1) {
		return 0;
	}
	p2 = p1 + src1_len;
	if (len > 0) {
		strncpy(p1, src1, len);
		strncpy(p2, src2, len);
		p1[len] = '\0';
		p2[len] = '\0';
	} else {
		strcpy(p1, src1);
		strcpy(p2, src2);
	}
	*dst1 = p1;
	*dst2 = p2;
	return 1;
}

static void TestAssertion_delete(TestAssertion *self)
{
	if (!self) return;
	if (GET_ASSERT_TYPE(self->type) == T_NSTR || GET_ASSERT_TYPE(self->type) == T_STR) {
		STR_FREE(self->expected.str);
	}
	FREE(self);
}

static void list_push(TestAssertion *list, TestAssertion *node)
{
	node->prev = list->prev;
	node->next = list;
	list->prev->next = node;
	list->prev = node;
}

static TestAssertion *list_pop(TestAssertion *list)
{
	TestAssertion *node;
	node = list->next;
	node->prev->next = node->next;
	node->next->prev = node->prev;
	return node;
}

static int list_empty(TestAssertion *list)
{
	return list->next == list;
}

