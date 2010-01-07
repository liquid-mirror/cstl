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
#include <string.h>
#include "common.h"


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
CSTL_EXTERN_C_BEGIN()\
Name *Name##_new(void);\
Name *Name##_new_reserve(size_t n);\
void Name##_delete(Name *self);\
int Name##_push_back(Name *self, Type data);\
int Name##_push_back_ref(Name *self, Type const *data);\
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
int Name##_insert_ref(Name *self, size_t idx, Type const *data);\
int Name##_insert_n(Name *self, size_t idx, size_t n, Type data);\
int Name##_insert_n_ref(Name *self, size_t idx, size_t n, Type const *data);\
int Name##_insert_array(Name *self, size_t idx, Type const *data, size_t n);\
int Name##_insert_range(Name *self, size_t idx, Name *x, size_t xidx, size_t n);\
void Name##_erase(Name *self, size_t idx, size_t n);\
void Name##_swap(Name *self, Name *x);\
CSTL_ALGORITHM_INTERFACE(Name, Type)\
CSTL_EXTERN_C_END()\



#define CSTL_VECTOR_IMPLEMENT_BASE(Name, Type)	\
/*! \
 * \brief vector構造体\
 */\
struct Name {\
	size_t size;\
	size_t capacity;\
	Type *buf;\
	CSTL_MAGIC(Name *magic;)\
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
	CSTL_MAGIC(self->magic = self);\
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
	CSTL_ASSERT(self->magic == self && "Vector_delete");\
	CSTL_MAGIC(self->magic = 0);\
	free(self->buf);\
	free(self);\
}\
\


#define CSTL_VECTOR_IMPLEMENT_PUSH_BACK(Name, Type)	\
int Name##_push_back(Name *self, Type data)\
{\
	CSTL_ASSERT(self && "Vector_push_back");\
	CSTL_ASSERT(self->magic == self && "Vector_push_back");\
	return Name##_push_back_ref(self, &data);\
}\
\
int Name##_push_back_ref(Name *self, Type const *data)\
{\
	CSTL_ASSERT(self && "Vector_push_back_ref");\
	CSTL_ASSERT(self->magic == self && "Vector_push_back_ref");\
	CSTL_ASSERT(data && "Vector_push_back_ref");\
	if (!Name##_expand(self, CSTL_VECTOR_SIZE(self) + 1)) return 0;\
	self->buf[self->size] = *data;\
	self->size++;\
	return 1;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_POP_BACK(Name, Type)	\
void Name##_pop_back(Name *self)\
{\
	CSTL_ASSERT(self && "Vector_pop_back");\
	CSTL_ASSERT(self->magic == self && "Vector_pop_back");\
	CSTL_ASSERT(!CSTL_VECTOR_EMPTY(self) && "Vector_pop_back");\
	self->size--;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_SIZE(Name, Type)	\
size_t Name##_size(Name *self)\
{\
	CSTL_ASSERT(self && "Vector_size");\
	CSTL_ASSERT(self->magic == self && "Vector_size");\
	return CSTL_VECTOR_SIZE(self);\
}\
\

#define CSTL_VECTOR_IMPLEMENT_CAPACITY(Name, Type)	\
size_t Name##_capacity(Name *self)\
{\
	CSTL_ASSERT(self && "Vector_capacity");\
	CSTL_ASSERT(self->magic == self && "Vector_capacity");\
	return CSTL_VECTOR_CAPACITY(self);\
}\
\

#define CSTL_VECTOR_IMPLEMENT_EMPTY(Name, Type)	\
int Name##_empty(Name *self)\
{\
	CSTL_ASSERT(self && "Vector_empty");\
	CSTL_ASSERT(self->magic == self && "Vector_empty");\
	return CSTL_VECTOR_EMPTY(self);\
}\
\

#define CSTL_VECTOR_IMPLEMENT_CLEAR(Name, Type)	\
void Name##_clear(Name *self)\
{\
	CSTL_ASSERT(self && "Vector_clear");\
	CSTL_ASSERT(self->magic == self && "Vector_clear");\
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
	CSTL_ASSERT(self && "Vector_reserve");\
	CSTL_ASSERT(self->magic == self && "Vector_reserve");\
	if (n <= CSTL_VECTOR_CAPACITY(self)) return 1;\
	if (n > ((size_t) -1) / sizeof(Type)) {\
		/* sizeof(Type)*n がオーバーフローする */\
		return 0;\
	}\
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
	CSTL_ASSERT(self && "Vector_shrink");\
	CSTL_ASSERT(self->magic == self && "Vector_shrink");\
	if (n >= CSTL_VECTOR_CAPACITY(self)) return;\
	if (n < CSTL_VECTOR_SIZE(self)) {\
		n = CSTL_VECTOR_SIZE(self);\
	}\
	self->capacity = n;\
	if (!n) {\
		free(self->buf);\
		self->buf = 0;\
		return;\
	}\
	newbuf = (Type *) realloc(self->buf, sizeof(Type) * n);\
	if (newbuf) {\
		self->buf = newbuf;\
	}\
}\
\

#define CSTL_VECTOR_IMPLEMENT_RESIZE(Name, Type)	\
int Name##_resize(Name *self, size_t n, Type data)\
{\
	size_t size;\
	CSTL_ASSERT(self && "Vector_resize");\
	CSTL_ASSERT(self->magic == self && "Vector_resize");\
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
	CSTL_ASSERT(self && "Vector_at");\
	CSTL_ASSERT(self->magic == self && "Vector_at");\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) > idx && "Vector_at");\
	return &CSTL_VECTOR_AT(self, idx);\
}\
\

#define CSTL_VECTOR_IMPLEMENT_FRONT(Name, Type)	\
Type *Name##_front(Name *self)\
{\
	CSTL_ASSERT(self && "Vector_front");\
	CSTL_ASSERT(self->magic == self && "Vector_front");\
	CSTL_ASSERT(!CSTL_VECTOR_EMPTY(self) && "Vector_front");\
	return &self->buf[0];\
}\
\

#define CSTL_VECTOR_IMPLEMENT_BACK(Name, Type)	\
Type *Name##_back(Name *self)\
{\
	CSTL_ASSERT(self && "Vector_back");\
	CSTL_ASSERT(self->magic == self && "Vector_back");\
	CSTL_ASSERT(!CSTL_VECTOR_EMPTY(self) && "Vector_back");\
	return &self->buf[self->size - 1];\
}\
\

#define CSTL_VECTOR_IMPLEMENT_MOVE_BACKWARD(Name, Type)	\
static void Name##_move_backward(Name *self, size_t first, size_t last, size_t n)\
{\
	memmove(&self->buf[first + n], &self->buf[first], sizeof(Type) * (last - first));\
}\
\

#define CSTL_VECTOR_IMPLEMENT_MOVE_FORWARD(Name, Type)	\
static void Name##_move_forward(Name *self, size_t first, size_t last, size_t n)\
{\
	memmove(&self->buf[first - n], &self->buf[first], sizeof(Type) * (last - first));\
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
	CSTL_ASSERT(self && "Vector_insert");\
	CSTL_ASSERT(self->magic == self && "Vector_insert");\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) >= idx && "Vector_insert");\
	return Name##_insert_n_ref(self, idx, 1, &data);\
}\
\
int Name##_insert_ref(Name *self, size_t idx, Type const *data)\
{\
	CSTL_ASSERT(self && "Vector_insert_ref");\
	CSTL_ASSERT(self->magic == self && "Vector_insert_ref");\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) >= idx && "Vector_insert_ref");\
	CSTL_ASSERT(data && "Vector_insert_ref");\
	return Name##_insert_n_ref(self, idx, 1, data);\
}\
\

#define CSTL_VECTOR_IMPLEMENT_INSERT_N(Name, Type)	\
int Name##_insert_n(Name *self, size_t idx, size_t n, Type data)\
{\
	CSTL_ASSERT(self && "Vector_insert_n");\
	CSTL_ASSERT(self->magic == self && "Vector_insert_n");\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) >= idx && "Vector_insert_n");\
	return Name##_insert_n_ref(self, idx, n, &data);\
}\
\
int Name##_insert_n_ref(Name *self, size_t idx, size_t n, Type const *data)\
{\
	register size_t i;\
	CSTL_ASSERT(self && "Vector_insert_n_ref");\
	CSTL_ASSERT(self->magic == self && "Vector_insert_n_ref");\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) >= idx && "Vector_insert_n_ref");\
	CSTL_ASSERT(data && "Vector_insert_n_ref");\
	if (!Name##_insert_n_no_data(self, idx, n)) {\
		return 0;\
	}\
	for (i = 0; i < n; i++) {\
		self->buf[idx + i] = *data;\
	}\
	return 1;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_INSERT_ARRAY(Name, Type)	\
int Name##_insert_array(Name *self, size_t idx, Type const *data, size_t n)\
{\
	CSTL_ASSERT(self && "Vector_insert_array");\
	CSTL_ASSERT(self->magic == self && "Vector_insert_array");\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) >= idx && "Vector_insert_array");\
	CSTL_ASSERT(data && "Vector_insert_array");\
	if (!Name##_insert_n_no_data(self, idx, n)) {\
		return 0;\
	}\
	memcpy(&self->buf[idx], data, sizeof(Type) * n);\
	return 1;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_INSERT_RANGE(Name, Type)	\
int Name##_insert_range(Name *self, size_t idx, Name *x, size_t xidx, size_t n)\
{\
	CSTL_ASSERT(self && "Vector_insert_range");\
	CSTL_ASSERT(self->magic == self && "Vector_insert_range");\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) >= idx && "Vector_insert_range");\
	CSTL_ASSERT(x && "Vector_insert_range");\
	CSTL_ASSERT(x->magic == x && "Vector_insert_range");\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(x) >= xidx + n && "Vector_insert_range");\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(x) >= n && "Vector_insert_range");\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(x) > xidx && "Vector_insert_range");\
	if (!Name##_insert_n_no_data(self, idx, n)) {\
		return 0;\
	}\
	if (self == x) {\
		if (idx <= xidx) {\
			memcpy(&CSTL_VECTOR_AT(self, idx), &CSTL_VECTOR_AT(self, xidx + n), sizeof(Type) * n);\
		} else if (xidx < idx && idx < xidx + n) {\
			memcpy(&CSTL_VECTOR_AT(self, idx), &CSTL_VECTOR_AT(self, xidx), sizeof(Type) * (idx - xidx));\
			memcpy(&CSTL_VECTOR_AT(self, idx + (idx - xidx)), &CSTL_VECTOR_AT(self, idx + n), sizeof(Type) * (n - (idx - xidx)));\
		} else {\
			memcpy(&CSTL_VECTOR_AT(self, idx), &CSTL_VECTOR_AT(self, xidx), sizeof(Type) * n);\
		}\
	} else {\
		memcpy(&CSTL_VECTOR_AT(self, idx), &CSTL_VECTOR_AT(x, xidx), sizeof(Type) * n);\
	}\
	return 1;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_ERASE(Name, Type)	\
void Name##_erase(Name *self, size_t idx, size_t n)\
{\
	CSTL_ASSERT(self && "Vector_erase");\
	CSTL_ASSERT(self->magic == self && "Vector_erase");\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) >= idx + n && "Vector_erase");\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) >= n && "Vector_erase");\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) > idx && "Vector_erase");\
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
	CSTL_ASSERT(self && "Vector_swap");\
	CSTL_ASSERT(x && "Vector_swap");\
	CSTL_ASSERT(self->magic == self && "Vector_swap");\
	CSTL_ASSERT(x->magic == x && "Vector_swap");\
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
CSTL_VECTOR_IMPLEMENT_RESERVE(Name, Type)\
CSTL_VECTOR_IMPLEMENT_MOVE_FORWARD(Name, Type)\
CSTL_VECTOR_IMPLEMENT_MOVE_BACKWARD(Name, Type)\
CSTL_VECTOR_IMPLEMENT_INSERT_N_NO_DATA(Name, Type)\
CSTL_VECTOR_IMPLEMENT_PUSH_BACK(Name, Type)\
CSTL_VECTOR_IMPLEMENT_POP_BACK(Name, Type)\
CSTL_VECTOR_IMPLEMENT_SIZE(Name, Type)\
CSTL_VECTOR_IMPLEMENT_CAPACITY(Name, Type)\
CSTL_VECTOR_IMPLEMENT_EMPTY(Name, Type)\
CSTL_VECTOR_IMPLEMENT_CLEAR(Name, Type)\
CSTL_VECTOR_IMPLEMENT_SHRINK(Name, Type)\
CSTL_VECTOR_IMPLEMENT_RESIZE(Name, Type)\
CSTL_VECTOR_IMPLEMENT_AT(Name, Type)\
CSTL_VECTOR_IMPLEMENT_FRONT(Name, Type)\
CSTL_VECTOR_IMPLEMENT_BACK(Name, Type)\
CSTL_VECTOR_IMPLEMENT_INSERT(Name, Type)\
CSTL_VECTOR_IMPLEMENT_INSERT_N(Name, Type)\
CSTL_VECTOR_IMPLEMENT_INSERT_ARRAY(Name, Type)\
CSTL_VECTOR_IMPLEMENT_INSERT_RANGE(Name, Type)\
CSTL_VECTOR_IMPLEMENT_ERASE(Name, Type)\
CSTL_VECTOR_IMPLEMENT_SWAP(Name, Type)\
CSTL_ALGORITHM_IMPLEMENT(Name, Type, CSTL_VECTOR_AT)\


#endif /* CSTL_VECTOR_H_INCLUDED */
