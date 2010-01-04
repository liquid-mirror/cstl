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
 * \file unordered_map.h
 * \brief unordered_map/unordered_multimapコンテナ
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2009-05-21
 * $URL$
 * $Id$
 */
#ifndef CSTL_UNORDERED_MAP_H_INCLUDED
#define CSTL_UNORDERED_MAP_H_INCLUDED

#include <stdlib.h>
#include "common.h"
#include "hashtable.h"


#define CSTL_COMMON_UNORDERED_MAP_IMPLEMENT(Name, KeyType, ValueType, Hasher, Compare)	\
\
typedef struct Name##Node Name##Node;\
/*! \
 * \brief unordered_mapノード構造体\
 */\
struct Name##Node {\
	struct Name##Node *next;\
	struct Name##Node **bucket;\
	KeyType key;\
	ValueType value;\
	CSTL_MAGIC(struct Name##Node_Vector *magic;)\
};\
\
static Name##Node *Name##Node_new(KeyType key, ValueType const *value)\
{\
	Name##Node *node;\
	node = (Name##Node *) malloc(sizeof(Name##Node));\
	if (!node) return 0;\
	node->key = key;\
	node->value = *value;\
	node->next = 0;\
	return node;\
}\
\
CSTL_HASHTABLE_IMPLEMENT(Name, KeyType, ValueType, Hasher, Compare)\
\
KeyType const *Name##_key(Name##Iterator pos)\
{\
	CSTL_ASSERT(pos && "UnorderedMap_key");\
	CSTL_ASSERT(pos->magic && "UnorderedMap_key");\
	CSTL_ASSERT(pos->bucket && "UnorderedMap_key"); /* pos != end() */\
	return &pos->key;\
}\
\
ValueType *Name##_value(Name##Iterator pos)\
{\
	CSTL_ASSERT(pos && "UnorderedMap_value");\
	CSTL_ASSERT(pos->magic && "UnorderedMap_value");\
	CSTL_ASSERT(pos->bucket && "UnorderedMap_value"); /* pos != end() */\
	return &pos->value;\
}\
\


/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param KeyType 要素のキーの型
 * \param ValueType 要素の値の型
 */
#define CSTL_UNORDERED_MAP_INTERFACE(Name, KeyType, ValueType)	\
CSTL_EXTERN_C_BEGIN()\
CSTL_HASHTABLE_INTERFACE(Name, KeyType, ValueType)\
Name##Iterator Name##_insert(Name *self, KeyType key, ValueType value, int *success);\
Name##Iterator Name##_insert_ref(Name *self, KeyType key, ValueType const *value, int *success);\
KeyType const *Name##_key(Name##Iterator pos);\
ValueType *Name##_value(Name##Iterator pos);\
ValueType *Name##_at(Name *self, KeyType key);\
CSTL_EXTERN_C_END()\

/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param KeyType 要素のキーの型
 * \param ValueType 要素の値の型
 * \param Hasher ハッシュ関数
 * \param Compare 要素の比較ルーチン
 */
#define CSTL_UNORDERED_MAP_IMPLEMENT(Name, KeyType, ValueType, Hasher, Compare)	\
CSTL_COMMON_UNORDERED_MAP_IMPLEMENT(Name, KeyType, ValueType, Hasher, Compare)\
CSTL_HASHTABLE_IMPLEMENT_REHASH(Name, KeyType, ValueType, Hasher, Compare)\
\
Name##Iterator Name##_insert(Name *self, KeyType key, ValueType value, int *success)\
{\
	CSTL_ASSERT(self && "UnorderedMap_insert");\
	CSTL_ASSERT(self->magic == self && "UnorderedMap_insert");\
	return Name##_insert_ref(self, key, &value, success);\
}\
\
Name##Iterator Name##_insert_ref(Name *self, KeyType key, ValueType const *value, int *success)\
{\
	Name##Node **alias;\
	Name##Node *node;\
	Name##Node *pos;\
	size_t hash_val;\
	size_t idx;\
	CSTL_ASSERT(self && "UnorderedMap_insert_ref");\
	CSTL_ASSERT(self->magic == self && "UnorderedMap_insert_ref");\
	CSTL_ASSERT(value && "UnorderedMap_insert_ref");\
	hash_val = Hasher(key);\
	idx = hash_val % Name##_bucket_count(self);\
	pos = Name##_find_node(self, key, idx);\
	if (pos != Name##_end(self)) {\
		if (success) *success = 0;\
		return pos;\
	}\
	node = Name##Node_new(key, value);\
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
	CSTL_ASSERT(self && "UnorderedMap_insert_range");\
	CSTL_ASSERT(self->magic == self && "UnorderedMap_insert_range");\
	CSTL_ASSERT(first && "UnorderedMap_insert_range");\
	CSTL_ASSERT(last && "UnorderedMap_insert_range");\
	CSTL_ASSERT(first->magic && "UnorderedMap_insert_range");\
	CSTL_ASSERT(last->magic && "UnorderedMap_insert_range");\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		if (Name##_find(self, pos->key) == Name##_end(self)) {\
			Name##Node *node;\
			node = Name##Node_new(pos->key, &pos->value);\
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
		CSTL_ASSERT(self->size + count <= self->max_load_factor * Name##_bucket_count(self) && "UnorderedMap_insert_range");\
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
ValueType *Name##_at(Name *self, KeyType key)\
{\
	Name##Iterator pos;\
	size_t hash_val;\
	size_t idx;\
	CSTL_ASSERT(self && "UnorderedMap_at");\
	CSTL_ASSERT(self->magic == self && "UnorderedMap_at");\
	hash_val = Hasher(key);\
	idx = hash_val % Name##_bucket_count(self);\
	pos = Name##_find_node(self, key, idx);\
	if (pos == Name##_end(self)) {\
		/* 新しい要素の値にはend_nodeの値を使用 */\
		pos = Name##Node_new(key, &self->end_node.value);\
		if (pos) {\
			Name##Node **alias;\
			/* rehash */\
			if (self->size + 1 > self->max_load_factor * Name##_bucket_count(self)) {\
				size_t s = (size_t) ((self->size + 1) / self->max_load_factor) + 1;\
				if (!Name##_rehash(self, s)) {\
					Name##Node_erase(pos);\
					/* メモリ不足 */\
					CSTL_ASSERT(0 && "UnorderedMap_at() 1");\
					return 0;\
				}\
				idx = hash_val % Name##_bucket_count(self);\
			}\
			alias = Name##Node_Vector_at(self->buckets, idx);\
			*alias = Name##Node_insert(*alias, pos, alias);\
			self->size++;\
			CSTL_MAGIC(pos->magic = self->buckets);\
		} else {\
			/* メモリ不足 */\
			CSTL_ASSERT(0 && "UnorderedMap_at() 2");\
			return 0;\
		}\
	}\
	return &pos->value;\
}\
\


/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param KeyType 要素のキーの型
 * \param ValueType 要素の値の型
 */
#define CSTL_UNORDERED_MULTIMAP_INTERFACE(Name, KeyType, ValueType)	\
CSTL_EXTERN_C_BEGIN()\
CSTL_HASHTABLE_INTERFACE(Name, KeyType, ValueType)\
Name##Iterator Name##_insert(Name *self, KeyType key, ValueType value);\
Name##Iterator Name##_insert_ref(Name *self, KeyType key, ValueType const *value);\
KeyType const *Name##_key(Name##Iterator pos);\
ValueType *Name##_value(Name##Iterator pos);\
CSTL_EXTERN_C_END()\

/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param KeyType 要素のキーの型
 * \param ValueType 要素の値の型
 * \param Hasher ハッシュ関数
 * \param Compare 要素の比較ルーチン
 */
#define CSTL_UNORDERED_MULTIMAP_IMPLEMENT(Name, KeyType, ValueType, Hasher, Compare)	\
CSTL_COMMON_UNORDERED_MAP_IMPLEMENT(Name, KeyType, ValueType, Hasher, Compare)\
CSTL_HASHTABLE_IMPLEMENT_REHASH_MULTI(Name, KeyType, ValueType, Hasher, Compare)\
\
Name##Iterator Name##_insert(Name *self, KeyType key, ValueType value)\
{\
	CSTL_ASSERT(self && "UnorderedMultiMap_insert");\
	CSTL_ASSERT(self->magic == self && "UnorderedMultiMap_insert");\
	return Name##_insert_ref(self, key, &value);\
}\
\
Name##Iterator Name##_insert_ref(Name *self, KeyType key, ValueType const *value)\
{\
	Name##Node **alias;\
	Name##Node *node;\
	register Name##Node *pos;\
	register Name##Node *prev;\
	size_t hash_val;\
	size_t idx;\
	CSTL_ASSERT(self && "UnorderedMultiMap_insert_ref");\
	CSTL_ASSERT(self->magic == self && "UnorderedMultiMap_insert_ref");\
	CSTL_ASSERT(value && "UnorderedMultiMap_insert_ref");\
	hash_val = Hasher(key);\
	idx = hash_val % Name##_bucket_count(self);\
	node = Name##Node_new(key, value);\
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
		if (Compare(key, pos->key) == 0) {\
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
	CSTL_ASSERT(self && "UnorderedMultiMap_insert_range");\
	CSTL_ASSERT(self->magic == self && "UnorderedMultiMap_insert_range");\
	CSTL_ASSERT(first && "UnorderedMultiMap_insert_range");\
	CSTL_ASSERT(last && "UnorderedMultiMap_insert_range");\
	CSTL_ASSERT(first->magic && "UnorderedMultiMap_insert_range");\
	CSTL_ASSERT(last->magic && "UnorderedMultiMap_insert_range");\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		Name##Node *node;\
		node = Name##Node_new(pos->key, &pos->value);\
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
		CSTL_ASSERT(self->size + count <= self->max_load_factor * Name##_bucket_count(self) && "UnorderedMultiMap_insert_range");\
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

#endif /* CSTL_UNORDERED_MAP_H_INCLUDED */
