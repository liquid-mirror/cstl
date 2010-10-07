#ifndef LIBCIMPL_H_INCLUDED
#define LIBCIMPL_H_INCLUDED

#include <stddef.h>

#ifndef NO_STD_PRINTF
# include <stdio.h>
# define PRINTF0	printf
# define PRINTF1	printf
# define PRINTF2	printf
# define PRINTF3	printf
# define PRINTF4	printf
# define FGETS	fgets
#else
# ifdef stdin
#  undef stdin
# endif
# define stdin	0
# define FGETS	LibcImpl_fgets
char *LibcImpl_fgets(char *s, int size, void *stream);
# ifdef STD_VSPRINTF
#  define PRINTF0	LibcImpl_printf
#  define PRINTF1	LibcImpl_printf
#  define PRINTF2	LibcImpl_printf
#  define PRINTF3	LibcImpl_printf
#  define PRINTF4	LibcImpl_printf
int LibcImpl_printf(const char *format, ...);
# else
#  define PRINTF0(f)					LibcImpl_printf(f, 0, 0, 0, 0)
#  define PRINTF1(f, a1)				LibcImpl_printf(f, (unsigned long)(a1), 0, 0, 0)
#  define PRINTF2(f, a1, a2)			LibcImpl_printf(f, (unsigned long)(a1), (unsigned long)(a2), 0, 0)
#  define PRINTF3(f, a1, a2, a3)		LibcImpl_printf(f, (unsigned long)(a1), (unsigned long)(a2), (unsigned long)(a3), 0)
#  define PRINTF4(f, a1, a2, a3, a4)	LibcImpl_printf(f, (unsigned long)(a1), (unsigned long)(a2), (unsigned long)(a3), (unsigned long)(a4))
int LibcImpl_printf(const char *format, unsigned long arg1, unsigned long arg2, unsigned long arg3, unsigned long arg4);
int LibcImpl_sprintf(char *buf, const char *format, unsigned long arg1, unsigned long arg2, unsigned long arg3, unsigned long arg4);
# endif
#endif


#ifndef NO_STD_MALLOC
# include <stdlib.h>
# define MALLOC	malloc
# define FREE	free
# define STR_MALLOC	malloc
# define STR_FREE	free
#else
# define MALLOC	LibcImpl_malloc
# define FREE	LibcImpl_free
# define STR_MALLOC	LibcImpl_str_malloc
# define STR_FREE	LibcImpl_str_free
void *LibcImpl_malloc(size_t size);
void LibcImpl_free(void *ptr);
char *LibcImpl_str_malloc(size_t size);
void LibcImpl_str_free(char *ptr);
#endif


#ifndef NO_STD_SETJMP
# include <setjmp.h>
# define SETJMP		setjmp
# define LONGJMP	longjmp
#else
# define jmp_buf	int
# define SETJMP(j)	((void)(j), 0)
# define LONGJMP(j, v)
#endif


#endif /* LIBCIMPL_H_INCLUDED */
