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

#define CSTL_VECTOR_ELEM(Type, internaldata)	((Type *) ((internaldata).data1))
#define CSTL_VECTOR_ELEM_ASSIGN(internaldata)	((internaldata).data1)
#define CSTL_VECTOR_SELF(Name, internaldata)	((Name *) ((internaldata).data2))
#define CSTL_VECTOR_SELF_ASSIGN(internaldata)	((internaldata).data2)
#define CSTL_VECTOR_IDX(internaldata)			((internaldata).data3)

/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_VECTOR_INTERFACE(Name, Type)	\
typedef struct Name Name;\
typedef struct Name##Iterator_Vtable Name##Iterator_Vtable;\
/*! \
 * \brief イテレータ\
 */\
typedef union Name##Iterator {\
	const Name##Iterator_Vtable *vptr;\
	CstlIterInternal internal;\
} Name##Iterator;\
\
typedef Name##Iterator Name##ReverseIterator;\
\
typedef Type *(*Name##Iterator_data_t)(CstlIterInternalData pos);\
typedef void *(*Name##Iterator_key_t)(CstlIterInternalData pos);\
typedef void *(*Name##Iterator_val_t)(CstlIterInternalData pos);\
typedef Name##Iterator (*Name##Iterator_next_t)(CstlIterInternalData pos);\
typedef Name##Iterator (*Name##Iterator_prev_t)(CstlIterInternalData pos);\
typedef void (*Name##Iterator_inc_t)(CstlIterInternalData *pos);\
typedef void (*Name##Iterator_dec_t)(CstlIterInternalData *pos);\
typedef int (*Name##Iterator_eq_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef int (*Name##Iterator_ne_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef Type *(*Name##Iterator_at_t)(CstlIterInternalData pos, size_t n);\
typedef Name##Iterator (*Name##Iterator_add_t)(CstlIterInternalData pos, size_t n);\
typedef Name##Iterator (*Name##Iterator_sub_t)(CstlIterInternalData pos, size_t n);\
typedef void (*Name##Iterator_inc_n_t)(CstlIterInternalData *pos, size_t n);\
typedef void (*Name##Iterator_dec_n_t)(CstlIterInternalData *pos, size_t n);\
typedef int (*Name##Iterator_diff_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef int (*Name##Iterator_lt_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef int (*Name##Iterator_le_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef int (*Name##Iterator_gt_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef int (*Name##Iterator_ge_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef Name##ReverseIterator (*Name##Iterator_reverse_iterator_t)(CstlIterInternalData pos);\
typedef Name##Iterator (*Name##ReverseIterator_base_t)(CstlIterInternalData pos);\
\
struct Name##Iterator_Vtable {\
	Name##Iterator_data_t  data;\
	Name##Iterator_key_t   key;\
	Name##Iterator_val_t   val;\
	Name##Iterator_next_t  next;\
	Name##Iterator_prev_t  prev;\
	Name##Iterator_inc_t   inc;\
	Name##Iterator_dec_t   dec;\
	Name##Iterator_eq_t    eq;\
	Name##Iterator_ne_t    ne;\
	Name##Iterator_at_t    at;\
	Name##Iterator_add_t   add;\
	Name##Iterator_sub_t   sub;\
	Name##Iterator_inc_n_t inc_n;\
	Name##Iterator_dec_n_t dec_n;\
	Name##Iterator_diff_t  diff;\
	Name##Iterator_lt_t    lt;\
	Name##Iterator_le_t    le;\
	Name##Iterator_gt_t    gt;\
	Name##Iterator_ge_t    ge;\
	Name##Iterator_reverse_iterator_t reverse_iterator;\
	Name##ReverseIterator_base_t base;\
	int container;\
	int is_rand_iter;\
	int is_reverse_iter;\
};\
\
typedef void (*Name##_delete_t)(Name *self);\
typedef int (*Name##_push_back_t)(Name *self, Type data);\
/*typedef int (*Name##_push_front_t)(Name *self, Type data);*/\
typedef int (*Name##_push_back_ref_t)(Name *self, Type const *data);\
/*typedef int (*Name##_push_front_ref_t)(Name *self, Type const *data);*/\
/*typedef void (*Name##_pop_front_t)(Name *self);*/\
typedef void (*Name##_pop_back_t)(Name *self);\
typedef int (*Name##_empty_t)(Name *self);\
typedef size_t (*Name##_size_t)(Name *self);\
typedef size_t (*Name##_capacity_t)(Name *self);\
typedef void (*Name##_clear_t)(Name *self);\
typedef int (*Name##_reserve_t)(Name *self, size_t n);\
typedef void (*Name##_shrink_t)(Name *self, size_t n);\
typedef Type *(*Name##_at_t)(Name *self, size_t idx);\
typedef Type *(*Name##_front_t)(Name *self);\
typedef Type *(*Name##_back_t)(Name *self);\
typedef Name##Iterator (*Name##_begin_t)(Name *self);\
typedef Name##Iterator (*Name##_end_t)(Name *self);\
typedef Name##Iterator (*Name##_rbegin_t)(Name *self);\
typedef Name##Iterator (*Name##_rend_t)(Name *self);\
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
struct Name##_Vtable {\
	Name##_delete_t         delete_;\
	Name##_push_back_t      push_back;\
/*	Name##_push_front_t     push_front;*/\
	Name##_push_back_ref_t  push_back_ref;\
/*	Name##_push_front_ref_t push_front_ref;*/\
/*	Name##_pop_front_t      pop_front;*/\
	Name##_pop_back_t       pop_back;\
	Name##_empty_t          empty;\
	Name##_size_t           size;\
	Name##_capacity_t       capacity;\
	Name##_clear_t          clear;\
	Name##_reserve_t        reserve;\
	Name##_shrink_t         shrink;\
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
 * \brief vector構造体\
 */\
struct Name {\
	union {\
		const struct Name##_Vtable *vptr;\
	} u;\
	size_t size;\
	size_t capacity;\
	Type *buf;\
	CSTL_MAGIC(Name *magic;)\
};\
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
Name##Iterator Name##_begin(Name *self);\
Name##Iterator Name##_end(Name *self);\
Name##Iterator Name##_rbegin(Name *self);\
Name##Iterator Name##_rend(Name *self);\
int Name##_insert(Name *self, CstlIterInternalData pos, Type data, Name##Iterator *iter);\
int Name##_insert_ref(Name *self, CstlIterInternalData pos, Type const *data, Name##Iterator *iter);\
int Name##_insert_n(Name *self, CstlIterInternalData pos, size_t n, Type data);\
int Name##_insert_n_ref(Name *self, CstlIterInternalData pos, size_t n, Type const *data);\
int Name##_insert_array(Name *self, CstlIterInternalData pos, Type const *data, size_t n);\
int Name##_insert_range(Name *self, CstlIterInternalData pos, CstlIterInternal first, CstlIterInternal last);\
Name##Iterator Name##_erase(Name *self, CstlIterInternalData pos);\
Name##Iterator Name##_erase_range(Name *self, CstlIterInternalData first, CstlIterInternalData last);\
void Name##_swap(Name *self, Name *x);\
Type *Name##Iterator_data(CstlIterInternalData pos);\
Name##Iterator Name##Iterator_next(CstlIterInternalData pos);\
Name##Iterator Name##Iterator_prev(CstlIterInternalData pos);\
void Name##Iterator_inc(CstlIterInternalData *pos);\
void Name##Iterator_dec(CstlIterInternalData *pos);\
int Name##Iterator_eq(CstlIterInternalData pos, CstlIterInternalData x);\
int Name##Iterator_ne(CstlIterInternalData pos, CstlIterInternalData x);\
Type *Name##Iterator_at(CstlIterInternalData pos, size_t n);\
Name##Iterator Name##Iterator_add(CstlIterInternalData pos, size_t n);\
Name##Iterator Name##Iterator_sub(CstlIterInternalData pos, size_t n);\
void Name##Iterator_inc_n(CstlIterInternalData *pos, size_t n);\
void Name##Iterator_dec_n(CstlIterInternalData *pos, size_t n);\
int Name##Iterator_diff(CstlIterInternalData pos, CstlIterInternalData x);\
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
Type *Name##ReverseIterator_at(CstlIterInternalData pos, size_t n);\
Name##Iterator Name##ReverseIterator_add(CstlIterInternalData pos, size_t n);\
Name##Iterator Name##ReverseIterator_sub(CstlIterInternalData pos, size_t n);\
void Name##ReverseIterator_inc_n(CstlIterInternalData *pos, size_t n);\
void Name##ReverseIterator_dec_n(CstlIterInternalData *pos, size_t n);\
int Name##ReverseIterator_diff(CstlIterInternalData pos, CstlIterInternalData x);\
int Name##ReverseIterator_lt(CstlIterInternalData pos, CstlIterInternalData x);\
int Name##ReverseIterator_le(CstlIterInternalData pos, CstlIterInternalData x);\
int Name##ReverseIterator_gt(CstlIterInternalData pos, CstlIterInternalData x);\
int Name##ReverseIterator_ge(CstlIterInternalData pos, CstlIterInternalData x);\
/*CSTL_ALGORITHM_INTERFACE(Name, Type)*/\
CSTL_EXTERN_C_END()\



#define CSTL_VECTOR_IMPLEMENT_BASE(Name, Type)	\
\
static void *Name##Iterator_key_dummy(CstlIterInternalData pos)\
{\
	CSTL_ASSERT(0 && "VectorIterator_key_dummy");\
	return 0;\
}\
\
static void *Name##Iterator_val_dummy(CstlIterInternalData pos)\
{\
	CSTL_ASSERT(0 && "VectorIterator_val_dummy");\
	return 0;\
}\
\
static const Name##Iterator_Vtable Name##Iterator_vtbl = {\
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
	CSTL_CONTAINER_VECTOR,\
	1, /* is_rand_iter */\
	0, /* is_reverse_iter */\
};\
\
static const Name##Iterator_Vtable Name##ReverseIterator_vtbl = {\
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
	CSTL_CONTAINER_VECTOR,\
	1, /* is_rand_iter */\
	1, /* is_reverse_iter */\
};\
\
static const struct Name##_Vtable Name##_vtbl = {\
	Name##_delete,\
	Name##_push_back,\
/*	Name##_push_front,*/\
	Name##_push_back_ref,\
/*	Name##_push_front_ref,*/\
/*	Name##_pop_front,*/\
	Name##_pop_back,\
	Name##_empty,\
	Name##_size,\
	Name##_capacity,\
	Name##_clear,\
	Name##_reserve,\
	Name##_shrink,\
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
	return CSTL_VECTOR_ELEM(Type, pos);\
}\
\
Name##Iterator Name##Iterator_next(CstlIterInternalData pos)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(Name##_size(CSTL_VECTOR_SELF(Name, pos)) > CSTL_VECTOR_IDX(pos));\
	iter.vptr = &Name##Iterator_vtbl;\
	CSTL_VECTOR_ELEM_ASSIGN(iter.internal.data) = CSTL_VECTOR_ELEM(Type, pos) + 1;\
	CSTL_VECTOR_SELF_ASSIGN(iter.internal.data) = CSTL_VECTOR_SELF(Name, pos);\
	CSTL_VECTOR_IDX(iter.internal.data) = CSTL_VECTOR_IDX(pos) + 1;\
	return iter;\
}\
\
Name##Iterator Name##Iterator_prev(CstlIterInternalData pos)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(CSTL_VECTOR_IDX(pos) > 0);\
	iter.vptr = &Name##Iterator_vtbl;\
	CSTL_VECTOR_ELEM_ASSIGN(iter.internal.data) = CSTL_VECTOR_ELEM(Type, pos) - 1;\
	CSTL_VECTOR_SELF_ASSIGN(iter.internal.data) = CSTL_VECTOR_SELF(Name, pos);\
	CSTL_VECTOR_IDX(iter.internal.data) = CSTL_VECTOR_IDX(pos) - 1;\
	return iter;\
}\
\
void Name##Iterator_inc(CstlIterInternalData *pos)\
{\
	CSTL_ASSERT(pos);\
	CSTL_ASSERT(Name##_size(CSTL_VECTOR_SELF(Name, *pos)) > CSTL_VECTOR_IDX(*pos));\
	CSTL_VECTOR_ELEM_ASSIGN(*pos) = CSTL_VECTOR_ELEM(Type, *pos) + 1;\
	CSTL_VECTOR_IDX(*pos)++;\
}\
\
void Name##Iterator_dec(CstlIterInternalData *pos)\
{\
	CSTL_ASSERT(pos);\
	CSTL_ASSERT(CSTL_VECTOR_IDX(*pos) > 0);\
	CSTL_VECTOR_ELEM_ASSIGN(*pos) = CSTL_VECTOR_ELEM(Type, *pos) - 1;\
	CSTL_VECTOR_IDX(*pos)--;\
}\
\
int Name##Iterator_eq(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(CSTL_VECTOR_SELF(Name, pos) == CSTL_VECTOR_SELF(Name, x));\
	return CSTL_VECTOR_IDX(pos) == CSTL_VECTOR_IDX(x);\
}\
\
int Name##Iterator_ne(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(CSTL_VECTOR_SELF(Name, pos) == CSTL_VECTOR_SELF(Name, x));\
	return CSTL_VECTOR_IDX(pos) != CSTL_VECTOR_IDX(x);\
}\
\
Type *Name##Iterator_at(CstlIterInternalData pos, size_t n)\
{\
	CSTL_ASSERT(Name##_size(CSTL_VECTOR_SELF(Name, pos)) > CSTL_VECTOR_IDX(pos) + n && "VectorIterator_at");\
	return &CSTL_VECTOR_AT(CSTL_VECTOR_SELF(Name, pos), CSTL_VECTOR_IDX(pos) + n);\
}\
\
Name##Iterator Name##Iterator_add(CstlIterInternalData pos, size_t n)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(1 && "VectorIterator_add");\
	iter.vptr = &Name##Iterator_vtbl;\
	CSTL_VECTOR_ELEM_ASSIGN(iter.internal.data) = CSTL_VECTOR_ELEM(Type, pos) + n;\
	CSTL_VECTOR_SELF_ASSIGN(iter.internal.data) = CSTL_VECTOR_SELF(Name, pos);\
	CSTL_VECTOR_IDX(iter.internal.data) = CSTL_VECTOR_IDX(pos) + n;\
	return iter;\
}\
\
Name##Iterator Name##Iterator_sub(CstlIterInternalData pos, size_t n)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(1 && "VectorIterator_sub");\
	CSTL_ASSERT(CSTL_VECTOR_IDX(pos) >= n && "VectorIterator_sub");\
	iter.vptr = &Name##Iterator_vtbl;\
	CSTL_VECTOR_ELEM_ASSIGN(iter.internal.data) = CSTL_VECTOR_ELEM(Type, pos) - n;\
	CSTL_VECTOR_SELF_ASSIGN(iter.internal.data) = CSTL_VECTOR_SELF(Name, pos);\
	CSTL_VECTOR_IDX(iter.internal.data) = CSTL_VECTOR_IDX(pos) - n;\
	return iter;\
}\
\
void Name##Iterator_inc_n(CstlIterInternalData *pos, size_t n)\
{\
	CSTL_ASSERT(1 && "VectorIterator_inc_n");\
	CSTL_VECTOR_ELEM_ASSIGN(*pos) = CSTL_VECTOR_ELEM(Type, *pos) + n;\
	CSTL_VECTOR_IDX(*pos) += n;\
}\
\
void Name##Iterator_dec_n(CstlIterInternalData *pos, size_t n)\
{\
	CSTL_ASSERT(1 && "VectorIterator_dec_n");\
	CSTL_ASSERT(CSTL_VECTOR_IDX(*pos) >= n && "VectorIterator_dec_n");\
	CSTL_VECTOR_ELEM_ASSIGN(*pos) = CSTL_VECTOR_ELEM(Type, *pos) - n;\
	CSTL_VECTOR_IDX(*pos) -= n;\
}\
\
int Name##Iterator_diff(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(1 && "VectorIterator_diff");\
	CSTL_ASSERT(CSTL_VECTOR_SELF(Name, pos) == CSTL_VECTOR_SELF(Name, x));\
	return (int) CSTL_VECTOR_IDX(pos) - (int) CSTL_VECTOR_IDX(x);\
}\
\
int Name##Iterator_lt(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(1 && "VectorIterator_lt");\
	CSTL_ASSERT(CSTL_VECTOR_SELF(Name, pos) == CSTL_VECTOR_SELF(Name, x));\
	return CSTL_VECTOR_IDX(pos) < CSTL_VECTOR_IDX(x);\
}\
\
int Name##Iterator_le(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(1 && "VectorIterator_le");\
	CSTL_ASSERT(CSTL_VECTOR_SELF(Name, pos) == CSTL_VECTOR_SELF(Name, x));\
	return CSTL_VECTOR_IDX(pos) <= CSTL_VECTOR_IDX(x);\
}\
\
int Name##Iterator_gt(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(1 && "VectorIterator_gt");\
	CSTL_ASSERT(CSTL_VECTOR_SELF(Name, pos) == CSTL_VECTOR_SELF(Name, x));\
	return CSTL_VECTOR_IDX(pos) > CSTL_VECTOR_IDX(x);\
}\
\
int Name##Iterator_ge(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(1 && "VectorIterator_ge");\
	CSTL_ASSERT(CSTL_VECTOR_SELF(Name, pos) == CSTL_VECTOR_SELF(Name, x));\
	return CSTL_VECTOR_IDX(pos) >= CSTL_VECTOR_IDX(x);\
}\
\
Name##ReverseIterator Name##Iterator_reverse_iterator(CstlIterInternalData pos)\
{\
	Name##ReverseIterator iter;\
	CSTL_ASSERT(1 && "VectorIterator_reverse_iterator");\
	iter.vptr = &Name##ReverseIterator_vtbl;\
	iter.internal.data = pos;\
	return iter;\
}\
\
Name##Iterator Name##ReverseIterator_base(CstlIterInternalData pos)\
{\
	Name##Iterator iter;\
	iter.vptr = &Name##Iterator_vtbl;\
	iter.internal.data = pos;\
	return iter;\
}\
\
Type *Name##ReverseIterator_data(CstlIterInternalData pos)\
{\
	return CSTL_VECTOR_ELEM(Type, pos) - 1;\
}\
\
Name##Iterator Name##ReverseIterator_next(CstlIterInternalData pos)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(CSTL_VECTOR_IDX(pos) > 0);\
	iter.vptr = &Name##ReverseIterator_vtbl;\
	CSTL_VECTOR_ELEM_ASSIGN(iter.internal.data) = CSTL_VECTOR_ELEM(Type, pos) - 1;\
	CSTL_VECTOR_SELF_ASSIGN(iter.internal.data) = CSTL_VECTOR_SELF(Name, pos);\
	CSTL_VECTOR_IDX(iter.internal.data) = CSTL_VECTOR_IDX(pos) - 1;\
	return iter;\
}\
\
Name##Iterator Name##ReverseIterator_prev(CstlIterInternalData pos)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(Name##_size(CSTL_VECTOR_SELF(Name, pos)) > CSTL_VECTOR_IDX(pos));\
	iter.vptr = &Name##ReverseIterator_vtbl;\
	CSTL_VECTOR_ELEM_ASSIGN(iter.internal.data) = CSTL_VECTOR_ELEM(Type, pos) + 1;\
	CSTL_VECTOR_SELF_ASSIGN(iter.internal.data) = CSTL_VECTOR_SELF(Name, pos);\
	CSTL_VECTOR_IDX(iter.internal.data) = CSTL_VECTOR_IDX(pos) + 1;\
	return iter;\
}\
\
void Name##ReverseIterator_inc(CstlIterInternalData *pos)\
{\
	CSTL_ASSERT(pos);\
	CSTL_ASSERT(CSTL_VECTOR_IDX(*pos) > 0);\
	CSTL_VECTOR_ELEM_ASSIGN(*pos) = CSTL_VECTOR_ELEM(Type, *pos) - 1;\
	CSTL_VECTOR_IDX(*pos)--;\
}\
\
void Name##ReverseIterator_dec(CstlIterInternalData *pos)\
{\
	CSTL_ASSERT(pos);\
	CSTL_ASSERT(Name##_size(CSTL_VECTOR_SELF(Name, *pos)) > CSTL_VECTOR_IDX(*pos));\
	CSTL_VECTOR_ELEM_ASSIGN(*pos) = CSTL_VECTOR_ELEM(Type, *pos) + 1;\
	CSTL_VECTOR_IDX(*pos)++;\
}\
\
int Name##ReverseIterator_eq(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(CSTL_VECTOR_SELF(Name, pos) == CSTL_VECTOR_SELF(Name, x));\
	return CSTL_VECTOR_IDX(pos) == CSTL_VECTOR_IDX(x);\
}\
\
int Name##ReverseIterator_ne(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(CSTL_VECTOR_SELF(Name, pos) == CSTL_VECTOR_SELF(Name, x));\
	return CSTL_VECTOR_IDX(pos) != CSTL_VECTOR_IDX(x);\
}\
\
Type *Name##ReverseIterator_at(CstlIterInternalData pos, size_t n)\
{\
	CSTL_ASSERT(Name##_size(CSTL_VECTOR_SELF(Name, pos)) > CSTL_VECTOR_IDX(pos) + n && "VectorIterator_at");\
	return &CSTL_VECTOR_AT(CSTL_VECTOR_SELF(Name, pos), CSTL_VECTOR_IDX(pos) + n);\
}\
\
Name##Iterator Name##ReverseIterator_add(CstlIterInternalData pos, size_t n)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(1 && "VectorIterator_add");\
	CSTL_ASSERT(CSTL_VECTOR_IDX(pos) >= n && "VectorIterator_add");\
	iter.vptr = &Name##ReverseIterator_vtbl;\
	CSTL_VECTOR_ELEM_ASSIGN(iter.internal.data) = CSTL_VECTOR_ELEM(Type, pos) - n;\
	CSTL_VECTOR_SELF_ASSIGN(iter.internal.data) = CSTL_VECTOR_SELF(Name, pos);\
	CSTL_VECTOR_IDX(iter.internal.data) = CSTL_VECTOR_IDX(pos) - n;\
	return iter;\
}\
\
Name##Iterator Name##ReverseIterator_sub(CstlIterInternalData pos, size_t n)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(1 && "VectorIterator_sub");\
	iter.vptr = &Name##ReverseIterator_vtbl;\
	CSTL_VECTOR_ELEM_ASSIGN(iter.internal.data) = CSTL_VECTOR_ELEM(Type, pos) + n;\
	CSTL_VECTOR_SELF_ASSIGN(iter.internal.data) = CSTL_VECTOR_SELF(Name, pos);\
	CSTL_VECTOR_IDX(iter.internal.data) = CSTL_VECTOR_IDX(pos) + n;\
	return iter;\
}\
\
void Name##ReverseIterator_inc_n(CstlIterInternalData *pos, size_t n)\
{\
	CSTL_ASSERT(1 && "VectorIterator_inc_n");\
	CSTL_ASSERT(CSTL_VECTOR_IDX(*pos) >= n && "VectorIterator_inc_n");\
	CSTL_VECTOR_ELEM_ASSIGN(*pos) = CSTL_VECTOR_ELEM(Type, *pos) - n;\
	CSTL_VECTOR_IDX(*pos) -= n;\
}\
\
void Name##ReverseIterator_dec_n(CstlIterInternalData *pos, size_t n)\
{\
	CSTL_ASSERT(1 && "VectorIterator_dec_n");\
	CSTL_VECTOR_ELEM_ASSIGN(*pos) = CSTL_VECTOR_ELEM(Type, *pos) + n;\
	CSTL_VECTOR_IDX(*pos) += n;\
}\
\
int Name##ReverseIterator_diff(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(1 && "VectorIterator_diff");\
	CSTL_ASSERT(CSTL_VECTOR_SELF(Name, pos) == CSTL_VECTOR_SELF(Name, x));\
	return (int) CSTL_VECTOR_IDX(x) - (int) CSTL_VECTOR_IDX(pos);\
}\
\
int Name##ReverseIterator_lt(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(1 && "VectorIterator_lt");\
	CSTL_ASSERT(CSTL_VECTOR_SELF(Name, pos) == CSTL_VECTOR_SELF(Name, x));\
	return CSTL_VECTOR_IDX(pos) > CSTL_VECTOR_IDX(x);\
}\
\
int Name##ReverseIterator_le(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(1 && "VectorIterator_le");\
	CSTL_ASSERT(CSTL_VECTOR_SELF(Name, pos) == CSTL_VECTOR_SELF(Name, x));\
	return CSTL_VECTOR_IDX(pos) >= CSTL_VECTOR_IDX(x);\
}\
\
int Name##ReverseIterator_gt(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(1 && "VectorIterator_gt");\
	CSTL_ASSERT(CSTL_VECTOR_SELF(Name, pos) == CSTL_VECTOR_SELF(Name, x));\
	return CSTL_VECTOR_IDX(pos) < CSTL_VECTOR_IDX(x);\
}\
\
int Name##ReverseIterator_ge(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(1 && "VectorIterator_ge");\
	CSTL_ASSERT(CSTL_VECTOR_SELF(Name, pos) == CSTL_VECTOR_SELF(Name, x));\
	return CSTL_VECTOR_IDX(pos) <= CSTL_VECTOR_IDX(x);\
}\
\
Name *Name##_new(void)\
{\
	Name *self;\
	self = (Name *) malloc(sizeof(Name));\
	if (!self) return 0;\
	self->u.vptr = &Name##_vtbl;\
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
Name##Iterator Name##_begin(Name *self)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(self && "Vector_begin");\
	CSTL_ASSERT(self->magic == self && "Vector_begin");\
	iter.vptr = &Name##Iterator_vtbl;\
	CSTL_VECTOR_ELEM_ASSIGN(iter.internal.data) = self->buf ? &CSTL_VECTOR_AT(self, 0) : 0;\
	CSTL_VECTOR_SELF_ASSIGN(iter.internal.data) = self;\
	CSTL_VECTOR_IDX(iter.internal.data) = 0;\
	return iter;\
}\
\
Name##Iterator Name##_end(Name *self)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(self && "Vector_end");\
	CSTL_ASSERT(self->magic == self && "Vector_end");\
	iter.vptr = &Name##Iterator_vtbl;\
	CSTL_VECTOR_ELEM_ASSIGN(iter.internal.data) = self->buf ? &CSTL_VECTOR_AT(self, self->size) : 0;\
	CSTL_VECTOR_SELF_ASSIGN(iter.internal.data) = self;\
	CSTL_VECTOR_IDX(iter.internal.data) = self->size;\
	return iter;\
}\
\
Name##Iterator Name##_rbegin(Name *self)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(self && "Vector_rbegin");\
	CSTL_ASSERT(self->magic == self && "Vector_rbegin");\
	iter.vptr = &Name##ReverseIterator_vtbl;\
	CSTL_VECTOR_ELEM_ASSIGN(iter.internal.data) = self->buf ? &CSTL_VECTOR_AT(self, self->size) : 0;\
	CSTL_VECTOR_SELF_ASSIGN(iter.internal.data) = self;\
	CSTL_VECTOR_IDX(iter.internal.data) = self->size;\
	return iter;\
}\
\
Name##Iterator Name##_rend(Name *self)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(self && "Vector_rend");\
	CSTL_ASSERT(self->magic == self && "Vector_rend");\
	iter.vptr = &Name##ReverseIterator_vtbl;\
	CSTL_VECTOR_ELEM_ASSIGN(iter.internal.data) = self->buf ? &CSTL_VECTOR_AT(self, 0) : 0;\
	CSTL_VECTOR_SELF_ASSIGN(iter.internal.data) = self;\
	CSTL_VECTOR_IDX(iter.internal.data) = 0;\
	return iter;\
}\
\
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
int Name##_insert(Name *self, CstlIterInternalData pos, Type data, Name##Iterator *iter)\
{\
	int ret;\
	CSTL_ASSERT(self && "Vector_insert");\
	CSTL_ASSERT(self->magic == self && "Vector_insert");\
/*	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) >= idx && "Vector_insert");*/\
	ret = Name##_insert_n_ref(self, pos, 1, &data);\
	if (ret && iter) {\
		iter->vptr = &Name##Iterator_vtbl;\
		CSTL_VECTOR_ELEM_ASSIGN(iter->internal.data) = &CSTL_VECTOR_AT(self, CSTL_VECTOR_IDX(pos));\
		CSTL_VECTOR_SELF_ASSIGN(iter->internal.data) = self;\
		CSTL_VECTOR_IDX(iter->internal.data) = CSTL_VECTOR_IDX(pos);\
	}\
	return ret;\
}\
\
int Name##_insert_ref(Name *self, CstlIterInternalData pos, Type const *data, Name##Iterator *iter)\
{\
	int ret;\
	CSTL_ASSERT(self && "Vector_insert_ref");\
	CSTL_ASSERT(self->magic == self && "Vector_insert_ref");\
/*	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) >= idx && "Vector_insert_ref");*/\
	CSTL_ASSERT(data && "Vector_insert_ref");\
	ret = Name##_insert_n_ref(self, pos, 1, data);\
	if (ret && iter) {\
		iter->vptr = &Name##Iterator_vtbl;\
		CSTL_VECTOR_ELEM_ASSIGN(iter->internal.data) = &CSTL_VECTOR_AT(self, CSTL_VECTOR_IDX(pos));\
		CSTL_VECTOR_SELF_ASSIGN(iter->internal.data) = self;\
		CSTL_VECTOR_IDX(iter->internal.data) = CSTL_VECTOR_IDX(pos);\
	}\
	return ret;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_INSERT_N(Name, Type)	\
int Name##_insert_n(Name *self, CstlIterInternalData pos, size_t n, Type data)\
{\
	CSTL_ASSERT(self && "Vector_insert_n");\
	CSTL_ASSERT(self->magic == self && "Vector_insert_n");\
/*	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) >= idx && "Vector_insert_n");*/\
	return Name##_insert_n_ref(self, pos, n, &data);\
}\
\
int Name##_insert_n_ref(Name *self, CstlIterInternalData pos, size_t n, Type const *data)\
{\
	register size_t i;\
	size_t idx;\
	CSTL_ASSERT(self && "Vector_insert_n_ref");\
	CSTL_ASSERT(self->magic == self && "Vector_insert_n_ref");\
	idx = CSTL_VECTOR_IDX(pos);\
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
int Name##_insert_array(Name *self, CstlIterInternalData pos, Type const *data, size_t n)\
{\
	size_t idx;\
	CSTL_ASSERT(self && "Vector_insert_array");\
	CSTL_ASSERT(self->magic == self && "Vector_insert_array");\
	idx = CSTL_VECTOR_IDX(pos);\
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
/*int Name##_insert_range(Name *self, size_t idx, Name *x, size_t xidx, size_t n)*/\
int Name##_insert_range(Name *self, CstlIterInternalData pos, CstlIterInternal first, CstlIterInternal last)\
{\
	size_t idx;\
	Name *x;\
	size_t n;\
	CSTL_ASSERT(self && "Vector_insert_range");\
	CSTL_ASSERT(self->magic == self && "Vector_insert_range");\
	CSTL_ASSERT(first.in_vptr == last.in_vptr && "Vector_insert_range");\
	CSTL_ASSERT(CSTL_CAST_VPTR(Name, first.in_vptr)->is_rand_iter == \
				CSTL_CAST_VPTR(Name, last.in_vptr)->is_rand_iter && "Vector_insert_range");\
	idx = CSTL_VECTOR_IDX(pos);\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) >= idx && "Vector_insert_range");\
/*	CSTL_ASSERT(CSTL_VECTOR_SELF(Name, first.data) == CSTL_VECTOR_SELF(Name, last.data) && "Vector_insert_range");\
	CSTL_ASSERT(CSTL_VECTOR_IDX(first.data) <= CSTL_VECTOR_IDX(last.data) && "Vector_insert_range");*/\
	x = CSTL_VECTOR_SELF(Name, first.data);\
	if (self == x) {\
		size_t xidx;\
		xidx = CSTL_VECTOR_IDX(first.data);\
		n = CSTL_VECTOR_IDX(last.data) - CSTL_VECTOR_IDX(first.data);\
		CSTL_ASSERT(x && "Vector_insert_range");\
		CSTL_ASSERT(x->magic == x && "Vector_insert_range");\
		CSTL_ASSERT(CSTL_VECTOR_SIZE(x) >= xidx + n && "Vector_insert_range");\
		CSTL_ASSERT(CSTL_VECTOR_SIZE(x) >= n && "Vector_insert_range");\
		CSTL_ASSERT(CSTL_VECTOR_SIZE(x) > xidx && "Vector_insert_range");\
		if (!Name##_insert_n_no_data(self, idx, n)) {\
			return 0;\
		}\
		/* この場合は[first, last)がvectorとして処理できる。イテレータの仮想関数を使わない */\
		if (idx <= xidx) {\
			memcpy(&CSTL_VECTOR_AT(self, idx), &CSTL_VECTOR_AT(self, xidx + n), sizeof(Type) * n);\
		} else if (xidx < idx && idx < xidx + n) {\
			memcpy(&CSTL_VECTOR_AT(self, idx), &CSTL_VECTOR_AT(self, xidx), sizeof(Type) * (idx - xidx));\
			memcpy(&CSTL_VECTOR_AT(self, idx + (idx - xidx)), &CSTL_VECTOR_AT(self, idx + n), sizeof(Type) * (n - (idx - xidx)));\
		} else {\
			memcpy(&CSTL_VECTOR_AT(self, idx), &CSTL_VECTOR_AT(self, xidx), sizeof(Type) * n);\
		}\
	} else {\
		CstlIterInternal i;\
		size_t j;\
		if (CSTL_CAST_VPTR(Name, first.in_vptr)->is_rand_iter) {\
			CSTL_ASSERT(CSTL_CAST_VPTR(Name, first.in_vptr)->diff(last.data, first.data) >= 0 && "Vector_insert_range");\
			n = (size_t) CSTL_CAST_VPTR(Name, first.in_vptr)->diff(last.data, first.data);\
		} else {\
			for (i = first, n = 0; CSTL_CAST_VPTR(Name, i.in_vptr)->ne(i.data, last.data); \
					CSTL_CAST_VPTR(Name, i.in_vptr)->inc(&i.data)) {\
				n++;\
			}\
		}\
		if (!Name##_insert_n_no_data(self, idx, n)) {\
			return 0;\
		}\
		if (CSTL_CAST_VPTR(Name, first.in_vptr)->container == CSTL_CONTAINER_VECTOR) {\
			size_t xidx;\
			xidx = CSTL_VECTOR_IDX(first.data);\
			CSTL_ASSERT(x && "Vector_insert_range");\
			CSTL_ASSERT(x->magic == x && "Vector_insert_range");\
			CSTL_ASSERT(CSTL_VECTOR_SIZE(x) >= xidx + n && "Vector_insert_range");\
			CSTL_ASSERT(CSTL_VECTOR_SIZE(x) >= n && "Vector_insert_range");\
			CSTL_ASSERT(CSTL_VECTOR_SIZE(x) > xidx && "Vector_insert_range");\
			memcpy(&CSTL_VECTOR_AT(self, idx), &CSTL_VECTOR_AT(x, xidx), sizeof(Type) * n);\
		} else {\
			for (i = first, j = 0; CSTL_CAST_VPTR(Name, i.in_vptr)->ne(i.data, last.data); \
					CSTL_CAST_VPTR(Name, i.in_vptr)->inc(&i.data), j++) {\
				CSTL_VECTOR_AT(self, idx + j) = *CSTL_CAST_VPTR(Name, i.in_vptr)->data(i.data);\
			}\
		}\
	}\
	return 1;\
}\
\

#define CSTL_VECTOR_IMPLEMENT_ERASE(Name, Type)	\
/*void Name##_erase(Name *self, size_t idx, size_t n)*/\
Name##Iterator Name##_erase(Name *self, CstlIterInternalData pos)\
{\
	Name##Iterator iter;\
	size_t idx;\
	size_t n;\
	CSTL_ASSERT(self && "Vector_erase");\
	CSTL_ASSERT(self->magic == self && "Vector_erase");\
	idx = CSTL_VECTOR_IDX(pos);\
	n = 1;\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) >= idx + n && "Vector_erase");\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) >= n && "Vector_erase");\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) > idx && "Vector_erase");\
	Name##_move_forward(self, idx + n, self->size, n);\
	self->size -= n;\
	iter.vptr = &Name##Iterator_vtbl;\
	CSTL_VECTOR_ELEM_ASSIGN(iter.internal.data) = &CSTL_VECTOR_AT(self, idx);\
	CSTL_VECTOR_SELF_ASSIGN(iter.internal.data) = self;\
	CSTL_VECTOR_IDX(iter.internal.data) = idx;\
	return iter;\
}\
\
Name##Iterator Name##_erase_range(Name *self, CstlIterInternalData first, CstlIterInternalData last)\
{\
	Name##Iterator iter;\
	size_t idx;\
	size_t n;\
	CSTL_ASSERT(self && "Vector_erase_range");\
	CSTL_ASSERT(self->magic == self && "Vector_erase_range");\
	CSTL_ASSERT(CSTL_VECTOR_SELF(Name, first) == CSTL_VECTOR_SELF(Name, last));\
	CSTL_ASSERT(CSTL_VECTOR_SELF(Name, first) == self);\
	CSTL_ASSERT(CSTL_VECTOR_IDX(first) <= CSTL_VECTOR_IDX(last) && "VectorIterator_sub");\
	idx = CSTL_VECTOR_IDX(first);\
	n = CSTL_VECTOR_IDX(last) - CSTL_VECTOR_IDX(first);\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) >= idx + n && "Vector_erase_range");\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) >= n && "Vector_erase_range");\
	CSTL_ASSERT(CSTL_VECTOR_SIZE(self) > idx && "Vector_erase_range");\
	Name##_move_forward(self, idx + n, self->size, n);\
	self->size -= n;\
	iter.vptr = &Name##Iterator_vtbl;\
	CSTL_VECTOR_ELEM_ASSIGN(iter.internal.data) = &CSTL_VECTOR_AT(self, idx);\
	CSTL_VECTOR_SELF_ASSIGN(iter.internal.data) = self;\
	CSTL_VECTOR_IDX(iter.internal.data) = idx;\
	return iter;\
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
