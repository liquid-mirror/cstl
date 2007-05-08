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
 * \file ring.h
 * \brief リングバッファ
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2006-02-25
 */
#ifndef CSTL_RING_H_INCLUDED
#define CSTL_RING_H_INCLUDED

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#ifdef __cplusplus
#define CSTL_RING_BEGIN_EXTERN_C()	extern "C" {
#define CSTL_RING_END_EXTERN_C()	}
#else
#define CSTL_RING_BEGIN_EXTERN_C()
#define CSTL_RING_END_EXTERN_C()
#endif

#ifndef NDEBUG
#define CSTL_RING_MAGIC(x) x
#else
#define CSTL_RING_MAGIC(x)
#endif

/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_RING_INTERFACE(Name, Type)	\
typedef struct Name##_t Name;\
/*! 
 * \brief リングバッファ構造体
 */\
struct Name##_t {\
	size_t begin;\
	size_t end;\
	size_t nelems;\
	Type *buf;\
	CSTL_RING_MAGIC(void *magic;)\
};\
\
CSTL_RING_BEGIN_EXTERN_C()\
Name *Name##_new(size_t n);\
void Name##_init(Name *self, Type *buf, size_t n);\
void Name##_delete(Name *self);\
int Name##_assign(Name *self, Name *x, size_t idx, size_t n);\
int Name##_push_back(Name *self, Type elem);\
int Name##_push_front(Name *self, Type elem);\
Type Name##_pop_front(Name *self);\
Type Name##_pop_back(Name *self);\
size_t Name##_size(Name *self);\
size_t Name##_max_size(Name *self);\
int Name##_empty(Name *self);\
int Name##_full(Name *self);\
void Name##_clear(Name *self);\
Type *Name##_at(Name *self, size_t idx);\
Type Name##_front(Name *self);\
Type Name##_back(Name *self);\
int Name##_insert(Name *self, size_t idx, Type elem);\
int Name##_insert_array(Name *self, size_t idx, Type *elems, size_t n);\
void Name##_erase(Name *self, size_t idx, size_t n);\
int Name##_resize(Name *self, size_t n, Type elem);\
void Name##_swap(Name *self, Name *x);\
CSTL_RING_END_EXTERN_C()\


/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_RING_IMPLEMENT(Name, Type)	\
\
static size_t Name##_forward(Name *self, size_t idx, size_t n)\
{\
	return (idx + n) % self->nelems;\
}\
\
static size_t Name##_backward(Name *self, size_t idx, size_t n)\
{\
	return (idx >= n) ? idx - n : self->nelems + idx - n;\
}\
\
static size_t Name##_next(Name *self, size_t idx)\
{\
	return Name##_forward(self, idx, 1);\
}\
\
static size_t Name##_prev(Name *self, size_t idx)\
{\
	return Name##_backward(self, idx, 1);\
}\
\
static size_t Name##_distance(Name *self, size_t first, size_t last)\
{\
	return (first <= last) ? last - first : self->nelems - first + last;\
}\
\
Name *Name##_new(size_t n)\
{\
	Name *self;\
	Type *buf;\
	if (!n) return 0;\
	self = (Name *) malloc(sizeof(Name));\
	if (!self) return 0;\
	buf = (Type *) malloc(sizeof(Type) * (n+1));\
	if (!buf) {\
		free(self);\
		return 0;\
	}\
	Name##_init(self, buf, (n+1));\
	return self;\
}\
\
void Name##_delete(Name *self)\
{\
	assert(self && "Ring_delete");\
	assert(self->magic == self && "Ring_delete");\
	CSTL_RING_MAGIC(self->magic = 0;)\
	free(self->buf);\
	free(self);\
}\
\
void Name##_init(Name *self, Type *buf, size_t n)\
{\
	assert(self && "Ring_init");\
	assert(buf && "Ring_init");\
	self->begin = 0;\
	self->end = 0;\
	self->buf = buf;\
	self->nelems = n;\
	CSTL_RING_MAGIC(self->magic = self;)\
}\
\
int Name##_assign(Name *self, Name *x, size_t idx, size_t n)\
{\
	size_t i;\
	assert(self && "Ring_assign");\
	assert(self->magic == self && "Ring_assign");\
	assert(x && "Ring_assign");\
	assert(x->magic == x && "Ring_assign");\
	assert(Name##_size(x) >= idx + n && "Ring_assign");\
	if (self == x) {\
		Name##_erase(self, idx + n, Name##_size(self) - (idx + n));\
		Name##_erase(self, 0, idx);\
	} else {\
		if (n > Name##_max_size(self)) return 0;\
		Name##_clear(self);\
		for (i = 0; i < n; i++) {\
			Name##_push_back(self, *Name##_at(x, i));\
		}\
	}\
	return 1;\
}\
\
int Name##_push_back(Name *self, Type elem)\
{\
	assert(self && "Ring_push_back");\
	assert(self->magic == self && "Ring_push_back");\
	if (Name##_full(self)) return 0;\
	self->buf[self->end] = elem;\
	self->end = Name##_next(self, self->end);\
	return 1;\
}\
\
int Name##_push_front(Name *self, Type elem)\
{\
	assert(self && "Ring_push_front");\
	assert(self->magic == self && "Ring_push_front");\
	if (Name##_full(self)) return 0;\
	self->begin = Name##_prev(self, self->begin);\
	self->buf[self->begin] = elem;\
	return 1;\
}\
\
Type Name##_pop_front(Name *self)\
{\
	size_t idx;\
	assert(self && "Ring_pop_front");\
	assert(self->magic == self && "Ring_pop_front");\
	assert(!Name##_empty(self) && "Ring_pop_front");\
	idx = self->begin;\
	self->begin = Name##_next(self, self->begin);\
	return self->buf[idx];\
}\
\
Type Name##_pop_back(Name *self)\
{\
	assert(self && "Ring_pop_back");\
	assert(self->magic == self && "Ring_pop_back");\
	assert(!Name##_empty(self) && "Ring_pop_back");\
	self->end = Name##_prev(self, self->end);\
	return self->buf[self->end];\
}\
\
size_t Name##_size(Name *self)\
{\
	assert(self && "Ring_size");\
	assert(self->magic == self && "Ring_size");\
	return Name##_distance(self, self->begin, self->end);\
}\
\
size_t Name##_max_size(Name *self)\
{\
	assert(self && "Ring_max_size");\
	assert(self->magic == self && "Ring_max_size");\
	return (self->nelems - 1);\
}\
\
int Name##_empty(Name *self)\
{\
	assert(self && "Ring_empty");\
	assert(self->magic == self && "Ring_empty");\
	return (self->begin == self->end);\
}\
\
int Name##_full(Name *self)\
{\
	assert(self && "Ring_full");\
	assert(self->magic == self && "Ring_full");\
	return (Name##_next(self, self->end) == self->begin);\
}\
\
void Name##_clear(Name *self)\
{\
	assert(self && "Ring_clear");\
	assert(self->magic == self && "Ring_clear");\
	self->end = self->begin;\
}\
\
Type *Name##_at(Name *self, size_t idx)\
{\
	assert(self && "Ring_at");\
	assert(self->magic == self && "Ring_at");\
	assert(Name##_size(self) > idx && "Ring_at");\
	return &self->buf[Name##_forward(self, self->begin, idx)];\
}\
\
Type Name##_front(Name *self)\
{\
	assert(self && "Ring_front");\
	assert(self->magic == self && "Ring_front");\
	assert(!Name##_empty(self) && "Ring_front");\
	return self->buf[self->begin];\
}\
\
Type Name##_back(Name *self)\
{\
	assert(self && "Ring_back");\
	assert(self->magic == self && "Ring_back");\
	assert(!Name##_empty(self) && "Ring_back");\
	return self->buf[Name##_prev(self, self->end)];\
}\
\
static void Name##_move_forward(Name *self, size_t first, size_t last, size_t n)\
{\
	size_t i;\
	for (i = Name##_prev(self, last); i != Name##_prev(self, first); i = Name##_prev(self, i)) {\
		self->buf[Name##_forward(self, i, n)] = self->buf[i];\
	}\
}\
\
static void Name##_move_backward(Name *self, size_t first, size_t last, size_t n)\
{\
	size_t i;\
	for (i = first; i != last; i = Name##_next(self, i)) {\
		self->buf[Name##_backward(self, i, n)] = self->buf[i];\
	}\
}\
\
int Name##_insert(Name *self, size_t idx, Type elem)\
{\
	assert(self && "Ring_insert");\
	assert(self->magic == self && "Ring_insert");\
	assert(Name##_size(self) >= idx && "Ring_insert");\
	return Name##_insert_array(self, idx, &elem, 1);\
}\
\
int Name##_insert_array(Name *self, size_t idx, Type *elems, size_t n)\
{\
	size_t i, j;\
	size_t pos;\
	assert(self && "Ring_insert_array");\
	assert(self->magic == self && "Ring_insert_array");\
	assert(Name##_size(self) >= idx && "Ring_insert_array");\
	assert(elems && "Ring_insert_array");\
	if (Name##_size(self) + n > Name##_max_size(self)) return 0;\
	pos = Name##_forward(self, self->begin, idx);\
	if (Name##_size(self) / 2 < idx) {\
		/* end側を移動 */\
		Name##_move_forward(self, pos, self->end, n);\
		self->end = Name##_forward(self, self->end, n);\
	} else {\
		/* begin側を移動 */\
		Name##_move_backward(self, self->begin, pos, n);\
		self->begin = Name##_backward(self, self->begin, n);\
		pos = Name##_backward(self, pos, n);\
	}\
	for (i = pos, j = 0; j < n; i = Name##_next(self, i), j++) {\
		self->buf[i] = elems[j];\
	}\
	return 1;\
}\
\
void Name##_erase(Name *self, size_t idx, size_t n)\
{\
	size_t pos1;\
	size_t pos2;\
	assert(self && "Ring_erase");\
	assert(self->magic == self && "Ring_erase");\
	assert(Name##_size(self) >= idx + n && "Ring_erase");\
	pos1 = Name##_forward(self, self->begin, idx);\
	pos2 = Name##_forward(self, pos1, n);\
	if (Name##_distance(self, self->begin, pos1) >= \
		Name##_distance(self, pos2, self->end)) {\
		/* end側を移動 */\
		Name##_move_backward(self, pos2, self->end, n);\
		self->end = Name##_backward(self, self->end, n);\
	} else {\
		/* begin側を移動 */\
		Name##_move_forward(self, self->begin, pos1, n);\
		self->begin = Name##_forward(self, self->begin, n);\
	}\
}\
\
int Name##_resize(Name *self, size_t n, Type elem)\
{\
	size_t size;\
	assert(self && "Ring_resize");\
	assert(self->magic == self && "Ring_resize");\
	size = Name##_size(self);\
	if (size >= n) {\
		self->end = Name##_backward(self, self->end, size - n);\
	} else {\
		size_t i;\
		if (Name##_max_size(self) < n) {\
			return 0;\
		}\
		for (i = 0; i < n - size; i++) {\
			Name##_push_back(self, elem);\
		}\
	}\
	return 1;\
}\
\
void Name##_swap(Name *self, Name *x)\
{\
	size_t tmp_begin;\
	size_t tmp_end;\
	size_t tmp_nelems;\
	Type *tmp_buf;\
	assert(self && "Ring_swap");\
	assert(x && "Ring_swap");\
	assert(self->magic == self && "Ring_swap");\
	assert(x->magic == x && "Ring_swap");\
	tmp_begin = self->begin;\
	tmp_end = self->end;\
	tmp_nelems = self->nelems;\
	tmp_buf = self->buf;\
	self->begin = x->begin;\
	self->end = x->end;\
	self->nelems = x->nelems;\
	self->buf = x->buf;\
	x->begin = tmp_begin;\
	x->end = tmp_end;\
	x->nelems = tmp_nelems;\
	x->buf = tmp_buf;\
}\
\

#endif /* CSTL_RING_H_INCLUDED */
