/* 
 * Copyright (c) 2006-2009, KATO Noriaki
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

#ifndef CSTL_ALGORITHM_INTERFACE
#define CSTL_ALGORITHM_INTERFACE(Name, Type)
#endif

#ifndef CSTL_ALGORITHM_IMPLEMENT
#define CSTL_ALGORITHM_IMPLEMENT(Name, Type, DIRECT_ACCESS)
#endif


#define CSTL_VECTOR_AT(self, idx)	(self)->buf[(idx)]
#define CSTL_VECTOR_SIZE(self)		(self)->size
#define CSTL_VECTOR_EMPTY(self)		((self)->size == 0)
#define CSTL_VECTOR_CAPACITY(self)	(self)->capacity
#define CSTL_VECTOR_FULL(self)		(CSTL_VECTOR_SIZE((self)) == CSTL_VECTOR_CAPACITY((self)))
#define CSTL_VECTOR_CLEAR(self)		do { (self)->size = 0; } while (0)


/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_VECTOR_INTERFACE(Name, Type)	\
typedef struct Name Name;\
\
CSTL_VECTOR_BEGIN_EXTERN_C()\
Name *Name##_new(void);\
Name *Name##_new_reserve(size_t n);\
void Name##_delete(Name *self);\
int Name##_push_back(Name *self, Type data);\
void Name##_pop_back(Name *self);\
size_t Name##_size(Name *self);\
size_t Name##_capacity(Name *self);\
int Name##_empty(Name *self);\
void Name##_clear(Name *self);\
int Name##_reserve(Name *self, size_t n);\
void Name##_shrink(Name *self, size_t n);\
int Name##_resize(Name *self, size_t n, Type data);\
Type *Name##_at(Name *self, size_t idx);\
Type *Name##_front(Name *self);\
Type *Name##_back(Name *self);\
int Name##_insert(Name *self, size_t idx, Type data);\
int Name##_insert_n(Name *self, size_t idx, size_t n, Type data);\
int Name##_insert_array(Name *self, size_t idx, Type const *data, size_t n);\
int Name##_insert_range(Name *self, size_t idx, Name *x, size_t xidx, size_t n);\
void Name##_erase(Name *self, size_t idx, size_t n);\
void Name##_swap(Name *self, Name *x);\
CSTL_ALGORITHM_INTERFACE(Name, Type)\
CSTL_VECTOR_END_EXTERN_C()\



#define CSTL_VECTOR_IMPLEMENT_BASE(Name, Type)	\
static int Name##_expand(Name *self, size_t size);\
static void Name##_move_forward(Name *self, size_t first, size_t last, size_t n);\
static void Name##_move_backward(Name *self, size_t first, size_t last, size_t n);\
static int Name##_insert_n_no_data(Name *self, size_t idx, size_t n);\
/*! \
 * \brief vector構造体\
 */\
struct Name {\
	size_t size;\
	size_t capacity;\
	Type *buf;\
	CSTL_VECTOR_MAGIC(Name *magic;)\
};\
\
Name *Name##_new(void)\
{\
	Name *self;\
	self = (Name *) malloc(sizeof(Name));\
	if (!self) return 0;\
	self->capacity = 0;\
	self->size = 0;\
	self->buf = 0;\
	CSTL_VECTOR_MAGIC(self->magic = self);\
	return self;\
}\
\
Name *Name##_new_reserve(size_t n)\
{\
	Name *self;\
	self = Name##_new();\
	if (!self) return 0;\
	if (!Name##_reserve(self, n)) {\
		Name##_delete(self);\
		return 0;\
	}\
	return self;\
}\
\
void Name##_delete(Name *self)\
{\
	if (!self) return;\
	assert(self->magic == self && "Vector_delete");\
	CSTL_VECTOR_MAGIC(self->magic = 0);\
	free(self->buf);\
	free(self);\
}\
\


#define CSTL_VECTOR_IMPLEMENT_PUSH_BACK(Name, Type)	\
int Name##_push_back(Name *self, Type data)\
{\
	assert(self && "Vector_push_back");\
	assert(self->magic == self && "Vector_push_back");\
	if (!Name##_expand(self, CSTL_VECTOR_SIZE(self) + 1)) return 0;\
	self->buf[self->size] = data;\
	self->size++;\
	return 1;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_POP_BACK(Name, Type)	\
void Name##_pop_back(Name *self)\
{\
	assert(self && "Vector_pop_back");\
	assert(self->magic == self && "Vector_pop_back");\
	assert(!CSTL_VECTOR_EMPTY(self) && "Vector_pop_back");\
	self->size--;\
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
	self->capacity = n;\
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
	self->capacity = n;\
	newbuf = (Type *) realloc(self->buf, sizeof(Type) * n);\
	if (!n) {\
		/* nが0ならreallocはfreeと等価 */\
		self->buf = 0;\
	} else if (newbuf) {\
		self->buf = newbuf;\
	}\
}\
\

#define CSTL_VECTOR_IMPLEMENT_RESIZE(Name, Type)	\
int Name##_resize(Name *self, size_t n, Type data)\
{\
	size_t size;\
	assert(self && "Vector_resize");\
	assert(self->magic == self && "Vector_resize");\
	size = CSTL_VECTOR_SIZE(self);\
	if (size >= n) {\
		self->size = n;\
	} else {\
		register size_t i;\
		if (!Name##_insert_n_no_data(self, size, n - size)) {\
			return 0;\
		}\
		for (i = size; i < n; i++) {\
			CSTL_VECTOR_AT(self, i) = data;\
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
Type *Name##_front(Name *self)\
{\
	assert(self && "Vector_front");\
	assert(self->magic == self && "Vector_front");\
	assert(!CSTL_VECTOR_EMPTY(self) && "Vector_front");\
	return &self->buf[0];\
}\
\

#define CSTL_VECTOR_IMPLEMENT_BACK(Name, Type)	\
Type *Name##_back(Name *self)\
{\
	assert(self && "Vector_back");\
	assert(self->magic == self && "Vector_back");\
	assert(!CSTL_VECTOR_EMPTY(self) && "Vector_back");\
	return &self->buf[self->size - 1];\
}\
\

#define CSTL_VECTOR_IMPLEMENT_MOVE_BACKWARD(Name, Type)	\
static void Name##_move_backward(Name *self, size_t first, size_t last, size_t n)\
{\
	register size_t i;\
	for (i = last; i > first; i--) {\
		self->buf[i - 1 + n] = self->buf[i - 1];\
	}\
}\
\

#define CSTL_VECTOR_IMPLEMENT_MOVE_FORWARD(Name, Type)	\
static void Name##_move_forward(Name *self, size_t first, size_t last, size_t n)\
{\
	register size_t i;\
	for (i = first; i < last; i++) {\
		self->buf[i - n] = self->buf[i];\
	}\
}\
\

#define CSTL_VECTOR_IMPLEMENT_INSERT_N_NO_DATA(Name, Type)	\
static int Name##_insert_n_no_data(Name *self, size_t idx, size_t n)\
{\
	if (!Name##_expand(self, CSTL_VECTOR_SIZE(self) + n)) return 0;\
	Name##_move_backward(self, idx, self->size, n);\
	self->size += n;\
	return 1;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_INSERT(Name, Type)	\
int Name##_insert(Name *self, size_t idx, Type data)\
{\
	assert(self && "Vector_insert");\
	assert(self->magic == self && "Vector_insert");\
	assert(CSTL_VECTOR_SIZE(self) >= idx && "Vector_insert");\
	return Name##_insert_array(self, idx, &data, 1);\
}\
\

#define CSTL_VECTOR_IMPLEMENT_INSERT_N(Name, Type)	\
int Name##_insert_n(Name *self, size_t idx, size_t n, Type data)\
{\
	register size_t i;\
	assert(self && "Vector_insert_n");\
	assert(self->magic == self && "Vector_insert_n");\
	assert(CSTL_VECTOR_SIZE(self) >= idx && "Vector_insert_n");\
	if (!Name##_insert_n_no_data(self, idx, n)) {\
		return 0;\
	}\
	for (i = 0; i < n; i++) {\
		self->buf[idx + i] = data;\
	}\
	return 1;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_INSERT_ARRAY(Name, Type)	\
int Name##_insert_array(Name *self, size_t idx, Type const *data, size_t n)\
{\
	register size_t i;\
	assert(self && "Vector_insert_array");\
	assert(self->magic == self && "Vector_insert_array");\
	assert(CSTL_VECTOR_SIZE(self) >= idx && "Vector_insert_array");\
	assert(data && "Vector_insert_array");\
	if (!Name##_insert_n_no_data(self, idx, n)) {\
		return 0;\
	}\
	for (i = 0; i < n; i++) {\
		self->buf[idx + i] = data[i];\
	}\
	return 1;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_INSERT_RANGE(Name, Type)	\
int Name##_insert_range(Name *self, size_t idx, Name *x, size_t xidx, size_t n)\
{\
	register size_t i;\
	assert(self && "Vector_insert_range");\
	assert(self->magic == self && "Vector_insert_range");\
	assert(CSTL_VECTOR_SIZE(self) >= idx && "Vector_insert_range");\
	assert(x && "Vector_insert_range");\
	assert(x->magic == x && "Vector_insert_range");\
	assert(CSTL_VECTOR_SIZE(x) >= xidx + n && "Vector_insert_range");\
	assert(CSTL_VECTOR_SIZE(x) >= n && "Vector_insert_range");\
	assert(CSTL_VECTOR_SIZE(x) > xidx && "Vector_insert_range");\
	if (!Name##_insert_n_no_data(self, idx, n)) {\
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
	Name##_move_forward(self, idx + n, self->size, n);\
	self->size -= n;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_SWAP(Name, Type)	\
void Name##_swap(Name *self, Name *x)\
{\
	size_t tmp_size;\
	size_t tmp_capacity;\
	Type *tmp_buf;\
	assert(self && "Vector_swap");\
	assert(x && "Vector_swap");\
	assert(self->magic == self && "Vector_swap");\
	assert(x->magic == x && "Vector_swap");\
	tmp_size = self->size;\
	tmp_capacity = self->capacity;\
	tmp_buf = self->buf;\
	self->size = x->size;\
	self->capacity = x->capacity;\
	self->buf = x->buf;\
	x->size = tmp_size;\
	x->capacity = tmp_capacity;\
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
CSTL_VECTOR_IMPLEMENT_INSERT_N_NO_DATA(Name, Type)\
CSTL_VECTOR_IMPLEMENT_INSERT(Name, Type)\
CSTL_VECTOR_IMPLEMENT_INSERT_N(Name, Type)\
CSTL_VECTOR_IMPLEMENT_INSERT_ARRAY(Name, Type)\
CSTL_VECTOR_IMPLEMENT_INSERT_RANGE(Name, Type)\
CSTL_VECTOR_IMPLEMENT_ERASE(Name, Type)\
CSTL_VECTOR_IMPLEMENT_SWAP(Name, Type)\
CSTL_ALGORITHM_IMPLEMENT(Name, Type, CSTL_VECTOR_AT)\


#endif /* CSTL_VECTOR_H_INCLUDED */
