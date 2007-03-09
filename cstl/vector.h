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
 */
#ifndef CSTL_VECTOR_H_INCLUDED
#define CSTL_VECTOR_H_INCLUDED

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#ifdef __cplusplus
#define VECTOR_BEGIN_EXTERN_C()	extern "C" {
#define VECTOR_END_EXTERN_C()	}
#else
#define VECTOR_BEGIN_EXTERN_C()
#define VECTOR_END_EXTERN_C()
#endif

#ifndef NDEBUG
#define VECTOR_MAGIC(x) x
#else
#define VECTOR_MAGIC(x)
#endif

#define VECTOR_INIT_SIZE	16

/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define VECTOR_INTERFACE(Name, Type)	\
typedef struct Name##_t Name;\
\
VECTOR_BEGIN_EXTERN_C()\
Name *Name##_new(void);\
Name *Name##_new_copy(Name *x);\
void Name##_delete(Name *self);\
int Name##_assign(Name *self, Type *elems, size_t n);\
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
int Name##_insert_n(Name *self, size_t idx, Type *elems, size_t n);\
void Name##_erase(Name *self, size_t idx);\
void Name##_erase_n(Name *self, size_t idx, size_t n);\
VECTOR_END_EXTERN_C()\


/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define VECTOR_IMPLEMENT(Name, Type)	\
/*! 
 * \brief vector構造体
 */\
struct Name##_t {\
	size_t end;\
	size_t nelems;\
	Type *buf;\
	VECTOR_MAGIC(void *magic;)\
};\
\
static int Name##_full(Name *self)\
{\
	return (Name##_size(self) == Name##_capacity(self));\
}\
\
static int Name##_expand(Name *self, size_t size)\
{\
	size_t n;\
	n = (size > Name##_capacity(self) * 2) ? size : Name##_capacity(self) * 2;\
	return Name##_reserve(self, n);\
}\
\
static Name *Name##_new_reserve(size_t n)\
{\
	Name *self;\
	Type *buf;\
	self = (Name *) malloc(sizeof(Name));\
	if (!self) return 0;\
	buf = (Type *) malloc(sizeof(Type) * (n+1));\
	if (!buf) {\
		free(self);\
		return 0;\
	}\
	self->end = 0;\
	self->buf = buf;\
	self->nelems = n+1;\
	VECTOR_MAGIC(self->magic = self);\
	return self;\
}\
\
Name *Name##_new(void)\
{\
	return Name##_new_reserve(VECTOR_INIT_SIZE-1);\
}\
\
Name *Name##_new_copy(Name *x)\
{\
	Name *self;\
	assert(x && "Vector_new_copy");\
	assert(x->magic == x && "Vector_new_copy");\
	self = Name##_new_reserve(Name##_size(x));\
	if (!self) return 0;\
	Name##_insert_n(self, 0, Name##_at(x, 0), Name##_size(x));\
	return self;\
}\
\
void Name##_delete(Name *self)\
{\
	assert(self && "Vector_delete");\
	assert(self->magic == self && "Vector_delete");\
	VECTOR_MAGIC(self->magic = 0);\
	free(self->buf);\
	free(self);\
}\
\
int Name##_assign(Name *self, Type *elems, size_t n)\
{\
	size_t i;\
	assert(self && "Vector_assign");\
	assert(self->magic == self && "Vector_assign");\
	assert(elems && "Vector_assign");\
	if (n > Name##_capacity(self)) {\
		if (!Name##_expand(self, Name##_capacity(self) + n)) return 0;\
	}\
	for (i = 0; i < n; i++) {\
		self->buf[i] = elems[i];\
	}\
	self->end = n;\
	return 1;\
}\
\
int Name##_push_back(Name *self, Type elem)\
{\
	assert(self && "Vector_push_back");\
	assert(self->magic == self && "Vector_push_back");\
	if (Name##_full(self)) {\
		if (!Name##_expand(self, Name##_capacity(self) + 1)) return 0;\
	}\
	self->buf[self->end] = elem;\
	self->end++;\
	return 1;\
}\
\
Type Name##_pop_back(Name *self)\
{\
	assert(self && "Vector_pop_back");\
	assert(self->magic == self && "Vector_pop_back");\
	assert(!Name##_empty(self) && "Vector_pop_back");\
	self->end--;\
	return self->buf[self->end];\
}\
\
size_t Name##_size(Name *self)\
{\
	assert(self && "Vector_size");\
	assert(self->magic == self && "Vector_size");\
	return self->end;\
}\
\
size_t Name##_capacity(Name *self)\
{\
	assert(self && "Vector_capacity");\
	assert(self->magic == self && "Vector_capacity");\
	return (self->nelems - 1);\
}\
\
int Name##_empty(Name *self)\
{\
	assert(self && "Vector_empty");\
	assert(self->magic == self && "Vector_empty");\
	return (self->end == 0);\
}\
\
void Name##_clear(Name *self)\
{\
	assert(self && "Vector_clear");\
	assert(self->magic == self && "Vector_clear");\
	self->end = 0;\
}\
\
int Name##_reserve(Name *self, size_t n)\
{\
	Type *newbuf;\
	assert(self && "Vector_reserve");\
	assert(self->magic == self && "Vector_reserve");\
	if (n <= Name##_capacity(self)) return 1;\
	newbuf = (Type *) realloc(self->buf, sizeof(Type) * (n+1));\
	if (!newbuf) return 0;\
	self->buf = newbuf;\
	self->nelems = n+1;\
	return 1;\
}\
\
void Name##_shrink(Name *self, size_t n)\
{\
	Type *newbuf;\
	assert(self && "Vector_shrink");\
	assert(self->magic == self && "Vector_shrink");\
	if (n >= Name##_capacity(self)) return;\
	if (n < Name##_size(self)) {\
		n = Name##_size(self);\
	}\
	newbuf = (Type *) realloc(self->buf, sizeof(Type) * (n+1));\
	if (newbuf) {\
		self->buf = newbuf;\
	}\
	self->nelems = n+1;\
}\
\
int Name##_resize(Name *self, size_t n, Type elem)\
{\
	size_t size;\
	assert(self && "Vector_resize");\
	assert(self->magic == self && "Vector_resize");\
	size = Name##_size(self);\
	if (size >= n) {\
		self->end = n;\
	} else {\
		size_t i;\
		if (!Name##_reserve(self, n)) {\
			return 0;\
		}\
		for (i = 0; i < n - size; i++) {\
			Name##_push_back(self, elem);\
		}\
	}\
	return 1;\
}\
\
Type *Name##_at(Name *self, size_t idx)\
{\
	assert(self && "Vector_at");\
	assert(self->magic == self && "Vector_at");\
	assert(Name##_size(self) > idx && "Vector_at");\
	return &self->buf[idx];\
}\
\
Type Name##_front(Name *self)\
{\
	assert(self && "Vector_front");\
	assert(self->magic == self && "Vector_front");\
	assert(!Name##_empty(self) && "Vector_front");\
	return self->buf[0];\
}\
\
Type Name##_back(Name *self)\
{\
	assert(self && "Vector_back");\
	assert(self->magic == self && "Vector_back");\
	assert(!Name##_empty(self) && "Vector_back");\
	return self->buf[self->end - 1];\
}\
\
static void Name##_move_forward(Name *self, size_t first, size_t last, size_t n)\
{\
	size_t i;\
	for (i = last; i > first; i--) {\
		self->buf[i-1+n] = self->buf[i-1];\
	}\
}\
\
static void Name##_move_backward(Name *self, size_t first, size_t last, size_t n)\
{\
	size_t i;\
	for (i = first; i < last; i++) {\
		self->buf[i-n] = self->buf[i];\
	}\
}\
\
int Name##_insert(Name *self, size_t idx, Type elem)\
{\
	assert(self && "Vector_insert");\
	assert(self->magic == self && "Vector_insert");\
	assert(Name##_size(self) >= idx && "Vector_insert");\
	return Name##_insert_n(self, idx, &elem, 1);\
}\
\
int Name##_insert_n(Name *self, size_t idx, Type *elems, size_t n)\
{\
	size_t i, j;\
	assert(self && "Vector_insert_n");\
	assert(self->magic == self && "Vector_insert_n");\
	assert(Name##_size(self) >= idx && "Vector_insert_n");\
	assert(elems && "Vector_insert_n");\
	if (Name##_size(self) + n > Name##_capacity(self)) {\
		if (!Name##_expand(self, Name##_capacity(self) + n)) return 0;\
	}\
	Name##_move_forward(self, idx, self->end, n);\
	self->end += n;\
	for (i = idx, j = 0; j < n; i++, j++) {\
		self->buf[i] = elems[j];\
	}\
	return 1;\
}\
\
void Name##_erase(Name *self, size_t idx)\
{\
	assert(self && "Vector_erase");\
	assert(self->magic == self && "Vector_erase");\
	assert(Name##_size(self) >= idx + 1 && "Vector_erase");\
	Name##_erase_n(self, idx, 1);\
}\
\
void Name##_erase_n(Name *self, size_t idx, size_t n)\
{\
	assert(self && "Vector_erase_n");\
	assert(self->magic == self && "Vector_erase_n");\
	assert(Name##_size(self) >= idx + n && "Vector_erase_n");\
	Name##_move_backward(self, idx + n, self->end, n);\
	self->end -= n;\
}\
\

#endif /* CSTL_VECTOR_H_INCLUDED */
