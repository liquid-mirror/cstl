#include "TestCase.h"
#include "LibcImpl.h"

#define LIST_BEGIN(list)	(list)->next
#define LIST_END(list)		(list)

static TestCase *current_case;
static jmp_buf fatal_jmp;

static TestAssertion *TestAssertion_new(int passed_flag, const char *expr, const char *file, size_t line);
static void TestAssertion_delete(TestAssertion *self);
static void list_push(TestAssertion *list, TestAssertion *node);
static TestAssertion *list_pop(TestAssertion *list);
static int list_empty(TestAssertion *list);

void TestCase_init(TestCase *self)
{
	self->assertion_list.next = &self->assertion_list;
	self->assertion_list.prev = &self->assertion_list;
}

void TestCase_cleanup(TestCase *self)
{
	TestAssertion *list = &self->assertion_list;
	while (!list_empty(list)) {
		TestAssertion *tmp = list_pop(list);
		TestAssertion_delete(tmp);
	}
}

void TestCase_test(TestCase *self, size_t *nasserts, size_t *nasserts_failed)
{
	int failed_flag = 0;
	size_t n = 1;
	TestAssertion *pos;
	TestAssertion *list = &self->assertion_list;
	current_case = self;
	*nasserts = 0;
	*nasserts_failed = 0;
	PRINTF("  Test: %s ... ", self->name);
	if (SETJMP(fatal_jmp) == 0) {
		self->test();
	}
	for (pos = LIST_BEGIN(list); pos != LIST_END(list); pos = pos->next) {
		(*nasserts)++;
		if (!pos->passed_flag) {
			(*nasserts_failed)++;
			if (!failed_flag) {
				failed_flag = 1;
				PRINTF("FAILED\n");
			}
			PRINTF("    %d. %s(%d) %s\n", n, pos->file, pos->line, pos->expr);
			n++;
		}
	}
	if (!failed_flag) {
		PRINTF("passed\n");
	}
}

void assert_impl(int passed_flag, const char *expr, const char *file, size_t line, int fatal_flag)
{
	TestAssertion *node = TestAssertion_new(passed_flag, expr, file, line);
	if (node) {
		list_push(&current_case->assertion_list, node);
	}
	if (fatal_flag && !passed_flag) {
		/* FATAL */
		LONGJMP(fatal_jmp, 1);
	}
}

static TestAssertion *TestAssertion_new(int passed_flag, const char *expr, const char *file, size_t line)
{
	TestAssertion *self = (TestAssertion *) MALLOC(sizeof(TestAssertion));
	if (!self) {
		PRINTF("malloc failed!!\n");
		return 0;
	}
	self->passed_flag = passed_flag;
	self->expr = expr;
	self->file = file;
	self->line = line;
	self->next = self->prev = 0;
	return self;
}

static void TestAssertion_delete(TestAssertion *self)
{
	if (!self) return;
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

