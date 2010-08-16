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

	sprintf(sp_buf, "%d", 9999);
	LibcImpl_sprintf(buf, "%d", (void *)9999, 0, 0, 0);
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

}

int main(void)
{
	test_LibcImpl_sprintf();
	return 0;
}

