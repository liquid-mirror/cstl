#ifndef CSTL_LIST_DEBUG_H_INCLUDED
#define CSTL_LIST_DEBUG_H_INCLUDED

#include <stdio.h>


#define CSTL_LIST_DEBUG_INTERFACE(Name, Type)	\
int Name##_verify(Name *self);\


#define CSTL_LIST_DEBUG_IMPLEMENT(Name, Type, format)	\
int Name##_verify(Name *self)\
{\
	Name##Iterator pos;\
	Name##Iterator ppos;\
	for (ppos = Name##_end(self), pos = Name##_begin(self); pos != Name##_end(self); pos = CSTL_LIST_NEXT(pos), ppos = CSTL_LIST_NEXT(ppos)) {\
		if (ppos != CSTL_LIST_PREV(pos)) {\
			printf("1: prev[%p], p[%p], next[%p]\n", (void *) CSTL_LIST_PREV(pos), (void *) pos, (void *) CSTL_LIST_NEXT(pos));\
			return 0;\
		}\
	}\
	if (ppos != CSTL_LIST_PREV(pos)) {\
		printf("2: prev[%p], p[%p], next[%p]\n", (void *) CSTL_LIST_PREV(pos), (void *) pos, (void *) CSTL_LIST_NEXT(pos));\
		return 0;\
	}\
	for (ppos = Name##_rend(self), pos = Name##_rbegin(self); pos != Name##_rend(self); pos = CSTL_LIST_PREV(pos), ppos = CSTL_LIST_PREV(ppos)) {\
		if (ppos != CSTL_LIST_NEXT(pos)) {\
			printf("3: prev[%p], p[%p], next[%p]\n", (void *) CSTL_LIST_PREV(pos), (void *) pos, (void *) CSTL_LIST_NEXT(pos));\
			return 0;\
		}\
	}\
	if (ppos != CSTL_LIST_NEXT(pos)) {\
		printf("4: prev[%p], p[%p], next[%p]\n", (void *) CSTL_LIST_PREV(pos), (void *) pos, (void *) CSTL_LIST_NEXT(pos));\
		return 0;\
	}\
	return 1;\
}\
\


#endif /* CSTL_LIST_DEBUG_H_INCLUDED */
