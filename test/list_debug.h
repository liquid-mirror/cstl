#ifndef CSTL_LIST_DEBUG_H_INCLUDED
#define CSTL_LIST_DEBUG_H_INCLUDED

#include <stdio.h>


#define CSTL_LIST_DEBUG_INTERFACE(Name)	\
int Name##_verify(Name *self);\


#define CSTL_LIST_DEBUG_IMPLEMENT(Name)	\
int Name##_verify(Name *self)\
{\
	Name##Iterator pos;\
	Name##Iterator ppos;\
	for (ppos = Name##_end(self), pos = Name##_begin(self); pos != Name##_end(self); pos = Name##_next(pos), ppos = Name##_next(ppos)) {\
		if (ppos != Name##_prev(pos)) {\
			printf("1: prev[%p], p[%p], next[%p]\n", (void *) Name##_prev(pos), (void *) pos, (void *) Name##_next(pos));\
			return 0;\
		}\
	}\
	if (ppos != Name##_prev(pos)) {\
		printf("2: prev[%p], p[%p], next[%p]\n", (void *) Name##_prev(pos), (void *) pos, (void *) Name##_next(pos));\
		return 0;\
	}\
	for (ppos = Name##_rend(self), pos = Name##_rbegin(self); pos != Name##_rend(self); pos = Name##_prev(pos), ppos = Name##_prev(ppos)) {\
		if (ppos != Name##_next(pos)) {\
			printf("3: prev[%p], p[%p], next[%p]\n", (void *) Name##_prev(pos), (void *) pos, (void *) Name##_next(pos));\
			return 0;\
		}\
	}\
	if (ppos != Name##_next(pos)) {\
		printf("4: prev[%p], p[%p], next[%p]\n", (void *) Name##_prev(pos), (void *) pos, (void *) Name##_next(pos));\
		return 0;\
	}\
	return 1;\
}\
\


#endif /* CSTL_LIST_DEBUG_H_INCLUDED */
