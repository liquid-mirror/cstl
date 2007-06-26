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
 * \file algorithm.h
 * \brief �A���S���Y��
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2007-06-20
 */
#ifndef CSTL_ALGORITHM_H_INCLUDED
#define CSTL_ALGORITHM_H_INCLUDED


#define CSTL_ALGORITHM_SWAP(i, j, tmp, DIRECT_ACCESS)	\
	tmp = DIRECT_ACCESS(self, i);\
	DIRECT_ACCESS(self, i) = DIRECT_ACCESS(self, j);\
	DIRECT_ACCESS(self, j) = tmp;\


#define CSTL_ALGORITHM_SORT(Name, Type, DIRECT_ACCESS)	\
static void Name##_insertion_sort(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *))\
{\
	register size_t i, j;\
	Type tmp;\
	for (i = 1; i < n; i++) {\
		for (j = i; j >= 1 && comp(&DIRECT_ACCESS(self, idx + j - 1), &DIRECT_ACCESS(self, idx + j)) > 0; j--) {\
			CSTL_ALGORITHM_SWAP(idx + j, idx + j - 1, tmp, DIRECT_ACCESS);\
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
	assert(self && "sort");\
	assert(self->magic == self && "sort");\
	assert(Name##_size(self) >= idx + n && "sort");\
	assert(Name##_size(self) >= n && "sort");\
	assert(Name##_size(self) > idx && "sort");\
	low[0] = idx;\
	high[0] = idx + n - 1;\
	sp = 1;\
	while (sp > 0) {\
		/* stack pop */\
		sp--;\
		l = low[sp];\
		r = high[sp];\
		if (l < r) {\
			if (r - l < 7) {\
				Name##_insertion_sort(self, l, r - l + 1, comp);\
				continue;\
			}\
			/* med3 */\
			middle = (l + r) / 2;\
			if (comp(&DIRECT_ACCESS(self, l), &DIRECT_ACCESS(self, middle)) > 0) {\
				CSTL_ALGORITHM_SWAP(l, middle, tmp, DIRECT_ACCESS);\
			}\
			if (comp(&DIRECT_ACCESS(self, l), &DIRECT_ACCESS(self, r)) > 0) {\
				CSTL_ALGORITHM_SWAP(l, r, tmp, DIRECT_ACCESS);\
			}\
			if (comp(&DIRECT_ACCESS(self, middle), &DIRECT_ACCESS(self, r)) > 0) {\
				CSTL_ALGORITHM_SWAP(middle, r, tmp, DIRECT_ACCESS);\
			}\
			CSTL_ALGORITHM_SWAP(middle, r - 1, tmp, DIRECT_ACCESS); /* r - 1�̗v�f���𐕎��ɂ��� */\
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
				CSTL_ALGORITHM_SWAP(i, j, tmp, DIRECT_ACCESS);\
			}\
			CSTL_ALGORITHM_SWAP(i, r - 1, tmp, DIRECT_ACCESS);\
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
	size_t i;\
	while (n) {\
		i = m % n;\
		m = n;\
		n = i;\
	}\
	return m;\
}\
\
size_t Name##_rotate(Name *self, size_t first, size_t middle, size_t last)\
{\
	size_t n, k, l;\
	size_t result;\
	size_t i, j;\
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
			CSTL_ALGORITHM_SWAP(first + i, middle + i, tmp, DIRECT_ACCESS);\
		}\
		return result;\
	}\
	/* �ő���� */\
	d = Name##_gcd(n, k);\
	for (i = 0; i < d; i++) {\
		tmp = DIRECT_ACCESS(self, first);\
		p = first;\
		if (k < l) {\
			for (j = 0; j < l / d; j++) {\
				if (p > first + l) {\
					DIRECT_ACCESS(self, p) = DIRECT_ACCESS(self, p - l);\
					p -= l;\
				}\
				DIRECT_ACCESS(self, p) = DIRECT_ACCESS(self, p + k);\
				p += k;\
			}\
		} else {\
			for (j = 0; j < k / d - 1; j++) {\
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
		if (comp(&DIRECT_ACCESS(self, first), &DIRECT_ACCESS(self, middle)) > 0) {\
			CSTL_ALGORITHM_SWAP(first, middle, tmp, DIRECT_ACCESS);\
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
	new_middle = Name##_rotate(self, first_cut, middle, second_cut);\
	Name##_merge_without_buffer(self, first, first_cut, new_middle, len11, len22, comp);\
	Name##_merge_without_buffer(self, new_middle, second_cut, last, len1 - len11, len2 - len22, comp);\
}\
\
static void Name##_merge_sort(Name *self, size_t idx, Type *buf, size_t low, size_t high, int (*comp)(const void *, const void *))\
{\
	size_t i, j, k;\
	size_t middle;\
	if (low >= high) {\
		return;\
	}\
	if (high - low < 8) {\
		Name##_insertion_sort(self, low, high - low + 1, comp);\
		return;\
	}\
	middle = (low + high) / 2;\
	Name##_merge_sort(self, idx, buf, low, middle, comp);\
	Name##_merge_sort(self, idx, buf, middle + 1, high, comp);\
	/* merge */\
	if (buf) {\
		/* with buffer */\
		for (i = low; i <= middle; i++) {\
			buf[i - idx] = DIRECT_ACCESS(self, i);\
		}\
		for (i = middle + 1, j = high; i <= high; i++, j--) {\
			buf[i - idx] = DIRECT_ACCESS(self, j);\
		}\
		i = low;\
		j = high;\
		for (k = low; k <= high; k++) {\
			if (comp(&buf[i - idx], &buf[j - idx]) <= 0) {\
				DIRECT_ACCESS(self, k) = buf[i - idx];\
				i++;\
			} else {\
				DIRECT_ACCESS(self, k) = buf[j - idx];\
				j--;\
			}\
		}\
	} else {\
		/* without buffer */\
		Name##_merge_without_buffer(self, low, middle + 1, high + 1, middle + 1 - low, high - middle, comp);\
	}\
}\
\
void Name##_stable_sort(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *))\
{\
	Type *buf;\
	assert(self && "stable_sort");\
	assert(self->magic == self && "stable_sort");\
	assert(Name##_size(self) >= idx + n && "stable_sort");\
	assert(Name##_size(self) >= n && "stable_sort");\
	assert(Name##_size(self) > idx && "stable_sort");\
	if (n < 9) {\
		Name##_insertion_sort(self, idx, n, comp);\
		return;\
	}\
	buf = (Type *) malloc(sizeof(Type) * n);\
	Name##_merge_sort(self, idx, buf, idx, idx + n - 1, comp);\
	free(buf);\
}\
\
size_t Name##_lower_bound(Name *self, size_t idx, size_t n, Type value, int (*comp)(const void *, const void *))\
{\
	size_t first;\
	size_t last;\
	size_t middle;\
	assert(self && "lower_bound");\
	assert(self->magic == self && "lower_bound");\
	assert(Name##_size(self) >= idx + n && "lower_bound");\
	assert(Name##_size(self) >= n && "lower_bound");\
	assert(Name##_size(self) > idx && "lower_bound");\
	first = idx;\
	last = idx + n - 1;\
	while (first < last) {\
		middle = (first + last) / 2;\
		if (comp(&value, &DIRECT_ACCESS(self, middle)) <= 0) {\
			last = middle;\
		} else {\
			first = middle + 1;\
		}\
	}\
	if (first == idx + n - 1 && comp(&value, &DIRECT_ACCESS(self, first)) > 0) {\
		first++;\
	}\
	return first;\
}\
\
size_t Name##_upper_bound(Name *self, size_t idx, size_t n, Type value, int (*comp)(const void *, const void *))\
{\
	size_t first;\
	size_t last;\
	size_t middle;\
	assert(self && "upper_bound");\
	assert(self->magic == self && "upper_bound");\
	assert(Name##_size(self) >= idx + n && "upper_bound");\
	assert(Name##_size(self) >= n && "upper_bound");\
	assert(Name##_size(self) > idx && "upper_bound");\
	first = idx;\
	last = idx + n - 1;\
	while (first < last) {\
		middle = (first + last) / 2;\
		if (comp(&value, &DIRECT_ACCESS(self, middle)) < 0) {\
			last = middle;\
		} else {\
			first = middle + 1;\
		}\
	}\
	if (first == idx + n - 1 && comp(&value, &DIRECT_ACCESS(self, first)) >= 0) {\
		first++;\
	}\
	return first;\
}\
\
size_t Name##_binary_search(Name *self, size_t idx, size_t n, Type value, int (*comp)(const void *, const void *))\
{\
	size_t i;\
	assert(self && "binary_search");\
	assert(self->magic == self && "binary_search");\
	assert(Name##_size(self) >= idx + n && "binary_search");\
	assert(Name##_size(self) >= n && "binary_search");\
	assert(Name##_size(self) > idx && "binary_search");\
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
	size_t first;\
	size_t last;\
	Type tmp;\
	assert(self && "reverse");\
	assert(self->magic == self && "reverse");\
	assert(Name##_size(self) >= idx + n && "reverse");\
	assert(Name##_size(self) >= n && "reverse");\
	assert(Name##_size(self) > idx && "reverse");\
	first = idx;\
	last = idx + n - 1;\
	while (first < last) {\
		CSTL_ALGORITHM_SWAP(first, last, tmp, DIRECT_ACCESS);\
		first++;\
		last--;\
	}\
}\
\


#endif /* CSTL_ALGORITHM_H_INCLUDED */
