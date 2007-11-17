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
 * \file list.h
 * \brief listコンテナ
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2006-02-26
 * $URL$
 * $Id$
 */
#ifndef CSTL_LIST_H_INCLUDED
#define CSTL_LIST_H_INCLUDED

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#ifdef __cplusplus
#define CSTL_LIST_BEGIN_EXTERN_C()	extern "C" {
#define CSTL_LIST_END_EXTERN_C()	}
#else
#define CSTL_LIST_BEGIN_EXTERN_C()
#define CSTL_LIST_END_EXTERN_C()
#endif

#ifndef NDEBUG
#define CSTL_LIST_MAGIC(x) x
#else
#define CSTL_LIST_MAGIC(x)
#endif


#define CSTL_LIST_BEGIN(self)	self->terminator->next
#define CSTL_LIST_END(self)		self->terminator
#define CSTL_LIST_RBEGIN(self)	self->terminator->prev
#define CSTL_LIST_REND(self)	self->terminator
#define CSTL_LIST_NEXT(pos)		pos->next
#define CSTL_LIST_PREV(pos)		pos->prev
#define CSTL_LIST_AT(pos)		pos->elem


/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_LIST_INTERFACE(Name, Type)	\
typedef struct Name##_t Name;\
/*! 
 * \brief イテレータ
 */\
typedef struct Name##Node_t *Name##Iterator;\
\
CSTL_LIST_BEGIN_EXTERN_C()\
Name *Name##_new(void);\
void Name##_delete(Name *self);\
int Name##_push_back(Name *self, Type elem);\
int Name##_push_front(Name *self, Type elem);\
Type Name##_pop_front(Name *self);\
Type Name##_pop_back(Name *self);\
int Name##_empty(Name *self);\
size_t Name##_size(Name *self);\
void Name##_clear(Name *self);\
Type *Name##_at(Name##Iterator pos);\
Type Name##_front(Name *self);\
Type Name##_back(Name *self);\
Name##Iterator Name##_begin(Name *self);\
Name##Iterator Name##_end(Name *self);\
Name##Iterator Name##_rbegin(Name *self);\
Name##Iterator Name##_rend(Name *self);\
Name##Iterator Name##_next(Name##Iterator pos);\
Name##Iterator Name##_prev(Name##Iterator pos);\
Name##Iterator Name##_insert(Name *self, Name##Iterator pos, Type elem);\
int Name##_insert_n(Name *self, Name##Iterator pos, size_t n, Type elem);\
int Name##_insert_array(Name *self, Name##Iterator pos, const Type *elems, size_t n);\
int Name##_insert_range(Name *self, Name##Iterator pos, Name##Iterator first, Name##Iterator last);\
Name##Iterator Name##_erase(Name *self, Name##Iterator pos);\
Name##Iterator Name##_erase_range(Name *self, Name##Iterator first, Name##Iterator last);\
int Name##_resize(Name *self, size_t n, Type elem);\
void Name##_swap(Name *self, Name *x);\
void Name##_splice(Name *self, Name##Iterator pos, Name *x, Name##Iterator first, Name##Iterator last);\
void Name##_sort(Name *self, int (*comp)(const void *, const void *));\
void Name##_merge(Name *self, Name *x, int (*comp)(const void *, const void *));\
void Name##_reverse(Name *self);\
CSTL_LIST_END_EXTERN_C()\


/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_LIST_IMPLEMENT(Name, Type)	\
typedef struct Name##Node_t Name##Node;\
/*! 
 * \brief listノード構造体
 */\
struct Name##Node_t {\
	Name##Node *prev;\
	Name##Node *next;\
	Type elem;\
	CSTL_LIST_MAGIC(Name##Node *magic);\
};\
\
/*! 
 * \brief list構造体
 */\
struct Name##_t {\
	Name##Node *terminator;\
	size_t nelems;\
	CSTL_LIST_MAGIC(Name *magic);\
};\
\
Name *Name##_new(void)\
{\
	Name *self = (Name *) malloc(sizeof(Name));\
	if (!self) return 0;\
	self->terminator = (Name##Node *) malloc(sizeof(Name##Node));\
	if (!self->terminator) {\
		free(self);\
		return 0;\
	}\
	self->terminator->next = self->terminator;\
	self->terminator->prev = self->terminator;\
	self->nelems = 0;\
	CSTL_LIST_MAGIC(self->terminator->magic = self->terminator);\
	CSTL_LIST_MAGIC(self->magic = self);\
	return self;\
}\
\
void Name##_delete(Name *self)\
{\
	assert(self && "List_delete");\
	assert(self->magic == self && "List_delete");\
	Name##_clear(self);\
	CSTL_LIST_MAGIC(self->terminator->magic = 0);\
	CSTL_LIST_MAGIC(self->magic = 0);\
	free(self->terminator);\
	free(self);\
}\
\
int Name##_push_back(Name *self, Type elem)\
{\
	Name##Node *node;\
	Name##Node *pos;\
	assert(self && "List_push_back");\
	assert(self->magic == self && "List_push_back");\
	node = (Name##Node *) malloc(sizeof(Name##Node));\
	if (!node) return 0;\
	pos = CSTL_LIST_END(self);\
	node->elem = elem;\
	node->next = pos;\
	node->prev = pos->prev;\
	pos->prev = node;\
	node->prev->next = node;\
	self->nelems++;\
	CSTL_LIST_MAGIC(node->magic = self->terminator);\
	return 1;\
}\
\
int Name##_push_front(Name *self, Type elem)\
{\
	Name##Node *node;\
	Name##Node *pos;\
	assert(self && "List_push_front");\
	assert(self->magic == self && "List_push_front");\
	node = (Name##Node *) malloc(sizeof(Name##Node));\
	if (!node) return 0;\
	pos = CSTL_LIST_BEGIN(self);\
	node->elem = elem;\
	node->next = pos;\
	node->prev = pos->prev;\
	pos->prev = node;\
	node->prev->next = node;\
	self->nelems++;\
	CSTL_LIST_MAGIC(node->magic = self->terminator);\
	return 1;\
}\
\
Type Name##_pop_front(Name *self)\
{\
	Type elem;\
	assert(self && "List_pop_front");\
	assert(self->magic == self && "List_pop_front");\
	assert(!Name##_empty(self) && "List_pop_front");\
	elem = CSTL_LIST_AT(CSTL_LIST_BEGIN(self));\
	Name##_erase(self, CSTL_LIST_BEGIN(self));\
	return elem;\
}\
\
Type Name##_pop_back(Name *self)\
{\
	Type elem;\
	assert(self && "List_pop_back");\
	assert(self->magic == self && "List_pop_back");\
	assert(!Name##_empty(self) && "List_pop_back");\
	elem = CSTL_LIST_AT(CSTL_LIST_RBEGIN(self));\
	Name##_erase(self, CSTL_LIST_RBEGIN(self));\
	return elem;\
}\
\
int Name##_empty(Name *self)\
{\
	assert(self && "List_empty");\
	assert(self->magic == self && "List_empty");\
	return self->terminator->next == self->terminator;\
}\
\
size_t Name##_size(Name *self)\
{\
	assert(self && "List_size");\
	assert(self->magic == self && "List_size");\
	return self->nelems;\
}\
\
void Name##_clear(Name *self)\
{\
	assert(self && "List_clear");\
	assert(self->magic == self && "List_clear");\
	Name##_erase_range(self, CSTL_LIST_BEGIN(self), CSTL_LIST_END(self));\
}\
\
Type *Name##_at(Name##Iterator pos)\
{\
	assert(pos && "List_at");\
	assert(pos->magic && "List_at");\
	return &CSTL_LIST_AT(pos);\
}\
\
Type Name##_front(Name *self)\
{\
	assert(self && "List_front");\
	assert(self->magic == self && "List_front");\
	assert(!Name##_empty(self) && "List_front");\
	return self->terminator->next->elem;\
}\
\
Type Name##_back(Name *self)\
{\
	assert(self && "List_back");\
	assert(self->magic == self && "List_back");\
	assert(!Name##_empty(self) && "List_back");\
	return self->terminator->prev->elem;\
}\
\
Name##Iterator Name##_begin(Name *self)\
{\
	assert(self && "List_begin");\
	assert(self->magic == self && "List_begin");\
	return CSTL_LIST_BEGIN(self);\
}\
\
Name##Iterator Name##_end(Name *self)\
{\
	assert(self && "List_end");\
	assert(self->magic == self && "List_end");\
	return CSTL_LIST_END(self);\
}\
\
Name##Iterator Name##_rbegin(Name *self)\
{\
	assert(self && "List_rbegin");\
	assert(self->magic == self && "List_rbegin");\
	return CSTL_LIST_RBEGIN(self);\
}\
\
Name##Iterator Name##_rend(Name *self)\
{\
	assert(self && "List_rend");\
	assert(self->magic == self && "List_rend");\
	return CSTL_LIST_REND(self);\
}\
\
Name##Iterator Name##_next(Name##Iterator pos)\
{\
	assert(pos && "List_next");\
	assert(pos->magic && "List_next");\
	return CSTL_LIST_NEXT(pos);\
}\
\
Name##Iterator Name##_prev(Name##Iterator pos)\
{\
	assert(pos && "List_prev");\
	assert(pos->magic && "List_prev");\
	return CSTL_LIST_PREV(pos);\
}\
\
Name##Iterator Name##_insert(Name *self, Name##Iterator pos, Type elem)\
{\
	Name##Node *node;\
	assert(self && "List_insert");\
	assert(self->magic == self && "List_insert");\
	assert(pos && "List_insert");\
	assert(pos->magic == self->terminator && "List_insert");\
	node = (Name##Node *) malloc(sizeof(Name##Node));\
	if (!node) return 0;\
	node->elem = elem;\
	node->next = pos;\
	node->prev = pos->prev;\
	pos->prev = node;\
	node->prev->next = node;\
	self->nelems++;\
	CSTL_LIST_MAGIC(node->magic = self->terminator);\
	return node;\
}\
\
int Name##_insert_n(Name *self, Name##Iterator pos, size_t n, Type elem)\
{\
	Name *x;\
	register size_t i;\
	assert(self && "List_insert_n");\
	assert(self->magic == self && "List_insert_n");\
	assert(pos && "List_insert_n");\
	assert(pos->magic == self->terminator && "List_insert_n");\
	x = Name##_new();\
	if (!x) return 0;\
	for (i = 0; i < n; i++) {\
		if (!Name##_push_back(x, elem)) {\
			Name##_delete(x);\
			return 0;\
		}\
	}\
	Name##_splice(self, pos, x, CSTL_LIST_BEGIN(x), CSTL_LIST_END(x));\
	Name##_delete(x);\
	return 1;\
}\
\
int Name##_insert_array(Name *self, Name##Iterator pos, const Type *elems, size_t n)\
{\
	Name *x;\
	register size_t i;\
	assert(self && "List_insert_array");\
	assert(self->magic == self && "List_insert_array");\
	assert(pos && "List_insert_array");\
	assert(pos->magic == self->terminator && "List_insert_array");\
	assert(elems && "List_insert_array");\
	x = Name##_new();\
	if (!x) return 0;\
	for (i = 0; i < n; i++) {\
		if (!Name##_push_back(x, ((Type *) elems)[i])) {\
			Name##_delete(x);\
			return 0;\
		}\
	}\
	Name##_splice(self, pos, x, CSTL_LIST_BEGIN(x), CSTL_LIST_END(x));\
	Name##_delete(x);\
	return 1;\
}\
\
int Name##_insert_range(Name *self, Name##Iterator pos, Name##Iterator first, Name##Iterator last)\
{\
	Name *x;\
	register Name##Iterator i;\
	assert(self && "List_insert_range");\
	assert(self->magic == self && "List_insert_range");\
	assert(pos && "List_insert_range");\
	assert(pos->magic == self->terminator && "List_insert_range");\
	assert(first && "List_insert_range");\
	assert(last && "List_insert_range");\
	assert(first->magic && "List_insert_range");\
	assert(last->magic && "List_insert_range");\
	x = Name##_new();\
	if (!x) return 0;\
	for (i = first; i != last; i = CSTL_LIST_NEXT(i)) {\
		assert(i->magic && "List_insert_range");\
		if (!Name##_push_back(x, CSTL_LIST_AT(i))) {\
			Name##_delete(x);\
			return 0;\
		}\
	}\
	Name##_splice(self, pos, x, CSTL_LIST_BEGIN(x), CSTL_LIST_END(x));\
	Name##_delete(x);\
	return 1;\
}\
\
Name##Iterator Name##_erase(Name *self, Name##Iterator pos)\
{\
	Name##Node *node;\
	assert(self && "List_erase");\
	assert(self->magic == self && "List_erase");\
	assert(pos && "List_erase");\
	assert(pos != self->terminator && "List_erase");\
	assert(pos->magic == self->terminator && "List_erase");\
	assert(!Name##_empty(self) && "List_erase");\
	node = pos->next;\
	pos->prev->next = pos->next;\
	pos->next->prev = pos->prev;\
	CSTL_LIST_MAGIC(pos->magic = 0);\
	free(pos);\
	self->nelems--;\
	return node;\
}\
\
Name##Iterator Name##_erase_range(Name *self, Name##Iterator first, Name##Iterator last)\
{\
	register Name##Iterator pos;\
	assert(self && "List_erase_range");\
	assert(self->magic == self && "List_erase_range");\
	assert(first && "List_erase_range");\
	assert(last && "List_erase_range");\
	assert(first->magic == self->terminator && "List_erase_range");\
	assert(last->magic == self->terminator && "List_erase_range");\
	pos = first;\
	while (pos != last) {\
		assert(!Name##_empty(self) && "List_erase_range");\
		pos = Name##_erase(self, pos);\
	}\
	return pos;\
}\
\
int Name##_resize(Name *self, size_t n, Type elem)\
{\
	register size_t i;\
	size_t size;\
	assert(self && "List_resize");\
	assert(self->magic == self && "List_resize");\
	size = Name##_size(self);\
	if (size >= n) {\
		for (i = 0; i < size - n; i++) {\
			Name##_erase(self, CSTL_LIST_RBEGIN(self));\
		}\
	} else {\
		Name *x;\
		x = Name##_new();\
		if (!x) return 0;\
		for (i = 0; i < n - size; i++) {\
			if (!Name##_push_back(x, elem)) {\
				Name##_delete(x);\
				return 0;\
			}\
		}\
		Name##_splice(self, CSTL_LIST_END(self), x, CSTL_LIST_BEGIN(x), CSTL_LIST_END(x));\
		Name##_delete(x);\
	}\
	return 1;\
}\
\
void Name##_swap(Name *self, Name *x)\
{\
	Name##Node *tmp_terminator;\
	size_t tmp_nelems;\
	assert(self && "List_swap");\
	assert(x && "List_swap");\
	assert(self->magic == self && "List_swap");\
	assert(x->magic == x && "List_swap");\
	tmp_terminator = self->terminator;\
	tmp_nelems = self->nelems;\
	self->terminator = x->terminator;\
	self->nelems = x->nelems;\
	x->terminator = tmp_terminator;\
	x->nelems = tmp_nelems;\
}\
\
void Name##_splice(Name *self, Name##Iterator pos, Name *x, Name##Iterator first, Name##Iterator last)\
{\
	register Name##Iterator i;\
	Name##Node *tmp;\
	register size_t count = 0;\
	assert(self && "List_splice");\
	assert(self->magic == self && "List_splice");\
	assert(pos && "List_splice");\
	assert(pos->magic == self->terminator && "List_splice");\
	assert(x && "List_splice");\
	assert(x->magic == x && "List_splice");\
	assert(first && "List_splice");\
	assert(last && "List_splice");\
	assert(first->magic == x->terminator && "List_splice");\
	assert(last->magic == x->terminator && "List_splice");\
	if (first == last || pos == last) return;\
	for (i = first; i != last; i = CSTL_LIST_NEXT(i)) {\
		assert(i != pos && "List_splice");\
		assert(i->magic == x->terminator && "List_splice");\
		CSTL_LIST_MAGIC(i->magic = self->terminator);\
		count++;\
	}\
	pos->prev->next = first;\
	tmp = first->prev;\
	first->prev = pos->prev;\
	tmp->next = last;\
	pos->prev = last->prev;\
	last->prev->next = pos;\
	last->prev = tmp;\
	self->nelems += count;\
	x->nelems -= count;\
}\
\
static Name##Node *Name##_merge_list(Name##Node *x, Name##Node *y, Name##Node *last, int (*comp)(const void *, const void *))\
{\
	register Name##Node *p;\
	register Name##Node *q;\
/*	assert(x->prev == y->prev && x->prev == last);*/\
	p = last;\
	while (x != last && y != last) {\
		if (comp(&CSTL_LIST_AT(x), &CSTL_LIST_AT(y)) <= 0) {\
			p->next = x;\
			x->prev = p;\
			p = x;\
			x = x->next;\
		} else {\
			p->next = y;\
			y->prev = p;\
			p = y;\
			y = y->next;\
		}\
	}\
	if (x == last) {\
		p->next = y;\
		y->prev = p;\
		p = y;\
	} else {\
		p->next = x;\
		x->prev = p;\
		p = x;\
	}\
	do {\
		q = p;\
		p = p->next;\
	} while (p != last);\
	last->prev = q;\
	return last->next;\
}\
\
static Name##Node *Name##_merge_sort(Name##Node *first, Name##Node *last, int (*comp)(const void *, const void *))\
{\
	register Name##Node *x;\
	register Name##Node *y;\
	if (first == last || first->next == last) {\
		return first;\
	}\
	x = first;\
	y = first;\
	do {\
		x = x->next;\
		y = y->next;\
		if (y != last) {\
			y = y->next;\
		}\
	} while (y != last);\
	x->prev->next = last;\
	x->prev = first->prev;\
	return Name##_merge_list(Name##_merge_sort(first, last, comp), Name##_merge_sort(x, last, comp), last, comp);\
}\
\
void Name##_sort(Name *self, int (*comp)(const void *, const void *))\
{\
	assert(self && "List_sort");\
	assert(self->magic == self && "List_sort");\
	assert(comp && "List_sort");\
	Name##_merge_sort(CSTL_LIST_BEGIN(self), CSTL_LIST_END(self), comp);\
}\
\
void Name##_merge(Name *self, Name *x, int (*comp)(const void *, const void *))\
{\
	Name##Node *p;\
	Name##Node *q;\
	assert(self && "List_merge");\
	assert(self->magic == self && "List_merge");\
	assert(x && "List_merge");\
	assert(x->magic == x && "List_merge");\
	assert(comp && "List_merge");\
	if (self == x || !Name##_size(x)) return;\
	if (!Name##_size(self)) {\
		Name##_splice(self, CSTL_LIST_END(self), x, CSTL_LIST_BEGIN(x), CSTL_LIST_END(x));\
		return;\
	}\
	p = CSTL_LIST_RBEGIN(self);\
	q = CSTL_LIST_BEGIN(x);\
	Name##_splice(self, CSTL_LIST_END(self), x, CSTL_LIST_BEGIN(x), CSTL_LIST_END(x));\
	p->next = CSTL_LIST_END(self);\
	Name##_merge_list(CSTL_LIST_BEGIN(self), q, CSTL_LIST_END(self), comp);\
}\
\
void Name##_reverse(Name *self)\
{\
	register Name##Node *p;\
	register Name##Node *tmp;\
	assert(self && "List_reverse");\
	assert(self->magic == self && "List_reverse");\
	p = self->terminator;\
	do {\
		tmp = p->next;\
		p->next = p->prev;\
		p->prev = tmp;\
		p = tmp;\
	} while (p != self->terminator);\
}\
\


#endif /* CSTL_LIST_H_INCLUDED */
