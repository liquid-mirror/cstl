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
 * \file set.h
 * \brief set/multisetコンテナ
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2006-11-11
 */
#ifndef CSTL_SET_H_INCLUDED
#define CSTL_SET_H_INCLUDED

#include "rbtree.h"

#ifdef __cplusplus
#define SET_BEGIN_EXTERN_C()	extern "C" {
#define SET_END_EXTERN_C()		}
#else
#define SET_BEGIN_EXTERN_C()
#define SET_END_EXTERN_C()
#endif


#define COMMON_SET_IMPLEMENT(Name, Type, Compare, Order)	\
/*! 
 * \brief setノード構造体
 */\
struct Name##RBTreeNode_t {\
	struct Name##RBTreeNode_t *parent;\
	struct Name##RBTreeNode_t *left;\
	struct Name##RBTreeNode_t *right;\
	RBTreeColor color;\
	Type key;\
	RBTREE_MAGIC(void *magic;)\
};\
\
RBTREE_WRAPPER_IMPLEMENT(Name, Type, Type, Compare, Order)\
\
static Name##RBTreeNode *Name##RBTreeNode_new(Type elem, Type value, RBTreeColor color)\
{\
	Name##RBTreeNode *self;\
	self = (Name##RBTreeNode *) malloc(sizeof(Name##RBTreeNode));\
	if (!self) return 0;\
	self->key = elem;\
	self->left = &Name##RBTree_nil;\
	self->right = &Name##RBTree_nil;\
	self->parent = &Name##RBTree_nil;\
	self->color = color;\
	return self;\
}\
\


/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define SET_INTERFACE(Name, Type)	\
\
SET_BEGIN_EXTERN_C()\
RBTREE_WRAPPER_INTERFACE(Name, Type, Type)\
Name##Iterator Name##_insert(Name *self, Type elem, int *success);\
SET_END_EXTERN_C()\

/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 * \param Compare 要素の比較ルーチン
 * \param Order ソートの順序
 */
#define SET_IMPLEMENT(Name, Type, Compare, Order)	\
COMMON_SET_IMPLEMENT(Name, Type, Compare, Order)\
\
Name##Iterator Name##_insert(Name *self, Type elem, int *success)\
{\
	Name##Iterator pos;\
	assert(self && "Set_insert");\
	assert(self->magic == self && "Set_insert");\
	pos = Name##RBTree_find(self->tree, elem);\
	if (pos == Name##RBTree_end(self->tree)) {\
		pos = Name##RBTree_insert(self->tree, elem, elem);\
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
	assert(self && "Set_assign");\
	assert(self->magic == self && "Set_assign");\
	assert(first && "Set_assign");\
	assert(last && "Set_assign");\
	assert(first->magic && "Set_assign");\
	assert(last->magic && "Set_assign");\
	x = Name##_new();\
	if (!x) return 0;\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		if (!Name##_insert(x, Name##_key(pos), 0)) {\
			Name##_delete(x);\
			return 0;\
		}\
	}\
	Name##_swap(self, x);\
	Name##_delete(x);\
	return 1;\
}\
\


/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define MULTISET_INTERFACE(Name, Type)	\
\
SET_BEGIN_EXTERN_C()\
RBTREE_WRAPPER_INTERFACE(Name, Type, Type)\
Name##Iterator Name##_insert(Name *self, Type elem);\
SET_END_EXTERN_C()\

/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 * \param Compare 要素の比較ルーチン
 * \param Order ソートの順序
 */
#define MULTISET_IMPLEMENT(Name, Type, Compare, Order)	\
COMMON_SET_IMPLEMENT(Name, Type, Compare, Order)\
\
Name##Iterator Name##_insert(Name *self, Type elem)\
{\
	Name##Iterator pos;\
	assert(self && "MultiSet_insert");\
	assert(self->magic == self && "MultiSet_insert");\
	pos = Name##RBTree_insert(self->tree, elem, elem);\
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
	assert(self && "MultiSet_assign");\
	assert(self->magic == self && "MultiSet_assign");\
	assert(first && "MultiSet_assign");\
	assert(last && "MultiSet_assign");\
	assert(first->magic && "MultiSet_assign");\
	assert(last->magic && "MultiSet_assign");\
	x = Name##_new();\
	if (!x) return 0;\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		if (!Name##_insert(x, Name##_key(pos))) {\
			Name##_delete(x);\
			return 0;\
		}\
	}\
	Name##_swap(self, x);\
	Name##_delete(x);\
	return 1;\
}\
\


#endif /* CSTL_SET_H_INCLUDED */
