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
 * \file list.h
 * \brief listコンテナ
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2006-02-26
 * $URL$
 * $Id$
 */
#ifndef CSTL_LIST_H_INCLUDED
#define CSTL_LIST_H_INCLUDED

#include <stdlib.h>
#include "common.h"


#if !defined(NDEBUG) && defined(CSTL_DEBUG)
#define CSTL_MAGIC_LIST(Name)	((Name *) -1)
#endif


#define CSTL_LIST_BEGIN_NODE(self)	(self)->next
#define CSTL_LIST_END_NODE(self)	(self)
#define CSTL_LIST_RBEGIN_NODE(self)	(self)->prev
#define CSTL_LIST_REND_NODE(self)	(self)

#define CSTL_LIST_NODE(Name, internaldata)		((Name *) ((internaldata).data1))
#define CSTL_LIST_NODE_ASSIGN(internaldata)		((internaldata).data1)

/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_LIST_INTERFACE(Name, Type)	\
typedef struct Name Name;\
/*! \
 * \brief イテレータ\
 */\
typedef struct Name##Iterator {\
	const struct Name##Iterator_Vtable *vptr;\
	CstlIterInternal internal;\
} Name##Iterator;\
\
typedef Type *(*Name##Iterator_data_t)(CstlIterInternalData pos);\
typedef void *(*Name##Iterator_key_t)(CstlIterInternalData pos);\
typedef void *(*Name##Iterator_val_t)(CstlIterInternalData pos);\
typedef Name##Iterator (*Name##Iterator_next_t)(CstlIterInternalData pos);\
typedef Name##Iterator (*Name##Iterator_prev_t)(CstlIterInternalData pos);\
typedef void (*Name##Iterator_incr_t)(CstlIterInternalData *pos);\
typedef void (*Name##Iterator_decr_t)(CstlIterInternalData *pos);\
typedef int (*Name##Iterator_eq_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef int (*Name##Iterator_ne_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef Type *(*Name##Iterator_at_t)(CstlIterInternalData pos, size_t n);\
typedef Name##Iterator (*Name##Iterator_add_t)(CstlIterInternalData pos, size_t n);\
typedef Name##Iterator (*Name##Iterator_sub_t)(CstlIterInternalData pos, size_t n);\
typedef void (*Name##Iterator_incr_n_t)(CstlIterInternalData *pos, size_t n);\
typedef void (*Name##Iterator_decr_n_t)(CstlIterInternalData *pos, size_t n);\
typedef int (*Name##Iterator_diff_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef int (*Name##Iterator_lt_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef int (*Name##Iterator_le_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef int (*Name##Iterator_gt_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef int (*Name##Iterator_ge_t)(CstlIterInternalData pos, CstlIterInternalData x);\
\
struct Name##Iterator_Vtable {\
	Name##Iterator_data_t   data;\
	Name##Iterator_key_t    key;\
	Name##Iterator_val_t    val;\
	Name##Iterator_next_t   next;\
	Name##Iterator_prev_t   prev;\
	Name##Iterator_incr_t   incr;\
	Name##Iterator_decr_t   decr;\
	Name##Iterator_eq_t     eq;\
	Name##Iterator_ne_t     ne;\
	Name##Iterator_at_t     at;\
	Name##Iterator_add_t    add;\
	Name##Iterator_sub_t    sub;\
	Name##Iterator_incr_n_t incr_n;\
	Name##Iterator_decr_n_t decr_n;\
	Name##Iterator_diff_t   diff;\
	Name##Iterator_lt_t     lt;\
	Name##Iterator_le_t     le;\
	Name##Iterator_gt_t     gt;\
	Name##Iterator_ge_t     ge;\
};\
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
typedef Type *(*Name##_front_t)(Name *self);\
typedef Type *(*Name##_back_t)(Name *self);\
typedef Name##Iterator (*Name##_begin_t)(Name *self);\
typedef Name##Iterator (*Name##_end_t)(Name *self);\
typedef Name##Iterator (*Name##_rbegin_t)(Name *self);\
typedef Name##Iterator (*Name##_rend_t)(Name *self);\
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
typedef void (*Name##_splice_t)(Name *self, CstlIterInternalData pos, Name *x, CstlIterInternalData first, CstlIterInternalData last);\
typedef void (*Name##_sort_t)(Name *self, int (*comp)(const void *, const void *));\
typedef void (*Name##_merge_t)(Name *self, Name *x, int (*comp)(const void *, const void *));\
typedef void (*Name##_reverse_t)(Name *self);\
\
struct Name##_Vtable {\
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
	Name##_splice_t         splice;\
	Name##_sort_t           sort;\
	Name##_merge_t          merge;\
	Name##_reverse_t        reverse;\
};\
\
/*! \
 * \brief list構造体\
 */\
struct Name {\
	const struct Name##_Vtable *vptr;\
	Name *prev;\
	Name *next;\
	Type data;\
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
int Name##_empty(Name *self);\
size_t Name##_size(Name *self);\
void Name##_clear(Name *self);\
Type *Name##Iterator_data(CstlIterInternalData pos);\
Type *Name##_front(Name *self);\
Type *Name##_back(Name *self);\
Name##Iterator Name##_begin(Name *self);\
Name##Iterator Name##_end(Name *self);\
Name##Iterator Name##_rbegin(Name *self);\
Name##Iterator Name##_rend(Name *self);\
Name##Iterator Name##Iterator_next(CstlIterInternalData pos);\
Name##Iterator Name##Iterator_prev(CstlIterInternalData pos);\
void Name##Iterator_incr(CstlIterInternalData *pos);\
void Name##Iterator_decr(CstlIterInternalData *pos);\
int Name##Iterator_eq(CstlIterInternalData pos, CstlIterInternalData x);\
int Name##Iterator_ne(CstlIterInternalData pos, CstlIterInternalData x);\
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
void Name##_splice(Name *self, CstlIterInternalData pos, Name *x, CstlIterInternalData first, CstlIterInternalData last);\
void Name##_sort(Name *self, int (*comp)(const void *, const void *));\
void Name##_merge(Name *self, Name *x, int (*comp)(const void *, const void *));\
void Name##_reverse(Name *self);\
CSTL_EXTERN_C_END()\


/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_LIST_IMPLEMENT(Name, Type)	\
\
static Name *Name##_erase_aux(Name *pos);\
\
static void *Name##Iterator_key_dummy(CstlIterInternalData pos)\
{\
	CSTL_ASSERT(0 && "ListIterator_key_dummy");\
	return 0;\
}\
\
static void *Name##Iterator_val_dummy(CstlIterInternalData pos)\
{\
	CSTL_ASSERT(0 && "ListIterator_val_dummy");\
	return 0;\
}\
\
static Type *Name##Iterator_at_dummy(CstlIterInternalData pos, size_t n)\
{\
	CSTL_ASSERT(0 && "ListIterator_at_dummy");\
	return 0;\
}\
\
static Name##Iterator Name##Iterator_add_dummy(CstlIterInternalData pos, size_t n)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(0 && "ListIterator_add_dummy");\
	return iter;\
}\
\
static Name##Iterator Name##Iterator_sub_dummy(CstlIterInternalData pos, size_t n)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(0 && "ListIterator_sub_dummy");\
	return iter;\
}\
\
static void Name##Iterator_incr_n_dummy(CstlIterInternalData *pos, size_t n)\
{\
	CSTL_ASSERT(0 && "ListIterator_incr_n_dummy");\
}\
\
static void Name##Iterator_decr_n_dummy(CstlIterInternalData *pos, size_t n)\
{\
	CSTL_ASSERT(0 && "ListIterator_decr_n_dummy");\
}\
\
static int Name##Iterator_diff_dummy(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(0 && "ListIterator_diff_dummy");\
	return 0;\
}\
\
static int Name##Iterator_lt_dummy(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(0 && "ListIterator_lt_dummy");\
	return 0;\
}\
\
static int Name##Iterator_le_dummy(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(0 && "ListIterator_le_dummy");\
	return 0;\
}\
\
static int Name##Iterator_gt_dummy(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(0 && "ListIterator_gt_dummy");\
	return 0;\
}\
\
static int Name##Iterator_ge_dummy(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	CSTL_ASSERT(0 && "ListIterator_ge_dummy");\
	return 0;\
}\
\
static const struct Name##Iterator_Vtable Name##Iterator_vtbl = {\
	Name##Iterator_data,\
	Name##Iterator_key_dummy,\
	Name##Iterator_val_dummy,\
	Name##Iterator_next,\
	Name##Iterator_prev,\
	Name##Iterator_incr,\
	Name##Iterator_decr,\
	Name##Iterator_eq,\
	Name##Iterator_ne,\
	Name##Iterator_at_dummy,\
	Name##Iterator_add_dummy,\
	Name##Iterator_sub_dummy,\
	Name##Iterator_incr_n_dummy,\
	Name##Iterator_decr_n_dummy,\
	Name##Iterator_diff_dummy,\
	Name##Iterator_lt_dummy,\
	Name##Iterator_le_dummy,\
	Name##Iterator_gt_dummy,\
	Name##Iterator_ge_dummy,\
};\
\
static const struct CstlIteratorVtable Name##Iterator_in_vtbl = {\
	(CstlIterator_data_t)   Name##Iterator_data,\
	(CstlIterator_key_t)    Name##Iterator_key_dummy,\
	(CstlIterator_val_t)    Name##Iterator_val_dummy,\
	(CstlIterator_next_t)   Name##Iterator_next,\
	(CstlIterator_prev_t)   Name##Iterator_prev,\
	(CstlIterator_incr_t)   Name##Iterator_incr,\
	(CstlIterator_decr_t)   Name##Iterator_decr,\
	(CstlIterator_eq_t)     Name##Iterator_eq,\
	(CstlIterator_ne_t)     Name##Iterator_ne,\
	(CstlIterator_at_t)     Name##Iterator_at_dummy,\
	(CstlIterator_add_t)    Name##Iterator_add_dummy,\
	(CstlIterator_sub_t)    Name##Iterator_sub_dummy,\
	(CstlIterator_incr_n_t) Name##Iterator_incr_n_dummy,\
	(CstlIterator_decr_n_t) Name##Iterator_decr_n_dummy,\
	(CstlIterator_diff_t)   Name##Iterator_diff_dummy,\
	(CstlIterator_lt_t)     Name##Iterator_lt_dummy,\
	(CstlIterator_le_t)     Name##Iterator_le_dummy,\
	(CstlIterator_gt_t)     Name##Iterator_gt_dummy,\
	(CstlIterator_ge_t)     Name##Iterator_ge_dummy,\
};\
\
static struct Name##_Vtable Name##_vtbl = {\
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
	Name##_splice,\
	Name##_sort,\
	Name##_merge,\
	Name##_reverse,\
};\
\
Name *Name##_new(void)\
{\
	Name *self;\
	self = (Name *) malloc(sizeof(Name));\
	if (!self) return 0;\
	self->vptr = &Name##_vtbl;\
	self->next = self;\
	self->prev = self;\
	CSTL_MAGIC(self->magic = self);\
	return self;\
}\
\
void Name##_delete(Name *self)\
{\
	if (!self) return;\
	CSTL_ASSERT(self->magic == self && "List_delete");\
	Name##_clear(self);\
	CSTL_MAGIC(self->magic = 0);\
	free(self);\
}\
\
int Name##_push_back(Name *self, Type data)\
{\
	CSTL_ASSERT(self && "List_push_back");\
	CSTL_ASSERT(self->magic == self && "List_push_back");\
	return Name##_insert_ref(self, Name##_end(self).internal.data, &data, 0);\
}\
\
int Name##_push_front(Name *self, Type data)\
{\
	CSTL_ASSERT(self && "List_push_front");\
	CSTL_ASSERT(self->magic == self && "List_push_front");\
	return Name##_insert_ref(self, Name##_begin(self).internal.data, &data, 0);\
}\
\
int Name##_push_back_ref(Name *self, Type const *data)\
{\
	CSTL_ASSERT(self && "List_push_back_ref");\
	CSTL_ASSERT(self->magic == self && "List_push_back_ref");\
	CSTL_ASSERT(data && "List_push_back_ref");\
	return Name##_insert_ref(self, Name##_end(self).internal.data, data, 0);\
}\
\
int Name##_push_front_ref(Name *self, Type const *data)\
{\
	CSTL_ASSERT(self && "List_push_front_ref");\
	CSTL_ASSERT(self->magic == self && "List_push_front_ref");\
	CSTL_ASSERT(data && "List_push_front_ref");\
	return Name##_insert_ref(self, Name##_begin(self).internal.data, data, 0);\
}\
\
void Name##_pop_front(Name *self)\
{\
	CSTL_ASSERT(self && "List_pop_front");\
	CSTL_ASSERT(self->magic == self && "List_pop_front");\
	CSTL_ASSERT(!Name##_empty(self) && "List_pop_front");\
	Name##_erase_aux(CSTL_LIST_BEGIN_NODE(self));\
}\
\
void Name##_pop_back(Name *self)\
{\
	CSTL_ASSERT(self && "List_pop_back");\
	CSTL_ASSERT(self->magic == self && "List_pop_back");\
	CSTL_ASSERT(!Name##_empty(self) && "List_pop_back");\
	Name##_erase_aux(CSTL_LIST_RBEGIN_NODE(self));\
}\
\
int Name##_empty(Name *self)\
{\
	CSTL_ASSERT(self && "List_empty");\
	CSTL_ASSERT(self->magic == self && "List_empty");\
	return self->next == self;\
}\
\
size_t Name##_size(Name *self)\
{\
	register Name *pos;\
	register size_t count = 0;\
	CSTL_ASSERT(self && "List_size");\
	CSTL_ASSERT(self->magic == self && "List_size");\
	for (pos = CSTL_LIST_BEGIN_NODE(self); pos != CSTL_LIST_END_NODE(self); pos = pos->next) {\
		count++;\
	}\
	return count;\
}\
\
void Name##_clear(Name *self)\
{\
	CSTL_ASSERT(self && "List_clear");\
	CSTL_ASSERT(self->magic == self && "List_clear");\
	Name##_erase_range(self, Name##_begin(self).internal.data, Name##_end(self).internal.data);\
}\
\
Type *Name##Iterator_data(CstlIterInternalData pos)\
{\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos) && "List_data");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos)->magic && "List_data");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos)->magic == CSTL_MAGIC_LIST(Name) && "List_data");\
	return &CSTL_LIST_NODE(Name, pos)->data;\
}\
\
Type *Name##_front(Name *self)\
{\
	CSTL_ASSERT(self && "List_front");\
	CSTL_ASSERT(self->magic == self && "List_front");\
	CSTL_ASSERT(!Name##_empty(self) && "List_front");\
	return &CSTL_LIST_BEGIN_NODE(self)->data;\
}\
\
Type *Name##_back(Name *self)\
{\
	CSTL_ASSERT(self && "List_back");\
	CSTL_ASSERT(self->magic == self && "List_back");\
	CSTL_ASSERT(!Name##_empty(self) && "List_back");\
	return &CSTL_LIST_RBEGIN_NODE(self)->data;\
}\
\
Name##Iterator Name##_begin(Name *self)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(self && "List_begin");\
	CSTL_ASSERT(self->magic == self && "List_begin");\
	iter.vptr = &Name##Iterator_vtbl;\
	iter.internal.in_vptr = &Name##Iterator_in_vtbl;\
	CSTL_LIST_NODE_ASSIGN(iter.internal.data) = CSTL_LIST_BEGIN_NODE(self);\
	return iter;\
}\
\
Name##Iterator Name##_end(Name *self)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(self && "List_end");\
	CSTL_ASSERT(self->magic == self && "List_end");\
	iter.vptr = &Name##Iterator_vtbl;\
	iter.internal.in_vptr = &Name##Iterator_in_vtbl;\
	CSTL_LIST_NODE_ASSIGN(iter.internal.data) = CSTL_LIST_END_NODE(self);\
	return iter;\
}\
\
Name##Iterator Name##_rbegin(Name *self)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(self && "List_rbegin");\
	CSTL_ASSERT(self->magic == self && "List_rbegin");\
	iter.vptr = &Name##Iterator_vtbl;\
	iter.internal.in_vptr = &Name##Iterator_in_vtbl;\
	CSTL_LIST_NODE_ASSIGN(iter.internal.data) = CSTL_LIST_RBEGIN_NODE(self);\
	return iter;\
}\
\
Name##Iterator Name##_rend(Name *self)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(self && "List_rend");\
	CSTL_ASSERT(self->magic == self && "List_rend");\
	iter.vptr = &Name##Iterator_vtbl;\
	iter.internal.in_vptr = &Name##Iterator_in_vtbl;\
	CSTL_LIST_NODE_ASSIGN(iter.internal.data) = CSTL_LIST_REND_NODE(self);\
	return iter;\
}\
\
Name##Iterator Name##Iterator_next(CstlIterInternalData pos)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos) && "List_next");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos)->magic && "List_next");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos)->magic == CSTL_MAGIC_LIST(Name) && "List_next");\
	iter.vptr = &Name##Iterator_vtbl;\
	iter.internal.in_vptr = &Name##Iterator_in_vtbl;\
	CSTL_LIST_NODE_ASSIGN(iter.internal.data) = CSTL_LIST_NODE(Name, pos)->next;\
	return iter;\
}\
\
Name##Iterator Name##Iterator_prev(CstlIterInternalData pos)\
{\
	Name##Iterator iter;\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos) && "List_prev");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos)->magic && "List_prev");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos)->magic == CSTL_MAGIC_LIST(Name) && "List_prev");\
	iter.vptr = &Name##Iterator_vtbl;\
	iter.internal.in_vptr = &Name##Iterator_in_vtbl;\
	CSTL_LIST_NODE_ASSIGN(iter.internal.data) = CSTL_LIST_NODE(Name, pos)->prev;\
	return iter;\
}\
\
void Name##Iterator_incr(CstlIterInternalData *pos)\
{\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, *pos) && "List_incr");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, *pos)->magic && "List_incr");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, *pos)->magic == CSTL_MAGIC_LIST(Name) && "List_incr");\
	CSTL_LIST_NODE_ASSIGN(*pos) = CSTL_LIST_NODE(Name, *pos)->next;\
}\
\
void Name##Iterator_decr(CstlIterInternalData *pos)\
{\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, *pos) && "List_decr");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, *pos)->magic && "List_decr");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, *pos)->magic == CSTL_MAGIC_LIST(Name) && "List_decr");\
	CSTL_LIST_NODE_ASSIGN(*pos) = CSTL_LIST_NODE(Name, *pos)->prev;\
}\
\
int Name##Iterator_eq(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	return CSTL_LIST_NODE(Name, pos) == CSTL_LIST_NODE(Name, x);\
}\
\
int Name##Iterator_ne(CstlIterInternalData pos, CstlIterInternalData x)\
{\
	return CSTL_LIST_NODE(Name, pos) != CSTL_LIST_NODE(Name, x);\
}\
\
int Name##_insert(Name *self, CstlIterInternalData pos, Type data, Name##Iterator *iter)\
{\
	CSTL_ASSERT(self && "List_insert");\
	CSTL_ASSERT(self->magic == self && "List_insert");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos) && "List_insert");\
	CSTL_ASSERT((CSTL_LIST_NODE(Name, pos)->magic == CSTL_MAGIC_LIST(Name) || \
				CSTL_LIST_NODE(Name, pos)->magic == self) && "List_insert");\
	return Name##_insert_ref(self, pos, &data, iter);\
}\
\
int Name##_insert_ref(Name *self, CstlIterInternalData pos, Type const *data, Name##Iterator *iter)\
{\
	Name *node;\
	CSTL_ASSERT(self && "List_insert_ref");\
	CSTL_ASSERT(self->magic == self && "List_insert_ref");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos) && "List_insert_ref");\
	CSTL_ASSERT((CSTL_LIST_NODE(Name, pos)->magic == CSTL_MAGIC_LIST(Name) || \
				CSTL_LIST_NODE(Name, pos)->magic == self) && "List_insert_ref");\
	CSTL_ASSERT(data && "List_insert_ref");\
	CSTL_UNUSED_PARAM(self);\
	node = (Name *) malloc(sizeof(Name));\
	if (!node) return 0;\
	node->data = *data;\
	node->next = CSTL_LIST_NODE(Name, pos);\
	node->prev = CSTL_LIST_NODE(Name, pos)->prev;\
	CSTL_LIST_NODE(Name, pos)->prev = node;\
	node->prev->next = node;\
	CSTL_MAGIC(node->magic = CSTL_MAGIC_LIST(Name));\
	if (iter) {\
		iter->vptr = &Name##Iterator_vtbl;\
		iter->internal.in_vptr = &Name##Iterator_in_vtbl;\
		CSTL_LIST_NODE_ASSIGN(iter->internal.data) = node;\
	}\
	return 1;\
}\
\
int Name##_insert_n(Name *self, CstlIterInternalData pos, size_t n, Type data)\
{\
	CSTL_ASSERT(self && "List_insert_n");\
	CSTL_ASSERT(self->magic == self && "List_insert_n");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos) && "List_insert_n");\
	CSTL_ASSERT((CSTL_LIST_NODE(Name, pos)->magic == CSTL_MAGIC_LIST(Name) || \
				CSTL_LIST_NODE(Name, pos)->magic == self) && "List_insert_n");\
	return Name##_insert_n_ref(self, pos, n, &data);\
}\
\
int Name##_insert_n_ref(Name *self, CstlIterInternalData pos, size_t n, Type const *data)\
{\
	Name x;\
	register size_t i;\
	Name##Iterator iter;\
	CSTL_ASSERT(self && "List_insert_n_ref");\
	CSTL_ASSERT(self->magic == self && "List_insert_n_ref");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos) && "List_insert_n_ref");\
	CSTL_ASSERT((CSTL_LIST_NODE(Name, pos)->magic == CSTL_MAGIC_LIST(Name) || \
				CSTL_LIST_NODE(Name, pos)->magic == self) && "List_insert_n_ref");\
	CSTL_ASSERT(data && "List_insert_n_ref");\
	x.vptr = &Name##_vtbl;\
	x.next = &x;\
	x.prev = &x;\
	CSTL_MAGIC(x.magic = &x);\
	iter = Name##_end(&x);\
	for (i = 0; i < n; i++) {\
		if (!Name##_insert_ref(&x, iter.internal.data, data, 0)) {\
			Name##_clear(&x);\
			return 0;\
		}\
	}\
	Name##_splice(self, pos, &x, Name##_begin(&x).internal.data, iter.internal.data);\
	return 1;\
}\
\
int Name##_insert_array(Name *self, CstlIterInternalData pos, Type const *data, size_t n)\
{\
	Name x;\
	register size_t i;\
	Name##Iterator iter;\
	CSTL_ASSERT(self && "List_insert_array");\
	CSTL_ASSERT(self->magic == self && "List_insert_array");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos) && "List_insert_array");\
	CSTL_ASSERT((CSTL_LIST_NODE(Name, pos)->magic == CSTL_MAGIC_LIST(Name) || \
				CSTL_LIST_NODE(Name, pos)->magic == self) && "List_insert_array");\
	CSTL_ASSERT(data && "List_insert_array");\
	x.vptr = &Name##_vtbl;\
	x.next = &x;\
	x.prev = &x;\
	CSTL_MAGIC(x.magic = &x);\
	iter = Name##_end(&x);\
	for (i = 0; i < n; i++) {\
		if (!Name##_insert_ref(&x, iter.internal.data, &data[i], 0)) {\
			Name##_clear(&x);\
			return 0;\
		}\
	}\
	Name##_splice(self, pos, &x, Name##_begin(&x).internal.data, iter.internal.data);\
	return 1;\
}\
\
int Name##_insert_range(Name *self, CstlIterInternalData pos, CstlIterInternal first, CstlIterInternal last)\
{\
	Name x;\
	CstlIterInternal i;\
	Name##Iterator iter;\
	CSTL_ASSERT(self && "List_insert_range");\
	CSTL_ASSERT(self->magic == self && "List_insert_range");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos) && "List_insert_range");\
	CSTL_ASSERT((CSTL_LIST_NODE(Name, pos)->magic == CSTL_MAGIC_LIST(Name) || \
				CSTL_LIST_NODE(Name, pos)->magic == self) && "List_insert_range");\
/*	CSTL_ASSERT(CSTL_LIST_NODE(Name, first.data) && "List_insert_range");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, last.data) && "List_insert_range");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, first.data)->magic && "List_insert_range");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, last.data)->magic && "List_insert_range");\*/\
	x.vptr = &Name##_vtbl;\
	x.next = &x;\
	x.prev = &x;\
	CSTL_MAGIC(x.magic = &x);\
	iter = Name##_end(&x);\
	for (i = first; i.in_vptr->ne(i.data, last.data); i.in_vptr->incr(&i.data)) {\
		/*CSTL_ASSERT(i->magic && "List_insert_range");*/\
		if (!Name##_insert_ref(&x, iter.internal.data, (Type *) i.in_vptr->data(i.data), 0)) {\
			Name##_clear(&x);\
			return 0;\
		}\
	}\
	Name##_splice(self, pos, &x, Name##_begin(&x).internal.data, iter.internal.data);\
	return 1;\
}\
\
static Name *Name##_erase_aux(Name *pos)\
{\
	Name *node;\
	node = pos->next;\
	pos->prev->next = pos->next;\
	pos->next->prev = pos->prev;\
	CSTL_MAGIC(pos->magic = 0);\
	free(pos);\
	return node;\
}\
\
Name##Iterator Name##_erase(Name *self, CstlIterInternalData pos)\
{\
	Name *node;\
	Name##Iterator iter;\
	CSTL_ASSERT(self && "List_erase");\
	CSTL_ASSERT(self->magic == self && "List_erase");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos) && "List_erase");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos) != self && "List_erase");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos)->magic == CSTL_MAGIC_LIST(Name) && "List_erase");\
	CSTL_ASSERT(!Name##_empty(self) && "List_erase");\
	CSTL_UNUSED_PARAM(self);\
	node = Name##_erase_aux(CSTL_LIST_NODE(Name, pos));\
	iter.vptr = &Name##Iterator_vtbl;\
	iter.internal.in_vptr = &Name##Iterator_in_vtbl;\
	CSTL_LIST_NODE_ASSIGN(iter.internal.data) = node;\
	return iter;\
}\
\
Name##Iterator Name##_erase_range(Name *self, CstlIterInternalData first, CstlIterInternalData last)\
{\
	Name *node;\
	Name##Iterator iter;\
	CSTL_ASSERT(self && "List_erase_range");\
	CSTL_ASSERT(self->magic == self && "List_erase_range");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, first) && "List_erase_range");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, last) && "List_erase_range");\
	CSTL_ASSERT((CSTL_LIST_NODE(Name, first)->magic == CSTL_MAGIC_LIST(Name) || \
				CSTL_LIST_NODE(Name, first)->magic == self) && "List_erase_range");\
	CSTL_ASSERT((CSTL_LIST_NODE(Name, last)->magic == CSTL_MAGIC_LIST(Name) || \
				CSTL_LIST_NODE(Name, last)->magic == self) && "List_erase_range");\
	node = CSTL_LIST_NODE(Name, first);\
	while (node != CSTL_LIST_NODE(Name, last)) {\
		CSTL_ASSERT(!Name##_empty(self) && "List_erase_range");\
		node = Name##_erase_aux(node);\
	}\
	iter.vptr = &Name##Iterator_vtbl;\
	iter.internal.in_vptr = &Name##Iterator_in_vtbl;\
	CSTL_LIST_NODE_ASSIGN(iter.internal.data) = node;\
	return iter;\
}\
\
int Name##_resize(Name *self, size_t n, Type data)\
{\
	register size_t i;\
	size_t size;\
	CSTL_ASSERT(self && "List_resize");\
	CSTL_ASSERT(self->magic == self && "List_resize");\
	size = Name##_size(self);\
	if (size >= n) {\
		for (i = 0; i < size - n; i++) {\
			Name##_erase_aux(CSTL_LIST_RBEGIN_NODE(self));\
		}\
	} else {\
		Name x;\
		Name##Iterator iter;\
		x.vptr = &Name##_vtbl;\
		x.next = &x;\
		x.prev = &x;\
		CSTL_MAGIC(x.magic = &x);\
		iter = Name##_end(&x);\
		for (i = 0; i < n - size; i++) {\
			if (!Name##_insert_ref(&x, iter.internal.data, &data, 0)) {\
				Name##_clear(&x);\
				return 0;\
			}\
		}\
		Name##_splice(self, Name##_end(self).internal.data, &x, Name##_begin(&x).internal.data, iter.internal.data);\
	}\
	return 1;\
}\
\
void Name##_swap(Name *self, Name *x)\
{\
	Name *tmp_next;\
	Name *tmp_prev;\
	CSTL_ASSERT(self && "List_swap");\
	CSTL_ASSERT(x && "List_swap");\
	CSTL_ASSERT(self->magic == self && "List_swap");\
	CSTL_ASSERT(x->magic == x && "List_swap");\
	self->next->prev = x;\
	self->prev->next = x;\
	x->next->prev = self;\
	x->prev->next = self;\
	tmp_next = self->next;\
	tmp_prev = self->prev;\
	self->next = x->next;\
	self->prev = x->prev;\
	x->next = tmp_next;\
	x->prev = tmp_prev;\
}\
\
void Name##_splice(Name *self, CstlIterInternalData pos, Name *x, CstlIterInternalData first, CstlIterInternalData last)\
{\
	Name *tmp;\
	CSTL_ASSERT(self && "List_splice");\
	CSTL_ASSERT(self->magic == self && "List_splice");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, pos) && "List_splice");\
	CSTL_ASSERT((CSTL_LIST_NODE(Name, pos)->magic == CSTL_MAGIC_LIST(Name) || \
				CSTL_LIST_NODE(Name, pos)->magic == self) && "List_splice");\
	CSTL_ASSERT(x && "List_splice");\
	CSTL_ASSERT(x->magic == x && "List_splice");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, first) && "List_splice");\
	CSTL_ASSERT(CSTL_LIST_NODE(Name, last) && "List_splice");\
	CSTL_ASSERT((CSTL_LIST_NODE(Name, first)->magic == CSTL_MAGIC_LIST(Name) || \
				CSTL_LIST_NODE(Name, first)->magic == x) && "List_splice");\
	CSTL_ASSERT((CSTL_LIST_NODE(Name, last)->magic == CSTL_MAGIC_LIST(Name) || \
				CSTL_LIST_NODE(Name, last)->magic == x) && "List_splice");\
	CSTL_UNUSED_PARAM(self);\
	CSTL_UNUSED_PARAM(x);\
	if (CSTL_LIST_NODE(Name, first) == CSTL_LIST_NODE(Name, last) || \
			CSTL_LIST_NODE(Name, pos) == CSTL_LIST_NODE(Name, last)) {\
		return;\
	}\
	CSTL_LIST_NODE(Name, pos)->prev->next = CSTL_LIST_NODE(Name, first);\
	tmp = CSTL_LIST_NODE(Name, first)->prev;\
	CSTL_LIST_NODE(Name, first)->prev = CSTL_LIST_NODE(Name, pos)->prev;\
	tmp->next = CSTL_LIST_NODE(Name, last);\
	CSTL_LIST_NODE(Name, pos)->prev = CSTL_LIST_NODE(Name, last)->prev;\
	CSTL_LIST_NODE(Name, last)->prev->next = CSTL_LIST_NODE(Name, pos);\
	CSTL_LIST_NODE(Name, last)->prev = tmp;\
}\
\
static Name *Name##_merge_list(Name *x, Name *y, int (*comp)(const void *, const void *))\
{\
	register Name *p;\
	Name *xlast;\
	Name *ylast;\
	Name head;\
	xlast = x->prev;\
	ylast = y->prev;\
	p = &head;\
	x->prev->next = &head;\
	y->prev->next = &head;\
	while (x != &head && y != &head) {\
		if (comp(&x->data, &y->data) <= 0) {\
			p->next = x;\
			x->prev = p;\
			p = x;\
			x = x->next;\
		} else {\
			p->next = y;\
			y->prev = p;\
			p = y;\
			y = y->next;\
		}\
	}\
	if (x == &head) {\
		p->next = y;\
		y->prev = p;\
		ylast->next = head.next;\
		head.next->prev = ylast;\
	} else {\
		p->next = x;\
		x->prev = p;\
		xlast->next = head.next;\
		head.next->prev = xlast;\
	}\
	return head.next;\
}\
\
static Name *Name##_merge_sort(Name *first, int (*comp)(const void *, const void *))\
{\
	register Name *x;\
	register Name *y;\
	Name *tmp;\
	if (first->next == first) {\
		return first;\
	}\
	x = first;\
	y = first;\
	do {\
		x = x->next;\
		y = y->next;\
		if (y != first) {\
			y = y->next;\
		}\
	} while (y != first);\
	tmp = x->prev;\
	x->prev->next = first;\
	x->prev = first->prev;\
	x->prev->next = x;\
	first->prev = tmp;\
	return Name##_merge_list(Name##_merge_sort(first, comp), Name##_merge_sort(x, comp), comp);\
}\
\
void Name##_sort(Name *self, int (*comp)(const void *, const void *))\
{\
	Name *first;\
	Name *last;\
	CSTL_ASSERT(self && "List_sort");\
	CSTL_ASSERT(self->magic == self && "List_sort");\
	CSTL_ASSERT(comp && "List_sort");\
	if (Name##_empty(self)) {\
		return;\
	}\
	/* 一時的に循環リストにする */\
	first = self->next;\
	last = self->prev;\
	first->prev = last;\
	last->next = first;\
	first = Name##_merge_sort(first, comp);\
	last = first->prev;\
	self->next = first;\
	first->prev = self;\
	last->next = self;\
	self->prev = last;\
}\
\
void Name##_merge(Name *self, Name *x, int (*comp)(const void *, const void *))\
{\
	Name *first1;\
	Name *last1;\
	Name *first2;\
	Name *last2;\
	CSTL_ASSERT(self && "List_merge");\
	CSTL_ASSERT(self->magic == self && "List_merge");\
	CSTL_ASSERT(x && "List_merge");\
	CSTL_ASSERT(x->magic == x && "List_merge");\
	CSTL_ASSERT(comp && "List_merge");\
	if (self == x || Name##_empty(x)) return;\
	if (Name##_empty(self)) {\
		Name##_splice(self, Name##_end(self).internal.data, x, Name##_begin(x).internal.data, Name##_end(x).internal.data);\
		return;\
	}\
	/* 一時的に循環リストにする */\
	first1 = self->next;\
	last1 = self->prev;\
	first1->prev = last1;\
	last1->next = first1;\
	first2 = x->next;\
	last2 = x->prev;\
	first2->prev = last2;\
	last2->next = first2;\
	first1 = Name##_merge_list(first1, first2, comp);\
	last1 = first1->prev;\
	self->next = first1;\
	first1->prev = self;\
	last1->next = self;\
	self->prev = last1;\
	x->next = x;\
	x->prev = x;\
}\
\
void Name##_reverse(Name *self)\
{\
	register Name *p;\
	register Name *tmp;\
	CSTL_ASSERT(self && "List_reverse");\
	CSTL_ASSERT(self->magic == self && "List_reverse");\
	p = self;\
	do {\
		tmp = p->next;\
		p->next = p->prev;\
		p->prev = tmp;\
		p = tmp;\
	} while (p != self);\
}\
\


#endif /* CSTL_LIST_H_INCLUDED */
