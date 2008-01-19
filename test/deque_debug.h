#ifndef CSTL_DEQUE_DEBUG_H_INCLUDED
#define CSTL_DEQUE_DEBUG_H_INCLUDED

#include <stdio.h>


#define CSTL_DEQUE_DEBUG_INTERFACE(Name, Type)	\
int Name##_verify(Name *self);\
void Name##_print(Name *self);\


#define CSTL_DEQUE_DEBUG_IMPLEMENT(Name, Type, format)	\
int Name##_verify(Name *self)\
{\
	size_t i;\
	size_t count = 0;\
	if (!self->map || !self->pool) {\
		return 0;\
	}\
	if (CSTL_VECTOR_CAPACITY(self->map) < CSTL_DEQUE_INITIAL_MAP_SIZE) {\
		return 0;\
	}\
	if (CSTL_VECTOR_CAPACITY(self->pool) != CSTL_DEQUE_INITIAL_MAP_SIZE) {\
		return 0;\
	}\
\
	if (self->begin >= self->end) {\
		return 0;\
	}\
	if (self->begin > CSTL_VECTOR_SIZE(self->map)) {\
		return 0;\
	}\
	if (self->end > CSTL_VECTOR_SIZE(self->map)) {\
		return 0;\
	}\
\
	for (i = 0; i < self->begin; i++) {\
		if (CSTL_VECTOR_AT(self->map, i)) {\
			return 0;\
		}\
	}\
	for (i = self->begin; i < self->end; i++) {\
		if (!CSTL_VECTOR_AT(self->map, i)) {\
			return 0;\
		}\
	}\
	for (i = self->end; i < CSTL_VECTOR_SIZE(self->map); i++) {\
		if (CSTL_VECTOR_AT(self->map, i)) {\
			return 0;\
		}\
	}\
	for (i = 0; i < CSTL_VECTOR_SIZE(self->pool); i++) {\
		if (!CSTL_VECTOR_AT(self->pool, i)) {\
			return 0;\
		}\
	}\
\
	for (i = self->begin; i < self->end; i++) {\
		if (CSTL_RING_SIZE(CSTL_VECTOR_AT(self->map, i)) > CSTL_DEQUE_RINGBUF_SIZE(Type)) {\
			return 0;\
		}\
		count += CSTL_RING_SIZE(CSTL_VECTOR_AT(self->map, i));\
	}\
	if (count != self->nelems) {\
		return 0;\
	}\
\
	return 1;\
}\
\
void Name##_print(Name *self)\
{\
	size_t i, j;\
	printf("begin[%d], end[%d], map_size[%d], nelems[%d]\n", self->begin, self->end, CSTL_VECTOR_SIZE(self->map), self->nelems);\
	printf("     ");\
	for (i = 0; i < CSTL_VECTOR_SIZE(self->map); i++) {\
		printf("%8d ", i);\
	}\
	printf("\n");\
	printf("map :");\
	for (i = 0; i < CSTL_VECTOR_SIZE(self->map); i++) {\
		printf("%8p ", (void *) CSTL_VECTOR_AT(self->map, i));\
	}\
	printf("\n");\
	printf("pool:");\
	for (i = 0; i < CSTL_VECTOR_SIZE(self->pool); i++) {\
		printf("%8p ", (void *) CSTL_VECTOR_AT(self->pool, i));\
	}\
	printf("\n");\
	for (i = self->begin; i < self->end; i++) {\
		printf("%p: size[%d]\n", (void *) CSTL_VECTOR_AT(self->map, i), CSTL_RING_SIZE(CSTL_VECTOR_AT(self->map, i)));\
		for (j = 0; j < CSTL_RING_SIZE(CSTL_VECTOR_AT(self->map, i)); j++) {\
			printf(#format" ", CSTL_RING_AT(CSTL_VECTOR_AT(self->map, i), j));\
		}\
		printf("\n");\
	}\
}\
\

#endif /* CSTL_DEQUE_DEBUG_H_INCLUDED */
