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
 * \file unordered_set.h
 * \brief unordered_set/unordered_multisetコンテナ
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2009-05-21
 * $URL$
 * $Id$
 */
#ifndef CSTL_UNORDERED_SET_H_INCLUDED
#define CSTL_UNORDERED_SET_H_INCLUDED

#include <stdlib.h>
#include "common.h"
#include "hashtable.h"


#define CSTL_COMMON_UNORDERED_SET_IMPLEMENT(Name, Type, Hasher, Compare)	\
\
typedef struct Name##Node Name##Node;\
/*! \
 * \brief unordered_setノード構造体\
 */\
struct Name##Node {\
	struct Name##Node *next;\
	struct Name##Node **bucket;\
	Type key;\
	CSTL_MAGIC(struct Name##Node_Vector *magic;)\
};\
\
static Name##Node *Name##Node_new(Type data)\
{\
	Name##Node *node;\
	node = (Name##Node *) malloc(sizeof(Name##Node));\
	if (!node) return 0;\
	node->key = data;\
	node->next = 0;\
	return node;\
}\
\
CSTL_HASHTABLE_IMPLEMENT(Name, Type, Type, Hasher, Compare)\
\
Type const *Name##_data(Name##Iterator pos)\
{\
	CSTL_ASSERT(pos && "UnorderedSet_data");\
	CSTL_ASSERT(pos->magic && "UnorderedSet_data");\
	CSTL_ASSERT(pos->bucket && "UnorderedSet_data"); /* pos != end() */\
	return &pos->key;\
}\
\


/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_UNORDERED_SET_INTERFACE(Name, Type)	\
CSTL_EXTERN_C_BEGIN()\
CSTL_HASHTABLE_INTERFACE(Name, Type, Type)\
Name##Iterator Name##_insert(Name *self, Type data, int *success);\
Type const *Name##_data(Name##Iterator pos);\
CSTL_EXTERN_C_END()\

/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 * \param Hasher ハッシュ関数
 * \param Compare 要素の比較ルーチン
 */
#define CSTL_UNORDERED_SET_IMPLEMENT(Name, Type, Hasher, Compare)	\
CSTL_COMMON_UNORDERED_SET_IMPLEMENT(Name, Type, Hasher, Compare)\
CSTL_HASHTABLE_IMPLEMENT_REHASH(Name, Type, Type, Hasher, Compare)\
\
Name##Iterator Name##_insert(Name *self, Type data, int *success)\
{\
	Name##Node **alias;\
	Name##Node *node;\
	Name##Node *pos;\
	size_t hash_val;\
	size_t idx;\
	CSTL_ASSERT(self && "UnorderedSet_insert");\
	CSTL_ASSERT(self->magic == self && "UnorderedSet_insert");\
	hash_val = Hasher(data);\
	idx = hash_val % Name##_bucket_count(self);\
	pos = Name##_find_node(self, data, idx);\
	if (pos != Name##_end(self)) {\
		if (success) *success = 0;\
		return pos;\
	}\
	node = Name##Node_new(data);\
	if (!node) {\
		if (success) *success = 0;\
		return node;\
	}\
	/* rehash */\
	if (self->size + 1 > self->max_load_factor * Name##_bucket_count(self)) {\
		size_t s = (size_t) ((self->size + 1) / self->max_load_factor) + 1;\
		if (!Name##_rehash(self, s)) {\
			Name##Node_erase(node);\
			if (success) *success = 0;\
			return 0;\
		}\
		idx = hash_val % Name##_bucket_count(self);\
	}\
	alias = Name##Node_Vector_at(self->buckets, idx);\
	*alias = Name##Node_insert(*alias, node, alias);\
	self->size++;\
	CSTL_MAGIC(node->magic = self->buckets);\
	if (success) *success = 1;\
	return node;\
}\
\
int Name##_insert_range(Name *self, Name##Iterator first, Name##Iterator last)\
{\
	Name##Node *list = 0;\
	register Name##Node *pos;\
	register size_t count = 0;\
	CSTL_ASSERT(self && "UnorderedSet_insert_range");\
	CSTL_ASSERT(self->magic == self && "UnorderedSet_insert_range");\
	CSTL_ASSERT(first && "UnorderedSet_insert_range");\
	CSTL_ASSERT(last && "UnorderedSet_insert_range");\
	CSTL_ASSERT(first->magic && "UnorderedSet_insert_range");\
	CSTL_ASSERT(last->magic && "UnorderedSet_insert_range");\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		if (Name##_find(self, pos->key) == Name##_end(self)) {\
			Name##Node *node;\
			node = Name##Node_new(pos->key);\
			if (!node) {\
				Name##Node_clear(list);\
				return 0;\
			}\
			list = Name##Node_insert(list, node, 0);\
			count++;\
		}\
	}\
	/* rehash */\
	if (self->size + count > self->max_load_factor * Name##_bucket_count(self)) {\
		size_t s = (size_t) ((self->size + count) / self->max_load_factor) + 1;\
		if (!Name##_rehash(self, s)) {\
			Name##Node_clear(list);\
			return 0;\
		}\
		CSTL_ASSERT(self->size + count <= self->max_load_factor * Name##_bucket_count(self) && "UnorderedSet_insert_range");\
	}\
	for (pos = list; pos != 0; pos = list) {\
		size_t idx;\
		Name##Node **alias;\
		/* posをリストから取り外す */\
		list = pos->next;\
		pos->next = 0;\
\
		idx = Hasher(pos->key) % Name##_bucket_count(self);\
		alias = Name##Node_Vector_at(self->buckets, idx);\
		*alias = Name##Node_insert(*alias, pos, alias);\
		CSTL_MAGIC(pos->magic = self->buckets);\
	}\
	self->size += count;\
	return 1;\
}\
\


/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_UNORDERED_MULTISET_INTERFACE(Name, Type)	\
CSTL_EXTERN_C_BEGIN()\
CSTL_HASHTABLE_INTERFACE(Name, Type, Type)\
Name##Iterator Name##_insert(Name *self, Type data);\
Type const *Name##_data(Name##Iterator pos);\
CSTL_EXTERN_C_END()\

/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 * \param Hasher ハッシュ関数
 * \param Compare 要素の比較ルーチン
 */
#define CSTL_UNORDERED_MULTISET_IMPLEMENT(Name, Type, Hasher, Compare)	\
CSTL_COMMON_UNORDERED_SET_IMPLEMENT(Name, Type, Hasher, Compare)\
CSTL_HASHTABLE_IMPLEMENT_REHASH_MULTI(Name, Type, Type, Hasher, Compare)\
\
Name##Iterator Name##_insert(Name *self, Type data)\
{\
	Name##Node **alias;\
	Name##Node *node;\
	register Name##Node *pos;\
	register Name##Node *prev;\
	size_t hash_val;\
	size_t idx;\
	CSTL_ASSERT(self && "UnorderedMultiSet_insert");\
	CSTL_ASSERT(self->magic == self && "UnorderedMultiSet_insert");\
	hash_val = Hasher(data);\
	idx = hash_val % Name##_bucket_count(self);\
	node = Name##Node_new(data);\
	if (!node) {\
		return node;\
	}\
	/* rehash */\
	if (self->size + 1 > self->max_load_factor * Name##_bucket_count(self)) {\
		size_t s = (size_t) ((self->size + 1) / self->max_load_factor) + 1;\
		if (!Name##_rehash(self, s)) {\
			Name##Node_erase(node);\
			return 0;\
		}\
		idx = hash_val % Name##_bucket_count(self);\
	}\
	alias = Name##Node_Vector_at(self->buckets, idx);\
	/* 同じキーを並べるため、バケットの各要素のキーをチェック */\
	for (pos = *alias, prev = 0; pos != 0; prev = pos, pos = pos->next) {\
		if (Compare(data, pos->key) == 0) {\
			pos = Name##Node_insert(pos, node, alias);\
			if (prev) {\
				prev->next = pos;\
			} else {\
				*alias = pos;\
			}\
			self->size++;\
			CSTL_MAGIC(node->magic = self->buckets);\
			return node;\
		}\
	}\
	*alias = Name##Node_insert(*alias, node, alias);\
	self->size++;\
	CSTL_MAGIC(node->magic = self->buckets);\
	return node;\
}\
\
int Name##_insert_range(Name *self, Name##Iterator first, Name##Iterator last)\
{\
	Name##Node *list = 0;\
	register Name##Node *pos;\
	register size_t count = 0;\
	CSTL_ASSERT(self && "UnorderedMultiSet_insert_range");\
	CSTL_ASSERT(self->magic == self && "UnorderedMultiSet_insert_range");\
	CSTL_ASSERT(first && "UnorderedMultiSet_insert_range");\
	CSTL_ASSERT(last && "UnorderedMultiSet_insert_range");\
	CSTL_ASSERT(first->magic && "UnorderedMultiSet_insert_range");\
	CSTL_ASSERT(last->magic && "UnorderedMultiSet_insert_range");\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		Name##Node *node;\
		node = Name##Node_new(pos->key);\
		if (!node) {\
			Name##Node_clear(list);\
			return 0;\
		}\
		list = Name##Node_insert(list, node, 0);\
		count++;\
	}\
	/* rehash */\
	if (self->size + count > self->max_load_factor * Name##_bucket_count(self)) {\
		size_t s = (size_t) ((self->size + count) / self->max_load_factor) + 1;\
		if (!Name##_rehash(self, s)) {\
			Name##Node_clear(list);\
			return 0;\
		}\
		CSTL_ASSERT(self->size + count <= self->max_load_factor * Name##_bucket_count(self) && "UnorderedMultiSet_insert_range");\
	}\
	for (pos = list; pos != 0; pos = list) {\
		register Name##Node *i;\
		register Name##Node *prev;\
		size_t idx;\
		Name##Node **alias;\
		/* posをリストから取り外す */\
		list = pos->next;\
		pos->next = 0;\
\
		idx = Hasher(pos->key) % Name##_bucket_count(self);\
		alias = Name##Node_Vector_at(self->buckets, idx);\
		/* 同じキーを並べるため、バケットの各要素のキーをチェック */\
		for (i = *alias, prev = 0; i != 0; prev = i, i = i->next) {\
			if (Compare(pos->key, i->key) == 0) {\
				i = Name##Node_insert(i, pos, alias);\
				if (prev) {\
					prev->next = i;\
				} else {\
					*alias = i;\
				}\
				CSTL_MAGIC(pos->magic = self->buckets);\
				goto next;\
			}\
		}\
		*alias = Name##Node_insert(*alias, pos, alias);\
		CSTL_MAGIC(pos->magic = self->buckets);\
next:\
		;\
	}\
	self->size += count;\
	return 1;\
}\
\

#endif /* CSTL_UNORDERED_SET_H_INCLUDED */
