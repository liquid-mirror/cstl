#include "LibcImpl.h"

#ifdef NO_STD_PRINTF

#define BUF_SIZE	512

char *LibcImpl_fgets(char *s, int size, void *stream)
{
	/* TODO */
	/* ex. SCI read */

	return s;
}

#ifdef STD_VSPRINTF
#undef stdin
#include <stdarg.h>
#include <stdio.h>

int LibcImpl_printf(const char *format, ...)
{
	/* NOTE: attention to buffer overflow */
	static char buf[BUF_SIZE];
	va_list ap;
	va_start(ap, format);
	vsprintf(buf, format, ap);
	va_end(ap);

	/* TODO */
	/* ex. SCI write */

	return 0;
}

#else
int LibcImpl_printf(const char *format, unsigned long arg1, unsigned long arg2, unsigned long arg3, unsigned long arg4)
{
	/* NOTE: attention to buffer overflow */
	static char buf[BUF_SIZE];
	LibcImpl_sprintf(buf, format, arg1, arg2, arg3, arg4);

	/* TODO */
	/* ex. SCI write */

	return 0;
}

static size_t set_ascii(char *ascii, const char *tmp, size_t size, size_t width, int left_flag, int zero_flag)
{
	size_t i;
	if (width > 1) {
		if (left_flag) {
			for (i = 0; i < size; i++) {
				ascii[i] = tmp[size - i - 1];
			}
			if (width > size) {
				for (i = 0; i < width - size; i++) {
					ascii[size + i] = ' ';
				}
				size += width - size;
			}
		} else {
			if (width > size) {
				for (i = 0; i < width - size; i++) {
					ascii[i] = zero_flag ? '0' : ' ';
				}
				for (i = 0; i < size; i++) {
					ascii[width - size + i] = tmp[size - i - 1];
				}
				size += width - size;
			} else {
				for (i = 0; i < size; i++) {
					ascii[i] = tmp[size - i - 1];
				}
			}
		}
	} else {
		for (i = 0; i < size; i++) {
			ascii[i] = tmp[size - i - 1];
		}
	}
	return size;
}

static size_t dec2ascii(char *ascii, unsigned int dec, size_t width, int left_flag, int zero_flag, int signed_flag)
{
	size_t i;
	char tmp[16];
	const char *num_str = "0123456789";
	int signed_dec = (int) dec;
	if (dec == 0) {
		tmp[0] = '0';
		i = 1;
	} else if (signed_flag && signed_dec < 0) {
		signed_dec = -signed_dec;
		for (i = 0; signed_dec > 0 && i < 15; i++) {
			tmp[i] = num_str[signed_dec % 10];
			signed_dec /= 10;
		}
		tmp[i++] = '-';
	} else {
		for (i = 0; dec > 0 && i < 16; i++) {
			tmp[i] = num_str[dec % 10];
			dec /= 10;
		}
	}
	return set_ascii(ascii, tmp, i, width, left_flag, zero_flag);
}

static size_t hex2ascii(char *ascii, unsigned long hex, size_t width, int left_flag, int zero_flag, char case_char)
{
	size_t i;
	char tmp[16];
	const char *num_str = (case_char == 'X') ? "0123456789ABCDEF" : "0123456789abcdef";
	if (hex == 0) {
		tmp[0] = '0';
		i = 1;
	} else {
		for (i = 0; hex > 0 && i < 16; i++) {
			tmp[i] = num_str[hex & 0xf];
			hex >>= 4;
		}
	}
	return set_ascii(ascii, tmp, i, width, left_flag, zero_flag);
}

int LibcImpl_sprintf(char *buf, const char *format, unsigned long arg1, unsigned long arg2, unsigned long arg3, unsigned long arg4)
{
	size_t i;
	const char *p = format;
	unsigned long arg_list[4];
	size_t arg_idx = 0;
	const char *tmp_str;
	unsigned long tmp_val;
	int left_flag;
	int zero_flag;
	size_t width;
	size_t inc;

	arg_list[0] = arg1;
	arg_list[1] = arg2;
	arg_list[2] = arg3;
	arg_list[3] = arg4;

	i = 0;
	while (*p != '\0') {
		if (*p != '%') {
			buf[i++] = *(p++);
			continue;
		}
		p++;
		if (*p == '%') {
			buf[i++] = *(p++);
			continue;
		}
		if (*p == '-') {
			left_flag = 1;
			p++;
		} else {
			left_flag = 0;
		}
		if (*p == '0') {
			zero_flag = 1;
			p++;
		} else {
			zero_flag = 0;
		}
		if ('1' <= *p && *p <= '9') {
			width = *p - '0';
			p++;
			if ('0' <= *p && *p <= '9') {
				width = 10 * width + (*p - '0');
				p++;
			}
		} else {
			width = 0;
		}
		switch (*p) {
		case 'c':
			buf[i++] = (char) arg_list[arg_idx++];
			p++;
			break;
		case 'd':
		case 'i':
		case 'u':
			tmp_val = arg_list[arg_idx++];
			inc = dec2ascii(&buf[i], (unsigned int) tmp_val, width, left_flag, zero_flag, (*p == 'u') ? 0 : 1);
			i += inc;
			p++;
			break;
		case 'p':
			buf[i++] = '0';
			buf[i++] = 'x';
			zero_flag = 1;
			width = 8;
		case 'x':
		case 'X':
			tmp_val = arg_list[arg_idx++];
			inc = hex2ascii(&buf[i], tmp_val, width, left_flag, zero_flag, *p);
			i += inc;
			p++;
			break;
		case 's':
			tmp_str = (const char *) arg_list[arg_idx++];
			while (*tmp_str != '\0') {
				buf[i++] = *(tmp_str++);
			}
			p++;
			break;
		default:
			p++;
			break;
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


