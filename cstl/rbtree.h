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
 * \file rbtree.h
 * \brief 赤黒木
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2006-10-29
 * $URL$
 * $Id$
 *
 * このファイルを直接インクルードしないこと
 */
#ifndef CSTL_RBTREE_H_INCLUDED
#define CSTL_RBTREE_H_INCLUDED

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#ifndef NDEBUG
#define CSTL_RBTREE_MAGIC(x) x
#else
#define CSTL_RBTREE_MAGIC(x)
#endif

/*! 
 * \brief 赤黒木の色
 */
enum {
	CSTL_RBTREE_RED,
	CSTL_RBTREE_BLACK,
	CSTL_RBTREE_HEAD
};

#define CSTL_LESS(x, y)		((x) == (y) ? 0 : (x) < (y) ? -1 : 1)
#define CSTL_GREATER(x, y)	((x) == (y) ? 0 : (x) > (y) ? -1 : 1)


#define CSTL_RBTREE_NODE_IS_HEAD(self)		(self->color == CSTL_RBTREE_HEAD)
#define CSTL_RBTREE_NODE_IS_ROOT(self)		CSTL_RBTREE_NODE_IS_HEAD(self->parent)
#define CSTL_RBTREE_NODE_IS_NIL(self, Name)	(self == &Name##_nil)


#define CSTL_RBTREE_IMPLEMENT(Name, KeyType, ValueType, Compare)	\
\
typedef struct Name##Node_t Name##Node;\
typedef struct Name##Node_t *Name##Iterator;\
static Name##Node Name##_nil = {&Name##_nil, &Name##_nil, &Name##_nil, CSTL_RBTREE_BLACK};\
\
static Name *Name##_new(void);\
static void Name##_delete(Name *self);\
static void Name##_clear(Name *self);\
static int Name##_empty(Name *self);\
static Name##Iterator Name##_insert(Name *self, KeyType key, ValueType value);\
static void Name##_erase(Name *self, Name##Iterator pos);\
static size_t Name##_count(Name *self, KeyType key);\
static Name##Iterator Name##_find(Name *self, KeyType key);\
static Name##Iterator Name##_lower_bound(Name *self, KeyType key);\
static Name##Iterator Name##_upper_bound(Name *self, KeyType key);\
static Name##Iterator Name##_begin(Name *self);\
static Name##Iterator Name##_end(Name *self);\
static Name##Iterator Name##_rbegin(Name *self);\
static Name##Iterator Name##_rend(Name *self);\
static Name##Iterator Name##_next(Name##Iterator pos);\
static Name##Iterator Name##_prev(Name##Iterator pos);\
\
static Name##Node *Name##Node_new(KeyType key, ValueType value, int color);\
static void Name##Node_set_left(Name##Node *self, Name##Node *t);\
static void Name##Node_set_right(Name##Node *self, Name##Node *t);\
static Name##Node *Name##_get_root(Name *self);\
static void Name##_set_root(Name *self, Name##Node *t);\
static Name##Node *Name##Node_find(Name *self, Name##Node *t, KeyType key);\
static Name##Node *Name##Node_replace_subtree(Name##Node *self, Name##Node *t);\
static void Name##Node_swap_parent_child(Name##Node *p, Name##Node *c);\
static void Name##Node_swap(Name##Node *s, Name##Node *t);\
static void Name##Node_rotate_right(Name##Node *self);\
static void Name##Node_rotate_left(Name##Node *self);\
static Name##Node *Name##Node_get_sibling(Name##Node *self);\
static Name##Node *Name##Node_get_uncle(Name##Node *self);\
static void Name##Node_balance_for_insert(Name##Node *n);\
static void Name##Node_balance_for_erase(Name##Node *n, Name##Node *p_of_n);\
\
\
static void Name##Node_set_left(Name##Node *self, Name##Node *t)\
{\
	assert(self && "RBTreeNode_set_left");\
	self->left = t;\
	if (!CSTL_RBTREE_NODE_IS_NIL(t, Name)) {\
		t->parent = self;\
	}\
}\
\
static void Name##Node_set_right(Name##Node *self, Name##Node *t)\
{\
	assert(self && "RBTreeNode_set_right");\
	self->right = t;\
	if (!CSTL_RBTREE_NODE_IS_NIL(t, Name)) {\
		t->parent = self;\
	}\
}\
\
static Name##Node *Name##_get_root(Name *self)\
{\
	assert(CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTree_get_root");\
	return self->right;\
}\
\
static void Name##_set_root(Name *self, Name##Node *t)\
{\
	assert(CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTree_set_root");\
	self->right = t;\
	if (!CSTL_RBTREE_NODE_IS_NIL(t, Name)) {\
		t->parent = self;\
	}\
}\
\
static Name *Name##_new(void)\
{\
	Name *self;\
	self = (Name *) malloc(sizeof(Name));\
	if (!self) return 0;\
	self->left = &Name##_nil;\
	self->right = &Name##_nil;\
	self->parent = &Name##_nil;\
	self->color = CSTL_RBTREE_HEAD;\
	CSTL_RBTREE_MAGIC(self->magic = self;)\
	return self;\
}\
\
static void Name##_clear(Name *self)\
{\
	Name##Node *t;\
	Name##Node *tmp;\
	assert(CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTree_clear");\
	t = Name##_get_root(self);\
	if (CSTL_RBTREE_NODE_IS_NIL(t, Name)) return;\
	while (1) {\
		if (!CSTL_RBTREE_NODE_IS_NIL(t->left, Name)) {\
			t = t->left;\
			continue;\
		}\
		if (!CSTL_RBTREE_NODE_IS_NIL(t->right, Name)) {\
			t = t->right;\
			continue;\
		}\
		if (t == t->parent->left) {\
			t->parent->left = &Name##_nil;\
		} else {\
			t->parent->right = &Name##_nil;\
		}\
		tmp = t->parent;\
		CSTL_RBTREE_MAGIC(t->magic = 0;)\
		free(t);\
		t = tmp;\
		if (CSTL_RBTREE_NODE_IS_HEAD(t)) break;\
	}\
}\
\
static void Name##_delete(Name *self)\
{\
	assert(CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTree_delete");\
	Name##_clear(self);\
	CSTL_RBTREE_MAGIC(self->magic = 0;)\
	free(self);\
}\
\
static int Name##_empty(Name *self)\
{\
	assert(CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTree_empty");\
	return CSTL_RBTREE_NODE_IS_NIL(Name##_get_root(self), Name);\
}\
\
static Name##Node *Name##Node_find(Name *self, Name##Node *t, KeyType key)\
{\
	int cmp;\
	while (!CSTL_RBTREE_NODE_IS_NIL(t, Name)) {\
		cmp = Compare(key, t->key);\
		if (cmp < 0) {\
			t = t->left;\
		} else if (cmp > 0) {\
			t = t->right;\
		} else {\
			break;\
		}\
	}\
	return t;\
}\
\
static Name##Iterator Name##_find(Name *self, KeyType key)\
{\
	Name##Node *t;\
	assert(CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTree_find");\
	t = Name##Node_find(self, Name##_get_root(self), key);\
	return CSTL_RBTREE_NODE_IS_NIL(t, Name) ? Name##_end(self) : t;\
}\
\
static size_t Name##_count(Name *self, KeyType key)\
{\
	Name##Iterator first;\
	Name##Iterator last;\
	Name##Iterator pos;\
	size_t count = 0;\
	assert(CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTree_count");\
	first = Name##_lower_bound(self, key);\
	last = Name##_upper_bound(self, key);\
	for (pos = first; pos != last; pos = Name##_next(pos)) {\
		count++;\
	}\
	return count;\
}\
\
static Name##Iterator Name##_lower_bound(Name *self, KeyType key)\
{\
	Name##Node *t;\
	Name##Node *tmp;\
	assert(CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTree_lower_bound");\
	tmp = Name##_end(self);\
	t = Name##_get_root(self);\
	while (!CSTL_RBTREE_NODE_IS_NIL(t, Name)) {\
		if (Compare(key, t->key) <= 0) {\
			tmp = t;\
			t = t->left;\
		} else {\
			t = t->right;\
		}\
	}\
	return tmp;\
}\
\
static Name##Iterator Name##_upper_bound(Name *self, KeyType key)\
{\
	Name##Node *t;\
	Name##Node *tmp;\
	assert(CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTree_upper_bound");\
	tmp = Name##_end(self);\
	t = Name##_get_root(self);\
	while (!CSTL_RBTREE_NODE_IS_NIL(t, Name)) {\
		if (Compare(key, t->key) < 0) {\
			tmp = t;\
			t = t->left;\
		} else {\
			t = t->right;\
		}\
	}\
	return tmp;\
}\
\
static Name##Node *Name##Node_replace_subtree(Name##Node *self, Name##Node *t)\
{\
	assert(!CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTreeNode_replace_subtree");\
	assert(!CSTL_RBTREE_NODE_IS_HEAD(t) && "RBTreeNode_replace_subtree");\
	if (self->parent->left == self) {\
		Name##Node_set_left(self->parent, t);\
	} else {\
		Name##Node_set_right(self->parent, t);\
	}\
	return self;\
}\
\
static void Name##Node_swap_parent_child(Name##Node *p, Name##Node *c)\
{\
	Name##Node *cl;\
	Name##Node *cr;\
	if (p->parent->left == p) {\
		Name##Node_set_left(p->parent, c);\
	} else {\
		Name##Node_set_right(p->parent, c);\
	}\
	cl = c->left;\
	cr = c->right;\
	if (p->left == c) {\
		Name##Node_set_left(c, p);\
		Name##Node_set_right(c, p->right);\
	} else {\
		Name##Node_set_right(c, p);\
		Name##Node_set_left(c, p->left);\
	}\
	Name##Node_set_left(p, cl);\
	Name##Node_set_right(p, cr);\
}\
\
static void Name##Node_swap(Name##Node *s, Name##Node *t)\
{\
	Name##Node *tp;\
	Name##Node *tl;\
	Name##Node *tr;\
	int c;\
	assert(!CSTL_RBTREE_NODE_IS_HEAD(s) && "RBTreeNode_swap");\
	assert(!CSTL_RBTREE_NODE_IS_HEAD(t) && "RBTreeNode_swap");\
	assert(!CSTL_RBTREE_NODE_IS_NIL(s, Name) && "RBTreeNode_swap");\
	assert(!CSTL_RBTREE_NODE_IS_NIL(t, Name) && "RBTreeNode_swap");\
	if (t->parent == s) {\
		Name##Node_swap_parent_child(s, t);\
	} else if (s->parent == t) {\
		Name##Node_swap_parent_child(t, s);\
	} else {\
		tp = t->parent;\
		tl = t->left;\
		tr = t->right;\
		if (s->parent->left == s) {\
			Name##Node_set_left(s->parent, t);\
		} else {\
			Name##Node_set_right(s->parent, t);\
		}\
		Name##Node_set_left(t, s->left);\
		Name##Node_set_right(t, s->right);\
		if (tp->left == t) {\
			Name##Node_set_left(tp, s);\
		} else {\
			Name##Node_set_right(tp, s);\
		}\
		Name##Node_set_left(s, tl);\
		Name##Node_set_right(s, tr);\
	}\
	c = s->color;\
	s->color = t->color;\
	t->color = c;\
}\
\
static void Name##Node_rotate_right(Name##Node *self)\
{\
	Name##Node *p;\
	Name##Node *n;\
	assert(!CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTreeNode_rotate_right");\
	assert(!CSTL_RBTREE_NODE_IS_NIL(self, Name) && "RBTreeNode_rotate_right");\
	p = self->parent;\
	n = self->left;\
	assert(!CSTL_RBTREE_NODE_IS_NIL(n, Name) && "RBTreeNode_rotate_right");\
	Name##Node_set_left(self, n->right);\
	Name##Node_set_right(n, self);\
	if (p->left == self) {\
		Name##Node_set_left(p, n);\
	} else {\
		Name##Node_set_right(p, n);\
	}\
}\
\
static void Name##Node_rotate_left(Name##Node *self)\
{\
	Name##Node *p;\
	Name##Node *n;\
	assert(!CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTreeNode_rotate_left");\
	assert(!CSTL_RBTREE_NODE_IS_NIL(self, Name) && "RBTreeNode_rotate_left");\
	p = self->parent;\
	n = self->right;\
	assert(!CSTL_RBTREE_NODE_IS_NIL(n, Name) && "RBTreeNode_rotate_left");\
	Name##Node_set_right(self, n->left);\
	Name##Node_set_left(n, self);\
	if (p->left == self) {\
		Name##Node_set_left(p, n);\
	} else {\
		Name##Node_set_right(p, n);\
	}\
}\
\
static Name##Node *Name##Node_get_sibling(Name##Node *self)\
{\
	assert(!CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTreeNode_get_sibling");\
	assert(!CSTL_RBTREE_NODE_IS_NIL(self, Name) && "RBTreeNode_get_sibling");\
	assert(!CSTL_RBTREE_NODE_IS_ROOT(self) && "RBTreeNode_get_sibling");\
	return (self->parent->left == self) ? \
		self->parent->right : self->parent->left;\
}\
\
static Name##Node *Name##Node_get_uncle(Name##Node *self)\
{\
	Name##Node *g;\
	assert(!CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTreeNode_get_uncle");\
	assert(!CSTL_RBTREE_NODE_IS_NIL(self, Name) && "RBTreeNode_get_uncle");\
	assert(!CSTL_RBTREE_NODE_IS_ROOT(self) && "RBTreeNode_get_uncle");\
	assert(!CSTL_RBTREE_NODE_IS_ROOT(self->parent) && "RBTreeNode_get_uncle");\
	g = self->parent->parent;\
	assert(!CSTL_RBTREE_NODE_IS_NIL(g, Name) && "RBTreeNode_get_uncle");\
	assert(!CSTL_RBTREE_NODE_IS_HEAD(g) && "RBTreeNode_get_uncle");\
	return (g->left == self->parent) ?\
		g->right : g->left;\
}\
\
static void Name##Node_balance_for_insert(Name##Node *n)\
{\
	Name##Node *p;\
	Name##Node *g;\
	Name##Node *u;\
	Name##Node *tmp;\
	while (1) {\
		p = n->parent;\
		if (CSTL_RBTREE_NODE_IS_ROOT(n)) {\
			/* case 1 nがroot */\
			n->color = CSTL_RBTREE_BLACK;\
			break;\
		}\
		if (p->color == CSTL_RBTREE_BLACK) {\
			/* case 2 pが黒 */\
			break;\
		}\
		/* 以下、pは赤、gは黒 */\
		g = p->parent;\
		u = Name##Node_get_uncle(n);\
		if (u->color == CSTL_RBTREE_RED) {\
			/* case 3 uが赤 */\
			g->color = CSTL_RBTREE_RED;\
			p->color = CSTL_RBTREE_BLACK;\
			u->color = CSTL_RBTREE_BLACK;\
			n = g;\
			/* gをnにして再調整。最悪の場合、根まで伝播 */\
			continue;\
		}\
		/* 以下、uは黒 */\
		if (g->left == p && p->right == n) {\
			/* case 4 pがgのleft */\
			Name##Node_rotate_left(p);\
			tmp = p;\
			p = n;\
			n = tmp;\
			/* case 5 leftへ */\
		} else if (g->right == p && p->left == n) {\
			/* case 4 pがgのright */\
			Name##Node_rotate_right(p);\
			tmp = p;\
			p = n;\
			n = tmp;\
			/* case 5 rightへ */\
		}\
		if (g->left == p) {\
			/* case 5 left */\
			Name##Node_rotate_right(g);\
		} else if (g->right == p) {\
			/* case 5 right */\
			Name##Node_rotate_left(g);\
		} else {\
			assert(0 && "RBTreeNode_balance_for_insert");\
		}\
		p->color = CSTL_RBTREE_BLACK;\
		g->color = CSTL_RBTREE_RED;\
		break;\
	}\
}\
\
static Name##Iterator Name##_insert(Name *self, KeyType key, ValueType value)\
{\
	Name##Node *n;\
	Name##Node *tmp;\
	assert(CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTree_insert");\
	n = Name##_get_root(self);\
	if (CSTL_RBTREE_NODE_IS_NIL(n, Name)) {\
		/* rootになる */\
		Name##Node *root = Name##Node_new(key, value, CSTL_RBTREE_BLACK);\
		if (!root) return 0;	/* メモリ不足 */\
		Name##_set_root(self, root);\
		CSTL_RBTREE_MAGIC(root->magic = self;)\
		return root;\
	}\
	/* 2分探索木の挿入 */\
	while (!CSTL_RBTREE_NODE_IS_NIL(n, Name)) {\
		tmp = n;\
		if (Compare(key, n->key) < 0) {\
			n = n->left;\
		} else {\
			/* 同じ値なら右へ */\
			n = n->right;\
		}\
	}\
	n = Name##Node_new(key, value, CSTL_RBTREE_RED);\
	if (!n) return 0;	/* メモリ不足 */\
	if (Compare(key, tmp->key) < 0) {\
		Name##Node_set_left(tmp, n);\
	} else {\
		Name##Node_set_right(tmp, n);\
	}\
	Name##Node_balance_for_insert(n);\
	CSTL_RBTREE_MAGIC(n->magic = self;)\
	return n;\
}\
\
static void Name##Node_balance_for_erase(Name##Node *n, Name##Node *p_of_n)\
{\
	Name##Node *p;\
	Name##Node *s;\
	Name##Node *sl;\
	Name##Node *sr;\
	int c;\
	if (n->color == CSTL_RBTREE_RED) {\
		/* case 0 nが赤 */\
		n->color = CSTL_RBTREE_BLACK;\
		return;\
	}\
	while (1) {\
		if (CSTL_RBTREE_NODE_IS_ROOT(n)) {\
			/* case 1 nがroot */\
			n->color = CSTL_RBTREE_BLACK;\
			break;\
		}\
		if (CSTL_RBTREE_NODE_IS_NIL(n, Name)) {\
			assert(!(CSTL_RBTREE_NODE_IS_NIL(p_of_n->left, Name) && CSTL_RBTREE_NODE_IS_NIL(p_of_n->right, Name)) && "RBTreeNode_balance_for_erase");\
			p = p_of_n;\
			s = (n == p_of_n->left) ? p_of_n->right : p_of_n->left;\
		} else {\
			p = n->parent;\
			s = Name##Node_get_sibling(n);\
		}\
		assert(!CSTL_RBTREE_NODE_IS_NIL(s, Name) && "RBTreeNode_balance_for_erase");\
		if (s->color == CSTL_RBTREE_RED) {\
			/* case 2 sが赤 */\
			p->color = CSTL_RBTREE_RED;\
			s->color = CSTL_RBTREE_BLACK;\
			if (p->left == n) {\
				Name##Node_rotate_left(p);\
				s = p->right;\
			} else {\
				Name##Node_rotate_right(p);\
				s = p->left;\
			}\
		}\
		/* 以下、sは黒 */\
		assert(!CSTL_RBTREE_NODE_IS_NIL(s, Name) && "RBTreeNode_balance_for_erase");\
		sl = s->left;\
		sr = s->right;\
		if (p->color == CSTL_RBTREE_BLACK && sl->color == CSTL_RBTREE_BLACK && sr->color == CSTL_RBTREE_BLACK) {\
			/* case 3 */\
			s->color = CSTL_RBTREE_RED;\
			n = p;\
			/* pをnにして再調整。最悪の場合、根まで伝播 */\
			continue;\
		}\
		if (p->color == CSTL_RBTREE_RED && sl->color == CSTL_RBTREE_BLACK && sr->color == CSTL_RBTREE_BLACK) {\
			/* case 4 */\
			p->color = CSTL_RBTREE_BLACK;\
			s->color = CSTL_RBTREE_RED;\
			break;\
		}\
		if (p->left == n && sl->color == CSTL_RBTREE_RED && sr->color == CSTL_RBTREE_BLACK) {\
			/* case 5 nがpのleft */\
			sl->color = CSTL_RBTREE_BLACK;\
			s->color = CSTL_RBTREE_RED;\
			Name##Node_rotate_right(s);\
			sr = s;\
			s = sl;\
			/* case 6 leftへ */\
		} else if (p->right == n && sl->color == CSTL_RBTREE_BLACK && sr->color == CSTL_RBTREE_RED) {\
			/* case 5 nがpのright */\
			sr->color = CSTL_RBTREE_BLACK;\
			s->color = CSTL_RBTREE_RED;\
			Name##Node_rotate_left(s);\
			sl = s;\
			s = sr;\
			/* case 6 rightへ */\
		}\
		if (p->left == n && sr->color == CSTL_RBTREE_RED) {\
			/* case 6 left */\
			sr->color = CSTL_RBTREE_BLACK;\
			Name##Node_rotate_left(p);\
		} else if (p->right == n && sl->color == CSTL_RBTREE_RED) {\
			/* case 6 right */\
			sl->color = CSTL_RBTREE_BLACK;\
			Name##Node_rotate_right(p);\
		} else {\
			assert(0 && "RBTreeNode_balance_for_erase");\
		}\
		c = p->color;\
		p->color = s->color;\
		s->color = c;\
		break;\
	}\
}\
\
static void Name##_erase(Name *self, Name##Iterator pos)\
{\
	Name##Node *n;\
	Name##Node *x;\
	assert(CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTree_erase");\
	assert(!CSTL_RBTREE_NODE_IS_HEAD(pos) && "RBTree_erase");\
	n = pos;\
	assert(!CSTL_RBTREE_NODE_IS_NIL(n, Name) && "RBTree_erase");\
	if (CSTL_RBTREE_NODE_IS_NIL(n->left, Name) && CSTL_RBTREE_NODE_IS_NIL(n->right, Name)) {\
		if (CSTL_RBTREE_NODE_IS_ROOT(n)) {\
			/* 最後の一つを削除 */\
			Name##_set_root(self, &Name##_nil);\
		} else {\
			n = Name##Node_replace_subtree(n, &Name##_nil);\
			if (n->color == CSTL_RBTREE_BLACK) {\
				Name##Node_balance_for_erase(&Name##_nil, n->parent);\
			}\
		}\
		goto end;\
	}\
	if (CSTL_RBTREE_NODE_IS_NIL(n->left, Name)) {\
		n = Name##Node_replace_subtree(n, n->right);\
		if (n->color == CSTL_RBTREE_BLACK) {\
			assert(!CSTL_RBTREE_NODE_IS_NIL(n->right, Name) && "RBTree_erase");\
			Name##Node_balance_for_erase(n->right, 0);\
		}\
		goto end;\
	}\
	if (CSTL_RBTREE_NODE_IS_NIL(n->right, Name)) {\
		n = Name##Node_replace_subtree(n, n->left);\
		if (n->color == CSTL_RBTREE_BLACK) {\
			assert(!CSTL_RBTREE_NODE_IS_NIL(n->left, Name) && "RBTree_erase");\
			Name##Node_balance_for_erase(n->left, 0);\
		}\
		goto end;\
	}\
	assert(!(CSTL_RBTREE_NODE_IS_NIL(n->left, Name) || CSTL_RBTREE_NODE_IS_NIL(n->right, Name)) && "RBTree_erase");\
	x = n->left;\
	while (!CSTL_RBTREE_NODE_IS_NIL(x->right, Name)) {\
		x = x->right;\
	}\
	Name##Node_swap(n, x);\
	n = Name##Node_replace_subtree(n, n->left);\
	if (n->color == CSTL_RBTREE_BLACK) {\
		assert(!CSTL_RBTREE_NODE_IS_NIL(n, Name) && "RBTree_erase");\
		Name##Node_balance_for_erase(n->left, n->parent);\
	}\
end:\
	CSTL_RBTREE_MAGIC(n->magic = 0;)\
	free(n);\
}\
\
static Name##Iterator Name##_begin(Name *self)\
{\
	Name##Node *t;\
	Name##Node *tmp;\
	assert(CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTree_begin");\
	tmp = Name##_end(self);\
	t = Name##_get_root(self);\
	while (!CSTL_RBTREE_NODE_IS_NIL(t, Name)) {\
		tmp = t;\
		t = t->left;\
	}\
	return tmp;\
}\
\
static Name##Iterator Name##_end(Name *self)\
{\
	assert(CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTree_end");\
	return self;\
}\
\
static Name##Iterator Name##_rbegin(Name *self)\
{\
	Name##Node *t;\
	Name##Node *tmp;\
	assert(CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTree_rbegin");\
	tmp = Name##_rend(self);\
	t = Name##_get_root(self);\
	while (!CSTL_RBTREE_NODE_IS_NIL(t, Name)) {\
		tmp = t;\
		t = t->right;\
	}\
	return tmp;\
}\
\
static Name##Iterator Name##_rend(Name *self)\
{\
	assert(CSTL_RBTREE_NODE_IS_HEAD(self) && "RBTree_rend");\
	return self;\
}\
\
static Name##Iterator Name##_next(Name##Iterator pos)\
{\
	assert(!CSTL_RBTREE_NODE_IS_HEAD(pos) && "RBTree_next");\
	assert(!CSTL_RBTREE_NODE_IS_NIL(pos, Name) && "RBTree_next");\
	/* 下位検索 */\
	if (!CSTL_RBTREE_NODE_IS_NIL(pos->right, Name)) {\
		pos = pos->right;\
		while (!CSTL_RBTREE_NODE_IS_NIL(pos->left, Name)) {\
			pos = pos->left;\
		}\
		return pos;\
	}\
	/* 上位検索 */\
	while (!CSTL_RBTREE_NODE_IS_ROOT(pos) && pos == pos->parent->right) {\
		pos = pos->parent;\
	}\
	/* 引数のposがrbegin()の時、pos->parentはend()となる。 */\
	return pos->parent;\
}\
\
static Name##Iterator Name##_prev(Name##Iterator pos)\
{\
	assert(!CSTL_RBTREE_NODE_IS_HEAD(pos) && "RBTree_prev");\
	assert(!CSTL_RBTREE_NODE_IS_NIL(pos, Name) && "RBTree_prev");\
	/* 下位検索 */\
	if (!CSTL_RBTREE_NODE_IS_NIL(pos->left, Name)) {\
		pos = pos->left;\
		while (!CSTL_RBTREE_NODE_IS_NIL(pos->right, Name)) {\
			pos = pos->right;\
		}\
		return pos;\
	}\
	/* 上位検索 */\
	while (!CSTL_RBTREE_NODE_IS_ROOT(pos) && pos == pos->parent->left) {\
		pos = pos->parent;\
	}\
	/* 引数のposがbegin()の時、pos->parentはrend()となる。 */\
	return pos->parent;\
}\
\



#define CSTL_RBTREE_WRAPPER_INTERFACE(Name, KeyType, ValueType)	\
\
typedef struct Name##_t Name;\
typedef struct Name##RBTreeNode_t *Name##Iterator;\
\
Name *Name##_new(void);\
void Name##_delete(Name *self);\
int Name##_assign(Name *self, Name##Iterator first, Name##Iterator last);\
void Name##_clear(Name *self);\
int Name##_empty(Name *self);\
size_t Name##_size(Name *self);\
Name##Iterator Name##_erase(Name *self, Name##Iterator pos);\
Name##Iterator Name##_erase_range(Name *self, Name##Iterator first, Name##Iterator last);\
size_t Name##_erase_key(Name *self, KeyType key);\
size_t Name##_count(Name *self, KeyType key);\
Name##Iterator Name##_find(Name *self, KeyType key);\
Name##Iterator Name##_lower_bound(Name *self, KeyType key);\
Name##Iterator Name##_upper_bound(Name *self, KeyType key);\
Name##Iterator Name##_begin(Name *self);\
Name##Iterator Name##_end(Name *self);\
Name##Iterator Name##_rbegin(Name *self);\
Name##Iterator Name##_rend(Name *self);\
Name##Iterator Name##_next(Name##Iterator pos);\
Name##Iterator Name##_prev(Name##Iterator pos);\
KeyType Name##_key(Name##Iterator pos);\
void Name##_swap(Name *self, Name *x);\


#define CSTL_RBTREE_WRAPPER_IMPLEMENT(Name, KeyType, ValueType, Compare)	\
\
typedef struct Name##RBTreeNode_t Name##RBTree;\
/*! 
 * \brief set/map構造体
 */\
struct Name##_t {\
	Name##RBTree *tree;\
	size_t nelems;\
	CSTL_RBTREE_MAGIC(Name *magic;)\
};\
\
CSTL_RBTREE_IMPLEMENT(Name##RBTree, KeyType, ValueType, Compare)\
\
Name *Name##_new(void)\
{\
	Name *self;\
	self = (Name *) malloc(sizeof(Name));\
	if (!self) return 0;\
	self->tree = Name##RBTree_new();\
	if (!self->tree) {\
		free(self);\
		return 0;\
	}\
	self->nelems = 0;\
	CSTL_RBTREE_MAGIC(self->magic = self;)\
	return self;\
}\
\
void Name##_delete(Name *self)\
{\
	assert(self && "RBTree_delete");\
	assert(self->magic == self && "RBTree_delete");\
	Name##RBTree_delete(self->tree);\
	CSTL_RBTREE_MAGIC(self->magic = 0;)\
	free(self);\
}\
\
void Name##_clear(Name *self)\
{\
	assert(self && "RBTree_clear");\
	assert(self->magic == self && "RBTree_clear");\
	Name##RBTree_clear(self->tree);\
	self->nelems = 0;\
}\
\
int Name##_empty(Name *self)\
{\
	assert(self && "RBTree_empty");\
	assert(self->magic == self && "RBTree_empty");\
	return Name##RBTree_empty(self->tree);\
}\
\
size_t Name##_size(Name *self)\
{\
	assert(self && "RBTree_size");\
	assert(self->magic == self && "RBTree_size");\
	return self->nelems;\
}\
\
Name##Iterator Name##_erase(Name *self, Name##Iterator pos)\
{\
	Name##Iterator tmp;\
	assert(self && "RBTree_erase");\
	assert(self->magic == self && "RBTree_erase");\
	assert(pos && "RBTree_erase");\
	assert(pos != self->tree && "RBTree_erase");\
	assert(pos->magic == self->tree && "RBTree_erase");\
	tmp = Name##_next(pos);\
	Name##RBTree_erase(self->tree, pos);\
	self->nelems--;\
	return tmp;\
}\
\
Name##Iterator Name##_erase_range(Name *self, Name##Iterator first, Name##Iterator last)\
{\
	Name##Iterator pos;\
	assert(self && "RBTree_erase_range");\
	assert(self->magic == self && "RBTree_erase_range");\
	assert(first && "RBTree_erase_range");\
	assert(last && "RBTree_erase_range");\
	assert(first->magic == self->tree && "RBTree_erase_range");\
	assert(last->magic == self->tree && "RBTree_erase_range");\
	pos = first;\
	while (pos != last) {\
		assert(!Name##_empty(self) && "RBTree_erase_range");\
		pos = Name##_erase(self, pos);\
	}\
	return pos;\
}\
\
size_t Name##_erase_key(Name *self, KeyType key)\
{\
	size_t count = 0;\
	Name##Iterator pos;\
	Name##Iterator last;\
	assert(self && "RBTree_erase_key");\
	assert(self->magic == self && "RBTree_erase_key");\
	pos = Name##_lower_bound(self, key);\
	last = Name##_upper_bound(self, key);\
	while (pos != last) {\
		assert(!Name##_empty(self) && "RBTree_erase_key");\
		pos = Name##_erase(self, pos);\
		count++;\
	}\
	return count;\
}\
\
size_t Name##_count(Name *self, KeyType key)\
{\
	assert(self && "RBTree_count");\
	assert(self->magic == self && "RBTree_count");\
	return Name##RBTree_count(self->tree, key);\
}\
\
Name##Iterator Name##_find(Name *self, KeyType key)\
{\
	assert(self && "RBTree_find");\
	assert(self->magic == self && "RBTree_find");\
	return Name##RBTree_find(self->tree, key);\
}\
\
Name##Iterator Name##_lower_bound(Name *self, KeyType key)\
{\
	assert(self && "RBTree_lower_bound");\
	assert(self->magic == self && "RBTree_lower_bound");\
	return Name##RBTree_lower_bound(self->tree, key);\
}\
\
Name##Iterator Name##_upper_bound(Name *self, KeyType key)\
{\
	assert(self && "RBTree_upper_bound");\
	assert(self->magic == self && "RBTree_upper_bound");\
	return Name##RBTree_upper_bound(self->tree, key);\
}\
\
Name##Iterator Name##_begin(Name *self)\
{\
	assert(self && "RBTree_begin");\
	assert(self->magic == self && "RBTree_begin");\
	return Name##RBTree_begin(self->tree);\
}\
\
Name##Iterator Name##_end(Name *self)\
{\
	assert(self && "RBTree_end");\
	assert(self->magic == self && "RBTree_end");\
	return Name##RBTree_end(self->tree);\
}\
\
Name##Iterator Name##_rbegin(Name *self)\
{\
	assert(self && "RBTree_rbegin");\
	assert(self->magic == self && "RBTree_rbegin");\
	return Name##RBTree_rbegin(self->tree);\
}\
\
Name##Iterator Name##_rend(Name *self)\
{\
	assert(self && "RBTree_rend");\
	assert(self->magic == self && "RBTree_rend");\
	return Name##RBTree_rend(self->tree);\
}\
\
Name##Iterator Name##_next(Name##Iterator pos)\
{\
	assert(pos && "RBTree_next");\
	assert(pos->magic && "RBTree_next");\
	return Name##RBTree_next(pos);\
}\
\
Name##Iterator Name##_prev(Name##Iterator pos)\
{\
	assert(pos && "RBTree_prev");\
	assert(pos->magic && "RBTree_prev");\
	return Name##RBTree_prev(pos);\
}\
\
KeyType Name##_key(Name##Iterator pos)\
{\
	assert(pos && "RBTree_key");\
	assert(pos->magic && "RBTree_key");\
	assert(!CSTL_RBTREE_NODE_IS_HEAD(pos) && "RBTree_key");\
	return pos->key;\
}\
\
void Name##_swap(Name *self, Name *x)\
{\
	Name##RBTree *tmp_tree;\
	size_t tmp_nelems;\
	assert(self && "RBTree_swap");\
	assert(x && "RBTree_swap");\
	assert(self->magic == self && "RBTree_swap");\
	assert(x->magic == x && "RBTree_swap");\
	tmp_tree = self->tree;\
	tmp_nelems = self->nelems;\
	self->tree = x->tree;\
	self->nelems = x->nelems;\
	x->tree = tmp_tree;\
	x->nelems = tmp_nelems;\
}\
\


#endif /* CSTL_RBTREE_H_INCLUDED */
