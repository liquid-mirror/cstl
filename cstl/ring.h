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
 * \file ring.h
 * \brief リングバッファ
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2006-02-25
 * $URL$
 * $Id$
 */
#ifndef CSTL_RING_H_INCLUDED
#define CSTL_RING_H_INCLUDED

#include <stdlib.h>
#include "common.h"


#define CSTL_RING_ADVANCE_IDX(self, idx, n)		(((idx) + (n)) & ((self)->max_size - 1))
#define CSTL_RING_RETREAT_IDX(self, idx, n)		(((self)->max_size + (idx) - (n)) & (((self)->max_size) - 1))
#define CSTL_RING_NEXT_IDX(self, idx)			CSTL_RING_ADVANCE_IDX((self), (idx), 1)
#define CSTL_RING_PREV_IDX(self, idx)			CSTL_RING_RETREAT_IDX((self), (idx), 1)
#define CSTL_RING_DISTANCE(self, first, last)	((first) <= (last) ? (last) - (first) : (self)->max_size - (first) + (last))
#define CSTL_RING_AT(self, idx)					(self)->buf[CSTL_RING_ADVANCE_IDX((self), (self)->begin, (idx))]
#define CSTL_RING_EMPTY(self)					((self)->size == 0)
#define CSTL_RING_MAX_SIZE(self)				(self)->max_size
#define CSTL_RING_FULL(self)					((self)->size == CSTL_RING_MAX_SIZE((self)))
#define CSTL_RING_SIZE(self)					(self)->size
#define CSTL_RING_FRONT(self)					(self)->buf[(self)->begin]
#define CSTL_RING_BACK(self)					(self)->buf[CSTL_RING_PREV_IDX((self), (self)->end)]
#define CSTL_RING_CLEAR(self)					do { (self)->size = 0; (self)->end = (self)->begin; } while (0)


/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_RING_INTERFACE(Name, Type)	\
\
typedef struct Name Name;\
/*! \
 * \brief リングバッファ構造体\
 */\
struct Name {\
	size_t begin;\
	size_t end;\
	size_t max_size;\
	size_t size;\
	Type *buf;\
	CSTL_MAGIC(Name *magic;)\
};\
\
CSTL_EXTERN_C_BEGIN()\
Name *Name##_new(size_t n);\
void Name##_init(Name *self, Type *buf, size_t n);\
void Name##_delete(Name *self);\
void Name##_erase(Name *self, size_t idx, size_t n);\
int Name##_push_back(Name *self, Type data);\
int Name##_push_front(Name *self, Type data);\
int Name##_push_back_ref(Name *self, Type const *data);\
int Name##_push_front_ref(Name *self, Type const *data);\
void Name##_pop_front(Name *self);\
void Name##_pop_back(Name *self);\
size_t Name##_size(Name *self);\
size_t Name##_max_size(Name *self);\
int Name##_empty(Name *self);\
int Name##_full(Name *self);\
void Name##_clear(Name *self);\
Type *Name##_at(Name *self, size_t idx);\
Type *Name##_front(Name *self);\
Type *Name##_back(Name *self);\
int Name##_insert(Name *self, size_t idx, Type data);\
int Name##_insert_array(Name *self, size_t idx, Type const *data, size_t n);\
int Name##_resize(Name *self, size_t n, Type data);\
void Name##_swap(Name *self, Name *x);\
CSTL_EXTERN_C_END()\


#define CSTL_RING_IMPLEMENT_FOR_DEQUE(Name, Type)	\
\
Name *Name##_new(size_t n)\
{\
	Name *self;\
	Type *buf;\
	self = (Name *) malloc(sizeof(Name));\
	if (!self) return 0;\
	buf = (Type *) malloc(sizeof(Type) * n);\
	if (!buf) {\
		free(self);\
		return 0;\
	}\
	Name##_init(self, buf, n);\
	return self;\
}\
\
void Name##_delete(Name *self)\
{\
	if (!self) return;\
	CSTL_ASSERT(self->magic == self && "Ring_delete");\
	CSTL_MAGIC(self->magic = 0);\
	free(self->buf);\
	free(self);\
}\
\
void Name##_init(Name *self, Type *buf, size_t n)\
{\
	CSTL_ASSERT(self && "Ring_init");\
	CSTL_ASSERT(buf && "Ring_init");\
	self->begin = 0;\
	self->end = 0;\
	self->buf = buf;\
	/* NOTE: max_sizeは必ず2の冪乗でなければならない */\
	self->max_size = n;\
	self->size = 0;\
	CSTL_MAGIC(self->magic = self);\
}\
\
static void Name##_move_backward(Name *self, size_t first, size_t last, size_t n)\
{\
	register size_t i;\
	register size_t j = CSTL_RING_PREV_IDX(self, first);\
	for (i = CSTL_RING_PREV_IDX(self, last); i != j; i = CSTL_RING_PREV_IDX(self, i)) {\
		self->buf[CSTL_RING_ADVANCE_IDX(self, i, n)] = self->buf[i];\
	}\
}\
\
static void Name##_move_forward(Name *self, size_t first, size_t last, size_t n)\
{\
	register size_t i;\
	for (i = first; i != last; i = CSTL_RING_NEXT_IDX(self, i)) {\
		self->buf[CSTL_RING_RETREAT_IDX(self, i, n)] = self->buf[i];\
	}\
}\
\
int Name##_push_back_ref(Name *self, Type const *data)\
{\
	CSTL_ASSERT(self && "Ring_push_back_ref");\
	CSTL_ASSERT(self->magic == self && "Ring_push_back_ref");\
	CSTL_ASSERT(data && "Ring_push_back_ref");\
	if (CSTL_RING_FULL(self)) return 0;\
	self->buf[self->end] = *data;\
	self->end = CSTL_RING_NEXT_IDX(self, self->end);\
	self->size++;\
	return 1;\
}\
\
int Name##_push_front_ref(Name *self, Type const *data)\
{\
	CSTL_ASSERT(self && "Ring_push_front_ref");\
	CSTL_ASSERT(self->magic == self && "Ring_push_front_ref");\
	CSTL_ASSERT(data && "Ring_push_front_ref");\
	if (CSTL_RING_FULL(self)) return 0;\
	self->begin = CSTL_RING_PREV_IDX(self, self->begin);\
	self->buf[self->begin] = *data;\
	self->size++;\
	return 1;\
}\
\
void Name##_pop_front(Name *self)\
{\
	CSTL_ASSERT(self && "Ring_pop_front");\
	CSTL_ASSERT(self->magic == self && "Ring_pop_front");\
	CSTL_ASSERT(!CSTL_RING_EMPTY(self) && "Ring_pop_front");\
	self->begin = CSTL_RING_NEXT_IDX(self, self->begin);\
	self->size--;\
}\
\
void Name##_pop_back(Name *self)\
{\
	CSTL_ASSERT(self && "Ring_pop_back");\
	CSTL_ASSERT(self->magic == self && "Ring_pop_back");\
	CSTL_ASSERT(!CSTL_RING_EMPTY(self) && "Ring_pop_back");\
	self->end = CSTL_RING_PREV_IDX(self, self->end);\
	self->size--;\
}\
\
void Name##_erase(Name *self, size_t idx, size_t n)\
{\
	size_t pos1;\
	size_t pos2;\
	CSTL_ASSERT(self && "Ring_erase");\
	CSTL_ASSERT(self->magic == self && "Ring_erase");\
	CSTL_ASSERT(CSTL_RING_SIZE(self) >= idx + n && "Ring_erase");\
	CSTL_ASSERT(CSTL_RING_SIZE(self) >= n && "Ring_erase");\
	CSTL_ASSERT(CSTL_RING_SIZE(self) > idx && "Ring_erase");\
	pos1 = CSTL_RING_ADVANCE_IDX(self, self->begin, idx);\
	pos2 = CSTL_RING_ADVANCE_IDX(self, pos1, n);\
	if (CSTL_RING_DISTANCE(self, self->begin, pos1) >= \
		CSTL_RING_DISTANCE(self, pos2, self->end)) {\
		/* end側を移動 */\
		Name##_move_forward(self, pos2, self->end, n);\
		self->end = CSTL_RING_RETREAT_IDX(self, self->end, n);\
	} else {\
		/* begin側を移動 */\
		Name##_move_backward(self, self->begin, pos1, n);\
		self->begin = CSTL_RING_ADVANCE_IDX(self, self->begin, n);\
	}\
	self->size -= n;\
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
int Name##_push_back(Name *self, Type data)\
{\
	CSTL_ASSERT(self && "Ring_push_back");\
	CSTL_ASSERT(self->magic == self && "Ring_push_back");\
	return Name##_push_back_ref(self, &data);\
}\
\
int Name##_push_front(Name *self, Type data)\
{\
	CSTL_ASSERT(self && "Ring_push_front");\
	CSTL_ASSERT(self->magic == self && "Ring_push_front");\
	return Name##_push_front_ref(self, &data);\
}\
\
size_t Name##_size(Name *self)\
{\
	CSTL_ASSERT(self && "Ring_size");\
	CSTL_ASSERT(self->magic == self && "Ring_size");\
	return CSTL_RING_SIZE(self);\
}\
\
size_t Name##_max_size(Name *self)\
{\
	CSTL_ASSERT(self && "Ring_max_size");\
	CSTL_ASSERT(self->magic == self && "Ring_max_size");\
	return CSTL_RING_MAX_SIZE(self);\
}\
\
int Name##_empty(Name *self)\
{\
	CSTL_ASSERT(self && "Ring_empty");\
	CSTL_ASSERT(self->magic == self && "Ring_empty");\
	return CSTL_RING_EMPTY(self);\
}\
\
int Name##_full(Name *self)\
{\
	CSTL_ASSERT(self && "Ring_full");\
	CSTL_ASSERT(self->magic == self && "Ring_full");\
	return CSTL_RING_FULL(self);\
}\
\
void Name##_clear(Name *self)\
{\
	CSTL_ASSERT(self && "Ring_clear");\
	CSTL_ASSERT(self->magic == self && "Ring_clear");\
	CSTL_RING_CLEAR(self);\
}\
\
Type *Name##_at(Name *self, size_t idx)\
{\
	CSTL_ASSERT(self && "Ring_at");\
	CSTL_ASSERT(self->magic == self && "Ring_at");\
	CSTL_ASSERT(CSTL_RING_SIZE(self) > idx && "Ring_at");\
	return &CSTL_RING_AT(self, idx);\
}\
\
Type *Name##_front(Name *self)\
{\
	CSTL_ASSERT(self && "Ring_front");\
	CSTL_ASSERT(self->magic == self && "Ring_front");\
	CSTL_ASSERT(!CSTL_RING_EMPTY(self) && "Ring_front");\
	return &CSTL_RING_FRONT(self);\
}\
\
Type *Name##_back(Name *self)\
{\
	CSTL_ASSERT(self && "Ring_back");\
	CSTL_ASSERT(self->magic == self && "Ring_back");\
	CSTL_ASSERT(!CSTL_RING_EMPTY(self) && "Ring_back");\
	return &CSTL_RING_BACK(self);\
}\
\
int Name##_insert(Name *self, size_t idx, Type data)\
{\
	CSTL_ASSERT(self && "Ring_insert");\
	CSTL_ASSERT(self->magic == self && "Ring_insert");\
	CSTL_ASSERT(CSTL_RING_SIZE(self) >= idx && "Ring_insert");\
	return Name##_insert_array(self, idx, &data, 1);\
}\
\
int Name##_insert_array(Name *self, size_t idx, Type const *data, size_t n)\
{\
	size_t i, j;\
	size_t pos;\
	CSTL_ASSERT(self && "Ring_insert_array");\
	CSTL_ASSERT(self->magic == self && "Ring_insert_array");\
	CSTL_ASSERT(CSTL_RING_SIZE(self) >= idx && "Ring_insert_array");\
	CSTL_ASSERT(data && "Ring_insert_array");\
	if (CSTL_RING_SIZE(self) + n > CSTL_RING_MAX_SIZE(self)) return 0;\
	pos = CSTL_RING_ADVANCE_IDX(self, self->begin, idx);\
	if (CSTL_RING_SIZE(self) / 2 < idx) {\
		/* end側を移動 */\
		Name##_move_backward(self, pos, self->end, n);\
		self->end = CSTL_RING_ADVANCE_IDX(self, self->end, n);\
	} else {\
		/* begin側を移動 */\
		Name##_move_forward(self, self->begin, pos, n);\
		self->begin = CSTL_RING_RETREAT_IDX(self, self->begin, n);\
		pos = CSTL_RING_RETREAT_IDX(self, pos, n);\
	}\
	for (i = pos, j = 0; j < n; i = CSTL_RING_NEXT_IDX(self, i), j++) {\
		self->buf[i] = data[j];\
	}\
	self->size += n;\
	return 1;\
}\
\
int Name##_resize(Name *self, size_t n, Type data)\
{\
	size_t size;\
	CSTL_ASSERT(self && "Ring_resize");\
	CSTL_ASSERT(self->magic == self && "Ring_resize");\
	size = CSTL_RING_SIZE(self);\
	if (size >= n) {\
		self->end = CSTL_RING_RETREAT_IDX(self, self->end, size - n);\
	} else {\
		size_t i;\
		if (CSTL_RING_MAX_SIZE(self) < n) {\
			return 0;\
		}\
		for (i = 0; i < n - size; i++) {\
			Name##_push_back(self, data);\
		}\
	}\
	self->size = n;\
	return 1;\
}\
\
void Name##_swap(Name *self, Name *x)\
{\
	size_t tmp_begin;\
	size_t tmp_end;\
	size_t tmp_max_size;\
	size_t tmp_size;\
	Type *tmp_buf;\
	CSTL_ASSERT(self && "Ring_swap");\
	CSTL_ASSERT(x && "Ring_swap");\
	CSTL_ASSERT(self->magic == self && "Ring_swap");\
	CSTL_ASSERT(x->magic == x && "Ring_swap");\
	tmp_begin = self->begin;\
	tmp_end = self->end;\
	tmp_max_size = self->max_size;\
	tmp_size = self->size;\
	tmp_buf = self->buf;\
	self->begin = x->begin;\
	self->end = x->end;\
	self->max_size = x->max_size;\
	self->size = x->size;\
	self->buf = x->buf;\
	x->begin = tmp_begin;\
	x->end = tmp_end;\
	x->max_size = tmp_max_size;\
	x->size = tmp_size;\
	x->buf = tmp_buf;\
}\
\

#endif /* CSTL_RING_H_INCLUDED */
