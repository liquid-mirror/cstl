/* 
 * Copyright (c) 2006-2009, KATO Noriaki
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
 * \file string.h
 * \brief 文字列コンテナ
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2007-01-09
 * $URL$
 * $Id$
 */
#ifndef CSTL_STRING_H_INCLUDED
#define CSTL_STRING_H_INCLUDED

#include <stdlib.h>
#include <assert.h>
#include "vector.h"

#ifdef __cplusplus
#define CSTL_STRING_BEGIN_EXTERN_C()	extern "C" {
#define CSTL_STRING_END_EXTERN_C()		}
#else
#define CSTL_STRING_BEGIN_EXTERN_C()
#define CSTL_STRING_END_EXTERN_C()
#endif

#ifndef NDEBUG
#define CSTL_STRING_MAGIC(x) x
#else
#define CSTL_STRING_MAGIC(x)
#endif

#ifndef CSTL_ALGORITHM_INTERFACE
#define CSTL_ALGORITHM_INTERFACE(Name, Type)
#endif

#ifndef CSTL_ALGORITHM_IMPLEMENT
#define CSTL_ALGORITHM_IMPLEMENT(Name, Type, DIRECT_ACCESS)
#endif

/*! 
 * \brief 検索失敗用インデックス
 */
#define CSTL_NPOS	((size_t)-1)

#define CSTL_STRING_AT(self, idx)	CSTL_VECTOR_AT((self)->data, (idx))
#define CSTL_STRING_DEFAULT_CAPACITY	(32 - 1)


/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_STRING_INTERFACE(Name, Type)	\
typedef struct Name Name;\
\
CSTL_STRING_BEGIN_EXTERN_C()\
Name *Name##_new(void);\
Name *Name##_new_reserve(size_t n);\
Name *Name##_new_assign(const Type *cstr);\
Name *Name##_new_assign_len(const Type *chars, size_t chars_len);\
Name *Name##_new_assign_c(size_t n, Type c);\
void Name##_delete(Name *self);\
void Name##_clear(Name *self);\
size_t Name##_size(Name *self);\
size_t Name##_length(Name *self);\
size_t Name##_capacity(Name *self);\
int Name##_reserve(Name *self, size_t n);\
void Name##_shrink(Name *self, size_t n);\
int Name##_empty(Name *self);\
int Name##_compare(Name *self, Name *x);\
Type *Name##_at(Name *self, size_t idx);\
const Type *Name##_c_str(Name *self);\
const Type *Name##_data(Name *self);\
Name *Name##_erase(Name *self, size_t idx, size_t len);\
void Name##_pop_back(Name *self);\
int Name##_resize(Name *self, size_t n, Type c);\
void Name##_swap(Name *self, Name *x);\
Name *Name##_assign(Name *self, const Type *cstr);\
Name *Name##_assign_len(Name *self, const Type *chars, size_t chars_len);\
Name *Name##_assign_c(Name *self, size_t n, Type c);\
Name *Name##_append(Name *self, const Type *cstr);\
Name *Name##_append_len(Name *self, const Type *chars, size_t chars_len);\
Name *Name##_append_c(Name *self, size_t n, Type c);\
Name *Name##_push_back(Name *self, Type c);\
Name *Name##_insert(Name *self, size_t idx, const Type *cstr);\
Name *Name##_insert_len(Name *self, size_t idx, const Type *chars, size_t chars_len);\
Name *Name##_insert_c(Name *self, size_t idx, size_t n, Type c);\
Name *Name##_replace(Name *self, size_t idx, size_t len, const Type *cstr);\
Name *Name##_replace_len(Name *self, size_t idx, size_t len, const Type *chars, size_t chars_len);\
Name *Name##_replace_c(Name *self, size_t idx, size_t len, size_t n, Type c);\
size_t Name##_find(Name *self, const Type *cstr, size_t idx);\
size_t Name##_find_len(Name *self, const Type *chars, size_t idx, size_t chars_len);\
size_t Name##_find_c(Name *self, Type c, size_t idx);\
size_t Name##_rfind(Name *self, const Type *cstr, size_t idx);\
size_t Name##_rfind_len(Name *self, const Type *chars, size_t idx, size_t chars_len);\
size_t Name##_rfind_c(Name *self, Type c, size_t idx);\
size_t Name##_find_first_of(Name *self, const Type *cstr, size_t idx);\
size_t Name##_find_first_of_len(Name *self, const Type *chars, size_t idx, size_t chars_len);\
size_t Name##_find_first_of_c(Name *self, Type c, size_t idx);\
size_t Name##_find_last_of(Name *self, const Type *cstr, size_t idx);\
size_t Name##_find_last_of_len(Name *self, const Type *chars, size_t idx, size_t chars_len);\
size_t Name##_find_last_of_c(Name *self, Type c, size_t idx);\
size_t Name##_find_first_not_of(Name *self, const Type *cstr, size_t idx);\
size_t Name##_find_first_not_of_len(Name *self, const Type *chars, size_t idx, size_t chars_len);\
size_t Name##_find_first_not_of_c(Name *self, Type c, size_t idx);\
size_t Name##_find_last_not_of(Name *self, const Type *cstr, size_t idx);\
size_t Name##_find_last_not_of_len(Name *self, const Type *chars, size_t idx, size_t chars_len);\
size_t Name##_find_last_not_of_c(Name *self, Type c, size_t idx);\
CSTL_ALGORITHM_INTERFACE(Name, Type)\
CSTL_STRING_END_EXTERN_C()\


/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_STRING_IMPLEMENT(Name, Type)	\
\
static int Name##_mymemcmp(const Type *x, const Type *y, size_t size)\
{\
	if (size) {\
		do {\
			if (*x != *y) {\
				return *x - *y;\
			}\
			x++;\
			y++;\
			size--;\
		} while (size);\
	}\
	return 0;\
}\
\
static size_t Name##_mystrlen(const Type *cstr)\
{\
	register size_t i = 0;\
	while (*cstr != '\0') {\
		cstr++;\
		i++;\
	}\
	return i;\
}\
\
CSTL_VECTOR_INTERFACE(Name##_CharVector, Type)\
CSTL_VECTOR_IMPLEMENT_BASE(Name##_CharVector, Type)\
CSTL_VECTOR_IMPLEMENT_PUSH_BACK(Name##_CharVector, Type)\
CSTL_VECTOR_IMPLEMENT_RESERVE(Name##_CharVector, Type)\
CSTL_VECTOR_IMPLEMENT_SHRINK(Name##_CharVector, Type)\
CSTL_VECTOR_IMPLEMENT_RESIZE(Name##_CharVector, Type)\
CSTL_VECTOR_IMPLEMENT_MOVE_FORWARD(Name##_CharVector, Type)\
CSTL_VECTOR_IMPLEMENT_MOVE_BACKWARD(Name##_CharVector, Type)\
CSTL_VECTOR_IMPLEMENT_INSERT_N_NO_DATA(Name##_CharVector, Type)\
CSTL_VECTOR_IMPLEMENT_INSERT_ARRAY(Name##_CharVector, Type)\
CSTL_VECTOR_IMPLEMENT_ERASE(Name##_CharVector, Type)\
\
/*! \
 * \brief string構造体\
 */\
struct Name {\
	Name##_CharVector *data;\
	CSTL_STRING_MAGIC(Name *magic;)\
};\
\
static int Name##_expand(Name *self, size_t n)\
{\
	return Name##_CharVector_expand(self->data, n + 1);\
}\
\
Name *Name##_new(void)\
{\
	return Name##_new_reserve(CSTL_STRING_DEFAULT_CAPACITY);\
}\
\
Name *Name##_new_reserve(size_t n)\
{\
	Name *self;\
	self = (Name *) malloc(sizeof(Name));\
	if (!self) return 0;\
	self->data = Name##_CharVector_new_reserve(n + 1);\
	if (!self->data) {\
		free(self);\
		return 0;\
	}\
	Name##_CharVector_push_back(self->data, '\0');\
	CSTL_STRING_MAGIC(self->magic = self);\
	return self;\
}\
\
Name *Name##_new_assign(const Type *cstr)\
{\
	assert(cstr && "String_new_assign");\
	return Name##_new_assign_len(cstr, CSTL_NPOS);\
}\
\
Name *Name##_new_assign_len(const Type *chars, size_t chars_len)\
{\
	Name *self;\
	assert(chars && "String_new_assign_len");\
	if (chars_len == CSTL_NPOS) {\
		chars_len = Name##_mystrlen(chars);\
	}\
	self = Name##_new_reserve(chars_len);\
	if (!self) return 0;\
	return Name##_assign_len(self, chars, chars_len);\
}\
\
Name *Name##_new_assign_c(size_t n, Type c)\
{\
	Name *self;\
	self = Name##_new_reserve(n);\
	if (!self) return 0;\
	return Name##_assign_c(self, n, c);\
}\
\
void Name##_delete(Name *self)\
{\
	if (!self) return;\
	assert(self->magic == self && "String_delete");\
	CSTL_STRING_MAGIC(self->magic = 0);\
	Name##_CharVector_delete(self->data);\
	free(self);\
}\
\
void Name##_clear(Name *self)\
{\
	assert(self && "String_clear");\
	assert(self->magic == self && "String_clear");\
	CSTL_VECTOR_CLEAR(self->data);\
	Name##_CharVector_push_back(self->data, '\0');\
}\
\
size_t Name##_size(Name *self)\
{\
	assert(self && "String_size");\
	assert(self->magic == self && "String_size");\
	return CSTL_VECTOR_SIZE(self->data) - 1;\
}\
\
size_t Name##_length(Name *self)\
{\
	assert(self && "String_length");\
	assert(self->magic == self && "String_length");\
	return CSTL_VECTOR_SIZE(self->data) - 1;\
}\
\
size_t Name##_capacity(Name *self)\
{\
	assert(self && "String_capacity");\
	assert(self->magic == self && "String_capacity");\
	return CSTL_VECTOR_CAPACITY(self->data) - 1;\
}\
\
int Name##_reserve(Name *self, size_t n)\
{\
	assert(self && "String_reserve");\
	assert(self->magic == self && "String_reserve");\
	return Name##_CharVector_reserve(self->data, n + 1);\
}\
\
void Name##_shrink(Name *self, size_t n)\
{\
	assert(self && "String_shrink");\
	assert(self->magic == self && "String_shrink");\
	Name##_CharVector_shrink(self->data, n + 1);\
}\
\
int Name##_empty(Name *self)\
{\
	assert(self && "String_empty");\
	assert(self->magic == self && "String_empty");\
	return CSTL_VECTOR_SIZE(self->data) == 1;\
}\
\
int Name##_compare(Name *self, Name *x)\
{\
	size_t len, xlen;\
	int ret;\
	assert(self && "String_compare");\
	assert(x && "String_compare");\
	assert(self->magic == self && "String_compare");\
	assert(x->magic == x && "String_compare");\
	len = Name##_size(self);\
	xlen = Name##_size(x);\
	ret = Name##_mymemcmp(Name##_c_str(self), Name##_c_str(x), (len > xlen) ? xlen : len);\
	if (ret) {\
		return ret;\
	} else if (len == xlen) {\
		return 0;\
	} else if (len > xlen) {\
		return 1;\
	} else {\
		return -1;\
	}\
}\
\
Type *Name##_at(Name *self, size_t idx)\
{\
	assert(self && "String_at");\
	assert(self->magic == self && "String_at");\
	assert(Name##_size(self) > idx && "String_at");\
	return &CSTL_VECTOR_AT(self->data, idx);\
}\
\
const Type *Name##_c_str(Name *self)\
{\
	assert(self && "String_c_str");\
	assert(self->magic == self && "String_c_str");\
	return &CSTL_VECTOR_AT(self->data, 0);\
}\
\
const Type *Name##_data(Name *self)\
{\
	assert(self && "String_data");\
	assert(self->magic == self && "String_data");\
	return &CSTL_VECTOR_AT(self->data, 0);\
}\
\
Name *Name##_erase(Name *self, size_t idx, size_t len)\
{\
	size_t size;\
	assert(self && "String_erase");\
	assert(self->magic == self && "String_erase");\
	assert(Name##_size(self) > idx && "String_erase");\
	size = Name##_size(self);\
	if (len > size - idx) {\
		len = size - idx;\
	}\
	Name##_CharVector_erase(self->data, idx, len);\
	return self;\
}\
\
void Name##_pop_back(Name *self)\
{\
	size_t size;\
	assert(self && "String_pop_back");\
	assert(self->magic == self && "String_pop_back");\
	assert(!Name##_empty(self) && "String_pop_back");\
	size = Name##_size(self);\
	Name##_CharVector_erase(self->data, size - 1, 1);\
}\
\
int Name##_resize(Name *self, size_t n, Type c)\
{\
	size_t num;\
	assert(self && "String_resize");\
	assert(self->magic == self && "String_resize");\
	num = CSTL_VECTOR_SIZE(self->data) - 1;\
	if (!Name##_CharVector_resize(self->data, n + 1, c)) {\
		return 0;\
	}\
	if (num < CSTL_VECTOR_SIZE(self->data)) {\
		/* '\0'を上書き */\
		CSTL_VECTOR_AT(self->data, num) = c;\
	}\
	CSTL_VECTOR_AT(self->data, CSTL_VECTOR_SIZE(self->data) - 1) = '\0';\
	return 1;\
}\
\
void Name##_swap(Name *self, Name *x)\
{\
	Name##_CharVector *tmp_data;\
	assert(self && "String_swap");\
	assert(x && "String_swap");\
	assert(self->magic == self && "String_swap");\
	assert(x->magic == x && "String_swap");\
	tmp_data = self->data;\
	self->data = x->data;\
	x->data = tmp_data;\
}\
\
Name *Name##_assign(Name *self, const Type *cstr)\
{\
	assert(self && "String_assign");\
	assert(self->magic == self && "String_assign");\
	assert(cstr && "String_assign");\
	return Name##_assign_len(self, cstr, CSTL_NPOS);\
}\
\
Name *Name##_assign_len(Name *self, const Type *chars, size_t chars_len)\
{\
	assert(self && "String_assign_len");\
	assert(self->magic == self && "String_assign_len");\
	assert(chars && "String_assign_len");\
	if (chars_len == CSTL_NPOS) {\
		chars_len = Name##_mystrlen(chars);\
	}\
	/* NOTE: charsがself内の文字列の場合、chars_lenはString_size()より小さいはずなので\
	 * Vector_expand()は許容量拡張せず、内部バッファのアドレスが変わることはない。 */\
	if (!Name##_CharVector_expand(self->data, chars_len + 1)) {\
		return 0;\
	}\
	CSTL_VECTOR_CLEAR(self->data);\
	Name##_CharVector_insert_array(self->data, 0, chars, chars_len);\
	Name##_CharVector_push_back(self->data, '\0');\
	return self;\
}\
\
Name *Name##_assign_c(Name *self, size_t n, Type c)\
{\
	assert(self && "String_assign_c");\
	assert(self->magic == self && "String_assign_c");\
	if (!Name##_expand(self, n)) {\
		return 0;\
	}\
	Name##_clear(self);\
	return Name##_insert_c(self, 0, n, c);\
}\
\
Name *Name##_append(Name *self, const Type *cstr)\
{\
	assert(self && "String_append");\
	assert(self->magic == self && "String_append");\
	assert(cstr && "String_append");\
	return Name##_append_len(self, cstr, CSTL_NPOS);\
}\
\
Name *Name##_append_len(Name *self, const Type *chars, size_t chars_len)\
{\
	assert(self && "String_append_len");\
	assert(self->magic == self && "String_append_len");\
	assert(chars && "String_append_len");\
	if (chars_len == CSTL_NPOS) {\
		chars_len = Name##_mystrlen(chars);\
	}\
	return Name##_insert_len(self, Name##_size(self), chars, chars_len);\
}\
\
Name *Name##_append_c(Name *self, size_t n, Type c)\
{\
	assert(self && "String_append_c");\
	assert(self->magic == self && "String_append_c");\
	return Name##_insert_c(self, Name##_size(self), n, c);\
}\
\
Name *Name##_push_back(Name *self, Type c)\
{\
	assert(self && "String_push_back");\
	assert(self->magic == self && "String_push_back");\
	return Name##_insert_c(self, Name##_size(self), 1, c);\
}\
\
static int Name##_insert_n_no_data(Name *self, size_t idx, size_t n)\
{\
	return Name##_CharVector_insert_n_no_data(self->data, idx, n);\
}\
\
Name *Name##_insert(Name *self, size_t idx, const Type *cstr)\
{\
	assert(self && "String_insert");\
	assert(self->magic == self && "String_insert");\
	assert(Name##_size(self) >= idx && "String_insert");\
	assert(cstr && "String_insert");\
	return Name##_insert_len(self, idx, cstr, CSTL_NPOS);\
}\
\
Name *Name##_insert_len(Name *self, size_t idx, const Type *chars, size_t chars_len)\
{\
	register size_t i;\
	size_t size;\
	assert(self && "String_insert_len");\
	assert(self->magic == self && "String_insert_len");\
	assert(chars && "String_insert_len");\
	size = Name##_size(self);\
	assert(size >= idx && "String_insert_len");\
	if (chars_len == CSTL_NPOS) {\
		chars_len = Name##_mystrlen(chars);\
	}\
	if (Name##_c_str(self) <= chars && chars < Name##_c_str(self) + size) {\
		if (Name##_capacity(self) < size + chars_len) {\
			/* NOTE: charsがself内の文字列の場合、許容量拡張で内部バッファのアドレスが変わって\
			 * charsが読めなくなる可能性があるためtmpにコピーして置き換える。 */\
			Type *tmp = (Type *) malloc(sizeof(Type) * chars_len);\
			if (!tmp) return 0;\
			for (i = 0; i < chars_len; i++) {\
				tmp[i] = chars[i];\
			}\
			chars = tmp;\
			if (!Name##_insert_n_no_data(self, idx, chars_len)) {\
				free(tmp);\
				return 0;\
			}\
			for (i = 0; i < chars_len; i++) {\
				CSTL_STRING_AT(self, idx + i) = chars[i];\
			}\
			free(tmp);\
		} else {\
			/* charsがself内の文字列だが、許容量拡張はされない */\
			/* insert_n_no_data()は必ず真を返す */\
			Name##_insert_n_no_data(self, idx, chars_len);\
			if (&CSTL_STRING_AT(self, idx) <= chars) {\
				for (i = 0; i < chars_len; i++) {\
					CSTL_STRING_AT(self, idx + i) = chars[chars_len + i];\
				}\
			} else if (chars < &CSTL_STRING_AT(self, idx) && &CSTL_STRING_AT(self, idx) < chars + chars_len) {\
				size_t k = &CSTL_STRING_AT(self, idx) - chars;\
				for (i = 0; i < k; i++) {\
					CSTL_STRING_AT(self, idx + i) = chars[i];\
				}\
				for (i = 0; i < chars_len - k; i++) {\
					CSTL_STRING_AT(self, idx + k + i) = CSTL_STRING_AT(self, idx + chars_len + i);\
				}\
			} else {\
				for (i = 0; i < chars_len; i++) {\
					CSTL_STRING_AT(self, idx + i) = chars[i];\
				}\
			}\
		}\
	} else {\
		if (!Name##_insert_n_no_data(self, idx, chars_len)) {\
			return 0;\
		}\
		for (i = 0; i < chars_len; i++) {\
			CSTL_STRING_AT(self, idx + i) = chars[i];\
		}\
	}\
	return self;\
}\
\
Name *Name##_insert_c(Name *self, size_t idx, size_t n, Type c)\
{\
	register size_t i;\
	assert(self && "String_insert_c");\
	assert(self->magic == self && "String_insert_c");\
	assert(Name##_size(self) >= idx && "String_insert_c");\
	if (!Name##_insert_n_no_data(self, idx, n)) {\
		return 0;\
	}\
	for (i = 0; i < n; i++) {\
		CSTL_STRING_AT(self, idx + i) = c;\
	}\
	return self;\
}\
\
Name *Name##_replace(Name *self, size_t idx, size_t len, const Type *cstr)\
{\
	assert(self && "String_replace");\
	assert(self->magic == self && "String_replace");\
	assert(cstr && "String_replace");\
	assert(Name##_size(self) >= idx && "String_replace");\
	return Name##_replace_len(self, idx, len, cstr, CSTL_NPOS);\
}\
\
Name *Name##_replace_len(Name *self, size_t idx, size_t len, const Type *chars, size_t chars_len)\
{\
	register size_t i;\
	size_t size;\
	assert(self && "String_replace_len");\
	assert(self->magic == self && "String_replace_len");\
	assert(chars && "String_replace_len");\
	size = Name##_size(self);\
	assert(size >= idx && "String_replace_len");\
	if (len > size - idx) {\
		len = size - idx;\
	}\
	if (chars_len == CSTL_NPOS) {\
		chars_len = Name##_mystrlen(chars);\
	}\
	if (Name##_c_str(self) <= chars && chars < Name##_c_str(self) + size) {\
		/* charsがself内の文字列 */\
		if (chars_len <= len) {\
			/* 拡張必要なし */\
			if (&CSTL_STRING_AT(self, idx) <= chars) {\
				for (i = 0; i < chars_len; i++) {\
					CSTL_STRING_AT(self, idx + i) = chars[i];\
				}\
			} else {\
				for (i = chars_len; i > 0; i--) {\
					CSTL_STRING_AT(self, idx + i - 1) = chars[i - 1];\
				}\
			}\
			if (chars_len != len) {\
				Name##_erase(self, idx + chars_len, len - chars_len);\
			}\
		} else {\
			if (Name##_capacity(self) < size + (chars_len - len)) {\
				/* 拡張必要あり */\
				/* NOTE: charsがself内の文字列の場合、許容量拡張で内部バッファのアドレスが変わって\
				 * charsが読めなくなる可能性があるためtmpにコピーして置き換える。 */\
				Type *tmp = (Type *) malloc(sizeof(Type) * chars_len);\
				if (!tmp) return 0;\
				for (i = 0; i < chars_len; i++) {\
					tmp[i] = chars[i];\
				}\
				chars = tmp;\
				if (!Name##_expand(self, size + (chars_len - len))) {\
					free(tmp);\
					return 0;\
				}\
				for (i = 0; i < len; i++) {\
					CSTL_STRING_AT(self, idx + i) = chars[i];\
				}\
				Name##_insert_len(self, idx + len, &chars[len], chars_len - len);\
				free(tmp);\
			} else {\
				/* charsがself内の文字列だが、許容量拡張はされない */\
				if (&CSTL_STRING_AT(self, idx) <= chars) {\
					for (i = 0; i < len; i++) {\
						CSTL_STRING_AT(self, idx + i) = chars[i];\
					}\
					Name##_insert_len(self, idx + len, &chars[len], chars_len - len);\
				} else {\
					Name##_insert_len(self, idx + len, &chars[len], chars_len - len);\
					for (i = len; i > 0; i--) {\
						CSTL_STRING_AT(self, idx + i - 1) = chars[i - 1];\
					}\
				}\
			}\
		}\
	} else {\
		if (chars_len <= len) {\
			/* 拡張必要なし */\
			for (i = 0; i < chars_len; i++) {\
				CSTL_STRING_AT(self, idx + i) = chars[i];\
			}\
			if (chars_len != len) {\
				Name##_erase(self, idx + chars_len, len - chars_len);\
			}\
		} else {\
			/* 拡張必要あり */\
			if (!Name##_expand(self, Name##_size(self) + (chars_len - len))) {\
				return 0;\
			}\
			for (i = 0; i < len; i++) {\
				CSTL_STRING_AT(self, idx + i) = chars[i];\
			}\
			Name##_insert_len(self, idx + len, &chars[len], chars_len - len);\
		}\
	}\
	return self;\
}\
\
Name *Name##_replace_c(Name *self, size_t idx, size_t len, size_t n, Type c)\
{\
	register size_t i;\
	size_t size;\
	assert(self && "String_replace_c");\
	assert(self->magic == self && "String_replace_c");\
	assert(Name##_size(self) >= idx && "String_replace_c");\
	size = Name##_size(self);\
	if (len > size - idx) {\
		len = size - idx;\
	}\
	if (n <= len) {\
		/* 拡張必要なし */\
		for (i = 0; i < n; i++) {\
			CSTL_STRING_AT(self, idx + i) = c;\
		}\
		if (n != len) {\
			Name##_erase(self, idx + n, len - n);\
		}\
	} else {\
		/* 拡張必要あり */\
		if (!Name##_insert_n_no_data(self, idx, n - len)) {\
			return 0;\
		}\
		for (i = 0; i < n; i++) {\
			CSTL_STRING_AT(self, idx + i) = c;\
		}\
	}\
	return self;\
}\
\
static size_t Name##_brute_force_search(const Type *str, size_t str_len, const Type *ptn, size_t ptn_len)\
{\
	register size_t i = 0;\
	register size_t j = 0;\
	if (str_len < ptn_len) return CSTL_NPOS;\
	while (i < str_len && j < ptn_len) {\
		if (str[i] == ptn[j]) {\
			i++;\
			j++;\
		} else {\
			i += 1 - j;\
			j = 0;\
		}\
	}\
	return (j == ptn_len) ? i - j : CSTL_NPOS;\
}\
\
static size_t Name##_brute_force_search_r(const Type *str, size_t str_len, const Type *ptn, size_t ptn_len)\
{\
	register size_t i = str_len;\
	register size_t j = ptn_len;\
	if (str_len < ptn_len) return CSTL_NPOS;\
	while (i > 0 && j > 0) {\
		if (str[i - 1] == ptn[j - 1]) {\
			i--;\
			j--;\
		} else {\
			i += ptn_len - j - 1;\
			j = ptn_len;\
		}\
	}\
	return (j == 0) ? i : CSTL_NPOS;\
}\
\
size_t Name##_find(Name *self, const Type *cstr, size_t idx)\
{\
	assert(self && "String_find");\
	assert(self->magic == self && "String_find");\
	assert(cstr && "String_find");\
	return Name##_find_len(self, cstr, idx, CSTL_NPOS);\
}\
\
size_t Name##_find_len(Name *self, const Type *chars, size_t idx, size_t chars_len)\
{\
	size_t i;\
	size_t size;\
	assert(self && "String_find_len");\
	assert(self->magic == self && "String_find_len");\
	assert(chars && "String_find_len");\
	if (chars_len == CSTL_NPOS) {\
		chars_len = Name##_mystrlen(chars);\
	}\
	if (chars_len == 0) {\
		return idx;\
	}\
	size = Name##_size(self);\
	if (size <= idx) return CSTL_NPOS;\
	i = Name##_brute_force_search(&Name##_c_str(self)[idx], size - idx, chars, chars_len);\
	if (i == CSTL_NPOS) return i;\
	return i + idx;\
}\
\
size_t Name##_find_c(Name *self, Type c, size_t idx)\
{\
	assert(self && "String_find_c");\
	assert(self->magic == self && "String_find_c");\
	return Name##_find_len(self, &c, idx, 1);\
}\
\
size_t Name##_rfind(Name *self, const Type *cstr, size_t idx)\
{\
	assert(self && "String_rfind");\
	assert(self->magic == self && "String_rfind");\
	assert(cstr && "String_rfind");\
	return Name##_rfind_len(self, cstr, idx, CSTL_NPOS);\
}\
\
size_t Name##_rfind_len(Name *self, const Type *chars, size_t idx, size_t chars_len)\
{\
	size_t size;\
	assert(self && "String_rfind_len");\
	assert(self->magic == self && "String_rfind_len");\
	assert(chars && "String_rfind_len");\
	if (chars_len == CSTL_NPOS) {\
		chars_len = Name##_mystrlen(chars);\
	}\
	size = Name##_size(self);\
	if (size < chars_len) {\
		return CSTL_NPOS;\
	}\
	if (size - chars_len < idx) {\
		idx = size - chars_len;\
	}\
	return Name##_brute_force_search_r(Name##_c_str(self), idx + chars_len, chars, chars_len);\
}\
\
size_t Name##_rfind_c(Name *self, Type c, size_t idx)\
{\
	assert(self && "String_rfind_c");\
	assert(self->magic == self && "String_rfind_c");\
	return Name##_rfind_len(self, &c, idx, 1);\
}\
\
size_t Name##_find_first_of(Name *self, const Type *cstr, size_t idx)\
{\
	assert(self && "String_find_first_of");\
	assert(self->magic == self && "String_find_first_of");\
	assert(cstr && "String_find_first_of");\
	return Name##_find_first_of_len(self, cstr, idx, CSTL_NPOS);\
}\
\
size_t Name##_find_first_of_len(Name *self, const Type *chars, size_t idx, size_t chars_len)\
{\
	register size_t i, j;\
	size_t size;\
	const Type *p;\
	assert(self && "String_find_first_of_len");\
	assert(self->magic == self && "String_find_first_of_len");\
	assert(chars && "String_find_first_of_len");\
	size = Name##_size(self);\
	if (size <= idx) return CSTL_NPOS;\
	if (chars_len == CSTL_NPOS) {\
		chars_len = Name##_mystrlen(chars);\
	}\
	p = Name##_c_str(self);\
	for (i = idx; i < size; i++) {\
		for (j = 0; j < chars_len; j++) {\
			if (p[i] == chars[j]) {\
				return i;\
			}\
		}\
	}\
	return CSTL_NPOS;\
}\
\
size_t Name##_find_first_of_c(Name *self, Type c, size_t idx)\
{\
	assert(self && "String_find_first_of_c");\
	assert(self->magic == self && "String_find_first_of_c");\
	return Name##_find_first_of_len(self, &c, idx, 1);\
}\
\
size_t Name##_find_last_of(Name *self, const Type *cstr, size_t idx)\
{\
	assert(self && "String_find_last_of");\
	assert(self->magic == self && "String_find_last_of");\
	assert(cstr && "String_find_last_of");\
	return Name##_find_last_of_len(self, cstr, idx, CSTL_NPOS);\
}\
\
size_t Name##_find_last_of_len(Name *self, const Type *chars, size_t idx, size_t chars_len)\
{\
	register size_t i, j;\
	size_t size;\
	const Type *p;\
	assert(self && "String_find_last_of_len");\
	assert(self->magic == self && "String_find_last_of_len");\
	assert(chars && "String_find_last_of_len");\
	size = Name##_size(self);\
	if (!size) {\
		return CSTL_NPOS;\
	}\
	if (size <= idx) {\
		idx = size - 1;\
	}\
	if (chars_len == CSTL_NPOS) {\
		chars_len = Name##_mystrlen(chars);\
	}\
	p = Name##_c_str(self);\
	for (i = idx + 1; i > 0; i--) {\
		for (j = 0; j < chars_len; j++) {\
			if (p[i - 1] == chars[j]) {\
				return i - 1;\
			}\
		}\
	}\
	return CSTL_NPOS;\
}\
\
size_t Name##_find_last_of_c(Name *self, Type c, size_t idx)\
{\
	assert(self && "String_find_last_of_c");\
	assert(self->magic == self && "String_find_last_of_c");\
	return Name##_find_last_of_len(self, &c, idx, 1);\
}\
\
size_t Name##_find_first_not_of(Name *self, const Type *cstr, size_t idx)\
{\
	assert(self && "String_find_first_not_of");\
	assert(self->magic == self && "String_find_first_not_of");\
	assert(cstr && "String_find_first_not_of");\
	return Name##_find_first_not_of_len(self, cstr, idx, CSTL_NPOS);\
}\
\
size_t Name##_find_first_not_of_len(Name *self, const Type *chars, size_t idx, size_t chars_len)\
{\
	register size_t i, j;\
	size_t size;\
	const Type *p;\
	assert(self && "String_find_first_not_of_len");\
	assert(self->magic == self && "String_find_first_not_of_len");\
	assert(chars && "String_find_first_not_of_len");\
	size = Name##_size(self);\
	if (size <= idx) return CSTL_NPOS;\
	if (chars_len == CSTL_NPOS) {\
		chars_len = Name##_mystrlen(chars);\
	}\
	if (chars_len == 0) {\
		return idx;\
	}\
	p = Name##_c_str(self);\
	for (i = idx; i < size; i++) {\
		for (j = 0; j < chars_len; j++) {\
			if (p[i] != chars[j]) {\
				if (j == chars_len - 1) return i;\
			} else {\
				break;\
			}\
		}\
	}\
	return CSTL_NPOS;\
}\
\
size_t Name##_find_first_not_of_c(Name *self, Type c, size_t idx)\
{\
	assert(self && "String_find_first_not_of_c");\
	assert(self->magic == self && "String_find_first_not_of_c");\
	return Name##_find_first_not_of_len(self, &c, idx, 1);\
}\
\
size_t Name##_find_last_not_of(Name *self, const Type *cstr, size_t idx)\
{\
	assert(self && "String_find_last_not_of");\
	assert(self->magic == self && "String_find_last_not_of");\
	assert(cstr && "String_find_last_not_of");\
	return Name##_find_last_not_of_len(self, cstr, idx, CSTL_NPOS);\
}\
\
size_t Name##_find_last_not_of_len(Name *self, const Type *chars, size_t idx, size_t chars_len)\
{\
	register size_t i, j;\
	size_t size;\
	const Type *p;\
	assert(self && "String_find_last_not_of_len");\
	assert(self->magic == self && "String_find_last_not_of_len");\
	assert(chars && "String_find_last_not_of_len");\
	size = Name##_size(self);\
	if (!size) {\
		return CSTL_NPOS;\
	}\
	if (size <= idx) {\
		idx = size - 1;\
	}\
	if (chars_len == CSTL_NPOS) {\
		chars_len = Name##_mystrlen(chars);\
	}\
	if (chars_len == 0) {\
		return idx;\
	}\
	p = Name##_c_str(self);\
	for (i = idx + 1; i > 0; i--) {\
		for (j = 0; j < chars_len; j++) {\
			if (p[i - 1] != chars[j]) {\
				if (j == chars_len - 1) return i - 1;\
			} else {\
				break;\
			}\
		}\
	}\
	return CSTL_NPOS;\
}\
\
size_t Name##_find_last_not_of_c(Name *self, Type c, size_t idx)\
{\
	assert(self && "String_find_last_not_of_c");\
	assert(self->magic == self && "String_find_last_not_of_c");\
	return Name##_find_last_not_of_len(self, &c, idx, 1);\
}\
\
CSTL_ALGORITHM_IMPLEMENT(Name, Type, CSTL_STRING_AT)\


#endif /* CSTL_STRING_H_INCLUDED */
