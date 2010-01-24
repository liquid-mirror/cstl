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
 * \file algorithm.h
 * \brief アルゴリズム
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2007-06-20
 * $URL$
 * $Id$
 */
#ifndef CSTL_ALGORITHM_H_INCLUDED
#define CSTL_ALGORITHM_H_INCLUDED

#include <stdlib.h>
#include "common.h"


#define CSTL_ALGORITHM_SWAP(i, j, tmp)	\
	tmp = *i;\
	*i = *j;\
	*j = tmp;\

#define CSTL_ALGORITHM_SWITCH_INSERTION_SORT	(9)

#ifdef CSTL_ALGORITHM_INTERFACE
#undef CSTL_ALGORITHM_INTERFACE
#endif

#ifdef CSTL_ALGORITHM_IMPLEMENT
#undef CSTL_ALGORITHM_IMPLEMENT
#endif

#define CSTL_ALGORITHM_INTERFACE(Name, Type)	\
void Name##_sort(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *));\
void Name##_stable_sort(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *));\
size_t Name##_binary_search(Name *self, size_t idx, size_t n, Type value, int (*comp)(const void *, const void *));\
size_t Name##_lower_bound(Name *self, size_t idx, size_t n, Type value, int (*comp)(const void *, const void *));\
size_t Name##_upper_bound(Name *self, size_t idx, size_t n, Type value, int (*comp)(const void *, const void *));\
void Name##_reverse(Name *self, size_t idx, size_t n);\
void Name##_rotate(Name *self, size_t first, size_t middle, size_t last);\
int Name##_merge(Name *self, size_t idx, Name *x, size_t xidx, size_t xn, Name *y, size_t yidx, size_t yn, int (*comp)(const void *, const void *));\
void Name##_inplace_merge(Name *self, size_t first, size_t middle, size_t last, int (*comp)(const void *, const void *));\
void Name##_push_heap(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *));\
void Name##_pop_heap(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *));\
void Name##_make_heap(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *));\
void Name##_sort_heap(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *));\
void Name##_partial_sort(Name *self, size_t idx, size_t sort_n, size_t n, int (*comp)(const void *, const void *));\


#define CSTL_ALGORITHM_IMPLEMENT(Name, Type, DIRECT_ACCESS)	\
static void Name##_insertion_sort(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *))\
{\
	register size_t i, j;\
	Type tmp;\
	Type *alias1;\
	Type *alias2;\
	for (i = 1; i < n; i++) {\
		for (j = i; j > 0 && comp(alias1 = &DIRECT_ACCESS(self, idx + j - 1), alias2 = &DIRECT_ACCESS(self, idx + j)) > 0; j--) {\
			CSTL_ALGORITHM_SWAP(alias2, alias1, tmp);\
		}\
	}\
}\
\
void Name##_sort(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *))\
{\
	size_t l, r;\
	size_t middle;\
	size_t low[32];\
	size_t high[32];\
	register size_t sp;\
	register size_t i, j;\
	Type tmp;\
	CSTL_ASSERT(self && "sort");\
	CSTL_ASSERT(self->magic == self && "sort");\
	CSTL_ASSERT(Name##_size(self) >= idx + n && "sort");\
	CSTL_ASSERT(Name##_size(self) >= n && "sort");\
	CSTL_ASSERT(Name##_size(self) > idx && "sort");\
	CSTL_ASSERT(comp && "sort");\
	low[0] = idx;\
	high[0] = idx + n - 1;\
	sp = 1;\
	while (sp > 0) {\
		/* stack pop */\
		sp--;\
		l = low[sp];\
		r = high[sp];\
		if (l < r) {\
			Type *alias1;\
			Type *alias2;\
			Type *alias3;\
			if (r - l < CSTL_ALGORITHM_SWITCH_INSERTION_SORT) {\
				Name##_insertion_sort(self, l, r - l + 1, comp);\
				continue;\
			}\
			/* med3 */\
			middle = (l + r) / 2;\
			alias1 = &DIRECT_ACCESS(self, l);\
			alias2 = &DIRECT_ACCESS(self, middle);\
			alias3 = &DIRECT_ACCESS(self, r);\
			if (comp(alias1, alias2) > 0) {\
				CSTL_ALGORITHM_SWAP(alias1, alias2, tmp);\
			}\
			if (comp(alias1, alias3) > 0) {\
				CSTL_ALGORITHM_SWAP(alias1, alias3, tmp);\
			}\
			if (comp(alias2, alias3) > 0) {\
				CSTL_ALGORITHM_SWAP(alias2, alias3, tmp);\
			}\
			/* r - 1の要素をを枢軸にする */\
			alias3 = &DIRECT_ACCESS(self, r - 1);\
			CSTL_ALGORITHM_SWAP(alias2, alias3, tmp);\
			/* partition */\
			i = l;\
			j = r - 1;\
			while (1) {\
				do {\
					i++;\
				} while (comp(&DIRECT_ACCESS(self, i), &DIRECT_ACCESS(self, r - 1)) < 0);\
				do {\
					j--;\
				} while (i < j && comp(&DIRECT_ACCESS(self, r - 1), &DIRECT_ACCESS(self, j)) < 0);\
				if (i >= j) {\
					break;\
				}\
				alias1 = &DIRECT_ACCESS(self, i);\
				alias2 = &DIRECT_ACCESS(self, j);\
				CSTL_ALGORITHM_SWAP(alias1, alias2, tmp);\
			}\
			alias1 = &DIRECT_ACCESS(self, i);\
			CSTL_ALGORITHM_SWAP(alias1, alias3, tmp);\
			/* stack push */\
			if (i - l < r - i) {\
				low[sp] = i + 1;\
				high[sp] = r;\
				sp++;\
				low[sp] = l;\
				high[sp] = i - 1;\
				sp++;\
			} else {\
				low[sp] = l;\
				high[sp] = i - 1;\
				sp++;\
				low[sp] = i + 1;\
				high[sp] = r;\
				sp++;\
			}\
		}\
	}\
}\
\
static size_t Name##_gcd(size_t m, size_t n)\
{\
	register size_t i;\
	while (n) {\
		i = m % n;\
		m = n;\
		n = i;\
	}\
	return m;\
}\
\
static size_t Name##_rotate_aux(Name *self, size_t first, size_t middle, size_t last)\
{\
	register size_t i, j;\
	size_t n, k, l;\
	size_t result;\
	size_t p;\
	size_t d;\
	Type tmp;\
	n = last - first;\
	k = middle - first;\
	l = n - k;\
	result = first + (last - middle);\
	if (k == 0) {\
		return last;\
	} else if (k == l) {\
		for (i = 0; i < k; i++) {\
			Type *alias1 = &DIRECT_ACCESS(self, first + i);\
			Type *alias2 = &DIRECT_ACCESS(self, middle + i);\
			CSTL_ALGORITHM_SWAP(alias1, alias2, tmp);\
		}\
		return result;\
	}\
	/* 最大公約数 */\
	d = Name##_gcd(n, k);\
	for (i = 0; i < d; i++) {\
		tmp = DIRECT_ACCESS(self, first);\
		p = first;\
		if (k < l) {\
			size_t t = l / d;\
			for (j = 0; j < t; j++) {\
				if (p > first + l) {\
					DIRECT_ACCESS(self, p) = DIRECT_ACCESS(self, p - l);\
					p -= l;\
				}\
				DIRECT_ACCESS(self, p) = DIRECT_ACCESS(self, p + k);\
				p += k;\
			}\
		} else {\
			size_t t = k / d - 1;\
			for (j = 0; j < t; j++) {\
				if (p < last - k) {\
					DIRECT_ACCESS(self, p) = DIRECT_ACCESS(self, p + k);\
					p += k;\
				}\
				DIRECT_ACCESS(self, p) = DIRECT_ACCESS(self, p - l);\
				p -= l;\
			}\
		}\
		DIRECT_ACCESS(self, p) = tmp;\
		first++;\
	}\
	return result;\
}\
\
static void Name##_merge_without_buffer(Name *self, size_t first, size_t middle, size_t last, \
							size_t len1, size_t len2, int (*comp)(const void *, const void *))\
{\
	size_t len11 = 0;\
	size_t len22 = 0;\
	size_t first_cut;\
	size_t second_cut;\
	size_t new_middle;\
	Type tmp;\
	if (len1 == 0 || len2 == 0) {\
		return;\
	}\
	if (len1 + len2 == 2) {\
		Type *alias1;\
		Type *alias2;\
		alias1 = &DIRECT_ACCESS(self, first);\
		alias2 = &DIRECT_ACCESS(self, middle);\
		if (comp(alias1, alias2) > 0) {\
			CSTL_ALGORITHM_SWAP(alias1, alias2, tmp);\
		}\
		return;\
	}\
	first_cut = first;\
	second_cut = middle;\
	if (len1 > len2) {\
		len11 = len1 / 2;\
		first_cut += len11;\
		second_cut = Name##_lower_bound(self, middle, last - middle, DIRECT_ACCESS(self, first_cut), comp);\
		len22 = second_cut - middle;\
	} else {\
		len22 = len2 / 2;\
		second_cut += len22;\
		first_cut = Name##_upper_bound(self, first, middle - first, DIRECT_ACCESS(self, second_cut), comp);\
		len11 = first_cut - first;\
	}\
	new_middle = Name##_rotate_aux(self, first_cut, middle, second_cut);\
	Name##_merge_without_buffer(self, first, first_cut, new_middle, len11, len22, comp);\
	Name##_merge_without_buffer(self, new_middle, second_cut, last, len1 - len11, len2 - len22, comp);\
}\
\
static void Name##_merge_with_buffer(Name *self, size_t first, size_t middle, size_t last, \
							Type *buf, int (*comp)(const void *, const void *))\
{\
	register size_t i, j, k;\
	for (i = first; i < middle; i++) {\
		buf[i - first] = DIRECT_ACCESS(self, i);\
	}\
	for (i = middle, j = last - 1; i < last; i++, j--) {\
		buf[i - first] = DIRECT_ACCESS(self, j);\
	}\
	i = first;\
	j = last - 1;\
	for (k = first; k < last; k++) {\
		if (i < middle && comp(&buf[i - first], &buf[j - first]) <= 0) {\
			DIRECT_ACCESS(self, k) = buf[i - first];\
			i++;\
		} else {\
			DIRECT_ACCESS(self, k) = buf[j - first];\
			j--;\
		}\
	}\
}\
\
static void Name##_merge_sort(Name *self, size_t first, size_t last, Type *buf, int (*comp)(const void *, const void *))\
{\
	size_t middle;\
	if (last - first <= 1) {\
		return;\
	}\
	if (last - first < CSTL_ALGORITHM_SWITCH_INSERTION_SORT) {\
		Name##_insertion_sort(self, first, last - first, comp);\
		return;\
	}\
	middle = (first + last) / 2;\
	Name##_merge_sort(self, first, middle, buf, comp);\
	Name##_merge_sort(self, middle, last, buf, comp);\
	/* merge */\
	if (buf) {\
		Name##_merge_with_buffer(self, first, middle, last, buf, comp);\
	} else {\
		Name##_merge_without_buffer(self, first, middle, last, middle - first, last - middle, comp);\
	}\
}\
\
void Name##_stable_sort(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *))\
{\
	Type *buf;\
	CSTL_ASSERT(self && "stable_sort");\
	CSTL_ASSERT(self->magic == self && "stable_sort");\
	CSTL_ASSERT(Name##_size(self) >= idx + n && "stable_sort");\
	CSTL_ASSERT(Name##_size(self) >= n && "stable_sort");\
	CSTL_ASSERT(Name##_size(self) > idx && "stable_sort");\
	CSTL_ASSERT(comp && "stable_sort");\
	if (n < CSTL_ALGORITHM_SWITCH_INSERTION_SORT) {\
		Name##_insertion_sort(self, idx, n, comp);\
		return;\
	}\
	buf = (Type *) malloc(sizeof(Type) * n);\
	Name##_merge_sort(self, idx, idx + n, buf, comp);\
	free(buf);\
}\
\
size_t Name##_lower_bound(Name *self, size_t idx, size_t n, Type value, int (*comp)(const void *, const void *))\
{\
	register size_t first;\
	register size_t last;\
	register size_t middle;\
	CSTL_ASSERT(self && "lower_bound");\
	CSTL_ASSERT(self->magic == self && "lower_bound");\
	CSTL_ASSERT(Name##_size(self) >= idx + n && "lower_bound");\
	CSTL_ASSERT(Name##_size(self) >= n && "lower_bound");\
	CSTL_ASSERT(Name##_size(self) > idx && "lower_bound");\
	CSTL_ASSERT(comp && "lower_bound");\
	first = idx;\
	last = idx + n;\
	while (first < last) {\
		middle = (first + last) / 2;\
		if (comp(&value, &DIRECT_ACCESS(self, middle)) <= 0) {\
			last = middle;\
		} else {\
			first = middle + 1;\
		}\
	}\
	return first;\
}\
\
size_t Name##_upper_bound(Name *self, size_t idx, size_t n, Type value, int (*comp)(const void *, const void *))\
{\
	register size_t first;\
	register size_t last;\
	register size_t middle;\
	CSTL_ASSERT(self && "upper_bound");\
	CSTL_ASSERT(self->magic == self && "upper_bound");\
	CSTL_ASSERT(Name##_size(self) >= idx + n && "upper_bound");\
	CSTL_ASSERT(Name##_size(self) >= n && "upper_bound");\
	CSTL_ASSERT(Name##_size(self) > idx && "upper_bound");\
	CSTL_ASSERT(comp && "upper_bound");\
	first = idx;\
	last = idx + n;\
	while (first < last) {\
		middle = (first + last) / 2;\
		if (comp(&value, &DIRECT_ACCESS(self, middle)) < 0) {\
			last = middle;\
		} else {\
			first = middle + 1;\
		}\
	}\
	return first;\
}\
\
size_t Name##_binary_search(Name *self, size_t idx, size_t n, Type value, int (*comp)(const void *, const void *))\
{\
	size_t i;\
	CSTL_ASSERT(self && "binary_search");\
	CSTL_ASSERT(self->magic == self && "binary_search");\
	CSTL_ASSERT(Name##_size(self) >= idx + n && "binary_search");\
	CSTL_ASSERT(Name##_size(self) >= n && "binary_search");\
	CSTL_ASSERT(Name##_size(self) > idx && "binary_search");\
	CSTL_ASSERT(comp && "binary_search");\
	i = Name##_lower_bound(self, idx, n, value, comp);\
	if (i == idx + n) {\
		return i;\
	} else if (comp(&value, &DIRECT_ACCESS(self, i)) != 0) {\
		return idx + n;\
	} else {\
		return i;\
	}\
}\
\
void Name##_reverse(Name *self, size_t idx, size_t n)\
{\
	register size_t first;\
	register size_t last;\
	Type tmp;\
	CSTL_ASSERT(self && "reverse");\
	CSTL_ASSERT(self->magic == self && "reverse");\
	CSTL_ASSERT(Name##_size(self) >= idx + n && "reverse");\
	CSTL_ASSERT(Name##_size(self) >= n && "reverse");\
	CSTL_ASSERT(Name##_size(self) > idx && "reverse");\
	first = idx;\
	last = idx + n - 1;\
	while (first < last) {\
		Type *alias1 = &DIRECT_ACCESS(self, first);\
		Type *alias2 = &DIRECT_ACCESS(self, last);\
		CSTL_ALGORITHM_SWAP(alias1, alias2, tmp);\
		first++;\
		last--;\
	}\
}\
\
void Name##_rotate(Name *self, size_t first, size_t middle, size_t last)\
{\
	CSTL_ASSERT(self && "rotate");\
	CSTL_ASSERT(self->magic == self && "rotate");\
	CSTL_ASSERT(first <= middle && "rotate");\
	CSTL_ASSERT(middle <= last && "rotate");\
	CSTL_ASSERT(Name##_size(self) >= last && "rotate");\
	Name##_rotate_aux(self, first, middle, last);\
}\
\
int Name##_merge(Name *self, size_t idx, \
		Name *x, size_t xidx, size_t xn, Name *y, size_t yidx, size_t yn, int (*comp)(const void *, const void *))\
{\
	register size_t i, j, k;\
	CSTL_ASSERT(self && "merge");\
	CSTL_ASSERT(self->magic == self && "merge");\
	CSTL_ASSERT(Name##_size(self) >= idx && "merge");\
	CSTL_ASSERT(self != x && "merge");\
	CSTL_ASSERT(self != y && "merge");\
	CSTL_ASSERT(x && "merge");\
	CSTL_ASSERT(x->magic == x && "merge");\
	CSTL_ASSERT(Name##_size(x) >= xidx + xn && "merge");\
	CSTL_ASSERT(Name##_size(x) >= xn && "merge");\
	CSTL_ASSERT(Name##_size(x) > xidx && "merge");\
	CSTL_ASSERT(y && "merge");\
	CSTL_ASSERT(y->magic == y && "merge");\
	CSTL_ASSERT(Name##_size(y) >= yidx + yn && "merge");\
	CSTL_ASSERT(Name##_size(y) >= yn && "merge");\
	CSTL_ASSERT(Name##_size(y) > yidx && "merge");\
	CSTL_ASSERT(comp && "merge");\
	if (!Name##_insert_n_no_data(self, idx, xn + yn)) {\
		return 0;\
	}\
	i = j = k = 0;\
	while (i < xn && j < yn) {\
		Type *alias1;\
		Type *alias2;\
		if (comp(alias1 = &DIRECT_ACCESS(x, xidx + i), alias2 = &DIRECT_ACCESS(y, yidx + j)) <= 0) {\
			DIRECT_ACCESS(self, idx + k) = *alias1;\
			i++;\
		} else {\
			DIRECT_ACCESS(self, idx + k) = *alias2;\
			j++;\
		}\
		k++;\
	}\
	if (i == xn) {\
		while (j < yn) {\
			DIRECT_ACCESS(self, idx + k) = DIRECT_ACCESS(y, yidx + j);\
			j++;\
			k++;\
		}\
	} else {\
		while (i < xn) {\
			DIRECT_ACCESS(self, idx + k) = DIRECT_ACCESS(x, xidx + i);\
			i++;\
			k++;\
		}\
	}\
	return 1;\
}\
\
void Name##_inplace_merge(Name *self, size_t first, size_t middle, size_t last, int (*comp)(const void *, const void *))\
{\
	Type *buf;\
	CSTL_ASSERT(self && "inplace_merge");\
	CSTL_ASSERT(self->magic == self && "inplace_merge");\
	CSTL_ASSERT(first <= middle && "inplace_merge");\
	CSTL_ASSERT(middle <= last && "inplace_merge");\
	CSTL_ASSERT(Name##_size(self) >= last && "inplace_merge");\
	CSTL_ASSERT(comp && "inplace_merge");\
	if (first == middle || middle == last) {\
		return;\
	}\
	buf = (Type *) malloc(sizeof(Type) * (last - first));\
	if (buf) {\
		Name##_merge_with_buffer(self, first, middle, last, buf, comp);\
		free(buf);\
	} else {\
		Name##_merge_without_buffer(self, first, middle, last, middle - first, last - middle, comp);\
	}\
}\
\
static void Name##_up_heap(Name *self, size_t top_idx, size_t hi_idx, int (*comp)(const void *, const void *))\
{\
	/* hi_の付く変数は1から始まるヒープのインデックスを示す */\
	register size_t hi_i;\
	Type tmp;\
	Type *alias;\
	hi_i = hi_idx;\
	tmp = DIRECT_ACCESS(self, hi_i + top_idx - 1);\
	while (hi_i > 1 && comp(alias = &DIRECT_ACCESS(self, hi_i / 2 + top_idx - 1), &tmp) < 0) {\
		DIRECT_ACCESS(self, hi_i + top_idx - 1) = *alias;\
		hi_i = hi_i / 2;\
	}\
	DIRECT_ACCESS(self, hi_i + top_idx - 1) = tmp;\
}\
\
static void Name##_down_heap(Name *self, size_t top_idx, size_t hi_from, size_t hi_to, int (*comp)(const void *, const void *))\
{\
	/* hi_の付く変数は1から始まるヒープのインデックスを示す */\
	register size_t hi_i, hi_j;\
	Type tmp;\
	Type *alias;\
	hi_j = hi_from;\
	tmp = DIRECT_ACCESS(self, hi_j + top_idx - 1);\
	while (2 * hi_j <= hi_to) {\
		hi_i = 2 * hi_j;\
		if (hi_i < hi_to && comp(&DIRECT_ACCESS(self, hi_i + top_idx - 1), &DIRECT_ACCESS(self, hi_i + top_idx)) < 0) {\
			/* 右の子が存在し、左より右の子が大きい */\
			hi_i++;\
		}\
		if (comp(&tmp, alias = &DIRECT_ACCESS(self, hi_i + top_idx - 1)) < 0) {\
			DIRECT_ACCESS(self, hi_j + top_idx - 1) = *alias;\
			hi_j = hi_i;\
		} else {\
			break;\
		}\
	}\
	DIRECT_ACCESS(self, hi_j + top_idx - 1) = tmp;\
}\
\
void Name##_push_heap(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *))\
{\
	CSTL_ASSERT(self && "push_heap");\
	CSTL_ASSERT(self->magic == self && "push_heap");\
	CSTL_ASSERT(Name##_size(self) >= idx + n && "push_heap");\
	CSTL_ASSERT(Name##_size(self) >= n && "push_heap");\
	CSTL_ASSERT(Name##_size(self) > idx && "push_heap");\
	CSTL_ASSERT(comp && "push_heap");\
	Name##_up_heap(self, idx, n, comp);\
}\
\
void Name##_pop_heap(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *))\
{\
	Type tmp;\
	Type *alias1;\
	Type *alias2;\
	CSTL_ASSERT(self && "pop_heap");\
	CSTL_ASSERT(self->magic == self && "pop_heap");\
	CSTL_ASSERT(Name##_size(self) >= idx + n && "pop_heap");\
	CSTL_ASSERT(Name##_size(self) >= n && "pop_heap");\
	CSTL_ASSERT(Name##_size(self) > idx && "pop_heap");\
	CSTL_ASSERT(comp && "pop_heap");\
	CSTL_ASSERT(n > 0 && "pop_heap");\
	alias1 = &DIRECT_ACCESS(self, idx);\
	alias2 = &DIRECT_ACCESS(self, idx + n - 1);\
	CSTL_ALGORITHM_SWAP(alias1, alias2, tmp);\
	Name##_down_heap(self, idx, 1, n - 1, comp);\
}\
\
void Name##_make_heap(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *))\
{\
	register size_t i;\
	CSTL_ASSERT(self && "make_heap");\
	CSTL_ASSERT(self->magic == self && "make_heap");\
	CSTL_ASSERT(Name##_size(self) >= idx + n && "make_heap");\
	CSTL_ASSERT(Name##_size(self) >= n && "make_heap");\
	CSTL_ASSERT(Name##_size(self) > idx && "make_heap");\
	CSTL_ASSERT(comp && "make_heap");\
	for (i = n / 2; i > 0; i--) {\
		Name##_down_heap(self, idx, i, n, comp);\
	}\
}\
\
void Name##_sort_heap(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *))\
{\
	register size_t i;\
	Type tmp;\
	CSTL_ASSERT(self && "sort_heap");\
	CSTL_ASSERT(self->magic == self && "sort_heap");\
	CSTL_ASSERT(Name##_size(self) >= idx + n && "sort_heap");\
	CSTL_ASSERT(Name##_size(self) >= n && "sort_heap");\
	CSTL_ASSERT(Name##_size(self) > idx && "sort_heap");\
	CSTL_ASSERT(comp && "sort_heap");\
	for (i = n; i > 1; i--) {\
		Type *alias1 = &DIRECT_ACCESS(self, idx);\
		Type *alias2 = &DIRECT_ACCESS(self, idx + i - 1);\
		CSTL_ALGORITHM_SWAP(alias1, alias2, tmp);\
		Name##_down_heap(self, idx, 1, i - 1, comp);\
	}\
}\
\
void Name##_partial_sort(Name *self, size_t idx, size_t sort_n, size_t n, int (*comp)(const void *, const void *))\
{\
	register size_t i;\
	Type tmp;\
	CSTL_ASSERT(self && "partial_sort");\
	CSTL_ASSERT(self->magic == self && "partial_sort");\
	CSTL_ASSERT(Name##_size(self) >= idx + n && "partial_sort");\
	CSTL_ASSERT(Name##_size(self) >= n && "partial_sort");\
	CSTL_ASSERT(Name##_size(self) > idx && "partial_sort");\
	CSTL_ASSERT(sort_n <= n && "partial_sort");\
	CSTL_ASSERT(comp && "partial_sort");\
	Name##_make_heap(self, idx, sort_n, comp);\
	for (i = idx + sort_n; i < idx + n; i++) {\
		if (comp(&DIRECT_ACCESS(self, idx), &DIRECT_ACCESS(self, i)) > 0) {\
			Type *alias1;\
			Type *alias2;\
			Name##_pop_heap(self, idx, sort_n, comp);\
			alias1 = &DIRECT_ACCESS(self, idx + sort_n - 1);\
			alias2 = &DIRECT_ACCESS(self, i);\
			CSTL_ALGORITHM_SWAP(alias1, alias2, tmp);\
			Name##_push_heap(self, idx, sort_n, comp);\
		}\
	}\
	Name##_sort_heap(self, idx, sort_n, comp);\
}\
\


#endif /* CSTL_ALGORITHM_H_INCLUDED */
