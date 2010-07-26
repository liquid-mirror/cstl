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
 * \file common.h
 * \brief 共通マクロ定義
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2009-12-21
 * $URL$
 * $Id$
 */
#ifndef CSTL_COMMON_H_INCLUDED
#define CSTL_COMMON_H_INCLUDED


#ifdef __cplusplus
#define CSTL_EXTERN_C_BEGIN()	extern "C" {
#define CSTL_EXTERN_C_END()		}
#else
#define CSTL_EXTERN_C_BEGIN()
#define CSTL_EXTERN_C_END()
#endif

#if !defined(NDEBUG) && defined(CSTL_DEBUG)
#include <assert.h>
#define CSTL_MAGIC(x)	x
#define CSTL_ASSERT(x)	assert(x)
#define CSTL_CHK_ITER_IS_MAP(p)		((p).vptr->container & (CSTL_CONTAINER_MAP | CSTL_CONTAINER_UNORDERED_MAP))
#define CSTL_CHK_ITER_IS_RAND(p)	((p).vptr->is_rand_iter)
#define CSTL_CHK_ITER_IS_BIDIR(p)	(!((p).vptr->container & (CSTL_CONTAINER_UNORDERED_SET | CSTL_CONTAINER_UNORDERED_MAP)))
#else
#define CSTL_MAGIC(x)
#define CSTL_ASSERT(x)	((void) 0)
#endif
#define CSTL_ASSERT_SELF(s)			(CSTL_ASSERT(s), CSTL_ASSERT((s)->magic == (s)))

#define CSTL_UNUSED_PARAM(x)	(void) x

#define cstl_delete(s)					((s) ? CSTL_ASSERT_SELF(s), (s)->u.vptr->delete_(s) : (void) 0)
#define cstl_push_back(s, d)			(CSTL_ASSERT_SELF(s), (s)->u.vptr->push_back(s, d))
#define cstl_push_front(s, d)			(CSTL_ASSERT_SELF(s), (s)->u.vptr->push_front(s, d))
#define cstl_push_back_ref(s, d)		(CSTL_ASSERT_SELF(s), (s)->u.vptr->push_back_ref(s, d))
#define cstl_push_front_ref(s, d)		(CSTL_ASSERT_SELF(s), (s)->u.vptr->push_front_ref(s, d))
#define cstl_pop_front(s)				(CSTL_ASSERT_SELF(s), (s)->u.vptr->pop_front(s))
#define cstl_pop_back(s)				(CSTL_ASSERT_SELF(s), (s)->u.vptr->pop_back(s))
#define cstl_empty(s)					(CSTL_ASSERT_SELF(s), (s)->u.vptr->empty(s))
#define cstl_size(s)					(CSTL_ASSERT_SELF(s), (s)->u.vptr->size(s))
#define cstl_clear(s)					(CSTL_ASSERT_SELF(s), (s)->u.vptr->clear(s))
#define cstl_front(s)					(CSTL_ASSERT_SELF(s), (s)->u.vptr->front(s))
#define cstl_back(s)					(CSTL_ASSERT_SELF(s), (s)->u.vptr->back(s))
#define cstl_begin(s)					(CSTL_ASSERT_SELF(s), (s)->u.vptr->begin(s))
#define cstl_end(s)						(CSTL_ASSERT_SELF(s), (s)->u.vptr->end(s))
#define cstl_rbegin(s)					(CSTL_ASSERT_SELF(s), (s)->u.vptr->rbegin(s))
#define cstl_rend(s)					(CSTL_ASSERT_SELF(s), (s)->u.vptr->rend(s))
#define cstl_insert(s, p, d, i)			(CSTL_ASSERT_SELF(s), (s)->u.vptr->insert(s, (p).internal.data, d, i))
#define cstl_insert_ref(s, p, d, i)		(CSTL_ASSERT_SELF(s), (s)->u.vptr->insert_ref(s, (p).internal.data, d, i))
#define cstl_insert_n(s, p, n, d)		(CSTL_ASSERT_SELF(s), (s)->u.vptr->insert_n(s, (p).internal.data, n, d))
#define cstl_insert_n_ref(s, p, n, d)	(CSTL_ASSERT_SELF(s), (s)->u.vptr->insert_n_ref(s, (p).internal.data, n, d))
#define cstl_insert_array(s, p, d, n)	(CSTL_ASSERT_SELF(s), (s)->u.vptr->insert_array(s, (p).internal.data, d, n))
#define cstl_insert_range(s, p, f, l)	(CSTL_ASSERT_SELF(s), (s)->u.vptr->insert_range(s, (p).internal.data, (f).internal, (l).internal))
#define cstl_erase(s, p)				(CSTL_ASSERT_SELF(s), (s)->u.vptr->erase(s, (p).internal.data))
#define cstl_erase_range(s, f, l)		(CSTL_ASSERT_SELF(s), (s)->u.vptr->erase_range(s, (f).internal.data, (l).internal.data))
#define cstl_resize(s, n, d)			(CSTL_ASSERT_SELF(s), (s)->u.vptr->resize(s, n, d))
#define cstl_swap(s, x)					(CSTL_ASSERT_SELF(s), (s)->u.vptr->swap(s, x))
#define cstl_capacity(s)				(CSTL_ASSERT_SELF(s), (s)->u.vptr->capacity(s))
#define cstl_reserve(s, n)				(CSTL_ASSERT_SELF(s), (s)->u.vptr->reserve(s, n))
#define cstl_shrink(s, n)				(CSTL_ASSERT_SELF(s), (s)->u.vptr->shrink(s, n))
#define cstl_at(s, i)					(CSTL_ASSERT_SELF(s), (s)->u.vptr->at(s, i))
#define cstl_splice(s, p, x, f, l)		(CSTL_ASSERT_SELF(s), (s)->u.vptr->splice(s, (p).internal.data, x, (f).internal.data, (l).internal.data))
#define cstl_sort(s, c)					(CSTL_ASSERT_SELF(s), (s)->u.vptr->sort(s, c))
#define cstl_merge(s, x, c)				(CSTL_ASSERT_SELF(s), (s)->u.vptr->merge(s, x, c))
#define cstl_reverse(s)					(CSTL_ASSERT_SELF(s), (s)->u.vptr->reverse(s))
#define cstl_set_insert(s, d, i, su)			(CSTL_ASSERT_SELF(s), (s)->u.vptr->set_insert(s, d, i, su))
#define cstl_multiset_insert(s, d, i)			(CSTL_ASSERT_SELF(s), (s)->u.vptr->multiset_insert(s, d, i))
#define cstl_map_insert(s, k, v, i, su)			(CSTL_ASSERT_SELF(s), (s)->u.vptr->map_insert(s, k, v, i, su))
#define cstl_multimap_insert(s, k, v, i)		(CSTL_ASSERT_SELF(s), (s)->u.vptr->multimap_insert(s, k, v, i))
#define cstl_map_insert_ref(s, k, v, i, su)		(CSTL_ASSERT_SELF(s), (s)->u.vptr->map_insert_ref(s, k, v, i, su))
#define cstl_multimap_insert_ref(s, k, v, i)	(CSTL_ASSERT_SELF(s), (s)->u.vptr->multimap_insert_ref(s, k, v, i))
#define cstl_assoc_insert_range(s, f, l)		(CSTL_ASSERT_SELF(s), (s)->u.vptr->assoc_insert_range(s, (f).internal, (l).internal))
#define cstl_erase_key(s, k)			(CSTL_ASSERT_SELF(s), (s)->u.vptr->erase_key(s, k))
#define cstl_count(s, k)				(CSTL_ASSERT_SELF(s), (s)->u.vptr->count(s, k))
#define cstl_find(s, k)					(CSTL_ASSERT_SELF(s), (s)->u.vptr->find(s, k))
#define cstl_lower_bound(s, k)			(CSTL_ASSERT_SELF(s), (s)->u.vptr->lower_bound(s, k))
#define cstl_upper_bound(s, k)			(CSTL_ASSERT_SELF(s), (s)->u.vptr->upper_bound(s, k))
#define cstl_equal_range(s, f, l)		(CSTL_ASSERT_SELF(s), (s)->u.vptr->equal_range(s, f, l))

#define cstl_iter_data(p)		(CSTL_ASSERT(!CSTL_CHK_ITER_IS_MAP(p)), (p).vptr->data((p).internal.data))
#define cstl_iter_key(p)		(CSTL_ASSERT(CSTL_CHK_ITER_IS_MAP(p)), (p).vptr->key((p).internal.data))
#define cstl_iter_val(p)		(CSTL_ASSERT(CSTL_CHK_ITER_IS_MAP(p)), (p).vptr->val((p).internal.data))
#define cstl_iter_next(p)		((p).vptr->next((p).internal.data))
#define cstl_iter_prev(p)		(CSTL_ASSERT(CSTL_CHK_ITER_IS_BIDIR(p)), (p).vptr->prev((p).internal.data))
#define cstl_iter_inc(pp)		((pp)->vptr->inc(&(pp)->internal.data))
#define cstl_iter_dec(pp)		(CSTL_ASSERT(CSTL_CHK_ITER_IS_BIDIR(*(pp))), (pp)->vptr->dec(&(pp)->internal.data))
#define cstl_iter_eq(p, x)		((p).vptr->eq((p).internal.data, (x).internal.data))
#define cstl_iter_ne(p, x)		((p).vptr->ne((p).internal.data, (x).internal.data))
#define cstl_iter_at(p, n)		(CSTL_ASSERT(CSTL_CHK_ITER_IS_RAND(p)), (p).vptr->at((p).internal.data, n))
#define cstl_iter_add(p, n)		(CSTL_ASSERT(CSTL_CHK_ITER_IS_RAND(p)), (p).vptr->add((p).internal.data, n))
#define cstl_iter_sub(p, n)		(CSTL_ASSERT(CSTL_CHK_ITER_IS_RAND(p)), (p).vptr->sub((p).internal.data, n))
#define cstl_iter_inc_n(pp, n)	(CSTL_ASSERT(CSTL_CHK_ITER_IS_RAND(*(pp))), (pp)->vptr->inc_n(&(pp)->internal.data, n))
#define cstl_iter_dec_n(pp, n)	(CSTL_ASSERT(CSTL_CHK_ITER_IS_RAND(*(pp))), (pp)->vptr->dec_n(&(pp)->internal.data, n))
#define cstl_iter_diff(p, x)	(CSTL_ASSERT(CSTL_CHK_ITER_IS_RAND(p)), (p).vptr->diff((p).internal.data, (x).internal.data))
#define cstl_iter_lt(p, x)		(CSTL_ASSERT(CSTL_CHK_ITER_IS_RAND(p)), (p).vptr->lt((p).internal.data, (x).internal.data))
#define cstl_iter_le(p, x)		(CSTL_ASSERT(CSTL_CHK_ITER_IS_RAND(p)), (p).vptr->le((p).internal.data, (x).internal.data))
#define cstl_iter_gt(p, x)		(CSTL_ASSERT(CSTL_CHK_ITER_IS_RAND(p)), (p).vptr->gt((p).internal.data, (x).internal.data))
#define cstl_iter_ge(p, x)		(CSTL_ASSERT(CSTL_CHK_ITER_IS_RAND(p)), (p).vptr->ge((p).internal.data, (x).internal.data))
#define cstl_iter_reverse_iterator(p)	(CSTL_ASSERT(CSTL_CHK_ITER_IS_BIDIR(p)), (p).vptr->reverse_iterator((p).internal.data))
#define cstl_riter_base(p)		(CSTL_ASSERT(CSTL_CHK_ITER_IS_BIDIR(p)), (p).vptr->base((p).internal.data))

enum {
	CSTL_CONTAINER_VECTOR        = 0x01, 
	CSTL_CONTAINER_DEQUE         = 0x02, 
	CSTL_CONTAINER_LIST          = 0x04, 
	CSTL_CONTAINER_SET           = 0x08, 
	CSTL_CONTAINER_MAP           = 0x10, 
	CSTL_CONTAINER_UNORDERED_SET = 0x20, 
	CSTL_CONTAINER_UNORDERED_MAP = 0x40, 
	CSTL_CONTAINER_STRING        = 0x80
};

typedef struct CstlIteratorDummyVtable CstlIteratorDummyVtable;

typedef struct CstlIterInternalData {
	void *data1;
	size_t data2;
} CstlIterInternalData;

typedef struct CstlIterInternal {
	const CstlIteratorDummyVtable *in_vptr;
	CstlIterInternalData data;
} CstlIterInternal;

struct CstlIteratorDummyVtable {
	void (*dummy)(void);
};


#define CSTL_DEFINE_ITERATOR(Name, Type, ValueType)	\
\
typedef struct Name##IteratorVtable Name##IteratorVtable;\
/*! \
 * \brief イテレータ\
 */\
typedef union Name##Iterator {\
	const Name##IteratorVtable *vptr;\
	CstlIterInternal internal;\
} Name##Iterator;\
\
typedef Name##Iterator Name##ReverseIterator;\
\
typedef Type *(*Name##Iterator_data_t)(CstlIterInternalData pos);\
typedef Type const *(*Name##Iterator_key_t)(CstlIterInternalData pos);\
typedef ValueType *(*Name##Iterator_val_t)(CstlIterInternalData pos);\
typedef Name##Iterator (*Name##Iterator_next_t)(CstlIterInternalData pos);\
typedef Name##Iterator (*Name##Iterator_prev_t)(CstlIterInternalData pos);\
typedef void (*Name##Iterator_inc_t)(CstlIterInternalData *pos);\
typedef void (*Name##Iterator_dec_t)(CstlIterInternalData *pos);\
typedef int (*Name##Iterator_eq_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef int (*Name##Iterator_ne_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef Type *(*Name##Iterator_at_t)(CstlIterInternalData pos, long n);\
typedef Name##Iterator (*Name##Iterator_add_t)(CstlIterInternalData pos, long n);\
typedef Name##Iterator (*Name##Iterator_sub_t)(CstlIterInternalData pos, long n);\
typedef void (*Name##Iterator_inc_n_t)(CstlIterInternalData *pos, long n);\
typedef void (*Name##Iterator_dec_n_t)(CstlIterInternalData *pos, long n);\
typedef long (*Name##Iterator_diff_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef int (*Name##Iterator_lt_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef int (*Name##Iterator_le_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef int (*Name##Iterator_gt_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef int (*Name##Iterator_ge_t)(CstlIterInternalData pos, CstlIterInternalData x);\
typedef Name##ReverseIterator (*Name##Iterator_reverse_iterator_t)(CstlIterInternalData pos);\
typedef Name##Iterator (*Name##ReverseIterator_base_t)(CstlIterInternalData pos);\
\
struct Name##IteratorVtable {\
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

#define CSTL_CAST_VPTR(Name, in_vptr)	((const Name##IteratorVtable *) (in_vptr))


#endif /* CSTL_COMMON_H_INCLUDED */
