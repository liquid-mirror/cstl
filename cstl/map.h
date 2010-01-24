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
 * \file map.h
 * \brief map/multimapコンテナ
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2006-11-11
 * $URL$
 * $Id$
 */
#ifndef CSTL_MAP_H_INCLUDED
#define CSTL_MAP_H_INCLUDED

#include <stdlib.h>
#include "common.h"
#include "rbtree.h"


#define CSTL_COMMON_MAP_IMPLEMENT(Name, KeyType, ValueType, Compare)	\
/*! \
 * \brief map赤黒木構造体\
 */\
struct Name##RBTree {\
	struct Name##RBTree *parent;\
	struct Name##RBTree *left;\
	struct Name##RBTree *right;\
	int color;\
	KeyType key;\
	ValueType value;\
	CSTL_MAGIC(struct Name##RBTree *magic;)\
};\
\
CSTL_RBTREE_WRAPPER_IMPLEMENT(Name, KeyType, ValueType, Compare)\
\
static Name##RBTree *Name##RBTree_new_node(KeyType key, ValueType const *value, int color)\
{\
	Name##RBTree *node;\
	node = (Name##RBTree *) malloc(sizeof(Name##RBTree));\
	if (!node) return 0;\
	node->key = key;\
	node->value = *value;\
	node->left = (Name##RBTree *) &Name##RBTree_nil;\
	node->right = (Name##RBTree *) &Name##RBTree_nil;\
	node->parent = (Name##RBTree *) &Name##RBTree_nil;\
	node->color = color;\
	return node;\
}\
\
KeyType const *Name##_key(Name##Iterator pos)\
{\
	CSTL_ASSERT(pos && "Map_key");\
	CSTL_ASSERT(pos->magic && "Map_key");\
	CSTL_ASSERT(!CSTL_RBTREE_IS_HEAD(pos, Name) && "Map_key");\
	return &pos->key;\
}\
\
ValueType *Name##_value(Name##Iterator pos)\
{\
	CSTL_ASSERT(pos && "Map_value");\
	CSTL_ASSERT(pos->magic && "Map_value");\
	CSTL_ASSERT(!CSTL_RBTREE_IS_HEAD(pos, Name) && "Map_value");\
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
#define CSTL_MAP_INTERFACE(Name, KeyType, ValueType)	\
CSTL_EXTERN_C_BEGIN()\
CSTL_RBTREE_WRAPPER_INTERFACE(Name, KeyType, ValueType)\
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
 * \param Compare 要素の比較ルーチン
 */
#define CSTL_MAP_IMPLEMENT(Name, KeyType, ValueType, Compare)	\
\
CSTL_COMMON_MAP_IMPLEMENT(Name, KeyType, ValueType, Compare)	\
\
Name##Iterator Name##_insert(Name *self, KeyType key, ValueType value, int *success)\
{\
	CSTL_ASSERT(self && "Map_insert");\
	CSTL_ASSERT(self->magic == self && "Map_insert");\
	return Name##_insert_ref(self, key, &value, success);\
}\
\
Name##Iterator Name##_insert_ref(Name *self, KeyType key, ValueType const *value, int *success)\
{\
	Name##Iterator pos;\
	CSTL_ASSERT(self && "Map_insert_ref");\
	CSTL_ASSERT(self->magic == self && "Map_insert_ref");\
	CSTL_ASSERT(value && "Map_insert_ref");\
	pos = Name##RBTree_find(self->tree, key);\
	if (pos == Name##RBTree_end(self->tree)) {\
		pos = Name##RBTree_new_node(key, value, Name##_COLOR_RED);\
		if (pos) {\
			Name##RBTree_insert(self->tree, pos);\
			if (success) *success = 1;\
			self->size++;\
		} else {\
			if (success) *success = 0;\
		}\
	} else {\
		if (success) *success = 0;\
	}\
	return pos;\
}\
\
int Name##_insert_range(Name *self, Name##Iterator first, Name##Iterator last)\
{\
	register Name##Iterator pos;\
	register Name##Iterator tmp;\
	Name##RBTree head;\
	register size_t count = 0;\
	CSTL_ASSERT(self && "Map_insert_range");\
	CSTL_ASSERT(self->magic == self && "Map_insert_range");\
	CSTL_ASSERT(first && "Map_insert_range");\
	CSTL_ASSERT(last && "Map_insert_range");\
	CSTL_ASSERT(first->magic && "Map_insert_range");\
	CSTL_ASSERT(last->magic && "Map_insert_range");\
	head.right = (Name##RBTree *) &Name##RBTree_nil;\
	tmp = &head;\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		if (Name##RBTree_find(self->tree, pos->key) == Name##RBTree_end(self->tree)) {\
			tmp->right = Name##RBTree_new_node(pos->key, &pos->value, Name##_COLOR_RED);\
			if (!tmp->right) {\
				for (pos = head.right; pos != 0; pos = tmp) {\
					tmp = pos->right;\
					free(pos);\
				}\
				return 0;\
			}\
			tmp = tmp->right;\
			count++;\
		}\
	}\
	for (pos = head.right; pos != (Name##RBTree *) &Name##RBTree_nil; pos = tmp) {\
		tmp = pos->right;\
		pos->right = (Name##RBTree *) &Name##RBTree_nil;\
		Name##RBTree_insert(self->tree, pos);\
	}\
	self->size += count;\
	return 1;\
}\
\
ValueType *Name##_at(Name *self, KeyType key)\
{\
	Name##Iterator pos;\
	CSTL_ASSERT(self && "Map_at");\
	CSTL_ASSERT(self->magic == self && "Map_at");\
	pos = Name##RBTree_find(self->tree, key);\
	if (pos == Name##RBTree_end(self->tree)) {\
		/* 新しい要素の値にはnilの値を使用 */\
		pos = Name##RBTree_new_node(key, &Name##RBTree_nil.value, Name##_COLOR_RED);\
		if (pos) {\
			Name##RBTree_insert(self->tree, pos);\
			self->size++;\
		} else {\
			/* メモリ不足 */\
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
#define CSTL_MULTIMAP_INTERFACE(Name, KeyType, ValueType)	\
CSTL_EXTERN_C_BEGIN()\
CSTL_RBTREE_WRAPPER_INTERFACE(Name, KeyType, ValueType)\
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
 * \param Compare 要素の比較ルーチン
 */
#define CSTL_MULTIMAP_IMPLEMENT(Name, KeyType, ValueType, Compare)	\
\
CSTL_COMMON_MAP_IMPLEMENT(Name, KeyType, ValueType, Compare)	\
\
Name##Iterator Name##_insert(Name *self, KeyType key, ValueType value)\
{\
	CSTL_ASSERT(self && "MultiMap_insert");\
	CSTL_ASSERT(self->magic == self && "MultiMap_insert");\
	return Name##_insert_ref(self, key, &value);\
}\
\
Name##Iterator Name##_insert_ref(Name *self, KeyType key, ValueType const *value)\
{\
	Name##Iterator pos;\
	CSTL_ASSERT(self && "MultiMap_insert_ref");\
	CSTL_ASSERT(self->magic == self && "MultiMap_insert_ref");\
	CSTL_ASSERT(value && "MultiMap_insert_ref");\
	pos = Name##RBTree_new_node(key, value, Name##_COLOR_RED);\
	if (pos) {\
		Name##RBTree_insert(self->tree, pos);\
		self->size++;\
	}\
	return pos;\
}\
\
int Name##_insert_range(Name *self, Name##Iterator first, Name##Iterator last)\
{\
	register Name##Iterator pos;\
	register Name##Iterator tmp;\
	Name##RBTree head;\
	register size_t count = 0;\
	CSTL_ASSERT(self && "MultiMap_insert_range");\
	CSTL_ASSERT(self->magic == self && "MultiMap_insert_range");\
	CSTL_ASSERT(first && "MultiMap_insert_range");\
	CSTL_ASSERT(last && "MultiMap_insert_range");\
	CSTL_ASSERT(first->magic && "MultiMap_insert_range");\
	CSTL_ASSERT(last->magic && "MultiMap_insert_range");\
	head.right = (Name##RBTree *) &Name##RBTree_nil;\
	tmp = &head;\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		tmp->right = Name##RBTree_new_node(pos->key, &pos->value, Name##_COLOR_RED);\
		if (!tmp->right) {\
			for (pos = head.right; pos != 0; pos = tmp) {\
				tmp = pos->right;\
				free(pos);\
			}\
			return 0;\
		}\
		tmp = tmp->right;\
		count++;\
	}\
	for (pos = head.right; pos != (Name##RBTree *) &Name##RBTree_nil; pos = tmp) {\
		tmp = pos->right;\
		pos->right = (Name##RBTree *) &Name##RBTree_nil;\
		Name##RBTree_insert(self->tree, pos);\
	}\
	self->size += count;\
	return 1;\
}\
\


#endif /* CSTL_MAP_H_INCLUDED */
