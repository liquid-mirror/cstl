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

#define cstl_delete(s)					((s) ? (s)->u.vptr->delete_(s) : (void)0)
#define cstl_push_back(s, d)			((s)->u.vptr->push_back(s, d))
#define cstl_push_front(s, d)			((s)->u.vptr->push_front(s, d))
#define cstl_push_back_ref(s, d)		((s)->u.vptr->push_back_ref(s, d))
#define cstl_push_front_ref(s, d)		((s)->u.vptr->push_front_ref(s, d))
#define cstl_pop_front(s)				((s)->u.vptr->pop_front(s))
#define cstl_pop_back(s)				((s)->u.vptr->pop_back(s))
#define cstl_empty(s)					((s)->u.vptr->empty(s))
#define cstl_size(s)					((s)->u.vptr->size(s))
#define cstl_clear(s)					((s)->u.vptr->clear(s))
#define cstl_front(s)					((s)->u.vptr->front(s))
#define cstl_back(s)					((s)->u.vptr->back(s))
#define cstl_begin(s)					((s)->u.vptr->begin(s))
#define cstl_end(s)						((s)->u.vptr->end(s))
#define cstl_rbegin(s)					((s)->u.vptr->rbegin(s))
#define cstl_rend(s)					((s)->u.vptr->rend(s))
#define cstl_insert(s, p, d, i)			((s)->u.vptr->insert(s, (p).internal.data, d, i))
#define cstl_insert_ref(s, p, d, i)		((s)->u.vptr->insert_ref(s, (p).internal.data, d, i))
#define cstl_insert_n(s, p, n, d)		((s)->u.vptr->insert_n(s, (p).internal.data, n, d))
#define cstl_insert_n_ref(s, p, n, d)	((s)->u.vptr->insert_n_ref(s, (p).internal.data, n, d))
#define cstl_insert_array(s, p, d, n)	((s)->u.vptr->insert_array(s, (p).internal.data, d, n))
#define cstl_insert_range(s, p, f, l)	((s)->u.vptr->insert_range(s, (p).internal.data, (f).internal, (l).internal))
#define cstl_erase(s, p)				((s)->u.vptr->erase(s, (p).internal.data))
#define cstl_erase_range(s, f, l)		((s)->u.vptr->erase_range(s, (f).internal.data, (l).internal.data))
#define cstl_resize(s, n, d)			((s)->u.vptr->resize(s, n, d))
#define cstl_swap(s, x)					((s)->u.vptr->swap(s, x))
#define cstl_capacity(s)				((s)->u.vptr->capacity(s))
#define cstl_reserve(s, n)				((s)->u.vptr->reserve(s, n))
#define cstl_shrink(s, n)				((s)->u.vptr->shrink(s, n))
#define cstl_at(s, i)					((s)->u.vptr->at(s, i))
#define cstl_splice(s, p, x, f, l)		((s)->u.vptr->splice(s, (p).internal.data, x, (f).internal.data, (l).internal.data))
#define cstl_sort(s, c)					((s)->u.vptr->sort(s, c))
#define cstl_merge(s, x, c)				((s)->u.vptr->merge(s, x, c))
#define cstl_reverse(s)					((s)->u.vptr->reverse(s))
#define cstl_insert_set(s, d, i, su)	((s)->u.vptr->insert_set(s, d, i, su))
#define cstl_insert_multiset(s, d, i)	((s)->u.vptr->insert_multiset(s, d, i))
#define cstl_insert_range_assoc(s, f, l)	((s)->u.vptr->insert_range_assoc(s, (f).internal, (l).internal))
#define cstl_erase_key(s, k)			((s)->u.vptr->erase_key(s, k))
#define cstl_count(s, k)				((s)->u.vptr->count(s, k))
#define cstl_find(s, k)					((s)->u.vptr->find(s, k))
#define cstl_lower_bound(s, k)			((s)->u.vptr->lower_bound(s, k))
#define cstl_upper_bound(s, k)			((s)->u.vptr->upper_bound(s, k))
#define cstl_equal_range(s, f, l)		((s)->u.vptr->equal_range(s, &(f), &(l)))

#define cstl_iter_data(p)		((p).vptr->data((p).internal.data))
#define cstl_iter_key(p)		((p).vptr->key((p).internal.data))
#define cstl_iter_val(p)		((p).vptr->val((p).internal.data))
#define cstl_iter_next(p)		((p).vptr->next((p).internal.data))
#define cstl_iter_prev(p)		((p).vptr->prev((p).internal.data))
#define cstl_iter_inc(pp)		((pp)->vptr->inc(&(pp)->internal.data))
#define cstl_iter_dec(pp)		((pp)->vptr->dec(&(pp)->internal.data))
#define cstl_iter_eq(p, x)		((p).vptr->eq((p).internal.data, (x).internal.data))
#define cstl_iter_ne(p, x)		((p).vptr->ne((p).internal.data, (x).internal.data))
#define cstl_iter_at(p, n)		((p).vptr->at((p).internal.data, n))
#define cstl_iter_add(p, n)		((p).vptr->add((p).internal.data, n))
#define cstl_iter_sub(p, n)		((p).vptr->sub((p).internal.data, n))
#define cstl_iter_inc_n(pp, n)	((pp)->vptr->inc_n(&(pp)->internal.data, n))
#define cstl_iter_dec_n(pp, n)	((pp)->vptr->dec_n(&(pp)->internal.data, n))
#define cstl_iter_diff(p, x)	((p).vptr->diff((p).internal.data, (x).internal.data))
#define cstl_iter_lt(p, x)		((p).vptr->lt((p).internal.data, (x).internal.data))
#define cstl_iter_le(p, x)		((p).vptr->le((p).internal.data, (x).internal.data))
#define cstl_iter_gt(p, x)		((p).vptr->gt((p).internal.data, (x).internal.data))
#define cstl_iter_ge(p, x)		((p).vptr->ge((p).internal.data, (x).internal.data))
#define cstl_iter_reverse_iterator(p)	((p).vptr->reverse_iterator((p).internal.data))
#define cstl_riter_base(p)		((p).vptr->base((p).internal.data))

#define CSTL_CAST_VPTR(Name, in_vptr)	((const Name##IteratorVtable *) (in_vptr))

enum {
	CSTL_CONTAINER_VECTOR,
	CSTL_CONTAINER_DEQUE,
	CSTL_CONTAINER_LIST,
	CSTL_CONTAINER_SET,
	CSTL_CONTAINER_MAP,
	CSTL_CONTAINER_UNORDERED_SET,
	CSTL_CONTAINER_UNORDERED_MAP,
	CSTL_CONTAINER_STRING
};

typedef struct CstlIteratorDummyVtable CstlIteratorDummyVtable;

typedef struct CstlIterInternalData {
	void *data1;
	void *data2;
	size_t data3;
} CstlIterInternalData;

typedef struct CstlIterInternal {
	const CstlIteratorDummyVtable *in_vptr;
	CstlIterInternalData data;
} CstlIterInternal;

struct CstlIteratorDummyVtable {
	void (*dummy)(void);
};


#endif /* CSTL_COMMON_H_INCLUDED */
