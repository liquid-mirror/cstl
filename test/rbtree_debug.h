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
 * \file rbtree_debug.h
 * \brief set/mapデバッグ用
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2006-11-14
 *
 * 赤黒木の表示とベリファイ
 */
#ifndef CSTL_RBTREE_DEBUG_H_INCLUDED
#define CSTL_RBTREE_DEBUG_H_INCLUDED

#include <stdio.h>
#include <limits.h>

enum {
	VERBOSE = 0,
	VISUAL
};

#define RBTREE_DEBUG_IMPLEMENT(Name, KeyType, ValueType, Compare, format1, format2, map, value, visual)	\
\
static size_t Name##_max_depth;\
static size_t Name##_min_depth;\
\
static char *Name##_str_color(RBTreeColor c)\
{\
	char *r = "(r)";\
	char *b = "(b)";\
	if (c == RBTREE_RED) {\
		return r;\
	} else if (c == RBTREE_BLACK) {\
		return b;\
	} else {\
		assert(0);\
		return 0;\
	}\
}\
\
static void Name##_p(Name##RBTreeNode *self, size_t depth)\
{\
	size_t i;\
	if (visual) {\
		for (i = 0; i < depth; i++) {\
			printf("	");\
		}\
		if (map) {\
			printf("%s["#format1"]["#format2"]", Name##_str_color(self->color), self->key, self->value);\
		} else {\
			printf("%s["#format1"]", Name##_str_color(self->color), self->key);\
		}\
	} else {\
		if (map) {\
			printf("n[%p], p[%p], l[%p], r[%p], k["#format1"], v["#format2"], c[%s], d[%d]",\
				self, self->parent, self->left, self->right, self->key, self->value,\
				Name##_str_color(self->color), depth);\
		} else {\
			printf("n[%p], p[%p], l[%p], r[%p], v["#format1"], c[%s], d[%d]",\
				self, self->parent, self->left, self->right, self->key,\
				Name##_str_color(self->color), depth);\
		}\
		if (Name##RBTreeNode_is_root(self)) {\
			printf(", root");\
		} else if (Name##RBTreeNode_is_nil(self->left) && Name##RBTreeNode_is_nil(self->right)) {\
			printf(", leaf");\
		}\
	}\
	printf("\n");\
}\
\
static void Name##Node_print(Name##RBTreeNode *self)\
{\
	static size_t depth = 0;\
	if (Name##RBTreeNode_is_nil(self)) {\
		return;\
	}\
	depth++;\
	if (Name##_max_depth < depth) Name##_max_depth = depth;\
	if (Name##RBTreeNode_is_nil(self->right) && Name##RBTreeNode_is_nil(self->left)) {\
		if (Name##_min_depth > depth) Name##_min_depth = depth;\
	}\
	if (visual) {\
		Name##Node_print(self->right);\
		Name##_p(self, depth);\
		Name##Node_print(self->left);\
	} else {\
		Name##_p(self, depth);\
		Name##Node_print(self->left);\
		Name##Node_print(self->right);\
	}\
	depth--;\
}\
\
void Name##_print(Name *self)\
{\
	Name##RBTreeNode *root;\
	assert(Name##RBTreeNode_is_head(self->tree));\
	root = Name##RBTree_get_root(self->tree);\
	Name##_max_depth = 0;\
	Name##_min_depth = UINT_MAX;\
	Name##Node_print(root);\
	printf("min depth[%d], max depth[%d]\n", Name##_min_depth, Name##_max_depth);\
	printf("\n");\
}\
\
static size_t Name##RBTreeNode_black_count(Name##RBTreeNode *t, Name##RBTreeNode *root)\
{\
	size_t count;\
	if (Name##RBTreeNode_is_nil(t)) {\
		return 0;\
	}\
	count = t->color == RBTREE_BLACK ? 1 : 0;\
	if (t == root) {\
		return count;\
	} else {\
		return count + Name##RBTreeNode_black_count(t->parent, root);\
	}\
}\
\
int Name##_verify(Name *self)\
{\
	size_t len;\
	Name##RBTreeNode *l;\
	Name##RBTreeNode *r;\
	Name##Iterator pos;\
	Name##RBTree *tree = self->tree;\
	if (Name##RBTree_empty(tree) || Name##RBTree_begin(tree) == Name##RBTree_end(tree)) {\
		return Name##RBTree_empty(tree) && Name##RBTree_begin(tree) == Name##RBTree_end(tree) &&\
			tree->left == &Name##RBTree_nil && tree->right == &Name##RBTree_nil && tree->parent == &Name##RBTree_nil;\
	}\
	len = Name##RBTreeNode_black_count(Name##RBTree_begin(tree), Name##RBTree_get_root(tree));\
	for (pos = Name##RBTree_begin(tree); pos != Name##RBTree_end(tree); pos = Name##RBTree_next(pos)) {\
		l = pos->left;\
		r = pos->right;\
		if (pos->color == RBTREE_RED) {\
			if ((!Name##RBTreeNode_is_nil(l) && l->color == RBTREE_RED) ||\
				(!Name##RBTreeNode_is_nil(r) && r->color == RBTREE_RED)) {\
				return 0;\
			}\
		}\
		if (!Name##RBTreeNode_is_nil(l) && Compare(pos->key, l->key) < 0) {\
			return 0;\
		}\
		if (!Name##RBTreeNode_is_nil(r) && Compare(r->key, pos->key) < 0) {\
			return 0;\
		}\
		if (Name##RBTreeNode_is_nil(l) && Name##RBTreeNode_is_nil(r) &&\
			Name##RBTreeNode_black_count(pos, Name##RBTree_get_root(tree)) != len) {\
			return 0;\
		}\
	}\
	return 1;\
}\
\

#define SET_DEBUG_INTERFACE(Name)	\
void Name##_print(Name *self);\
int Name##_verify(Name *self);\

#define MAP_DEBUG_INTERFACE(Name)	\
void Name##_print(Name *self);\
int Name##_verify(Name *self);\

#define SET_DEBUG_IMPLEMENT(Name, Type, Compare, format, visual)	\
RBTREE_DEBUG_IMPLEMENT(Name, Type, Type, Compare, format, format, 0, key, visual)	\

#define MAP_DEBUG_IMPLEMENT(Name, KeyType, ValueType, Compare, format1, format2, visual)	\
RBTREE_DEBUG_IMPLEMENT(Name, KeyType, ValueType, Compare, format1, format2, 1, value, visual)	\

#endif /* CSTL_RBTREE_DEBUG_H_INCLUDED */
