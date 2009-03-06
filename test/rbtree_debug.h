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
 * $URL$
 * $Id$
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

#define CSTL_RBTREE_DEBUG_IMPLEMENT(Name, KeyType, ValueType, Compare, format1, format2, map, value, visual)	\
\
static size_t Name##_max_depth;\
static size_t Name##_min_depth;\
\
static char *Name##_str_color(int c)\
{\
	char *r = "(r)";\
	char *b = "(b)";\
	if (c == CSTL_RBTREE_RED) {\
		return r;\
	} else if (c == CSTL_RBTREE_BLACK) {\
		return b;\
	} else {\
		assert(0);\
		return 0;\
	}\
}\
\
static void Name##RBTree_p(Name##RBTree *node, size_t depth)\
{\
	size_t i;\
	if (visual) {\
		for (i = 0; i < depth; i++) {\
			printf("	");\
		}\
		if (map) {\
			printf("%s["#format1"]["#format2"]", Name##_str_color(node->color), node->key, node->value);\
		} else {\
			printf("%s["#format1"]", Name##_str_color(node->color), node->key);\
		}\
	} else {\
		if (map) {\
			printf("n[%p], p[%p], l[%p], r[%p], k["#format1"], v["#format2"], c[%s], d[%d]",\
				(void *) node, (void *) node->parent, (void *) node->left, (void *) node->right, node->key, node->value,\
				Name##_str_color(node->color), depth);\
		} else {\
			printf("n[%p], p[%p], l[%p], r[%p], v["#format1"], c[%s], d[%d]",\
				(void *) node, (void *) node->parent, (void *) node->left, (void *) node->right, node->key,\
				Name##_str_color(node->color), depth);\
		}\
		if (CSTL_RBTREE_IS_ROOT(node)) {\
			printf(", root");\
		} else if (CSTL_RBTREE_IS_NIL(node->left, Name) && CSTL_RBTREE_IS_NIL(node->right, Name)) {\
			printf(", leaf");\
		}\
	}\
	printf("\n");\
}\
\
static void Name##RBTree_print(Name##RBTree *node)\
{\
	static size_t depth = 0;\
	if (CSTL_RBTREE_IS_NIL(node, Name)) {\
		return;\
	}\
	depth++;\
	if (Name##_max_depth < depth) Name##_max_depth = depth;\
	if (CSTL_RBTREE_IS_NIL(node->right, Name) && CSTL_RBTREE_IS_NIL(node->left, Name)) {\
		if (Name##_min_depth > depth) Name##_min_depth = depth;\
	}\
	if (visual) {\
		Name##RBTree_print(node->right);\
		Name##RBTree_p(node, depth);\
		Name##RBTree_print(node->left);\
	} else {\
		Name##RBTree_p(node, depth);\
		Name##RBTree_print(node->left);\
		Name##RBTree_print(node->right);\
	}\
	depth--;\
}\
\
void Name##_print(Name *self)\
{\
	Name##RBTree *root;\
	assert(CSTL_RBTREE_IS_HEAD(self->tree));\
	root = Name##RBTree_get_root(self->tree);\
	Name##_max_depth = 0;\
	Name##_min_depth = UINT_MAX;\
	Name##RBTree_print(root);\
	printf("min depth[%d], max depth[%d]\n", Name##_min_depth, Name##_max_depth);\
	printf("\n");\
}\
\
static size_t Name##RBTree_black_count(Name##RBTree *t, Name##RBTree *root)\
{\
	size_t count;\
	if (CSTL_RBTREE_IS_NIL(t, Name)) {\
		return 0;\
	}\
	count = t->color == CSTL_RBTREE_BLACK ? 1 : 0;\
	if (t == root) {\
		return count;\
	} else {\
		return count + Name##RBTree_black_count(t->parent, root);\
	}\
}\
\
int Name##_verify(Name *self)\
{\
	size_t len;\
	Name##RBTree *l;\
	Name##RBTree *r;\
	Name##Iterator pos;\
	Name##RBTree *tree = self->tree;\
	if (Name##RBTree_empty(tree) || Name##RBTree_begin(tree) == Name##RBTree_end(tree)) {\
		return Name##RBTree_empty(tree) && Name##RBTree_begin(tree) == Name##RBTree_end(tree) &&\
			tree->left == (Name##RBTree *) &Name##RBTree_nil &&\
			tree->right == (Name##RBTree *) &Name##RBTree_nil &&\
			tree->parent == (Name##RBTree *) &Name##RBTree_nil;\
	}\
	len = Name##RBTree_black_count(Name##RBTree_begin(tree), Name##RBTree_get_root(tree));\
	for (pos = Name##RBTree_begin(tree); pos != Name##RBTree_end(tree); pos = Name##RBTreeIterator_next(pos)) {\
		l = pos->left;\
		r = pos->right;\
		if (pos->color == CSTL_RBTREE_RED) {\
			if ((!CSTL_RBTREE_IS_NIL(l, Name) && l->color == CSTL_RBTREE_RED) ||\
				(!CSTL_RBTREE_IS_NIL(r, Name) && r->color == CSTL_RBTREE_RED)) {\
				return 0;\
			}\
		}\
		if (!CSTL_RBTREE_IS_NIL(l, Name) && Compare(pos->key, l->key) < 0) {\
			return 0;\
		}\
		if (!CSTL_RBTREE_IS_NIL(r, Name) && Compare(r->key, pos->key) < 0) {\
			return 0;\
		}\
		if (CSTL_RBTREE_IS_NIL(l, Name) && CSTL_RBTREE_IS_NIL(r, Name) &&\
			Name##RBTree_black_count(pos, Name##RBTree_get_root(tree)) != len) {\
			return 0;\
		}\
	}\
	return 1;\
}\
\

#define CSTL_SET_DEBUG_INTERFACE(Name)	\
void Name##_print(Name *self);\
int Name##_verify(Name *self);\

#define CSTL_MAP_DEBUG_INTERFACE(Name)	\
void Name##_print(Name *self);\
int Name##_verify(Name *self);\

#define CSTL_SET_DEBUG_IMPLEMENT(Name, Type, Compare, format, visual)	\
CSTL_RBTREE_DEBUG_IMPLEMENT(Name, Type, Type, Compare, format, format, 0, key, visual)	\

#define CSTL_MAP_DEBUG_IMPLEMENT(Name, KeyType, ValueType, Compare, format1, format2, visual)	\
CSTL_RBTREE_DEBUG_IMPLEMENT(Name, KeyType, ValueType, Compare, format1, format2, 1, value, visual)	\

#endif /* CSTL_RBTREE_DEBUG_H_INCLUDED */
