/* 
 * Copyright (c) 2006-2010, KATO Noriaki
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
#include "common.h"
#include "ring.h"
#include "vector.h"


#ifndef CSTL_ALGORITHM_INTERFACE
#define CSTL_ALGORITHM_INTERFACE(Name, Type)
#endif

#ifndef CSTL_ALGORITHM_IMPLEMENT
#define CSTL_ALGORITHM_IMPLEMENT(Name, Type, DIRECT_ACCESS)
#endif


#define CSTL_DEQUE_SIZE(self)	(self)->size

#define CSTL_DEQUE_SELF(Name, internaldata)	((Name *) ((internaldata).data2))
#define CSTL_DEQUE_SELF_ASSIGN(internaldata)	((internaldata).data2)
#define CSTL_DEQUE_IDX(internaldata)			((internaldata).data3)

/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_DEQUE_INTERFACE(Name, Type)	\
typedef struct Name Name;\
\
CSTL_DEFINE_ITERATOR(Name, Type, Type)\
\
typedef void (*Name##_delete_t)(Name *self);\
typedef int (*Name##_push_back_t)(Name *self, Type data);\
typedef int (*Name##_push_front_t)(Name *self, Type data);\
typedef int (*Name##_push_back_ref_t)(Name *self, Type const *data);\
typedef int (*Name##_push_front_ref_t)(Name *self, Type const *data);\
typedef void (*Name##_pop_front_t)(Name *self);\
typedef void (*Name##_pop_back_t)(Name *self);\
typedef int (*Name##_empty_t)(Name *self);\
typedef size_t (*Name##_size_t)(Name *self);\
typedef void (*Name##_clear_t)(Name *self);\
typedef Type *(*Name##_at_t)(Name *self, size_t idx);\
typedef Type *(*Name##_front_t)(Name *self);\
typedef Type *(*Name##_back_t)(Name *self);\
typedef Name##Iterator (*Name##_begin_t)(Name *self);\
typedef Name##Iterator (*Name##_end_t)(Name *self);\
typedef Name##ReverseIterator (*Name##_rbegin_t)(Name *self);\
typedef Name##ReverseIterator (*Name##_rend_t)(Name *self);\
/*typedef Name##Iterator (*Name##_insert_t)(Name *self, CstlIterInternalData pos, Type data);\
typedef Name##Iterator (*Name##_insert_ref_t)(Name *self, CstlIterInternalData pos, Type const *data);*/\
typedef int (*Name##_insert_t)(Name *self, CstlIterInternalData pos, Type data, Name##Iterator *iter);\
typedef int (*Name##_insert_ref_t)(Name *self, CstlIterInternalData pos, Type const *data, Name##Iterator *iter);\
typedef int (*Name##_insert_n_t)(Name *self, CstlIterInternalData pos, size_t n, Type data);\
typedef int (*Name##_insert_n_ref_t)(Name *self, CstlIterInternalData pos, size_t n, Type const *data);\
typedef int (*Name##_insert_array_t)(Name *self, CstlIterInternalData pos, Type const *data, size_t n);\
typedef int (*Name##_insert_range_t)(Name *self, CstlIterInternalData pos, CstlIterInternal first, CstlIterInternal last);\
typedef Name##Iterator (*Name##_erase_t)(Name *self, CstlIterInternalData pos);\
typedef Name##Iterator (*Name##_erase_range_t)(Name *self, CstlIterInternalData first, CstlIterInternalData last);\
typedef int (*Name##_resize_t)(Name *self, size_t n, Type data);\
typedef void (*Name##_swap_t)(Name *self, Name *x);\
\
struct Name##Vtable {\
	Name##_delete_t         delete_;\
	Name##_push_back_t      push_back;\
	Name##_push_front_t     push_front;\
	Name##_push_back_ref_t  push_back_ref;\
	Name##_push_front_ref_t push_front_ref;\
	Name##_pop_front_t      pop_front;\
	Name##_pop_back_t       pop_back;\
	Name##_empty_t          empty;\
	Name##_size_t           size;\
	Name##_clear_t          clear;\
	Name##_at_t             at;\
	Name##_front_t          front;\
	Name##_back_t           back;\
	Name##_begin_t          begin;\
	Name##_end_t            end;\
	Name##_rbegin_t         rbegin;\
	Name##_rend_t           rend;\
	Name##_insert_t         insert;\
	Name##_insert_ref_t     insert_ref;\
	Name##_insert_n_t       insert_n;\
	Name##_insert_n_ref_t   insert_n_ref;\
	Name##_insert_array_t   insert_array;\
	Name##_insert_range_t   insert_range;\
	Name##_erase_t          erase;\
	Name##_erase_range_t    erase_range;\
	Name##_resize_t         resize;\
	Name##_swap_t           swap;\
};\
\
/*! \
 * \brief deque構造体\
 */\
struct Name {\
	union {\
		const struct Name##Vtable *vptr;\
	} u;\
	size_t begin;\
	size_t end;\
	size_t size;\
	struct Name##_RingVector *map;\
	struct Name##_RingVector *pool;\
	CSTL_MAGIC(Name *magic;)\
};\
\
CSTL_EXTERN_C_BEGIN()\
Name *Name##_new(void);\
void Name##_delete(Name *self);\
int Name##_push_back(Name *self, Type data);\
int Name##_push_front(Name *self, Type data);\
int Name##_push_back_ref(Name *self, Type const *data);\
int Name##_push_front_ref(Name *self, Type const *data);\
void Name##_pop_front(Name *self);\
void Name##_pop_back(Name *self);\
size_t Name##_size(Name *self);\
int Name##_empty(Name *self);\
void Name##_clear(Name *self);\
Type *Name##_at(Name *self, size_t idx);\
Type *Name##_front(Name *self);\
Type *Name##_back(Name *self);\
/*int Name##_insert(Name *self, size_t idx, Type data);\
int Name##_insert_ref(Name *self, size_t idx, Type const *data);\
int Name##_insert_n(Name *self, size_t idx, size_t n, Type data);\
int Name##_insert_n_ref(Name *self, size_t idx, size_t n, Type const *data);\
int Name##_insert_array(Name *self, size_t idx, Type const *data, size_t n);\
int Name##_insert_range(Name *self, size_t idx, Name *x, size_t xidx, size_t n);\
void Name##_erase(Name *self, size_t idx, size_t n);*/\
Name##Iterator Name##_begin(Name *self);\
Name##Iterator Name##_end(Name *self);\
Name##ReverseIterator Name##_rbegin(Name *self);\
Name##ReverseIterator Name##_rend(Name *self);\
int Name##_insert(Name *self, CstlIterInternalData pos, Type data, Name##Iterator *iter);\
int Name##_insert_ref(Name *self, CstlIterInternalData pos, Type const *data, Name##Iterator *iter);\
int Name##_insert_n(Name *self, CstlIterInternalData pos, size_t n, Type data);\
int Name##_insert_n_ref(Name *self, CstlIterInternalData pos, size_t n, Type const *data);\
int Name##_insert_array(Name *self, CstlIterInternalData pos, Type const *data, size_t n);\
int Name##_insert_range(Name *self, CstlIterInternalData pos, CstlIterInternal first, CstlIterInternal last);\
Name##Iterator Name##_erase(Name *self, CstlIterInternalData pos);\
Name##Iterator Name##_erase_range(Name *self, CstlIterInternalData first, CstlIterInternalData last);\
int Name##_resize(Name *self, size_t n, Type data);\
void Name##_swap(Name *self, Name *x);\
Type *Name##Iterator_data(CstlIterInternalData pos);\
Name##Iterator Name##Iterator_next(CstlIterInternalData pos);\
Name##Iterator Name##Iterator_prev(CstlIterInternalData pos);\
void Name##Iterator_inc(CstlIterInternalData *pos);\
void Name##Iterator_dec(CstlIterInternalData *pos);\
int Name##Iterator_eq(CstlIterInternalData pos, CstlIterInternalData x);\
int Name##Iterator_ne(CstlIterInternalData pos, CstlIterInternalData x);\
Type *Name##Iterator_at(CstlIterInternalData pos, long n);\
Name##Iterator Name##Iterator_add(CstlIterInternalData pos, long n);\
Name##Iterator Name##Iterator_sub(CstlIterInternalData pos, long n);\
void Name##Iterator_inc_n(CstlIterInternalData *pos, long n);\
void Name##Iterator_dec_n(CstlIterInternalData *pos, long n);\
long Name##Iterator_diff(CstlIterInternalData pos, CstlIterInternalData x);\
int Name##Iterator_lt(CstlIterInternalData pos, CstlIterInternalData x);\
int Name##Iterator_le(CstlIterInternalData pos, CstlIterInternalData x);\
int Name##Iterator_gt(CstlIterInternalData pos, CstlIterInternalData x);\
int Name##Iterator_ge(CstlIterInternalData pos, CstlIterInternalData x);\
Name##ReverseIterator Name##Iterator_reverse_iterator(CstlIterInternalData pos);\
Name##Iterator Name##ReverseIterator_base(CstlIterInternalData pos);\
Type *Name##ReverseIterator_data(CstlIterInternalData pos);\
Name##Iterator Name##ReverseIterator_next(CstlIterInternalData pos);\
Name##Iterator Name##ReverseIterator_prev(CstlIterInternalData pos);\
void Name##ReverseIterator_inc(CstlIterInternalData *pos);\
void Name##ReverseIterator_dec(CstlIterInternalData *pos);\
int Name##ReverseIterator_eq(CstlIterInternalData pos, CstlIterInternalData x);\
int Name##ReverseIterator_ne(CstlIterInternalData pos, CstlIterInternalData x);\
Type *Name##ReverseIterator_at(CstlIterInternalData pos, long n);\
Name##Iterator Name##ReverseIterator_add(CstlIterInternalData pos, long n);\
Name##Iterator Name##ReverseIterator_sub(CstlIterInternalData pos, long n);\
void Name##ReverseIterator_inc_n(CstlIterInternalData *pos, long n);\
void Name##ReverseIterator_dec_n(CstlIterInternalData *pos, long n);\
long Name##ReverseIterator_diff(CstlIterInternalData pos, CstlIterInternalData x);\
int Name##ReverseIterator_lt(CstlIterInternalData pos, CstlIterInternalData x);\
int Name##ReverseIterator_le(CstlIterInternalData pos, CstlIterInternalData x);\
int Name##ReverseIterator_gt(CstlIterInternalData pos, CstlIterInternalData x);\
int Name##ReverseIterator_ge(CstlIterInternalData pos, CstlIterInternalData x);\
/*CSTL_ALGORITHM_INTERFACE(Name, Type)*/\
CSTL_EXTERN_C_END()\


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
static const struct Name##_RingVectorVtable Name##_RingVector_vtbl = {0}; /* dummy */\
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
enum {\
	Name##_INITIAL_MAP_SIZE = 8,\
	/* リングバッファのサイズ(必ず2の冪乗となる) */\
	Name##_RINGBUF_SIZE = \
						((sizeof(Type) > 0x100) ? 0x200 / 0x200 :\
						 (sizeof(Type) > 0x080) ? 0x200 / 0x100 :\
						 (sizeof(Type) > 0x040) ? 0x200 / 0x080 :\
						 (sizeof(Type) > 0x020) ? 0x200 / 0x040 :\
						 (sizeof(Type) > 0x010) ? 0x200 / 0x020 :\
						 (sizeof(Type) > 0x008) ? 0x200 / 0x010 :\
						 (sizeof(Type) > 0x004) ? 0x200 / 0x008 :\
						 (sizeof(Type) > 0x002) ? 0x200 / 0x004 :\
						 (sizeof(Type) > 0x001) ? 0x200 / 0x002 :\
						                          0x200 / 0x001)\
};\
\
static Type const *Name##Iterator_key_dummy(CstlIterInternalData pos)\
{\
	CSTL_ASSERT(0 && "DequeIterator_key_dummy");\
	return 0;\
}\
\
static Type *Name##Iterator_val_dummy(CstlIterInternalData pos)\
{\
	CSTL_ASSERT(0 && "DequeIterator_val_dummy");\
	return 0;\
}\
\
static const Name##IteratorVtable Name##Iterator_vtbl = {\
	Name##Iterator_data,\
	Name##Iterator_key_dummy,\
	Name##Iterator_val_dummy,\
	Name##Iterator_next,\
	Name##Iterator_prev,\
	Name##Iterator_inc,\
	Name##Iterator_dec,\
	Name##Iterator_eq,\
	Name##Iterator_ne,\
	Name##Iterator_at,\
	Name##Iterator_add,\
	Name##Iterator_sub,\
	Name##Iterator_inc_n,\
	Name##Iterator_dec_n,\
	Name##Iterator_diff,\
	Name##Iterator_lt,\
	Name##Iterator_le,\
	Name##Iterator_gt,\
	Name##Iterator_ge,\
	Name##Iterator_reverse_iterator,\
	0, /* ReverseIterator_base */\
	CSTL_CONTAINER_DEQUE,\
	1, /* is_rand_iter */\
	0, /* is_reverse_iter */\
};\
\
static const Name##IteratorVtable Name##ReverseIterator_vtbl = {\
	Name##ReverseIterator_data,\
	Name##Iterator_key_dummy,\
	Name##Iterator_val_dummy,\
	Name##ReverseIterator_next,\
	Name##ReverseIterator_prev,\
	Name##ReverseIterator_inc,\
	Name##ReverseIterator_dec,\
	Name##ReverseIterator_eq,\
	Name##ReverseIterator_ne,\
	Name##ReverseIterator_at,\
	Name##ReverseIterator_add,\
	Name##ReverseIterator_sub,\
	Name##ReverseIterator_inc_n,\
	Name##ReverseIterator_dec_n,\
	Name##ReverseIterator_diff,\
	Name##ReverseIterator_lt,\
	Name##ReverseIterator_le,\
	Name##ReverseIterator_gt,\
	Name##ReverseIterator_ge,\
	Name##Iterator_reverse_iterator,\
	Name##ReverseIterator_base, /* ReverseIterator_base */\
	CSTL_CONTAINER_DEQUE,\
	1, /* is_rand_iter */\
	1, /* is_reverse_iter */\
};\
\
static const struct Name##Vtable Name##_vtbl = {\
	Name##_delete,\
	Name##_push_back,\
	Name##_push_front,\
	Name##_push_back_ref,\
	Name##_push_front_ref,\
	Name##_pop_front,\
	Name##_pop_back,\
	Name##_empty,\
	Name##_size,\
	Name##_clear,\
	Name##_at,\
	Name##_front,\
	Name##_back,\
	Name##_begin,\
	Name##_end,\
	Name##_rbegin,\
	Name##_rend,\
	Name##_insert,\
	Name##_insert_ref,\
	Name##_insert_n,\
	Name##_insert_n_ref,\
	Name##_insert_array,\
	Name##_insert_range,\
	Name##_erase,\
	Name##_erase_range,\
	Name##_resize,\
	Name##_swap,\
};\
\
Type *Name##Iterator_data(CstlIterInternalData pos)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_data");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_data");\
	CSTL_ASSERT(CSTL_DEQUE_SIZE(CSTL_DEQUE_SELF(Name, pos)) > CSTL_DEQUE_IDX(pos) && "DequeIterator_data");\
	return Name##_at(CSTL_DEQUE_SELF(Name, pos), CSTL_DEQUE_IDX(pos));\
}\
\
Name##Iterator Name##Iterator_next(CstlIterInternalData pos)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_next");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_next");\
	CSTL_ASSERT(CSTL_DEQUE_SIZE(CSTL_DEQUE_SELF(Name, pos)) > CSTL_DEQUE_IDX(pos) && "DequeIterator_next");\
	iter.vptr = &Name##Iterator_vtbl;\
	CSTL_DEQUE_SELF_ASSIGN(iter.internal.data) = CSTL_DEQUE_SELF(Name, pos);\
	CSTL_DEQUE_IDX(iter.internal.data) = CSTL_DEQUE_IDX(pos) + 1;\
	return iter;\
}\
\
Name##Iterator Name##Iterator_prev(CstlIterInternalData pos)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_prev");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_prev");\
	CSTL_ASSERT(CSTL_DEQUE_IDX(pos) > 0 && "DequeIterator_prev");\
	iter.vptr = &Name##Iterator_vtbl;\
	CSTL_DEQUE_SELF_ASSIGN(iter.internal.data) = CSTL_DEQUE_SELF(Name, pos);\
	CSTL_DEQUE_IDX(iter.internal.data) = CSTL_DEQUE_IDX(pos) - 1;\
	return iter;\
}\
\
void Name##Iterator_inc(CstlIterInternalData *pos)\
{\
	CSTL_ASSERT(pos && "DequeIterator_inc");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, *pos) && "DequeIterator_inc");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, *pos)->magic == CSTL_DEQUE_SELF(Name, *pos) && "DequeIterator_inc");\
	CSTL_ASSERT(CSTL_DEQUE_SIZE(CSTL_DEQUE_SELF(Name, *pos)) > CSTL_DEQUE_IDX(*pos) && "DequeIterator_inc");\
	CSTL_DEQUE_IDX(*pos)++;\
}\
\
void Name##Iterator_dec(CstlIterInternalData *pos)\
{\
	CSTL_ASSERT(pos && "DequeIterator_dec");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, *pos) && "DequeIterator_dec");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, *pos)->magic == CSTL_DEQUE_SELF(Name, *pos) && "DequeIterator_dec");\
	CSTL_ASSERT(CSTL_DEQUE_IDX(*pos) > 0 && "DequeIterator_dec");\
	CSTL_DEQUE_IDX(*pos)--;\
}\
\
int Name##Iterator_eq(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_eq");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_eq");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) == CSTL_DEQUE_SELF(Name, x) && "DequeIterator_eq");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, x)->magic && "DequeIterator_eq");\
	return CSTL_DEQUE_IDX(pos) == CSTL_DEQUE_IDX(x);\
}\
\
int Name##Iterator_ne(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_ne");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_ne");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) == CSTL_DEQUE_SELF(Name, x) && "DequeIterator_ne");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, x)->magic && "DequeIterator_ne");\
	return CSTL_DEQUE_IDX(pos) != CSTL_DEQUE_IDX(x);\
}\
\
Type *Name##Iterator_at(CstlIterInternalData pos, long n)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_at");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_at");\
	CSTL_ASSERT(0 <= (long) CSTL_DEQUE_IDX(pos) + n && \
			(long) CSTL_DEQUE_IDX(pos) + n < (long) CSTL_DEQUE_SIZE(CSTL_DEQUE_SELF(Name, pos)) && \
			"DequeIterator_at");\
	return Name##_at(CSTL_DEQUE_SELF(Name, pos), CSTL_DEQUE_IDX(pos) + n);\
}\
\
Name##Iterator Name##Iterator_add(CstlIterInternalData pos, long n)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_add");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_add");\
	CSTL_ASSERT(0 <= (long) CSTL_DEQUE_IDX(pos) + n && \
			(long) CSTL_DEQUE_IDX(pos) + n <= (long) CSTL_DEQUE_SIZE(CSTL_DEQUE_SELF(Name, pos)) && \
			"DequeIterator_add");\
	iter.vptr = &Name##Iterator_vtbl;\
	CSTL_DEQUE_SELF_ASSIGN(iter.internal.data) = CSTL_DEQUE_SELF(Name, pos);\
	CSTL_DEQUE_IDX(iter.internal.data) = CSTL_DEQUE_IDX(pos) + n;\
	return iter;\
}\
\
Name##Iterator Name##Iterator_sub(CstlIterInternalData pos, long n)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_sub");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_sub");\
	CSTL_ASSERT(0 <= (long) CSTL_DEQUE_IDX(pos) - n && \
			(long) CSTL_DEQUE_IDX(pos) - n <= (long) CSTL_DEQUE_SIZE(CSTL_DEQUE_SELF(Name, pos)) && \
			"DequeIterator_sub");\
	iter.vptr = &Name##Iterator_vtbl;\
	CSTL_DEQUE_SELF_ASSIGN(iter.internal.data) = CSTL_DEQUE_SELF(Name, pos);\
	CSTL_DEQUE_IDX(iter.internal.data) = CSTL_DEQUE_IDX(pos) - n;\
	return iter;\
}\
\
void Name##Iterator_inc_n(CstlIterInternalData *pos, long n)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, *pos) && "DequeIterator_inc_n");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, *pos)->magic == CSTL_DEQUE_SELF(Name, *pos) && "DequeIterator_inc_n");\
	CSTL_ASSERT(0 <= (long) CSTL_DEQUE_IDX(*pos) + n && \
			(long) CSTL_DEQUE_IDX(*pos) + n <= (long) CSTL_DEQUE_SIZE(CSTL_DEQUE_SELF(Name, *pos)) && \
			"DequeIterator_inc_n");\
	CSTL_DEQUE_IDX(*pos) += n;\
}\
\
void Name##Iterator_dec_n(CstlIterInternalData *pos, long n)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, *pos) && "DequeIterator_dec_n");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, *pos)->magic == CSTL_DEQUE_SELF(Name, *pos) && "DequeIterator_dec_n");\
	CSTL_ASSERT(0 <= (long) CSTL_DEQUE_IDX(*pos) - n && \
			(long) CSTL_DEQUE_IDX(*pos) - n <= (long) CSTL_DEQUE_SIZE(CSTL_DEQUE_SELF(Name, *pos)) && \
			"DequeIterator_dec_n");\
	CSTL_DEQUE_IDX(*pos) -= n;\
}\
\
long Name##Iterator_diff(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_diff");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_diff");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) == CSTL_DEQUE_SELF(Name, x) && "DequeIterator_diff");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, x)->magic && "DequeIterator_diff");\
	return (long) CSTL_DEQUE_IDX(pos) - (long) CSTL_DEQUE_IDX(x);\
}\
\
int Name##Iterator_lt(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_lt");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_lt");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) == CSTL_DEQUE_SELF(Name, x) && "DequeIterator_lt");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, x)->magic && "DequeIterator_lt");\
	return CSTL_DEQUE_IDX(pos) < CSTL_DEQUE_IDX(x);\
}\
\
int Name##Iterator_le(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_le");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_le");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) == CSTL_DEQUE_SELF(Name, x) && "DequeIterator_le");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, x)->magic && "DequeIterator_le");\
	return CSTL_DEQUE_IDX(pos) <= CSTL_DEQUE_IDX(x);\
}\
\
int Name##Iterator_gt(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_gt");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_gt");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) == CSTL_DEQUE_SELF(Name, x) && "DequeIterator_gt");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, x)->magic && "DequeIterator_gt");\
	return CSTL_DEQUE_IDX(pos) > CSTL_DEQUE_IDX(x);\
}\
\
int Name##Iterator_ge(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_ge");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_ge");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) == CSTL_DEQUE_SELF(Name, x) && "DequeIterator_ge");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, x)->magic && "DequeIterator_ge");\
	return CSTL_DEQUE_IDX(pos) >= CSTL_DEQUE_IDX(x);\
}\
\
Name##ReverseIterator Name##Iterator_reverse_iterator(CstlIterInternalData pos)\
{\
	Name##ReverseIterator iter;\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_reverse_iterator");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeIterator_reverse_iterator");\
	iter.vptr = &Name##ReverseIterator_vtbl;\
	iter.internal.data = pos;\
	return iter;\
}\
\
Name##Iterator Name##ReverseIterator_base(CstlIterInternalData pos)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_base");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_base");\
	iter.vptr = &Name##Iterator_vtbl;\
	iter.internal.data = pos;\
	return iter;\
}\
\
Type *Name##ReverseIterator_data(CstlIterInternalData pos)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_data");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_data");\
	CSTL_ASSERT(CSTL_DEQUE_IDX(pos) > 0 && "DequeReverseIterator_data");\
	return Name##_at(CSTL_DEQUE_SELF(Name, pos), CSTL_DEQUE_IDX(pos) - 1);\
}\
\
Name##Iterator Name##ReverseIterator_next(CstlIterInternalData pos)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_next");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_next");\
	CSTL_ASSERT(CSTL_DEQUE_IDX(pos) > 0 && "DequeReverseIterator_next");\
	iter.vptr = &Name##ReverseIterator_vtbl;\
	CSTL_DEQUE_SELF_ASSIGN(iter.internal.data) = CSTL_DEQUE_SELF(Name, pos);\
	CSTL_DEQUE_IDX(iter.internal.data) = CSTL_DEQUE_IDX(pos) - 1;\
	return iter;\
}\
\
Name##Iterator Name##ReverseIterator_prev(CstlIterInternalData pos)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_prev");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_prev");\
	CSTL_ASSERT(CSTL_DEQUE_SIZE(CSTL_DEQUE_SELF(Name, pos)) > CSTL_DEQUE_IDX(pos) && "DequeReverseIterator_prev");\
	iter.vptr = &Name##ReverseIterator_vtbl;\
	CSTL_DEQUE_SELF_ASSIGN(iter.internal.data) = CSTL_DEQUE_SELF(Name, pos);\
	CSTL_DEQUE_IDX(iter.internal.data) = CSTL_DEQUE_IDX(pos) + 1;\
	return iter;\
}\
\
void Name##ReverseIterator_inc(CstlIterInternalData *pos)\
{\
	CSTL_ASSERT(pos && "DequeReverseIterator_inc");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, *pos) && "DequeReverseIterator_inc");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, *pos)->magic == CSTL_DEQUE_SELF(Name, *pos) && "DequeReverseIterator_inc");\
	CSTL_ASSERT(CSTL_DEQUE_IDX(*pos) > 0 && "DequeReverseIterator_inc");\
	CSTL_DEQUE_IDX(*pos)--;\
}\
\
void Name##ReverseIterator_dec(CstlIterInternalData *pos)\
{\
	CSTL_ASSERT(pos && "DequeReverseIterator_dec");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, *pos) && "DequeReverseIterator_dec");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, *pos)->magic == CSTL_DEQUE_SELF(Name, *pos) && "DequeReverseIterator_dec");\
	CSTL_ASSERT(CSTL_DEQUE_SIZE(CSTL_DEQUE_SELF(Name, *pos)) > CSTL_DEQUE_IDX(*pos) && "DequeReverseIterator_dec");\
	CSTL_DEQUE_IDX(*pos)++;\
}\
\
int Name##ReverseIterator_eq(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_eq");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_eq");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) == CSTL_DEQUE_SELF(Name, x) && "DequeReverseIterator_eq");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, x)->magic && "DequeReverseIterator_eq");\
	return CSTL_DEQUE_IDX(pos) == CSTL_DEQUE_IDX(x);\
}\
\
int Name##ReverseIterator_ne(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_ne");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_ne");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) == CSTL_DEQUE_SELF(Name, x) && "DequeReverseIterator_ne");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, x)->magic && "DequeReverseIterator_ne");\
	return CSTL_DEQUE_IDX(pos) != CSTL_DEQUE_IDX(x);\
}\
\
Type *Name##ReverseIterator_at(CstlIterInternalData pos, long n)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_at");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_at");\
	CSTL_ASSERT(0 <= (long) CSTL_DEQUE_IDX(pos) - n - 1 && \
			(long) CSTL_DEQUE_IDX(pos) - n - 1 < (long) CSTL_DEQUE_SIZE(CSTL_DEQUE_SELF(Name, pos)) && \
			"DequeReverseIterator_at");\
	return Name##_at(CSTL_DEQUE_SELF(Name, pos), CSTL_DEQUE_IDX(pos) - n - 1);\
}\
\
Name##Iterator Name##ReverseIterator_add(CstlIterInternalData pos, long n)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_add");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_add");\
	CSTL_ASSERT(0 <= (long) CSTL_DEQUE_IDX(pos) - n - 1 && \
			(long) CSTL_DEQUE_IDX(pos) - n - 1 <= (long) CSTL_DEQUE_SIZE(CSTL_DEQUE_SELF(Name, pos)) && \
			"DequeReverseIterator_add");\
	iter.vptr = &Name##ReverseIterator_vtbl;\
	CSTL_DEQUE_SELF_ASSIGN(iter.internal.data) = CSTL_DEQUE_SELF(Name, pos);\
	CSTL_DEQUE_IDX(iter.internal.data) = CSTL_DEQUE_IDX(pos) - n;\
	return iter;\
}\
\
Name##Iterator Name##ReverseIterator_sub(CstlIterInternalData pos, long n)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_sub");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_sub");\
	CSTL_ASSERT(0 <= (long) CSTL_DEQUE_IDX(pos) + n - 1 && \
			(long) CSTL_DEQUE_IDX(pos) + n - 1 <= (long) CSTL_DEQUE_SIZE(CSTL_DEQUE_SELF(Name, pos)) && \
			"DequeReverseIterator_sub");\
	iter.vptr = &Name##ReverseIterator_vtbl;\
	CSTL_DEQUE_SELF_ASSIGN(iter.internal.data) = CSTL_DEQUE_SELF(Name, pos);\
	CSTL_DEQUE_IDX(iter.internal.data) = CSTL_DEQUE_IDX(pos) + n;\
	return iter;\
}\
\
void Name##ReverseIterator_inc_n(CstlIterInternalData *pos, long n)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, *pos) && "DequeReverseIterator_inc_n");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, *pos)->magic == CSTL_DEQUE_SELF(Name, *pos) && "DequeReverseIterator_inc_n");\
	CSTL_ASSERT(0 <= (long) CSTL_DEQUE_IDX(*pos) - n - 1 && \
			(long) CSTL_DEQUE_IDX(*pos) - n - 1 <= (long) CSTL_DEQUE_SIZE(CSTL_DEQUE_SELF(Name, *pos)) && \
			"DequeReverseIterator_inc_n");\
	CSTL_DEQUE_IDX(*pos) -= n;\
}\
\
void Name##ReverseIterator_dec_n(CstlIterInternalData *pos, long n)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, *pos) && "DequeReverseIterator_dec_n");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, *pos)->magic == CSTL_DEQUE_SELF(Name, *pos) && "DequeReverseIterator_dec_n");\
	CSTL_ASSERT(0 <= (long) CSTL_DEQUE_IDX(*pos) + n - 1 && \
			(long) CSTL_DEQUE_IDX(*pos) + n - 1 <= (long) CSTL_DEQUE_SIZE(CSTL_DEQUE_SELF(Name, *pos)) && \
			"DequeReverseIterator_dec_n");\
	CSTL_DEQUE_IDX(*pos) += n;\
}\
\
long Name##ReverseIterator_diff(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_diff");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_diff");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) == CSTL_DEQUE_SELF(Name, x) && "DequeReverseIterator_diff");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, x)->magic && "DequeReverseIterator_diff");\
	return (long) CSTL_DEQUE_IDX(x) - (long) CSTL_DEQUE_IDX(pos);\
}\
\
int Name##ReverseIterator_lt(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_lt");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_lt");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) == CSTL_DEQUE_SELF(Name, x) && "DequeReverseIterator_lt");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, x)->magic && "DequeReverseIterator_lt");\
	return CSTL_DEQUE_IDX(pos) > CSTL_DEQUE_IDX(x);\
}\
\
int Name##ReverseIterator_le(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_le");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_le");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) == CSTL_DEQUE_SELF(Name, x) && "DequeReverseIterator_le");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, x)->magic && "DequeReverseIterator_le");\
	return CSTL_DEQUE_IDX(pos) >= CSTL_DEQUE_IDX(x);\
}\
\
int Name##ReverseIterator_gt(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_gt");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_gt");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) == CSTL_DEQUE_SELF(Name, x) && "DequeReverseIterator_gt");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, x)->magic && "DequeReverseIterator_gt");\
	return CSTL_DEQUE_IDX(pos) < CSTL_DEQUE_IDX(x);\
}\
\
int Name##ReverseIterator_ge(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_ge");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, pos) && "DequeReverseIterator_ge");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) == CSTL_DEQUE_SELF(Name, x) && "DequeReverseIterator_ge");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos)->magic == CSTL_DEQUE_SELF(Name, x)->magic && "DequeReverseIterator_ge");\
	return CSTL_DEQUE_IDX(pos) <= CSTL_DEQUE_IDX(x);\
}\
\
static void Name##_coordinate(Name *self, size_t idx, size_t *map_idx, size_t *ring_idx)\
{\
	size_t n;\
	n = CSTL_RING_SIZE(CSTL_VECTOR_AT(self->map, self->begin));\
	if (idx < n) {\
		*map_idx = self->begin;\
		*ring_idx = idx;\
	} else {\
		*map_idx = (self->begin + 1) + (idx - n) / Name##_RINGBUF_SIZE;\
		*ring_idx = (idx - n) & (Name##_RINGBUF_SIZE - 1);\
	}\
}\
\
static Name##_Ring *Name##_pop_ring(Name *self)\
{\
	if (CSTL_VECTOR_EMPTY(self->pool)) {\
		return Name##_Ring_new(Name##_RINGBUF_SIZE);\
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
/* n個の要素をbegin側から挿入するために、mapの拡張とringの確保をする */\
static int Name##_expand_begin_side(Name *self, size_t n)\
{\
	size_t m;\
	size_t expand;\
	register size_t i;\
	m = Name##_RINGBUF_SIZE - \
		CSTL_RING_SIZE(CSTL_VECTOR_AT(self->map, self->begin));\
	if (n <= m) {\
		return 1;\
	}\
	/* 追加しなければならないringの数 */\
	expand = 1 + (n - m - 1) / Name##_RINGBUF_SIZE;\
	if (self->begin < expand) {\
		if (CSTL_VECTOR_SIZE(self->map) - self->end + self->begin < expand) {\
			/* mapを拡張する */\
			size_t map_expand = (CSTL_VECTOR_SIZE(self->map) > expand) ? CSTL_VECTOR_SIZE(self->map) : expand;\
			if (!Name##_RingVector_insert_n_no_data(self->map, 0, map_expand)) {\
				return 0;\
			}\
			for (i = 0; i < map_expand; i++) {\
				CSTL_VECTOR_AT(self->map, i) = 0;\
			}\
			self->begin += map_expand;\
			self->end += map_expand;\
		} else {\
			/* mapをずらす */\
			size_t slide = ((expand - self->begin) + (CSTL_VECTOR_SIZE(self->map) - self->end)) / 2;\
			CSTL_ASSERT(self->end + slide <= CSTL_VECTOR_SIZE(self->map) && "Deque_expand_begin_side");\
			Name##_RingVector_move_backward(self->map, self->begin, self->end, slide);\
			for (i = self->begin; i < self->begin + slide; i++) {\
				CSTL_VECTOR_AT(self->map, i) = 0;\
			}\
			self->begin += slide;\
			self->end += slide;\
		}\
	}\
	/* expand分のringの確保をしておく。この関数を出た後、必ずself->beginの調整をする。 */\
	for (i = self->begin - expand; i < self->begin; i++) {\
		CSTL_ASSERT(!CSTL_VECTOR_AT(self->map, i) && "Deque_expand_begin_side");\
		CSTL_VECTOR_AT(self->map, i) = Name##_pop_ring(self);\
		if (!CSTL_VECTOR_AT(self->map, i)) {\
			register size_t j;\
			for (j = self->begin - expand; j < i; j++) {\
				Name##_push_ring(self, CSTL_VECTOR_AT(self->map, j));\
				CSTL_VECTOR_AT(self->map, j) = 0;\
			}\
			return 0;\
		}\
	}\
	return 1;\
}\
\
/* n個の要素をend側から挿入するために、mapの拡張とringの確保をする */\
static int Name##_expand_end_side(Name *self, size_t n)\
{\
	size_t m;\
	size_t expand;\
	register size_t i;\
	m = Name##_RINGBUF_SIZE - \
		CSTL_RING_SIZE(CSTL_VECTOR_AT(self->map, self->end - 1));\
	if (n <= m) {\
		return 1;\
	}\
	/* 追加しなければならないringの数 */\
	expand = 1 + (n - m - 1) / Name##_RINGBUF_SIZE;\
	if (CSTL_VECTOR_SIZE(self->map) - self->end < expand) {\
		if (CSTL_VECTOR_SIZE(self->map) - self->end + self->begin < expand) {\
			/* mapを拡張する */\
			size_t map_size = CSTL_VECTOR_SIZE(self->map);\
			size_t map_expand = (map_size > expand) ? map_size : expand;\
			if (!Name##_RingVector_insert_n_no_data(self->map, map_size, map_expand)) {\
				return 0;\
			}\
			for (i = map_size; i < map_size + map_expand; i++) {\
				CSTL_VECTOR_AT(self->map, i) = 0;\
			}\
		} else {\
			/* mapをずらす */\
			size_t slide = ((expand - (CSTL_VECTOR_SIZE(self->map) - self->end)) + self->begin) / 2;\
			CSTL_ASSERT(self->begin >= slide && "Deque_expand_end_side");\
			Name##_RingVector_move_forward(self->map, self->begin, self->end, slide);\
			for (i = self->end - slide; i < self->end; i++) {\
				CSTL_VECTOR_AT(self->map, i) = 0;\
			}\
			self->begin -= slide;\
			self->end -= slide;\
		}\
	}\
	/* expand分のringの確保をしておく。この関数を出た後、必ずself->endの調整をする。 */\
	for (i = self->end; i < self->end + expand; i++) {\
		CSTL_ASSERT(!CSTL_VECTOR_AT(self->map, i) && "Deque_expand_end_side");\
		CSTL_VECTOR_AT(self->map, i) = Name##_pop_ring(self);\
		if (!CSTL_VECTOR_AT(self->map, i)) {\
			register size_t j;\
			for (j = self->end; j < i; j++) {\
				Name##_push_ring(self, CSTL_VECTOR_AT(self->map, j));\
				CSTL_VECTOR_AT(self->map, j) = 0;\
			}\
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
			CSTL_ASSERT(CSTL_RING_EMPTY(CSTL_VECTOR_AT(self->map, self->begin)) && "Deque_push_front_n_no_data");\
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
			CSTL_ASSERT(CSTL_RING_EMPTY(CSTL_VECTOR_AT(self->map, self->end - 1)) && "Deque_push_back_n_no_data");\
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
	self->map = Name##_RingVector_new_reserve(Name##_INITIAL_MAP_SIZE);\
	if (!self->map) {\
		free(self);\
		return 0;\
	}\
	Name##_RingVector_resize(self->map, Name##_INITIAL_MAP_SIZE, 0);\
	self->pool = Name##_RingVector_new_reserve(Name##_INITIAL_MAP_SIZE);\
	if (!self->pool) {\
		Name##_RingVector_delete(self->map);\
		free(self);\
		return 0;\
	}\
	self->u.vptr = &Name##_vtbl;\
	self->begin = CSTL_VECTOR_SIZE(self->map) / 2;\
	self->end = self->begin + 1;\
	self->size = 0;\
	CSTL_VECTOR_AT(self->map, self->begin) = Name##_Ring_new(Name##_RINGBUF_SIZE);\
	if (!CSTL_VECTOR_AT(self->map, self->begin)) {\
		Name##_RingVector_delete(self->map);\
		Name##_RingVector_delete(self->pool);\
		free(self);\
		return 0;\
	}\
	CSTL_MAGIC(self->magic = self);\
	return self;\
}\
\
void Name##_delete(Name *self)\
{\
	register size_t i;\
	if (!self) return;\
	CSTL_ASSERT(self->magic == self && "Deque_delete");\
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
	CSTL_MAGIC(self->magic = 0);\
	free(self);\
}\
\
int Name##_push_back(Name *self, Type data)\
{\
	CSTL_ASSERT(self && "Deque_push_back");\
	CSTL_ASSERT(self->magic == self && "Deque_push_back");\
	return Name##_push_back_ref(self, &data);\
}\
\
int Name##_push_front(Name *self, Type data)\
{\
	CSTL_ASSERT(self && "Deque_push_front");\
	CSTL_ASSERT(self->magic == self && "Deque_push_front");\
	return Name##_push_front_ref(self, &data);\
}\
\
int Name##_push_back_ref(Name *self, Type const *data)\
{\
	CSTL_ASSERT(self && "Deque_push_back_ref");\
	CSTL_ASSERT(self->magic == self && "Deque_push_back_ref");\
	CSTL_ASSERT(data && "Deque_push_back_ref");\
	if (CSTL_RING_FULL(CSTL_VECTOR_AT(self->map, self->end - 1))) {\
		if (!Name##_expand_end_side(self, 1)) {\
			return 0;\
		}\
		self->end++;\
	}\
	Name##_Ring_push_back_ref(CSTL_VECTOR_AT(self->map, self->end - 1), data);\
	self->size++;\
	return 1;\
}\
\
int Name##_push_front_ref(Name *self, Type const *data)\
{\
	CSTL_ASSERT(self && "Deque_push_back_ref");\
	CSTL_ASSERT(self->magic == self && "Deque_push_back_ref");\
	CSTL_ASSERT(data && "Deque_push_back_ref");\
	if (CSTL_RING_FULL(CSTL_VECTOR_AT(self->map, self->begin))) {\
		if (!Name##_expand_begin_side(self, 1)) {\
			return 0;\
		}\
		self->begin--;\
	}\
	Name##_Ring_push_front_ref(CSTL_VECTOR_AT(self->map, self->begin), data);\
	self->size++;\
	return 1;\
}\
\
void Name##_pop_front(Name *self)\
{\
	CSTL_ASSERT(self && "Deque_pop_front");\
	CSTL_ASSERT(self->magic == self && "Deque_pop_front");\
	CSTL_ASSERT(!Name##_empty(self) && "Deque_pop_front");\
	Name##_Ring_pop_front(CSTL_VECTOR_AT(self->map, self->begin));\
	self->size--;\
	if (CSTL_RING_EMPTY(CSTL_VECTOR_AT(self->map, self->begin)) && self->size > 0) {\
		Name##_push_ring(self, CSTL_VECTOR_AT(self->map, self->begin));\
		CSTL_VECTOR_AT(self->map, self->begin) = 0;\
		self->begin++;\
		CSTL_ASSERT(self->begin < self->end && "Deque_pop_front");\
	}\
}\
\
void Name##_pop_back(Name *self)\
{\
	CSTL_ASSERT(self && "Deque_pop_back");\
	CSTL_ASSERT(self->magic == self && "Deque_pop_back");\
	CSTL_ASSERT(!Name##_empty(self) && "Deque_pop_back");\
	Name##_Ring_pop_back(CSTL_VECTOR_AT(self->map, self->end - 1));\
	self->size--;\
	if (CSTL_RING_EMPTY(CSTL_VECTOR_AT(self->map, self->end - 1)) && self->size > 0) {\
		Name##_push_ring(self, CSTL_VECTOR_AT(self->map, self->end - 1));\
		CSTL_VECTOR_AT(self->map, self->end - 1) = 0;\
		self->end--;\
		CSTL_ASSERT(self->begin < self->end && "Deque_pop_back");\
	}\
}\
\
size_t Name##_size(Name *self)\
{\
	CSTL_ASSERT(self && "Deque_size");\
	CSTL_ASSERT(self->magic == self && "Deque_size");\
	return CSTL_DEQUE_SIZE(self);\
}\
\
int Name##_empty(Name *self)\
{\
	CSTL_ASSERT(self && "Deque_empty");\
	CSTL_ASSERT(self->magic == self && "Deque_empty");\
	return (self->size == 0);\
}\
\
void Name##_clear(Name *self)\
{\
	register size_t i;\
	CSTL_ASSERT(self && "Deque_clear");\
	CSTL_ASSERT(self->magic == self && "Deque_clear");\
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
	CSTL_ASSERT(self && "Deque_at");\
	CSTL_ASSERT(self->magic == self && "Deque_at");\
	CSTL_ASSERT(Name##_size(self) > idx && "Deque_at");\
	Name##_coordinate(self, idx, &m, &n);\
	return &CSTL_RING_AT(CSTL_VECTOR_AT(self->map, m), n);\
}\
\
Type *Name##_front(Name *self)\
{\
	CSTL_ASSERT(self && "Deque_front");\
	CSTL_ASSERT(self->magic == self && "Deque_front");\
	CSTL_ASSERT(!Name##_empty(self) && "Deque_front");\
	return &CSTL_RING_FRONT(CSTL_VECTOR_AT(self->map, self->begin));\
}\
\
Type *Name##_back(Name *self)\
{\
	CSTL_ASSERT(self && "Deque_back");\
	CSTL_ASSERT(self->magic == self && "Deque_back");\
	CSTL_ASSERT(!Name##_empty(self) && "Deque_back");\
	return &CSTL_RING_BACK(CSTL_VECTOR_AT(self->map, self->end - 1));\
}\
\
Name##Iterator Name##_begin(Name *self)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(self && "Deque_begin");\
	CSTL_ASSERT(self->magic == self && "Deque_begin");\
	iter.vptr = &Name##Iterator_vtbl;\
	CSTL_VECTOR_SELF_ASSIGN(iter.internal.data) = self;\
	CSTL_VECTOR_IDX(iter.internal.data) = 0;\
	return iter;\
}\
\
Name##Iterator Name##_end(Name *self)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(self && "Deque_end");\
	CSTL_ASSERT(self->magic == self && "Deque_end");\
	iter.vptr = &Name##Iterator_vtbl;\
	CSTL_VECTOR_SELF_ASSIGN(iter.internal.data) = self;\
	CSTL_VECTOR_IDX(iter.internal.data) = self->size;\
	return iter;\
}\
\
Name##ReverseIterator Name##_rbegin(Name *self)\
{\
	Name##ReverseIterator iter;\
	CSTL_ASSERT(self && "Deque_rbegin");\
	CSTL_ASSERT(self->magic == self && "Deque_rbegin");\
	iter.vptr = &Name##ReverseIterator_vtbl;\
	CSTL_VECTOR_SELF_ASSIGN(iter.internal.data) = self;\
	CSTL_VECTOR_IDX(iter.internal.data) = self->size;\
	return iter;\
}\
\
Name##ReverseIterator Name##_rend(Name *self)\
{\
	Name##ReverseIterator iter;\
	CSTL_ASSERT(self && "Deque_rend");\
	CSTL_ASSERT(self->magic == self && "Deque_rend");\
	iter.vptr = &Name##ReverseIterator_vtbl;\
	CSTL_VECTOR_SELF_ASSIGN(iter.internal.data) = self;\
	CSTL_VECTOR_IDX(iter.internal.data) = 0;\
	return iter;\
}\
\
/*int Name##_insert(Name *self, size_t idx, Type data)*/\
int Name##_insert(Name *self, CstlIterInternalData pos, Type data, Name##Iterator *iter)\
{\
	int ret;\
	CSTL_ASSERT(self && "Deque_insert");\
	CSTL_ASSERT(self->magic == self && "Deque_insert");\
	/*CSTL_ASSERT(Name##_size(self) >= idx && "Deque_insert");*/\
	ret = Name##_insert_n_ref(self, pos, 1, &data);\
	if (ret && iter) {\
		iter->vptr = &Name##Iterator_vtbl;\
		CSTL_DEQUE_SELF_ASSIGN(iter->internal.data) = self;\
		CSTL_DEQUE_IDX(iter->internal.data) = CSTL_DEQUE_IDX(pos);\
	}\
	return ret;\
}\
\
/*int Name##_insert_ref(Name *self, size_t idx, Type const *data)*/\
int Name##_insert_ref(Name *self, CstlIterInternalData pos, Type const *data, Name##Iterator *iter)\
{\
	int ret;\
	CSTL_ASSERT(self && "Deque_insert_ref");\
	CSTL_ASSERT(self->magic == self && "Deque_insert_ref");\
	/*CSTL_ASSERT(Name##_size(self) >= idx && "Deque_insert_ref");*/\
	CSTL_ASSERT(data && "Deque_insert_ref");\
	ret = Name##_insert_n_ref(self, pos, 1, data);\
	if (ret && iter) {\
		iter->vptr = &Name##Iterator_vtbl;\
		CSTL_DEQUE_SELF_ASSIGN(iter->internal.data) = self;\
		CSTL_DEQUE_IDX(iter->internal.data) = CSTL_DEQUE_IDX(pos);\
	}\
	return ret;\
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
/*int Name##_insert_n(Name *self, size_t idx, size_t n, Type data)*/\
int Name##_insert_n(Name *self, CstlIterInternalData pos, size_t n, Type data)\
{\
	CSTL_ASSERT(self && "Deque_insert_n");\
	CSTL_ASSERT(self->magic == self && "Deque_insert_n");\
	/*CSTL_ASSERT(Name##_size(self) >= idx && "Deque_insert_n");*/\
	return Name##_insert_n_ref(self, pos, n, &data);\
}\
\
/*int Name##_insert_n_ref(Name *self, size_t idx, size_t n, Type const *data)*/\
int Name##_insert_n_ref(Name *self, CstlIterInternalData pos, size_t n, Type const *data)\
{\
	register size_t i;\
	size_t idx;\
	CSTL_ASSERT(self && "Deque_insert_n_ref");\
	CSTL_ASSERT(self->magic == self && "Deque_insert_n_ref");\
	idx = CSTL_DEQUE_IDX(pos);\
	CSTL_ASSERT(Name##_size(self) >= idx && "Deque_insert_n_ref");\
	CSTL_ASSERT(data && "Deque_insert_n_ref");\
	if (!Name##_insert_n_no_data(self, idx, n)) {\
		return 0;\
	}\
	for (i = 0; i < n; i++) {\
		*Name##_at(self, idx + i) = *data;\
	}\
	return 1;\
}\
\
/*int Name##_insert_array(Name *self, size_t idx, Type const *data, size_t n)*/\
int Name##_insert_array(Name *self, CstlIterInternalData pos, Type const *data, size_t n)\
{\
	register size_t i;\
	size_t idx;\
	CSTL_ASSERT(self && "Deque_insert_array");\
	CSTL_ASSERT(self->magic == self && "Deque_insert_array");\
	idx = CSTL_DEQUE_IDX(pos);\
	CSTL_ASSERT(Name##_size(self) >= idx && "Deque_insert_array");\
	CSTL_ASSERT(data && "Deque_insert_array");\
	if (!Name##_insert_n_no_data(self, idx, n)) {\
		return 0;\
	}\
	for (i = 0; i < n; i++) {\
		*Name##_at(self, idx + i) = data[i];\
	}\
	return 1;\
}\
\
/*int Name##_insert_range(Name *self, size_t idx, Name *x, size_t xidx, size_t n)*/\
int Name##_insert_range(Name *self, CstlIterInternalData pos, CstlIterInternal first, CstlIterInternal last)\
{\
	register size_t i;\
	size_t idx;\
	Name *x;\
	size_t n;\
	CSTL_ASSERT(self && "Deque_insert_range");\
	CSTL_ASSERT(self->magic == self && "Deque_insert_range");\
	CSTL_ASSERT(first.in_vptr == last.in_vptr && "Deque_insert_range");\
	CSTL_ASSERT(CSTL_CAST_VPTR(Name, first.in_vptr)->is_rand_iter == \
				CSTL_CAST_VPTR(Name, last.in_vptr)->is_rand_iter && "Deque_insert_range");\
	idx = CSTL_DEQUE_IDX(pos);\
	CSTL_ASSERT(Name##_size(self) >= idx && "Deque_insert_range");\
	x = CSTL_DEQUE_SELF(Name, first.data);\
	if (self == x) {\
		size_t xidx;\
		xidx = CSTL_DEQUE_IDX(first.data);\
		n = CSTL_DEQUE_IDX(last.data) - CSTL_DEQUE_IDX(first.data);\
		CSTL_ASSERT(Name##_size(x) >= xidx + n && "Deque_insert_range");\
		CSTL_ASSERT(Name##_size(x) >= n && "Deque_insert_range");\
		CSTL_ASSERT(Name##_size(x) > xidx && "Deque_insert_range");\
		if (!Name##_insert_n_no_data(self, idx, n)) {\
			return 0;\
		}\
		/* この場合は[first, last)がdequeとして処理できる。イテレータの仮想関数を使わない */\
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
		CstlIterInternal internal;\
		if (CSTL_CAST_VPTR(Name, first.in_vptr)->is_rand_iter) {\
			CSTL_ASSERT(CSTL_CAST_VPTR(Name, first.in_vptr)->diff(last.data, first.data) >= 0 && "Deque_insert_range");\
			n = (size_t) CSTL_CAST_VPTR(Name, first.in_vptr)->diff(last.data, first.data);\
		} else {\
			for (internal = first, n = 0; CSTL_CAST_VPTR(Name, internal.in_vptr)->ne(internal.data, last.data); \
					CSTL_CAST_VPTR(Name, internal.in_vptr)->inc(&internal.data)) {\
				n++;\
			}\
		}\
		if (!Name##_insert_n_no_data(self, idx, n)) {\
			return 0;\
		}\
		for (internal = first, i = 0; CSTL_CAST_VPTR(Name, internal.in_vptr)->ne(internal.data, last.data); \
				CSTL_CAST_VPTR(Name, internal.in_vptr)->inc(&internal.data), i++) {\
			*Name##_at(self, idx + i) = *CSTL_CAST_VPTR(Name, internal.in_vptr)->data(internal.data);\
		}\
		CSTL_ASSERT(i == n && "Deque_insert_range");\
	}\
	return 1;\
}\
\
static void Name##_erase_aux(Name *self, size_t idx, size_t n)\
{\
	size_t i, j;\
	register size_t k;\
	size_t size;\
	if (!n) return;\
	size = CSTL_DEQUE_SIZE(self);\
	if (idx >= size - (idx + n)) {\
		/* end側を移動 */\
		Name##_move_forward(self, idx + n, size, n);\
		Name##_coordinate(self, size - n, &i, &j);\
		CSTL_ASSERT(i >= self->begin && "Deque_erase_aux");\
		if (i == self->begin || j != 0) {\
			Name##_Ring_erase(CSTL_VECTOR_AT(self->map, i), j, \
					CSTL_RING_SIZE(CSTL_VECTOR_AT(self->map, i)) - j);\
			self->end = i + 1;\
		} else {\
			self->end = i;\
		}\
		CSTL_ASSERT(self->begin < self->end && "Deque_erase_aux1");\
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
		CSTL_ASSERT(self->begin < self->end && "Deque_erase_aux2");\
		for (k = self->begin; k > 0 && CSTL_VECTOR_AT(self->map, k - 1); k--) {\
			Name##_push_ring(self, CSTL_VECTOR_AT(self->map, k - 1));\
			CSTL_VECTOR_AT(self->map, k - 1) = 0;\
		}\
	}\
	self->size -= n;\
}\
\
/*void Name##_erase(Name *self, size_t idx, size_t n)*/\
Name##Iterator Name##_erase(Name *self, CstlIterInternalData pos)\
{\
	Name##Iterator iter;\
	size_t idx;\
	CSTL_ASSERT(self && "Deque_erase");\
	CSTL_ASSERT(self->magic == self && "Deque_erase");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, pos) == self);\
	idx = CSTL_DEQUE_IDX(pos);\
	CSTL_ASSERT(CSTL_DEQUE_SIZE(self) >= idx + 1 && "Deque_erase");\
	CSTL_ASSERT(CSTL_DEQUE_SIZE(self) >= 1 && "Deque_erase");\
	CSTL_ASSERT(CSTL_DEQUE_SIZE(self) > idx && "Deque_erase");\
	Name##_erase_aux(self, idx, 1);\
	iter.vptr = &Name##Iterator_vtbl;\
	CSTL_DEQUE_SELF_ASSIGN(iter.internal.data) = self;\
	CSTL_DEQUE_IDX(iter.internal.data) = idx;\
	return iter;\
}\
\
Name##Iterator Name##_erase_range(Name *self, CstlIterInternalData first, CstlIterInternalData last)\
{\
	Name##Iterator iter;\
	size_t idx;\
	size_t n;\
	CSTL_ASSERT(self && "Deque_erase_range");\
	CSTL_ASSERT(self->magic == self && "Deque_erase_range");\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, first) == CSTL_DEQUE_SELF(Name, last));\
	CSTL_ASSERT(CSTL_DEQUE_SELF(Name, first) == self);\
	CSTL_ASSERT(CSTL_DEQUE_IDX(first) <= CSTL_DEQUE_IDX(last) && "Deque_erase_range");\
	idx = CSTL_DEQUE_IDX(first);\
	n = CSTL_DEQUE_IDX(last) - CSTL_DEQUE_IDX(first);\
	CSTL_ASSERT(CSTL_DEQUE_SIZE(self) >= idx + n && "Deque_erase_range");\
	CSTL_ASSERT(CSTL_DEQUE_SIZE(self) >= n && "Deque_erase_range");\
	CSTL_ASSERT(CSTL_DEQUE_SIZE(self) > idx && "Deque_erase_range");\
	Name##_erase_aux(self, idx, n);\
	iter.vptr = &Name##Iterator_vtbl;\
	CSTL_DEQUE_SELF_ASSIGN(iter.internal.data) = self;\
	CSTL_DEQUE_IDX(iter.internal.data) = idx;\
	return iter;\
}\
\
int Name##_resize(Name *self, size_t n, Type data)\
{\
	register size_t i;\
	size_t size;\
	CSTL_ASSERT(self && "Deque_resize");\
	CSTL_ASSERT(self->magic == self && "Deque_resize");\
	size = CSTL_DEQUE_SIZE(self);\
	if (size >= n) {\
		Name##_erase_aux(self, n, size - n);\
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
	CSTL_ASSERT(self && "Deque_swap");\
	CSTL_ASSERT(x && "Deque_swap");\
	CSTL_ASSERT(self->magic == self && "Deque_swap");\
	CSTL_ASSERT(x->magic == x && "Deque_swap");\
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
