#include "LibcImpl.h"

#ifdef NO_STD_PRINTF
#include <stdarg.h>
#include <stdio.h>

int LibcImpl_printf(const char *format, ...)
{
	/* NOTE: attention to buffer overflow */
	static char buf[1024];
	va_list ap;
	va_start(ap, format);
	vsprintf(buf, format, ap);
	va_end(ap);

	/* TODO */
	/* ex. SCI write */

	return 0;
}

char *LibcImpl_fgets(char *s, int size, void *stream)
{
	/* TODO */
	/* ex. SCI read */

	return s;
}
#endif

#ifdef NO_STD_MALLOC
#include "UnitTest.h"

/* You can change the value of MAX_NUM_ASSERTS. */
#define MAX_NUM_ASSERTS		256

typedef struct {
	TestAssertion obj;
	int used_flag;
} TestAssertionBlock;

static TestAssertionBlock pool[MAX_NUM_ASSERTS];

void *LibcImpl_malloc(size_t size)
{
	size_t i;
	TestAssertionBlock *p = pool;
	(void) size;
	for (i = 0; i < MAX_NUM_ASSERTS; i++) {
		if (p[i].used_flag == 0) {
			p[i].used_flag = 1;
			return &p[i];
		}
	}
	return 0;
}

void LibcImpl_free(void *ptr)
{
	if (!ptr) return;
	((TestAssertionBlock *) ptr)->used_flag = 0;
}
#endif


