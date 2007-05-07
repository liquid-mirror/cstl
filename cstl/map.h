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
 */
#ifndef CSTL_MAP_H_INCLUDED
#define CSTL_MAP_H_INCLUDED

#include "rbtree.h"

#ifdef __cplusplus
#define MAP_BEGIN_EXTERN_C()	extern "C" {
#define MAP_END_EXTERN_C()		}
#else
#define MAP_BEGIN_EXTERN_C()
#define MAP_END_EXTERN_C()
#endif


#define COMMON_MAP_IMPLEMENT(Name, KeyType, ValueType, Compare)	\
/*! 
 * \brief mapノード構造体
 */\
struct Name##RBTreeNode_t {\
	struct Name##RBTreeNode_t *parent;\
	struct Name##RBTreeNode_t *left;\
	struct Name##RBTreeNode_t *right;\
	RBTreeColor color;\
	KeyType key;\
	ValueType value;\
	RBTREE_MAGIC(void *magic;)\
};\
\
RBTREE_WRAPPER_IMPLEMENT(Name, KeyType, ValueType, Compare)\
\
static Name##RBTreeNode *Name##RBTreeNode_new(KeyType key, ValueType value, RBTreeColor color)\
{\
	Name##RBTreeNode *self;\
	self = (Name##RBTreeNode *) malloc(sizeof(Name##RBTreeNode));\
	if (!self) return 0;\
	self->key = key;\
	self->value = value;\
	self->left = &Name##RBTree_nil;\
	self->right = &Name##RBTree_nil;\
	self->parent = &Name##RBTree_nil;\
	self->color = color;\
	return self;\
}\
\
ValueType *Name##_value(Name##Iterator pos)\
{\
	assert(pos && "Map_value");\
	assert(pos->magic && "Map_value");\
	assert(!Name##RBTreeNode_is_head(pos) && "Map_value");\
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
#define MAP_INTERFACE(Name, KeyType, ValueType)	\
MAP_BEGIN_EXTERN_C()\
RBTREE_WRAPPER_INTERFACE(Name, KeyType, ValueType)\
Name##Iterator Name##_insert(Name *self, KeyType key, ValueType value, int *success);\
ValueType *Name##_value(Name##Iterator pos);\
ValueType *Name##_lookup(Name *self, KeyType key);\
MAP_END_EXTERN_C()\

/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param KeyType 要素のキーの型
 * \param ValueType 要素の値の型
 * \param Compare 要素の比較ルーチン
 */
#define MAP_IMPLEMENT(Name, KeyType, ValueType, Compare)	\
\
COMMON_MAP_IMPLEMENT(Name, KeyType, ValueType, Compare)	\
\
Name##Iterator Name##_insert(Name *self, KeyType key, ValueType value, int *success)\
{\
	Name##Iterator pos;\
	assert(self && "Map_insert");\
	assert(self->magic == self && "Map_insert");\
	pos = Name##RBTree_find(self->tree, key);\
	if (pos == Name##RBTree_end(self->tree)) {\
		pos = Name##RBTree_insert(self->tree, key, value);\
		if (pos) {\
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
int Name##_assign(Name *self, Name##Iterator first, Name##Iterator last)\
{\
	Name *x;\
	Name##Iterator pos;\
	assert(self && "Map_assign");\
	assert(self->magic == self && "Map_assign");\
	assert(first && "Map_assign");\
	assert(last && "Map_assign");\
	assert(first->magic && "Map_assign");\
	assert(last->magic && "Map_assign");\
	x = Name##_new();\
	if (!x) return 0;\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		if (!Name##_insert(x, Name##_key(pos), *Name##_value(pos), 0)) {\
			Name##_delete(x);\
			return 0;\
		}\
	}\
	Name##_swap(self, x);\
	Name##_delete(x);\
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
		pos = Name##RBTree_insert(self->tree, key, value);\
		if (pos) {\
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
#define MULTIMAP_INTERFACE(Name, KeyType, ValueType)	\
MAP_BEGIN_EXTERN_C()\
RBTREE_WRAPPER_INTERFACE(Name, KeyType, ValueType)\
Name##Iterator Name##_insert(Name *self, KeyType key, ValueType value);\
ValueType *Name##_value(Name##Iterator pos);\
MAP_END_EXTERN_C()\

/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param KeyType 要素のキーの型
 * \param ValueType 要素の値の型
 * \param Compare 要素の比較ルーチン
 */
#define MULTIMAP_IMPLEMENT(Name, KeyType, ValueType, Compare)	\
\
COMMON_MAP_IMPLEMENT(Name, KeyType, ValueType, Compare)	\
\
Name##Iterator Name##_insert(Name *self, KeyType key, ValueType value)\
{\
	Name##Iterator pos;\
	assert(self && "MultiMap_insert");\
	assert(self->magic == self && "MultiMap_insert");\
	pos = Name##RBTree_insert(self->tree, key, value);\
	if (pos) {\
		self->nelems++;\
	}\
	return pos;\
}\
\
int Name##_assign(Name *self, Name##Iterator first, Name##Iterator last)\
{\
	Name *x;\
	Name##Iterator pos;\
	assert(self && "MultiMap_assign");\
	assert(self->magic == self && "MultiMap_assign");\
	assert(first && "MultiMap_assign");\
	assert(last && "MultiMap_assign");\
	assert(first->magic && "MultiMap_assign");\
	assert(last->magic && "MultiMap_assign");\
	x = Name##_new();\
	if (!x) return 0;\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		if (!Name##_insert(x, Name##_key(pos), *Name##_value(pos))) {\
			Name##_delete(x);\
			return 0;\
		}\
	}\
	Name##_swap(self, x);\
	Name##_delete(x);\
	return 1;\
}\
\


#endif /* CSTL_MAP_H_INCLUDED */
