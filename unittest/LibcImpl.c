#include "LibcImpl.h"

#ifdef NO_STD_PRINTF

char *LibcImpl_fgets(char *s, int size, void *stream)
{
	/* TODO */
	/* ex. SCI read */

	return s;
}

#ifdef STD_VSPRINTF
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

#else
int LibcImpl_printf(const char *format, void *arg1, void *arg2, void *arg3, void *arg4)
{
	/* NOTE: attention to buffer overflow */
	static char buf[1024];
	LibcImpl_sprintf(buf, format, arg1, arg2, arg3, arg4);

	/* TODO */
	/* ex. SCI write */

	return 0;
}

static size_t dec2ascii(char *ascii, unsigned int dec, size_t width, int left_flag)
{
	size_t i;
	size_t ret;
	char tmp[16];
	for (i = 0; dec > 0 && i < 16; i++) {
		tmp[i] = dec % 10;
		dec /= 10;
	}
	ret = i;

	if (width > 1) {
		if (left_flag) {
			for (i = 0; i < ret; i++) {
				ascii[i] = tmp[ret - i - 1] + '0';
			}
			if (width > ret) {
				for (i = 0; i < width - ret; i++) {
					ascii[ret + i] = ' ';
				}
				ret += width - ret;
			}
		} else {
			if (width > ret) {
				for (i = 0; i < width - ret; i++) {
					ascii[i] = ' ';
				}
				for (i = 0; i < ret; i++) {
					ascii[width - ret + i] = tmp[ret - i - 1] + '0';
				}
				ret += width - ret;
			} else {
				for (i = 0; i < ret; i++) {
					ascii[i] = tmp[ret - i - 1] + '0';
				}
			}
		}
	} else {
		for (i = 0; i < ret; i++) {
			ascii[i] = tmp[ret - i - 1] + '0';
		}
	}
	return ret;
}

int LibcImpl_sprintf(char *buf, const char *format, void *arg1, void *arg2, void *arg3, void *arg4)
{
	size_t i;
	const char *p = format;
	void *arg_list[4];
	size_t arg_idx = 0;
	const char *tmp_str;
	unsigned int tmp_val;
	int left_flag;
	size_t width;
	size_t inc;

	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	arg_list[3] = arg4;

	i = 0;
	while (*p != '\0') {
		if (*p == '%') {
			p++;
			if (*p == '%') {
				buf[i++] = *(p++);
			} else {
				if (*p == '-') {
					left_flag = 1;
					p++;
				} else {
					left_flag = 0;
				}
				if ('1' <= *p && *p <= '9') {
					width = *p - '0';
					p++;
				} else {
					width = 0;
				}
				switch (*p) {
				case 'd':
					tmp_val = (unsigned int) arg_list[arg_idx];
					inc = dec2ascii(&buf[i], tmp_val, width, left_flag);
					i += inc;
					p++;
					arg_idx++;
					break;
				case 's':
					tmp_str = (const char *) arg_list[arg_idx];
					while (*tmp_str != '\0') {
						buf[i++] = *(tmp_str++);
					}
					p++;
					arg_idx++;
					break;
				default:
					p++;
					break;
				}
			}
		} else {
			buf[i++] = *(p++);
		}
	}
	buf[i] = '\0';
	return (int) i;
}

#endif
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


