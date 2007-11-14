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
 * $URL$
 * $Id$
 */
#ifndef CSTL_SET_H_INCLUDED
#define CSTL_SET_H_INCLUDED

#include <stdlib.h>
#include <assert.h>
#include "rbtree.h"

#ifdef __cplusplus
#define CSTL_SET_BEGIN_EXTERN_C()	extern "C" {
#define CSTL_SET_END_EXTERN_C()		}
#else
#define CSTL_SET_BEGIN_EXTERN_C()
#define CSTL_SET_END_EXTERN_C()
#endif


#define CSTL_COMMON_SET_IMPLEMENT(Name, Type, Compare)	\
/*! 
 * \brief setノード構造体
 */\
struct Name##RBTreeNode_t {\
	struct Name##RBTreeNode_t *parent;\
	struct Name##RBTreeNode_t *left;\
	struct Name##RBTreeNode_t *right;\
	int color;\
	Type key;\
	CSTL_RBTREE_MAGIC(struct Name##RBTreeNode_t *magic);\
};\
\
CSTL_RBTREE_WRAPPER_IMPLEMENT(Name, Type, Type, Compare)\
\
static Name##RBTreeNode *Name##RBTreeNode_new(Type elem, int color)\
{\
	Name##RBTreeNode *self;\
	self = (Name##RBTreeNode *) malloc(sizeof(Name##RBTreeNode));\
	if (!self) return 0;\
	self->key = elem;\
	self->left = (Name##RBTreeNode *) &Name##RBTree_nil;\
	self->right = (Name##RBTreeNode *) &Name##RBTree_nil;\
	self->parent = (Name##RBTreeNode *) &Name##RBTree_nil;\
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
#define CSTL_SET_INTERFACE(Name, Type)	\
\
CSTL_SET_BEGIN_EXTERN_C()\
CSTL_RBTREE_WRAPPER_INTERFACE(Name, Type, Type)\
Name##Iterator Name##_insert(Name *self, Type elem, int *success);\
CSTL_SET_END_EXTERN_C()\

/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 * \param Compare 要素の比較ルーチン
 */
#define CSTL_SET_IMPLEMENT(Name, Type, Compare)	\
CSTL_COMMON_SET_IMPLEMENT(Name, Type, Compare)\
\
Name##Iterator Name##_insert(Name *self, Type elem, int *success)\
{\
	Name##Iterator pos;\
	assert(self && "Set_insert");\
	assert(self->magic == self && "Set_insert");\
	pos = Name##RBTree_find(self->tree, elem);\
	if (pos == Name##RBTree_end(self->tree)) {\
		pos = Name##RBTreeNode_new(elem, CSTL_RBTREE_RED);\
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
	Name##Iterator *tmp;\
	register size_t count = 0;\
	register size_t i = 0;\
	register size_t j;\
	assert(self && "Set_insert_range");\
	assert(self->magic == self && "Set_insert_range");\
	assert(first && "Set_insert_range");\
	assert(last && "Set_insert_range");\
	assert(first->magic && "Set_insert_range");\
	assert(last->magic && "Set_insert_range");\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		if (Name##RBTree_find(self->tree, Name##_key(pos)) == Name##RBTree_end(self->tree)) {\
			count++;\
		}\
	}\
	tmp = (Name##Iterator *) malloc(sizeof(Name##Iterator) * count);\
	if (!tmp) return 0;\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		if (Name##RBTree_find(self->tree, Name##_key(pos)) == Name##RBTree_end(self->tree)) {\
			tmp[i] = Name##RBTreeNode_new(Name##_key(pos), CSTL_RBTREE_RED);\
			if (!tmp[i]) {\
				for (j = 0; j < i; j++) {\
					free(tmp[j]);\
				}\
				free(tmp);\
				return 0;\
			}\
			i++;\
		}\
	}\
	assert(i == count);\
	for (i = 0; i < count; i++) {\
		Name##RBTree_insert(self->tree, tmp[i]);\
	}\
	self->nelems += count;\
	free(tmp);\
	return 1;\
}\
\


/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_MULTISET_INTERFACE(Name, Type)	\
\
CSTL_SET_BEGIN_EXTERN_C()\
CSTL_RBTREE_WRAPPER_INTERFACE(Name, Type, Type)\
Name##Iterator Name##_insert(Name *self, Type elem);\
CSTL_SET_END_EXTERN_C()\

/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 * \param Compare 要素の比較ルーチン
 */
#define CSTL_MULTISET_IMPLEMENT(Name, Type, Compare)	\
CSTL_COMMON_SET_IMPLEMENT(Name, Type, Compare)\
\
Name##Iterator Name##_insert(Name *self, Type elem)\
{\
	Name##Iterator pos;\
	assert(self && "MultiSet_insert");\
	assert(self->magic == self && "MultiSet_insert");\
	pos = Name##RBTreeNode_new(elem, CSTL_RBTREE_RED);\
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
	Name##Iterator *tmp;\
	register size_t count = 0;\
	register size_t i = 0;\
	register size_t j;\
	assert(self && "MultiSet_insert_range");\
	assert(self->magic == self && "MultiSet_insert_range");\
	assert(first && "MultiSet_insert_range");\
	assert(last && "MultiSet_insert_range");\
	assert(first->magic && "MultiSet_insert_range");\
	assert(last->magic && "MultiSet_insert_range");\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		count++;\
	}\
	tmp = (Name##Iterator *) malloc(sizeof(Name##Iterator) * count);\
	if (!tmp) return 0;\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		tmp[i] = Name##RBTreeNode_new(Name##_key(pos), CSTL_RBTREE_RED);\
		if (!tmp[i]) {\
			for (j = 0; j < i; j++) {\
				free(tmp[j]);\
			}\
			free(tmp);\
			return 0;\
		}\
		i++;\
	}\
	assert(i == count);\
	for (i = 0; i < count; i++) {\
		Name##RBTree_insert(self->tree, tmp[i]);\
	}\
	self->nelems += count;\
	free(tmp);\
	return 1;\
}\
\


#endif /* CSTL_SET_H_INCLUDED */
