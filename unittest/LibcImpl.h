#ifndef LIBCIMPL_H_INCLUDED
#define LIBCIMPL_H_INCLUDED


#ifndef NO_STD_PRINTF
# include <stdio.h>
# define PRINTF	printf
# define FGETS	fgets
#else
# ifdef stdin
#  undef stdin
# endif
# define stdin	0
# define PRINTF	LibcImpl_printf
# define FGETS	LibcImpl_fgets
int LibcImpl_printf(const char *format, ...);
char *LibcImpl_fgets(char *s, int size, void *stream);
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
