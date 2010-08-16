#ifndef LIBCIMPL_H_INCLUDED
#define LIBCIMPL_H_INCLUDED


#ifndef NO_STD_PRINTF
# include <stdio.h>
# define PRINTF		printf
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
#  define PRINTF	LibcImpl_printf
#  define PRINTF1	LibcImpl_printf
#  define PRINTF2	LibcImpl_printf
#  define PRINTF3	LibcImpl_printf
#  define PRINTF4	LibcImpl_printf
int LibcImpl_printf(const char *format, ...);
# else
#  define PRINTF(f)						LibcImpl_printf(f, 0, 0, 0, 0)
#  define PRINTF1(f, a1)				LibcImpl_printf(f, (void *)(a1), 0, 0, 0)
#  define PRINTF2(f, a1, a2)			LibcImpl_printf(f, (void *)(a1), (void *)(a2), 0, 0)
#  define PRINTF3(f, a1, a2, a3)		LibcImpl_printf(f, (void *)(a1), (void *)(a2), (void *)(a3), 0)
#  define PRINTF4(f, a1, a2, a3, a4)	LibcImpl_printf(f, (void *)(a1), (void *)(a2), (void *)(a3), (void *)(a4))
int LibcImpl_printf(const char *format, void *arg1, void *arg2, void *arg3, void *arg4);
int LibcImpl_sprintf(char *buf, const char *format, void *arg1, void *arg2, void *arg3, void *arg4);
# endif
#endif


#ifndef NO_STD_MALLOC
# include <stdlib.h>
# define MALLOC	malloc
# define FREE	free
#else
# define MALLOC	LibcImpl_malloc
# define FREE	LibcImpl_free
void *LibcImpl_malloc(size_t size);
void LibcImpl_free(void *ptr);
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
