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
 * \brief ÉAÉãÉSÉäÉYÉÄ
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
	size_t mid;\
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
			mid = (l + r) / 2;\
			if (comp(&DIRECT_ACCESS(self, l), &DIRECT_ACCESS(self, mid)) > 0) {\
				CSTL_ALGORITHM_SWAP(l, mid, tmp, DIRECT_ACCESS);\
			}\
			if (comp(&DIRECT_ACCESS(self, l), &DIRECT_ACCESS(self, r)) > 0) {\
				CSTL_ALGORITHM_SWAP(l, r, tmp, DIRECT_ACCESS);\
			}\
			if (comp(&DIRECT_ACCESS(self, mid), &DIRECT_ACCESS(self, r)) > 0) {\
				CSTL_ALGORITHM_SWAP(mid, r, tmp, DIRECT_ACCESS);\
			}\
			CSTL_ALGORITHM_SWAP(mid, r - 1, tmp, DIRECT_ACCESS); /* r - 1ÇÃóvëfÇÇêïé≤Ç…Ç∑ÇÈ */\
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
static void Name##_merge_sort(Name *self, size_t idx, Type *buf, size_t low, size_t high, int (*comp)(const void *, const void *))\
{\
	size_t i, j, k;\
	size_t mid;\
	if (low >= high) {\
		return;\
	}\
	if (high - low < 8) {\
		Name##_insertion_sort(self, low, high - low + 1, comp);\
		return;\
	}\
	mid = (low + high) / 2;\
	Name##_merge_sort(self, idx, buf, low, mid, comp);\
	Name##_merge_sort(self, idx, buf, mid + 1, high, comp);\
	/* merge */\
	for (i = low; i <= mid; i++) {\
		buf[i - idx] = DIRECT_ACCESS(self, i);\
	}\
	for (i = mid + 1, j = high; i <= high; i++, j--) {\
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
}\
\
int Name##_stable_sort(Name *self, size_t idx, size_t n, int (*comp)(const void *, const void *))\
{\
	Type *buf;\
	assert(self && "stable_sort");\
	assert(self->magic == self && "stable_sort");\
	assert(Name##_size(self) >= idx + n && "stable_sort");\
	assert(Name##_size(self) >= n && "stable_sort");\
	assert(Name##_size(self) > idx && "stable_sort");\
	if (n < 9) {\
		Name##_insertion_sort(self, idx, n, comp);\
		return 1;\
	}\
	buf = (Type *) malloc(sizeof(Type) * n);\
	if (!buf) {\
		return 0;\
	}\
	Name##_merge_sort(self, idx, buf, idx, idx + n - 1, comp);\
	free(buf);\
	return 1;\
}\
\
size_t Name##_lower_bound(Name *self, size_t idx, size_t n, Type value, int (*comp)(const void *, const void *))\
{\
	size_t first;\
	size_t last;\
	size_t mid;\
	first = idx;\
	last = idx + n - 1;\
	while (first < last) {\
		mid = (first + last) / 2;\
		if (comp(&value, &DIRECT_ACCESS(self, mid)) <= 0) {\
			last = mid;\
		} else {\
			first = mid + 1;\
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
	size_t mid;\
	first = idx;\
	last = idx + n - 1;\
	while (first < last) {\
		mid = (first + last) / 2;\
		if (comp(&value, &DIRECT_ACCESS(self, mid)) < 0) {\
			last = mid;\
		} else {\
			first = mid + 1;\
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


#endif /* CSTL_ALGORITHM_H_INCLUDED */
