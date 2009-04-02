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
/*! \
 * \brief set赤黒木構造体\
 */\
struct Name##RBTree {\
	struct Name##RBTree *parent;\
	struct Name##RBTree *left;\
	struct Name##RBTree *right;\
	int color;\
	Type key;\
	CSTL_RBTREE_MAGIC(struct Name##RBTree *magic;)\
};\
\
CSTL_RBTREE_WRAPPER_IMPLEMENT(Name, Type, Type, Compare)\
\
static Name##RBTree *Name##RBTree_new_node(Type const *elem, int color)\
{\
	Name##RBTree *node;\
	node = (Name##RBTree *) malloc(sizeof(Name##RBTree));\
	if (!node) return 0;\
	node->key = *elem;\
	node->left = (Name##RBTree *) &Name##RBTree_nil;\
	node->right = (Name##RBTree *) &Name##RBTree_nil;\
	node->parent = (Name##RBTree *) &Name##RBTree_nil;\
	node->color = color;\
	return node;\
}\
\
Type const *Name##_elem(Name##Iterator pos)\
{\
	assert(pos && "Set_elem");\
	assert(pos->magic && "Set_elem");\
	assert(!CSTL_RBTREE_IS_HEAD(pos) && "Set_elem");\
	return &pos->key;\
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
Type const *Name##_elem(Name##Iterator pos);\
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
		pos = Name##RBTree_new_node(&elem, CSTL_RBTREE_RED);\
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
	assert(self && "Set_insert_range");\
	assert(self->magic == self && "Set_insert_range");\
	assert(first && "Set_insert_range");\
	assert(last && "Set_insert_range");\
	assert(first->magic && "Set_insert_range");\
	assert(last->magic && "Set_insert_range");\
	head.right = (Name##RBTree *) &Name##RBTree_nil;\
	tmp = &head;\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		if (Name##RBTree_find(self->tree, *Name##_elem(pos)) == Name##RBTree_end(self->tree)) {\
			tmp->right = Name##RBTree_new_node(Name##_elem(pos), CSTL_RBTREE_RED);\
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
Type const *Name##_elem(Name##Iterator pos);\
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
	pos = Name##RBTree_new_node(&elem, CSTL_RBTREE_RED);\
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
	assert(self && "MultiSet_insert_range");\
	assert(self->magic == self && "MultiSet_insert_range");\
	assert(first && "MultiSet_insert_range");\
	assert(last && "MultiSet_insert_range");\
	assert(first->magic && "MultiSet_insert_range");\
	assert(last->magic && "MultiSet_insert_range");\
	head.right = (Name##RBTree *) &Name##RBTree_nil;\
	tmp = &head;\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		tmp->right = Name##RBTree_new_node(Name##_elem(pos), CSTL_RBTREE_RED);\
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


#endif /* CSTL_SET_H_INCLUDED */
