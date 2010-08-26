#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../LibcImpl.h"


void test_LibcImpl_sprintf(void)
{
	static char buf[1024];
	static char sp_buf[1024];

	sprintf(sp_buf, "hogehoge");
	LibcImpl_sprintf(buf, "hogehoge", 0, 0, 0, 0);
	printf("%s\n", buf);
	assert(strcmp(buf, sp_buf) == 0);

	sprintf(sp_buf, "%s", "piyopiyo");
	LibcImpl_sprintf(buf, "%s", "piyopiyo", 0, 0, 0);
	printf("%s\n", buf);
	assert(strcmp(buf, sp_buf) == 0);

	sprintf(sp_buf, "foo%%bar%s", "hoge");
	LibcImpl_sprintf(buf, "foo%%bar%s", "hoge", 0, 0, 0);
	printf("%s\n", buf);
	assert(strcmp(buf, sp_buf) == 0);

	sprintf(sp_buf, "%d, %d, %d, \"%8d\"", 9999, 0, -1, -1234);
	LibcImpl_sprintf(buf, "%d, %d, %d, \"%8d\"", (void *)9999, (void*)0, (void*)-1, (void*)-1234);
	printf("%s\n", buf);
	assert(strcmp(buf, sp_buf) == 0);

	sprintf(sp_buf, "%u, %u, %-d, \"%-8d\"", 12345, 0, -123, -111);
	LibcImpl_sprintf(buf, "%u, %u, %-d, \"%-8d\"", (void *)12345, (void*)0, (void*)-123, (void*)-111);
	printf("%s\n", buf);
	assert(strcmp(buf, sp_buf) == 0);

	sprintf(sp_buf, "\"%d\", \"%8d\", \"%-8d\", \"%s\"", 123456789, 123456, 789, "abcd");
	LibcImpl_sprintf(buf, "\"%d\", \"%8d\", \"%-8d\", \"%s\"", (void *)123456789, (void *)123456, (void *)789, "abcd");
	printf("%s\n", buf);
	assert(strcmp(buf, sp_buf) == 0);

	sprintf(sp_buf, "\"%s\", \"%9d\", \"%s\", \"%s\"", "foo", 1234567890, "bar", "abcd");
	LibcImpl_sprintf(buf, "\"%s\", \"%9d\", \"%s\", \"%s\"", "foo", (void *)1234567890, "bar", "abcd");
	printf("%s\n", buf);
	assert(strcmp(buf, sp_buf) == 0);

	sprintf(sp_buf, "%x, \"%8x\", \"%08X\", \"%10d\"", 0x1234, 0x5678, 0xABCD, 12345);
	LibcImpl_sprintf(buf, "%x, \"%8x\", \"%08X\", \"%10d\"", (void *)0x1234, (void*)0x5678, (void*)0xABCD, (void*)12345);
	printf("%s\n", buf);
	assert(strcmp(buf, sp_buf) == 0);

	sprintf(sp_buf, "%x, \"%8x\", \"%-8X\", \"%-10d\"", 0x0, 0x5678, 0xABCD, 12345);
	LibcImpl_sprintf(buf, "%x, \"%8x\", \"%-8X\", \"%-10d\"", (void *)0x0, (void*)0x5678, (void*)0xABCD, (void*)12345);
	printf("%s\n", buf);
	assert(strcmp(buf, sp_buf) == 0);

	sprintf(sp_buf, "%c, %c, %c, %c", 'a', '#', '\\', '9');
	LibcImpl_sprintf(buf, "%c, %c, %c, %c", (void *)'a', (void*)'#', (void*)'\\', (void*)'9');
	printf("%s\n", buf);
	assert(strcmp(buf, sp_buf) == 0);

	sprintf(sp_buf, "%p, %p, %p, %p", (void*)0, (void*)buf, (void*)sp_buf, (void*)0);
	LibcImpl_sprintf(buf, "%p, %p, %p, %p", (void *)0, (void*)buf, (void*)sp_buf, (void*)0);
	printf("%s\n", sp_buf);
	printf("%s\n", buf);

}

int main(void)
{
	test_LibcImpl_sprintf();
	return 0;
}
