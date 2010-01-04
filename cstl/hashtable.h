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
 * \file hashtable.h
 * \brief ハッシュテーブル
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2009-05-21
 * $URL$
 * $Id$
 *
 * このファイルを直接インクルードしないこと
 */
#ifndef CSTL_HASHTABLE_H_INCLUDED
#define CSTL_HASHTABLE_H_INCLUDED

#include <stdlib.h>
#include "common.h"
#include "vector.h"


#define CSTL_EQUAL_TO(x, y)		((x) == (y) ? 0 : 1)


#define CSTL_HASHTABLE_INTERFACE(Name, KeyType, ValueType)	\
\
typedef struct Name Name;\
typedef struct Name##Node *Name##Iterator;\
typedef struct Name##Node *Name##LocalIterator;\
size_t Name##_hash_string(register const char *str);\
size_t Name##_hash_wstring(register const wchar_t *str);\
size_t Name##_hash_char(char n);\
size_t Name##_hash_uchar(unsigned char n);\
size_t Name##_hash_short(short n);\
size_t Name##_hash_ushort(unsigned short n);\
size_t Name##_hash_int(int n);\
size_t Name##_hash_uint(unsigned int n);\
size_t Name##_hash_long(long n);\
size_t Name##_hash_ulong(unsigned long n);\
Name *Name##_new(void);\
Name *Name##_new_reserve(size_t n);\
void Name##_delete(Name *self);\
void Name##_clear(Name *self);\
int Name##_empty(Name *self);\
size_t Name##_size(Name *self);\
int Name##_insert_range(Name *self, Name##Iterator first, Name##Iterator last);\
Name##Iterator Name##_erase(Name *self, Name##Iterator pos);\
Name##Iterator Name##_erase_range(Name *self, Name##Iterator first, Name##Iterator last);\
size_t Name##_erase_key(Name *self, KeyType key);\
size_t Name##_count(Name *self, KeyType key);\
Name##Iterator Name##_find(Name *self, KeyType key);\
void Name##_equal_range(Name *self, KeyType key, Name##Iterator *first, Name##Iterator *last);\
Name##Iterator Name##_begin(Name *self);\
Name##Iterator Name##_end(Name *self);\
Name##Iterator Name##_next(Name##Iterator pos);\
void Name##_swap(Name *self, Name *x);\
size_t Name##_bucket_count(Name *self);\
size_t Name##_bucket_size(Name *self, size_t idx);\
size_t Name##_bucket(Name *self, KeyType key);\
Name##LocalIterator Name##_bucket_begin(Name *self, size_t idx);\
Name##LocalIterator Name##_bucket_end(Name *self, size_t idx);\
Name##LocalIterator Name##_bucket_next(Name##LocalIterator pos);\
float Name##_load_factor(Name *self);\
float Name##_get_max_load_factor(Name *self);\
void Name##_set_max_load_factor(Name *self, float z);\
int Name##_rehash(Name *self, size_t n);\
\


#define CSTL_HASHTABLE_IMPLEMENT(Name, KeyType, ValueType, Hasher, Compare)	\
\
size_t Name##_hash_string(register const char *str)\
{\
	register int c;\
	register int val = 0;\
	while ((c = *str++) != '\0') {\
		val = val * 997 + c;\
	}\
	return val + (val >> 5);\
}\
\
size_t Name##_hash_wstring(register const wchar_t *str)\
{\
	register int c;\
	register int val = 0;\
	while ((c = *str++) != '\0') {\
		val = val * 997 + c;\
	}\
	return val + (val >> 5);\
}\
\
size_t Name##_hash_char(char n)\
{\
    return n;\
}\
\
size_t Name##_hash_uchar(unsigned char n)\
{\
    return n;\
}\
\
size_t Name##_hash_short(short n)\
{\
    return n;\
}\
\
size_t Name##_hash_ushort(unsigned short n)\
{\
    return n;\
}\
\
size_t Name##_hash_int(int n)\
{\
    return n;\
}\
\
size_t Name##_hash_uint(unsigned int n)\
{\
    return n;\
}\
\
size_t Name##_hash_long(long n)\
{\
    return n;\
}\
\
size_t Name##_hash_ulong(unsigned long n)\
{\
    return n;\
}\
\
static Name##Node *Name##Node_insert(Name##Node *list, Name##Node *node, Name##Node **bucket)\
{\
	node->bucket = bucket;\
	if (!list) {\
		return node;\
	}\
	node->next = list;\
	return node;\
}\
\
static Name##Node *Name##Node_erase(Name##Node *list)\
{\
	Name##Node *tmp;\
	if (!list) {\
		return list;\
	}\
	tmp = list->next;\
	CSTL_MAGIC(list->magic = 0);\
	free(list);\
	return tmp;\
}\
\
static Name##Node *Name##Node_clear(Name##Node *list)\
{\
	register Name##Node *pos;\
	for (pos = list; pos != 0;) {\
		pos = Name##Node_erase(pos);\
	}\
	return pos;\
}\
\
static size_t Name##Node_size(Name##Node *list)\
{\
	register size_t count = 0;\
	register Name##Node *pos;\
	for (pos = list; pos != 0; pos = pos->next) {\
		count++;\
	}\
	return count;\
}\
\
CSTL_VECTOR_INTERFACE(Name##Node_Vector, Name##Node *)\
CSTL_VECTOR_IMPLEMENT_BASE(Name##Node_Vector, Name##Node *)\
CSTL_VECTOR_IMPLEMENT_RESERVE(Name##Node_Vector, Name##Node *)\
CSTL_VECTOR_IMPLEMENT_MOVE_BACKWARD(Name##Node_Vector, Name##Node *)\
CSTL_VECTOR_IMPLEMENT_INSERT_N_NO_DATA(Name##Node_Vector, Name##Node *)\
CSTL_VECTOR_IMPLEMENT_SIZE(Name##Node_Vector, Name##Node *)\
CSTL_VECTOR_IMPLEMENT_RESIZE(Name##Node_Vector, Name##Node *)\
CSTL_VECTOR_IMPLEMENT_AT(Name##Node_Vector, Name##Node *)\
CSTL_VECTOR_IMPLEMENT_BACK(Name##Node_Vector, Name##Node *)\
CSTL_VECTOR_IMPLEMENT_SWAP(Name##Node_Vector, Name##Node *)\
\
/*\
 * Table of prime numbers 2^n+a, 2<=n<=30.\
 */\
static const size_t Name##_primes[] = {\
	8 + 3,\
	16 + 3,\
	32 + 5,\
	64 + 3,\
	128 + 3,\
	256 + 27,\
	512 + 9,\
	1024 + 9,\
	2048 + 5,\
	4096 + 3,\
	8192 + 27,\
	16384 + 43,\
	32768 + 3,\
	65536 + 45,\
	131072 + 29,\
	262144 + 3,\
	524288 + 21,\
	1048576 + 7,\
	2097152 + 17,\
	4194304 + 15,\
	8388608 + 9,\
	16777216 + 43,\
	33554432 + 35,\
	67108864 + 15,\
	134217728 + 29,\
	268435456 + 3,\
	536870912 + 11,\
	1073741824 + 85,\
};\
static const size_t Name##_primes_size = sizeof Name##_primes / sizeof Name##_primes[0];\
\
static const float Name##_minimum_mlf = 1e-3f;\
static const float Name##_default_mlf = 1.0f;\
\
/*! \
 * \brief unordered_set/unordered_map構造体\
 */\
struct Name {\
	Name##Node_Vector *buckets;\
	size_t size;\
	float max_load_factor;\
	Name##Node end_node;\
	CSTL_MAGIC(Name *magic;)\
};\
\
static size_t Name##_next_prime(size_t n)\
{\
	register size_t i;\
	for (i = 0; i < Name##_primes_size; i++) {\
		if (Name##_primes[i] >= n) {\
			return Name##_primes[i];\
		}\
	}\
	return Name##_primes[Name##_primes_size - 1];\
}\
\
Name *Name##_new(void)\
{\
	return Name##_new_reserve(0);\
}\
\
Name *Name##_new_reserve(size_t n)\
{\
	size_t nbuckets;\
	Name *self;\
	self = (Name *) malloc(sizeof(Name));\
	if (!self) return 0;\
	nbuckets = Name##_next_prime(n);\
	self->buckets = Name##Node_Vector_new_reserve(nbuckets + 1); /* +1はend()の分 */\
	if (!self->buckets) {\
		free(self);\
		return 0;\
	}\
	Name##Node_Vector_resize(self->buckets, nbuckets + 1, 0);\
	self->end_node.next = 0;\
	self->end_node.bucket = 0; /* end()判定に使用 */\
	*Name##Node_Vector_back(self->buckets) = &self->end_node; /* end()の値 */\
	CSTL_MAGIC(self->end_node.magic = self->buckets);\
	self->size = 0;\
	self->max_load_factor = Name##_default_mlf;\
	CSTL_MAGIC(self->magic = self);\
	return self;\
}\
\
void Name##_delete(Name *self)\
{\
	if (!self) return;\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_delete");\
	Name##_clear(self);\
	Name##Node_Vector_delete(self->buckets);\
	CSTL_MAGIC(self->magic = 0);\
	free(self);\
}\
\
void Name##_clear(Name *self)\
{\
	register size_t i;\
	size_t bc;\
	CSTL_ASSERT(self && "Unordered(Set|Map)_clear");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_clear");\
	if (self->size == 0) {\
		return;\
	}\
	bc = Name##_bucket_count(self);\
	for (i = 0; i < bc; i++) {\
		Name##Node **alias;\
		alias = Name##Node_Vector_at(self->buckets, i);\
		*alias = Name##Node_clear(*alias);\
	}\
	self->size = 0;\
}\
\
int Name##_empty(Name *self)\
{\
	CSTL_ASSERT(self && "Unordered(Set|Map)_empty");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_empty");\
	return self->size == 0;\
}\
\
size_t Name##_size(Name *self)\
{\
	CSTL_ASSERT(self && "Unordered(Set|Map)_size");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_size");\
	return self->size;\
}\
\
Name##Iterator Name##_begin(Name *self)\
{\
	register size_t i;\
	CSTL_ASSERT(self && "Unordered(Set|Map)_begin");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_begin");\
	if (self->size == 0) {\
		return Name##_end(self);\
	}\
	for (i = 0;; i++) {\
		Name##Node **alias;\
		alias = Name##Node_Vector_at(self->buckets, i);\
		if (*alias) {\
			return *alias;\
		}\
	}\
	/* NOTREACHED */\
	CSTL_ASSERT(0 && "Unordered(Set|Map)_begin");\
	return 0;\
}\
\
Name##Iterator Name##_end(Name *self)\
{\
	CSTL_ASSERT(self && "Unordered(Set|Map)_end");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_end");\
	return &self->end_node;\
}\
\
Name##Iterator Name##_next(Name##Iterator pos)\
{\
	register size_t i;\
	CSTL_ASSERT(pos && "Unordered(Set|Map)_next");\
	CSTL_ASSERT(pos->magic && "Unordered(Set|Map)_next");\
	CSTL_ASSERT(pos->bucket && "Unordered(Set|Map)_next"); /* pos != end() */\
	if (pos->next) {\
		return pos->next;\
	}\
	for (i = 1;; i++) {\
		if (pos->bucket[i]) {\
			return pos->bucket[i];\
		}\
	}\
	/* NOTREACHED */\
	CSTL_ASSERT(0 && "Unordered(Set|Map)_next");\
	return 0;\
}\
\
static Name##Iterator Name##_find_node(Name *self, KeyType key, size_t idx)\
{\
	Name##Node **alias;\
	register Name##Node *pos;\
	alias = Name##Node_Vector_at(self->buckets, idx);\
	for (pos = *alias; pos != 0; pos = pos->next) {\
		if (Compare(key, pos->key) == 0) {\
			return pos;\
		}\
	}\
	return Name##_end(self);\
}\
\
Name##Iterator Name##_find(Name *self, KeyType key)\
{\
	size_t idx;\
	CSTL_ASSERT(self && "Unordered(Set|Map)_find");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_find");\
	idx = Hasher(key) % Name##_bucket_count(self);\
	return Name##_find_node(self, key, idx);\
}\
\
void Name##_equal_range(Name *self, KeyType key, Name##Iterator *first, Name##Iterator *last)\
{\
	register Name##Node *pos;\
	Name##Node *end_pos;\
	CSTL_ASSERT(self && "Unordered(Set|Map)_equal_range");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_equal_range");\
	CSTL_ASSERT(first && "Unordered(Set|Map)_equal_range");\
	CSTL_ASSERT(last && "Unordered(Set|Map)_equal_range");\
	*first = Name##_find(self, key);\
	end_pos = Name##_end(self);\
	if (*first == end_pos) {\
		*last = end_pos;\
		return;\
	}\
	for (pos = Name##_next(*first); pos != end_pos; pos = Name##_next(pos)) {\
		if (Compare(key, pos->key) != 0) {\
			*last = pos;\
			return;\
		}\
	}\
	*last = end_pos;\
}\
\
size_t Name##_count(Name *self, KeyType key)\
{\
	register Name##Node *pos;\
	register size_t count = 0;\
	Name##Node *first;\
	Name##Node *last;\
	CSTL_ASSERT(self && "Unordered(Set|Map)_count");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_count");\
	Name##_equal_range(self, key, &first, &last);\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		count++;\
	}\
	return count;\
}\
\
void Name##_swap(Name *self, Name *x)\
{\
	Name##Node_Vector *tmp_buckets;\
	size_t tmp_size;\
	float tmp_max_load_factor;\
	CSTL_ASSERT(self && "Unordered(Set|Map)_swap");\
	CSTL_ASSERT(x && "Unordered(Set|Map)_swap");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_swap");\
	CSTL_ASSERT(x->magic == x && "Unordered(Set|Map)_swap");\
	tmp_buckets = self->buckets;\
	tmp_size = self->size;\
	tmp_max_load_factor = self->max_load_factor;\
	self->buckets = x->buckets;\
	self->size = x->size;\
	self->max_load_factor = x->max_load_factor;\
	x->buckets = tmp_buckets;\
	x->size = tmp_size;\
	x->max_load_factor = tmp_max_load_factor;\
	CSTL_ASSERT(*Name##Node_Vector_back(self->buckets) == &x->end_node && "Unordered(Set|Map)_swap");\
	CSTL_ASSERT(*Name##Node_Vector_back(x->buckets) == &self->end_node && "Unordered(Set|Map)_swap");\
	*Name##Node_Vector_back(self->buckets) = &self->end_node;\
	*Name##Node_Vector_back(x->buckets) = &x->end_node;\
	CSTL_ASSERT(self->end_node.magic == x->buckets && "Unordered(Set|Map)_swap");\
	CSTL_ASSERT(x->end_node.magic == self->buckets && "Unordered(Set|Map)_swap");\
	CSTL_MAGIC(self->end_node.magic = self->buckets);\
	CSTL_MAGIC(x->end_node.magic = x->buckets);\
}\
\
size_t Name##_bucket_count(Name *self)\
{\
	CSTL_ASSERT(self && "Unordered(Set|Map)_bucket_count");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_bucket_count");\
	return Name##Node_Vector_size(self->buckets) - 1;\
}\
\
size_t Name##_bucket_size(Name *self, size_t idx)\
{\
	CSTL_ASSERT(self && "Unordered(Set|Map)_bucket_size");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_bucket_size");\
	CSTL_ASSERT(idx < Name##_bucket_count(self) && "Unordered(Set|Map)_bucket_size");\
	return Name##Node_size(*Name##Node_Vector_at(self->buckets, idx));\
}\
\
size_t Name##_bucket(Name *self, KeyType key)\
{\
	CSTL_ASSERT(self && "Unordered(Set|Map)_bucket");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_bucket");\
	return Hasher(key) % Name##_bucket_count(self);\
}\
\
Name##LocalIterator Name##_bucket_begin(Name *self, size_t idx)\
{\
	CSTL_ASSERT(self && "Unordered(Set|Map)_bucket_begin");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_bucket_begin");\
	CSTL_ASSERT(idx < Name##_bucket_count(self) && "Unordered(Set|Map)_bucket_begin");\
	return *Name##Node_Vector_at(self->buckets, idx);\
}\
\
Name##LocalIterator Name##_bucket_end(Name *self, size_t idx)\
{\
	CSTL_ASSERT(self && "Unordered(Set|Map)_bucket_end");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_bucket_end");\
	CSTL_ASSERT(idx < Name##_bucket_count(self) && "Unordered(Set|Map)_bucket_end");\
	CSTL_UNUSED_PARAM(self);\
	CSTL_UNUSED_PARAM(idx);\
	return 0;\
}\
\
Name##LocalIterator Name##_bucket_next(Name##LocalIterator pos)\
{\
	CSTL_ASSERT(pos && "Unordered(Set|Map)_bucket_next");\
	CSTL_ASSERT(pos->magic && "Unordered(Set|Map)_bucket_next");\
	CSTL_ASSERT(pos->bucket && "Unordered(Set|Map)_bucket_next"); /* pos != end() */\
	return pos->next;\
}\
\
float Name##_load_factor(Name *self)\
{\
	CSTL_ASSERT(self && "Unordered(Set|Map)_load_factor");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_load_factor");\
	return (float) self->size / (float) Name##_bucket_count(self);\
}\
\
float Name##_get_max_load_factor(Name *self)\
{\
	CSTL_ASSERT(self && "Unordered(Set|Map)_get_max_load_factor");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_get_max_load_factor");\
	return self->max_load_factor;\
}\
\
void Name##_set_max_load_factor(Name *self, float z)\
{\
	CSTL_ASSERT(self && "Unordered(Set|Map)_set_max_load_factor");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_set_max_load_factor");\
	self->max_load_factor = (z < Name##_minimum_mlf) ? Name##_minimum_mlf : z;\
}\
\
Name##Iterator Name##_erase(Name *self, Name##Iterator pos)\
{\
	Name##Node *ret;\
	register Name##Node *i;\
	register Name##Node *prev;\
	CSTL_ASSERT(self && "Unordered(Set|Map)_erase");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_erase");\
	CSTL_ASSERT(pos && "Unordered(Set|Map)_erase");\
	CSTL_ASSERT(pos->magic == self->buckets && "Unordered(Set|Map)_erase");\
	CSTL_ASSERT(pos != Name##_end(self) && "Unordered(Set|Map)_erase");\
	ret = Name##_next(pos);\
	for (i = *pos->bucket, prev = 0; i != pos; prev = i, i = i->next) {\
		;\
	}\
	if (prev) {\
		prev->next = Name##Node_erase(pos);\
	} else {\
		Name##Node **tmp = pos->bucket;\
		*tmp = Name##Node_erase(pos);\
	}\
	self->size--;\
	return ret;\
}\
\
Name##Iterator Name##_erase_range(Name *self, Name##Iterator first, Name##Iterator last)\
{\
	register Name##Iterator pos;\
	CSTL_ASSERT(self && "Unordered(Set|Map)_erase_range");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_erase_range");\
	CSTL_ASSERT(first && "Unordered(Set|Map)_erase_range");\
	CSTL_ASSERT(last && "Unordered(Set|Map)_erase_range");\
	CSTL_ASSERT((first->magic == self->buckets || first == Name##_end(self)) && "Unordered(Set|Map)_erase_range");\
	CSTL_ASSERT((last->magic == self->buckets || last == Name##_end(self)) && "Unordered(Set|Map)_erase_range");\
	pos = first;\
	while (pos != last) {\
		CSTL_ASSERT(!Name##_empty(self) && "Unordered(Set|Map)_erase_range");\
		pos = Name##_erase(self, pos);\
	}\
	return pos;\
}\
\
size_t Name##_erase_key(Name *self, KeyType key)\
{\
	register size_t count = 0;\
	Name##Iterator pos;\
	Name##Iterator last;\
	CSTL_ASSERT(self && "Unordered(Set|Map)_erase_key");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_erase_key");\
	Name##_equal_range(self, key, &pos, &last);\
	while (pos != last) {\
		CSTL_ASSERT(!Name##_empty(self) && "Unordered(Set|(Set|Map))_erase");\
		pos = Name##_erase(self, pos);\
		count++;\
	}\
	return count;\
}\
\


#define CSTL_HASHTABLE_IMPLEMENT_REHASH(Name, KeyType, ValueType, Hasher, Compare)	\
int Name##_rehash(Name *self, size_t n)\
{\
	register Name##Node *node;\
	size_t nbuckets;\
	Name##Node_Vector *new_buckets;\
	Name##Node *end_pos;\
	CSTL_ASSERT(self && "Unordered(Set|Map)_rehash");\
	CSTL_ASSERT(self->magic == self && "Unordered(Set|Map)_rehash");\
	nbuckets = Name##_next_prime(n);\
	if (nbuckets <= Name##_bucket_count(self)) {\
		return 1;\
	}\
	new_buckets = Name##Node_Vector_new_reserve(nbuckets + 1);\
	if (!new_buckets) {\
		return 0;\
	}\
	Name##Node_Vector_resize(new_buckets, nbuckets + 1, 0);\
\
	end_pos = Name##_end(self);\
	/* 各ノードに対して再ハッシュ */\
	for (node = Name##_begin(self); node != end_pos;) {\
		size_t idx;\
		Name##Node **alias;\
		Name##Node *tmp = Name##_next(node);\
		/* nodeをリストから取り外す */\
		*node->bucket = node->next;\
		node->next = 0;\
\
		idx = Hasher(node->key) % nbuckets;\
		alias = Name##Node_Vector_at(new_buckets, idx);\
		*alias = Name##Node_insert(*alias, node, alias);\
		node = tmp;\
	}\
	/* end()を指すポインタ */\
	*Name##Node_Vector_back(new_buckets) = &self->end_node;\
	CSTL_ASSERT(Name##_begin(self) == Name##_end(self) && "Unordered(Set|Map)_rehash");\
\
	Name##Node_Vector_swap(self->buckets, new_buckets);\
	Name##Node_Vector_delete(new_buckets);\
	return 1;\
}\
\


#define CSTL_HASHTABLE_IMPLEMENT_REHASH_MULTI(Name, KeyType, ValueType, Hasher, Compare)	\
int Name##_rehash(Name *self, size_t n)\
{\
	register Name##Node *node;\
	size_t nbuckets;\
	Name##Node_Vector *new_buckets;\
	Name##Node *end_pos;\
	CSTL_ASSERT(self && "UnorderedMulti(Set|Map)_rehash");\
	CSTL_ASSERT(self->magic == self && "UnorderedMulti(Set|Map)_rehash");\
	nbuckets = Name##_next_prime(n);\
	if (nbuckets <= Name##_bucket_count(self)) {\
		return 1;\
	}\
	new_buckets = Name##Node_Vector_new_reserve(nbuckets + 1);\
	if (!new_buckets) {\
		return 0;\
	}\
	Name##Node_Vector_resize(new_buckets, nbuckets + 1, 0);\
\
	end_pos = Name##_end(self);\
	/* 各ノードに対して再ハッシュ */\
	for (node = Name##_begin(self); node != end_pos;) {\
		register Name##Node *pos;\
		register Name##Node *prev;\
		size_t idx;\
		Name##Node **alias;\
		Name##Node *tmp = Name##_next(node);\
		/* nodeをリストから取り外す */\
		*node->bucket = node->next;\
		node->next = 0;\
\
		idx = Hasher(node->key) % nbuckets;\
		alias = Name##Node_Vector_at(new_buckets, idx);\
		/* 同じキーを並べるため、バケットの各要素のキーをチェック */\
		for (pos = *alias, prev = 0; pos != 0; prev = pos, pos = pos->next) {\
			if (Compare(node->key, pos->key) == 0) {\
				pos = Name##Node_insert(pos, node, alias);\
				if (prev) {\
					prev->next = pos;\
				} else {\
					*alias = pos;\
				}\
				goto next;\
			}\
		}\
		*alias = Name##Node_insert(*alias, node, alias);\
next:\
		node = tmp;\
	}\
	/* end()を指すポインタ */\
	*Name##Node_Vector_back(new_buckets) = &self->end_node;\
	CSTL_ASSERT(Name##_begin(self) == Name##_end(self) && "UnorderedMulti(Set|Map)_rehash");\
\
	Name##Node_Vector_swap(self->buckets, new_buckets);\
	Name##Node_Vector_delete(new_buckets);\
	return 1;\
}\
\

#endif /* CSTL_HASHTABLE_H_INCLUDED */
