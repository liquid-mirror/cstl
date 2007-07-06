/* 
 * Copyright (c) 2006, KATO Noriaki
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*! 
 * \file vector.h
 * \brief vectorコンテナ
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2006-11-29
 * $URL$
 * $Id$
 */
#ifndef CSTL_VECTOR_H_INCLUDED
#define CSTL_VECTOR_H_INCLUDED

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#ifdef __cplusplus
#define CSTL_VECTOR_BEGIN_EXTERN_C()	extern "C" {
#define CSTL_VECTOR_END_EXTERN_C()		}
#else
#define CSTL_VECTOR_BEGIN_EXTERN_C()
#define CSTL_VECTOR_END_EXTERN_C()
#endif

#ifndef NDEBUG
#define CSTL_VECTOR_MAGIC(x) x
#else
#define CSTL_VECTOR_MAGIC(x)
#endif

#ifndef CSTL_ALGORITHM_IMPLEMENT
#define CSTL_ALGORITHM_IMPLEMENT(Name, Type, DIRECT_ACCESS)
#endif


#define CSTL_VECTOR_AT(self, idx)	self->buf[idx]
#define CSTL_VECTOR_SIZE(self)		self->end
#define CSTL_VECTOR_EMPTY(self)		(self->end == 0)
#define CSTL_VECTOR_CAPACITY(self)	self->nelems
#define CSTL_VECTOR_FULL(self)		(CSTL_VECTOR_SIZE(self) == CSTL_VECTOR_CAPACITY(self))
#define CSTL_VECTOR_CLEAR(self)		self->end = 0


/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_VECTOR_INTERFACE(Name, Type)	\
typedef struct Name##_t Name;\
\
CSTL_VECTOR_BEGIN_EXTERN_C()\
Name *Name##_new(size_t n);\
void Name##_delete(Name *self);\
int Name##_assign(Name *self, Name *x, size_t idx, size_t n);\
int Name##_push_back(Name *self, Type elem);\
Type Name##_pop_back(Name *self);\
size_t Name##_size(Name *self);\
size_t Name##_capacity(Name *self);\
int Name##_empty(Name *self);\
void Name##_clear(Name *self);\
int Name##_reserve(Name *self, size_t n);\
void Name##_shrink(Name *self, size_t n);\
int Name##_resize(Name *self, size_t n, Type elem);\
Type *Name##_at(Name *self, size_t idx);\
Type Name##_front(Name *self);\
Type Name##_back(Name *self);\
int Name##_insert(Name *self, size_t idx, Type elem);\
int Name##_insert_n(Name *self, size_t idx, size_t n, Type elem);\
int Name##_insert_array(Name *self, size_t idx, Type *elems, size_t n);\
int Name##_insert_range(Name *self, size_t idx, Name *x, size_t xidx, size_t n);\
void Name##_erase(Name *self, size_t idx, size_t n);\
void Name##_swap(Name *self, Name *x);\
void Name##_sort(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *));\
void Name##_stable_sort(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *));\
size_t Name##_binary_search(Name *self, size_t idx, size_t n, Type value, int (*comp)(const void *, const void *));\
size_t Name##_lower_bound(Name *self, size_t idx, size_t n, Type value, int (*comp)(const void *, const void *));\
size_t Name##_upper_bound(Name *self, size_t idx, size_t n, Type value, int (*comp)(const void *, const void *));\
void Name##_reverse(Name *self, size_t idx, size_t n);\
void Name##_rotate(Name *self, size_t first, size_t middle, size_t last);\
int Name##_merge(Name *self, size_t idx, Name *x, size_t xidx, size_t xn, Name *y, size_t yidx, size_t yn, int (*comp)(const void *, const void *));\
void Name##_inplace_merge(Name *self, size_t first, size_t middle, size_t last, int (*comp)(const void *, const void *));\
CSTL_VECTOR_END_EXTERN_C()\



#define CSTL_VECTOR_IMPLEMENT_BASE(Name, Type)	\
static int Name##_expand(Name *self, size_t size);\
static void Name##_move_forward(Name *self, size_t first, size_t last, size_t n);\
static void Name##_move_backward(Name *self, size_t first, size_t last, size_t n);\
/*! 
 * \brief vector構造体
 */\
struct Name##_t {\
	size_t end;\
	size_t nelems;\
	Type *buf;\
	CSTL_VECTOR_MAGIC(Name *magic);\
};\
\
Name *Name##_new(size_t n)\
{\
	Name *self;\
	Type *buf;\
	self = (Name *) malloc(sizeof(Name));\
	if (!self) return 0;\
	self->nelems = n;\
	if (!n) n = 1;\
	buf = (Type *) malloc(sizeof(Type) * n);\
	if (!buf) {\
		free(self);\
		return 0;\
	}\
	self->end = 0;\
	self->buf = buf;\
	CSTL_VECTOR_MAGIC(self->magic = self);\
	return self;\
}\
\
void Name##_delete(Name *self)\
{\
	assert(self && "Vector_delete");\
	assert(self->magic == self && "Vector_delete");\
	CSTL_VECTOR_MAGIC(self->magic = 0);\
	free(self->buf);\
	free(self);\
}\
\

#define CSTL_VECTOR_IMPLEMENT_ASSIGN(Name, Type)	\
int Name##_assign(Name *self, Name *x, size_t idx, size_t n)\
{\
	size_t i;\
	assert(self && "Vector_assign");\
	assert(self->magic == self && "Vector_assign");\
	assert(x && "Vector_assign");\
	assert(x->magic == x && "Vector_assign");\
	assert(CSTL_VECTOR_SIZE(x) >= idx + n && "Vector_assign");\
	assert(CSTL_VECTOR_SIZE(x) >= n && "Vector_assign");\
	assert(CSTL_VECTOR_SIZE(x) > idx && "Vector_assign");\
	if (self == x) {\
		if (CSTL_VECTOR_SIZE(self) > idx + n) {\
			Name##_erase(self, idx + n, CSTL_VECTOR_SIZE(self) - (idx + n));\
		}\
		if (!CSTL_VECTOR_EMPTY(self)) {\
			Name##_erase(self, 0, idx);\
		}\
	} else {\
		if (!Name##_expand(self, CSTL_VECTOR_SIZE(self) + n)) return 0;\
		CSTL_VECTOR_CLEAR(self);\
		for (i = 0; i < n; i++) {\
			Name##_push_back(self, CSTL_VECTOR_AT(x, i));\
		}\
	}\
	return 1;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_PUSH_BACK(Name, Type)	\
int Name##_push_back(Name *self, Type elem)\
{\
	assert(self && "Vector_push_back");\
	assert(self->magic == self && "Vector_push_back");\
	if (!Name##_expand(self, CSTL_VECTOR_SIZE(self) + 1)) return 0;\
	self->buf[self->end] = elem;\
	self->end++;\
	return 1;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_POP_BACK(Name, Type)	\
Type Name##_pop_back(Name *self)\
{\
	assert(self && "Vector_pop_back");\
	assert(self->magic == self && "Vector_pop_back");\
	assert(!CSTL_VECTOR_EMPTY(self) && "Vector_pop_back");\
	self->end--;\
	return self->buf[self->end];\
}\
\

#define CSTL_VECTOR_IMPLEMENT_SIZE(Name, Type)	\
size_t Name##_size(Name *self)\
{\
	assert(self && "Vector_size");\
	assert(self->magic == self && "Vector_size");\
	return CSTL_VECTOR_SIZE(self);\
}\
\

#define CSTL_VECTOR_IMPLEMENT_CAPACITY(Name, Type)	\
size_t Name##_capacity(Name *self)\
{\
	assert(self && "Vector_capacity");\
	assert(self->magic == self && "Vector_capacity");\
	return CSTL_VECTOR_CAPACITY(self);\
}\
\

#define CSTL_VECTOR_IMPLEMENT_EMPTY(Name, Type)	\
int Name##_empty(Name *self)\
{\
	assert(self && "Vector_empty");\
	assert(self->magic == self && "Vector_empty");\
	return CSTL_VECTOR_EMPTY(self);\
}\
\

#define CSTL_VECTOR_IMPLEMENT_CLEAR(Name, Type)	\
void Name##_clear(Name *self)\
{\
	assert(self && "Vector_clear");\
	assert(self->magic == self && "Vector_clear");\
	CSTL_VECTOR_CLEAR(self);\
}\
\

#define CSTL_VECTOR_IMPLEMENT_RESERVE(Name, Type)	\
static int Name##_expand(Name *self, size_t size)\
{\
	size_t n;\
	if (size <= CSTL_VECTOR_CAPACITY(self)) return 1;\
	n = (size > CSTL_VECTOR_CAPACITY(self) * 2) ? size : CSTL_VECTOR_CAPACITY(self) * 2;\
	return Name##_reserve(self, n);\
}\
\
int Name##_reserve(Name *self, size_t n)\
{\
	Type *newbuf;\
	assert(self && "Vector_reserve");\
	assert(self->magic == self && "Vector_reserve");\
	if (n <= CSTL_VECTOR_CAPACITY(self)) return 1;\
	newbuf = (Type *) realloc(self->buf, sizeof(Type) * n);\
	if (!newbuf) return 0;\
	self->buf = newbuf;\
	self->nelems = n;\
	return 1;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_SHRINK(Name, Type)	\
void Name##_shrink(Name *self, size_t n)\
{\
	Type *newbuf;\
	assert(self && "Vector_shrink");\
	assert(self->magic == self && "Vector_shrink");\
	if (n >= CSTL_VECTOR_CAPACITY(self)) return;\
	if (n < CSTL_VECTOR_SIZE(self)) {\
		n = CSTL_VECTOR_SIZE(self);\
	}\
	self->nelems = n;\
	if (!n) n = 1;\
	newbuf = (Type *) realloc(self->buf, sizeof(Type) * n);\
	if (newbuf) {\
		self->buf = newbuf;\
	}\
}\
\

#define CSTL_VECTOR_IMPLEMENT_RESIZE(Name, Type)	\
int Name##_resize(Name *self, size_t n, Type elem)\
{\
	size_t size;\
	assert(self && "Vector_resize");\
	assert(self->magic == self && "Vector_resize");\
	size = CSTL_VECTOR_SIZE(self);\
	if (size >= n) {\
		self->end = n;\
	} else {\
		size_t i;\
		if (!Name##_expand(self, n)) {\
			return 0;\
		}\
		for (i = 0; i < n - size; i++) {\
			Name##_push_back(self, elem);\
		}\
	}\
	return 1;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_AT(Name, Type)	\
Type *Name##_at(Name *self, size_t idx)\
{\
	assert(self && "Vector_at");\
	assert(self->magic == self && "Vector_at");\
	assert(CSTL_VECTOR_SIZE(self) > idx && "Vector_at");\
	return &CSTL_VECTOR_AT(self, idx);\
}\
\

#define CSTL_VECTOR_IMPLEMENT_FRONT(Name, Type)	\
Type Name##_front(Name *self)\
{\
	assert(self && "Vector_front");\
	assert(self->magic == self && "Vector_front");\
	assert(!CSTL_VECTOR_EMPTY(self) && "Vector_front");\
	return self->buf[0];\
}\
\

#define CSTL_VECTOR_IMPLEMENT_BACK(Name, Type)	\
Type Name##_back(Name *self)\
{\
	assert(self && "Vector_back");\
	assert(self->magic == self && "Vector_back");\
	assert(!CSTL_VECTOR_EMPTY(self) && "Vector_back");\
	return self->buf[self->end - 1];\
}\
\

#define CSTL_VECTOR_IMPLEMENT_MOVE_FORWARD(Name, Type)	\
static void Name##_move_forward(Name *self, size_t first, size_t last, size_t n)\
{\
	size_t i;\
	for (i = last; i > first; i--) {\
		self->buf[i - 1 + n] = self->buf[i - 1];\
	}\
}\
\

#define CSTL_VECTOR_IMPLEMENT_MOVE_BACKWARD(Name, Type)	\
static void Name##_move_backward(Name *self, size_t first, size_t last, size_t n)\
{\
	size_t i;\
	for (i = first; i < last; i++) {\
		self->buf[i - n] = self->buf[i];\
	}\
}\
\

#define CSTL_VECTOR_IMPLEMENT_INSERT_N_NO_ELEM(Name, Type)	\
static int Name##_insert_n_no_elem(Name *self, size_t idx, size_t n)\
{\
	if (!Name##_expand(self, CSTL_VECTOR_SIZE(self) + n)) return 0;\
	Name##_move_forward(self, idx, self->end, n);\
	self->end += n;\
	return 1;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_INSERT(Name, Type)	\
CSTL_VECTOR_IMPLEMENT_INSERT_N_NO_ELEM(Name, Type)\
\
int Name##_insert(Name *self, size_t idx, Type elem)\
{\
	assert(self && "Vector_insert");\
	assert(self->magic == self && "Vector_insert");\
	assert(CSTL_VECTOR_SIZE(self) >= idx && "Vector_insert");\
	return Name##_insert_array(self, idx, &elem, 1);\
}\
\
int Name##_insert_n(Name *self, size_t idx, size_t n, Type elem)\
{\
	size_t i;\
	assert(self && "Vector_insert_n");\
	assert(self->magic == self && "Vector_insert_n");\
	assert(CSTL_VECTOR_SIZE(self) >= idx && "Vector_insert_n");\
	if (!Name##_insert_n_no_elem(self, idx, n)) {\
		return 0;\
	}\
	for (i = 0; i < n; i++) {\
		self->buf[idx + i] = elem;\
	}\
	return 1;\
}\
\
int Name##_insert_array(Name *self, size_t idx, Type *elems, size_t n)\
{\
	size_t i;\
	assert(self && "Vector_insert_array");\
	assert(self->magic == self && "Vector_insert_array");\
	assert(CSTL_VECTOR_SIZE(self) >= idx && "Vector_insert_array");\
	assert(elems && "Vector_insert_array");\
	if (!Name##_insert_n_no_elem(self, idx, n)) {\
		return 0;\
	}\
	for (i = 0; i < n; i++) {\
		self->buf[idx + i] = elems[i];\
	}\
	return 1;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_INSERT_RANGE(Name, Type)	\
int Name##_insert_range(Name *self, size_t idx, Name *x, size_t xidx, size_t n)\
{\
	size_t i;\
	assert(self && "Vector_insert_range");\
	assert(self->magic == self && "Vector_insert_range");\
	assert(CSTL_VECTOR_SIZE(self) >= idx && "Vector_insert_range");\
	assert(x && "Vector_insert_range");\
	assert(x->magic == x && "Vector_insert_range");\
	assert(CSTL_VECTOR_SIZE(x) >= xidx + n && "Vector_insert_range");\
	assert(CSTL_VECTOR_SIZE(x) >= n && "Vector_insert_range");\
	assert(CSTL_VECTOR_SIZE(x) > xidx && "Vector_insert_range");\
	if (!Name##_insert_n_no_elem(self, idx, n)) {\
		return 0;\
	}\
	if (self == x) {\
		if (idx <= xidx) {\
			for (i = 0; i < n; i++) {\
				CSTL_VECTOR_AT(self, idx + i) = CSTL_VECTOR_AT(self, xidx + n + i);\
			}\
		} else if (xidx < idx && idx < xidx + n) {\
			for (i = 0; i < idx - xidx; i++) {\
				CSTL_VECTOR_AT(self, idx + i) = CSTL_VECTOR_AT(self, xidx + i);\
			}\
			for (i = 0; i < n - (idx - xidx); i++) {\
				CSTL_VECTOR_AT(self, idx + (idx - xidx) + i) = CSTL_VECTOR_AT(self, idx + n + i);\
			}\
		} else {\
			for (i = 0; i < n; i++) {\
				CSTL_VECTOR_AT(self, idx + i) = CSTL_VECTOR_AT(self, xidx + i);\
			}\
		}\
	} else {\
		for (i = 0; i < n; i++) {\
			CSTL_VECTOR_AT(self, idx + i) = CSTL_VECTOR_AT(x, xidx + i);\
		}\
	}\
	return 1;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_ERASE(Name, Type)	\
void Name##_erase(Name *self, size_t idx, size_t n)\
{\
	assert(self && "Vector_erase");\
	assert(self->magic == self && "Vector_erase");\
	assert(CSTL_VECTOR_SIZE(self) >= idx + n && "Vector_erase");\
	assert(CSTL_VECTOR_SIZE(self) >= n && "Vector_erase");\
	assert(CSTL_VECTOR_SIZE(self) > idx && "Vector_erase");\
	Name##_move_backward(self, idx + n, self->end, n);\
	self->end -= n;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_SWAP(Name, Type)	\
void Name##_swap(Name *self, Name *x)\
{\
	size_t tmp_end;\
	size_t tmp_nelems;\
	Type *tmp_buf;\
	assert(self && "Vector_swap");\
	assert(x && "Vector_swap");\
	assert(self->magic == self && "Vector_swap");\
	assert(x->magic == x && "Vector_swap");\
	tmp_end = self->end;\
	tmp_nelems = self->nelems;\
	tmp_buf = self->buf;\
	self->end = x->end;\
	self->nelems = x->nelems;\
	self->buf = x->buf;\
	x->end = tmp_end;\
	x->nelems = tmp_nelems;\
	x->buf = tmp_buf;\
}\
\


/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_VECTOR_IMPLEMENT(Name, Type)	\
CSTL_VECTOR_IMPLEMENT_BASE(Name, Type)\
CSTL_VECTOR_IMPLEMENT_ASSIGN(Name, Type)\
CSTL_VECTOR_IMPLEMENT_PUSH_BACK(Name, Type)\
CSTL_VECTOR_IMPLEMENT_POP_BACK(Name, Type)\
CSTL_VECTOR_IMPLEMENT_SIZE(Name, Type)\
CSTL_VECTOR_IMPLEMENT_CAPACITY(Name, Type)\
CSTL_VECTOR_IMPLEMENT_EMPTY(Name, Type)\
CSTL_VECTOR_IMPLEMENT_CLEAR(Name, Type)\
CSTL_VECTOR_IMPLEMENT_RESERVE(Name, Type)\
CSTL_VECTOR_IMPLEMENT_SHRINK(Name, Type)\
CSTL_VECTOR_IMPLEMENT_RESIZE(Name, Type)\
CSTL_VECTOR_IMPLEMENT_AT(Name, Type)\
CSTL_VECTOR_IMPLEMENT_FRONT(Name, Type)\
CSTL_VECTOR_IMPLEMENT_BACK(Name, Type)\
CSTL_VECTOR_IMPLEMENT_MOVE_FORWARD(Name, Type)\
CSTL_VECTOR_IMPLEMENT_MOVE_BACKWARD(Name, Type)\
CSTL_VECTOR_IMPLEMENT_INSERT(Name, Type)\
CSTL_VECTOR_IMPLEMENT_INSERT_RANGE(Name, Type)\
CSTL_VECTOR_IMPLEMENT_ERASE(Name, Type)\
CSTL_VECTOR_IMPLEMENT_SWAP(Name, Type)\
CSTL_ALGORITHM_IMPLEMENT(Name, Type, CSTL_VECTOR_AT)\


#endif /* CSTL_VECTOR_H_INCLUDED */
