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
static const Name##IteratorVtable Name##Iterator_vtbl = {\
	0, /* data */\
	Name##Iterator_key,\
	Name##Iterator_val,\
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
	CSTL_CONTAINER_MAP,\
	0, /* is_rand_iter */\
	0, /* is_reverse_iter */\
};\
\
static const Name##IteratorVtable Name##ReverseIterator_vtbl = {\
	0, /* data */\
	Name##ReverseIterator_key,\
	Name##ReverseIterator_val,\
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
	CSTL_CONTAINER_MAP,\
	0, /* is_rand_iter */\
	1, /* is_reverse_iter */\
};\
\
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
KeyType const *Name##Iterator_key(Name##Iterator pos)\
{\
	CSTL_ASSERT(pos && "MapIterator_key");\
	CSTL_ASSERT(pos->magic && "MapIterator_key");\
	CSTL_ASSERT(!CSTL_RBTREE_IS_HEAD(pos, Name) && "MapIterator_key");\
	return &pos->key;\
}\
\
ValueType *Name##Iterator_val(Name##Iterator pos)\
{\
	CSTL_ASSERT(pos && "MapIterator_val");\
	CSTL_ASSERT(pos->magic && "MapIterator_val");\
	CSTL_ASSERT(!CSTL_RBTREE_IS_HEAD(pos, Name) && "MapIterator_val");\
	return &pos->value;\
}\
\
KeyType const *Name##ReverseIterator_key(Name##Iterator pos)\
{\
	CSTL_ASSERT(pos && "MapReverseIterator_key");\
	CSTL_ASSERT(pos->magic && "MapReverseIterator_key");\
	CSTL_ASSERT(!CSTL_RBTREE_IS_HEAD(pos, Name) && "MapReverseIterator_key");\
	/* TODO */\
	return &pos->key;\
}\
\
ValueType *Name##ReverseIterator_val(Name##Iterator pos)\
{\
	CSTL_ASSERT(pos && "MapReverseIterator_val");\
	CSTL_ASSERT(pos->magic && "MapReverseIterator_val");\
	CSTL_ASSERT(!CSTL_RBTREE_IS_HEAD(pos, Name) && "MapReverseIterator_val");\
	/* TODO */\
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
\
\
typedef int (*Name##_map_insert_t)(Name *self, keyType key, ValueType value, Name##Iterator *iter, int *success);\
typedef int (*Name##_map_insert_ref_t)(Name *self, keyType key, ValueType const *value, Name##Iterator *iter, int *success);\
typedef ValueType (*Name##_at_t)(Name *self, KeyType key);\
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
	Name##_assoc_insert_range_t assoc_insert_range;\
	Name##_erase_t        erase;\
	Name##_erase_range_t  erase_range;\
	Name##_erase_key_t    erase_key;\
	Name##_count_t        count;\
	Name##_find_t         find;\
	Name##_lower_bound_t  lower_bound;\
	Name##_upper_bound_t  upper_bound;\
	Name##_equal_range_t  equal_range;\
	Name##_swap_t         swap;\
	Name##_map_insert_t   map_insert;\
	Name##_map_insert_ref_t map_insert_ref;\
	Name##_at_t           at;\
};\
\
int Name##_map_insert(Name *self, KeyType key, ValueType value, Name##Iterator *iter, int *success);\
int Name##_map_insert_ref(Name *self, KeyType key, ValueType const *value, Name##Iterator *iter, int *success);\
KeyType const *Name##Iterator_key(CstlIterInternalData pos);\
ValueType *Name##Iterator_val(CstlIterInternalData pos);\
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
static const struct Name##Vtable Name##_vtbl = {\
	Name##_delete,\
	Name##_empty,\
	Name##_size,\
	Name##_clear,\
	Name##_begin,\
	Name##_end,\
	Name##_rbegin,\
	Name##_rend,\
	Name##_assoc_insert_range,\
	Name##_erase,\
	Name##_erase_range,\
	Name##_erase_key,\
	Name##_count,\
	Name##_find,\
	Name##_lower_bound,\
	Name##_upper_bound,\
	Name##_equal_range,\
	Name##_swap,\
	Name##_map_insert,\
	Name##_map_insert_ref,\
	Name##_at,\
};\
\
CSTL_COMMON_MAP_IMPLEMENT(Name, KeyType, ValueType, Compare)	\
\
/*Name##Iterator Name##_insert(Name *self, KeyType key, ValueType value, int *success)*/\
int Name##_map_insert(Name *self, KeyType key, ValueType value, Name##Iterator *iter, int *success)\
{\
	/* TODO */\
	CSTL_ASSERT(self && "Map_insert");\
	CSTL_ASSERT(self->magic == self && "Map_insert");\
	return Name##_insert_ref(self, key, &value, success);\
}\
\
/*Name##Iterator Name##_insert_ref(Name *self, KeyType key, ValueType const *value, int *success)*/\
int Name##_map_insert_ref(Name *self, KeyType key, ValueType const *value, Name##Iterator *iter, int *success)\
{\
	/* TODO */\
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
/*int Name##_insert_range_assoc(Name *self, Name##Iterator first, Name##Iterator last)*/\
int Name##_assoc_insert_range(Name *self, CstlIterInternal first, CstlIterInternal last)\
{\
	/* TODO */\
	register Name##Iterator pos;\
	register Name##Iterator tmp;\
	Name##RBTree head;\
	register size_t count = 0;\
	CSTL_ASSERT(self && "Map_insert_range_assoc");\
	CSTL_ASSERT(self->magic == self && "Map_insert_range_assoc");\
	CSTL_ASSERT(first && "Map_insert_range_assoc");\
	CSTL_ASSERT(last && "Map_insert_range_assoc");\
	CSTL_ASSERT(first->magic && "Map_insert_range_assoc");\
	CSTL_ASSERT(last->magic && "Map_insert_range_assoc");\
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
\
typedef int (*Name##_multimap_insert_t)(Name *self, keyType key, ValueType value, Name##Iterator *iter);\
typedef int (*Name##_multimap_insert_ref_t)(Name *self, keyType key, ValueType const *value, Name##Iterator *iter);\
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
	Name##_assoc_insert_range_t assoc_insert_range;\
	Name##_erase_t        erase;\
	Name##_erase_range_t  erase_range;\
	Name##_erase_key_t    erase_key;\
	Name##_count_t        count;\
	Name##_find_t         find;\
	Name##_lower_bound_t  lower_bound;\
	Name##_upper_bound_t  upper_bound;\
	Name##_equal_range_t  equal_range;\
	Name##_swap_t         swap;\
	Name##_multimap_insert_t multimap_insert;\
	Name##_multimap_insert_ref_t multimap_insert_ref;\
};\
\
int Name##_multimap_insert(Name *self, KeyType key, ValueType value, Name##Iterator *iter);\
int Name##_multimap_insert_ref(Name *self, KeyType key, ValueType const *value, Name##Iterator *iter);\
/*Name##Iterator Name##_insert(Name *self, KeyType key, ValueType value);\
Name##Iterator Name##_insert_ref(Name *self, KeyType key, ValueType const *value);*/\
KeyType const *Name##Iterator_key(Name##Iterator pos);\
ValueType *Name##Iterator_val(Name##Iterator pos);\
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
static const struct Name##Vtable Name##_vtbl = {\
	Name##_delete,\
	Name##_empty,\
	Name##_size,\
	Name##_clear,\
	Name##_begin,\
	Name##_end,\
	Name##_rbegin,\
	Name##_rend,\
	Name##_assoc_insert_range,\
	Name##_erase,\
	Name##_erase_range,\
	Name##_erase_key,\
	Name##_count,\
	Name##_find,\
	Name##_lower_bound,\
	Name##_upper_bound,\
	Name##_equal_range,\
	Name##_swap,\
	Name##_multimap_insert,\
	Name##_multimap_insert_ref,\
};\
\
\
CSTL_COMMON_MAP_IMPLEMENT(Name, KeyType, ValueType, Compare)	\
\
/*Name##Iterator Name##_insert(Name *self, KeyType key, ValueType value)*/\
int Name##_multimap_insert(Name *self, KeyType key, ValueType value, Name##Iterator *iter)\
{\
	/* TODO */\
	CSTL_ASSERT(self && "MultiMap_insert");\
	CSTL_ASSERT(self->magic == self && "MultiMap_insert");\
	return Name##_insert_ref(self, key, &value);\
}\
\
/*Name##Iterator Name##_insert_ref(Name *self, KeyType key, ValueType const *value)*/\
int Name##_multimap_insert_ref(Name *self, KeyType key, ValueType const *value, Name##Iterator *iter)\
{\
	/* TODO */\
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
/*int Name##_insert_range_assoc(Name *self, Name##Iterator first, Name##Iterator last)*/\
int Name##_assoc_insert_range(Name *self, CstlIterInternal first, CstlIterInternal last)\
{\
	/* TODO */\
	register Name##Iterator pos;\
	register Name##Iterator tmp;\
	Name##RBTree head;\
	register size_t count = 0;\
	CSTL_ASSERT(self && "MultiMap_insert_range_assoc");\
	CSTL_ASSERT(self->magic == self && "MultiMap_insert_range_assoc");\
	CSTL_ASSERT(first && "MultiMap_insert_range_assoc");\
	CSTL_ASSERT(last && "MultiMap_insert_range_assoc");\
	CSTL_ASSERT(first->magic && "MultiMap_insert_range_assoc");\
	CSTL_ASSERT(last->magic && "MultiMap_insert_range_assoc");\
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
