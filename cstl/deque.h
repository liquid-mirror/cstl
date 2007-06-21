/* 
 * Copyright (c) 2007, KATO Noriaki
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
 */
#ifndef CSTL_DEQUE_H_INCLUDED
#define CSTL_DEQUE_H_INCLUDED

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include "algorithm.h"
#include "ring.h"
#include "vector.h"

#ifdef __cplusplus
#define CSTL_DEQUE_BEGIN_EXTERN_C()	extern "C" {
#define CSTL_DEQUE_END_EXTERN_C()	}
#else
#define CSTL_DEQUE_BEGIN_EXTERN_C()
#define CSTL_DEQUE_END_EXTERN_C()
#endif

#ifndef NDEBUG
#define CSTL_DEQUE_MAGIC(x) x
#else
#define CSTL_DEQUE_MAGIC(x)
#endif

#define CSTL_DEQUE_RINGBUF_SIZE(Type)	(sizeof(Type) < 511 ? 511 / sizeof(Type) : 1)
#define CSTL_DEQUE_INITIAL_MAP_SIZE		(8)

/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_DEQUE_INTERFACE(Name, Type)	\
typedef struct Name##_t Name;\
\
CSTL_DEQUE_BEGIN_EXTERN_C()\
Name *Name##_new(void);\
void Name##_delete(Name *self);\
int Name##_assign(Name *self, Name *x, size_t idx, size_t n);\
int Name##_push_back(Name *self, Type elem);\
int Name##_push_front(Name *self, Type elem);\
Type Name##_pop_front(Name *self);\
Type Name##_pop_back(Name *self);\
size_t Name##_size(Name *self);\
int Name##_empty(Name *self);\
void Name##_clear(Name *self);\
Type *Name##_at(Name *self, size_t idx);\
Type Name##_front(Name *self);\
Type Name##_back(Name *self);\
int Name##_insert(Name *self, size_t idx, Type elem);\
int Name##_insert_array(Name *self, size_t idx, Type *elems, size_t n);\
void Name##_erase(Name *self, size_t idx, size_t n);\
int Name##_resize(Name *self, size_t n, Type elem);\
void Name##_swap(Name *self, Name *x);\
void Name##_sort(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *));\
int Name##_stable_sort(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *));\
CSTL_DEQUE_END_EXTERN_C()\


/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_DEQUE_IMPLEMENT(Name, Type)	\
\
CSTL_RING_INTERFACE(Name##__Ring, Type)\
CSTL_RING_IMPLEMENT(Name##__Ring, Type)\
CSTL_VECTOR_INTERFACE(Name##__RingVector, Name##__Ring*)\
CSTL_VECTOR_IMPLEMENT(Name##__RingVector, Name##__Ring*)\
\
/*! 
 * \brief deque構造体
 */\
struct Name##_t {\
	size_t begin;\
	size_t end;\
	size_t nelems;\
	Name##__RingVector *map;\
	Name##__RingVector *pool;\
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
static Name##__Ring *Name##_pop_ring(Name *self)\
{\
	if (CSTL_VECTOR_EMPTY(self->pool)) {\
		return Name##__Ring_new(CSTL_DEQUE_RINGBUF_SIZE(Type));\
	} else {\
		return Name##__RingVector_pop_back(self->pool);\
	}\
}\
\
static void Name##_push_ring(Name *self, Name##__Ring *ring)\
{\
	if (CSTL_VECTOR_FULL(self->pool)) {\
		/* poolの拡張はしない */\
		Name##__Ring_delete(ring);\
	} else {\
		Name##__Ring_clear(ring);\
		Name##__RingVector_push_back(self->pool, ring);\
	}\
}\
\
static void Name##__RingVector_insert_array_no_elems(Name##__RingVector *self, size_t idx, size_t n)\
{\
	Name##__RingVector_move_forward(self, idx, self->end, n);\
	self->end += n;\
}\
\
static size_t Name##_map_idx_begin_side(Name *self)\
{\
	size_t i;\
	for (i = self->begin; i > 0; i--) {\
		if (!CSTL_VECTOR_AT(self->map, i - 1)) {\
			break;\
		}\
	}\
	return i;\
}\
\
static size_t Name##_map_idx_end_side(Name *self)\
{\
	size_t i;\
	for (i = self->end; i < CSTL_VECTOR_SIZE(self->map); i++) {\
		if (!CSTL_VECTOR_AT(self->map, i)) {\
			break;\
		}\
	}\
	return i;\
}\
\
static int Name##_expand_begin_side(Name *self, size_t n)\
{\
	size_t m;\
	size_t s;\
	size_t i;\
	m = CSTL_RING_MAX_SIZE(CSTL_VECTOR_AT(self->map, self->begin)) - \
		CSTL_RING_SIZE(CSTL_VECTOR_AT(self->map, self->begin));\
	if (n <= m) {\
		return 1;\
	}\
	s = 1 + (n - m - 1) / CSTL_DEQUE_RINGBUF_SIZE(Type);\
	if (self->begin < s) {\
		size_t b, e;\
		b = Name##_map_idx_begin_side(self);\
		e = Name##_map_idx_end_side(self);\
		if (CSTL_VECTOR_SIZE(self->map) - e + self->begin < s) {\
			size_t j;\
			size_t k = (CSTL_VECTOR_SIZE(self->map) > s) ? CSTL_VECTOR_SIZE(self->map) : s;\
			/* mapを拡張する */\
			if (!Name##__RingVector_expand(self->map, CSTL_VECTOR_SIZE(self->map) + k)) {\
				return 0;\
			}\
			Name##__RingVector_insert_array_no_elems(self->map, 0, k);\
			for (j = 0; j < k; j++) {\
				CSTL_VECTOR_AT(self->map, j) = 0;\
			}\
			self->begin += k;\
			self->end += k;\
		} else {\
			/* mapをずらす */\
			size_t d = s - self->begin;\
			assert(e + d <= CSTL_VECTOR_SIZE(self->map) && "Deque_expand_begin_side");\
			Name##__RingVector_move_forward(self->map, b, e, d);\
			self->begin += d;\
			self->end += d;\
			for (i = b; i < b + d; i++) {\
				CSTL_VECTOR_AT(self->map, i) = 0;\
			}\
		}\
	}\
	for (i = self->begin - s; i < self->begin; i++) {\
		if (!CSTL_VECTOR_AT(self->map, i)) {\
			CSTL_VECTOR_AT(self->map, i) = Name##_pop_ring(self);\
			if (!CSTL_VECTOR_AT(self->map, i)) {\
				return 0;\
			}\
		} else {\
			Name##__Ring_clear(CSTL_VECTOR_AT(self->map, i));\
		}\
	}\
	return 1;\
}\
\
static int Name##_expand_end_side(Name *self, size_t n)\
{\
	size_t m;\
	size_t s;\
	size_t i;\
	m = CSTL_RING_MAX_SIZE(CSTL_VECTOR_AT(self->map, self->end - 1)) - \
		CSTL_RING_SIZE(CSTL_VECTOR_AT(self->map, self->end - 1));\
	if (n <= m) {\
		return 1;\
	}\
	s = 1 + (n - m - 1) / CSTL_DEQUE_RINGBUF_SIZE(Type);\
	if (CSTL_VECTOR_SIZE(self->map) - self->end < s) {\
		size_t b, e;\
		b = Name##_map_idx_begin_side(self);\
		e = Name##_map_idx_end_side(self);\
		if (CSTL_VECTOR_SIZE(self->map) - self->end + b < s) {\
			size_t k = (CSTL_VECTOR_SIZE(self->map) > s) ? CSTL_VECTOR_SIZE(self->map) : s;\
			/* mapを拡張する */\
			if (!Name##__RingVector_resize(self->map, self->end + k, 0)) {\
				return 0;\
			}\
		} else {\
			/* mapをずらす */\
			size_t d = s - (CSTL_VECTOR_SIZE(self->map) - self->end);\
			assert(b >= d && "Deque_expand_end_side");\
			Name##__RingVector_move_backward(self->map, b, e, d);\
			self->begin -= d;\
			self->end -= d;\
			for (i = e - d; i < e; i++) {\
				CSTL_VECTOR_AT(self->map, i) = 0;\
			}\
		}\
	}\
	for (i = self->end; i < self->end + s; i++) {\
		if (!CSTL_VECTOR_AT(self->map, i)) {\
			CSTL_VECTOR_AT(self->map, i) = Name##_pop_ring(self);\
			if (!CSTL_VECTOR_AT(self->map, i)) {\
				return 0;\
			}\
		} else {\
			Name##__Ring_clear(CSTL_VECTOR_AT(self->map, i));\
		}\
	}\
	return 1;\
}\
\
static int Name##__Ring_push_back_no_elem(Name##__Ring *self)\
{\
	if (CSTL_RING_FULL(self)) return 0;\
	self->end = CSTL_RING_NEXT(self, self->end);\
	return 1;\
}\
\
static int Name##__Ring_push_front_no_elem(Name##__Ring *self)\
{\
	if (CSTL_RING_FULL(self)) return 0;\
	self->begin = CSTL_RING_PREV(self, self->begin);\
	return 1;\
}\
\
static void Name##__Ring_pop_back_no_elem(Name##__Ring *self)\
{\
	self->end = CSTL_RING_PREV(self, self->end);\
}\
\
static void Name##__Ring_pop_front_no_elem(Name##__Ring *self)\
{\
	self->begin = CSTL_RING_NEXT(self, self->begin);\
}\
\
static void Name##_push_front_n_no_elem(Name *self, size_t n)\
{\
	size_t i;\
	for (i = 0; i < n; i++) {\
		if (!Name##__Ring_push_front_no_elem(CSTL_VECTOR_AT(self->map, self->begin))) {\
			self->begin--;\
			assert(CSTL_RING_EMPTY(CSTL_VECTOR_AT(self->map, self->begin)) && "Deque_push_front_n_no_elem");\
			Name##__Ring_push_front_no_elem(CSTL_VECTOR_AT(self->map, self->begin));\
		}\
	}\
	self->nelems += n;\
}\
\
static void Name##_push_back_n_no_elem(Name *self, size_t n)\
{\
	size_t i;\
	for (i = 0; i < n; i++) {\
		if (!Name##__Ring_push_back_no_elem(CSTL_VECTOR_AT(self->map, self->end - 1))) {\
			self->end++;\
			assert(CSTL_RING_EMPTY(CSTL_VECTOR_AT(self->map, self->end - 1)) && "Deque_push_back_n_no_elem");\
			Name##__Ring_push_back_no_elem(CSTL_VECTOR_AT(self->map, self->end - 1));\
		}\
	}\
	self->nelems += n;\
}\
\
static void Name##_move_forward(Name *self, size_t first, size_t last, size_t n)\
{\
	size_t i;\
	for (i = last; i > first; i--) {\
		*Name##_at(self, i - 1 + n) = *Name##_at(self, i - 1);\
	}\
}\
\
static void Name##_move_backward(Name *self, size_t first, size_t last, size_t n)\
{\
	size_t i;\
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
	self->map = Name##__RingVector_new(CSTL_DEQUE_INITIAL_MAP_SIZE);\
	if (!self->map) {\
		free(self);\
		return 0;\
	}\
	Name##__RingVector_resize(self->map, CSTL_DEQUE_INITIAL_MAP_SIZE, 0);\
	self->pool = Name##__RingVector_new(CSTL_DEQUE_INITIAL_MAP_SIZE);\
	if (!self->pool) {\
		Name##__RingVector_delete(self->map);\
		free(self);\
		return 0;\
	}\
	self->begin = CSTL_VECTOR_SIZE(self->map) / 2;\
	self->end = self->begin + 1;\
	self->nelems = 0;\
	CSTL_VECTOR_AT(self->map, self->begin) = Name##__Ring_new(CSTL_DEQUE_RINGBUF_SIZE(Type));\
	if (!CSTL_VECTOR_AT(self->map, self->begin)) {\
		Name##__RingVector_delete(self->map);\
		Name##__RingVector_delete(self->pool);\
		free(self);\
		return 0;\
	}\
	CSTL_DEQUE_MAGIC(self->magic = self;)\
	return self;\
}\
\
void Name##_delete(Name *self)\
{\
	size_t i;\
	assert(self && "Deque_delete");\
	assert(self->magic == self && "Deque_delete");\
	for (i = 0; i < CSTL_VECTOR_SIZE(self->map); i++) {\
		if (CSTL_VECTOR_AT(self->map, i)) {\
			Name##__Ring_delete(CSTL_VECTOR_AT(self->map, i));\
		}\
	}\
	while (!CSTL_VECTOR_EMPTY(self->pool)) {\
		Name##__Ring_delete(Name##__RingVector_pop_back(self->pool));\
	}\
	Name##__RingVector_delete(self->map);\
	Name##__RingVector_delete(self->pool);\
	CSTL_DEQUE_MAGIC(self->magic = 0;)\
	free(self);\
}\
\
int Name##_assign(Name *self, Name *x, size_t idx, size_t n)\
{\
	size_t i;\
	assert(self && "Deque_assign");\
	assert(self->magic == self && "Deque_assign");\
	assert(x && "Deque_assign");\
	assert(x->magic == x && "Deque_assign");\
	assert(Name##_size(x) >= idx + n && "Deque_assign");\
	assert(Name##_size(x) >= n && "Deque_assign");\
	assert(Name##_size(x) > idx && "Deque_assign");\
	if (self == x) {\
		if (Name##_size(self) > idx + n) {\
			Name##_erase(self, idx + n, Name##_size(self) - (idx + n));\
		}\
		if (!Name##_empty(self)) {\
			Name##_erase(self, 0, idx);\
		}\
	} else {\
		if (n > Name##_size(self)) {\
			if (!Name##_expand_end_side(self, n - Name##_size(self))) {\
				return 0;\
			}\
		}\
		self->end = self->begin + 1;\
		self->nelems = 0;\
		Name##__Ring_clear(CSTL_VECTOR_AT(self->map, self->begin));\
		for (i = 0; i < n; i++) {\
			Name##_push_back(self, *Name##_at(x, i));\
		}\
	}\
	return 1;\
}\
\
int Name##_push_back(Name *self, Type elem)\
{\
	assert(self && "Deque_push_back");\
	assert(self->magic == self && "Deque_push_back");\
	if (CSTL_RING_FULL(CSTL_VECTOR_AT(self->map, self->end - 1))) {\
		if (!Name##_expand_end_side(self, 1)) {\
			return 0;\
		}\
		Name##__Ring_push_back(CSTL_VECTOR_AT(self->map, self->end), elem);\
		self->end++;\
	} else {\
		Name##__Ring_push_back(CSTL_VECTOR_AT(self->map, self->end - 1), elem);\
	}\
	self->nelems++;\
	return 1;\
}\
\
int Name##_push_front(Name *self, Type elem)\
{\
	assert(self && "Deque_push_front");\
	assert(self->magic == self && "Deque_push_front");\
	if (CSTL_RING_FULL(CSTL_VECTOR_AT(self->map, self->begin))) {\
		if (!Name##_expand_begin_side(self, 1)) {\
			return 0;\
		}\
		Name##__Ring_push_front(CSTL_VECTOR_AT(self->map, self->begin - 1), elem);\
		self->begin--;\
	} else {\
		Name##__Ring_push_front(CSTL_VECTOR_AT(self->map, self->begin), elem);\
	}\
	self->nelems++;\
	return 1;\
}\
\
Type Name##_pop_front(Name *self)\
{\
	Type elem;\
	assert(self && "Deque_pop_front");\
	assert(self->magic == self && "Deque_pop_front");\
	assert(!Name##_empty(self) && "Deque_pop_front");\
	elem = CSTL_RING_FRONT(CSTL_VECTOR_AT(self->map, self->begin));\
	Name##__Ring_pop_front_no_elem(CSTL_VECTOR_AT(self->map, self->begin));\
	self->nelems--;\
	if (CSTL_RING_EMPTY(CSTL_VECTOR_AT(self->map, self->begin)) && self->nelems > 0) {\
		Name##_push_ring(self, CSTL_VECTOR_AT(self->map, self->begin));\
		CSTL_VECTOR_AT(self->map, self->begin) = 0;\
		self->begin++;\
		assert(self->begin < self->end && "Deque_pop_front");\
	}\
	return elem;\
}\
\
Type Name##_pop_back(Name *self)\
{\
	Type elem;\
	assert(self && "Deque_pop_back");\
	assert(self->magic == self && "Deque_pop_back");\
	assert(!Name##_empty(self) && "Deque_pop_back");\
	elem = CSTL_RING_BACK(CSTL_VECTOR_AT(self->map, self->end - 1));\
	Name##__Ring_pop_back_no_elem(CSTL_VECTOR_AT(self->map, self->end - 1));\
	self->nelems--;\
	if (CSTL_RING_EMPTY(CSTL_VECTOR_AT(self->map, self->end - 1)) && self->nelems > 0) {\
		Name##_push_ring(self, CSTL_VECTOR_AT(self->map, self->end - 1));\
		CSTL_VECTOR_AT(self->map, self->end - 1) = 0;\
		self->end--;\
		assert(self->begin < self->end && "Deque_pop_back");\
	}\
	return elem;\
}\
\
size_t Name##_size(Name *self)\
{\
	assert(self && "Deque_size");\
	assert(self->magic == self && "Deque_size");\
	return self->nelems;\
}\
\
int Name##_empty(Name *self)\
{\
	assert(self && "Deque_empty");\
	assert(self->magic == self && "Deque_empty");\
	return (self->nelems == 0);\
}\
\
void Name##_clear(Name *self)\
{\
	size_t i;\
	assert(self && "Deque_clear");\
	assert(self->magic == self && "Deque_clear");\
	self->end = self->begin + 1;\
	self->nelems = 0;\
	Name##__Ring_clear(CSTL_VECTOR_AT(self->map, self->begin));\
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
Type Name##_front(Name *self)\
{\
	assert(self && "Deque_front");\
	assert(self->magic == self && "Deque_front");\
	assert(!Name##_empty(self) && "Deque_front");\
	return CSTL_RING_FRONT(CSTL_VECTOR_AT(self->map, self->begin));\
}\
\
Type Name##_back(Name *self)\
{\
	assert(self && "Deque_back");\
	assert(self->magic == self && "Deque_back");\
	assert(!Name##_empty(self) && "Deque_back");\
	return CSTL_RING_BACK(CSTL_VECTOR_AT(self->map, self->end - 1));\
}\
\
int Name##_insert(Name *self, size_t idx, Type elem)\
{\
	assert(self && "Deque_insert");\
	assert(self->magic == self && "Deque_insert");\
	assert(Name##_size(self) >= idx && "Deque_insert");\
	return Name##_insert_array(self, idx, &elem, 1);\
}\
\
int Name##_insert_array(Name *self, size_t idx, Type *elems, size_t n)\
{\
	size_t i;\
	assert(self && "Deque_insert_array");\
	assert(self->magic == self && "Deque_insert_array");\
	assert(Name##_size(self) >= idx && "Deque_insert_array");\
	assert(elems && "Deque_insert_array");\
	if (Name##_size(self) / 2 < idx) {\
		/* end側を移動 */\
		size_t s;\
		if (!Name##_expand_end_side(self, n)) {\
			return 0;\
		}\
		s = Name##_size(self);\
		Name##_push_back_n_no_elem(self, n);\
		Name##_move_forward(self, idx, s, n);\
	} else {\
		/* begin側を移動 */\
		if (!Name##_expand_begin_side(self, n)) {\
			return 0;\
		}\
		Name##_push_front_n_no_elem(self, n);\
		Name##_move_backward(self, n, n + idx, n);\
	}\
	for (i = 0; i < n; i++) {\
		*Name##_at(self, idx + i) = elems[i];\
	}\
	return 1;\
}\
\
void Name##_erase(Name *self, size_t idx, size_t n)\
{\
	size_t i, j, k;\
	assert(self && "Deque_erase");\
	assert(self->magic == self && "Deque_erase");\
	assert(Name##_size(self) >= idx + n && "Deque_erase");\
	assert(Name##_size(self) >= n && "Deque_erase");\
	assert(Name##_size(self) > idx && "Deque_erase");\
	if (!n) return;\
	if (idx >= Name##_size(self) - (idx + n)) {\
		/* end側を移動 */\
		Name##_move_backward(self, idx + n, Name##_size(self), n);\
		Name##_coordinate(self, Name##_size(self) - n, &i, &j);\
		assert(i >= self->begin && "Deque_erase");\
		if (i == self->begin || j != 0) {\
			Name##__Ring_erase(CSTL_VECTOR_AT(self->map, i), j, \
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
		Name##_move_forward(self, 0, idx, n);\
		Name##_coordinate(self, n, &i, &j);\
		self->begin = i;\
		Name##__Ring_erase(CSTL_VECTOR_AT(self->map, i), 0, j);\
		assert(self->begin < self->end && "Deque_erase2");\
		for (k = self->begin; k > 0 && CSTL_VECTOR_AT(self->map, k - 1); k--) {\
			Name##_push_ring(self, CSTL_VECTOR_AT(self->map, k - 1));\
			CSTL_VECTOR_AT(self->map, k - 1) = 0;\
		}\
	}\
	self->nelems -= n;\
}\
\
int Name##_resize(Name *self, size_t n, Type elem)\
{\
	size_t i;\
	size_t size;\
	assert(self && "Deque_resize");\
	assert(self->magic == self && "Deque_resize");\
	size = Name##_size(self);\
	if (size >= n) {\
		Name##_erase(self, n, size - n);\
	} else {\
		if (!Name##_expand_end_side(self, n - size)) {\
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
	Name##__RingVector *tmp_map;\
	Name##__RingVector *tmp_pool;\
	assert(self && "Deque_swap");\
	assert(x && "Deque_swap");\
	assert(self->magic == self && "Deque_swap");\
	assert(x->magic == x && "Deque_swap");\
	tmp_begin = self->begin;\
	tmp_end = self->end;\
	tmp_nelems = self->nelems;\
	tmp_map = self->map;\
	tmp_pool = self->pool;\
	self->begin = x->begin;\
	self->end = x->end;\
	self->nelems = x->nelems;\
	self->map = x->map;\
	self->pool = x->pool;\
	x->begin = tmp_begin;\
	x->end = tmp_end;\
	x->nelems = tmp_nelems;\
	x->map = tmp_map;\
	x->pool = tmp_pool;\
}\
\
CSTL_ALGORITHM_SORT(Name, Type, *Name##_at)\


#endif /* CSTL_DEQUE_H_INCLUDED */
