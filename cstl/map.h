/* 
 * Copyright (c) 2006, KATO Noriaki
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
#include <assert.h>
#include "rbtree.h"

#ifdef __cplusplus
#define CSTL_MAP_BEGIN_EXTERN_C()	extern "C" {
#define CSTL_MAP_END_EXTERN_C()		}
#else
#define CSTL_MAP_BEGIN_EXTERN_C()
#define CSTL_MAP_END_EXTERN_C()
#endif


#define CSTL_COMMON_MAP_IMPLEMENT(Name, KeyType, ValueType, Compare)	\
/*! 
 * \brief mapノード構造体
 */\
struct Name##RBTreeNode {\
	struct Name##RBTreeNode *parent;\
	struct Name##RBTreeNode *left;\
	struct Name##RBTreeNode *right;\
	int color;\
	KeyType key;\
	ValueType value;\
	CSTL_RBTREE_MAGIC(struct Name##RBTreeNode *magic;)\
};\
\
CSTL_RBTREE_WRAPPER_IMPLEMENT(Name, KeyType, ValueType, Compare)\
\
static Name##RBTreeNode *Name##RBTreeNode_new(KeyType key, ValueType value, int color)\
{\
	Name##RBTreeNode *self;\
	self = (Name##RBTreeNode *) malloc(sizeof(Name##RBTreeNode));\
	if (!self) return 0;\
	self->key = key;\
	self->value = value;\
	self->left = (Name##RBTreeNode *) &Name##RBTree_nil;\
	self->right = (Name##RBTreeNode *) &Name##RBTree_nil;\
	self->parent = (Name##RBTreeNode *) &Name##RBTree_nil;\
	self->color = color;\
	return self;\
}\
\
ValueType *Name##_value(Name##Iterator pos)\
{\
	assert(pos && "Map_value");\
	assert(pos->magic && "Map_value");\
	assert(!CSTL_RBTREE_NODE_IS_HEAD(pos) && "Map_value");\
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
CSTL_MAP_BEGIN_EXTERN_C()\
CSTL_RBTREE_WRAPPER_INTERFACE(Name, KeyType, ValueType)\
Name##Iterator Name##_insert(Name *self, KeyType key, ValueType value, int *success);\
ValueType *Name##_value(Name##Iterator pos);\
ValueType *Name##_lookup(Name *self, KeyType key);\
CSTL_MAP_END_EXTERN_C()\

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
	Name##Iterator pos;\
	assert(self && "Map_insert");\
	assert(self->magic == self && "Map_insert");\
	pos = Name##RBTree_find(self->tree, key);\
	if (pos == Name##RBTree_end(self->tree)) {\
		pos = Name##RBTreeNode_new(key, value, CSTL_RBTREE_RED);\
		if (pos) {\
			Name##RBTree_insert(self->tree, pos);\
			if (success) *success = 1;\
			self->nelems++;\
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
	Name##RBTreeNode head;\
	register size_t count = 0;\
	assert(self && "Map_insert_range");\
	assert(self->magic == self && "Map_insert_range");\
	assert(first && "Map_insert_range");\
	assert(last && "Map_insert_range");\
	assert(first->magic && "Map_insert_range");\
	assert(last->magic && "Map_insert_range");\
	head.right = (Name##RBTreeNode *) &Name##RBTree_nil;\
	tmp = &head;\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		if (Name##RBTree_find(self->tree, Name##_key(pos)) == Name##RBTree_end(self->tree)) {\
			tmp->right = Name##RBTreeNode_new(Name##_key(pos), *Name##_value(pos), CSTL_RBTREE_RED);\
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
	for (pos = head.right; pos != (Name##RBTreeNode *) &Name##RBTree_nil; pos = tmp) {\
		tmp = pos->right;\
		pos->right = (Name##RBTreeNode *) &Name##RBTree_nil;\
		Name##RBTree_insert(self->tree, pos);\
	}\
	self->nelems += count;\
	return 1;\
}\
\
ValueType *Name##_lookup(Name *self, KeyType key)\
{\
	Name##Iterator pos;\
	assert(self && "Map_lookup");\
	assert(self->magic == self && "Map_lookup");\
	pos = Name##RBTree_find(self->tree, key);\
	if (pos == Name##RBTree_end(self->tree)) {\
		static ValueType value; /* 新しい要素の値 */\
		pos = Name##RBTreeNode_new(key, value, CSTL_RBTREE_RED);\
		if (pos) {\
			Name##RBTree_insert(self->tree, pos);\
			self->nelems++;\
		} else {\
			/* メモリ不足 */\
			assert(0 && "Map_lookup");\
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
CSTL_MAP_BEGIN_EXTERN_C()\
CSTL_RBTREE_WRAPPER_INTERFACE(Name, KeyType, ValueType)\
Name##Iterator Name##_insert(Name *self, KeyType key, ValueType value);\
ValueType *Name##_value(Name##Iterator pos);\
CSTL_MAP_END_EXTERN_C()\

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
	Name##Iterator pos;\
	assert(self && "MultiMap_insert");\
	assert(self->magic == self && "MultiMap_insert");\
	pos = Name##RBTreeNode_new(key, value, CSTL_RBTREE_RED);\
	if (pos) {\
		Name##RBTree_insert(self->tree, pos);\
		self->nelems++;\
	}\
	return pos;\
}\
\
int Name##_insert_range(Name *self, Name##Iterator first, Name##Iterator last)\
{\
	register Name##Iterator pos;\
	register Name##Iterator tmp;\
	Name##RBTreeNode head;\
	register size_t count = 0;\
	assert(self && "MultiMap_insert_range");\
	assert(self->magic == self && "MultiMap_insert_range");\
	assert(first && "MultiMap_insert_range");\
	assert(last && "MultiMap_insert_range");\
	assert(first->magic && "MultiMap_insert_range");\
	assert(last->magic && "MultiMap_insert_range");\
	head.right = (Name##RBTreeNode *) &Name##RBTree_nil;\
	tmp = &head;\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		tmp->right = Name##RBTreeNode_new(Name##_key(pos), *Name##_value(pos), CSTL_RBTREE_RED);\
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
	for (pos = head.right; pos != (Name##RBTreeNode *) &Name##RBTree_nil; pos = tmp) {\
		tmp = pos->right;\
		pos->right = (Name##RBTreeNode *) &Name##RBTree_nil;\
		Name##RBTree_insert(self->tree, pos);\
	}\
	self->nelems += count;\
	return 1;\
}\
\


#endif /* CSTL_MAP_H_INCLUDED */
