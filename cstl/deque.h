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
 * \file deque.h
 * \brief dequeコンテナ
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2007-04-01
 * $URL$
 * $Id$
 */
#ifndef CSTL_DEQUE_H_INCLUDED
#define CSTL_DEQUE_H_INCLUDED

#include <stdlib.h>
#include <assert.h>
#include "ring.h"
#include "vector.h"

#ifdef __cplusplus
#define CSTL_DEQUE_BEGIN_EXTERN_C()	extern "C" {
#define CSTL_DEQUE_END_EXTERN_C()	}
#else
#define CSTL_DEQUE_BEGIN_EXTERN_C()
#define CSTL_DEQUE_END_EXTERN_C()
#endif

#ifdef NDEBUG
#define CSTL_DEQUE_MAGIC(x)
#else
#define CSTL_DEQUE_MAGIC(x) x
#endif

#ifndef CSTL_ALGORITHM_INTERFACE
#define CSTL_ALGORITHM_INTERFACE(Name, Type)
#endif

#ifndef CSTL_ALGORITHM_IMPLEMENT
#define CSTL_ALGORITHM_IMPLEMENT(Name, Type, DIRECT_ACCESS)
#endif

#define CSTL_DEQUE_RINGBUF_SIZE(Type)	(sizeof(Type) < 512 ? 512 / sizeof(Type) : 1)
#define CSTL_DEQUE_INITIAL_MAP_SIZE		(8)
#define CSTL_DEQUE_SIZE(self)			(self)->size

/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_DEQUE_INTERFACE(Name, Type)	\
typedef struct Name Name;\
\
CSTL_DEQUE_BEGIN_EXTERN_C()\
Name *Name##_new(void);\
void Name##_delete(Name *self);\
int Name##_push_back(Name *self, Type data);\
int Name##_push_front(Name *self, Type data);\
void Name##_pop_front(Name *self);\
void Name##_pop_back(Name *self);\
size_t Name##_size(Name *self);\
int Name##_empty(Name *self);\
void Name##_clear(Name *self);\
Type *Name##_at(Name *self, size_t idx);\
Type *Name##_front(Name *self);\
Type *Name##_back(Name *self);\
int Name##_insert(Name *self, size_t idx, Type data);\
int Name##_insert_n(Name *self, size_t idx, size_t n, Type data);\
int Name##_insert_array(Name *self, size_t idx, Type const *data, size_t n);\
int Name##_insert_range(Name *self, size_t idx, Name *x, size_t xidx, size_t n);\
void Name##_erase(Name *self, size_t idx, size_t n);\
int Name##_resize(Name *self, size_t n, Type data);\
void Name##_swap(Name *self, Name *x);\
CSTL_ALGORITHM_INTERFACE(Name, Type)\
CSTL_DEQUE_END_EXTERN_C()\


/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_DEQUE_IMPLEMENT(Name, Type)	\
\
CSTL_RING_INTERFACE(Name##_Ring, Type)\
CSTL_RING_IMPLEMENT_FOR_DEQUE(Name##_Ring, Type)\
CSTL_VECTOR_INTERFACE(Name##_RingVector, Name##_Ring*)\
CSTL_VECTOR_IMPLEMENT_BASE(Name##_RingVector, Name##_Ring*)\
CSTL_VECTOR_IMPLEMENT_RESERVE(Name##_RingVector, Name##_Ring*)\
CSTL_VECTOR_IMPLEMENT_MOVE_FORWARD(Name##_RingVector, Name##_Ring*)\
CSTL_VECTOR_IMPLEMENT_MOVE_BACKWARD(Name##_RingVector, Name##_Ring*)\
CSTL_VECTOR_IMPLEMENT_INSERT_N_NO_DATA(Name##_RingVector, Name##_Ring*)\
CSTL_VECTOR_IMPLEMENT_PUSH_BACK(Name##_RingVector, Name##_Ring*)\
CSTL_VECTOR_IMPLEMENT_POP_BACK(Name##_RingVector, Name##_Ring*)\
CSTL_VECTOR_IMPLEMENT_BACK(Name##_RingVector, Name##_Ring*)\
CSTL_VECTOR_IMPLEMENT_RESIZE(Name##_RingVector, Name##_Ring*)\
\
/*! \
 * \brief deque構造体\
 */\
struct Name {\
	size_t begin;\
	size_t end;\
	size_t size;\
	Name##_RingVector *map;\
	Name##_RingVector *pool;\
	CSTL_DEQUE_MAGIC(Name *magic;)\
};\
\
static void Name##_coordinate(Name *self, size_t idx, size_t *map_idx, size_t *ring_idx)\
{\
	size_t n;\
	n = CSTL_RING_SIZE(CSTL_VECTOR_AT(self->map, self->begin));\
	if (idx < n) {\
		*map_idx = self->begin;\
		*ring_idx = idx;\
	} else {\
		*map_idx = (self->begin + 1) + (idx - n) / CSTL_DEQUE_RINGBUF_SIZE(Type);\
		*ring_idx = (idx - n) % CSTL_DEQUE_RINGBUF_SIZE(Type);\
	}\
}\
\
static Name##_Ring *Name##_pop_ring(Name *self)\
{\
	if (CSTL_VECTOR_EMPTY(self->pool)) {\
		return Name##_Ring_new(CSTL_DEQUE_RINGBUF_SIZE(Type));\
	} else {\
		Name##_Ring *ret = *Name##_RingVector_back(self->pool);\
		Name##_RingVector_pop_back(self->pool);\
		return ret;\
	}\
}\
\
static void Name##_push_ring(Name *self, Name##_Ring *ring)\
{\
	if (CSTL_VECTOR_FULL(self->pool)) {\
		/* poolの拡張はしない */\
		Name##_Ring_delete(ring);\
	} else {\
		CSTL_RING_CLEAR(ring);\
		Name##_RingVector_push_back(self->pool, ring);\
	}\
}\
\
static int Name##_expand_begin_side(Name *self, size_t n)\
{\
	size_t m;\
	size_t expand;\
	register size_t i;\
	m = CSTL_DEQUE_RINGBUF_SIZE(Type) - \
		CSTL_RING_SIZE(CSTL_VECTOR_AT(self->map, self->begin));\
	if (n <= m) {\
		return 1;\
	}\
	expand = 1 + (n - m - 1) / CSTL_DEQUE_RINGBUF_SIZE(Type);\
	if (self->begin < expand) {\
		size_t b, e;\
		b = self->begin;\
		e = self->end;\
		if (CSTL_VECTOR_SIZE(self->map) - e + self->begin < expand) {\
			size_t j = (CSTL_VECTOR_SIZE(self->map) > expand) ? CSTL_VECTOR_SIZE(self->map) : expand;\
			/* mapを拡張する */\
			if (!Name##_RingVector_insert_n_no_data(self->map, 0, j)) {\
				return 0;\
			}\
			for (i = 0; i < j; i++) {\
				CSTL_VECTOR_AT(self->map, i) = 0;\
			}\
			self->begin += j;\
			self->end += j;\
		} else {\
			/* mapをずらす */\
			size_t slide = ((expand - self->begin) + (CSTL_VECTOR_SIZE(self->map) - e)) / 2;\
			assert(e + slide <= CSTL_VECTOR_SIZE(self->map) && "Deque_expand_begin_side");\
			Name##_RingVector_move_backward(self->map, b, e, slide);\
			for (i = b; i < b + slide; i++) {\
				CSTL_VECTOR_AT(self->map, i) = 0;\
			}\
			self->begin += slide;\
			self->end += slide;\
		}\
	}\
	for (i = self->begin - expand; i < self->begin; i++) {\
		assert(!CSTL_VECTOR_AT(self->map, i) && "Deque_expand_begin_side");\
		CSTL_VECTOR_AT(self->map, i) = Name##_pop_ring(self);\
		if (!CSTL_VECTOR_AT(self->map, i)) {\
			return 0;\
		}\
	}\
	return 1;\
}\
\
static int Name##_expand_end_side(Name *self, size_t n)\
{\
	size_t m;\
	size_t expand;\
	register size_t i;\
	m = CSTL_DEQUE_RINGBUF_SIZE(Type) - \
		CSTL_RING_SIZE(CSTL_VECTOR_AT(self->map, self->end - 1));\
	if (n <= m) {\
		return 1;\
	}\
	expand = 1 + (n - m - 1) / CSTL_DEQUE_RINGBUF_SIZE(Type);\
	if (CSTL_VECTOR_SIZE(self->map) - self->end < expand) {\
		size_t b, e;\
		b = self->begin;\
		e = self->end;\
		if (CSTL_VECTOR_SIZE(self->map) - self->end + b < expand) {\
			size_t j = (CSTL_VECTOR_SIZE(self->map) > expand) ? CSTL_VECTOR_SIZE(self->map) : expand;\
			/* mapを拡張する */\
			if (!Name##_RingVector_resize(self->map, self->end + j, 0)) {\
				return 0;\
			}\
		} else {\
			/* mapをずらす */\
			size_t slide = ((expand - (CSTL_VECTOR_SIZE(self->map) - self->end)) + b) / 2;\
			assert(b >= slide && "Deque_expand_end_side");\
			Name##_RingVector_move_forward(self->map, b, e, slide);\
			for (i = e - slide; i < e; i++) {\
				CSTL_VECTOR_AT(self->map, i) = 0;\
			}\
			self->begin -= slide;\
			self->end -= slide;\
		}\
	}\
	for (i = self->end; i < self->end + expand; i++) {\
		assert(!CSTL_VECTOR_AT(self->map, i) && "Deque_expand_end_side");\
		CSTL_VECTOR_AT(self->map, i) = Name##_pop_ring(self);\
		if (!CSTL_VECTOR_AT(self->map, i)) {\
			return 0;\
		}\
	}\
	return 1;\
}\
\
static int Name##_Ring_push_back_no_data(Name##_Ring *self)\
{\
	if (CSTL_RING_FULL(self)) return 0;\
	self->end = CSTL_RING_NEXT_IDX(self, self->end);\
	self->size++;\
	return 1;\
}\
\
static int Name##_Ring_push_front_no_data(Name##_Ring *self)\
{\
	if (CSTL_RING_FULL(self)) return 0;\
	self->begin = CSTL_RING_PREV_IDX(self, self->begin);\
	self->size++;\
	return 1;\
}\
\
static void Name##_push_front_n_no_data(Name *self, size_t n)\
{\
	register size_t i;\
	for (i = 0; i < n; i++) {\
		if (!Name##_Ring_push_front_no_data(CSTL_VECTOR_AT(self->map, self->begin))) {\
			self->begin--;\
			assert(CSTL_RING_EMPTY(CSTL_VECTOR_AT(self->map, self->begin)) && "Deque_push_front_n_no_data");\
			Name##_Ring_push_front_no_data(CSTL_VECTOR_AT(self->map, self->begin));\
		}\
	}\
	self->size += n;\
}\
\
static void Name##_push_back_n_no_data(Name *self, size_t n)\
{\
	register size_t i;\
	for (i = 0; i < n; i++) {\
		if (!Name##_Ring_push_back_no_data(CSTL_VECTOR_AT(self->map, self->end - 1))) {\
			self->end++;\
			assert(CSTL_RING_EMPTY(CSTL_VECTOR_AT(self->map, self->end - 1)) && "Deque_push_back_n_no_data");\
			Name##_Ring_push_back_no_data(CSTL_VECTOR_AT(self->map, self->end - 1));\
		}\
	}\
	self->size += n;\
}\
\
static void Name##_move_backward(Name *self, size_t first, size_t last, size_t n)\
{\
	register size_t i;\
	for (i = last; i > first; i--) {\
		*Name##_at(self, i - 1 + n) = *Name##_at(self, i - 1);\
	}\
}\
\
static void Name##_move_forward(Name *self, size_t first, size_t last, size_t n)\
{\
	register size_t i;\
	for (i = first; i < last; i++) {\
		*Name##_at(self, i - n) = *Name##_at(self, i);\
	}\
}\
\
Name *Name##_new(void)\
{\
	Name *self;\
	self = (Name *) malloc(sizeof(Name));\
	if (!self) return 0;\
	self->map = Name##_RingVector_new_reserve(CSTL_DEQUE_INITIAL_MAP_SIZE);\
	if (!self->map) {\
		free(self);\
		return 0;\
	}\
	Name##_RingVector_resize(self->map, CSTL_DEQUE_INITIAL_MAP_SIZE, 0);\
	self->pool = Name##_RingVector_new_reserve(CSTL_DEQUE_INITIAL_MAP_SIZE);\
	if (!self->pool) {\
		Name##_RingVector_delete(self->map);\
		free(self);\
		return 0;\
	}\
	self->begin = CSTL_VECTOR_SIZE(self->map) / 2;\
	self->end = self->begin + 1;\
	self->size = 0;\
	CSTL_VECTOR_AT(self->map, self->begin) = Name##_Ring_new(CSTL_DEQUE_RINGBUF_SIZE(Type));\
	if (!CSTL_VECTOR_AT(self->map, self->begin)) {\
		Name##_RingVector_delete(self->map);\
		Name##_RingVector_delete(self->pool);\
		free(self);\
		return 0;\
	}\
	CSTL_DEQUE_MAGIC(self->magic = self);\
	return self;\
}\
\
void Name##_delete(Name *self)\
{\
	register size_t i;\
	if (!self) return;\
	assert(self->magic == self && "Deque_delete");\
	for (i = 0; i < CSTL_VECTOR_SIZE(self->map); i++) {\
		if (CSTL_VECTOR_AT(self->map, i)) {\
			Name##_Ring_delete(CSTL_VECTOR_AT(self->map, i));\
		}\
	}\
	while (!CSTL_VECTOR_EMPTY(self->pool)) {\
		Name##_Ring_delete(*Name##_RingVector_back(self->pool));\
		Name##_RingVector_pop_back(self->pool);\
	}\
	Name##_RingVector_delete(self->map);\
	Name##_RingVector_delete(self->pool);\
	CSTL_DEQUE_MAGIC(self->magic = 0);\
	free(self);\
}\
\
int Name##_push_back(Name *self, Type data)\
{\
	assert(self && "Deque_push_back");\
	assert(self->magic == self && "Deque_push_back");\
	if (CSTL_RING_FULL(CSTL_VECTOR_AT(self->map, self->end - 1))) {\
		if (!Name##_expand_end_side(self, 1)) {\
			return 0;\
		}\
		Name##_Ring_push_back_no_data(CSTL_VECTOR_AT(self->map, self->end));\
		CSTL_RING_BACK(CSTL_VECTOR_AT(self->map, self->end)) = data;\
		self->end++;\
	} else {\
		Name##_Ring_push_back_no_data(CSTL_VECTOR_AT(self->map, self->end - 1));\
		CSTL_RING_BACK(CSTL_VECTOR_AT(self->map, self->end - 1)) = data;\
	}\
	self->size++;\
	return 1;\
}\
\
int Name##_push_front(Name *self, Type data)\
{\
	assert(self && "Deque_push_front");\
	assert(self->magic == self && "Deque_push_front");\
	if (CSTL_RING_FULL(CSTL_VECTOR_AT(self->map, self->begin))) {\
		if (!Name##_expand_begin_side(self, 1)) {\
			return 0;\
		}\
		Name##_Ring_push_front_no_data(CSTL_VECTOR_AT(self->map, self->begin - 1));\
		CSTL_RING_FRONT(CSTL_VECTOR_AT(self->map, self->begin - 1)) = data;\
		self->begin--;\
	} else {\
		Name##_Ring_push_front_no_data(CSTL_VECTOR_AT(self->map, self->begin));\
		CSTL_RING_FRONT(CSTL_VECTOR_AT(self->map, self->begin)) = data;\
	}\
	self->size++;\
	return 1;\
}\
\
void Name##_pop_front(Name *self)\
{\
	assert(self && "Deque_pop_front");\
	assert(self->magic == self && "Deque_pop_front");\
	assert(!Name##_empty(self) && "Deque_pop_front");\
	Name##_Ring_pop_front(CSTL_VECTOR_AT(self->map, self->begin));\
	self->size--;\
	if (CSTL_RING_EMPTY(CSTL_VECTOR_AT(self->map, self->begin)) && self->size > 0) {\
		Name##_push_ring(self, CSTL_VECTOR_AT(self->map, self->begin));\
		CSTL_VECTOR_AT(self->map, self->begin) = 0;\
		self->begin++;\
		assert(self->begin < self->end && "Deque_pop_front");\
	}\
}\
\
void Name##_pop_back(Name *self)\
{\
	assert(self && "Deque_pop_back");\
	assert(self->magic == self && "Deque_pop_back");\
	assert(!Name##_empty(self) && "Deque_pop_back");\
	Name##_Ring_pop_back(CSTL_VECTOR_AT(self->map, self->end - 1));\
	self->size--;\
	if (CSTL_RING_EMPTY(CSTL_VECTOR_AT(self->map, self->end - 1)) && self->size > 0) {\
		Name##_push_ring(self, CSTL_VECTOR_AT(self->map, self->end - 1));\
		CSTL_VECTOR_AT(self->map, self->end - 1) = 0;\
		self->end--;\
		assert(self->begin < self->end && "Deque_pop_back");\
	}\
}\
\
size_t Name##_size(Name *self)\
{\
	assert(self && "Deque_size");\
	assert(self->magic == self && "Deque_size");\
	return CSTL_DEQUE_SIZE(self);\
}\
\
int Name##_empty(Name *self)\
{\
	assert(self && "Deque_empty");\
	assert(self->magic == self && "Deque_empty");\
	return (self->size == 0);\
}\
\
void Name##_clear(Name *self)\
{\
	register size_t i;\
	assert(self && "Deque_clear");\
	assert(self->magic == self && "Deque_clear");\
	self->end = self->begin + 1;\
	self->size = 0;\
	CSTL_RING_CLEAR(CSTL_VECTOR_AT(self->map, self->begin));\
	for (i = self->end; i < CSTL_VECTOR_SIZE(self->map) && CSTL_VECTOR_AT(self->map, i); i++) {\
		Name##_push_ring(self, CSTL_VECTOR_AT(self->map, i));\
		CSTL_VECTOR_AT(self->map, i) = 0;\
	}\
}\
\
Type *Name##_at(Name *self, size_t idx)\
{\
	size_t m, n;\
	assert(self && "Deque_at");\
	assert(self->magic == self && "Deque_at");\
	assert(Name##_size(self) > idx && "Deque_at");\
	Name##_coordinate(self, idx, &m, &n);\
	return &CSTL_RING_AT(CSTL_VECTOR_AT(self->map, m), n);\
}\
\
Type *Name##_front(Name *self)\
{\
	assert(self && "Deque_front");\
	assert(self->magic == self && "Deque_front");\
	assert(!Name##_empty(self) && "Deque_front");\
	return &CSTL_RING_FRONT(CSTL_VECTOR_AT(self->map, self->begin));\
}\
\
Type *Name##_back(Name *self)\
{\
	assert(self && "Deque_back");\
	assert(self->magic == self && "Deque_back");\
	assert(!Name##_empty(self) && "Deque_back");\
	return &CSTL_RING_BACK(CSTL_VECTOR_AT(self->map, self->end - 1));\
}\
\
int Name##_insert(Name *self, size_t idx, Type data)\
{\
	assert(self && "Deque_insert");\
	assert(self->magic == self && "Deque_insert");\
	assert(Name##_size(self) >= idx && "Deque_insert");\
	return Name##_insert_array(self, idx, &data, 1);\
}\
\
static int Name##_insert_n_no_data(Name *self, size_t idx, size_t n)\
{\
	size_t size = CSTL_DEQUE_SIZE(self);\
	if (size / 2 < idx) {\
		/* end側を移動 */\
		if (!Name##_expand_end_side(self, n)) {\
			return 0;\
		}\
		Name##_push_back_n_no_data(self, n);\
		Name##_move_backward(self, idx, size, n);\
	} else {\
		/* begin側を移動 */\
		if (!Name##_expand_begin_side(self, n)) {\
			return 0;\
		}\
		Name##_push_front_n_no_data(self, n);\
		Name##_move_forward(self, n, n + idx, n);\
	}\
	return 1;\
}\
\
int Name##_insert_n(Name *self, size_t idx, size_t n, Type data)\
{\
	register size_t i;\
	assert(self && "Deque_insert_n");\
	assert(self->magic == self && "Deque_insert_n");\
	assert(Name##_size(self) >= idx && "Deque_insert_n");\
	if (!Name##_insert_n_no_data(self, idx, n)) {\
		return 0;\
	}\
	for (i = 0; i < n; i++) {\
		*Name##_at(self, idx + i) = data;\
	}\
	return 1;\
}\
\
int Name##_insert_array(Name *self, size_t idx, Type const *data, size_t n)\
{\
	register size_t i;\
	assert(self && "Deque_insert_array");\
	assert(self->magic == self && "Deque_insert_array");\
	assert(Name##_size(self) >= idx && "Deque_insert_array");\
	assert(data && "Deque_insert_array");\
	if (!Name##_insert_n_no_data(self, idx, n)) {\
		return 0;\
	}\
	for (i = 0; i < n; i++) {\
		*Name##_at(self, idx + i) = data[i];\
	}\
	return 1;\
}\
\
int Name##_insert_range(Name *self, size_t idx, Name *x, size_t xidx, size_t n)\
{\
	register size_t i;\
	assert(self && "Deque_insert_range");\
	assert(self->magic == self && "Deque_insert_range");\
	assert(Name##_size(self) >= idx && "Deque_insert_range");\
	assert(x && "Deque_insert_range");\
	assert(x->magic == x && "Deque_insert_range");\
	assert(Name##_size(x) >= xidx + n && "Deque_insert_range");\
	assert(Name##_size(x) >= n && "Deque_insert_range");\
	assert(Name##_size(x) > xidx && "Deque_insert_range");\
	if (!Name##_insert_n_no_data(self, idx, n)) {\
		return 0;\
	}\
	if (self == x) {\
		if (idx <= xidx) {\
			for (i = 0; i < n; i++) {\
				*Name##_at(self, idx + i) = *Name##_at(self, xidx + n + i);\
			}\
		} else if (xidx < idx && idx < xidx + n) {\
			for (i = 0; i < idx - xidx; i++) {\
				*Name##_at(self, idx + i) = *Name##_at(self, xidx + i);\
			}\
			for (i = 0; i < n - (idx - xidx); i++) {\
				*Name##_at(self, idx + (idx - xidx) + i) = *Name##_at(self, idx + n + i);\
			}\
		} else {\
			for (i = 0; i < n; i++) {\
				*Name##_at(self, idx + i) = *Name##_at(self, xidx + i);\
			}\
		}\
	} else {\
		for (i = 0; i < n; i++) {\
			*Name##_at(self, idx + i) = *Name##_at(x, xidx + i);\
		}\
	}\
	return 1;\
}\
\
void Name##_erase(Name *self, size_t idx, size_t n)\
{\
	size_t i, j;\
	register size_t k;\
	size_t size;\
	assert(self && "Deque_erase");\
	assert(self->magic == self && "Deque_erase");\
	assert(Name##_size(self) >= idx + n && "Deque_erase");\
	assert(Name##_size(self) >= n && "Deque_erase");\
	assert(Name##_size(self) > idx && "Deque_erase");\
	if (!n) return;\
	size = CSTL_DEQUE_SIZE(self);\
	if (idx >= size - (idx + n)) {\
		/* end側を移動 */\
		Name##_move_forward(self, idx + n, size, n);\
		Name##_coordinate(self, size - n, &i, &j);\
		assert(i >= self->begin && "Deque_erase");\
		if (i == self->begin || j != 0) {\
			Name##_Ring_erase(CSTL_VECTOR_AT(self->map, i), j, \
					CSTL_RING_SIZE(CSTL_VECTOR_AT(self->map, i)) - j);\
			self->end = i + 1;\
		} else {\
			self->end = i;\
		}\
		assert(self->begin < self->end && "Deque_erase1");\
		for (k = self->end; k < CSTL_VECTOR_SIZE(self->map) && CSTL_VECTOR_AT(self->map, k); k++) {\
			Name##_push_ring(self, CSTL_VECTOR_AT(self->map, k));\
			CSTL_VECTOR_AT(self->map, k) = 0;\
		}\
	} else {\
		/* begin側を移動 */\
		Name##_move_backward(self, 0, idx, n);\
		Name##_coordinate(self, n, &i, &j);\
		self->begin = i;\
		Name##_Ring_erase(CSTL_VECTOR_AT(self->map, i), 0, j);\
		assert(self->begin < self->end && "Deque_erase2");\
		for (k = self->begin; k > 0 && CSTL_VECTOR_AT(self->map, k - 1); k--) {\
			Name##_push_ring(self, CSTL_VECTOR_AT(self->map, k - 1));\
			CSTL_VECTOR_AT(self->map, k - 1) = 0;\
		}\
	}\
	self->size -= n;\
}\
\
int Name##_resize(Name *self, size_t n, Type data)\
{\
	register size_t i;\
	size_t size;\
	assert(self && "Deque_resize");\
	assert(self->magic == self && "Deque_resize");\
	size = CSTL_DEQUE_SIZE(self);\
	if (size >= n) {\
		Name##_erase(self, n, size - n);\
	} else {\
		if (!Name##_insert_n_no_data(self, size, n - size)) {\
			return 0;\
		}\
		for (i = size; i < n; i++) {\
			*Name##_at(self, i) = data;\
		}\
	}\
	return 1;\
}\
\
void Name##_swap(Name *self, Name *x)\
{\
	size_t tmp_begin;\
	size_t tmp_end;\
	size_t tmp_size;\
	Name##_RingVector *tmp_map;\
	Name##_RingVector *tmp_pool;\
	assert(self && "Deque_swap");\
	assert(x && "Deque_swap");\
	assert(self->magic == self && "Deque_swap");\
	assert(x->magic == x && "Deque_swap");\
	tmp_begin = self->begin;\
	tmp_end = self->end;\
	tmp_size = self->size;\
	tmp_map = self->map;\
	tmp_pool = self->pool;\
	self->begin = x->begin;\
	self->end = x->end;\
	self->size = x->size;\
	self->map = x->map;\
	self->pool = x->pool;\
	x->begin = tmp_begin;\
	x->end = tmp_end;\
	x->size = tmp_size;\
	x->map = tmp_map;\
	x->pool = tmp_pool;\
}\
\
CSTL_ALGORITHM_IMPLEMENT(Name, Type, *Name##_at)\


#endif /* CSTL_DEQUE_H_INCLUDED */
