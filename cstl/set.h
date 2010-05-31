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
#include "common.h"
#include "rbtree.h"


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
	CSTL_MAGIC(struct Name##RBTree *magic;)\
};\
\
CSTL_RBTREE_WRAPPER_IMPLEMENT(Name, Type, Type, Compare)\
\
static Name##RBTree *Name##RBTree_new_node(Type data, int color)\
{\
	Name##RBTree *node;\
	node = (Name##RBTree *) malloc(sizeof(Name##RBTree));\
	if (!node) return 0;\
	node->key = data;\
	node->left = (Name##RBTree *) &Name##RBTree_nil;\
	node->right = (Name##RBTree *) &Name##RBTree_nil;\
	node->parent = (Name##RBTree *) &Name##RBTree_nil;\
	node->color = color;\
	return node;\
}\
\
Type const *Name##_data(Name##Iterator pos)\
{\
	CSTL_ASSERT(pos && "Set_data");\
	CSTL_ASSERT(pos->magic && "Set_data");\
	CSTL_ASSERT(!CSTL_RBTREE_IS_HEAD(pos, Name) && "Set_data");\
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
CSTL_EXTERN_C_BEGIN()\
CSTL_RBTREE_WRAPPER_INTERFACE(Name, Type, Type)\
\
typedef int (*Name##_insert_set_t)(Name *self, CstlIterInternalData pos, Type data, Name##Iterator *iter, int *success);\
typedef int (*Name##_insert_multiset_t)(Name *self, CstlIterInternalData pos, Type data, Name##Iterator *iter);\
\
struct Name##Vtable {\
	Name##_delete_t       delete_;\
	Name##_empty_t        empty;\
	Name##_size_t         size;\
	Name##_clear_t        clear;\
	Name##_begin_t        begin;\
	Name##_end_t          end;\
	Name##_rbegin_t       rbegin;\
	Name##_rend_t         rend;\
	Name##_insert_range_t insert_range;\
	Name##_insert_range_assoc_t insert_range_assoc;\
	Name##_erase_t        erase;\
	Name##_erase_range_t  erase_range;\
	Name##_erase_key_t    erase_key;\
	Name##_swap_t         swap;\
	Name##_insert_set_t   insert_set;\
}\
\
Name##Iterator Name##_insert(Name *self, Type data, int *success);\
Type const *Name##_data(Name##Iterator pos);\
CSTL_EXTERN_C_END()\

/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 * \param Compare 要素の比較ルーチン
 */
#define CSTL_SET_IMPLEMENT(Name, Type, Compare)	\
static const Name##IteratorVtable Name##Iterator_vtbl = {\
	Name##Iterator_data,\
	0, /* key */\
	0, /* val */\
	Name##Iterator_next,\
	Name##Iterator_prev,\
	Name##Iterator_inc,\
	Name##Iterator_dec,\
	Name##Iterator_eq,\
	Name##Iterator_ne,\
	0, /* at */\
	0, /* add */\
	0, /* sub */\
	0, /* inc_n */\
	0, /* dec_n */\
	0, /* diff */\
	0, /* lt */\
	0, /* le */\
	0, /* gt */\
	0, /* ge */\
	Name##Iterator_reverse_iterator,\
	0, /* ReverseIterator_base */\
	CSTL_CONTAINER_SET,\
	0, /* is_rand_iter */\
	0, /* is_reverse_iter */\
};\
\
static const Name##IteratorVtable Name##ReverseIterator_vtbl = {\
	Name##ReverseIterator_data,\
	0, /* key */\
	0, /* val */\
	Name##ReverseIterator_next,\
	Name##ReverseIterator_prev,\
	Name##ReverseIterator_inc,\
	Name##ReverseIterator_dec,\
	Name##ReverseIterator_eq,\
	Name##ReverseIterator_ne,\
	0, /* at */\
	0, /* add */\
	0, /* sub */\
	0, /* inc_n */\
	0, /* dec_n */\
	0, /* diff */\
	0, /* lt */\
	0, /* le */\
	0, /* gt */\
	0, /* ge */\
	Name##Iterator_reverse_iterator,\
	Name##ReverseIterator_base, /* ReverseIterator_base */\
	CSTL_CONTAINER_SET,\
	0, /* is_rand_iter */\
	1, /* is_reverse_iter */\
};\
\
static const struct Name##Vtable Name##_vtbl = {\
	Name##_delete,\
	Name##_empty,\
	Name##_size,\
	Name##_clear,\
	Name##_begin,\
	Name##_end,\
	Name##_rbegin,\
	Name##_rend,\
	Name##_insert_range,\
	Name##_insert_range_assoc,\
	Name##_erase,\
	Name##_erase_range,\
	Name##_erase_key,\
	Name##_swap,\
	Name##_insert_set,\
};\
\
CSTL_COMMON_SET_IMPLEMENT(Name, Type, Compare)\
\
Name##Iterator Name##_insert(Name *self, Type data, int *success)\
{\
	Name##Iterator pos;\
	CSTL_ASSERT(self && "Set_insert");\
	CSTL_ASSERT(self->magic == self && "Set_insert");\
	pos = Name##RBTree_find(self->tree, data);\
	if (pos == Name##RBTree_end(self->tree)) {\
		pos = Name##RBTree_new_node(data, Name##_COLOR_RED);\
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
int Name##_insert_range_assoc(Name *self, Name##Iterator first, Name##Iterator last)\
{\
	register Name##Iterator pos;\
	register Name##Iterator tmp;\
	Name##RBTree head;\
	register size_t count = 0;\
	CSTL_ASSERT(self && "Set_insert_range_assoc");\
	CSTL_ASSERT(self->magic == self && "Set_insert_range_assoc");\
	CSTL_ASSERT(first && "Set_insert_range_assoc");\
	CSTL_ASSERT(last && "Set_insert_range_assoc");\
	CSTL_ASSERT(first->magic && "Set_insert_range_assoc");\
	CSTL_ASSERT(last->magic && "Set_insert_range_assoc");\
	head.right = (Name##RBTree *) &Name##RBTree_nil;\
	tmp = &head;\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		if (Name##RBTree_find(self->tree, pos->key) == Name##RBTree_end(self->tree)) {\
			tmp->right = Name##RBTree_new_node(pos->key, Name##_COLOR_RED);\
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
CSTL_EXTERN_C_BEGIN()\
CSTL_RBTREE_WRAPPER_INTERFACE(Name, Type, Type)\
Name##Iterator Name##_insert(Name *self, Type data);\
Type const *Name##_data(Name##Iterator pos);\
CSTL_EXTERN_C_END()\

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
Name##Iterator Name##_insert(Name *self, Type data)\
{\
	Name##Iterator pos;\
	CSTL_ASSERT(self && "MultiSet_insert");\
	CSTL_ASSERT(self->magic == self && "MultiSet_insert");\
	pos = Name##RBTree_new_node(data, Name##_COLOR_RED);\
	if (pos) {\
		Name##RBTree_insert(self->tree, pos);\
		self->size++;\
	}\
	return pos;\
}\
\
int Name##_insert_range_assoc(Name *self, Name##Iterator first, Name##Iterator last)\
{\
	register Name##Iterator pos;\
	register Name##Iterator tmp;\
	Name##RBTree head;\
	register size_t count = 0;\
	CSTL_ASSERT(self && "MultiSet_insert_range_assoc");\
	CSTL_ASSERT(self->magic == self && "MultiSet_insert_range_assoc");\
	CSTL_ASSERT(first && "MultiSet_insert_range_assoc");\
	CSTL_ASSERT(last && "MultiSet_insert_range_assoc");\
	CSTL_ASSERT(first->magic && "MultiSet_insert_range_assoc");\
	CSTL_ASSERT(last->magic && "MultiSet_insert_range_assoc");\
	head.right = (Name##RBTree *) &Name##RBTree_nil;\
	tmp = &head;\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		tmp->right = Name##RBTree_new_node(pos->key, Name##_COLOR_RED);\
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
