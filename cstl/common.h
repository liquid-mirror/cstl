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
#else
#define CSTL_MAGIC(x)
#define CSTL_ASSERT(x)
#endif

#define CSTL_UNUSED_PARAM(x)	(void) x

#define cstl_delete(s)					((s) ? (s)->vptr->delete_(s) : (void)0)
#define cstl_push_back(s, d)			((s)->vptr->push_back(s, d))
#define cstl_push_front(s, d)			((s)->vptr->push_front(s, d))
#define cstl_push_back_ref(s, d)		((s)->vptr->push_back_ref(s, d))
#define cstl_push_front_ref(s, d)		((s)->vptr->push_front_ref(s, d))
#define cstl_pop_front(s)				((s)->vptr->pop_front(s))
#define cstl_pop_back(s)				((s)->vptr->pop_back(s))
#define cstl_empty(s)					((s)->vptr->empty(s))
#define cstl_size(s)					((s)->vptr->size(s))
#define cstl_clear(s)					((s)->vptr->clear(s))
#define cstl_front(s)					((s)->vptr->front(s))
#define cstl_back(s)					((s)->vptr->back(s))
#define cstl_begin(s)					((s)->vptr->begin(s))
#define cstl_end(s)						((s)->vptr->end(s))
#define cstl_rbegin(s)					((s)->vptr->rbegin(s))
#define cstl_rend(s)					((s)->vptr->rend(s))
#define cstl_insert(s, p, d, i)			((s)->vptr->insert(s, (p).internal.data, d, i))
#define cstl_insert_ref(s, p, d, i)		((s)->vptr->insert_ref(s, (p).internal.data, d, i))
#define cstl_insert_n(s, p, n, d)		((s)->vptr->insert_n(s, (p).internal.data, n, d))
#define cstl_insert_n_ref(s, p, n, d)	((s)->vptr->insert_n_ref(s, (p).internal.data, n, d))
#define cstl_insert_array(s, p, d, n)	((s)->vptr->insert_array(s, (p).internal.data, d, n))
#define cstl_insert_range(s, p, f, l)	((s)->vptr->insert_range(s, (p).internal.data, (f).internal, (l).internal))
#define cstl_erase(s, p)				((s)->vptr->erase(s, (p).internal.data))
#define cstl_erase_range(s, f, l)		((s)->vptr->erase_range(s, (f).internal.data, (l).internal.data))
#define cstl_resize(s, n, d)			((s)->vptr->resize(s, n, d))
#define cstl_swap(s, x)					((s)->vptr->swap(s, x))
#define cstl_capacity(s)				((s)->vptr->capacity(s))
#define cstl_reserve(s, n)				((s)->vptr->reserve(s, n))
#define cstl_shrink(s, n)				((s)->vptr->shrink(s, n))
#define cstl_at(s, i)					((s)->vptr->at(s, i))
#define cstl_splice(s, p, x, f, l)		((s)->vptr->splice(s, (p).internal.data, x, (f).internal.data, (l).internal.data))
#define cstl_sort(s, c)					((s)->vptr->sort(s, c))
#define cstl_merge(s, x, c)				((s)->vptr->merge(s, x, c))
#define cstl_reverse(s)					((s)->vptr->reverse(s))
#define cstl_erase_key(s, k)			((s)->vptr->erase_key(s, k))
#define cstl_count(s, k)				((s)->vptr->count(s, k))
#define cstl_find(s, k)					((s)->vptr->find(s, k))
#define cstl_lower_bound(s, k)			((s)->vptr->lower_bound(s, k))
#define cstl_upper_bound(s, k)			((s)->vptr->upper_bound(s, k))
#define cstl_equal_range(s, f, l)		((s)->vptr->equal_range(s, &(f), &(l)))

#define cstl_iter_data(p)		((p).vptr->data((p).internal.data))
#define cstl_iter_key(p)		((p).vptr->key((p).internal.data))
#define cstl_iter_val(p)		((p).vptr->val((p).internal.data))
#define cstl_iter_next(p)		((p).vptr->next((p).internal.data))
#define cstl_iter_prev(p)		((p).vptr->prev((p).internal.data))
#define cstl_iter_incr(pp)		((pp)->vptr->incr(&(pp)->internal.data))
#define cstl_iter_decr(pp)		((pp)->vptr->decr(&(pp)->internal.data))
#define cstl_iter_eq(p, x)		((p).vptr->eq((p).internal.data, (x).internal.data))
#define cstl_iter_ne(p, x)		((p).vptr->ne((p).internal.data, (x).internal.data))
#define cstl_iter_at(p, n)		((p).vptr->at((p).internal.data, n))
#define cstl_iter_add(p, n)		((p).vptr->add((p).internal.data, n))
#define cstl_iter_sub(p, n)		((p).vptr->sub((p).internal.data, n))
#define cstl_iter_incr_n(pp, n)	((pp)->vptr->incr_n(&(pp)->internal.data, n))
#define cstl_iter_decr_n(pp, n)	((pp)->vptr->decr_n(&(pp)->internal.data, n))
#define cstl_iter_diff(p, x)	((p).vptr->diff((p).internal.data, (x).internal.data))
#define cstl_iter_lt(p, x)		((p).vptr->lt((p).internal.data, (x).internal.data))
#define cstl_iter_le(p, x)		((p).vptr->le((p).internal.data, (x).internal.data))
#define cstl_iter_gt(p, x)		((p).vptr->gt((p).internal.data, (x).internal.data))
#define cstl_iter_ge(p, x)		((p).vptr->ge((p).internal.data, (x).internal.data))

typedef struct CstlIteratorVtable CstlIteratorVtable;

typedef struct CstlIterInternalData {
	void *data1;
	void *data2;
	void *data3;
} CstlIterInternalData;

typedef struct CstlIterInternal {
	const CstlIteratorVtable *in_vptr;
	CstlIterInternalData data;
} CstlIterInternal;

typedef struct CstlIterator {
	const CstlIteratorVtable *vptr;
	CstlIterInternal internal;
} CstlIterator;

typedef void *(*CstlIterator_data_t)(CstlIterInternalData pos);
typedef void *(*CstlIterator_key_t)(CstlIterInternalData pos);
typedef void *(*CstlIterator_val_t)(CstlIterInternalData pos);
typedef CstlIterator (*CstlIterator_next_t)(CstlIterInternalData pos);
typedef CstlIterator (*CstlIterator_prev_t)(CstlIterInternalData pos);
typedef void (*CstlIterator_incr_t)(CstlIterInternalData *pos);
typedef void (*CstlIterator_decr_t)(CstlIterInternalData *pos);
typedef int (*CstlIterator_eq_t)(CstlIterInternalData pos, CstlIterInternalData x);
typedef int (*CstlIterator_ne_t)(CstlIterInternalData pos, CstlIterInternalData x);
typedef void *(*CstlIterator_at_t)(CstlIterInternalData pos, size_t n);
typedef CstlIterator (*CstlIterator_add_t)(CstlIterInternalData pos, size_t n);
typedef CstlIterator (*CstlIterator_sub_t)(CstlIterInternalData pos, size_t n);
typedef void (*CstlIterator_incr_n_t)(CstlIterInternalData *pos, size_t n);
typedef void (*CstlIterator_decr_n_t)(CstlIterInternalData *pos, size_t n);
typedef int (*CstlIterator_diff_t)(CstlIterInternalData pos, CstlIterInternalData x);
typedef int (*CstlIterator_lt_t)(CstlIterInternalData pos, CstlIterInternalData x);
typedef int (*CstlIterator_le_t)(CstlIterInternalData pos, CstlIterInternalData x);
typedef int (*CstlIterator_gt_t)(CstlIterInternalData pos, CstlIterInternalData x);
typedef int (*CstlIterator_ge_t)(CstlIterInternalData pos, CstlIterInternalData x);

struct CstlIteratorVtable {
	CstlIterator_data_t   data;
	CstlIterator_key_t    key;
	CstlIterator_val_t    val;
	CstlIterator_next_t   next;
	CstlIterator_prev_t   prev;
	CstlIterator_incr_t   incr;
	CstlIterator_decr_t   decr;
	CstlIterator_eq_t     eq;
	CstlIterator_ne_t     ne;
	CstlIterator_at_t     at;
	CstlIterator_add_t    add;
	CstlIterator_sub_t    sub;
	CstlIterator_incr_n_t incr_n;
	CstlIterator_decr_n_t decr_n;
	CstlIterator_diff_t   diff;
	CstlIterator_lt_t     lt;
	CstlIterator_le_t     le;
	CstlIterator_gt_t     gt;
	CstlIterator_ge_t     ge;
	int is_rand_iter;
};


#endif /* CSTL_COMMON_H_INCLUDED */
