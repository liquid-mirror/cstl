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
 * $URL$
 * $Id$
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


#define CSTL_RING_FORWARD(self, idx, n)			((idx) + (n) >= (self)->nelems ? (idx) + (n) - (self)->nelems : (idx) + (n))
#define CSTL_RING_BACKWARD(self, idx, n)		((idx) >= (n) ? (idx) - (n) : (self)->nelems + (idx) - (n))
#define CSTL_RING_NEXT(self, idx)				CSTL_RING_FORWARD((self), (idx), 1)
#define CSTL_RING_PREV(self, idx)				CSTL_RING_BACKWARD((self), (idx), 1)
#define CSTL_RING_DISTANCE(self, first, last)	((first) <= (last) ? (last) - (first) : (self)->nelems - (first) + (last))
#define CSTL_RING_AT(self, idx)					(self)->buf[CSTL_RING_FORWARD((self), (self)->begin, (idx))]
#define CSTL_RING_EMPTY(self)					((self)->begin == (self)->end)
#define CSTL_RING_MAX_SIZE(self)				((self)->nelems - 1)
#define CSTL_RING_FULL(self)					(CSTL_RING_NEXT((self), (self)->end) == (self)->begin)
#define CSTL_RING_SIZE(self)					CSTL_RING_DISTANCE((self), (self)->begin, (self)->end)
#define CSTL_RING_FRONT(self)					(self)->buf[(self)->begin]
#define CSTL_RING_BACK(self)					(self)->buf[CSTL_RING_PREV((self), (self)->end)]
#define CSTL_RING_CLEAR(self)					(self)->end = (self)->begin


/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_RING_INTERFACE(Name, Type)	\
\
typedef struct Name##_t Name;\
/*! 
 * \brief リングバッファ構造体
 */\
struct Name##_t {\
	size_t begin;\
	size_t end;\
	size_t nelems;\
	Type *buf;\
	CSTL_RING_MAGIC(Name *magic;)\
};\
\
CSTL_RING_BEGIN_EXTERN_C()\
Name *Name##_new(size_t n);\
void Name##_init(Name *self, Type *buf, size_t n);\
void Name##_delete(Name *self);\
void Name##_erase(Name *self, size_t idx, size_t n);\
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
int Name##_insert_array(Name *self, size_t idx, const Type *elems, size_t n);\
int Name##_resize(Name *self, size_t n, Type elem);\
void Name##_swap(Name *self, Name *x);\
CSTL_RING_END_EXTERN_C()\


#define CSTL_RING_IMPLEMENT_FOR_DEQUE(Name, Type)	\
\
Name *Name##_new(size_t n)\
{\
	Name *self;\
	Type *buf;\
	self = (Name *) malloc(sizeof(Name));\
	if (!self) return 0;\
	buf = (Type *) malloc(sizeof(Type) * (n + 1));\
	if (!buf) {\
		free(self);\
		return 0;\
	}\
	Name##_init(self, buf, (n + 1));\
	return self;\
}\
\
void Name##_delete(Name *self)\
{\
	if (!self) return;\
	assert(self->magic == self && "Ring_delete");\
	CSTL_RING_MAGIC(self->magic = 0);\
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
	CSTL_RING_MAGIC(self->magic = self);\
}\
\
static void Name##_move_forward(Name *self, size_t first, size_t last, size_t n)\
{\
	register size_t i;\
	register size_t j = CSTL_RING_PREV(self, first);\
	for (i = CSTL_RING_PREV(self, last); i != j; i = CSTL_RING_PREV(self, i)) {\
		self->buf[CSTL_RING_FORWARD(self, i, n)] = self->buf[i];\
	}\
}\
\
static void Name##_move_backward(Name *self, size_t first, size_t last, size_t n)\
{\
	register size_t i;\
	for (i = first; i != last; i = CSTL_RING_NEXT(self, i)) {\
		self->buf[CSTL_RING_BACKWARD(self, i, n)] = self->buf[i];\
	}\
}\
\
void Name##_erase(Name *self, size_t idx, size_t n)\
{\
	size_t pos1;\
	size_t pos2;\
	assert(self && "Ring_erase");\
	assert(self->magic == self && "Ring_erase");\
	assert(CSTL_RING_SIZE(self) >= idx + n && "Ring_erase");\
	assert(CSTL_RING_SIZE(self) >= n && "Ring_erase");\
	assert(CSTL_RING_SIZE(self) > idx && "Ring_erase");\
	pos1 = CSTL_RING_FORWARD(self, self->begin, idx);\
	pos2 = CSTL_RING_FORWARD(self, pos1, n);\
	if (CSTL_RING_DISTANCE(self, self->begin, pos1) >= \
		CSTL_RING_DISTANCE(self, pos2, self->end)) {\
		/* end側を移動 */\
		Name##_move_backward(self, pos2, self->end, n);\
		self->end = CSTL_RING_BACKWARD(self, self->end, n);\
	} else {\
		/* begin側を移動 */\
		Name##_move_forward(self, self->begin, pos1, n);\
		self->begin = CSTL_RING_FORWARD(self, self->begin, n);\
	}\
}\
\


/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_RING_IMPLEMENT(Name, Type)	\
\
CSTL_RING_IMPLEMENT_FOR_DEQUE(Name, Type)\
\
int Name##_push_back(Name *self, Type elem)\
{\
	assert(self && "Ring_push_back");\
	assert(self->magic == self && "Ring_push_back");\
	if (CSTL_RING_FULL(self)) return 0;\
	self->buf[self->end] = elem;\
	self->end = CSTL_RING_NEXT(self, self->end);\
	return 1;\
}\
\
int Name##_push_front(Name *self, Type elem)\
{\
	assert(self && "Ring_push_front");\
	assert(self->magic == self && "Ring_push_front");\
	if (CSTL_RING_FULL(self)) return 0;\
	self->begin = CSTL_RING_PREV(self, self->begin);\
	self->buf[self->begin] = elem;\
	return 1;\
}\
\
Type Name##_pop_front(Name *self)\
{\
	size_t idx;\
	assert(self && "Ring_pop_front");\
	assert(self->magic == self && "Ring_pop_front");\
	assert(!CSTL_RING_EMPTY(self) && "Ring_pop_front");\
	idx = self->begin;\
	self->begin = CSTL_RING_NEXT(self, self->begin);\
	return self->buf[idx];\
}\
\
Type Name##_pop_back(Name *self)\
{\
	assert(self && "Ring_pop_back");\
	assert(self->magic == self && "Ring_pop_back");\
	assert(!CSTL_RING_EMPTY(self) && "Ring_pop_back");\
	self->end = CSTL_RING_PREV(self, self->end);\
	return self->buf[self->end];\
}\
\
size_t Name##_size(Name *self)\
{\
	assert(self && "Ring_size");\
	assert(self->magic == self && "Ring_size");\
	return CSTL_RING_SIZE(self);\
}\
\
size_t Name##_max_size(Name *self)\
{\
	assert(self && "Ring_max_size");\
	assert(self->magic == self && "Ring_max_size");\
	return CSTL_RING_MAX_SIZE(self);\
}\
\
int Name##_empty(Name *self)\
{\
	assert(self && "Ring_empty");\
	assert(self->magic == self && "Ring_empty");\
	return CSTL_RING_EMPTY(self);\
}\
\
int Name##_full(Name *self)\
{\
	assert(self && "Ring_full");\
	assert(self->magic == self && "Ring_full");\
	return CSTL_RING_FULL(self);\
}\
\
void Name##_clear(Name *self)\
{\
	assert(self && "Ring_clear");\
	assert(self->magic == self && "Ring_clear");\
	CSTL_RING_CLEAR(self);\
}\
\
Type *Name##_at(Name *self, size_t idx)\
{\
	assert(self && "Ring_at");\
	assert(self->magic == self && "Ring_at");\
	assert(CSTL_RING_SIZE(self) > idx && "Ring_at");\
	return &CSTL_RING_AT(self, idx);\
}\
\
Type Name##_front(Name *self)\
{\
	assert(self && "Ring_front");\
	assert(self->magic == self && "Ring_front");\
	assert(!CSTL_RING_EMPTY(self) && "Ring_front");\
	return CSTL_RING_FRONT(self);\
}\
\
Type Name##_back(Name *self)\
{\
	assert(self && "Ring_back");\
	assert(self->magic == self && "Ring_back");\
	assert(!CSTL_RING_EMPTY(self) && "Ring_back");\
	return CSTL_RING_BACK(self);\
}\
\
int Name##_insert(Name *self, size_t idx, Type elem)\
{\
	assert(self && "Ring_insert");\
	assert(self->magic == self && "Ring_insert");\
	assert(CSTL_RING_SIZE(self) >= idx && "Ring_insert");\
	return Name##_insert_array(self, idx, (const Type *) &elem, 1);\
}\
\
int Name##_insert_array(Name *self, size_t idx, const Type *elems, size_t n)\
{\
	size_t i, j;\
	size_t pos;\
	assert(self && "Ring_insert_array");\
	assert(self->magic == self && "Ring_insert_array");\
	assert(CSTL_RING_SIZE(self) >= idx && "Ring_insert_array");\
	assert(elems && "Ring_insert_array");\
	if (CSTL_RING_SIZE(self) + n > CSTL_RING_MAX_SIZE(self)) return 0;\
	pos = CSTL_RING_FORWARD(self, self->begin, idx);\
	if (CSTL_RING_SIZE(self) / 2 < idx) {\
		/* end側を移動 */\
		Name##_move_forward(self, pos, self->end, n);\
		self->end = CSTL_RING_FORWARD(self, self->end, n);\
	} else {\
		/* begin側を移動 */\
		Name##_move_backward(self, self->begin, pos, n);\
		self->begin = CSTL_RING_BACKWARD(self, self->begin, n);\
		pos = CSTL_RING_BACKWARD(self, pos, n);\
	}\
	for (i = pos, j = 0; j < n; i = CSTL_RING_NEXT(self, i), j++) {\
		self->buf[i] = ((Type *) elems)[j];\
	}\
	return 1;\
}\
\
int Name##_resize(Name *self, size_t n, Type elem)\
{\
	size_t size;\
	assert(self && "Ring_resize");\
	assert(self->magic == self && "Ring_resize");\
	size = CSTL_RING_SIZE(self);\
	if (size >= n) {\
		self->end = CSTL_RING_BACKWARD(self, self->end, size - n);\
	} else {\
		size_t i;\
		if (CSTL_RING_MAX_SIZE(self) < n) {\
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
