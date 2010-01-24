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
 * \file list.h
 * \brief listコンテナ
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2006-02-26
 * $URL$
 * $Id$
 */
#ifndef CSTL_LIST_H_INCLUDED
#define CSTL_LIST_H_INCLUDED

#include <stdlib.h>
#include "common.h"


#if !defined(NDEBUG) && defined(CSTL_DEBUG)
#define CSTL_MAGIC_LIST(Name)	((Name *) -1)
#endif


#define CSTL_LIST_BEGIN(self)	(self)->next
#define CSTL_LIST_END(self)		(self)
#define CSTL_LIST_RBEGIN(self)	(self)->prev
#define CSTL_LIST_REND(self)	(self)


/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_LIST_INTERFACE(Name, Type)	\
typedef struct Name Name;\
/*! \
 * \brief イテレータ\
 */\
typedef struct Name *Name##Iterator;\
\
CSTL_EXTERN_C_BEGIN()\
Name *Name##_new(void);\
void Name##_delete(Name *self);\
int Name##_push_back(Name *self, Type data);\
int Name##_push_front(Name *self, Type data);\
int Name##_push_back_ref(Name *self, Type const *data);\
int Name##_push_front_ref(Name *self, Type const *data);\
void Name##_pop_front(Name *self);\
void Name##_pop_back(Name *self);\
int Name##_empty(Name *self);\
size_t Name##_size(Name *self);\
void Name##_clear(Name *self);\
Type *Name##_data(Name##Iterator pos);\
Type *Name##_front(Name *self);\
Type *Name##_back(Name *self);\
Name##Iterator Name##_begin(Name *self);\
Name##Iterator Name##_end(Name *self);\
Name##Iterator Name##_rbegin(Name *self);\
Name##Iterator Name##_rend(Name *self);\
Name##Iterator Name##_next(Name##Iterator pos);\
Name##Iterator Name##_prev(Name##Iterator pos);\
Name##Iterator Name##_insert(Name *self, Name##Iterator pos, Type data);\
Name##Iterator Name##_insert_ref(Name *self, Name##Iterator pos, Type const *data);\
int Name##_insert_n(Name *self, Name##Iterator pos, size_t n, Type data);\
int Name##_insert_n_ref(Name *self, Name##Iterator pos, size_t n, Type const *data);\
int Name##_insert_array(Name *self, Name##Iterator pos, Type const *data, size_t n);\
int Name##_insert_range(Name *self, Name##Iterator pos, Name##Iterator first, Name##Iterator last);\
Name##Iterator Name##_erase(Name *self, Name##Iterator pos);\
Name##Iterator Name##_erase_range(Name *self, Name##Iterator first, Name##Iterator last);\
int Name##_resize(Name *self, size_t n, Type data);\
void Name##_swap(Name *self, Name *x);\
void Name##_splice(Name *self, Name##Iterator pos, Name *x, Name##Iterator first, Name##Iterator last);\
void Name##_sort(Name *self, int (*comp)(const void *, const void *));\
void Name##_merge(Name *self, Name *x, int (*comp)(const void *, const void *));\
void Name##_reverse(Name *self);\
CSTL_EXTERN_C_END()\


/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define CSTL_LIST_IMPLEMENT(Name, Type)	\
/*! \
 * \brief list構造体\
 */\
struct Name {\
	Name *prev;\
	Name *next;\
	Type data;\
	CSTL_MAGIC(Name *magic;)\
};\
\
Name *Name##_new(void)\
{\
	Name *self;\
	self = (Name *) malloc(sizeof(Name));\
	if (!self) return 0;\
	self->next = self;\
	self->prev = self;\
	CSTL_MAGIC(self->magic = self);\
	return self;\
}\
\
void Name##_delete(Name *self)\
{\
	if (!self) return;\
	CSTL_ASSERT(self->magic == self && "List_delete");\
	Name##_clear(self);\
	CSTL_MAGIC(self->magic = 0);\
	free(self);\
}\
\
int Name##_push_back(Name *self, Type data)\
{\
	CSTL_ASSERT(self && "List_push_back");\
	CSTL_ASSERT(self->magic == self && "List_push_back");\
	return Name##_insert_ref(self, CSTL_LIST_END(self), &data) ? 1 : 0;\
}\
\
int Name##_push_front(Name *self, Type data)\
{\
	CSTL_ASSERT(self && "List_push_front");\
	CSTL_ASSERT(self->magic == self && "List_push_front");\
	return Name##_insert_ref(self, CSTL_LIST_BEGIN(self), &data) ? 1 : 0;\
}\
\
int Name##_push_back_ref(Name *self, Type const *data)\
{\
	CSTL_ASSERT(self && "List_push_back_ref");\
	CSTL_ASSERT(self->magic == self && "List_push_back_ref");\
	CSTL_ASSERT(data && "List_push_back_ref");\
	return Name##_insert_ref(self, CSTL_LIST_END(self), data) ? 1 : 0;\
}\
\
int Name##_push_front_ref(Name *self, Type const *data)\
{\
	CSTL_ASSERT(self && "List_push_front_ref");\
	CSTL_ASSERT(self->magic == self && "List_push_front_ref");\
	CSTL_ASSERT(data && "List_push_front_ref");\
	return Name##_insert_ref(self, CSTL_LIST_BEGIN(self), data) ? 1 : 0;\
}\
\
void Name##_pop_front(Name *self)\
{\
	CSTL_ASSERT(self && "List_pop_front");\
	CSTL_ASSERT(self->magic == self && "List_pop_front");\
	CSTL_ASSERT(!Name##_empty(self) && "List_pop_front");\
	Name##_erase(self, CSTL_LIST_BEGIN(self));\
}\
\
void Name##_pop_back(Name *self)\
{\
	CSTL_ASSERT(self && "List_pop_back");\
	CSTL_ASSERT(self->magic == self && "List_pop_back");\
	CSTL_ASSERT(!Name##_empty(self) && "List_pop_back");\
	Name##_erase(self, CSTL_LIST_RBEGIN(self));\
}\
\
int Name##_empty(Name *self)\
{\
	CSTL_ASSERT(self && "List_empty");\
	CSTL_ASSERT(self->magic == self && "List_empty");\
	return self->next == self;\
}\
\
size_t Name##_size(Name *self)\
{\
	register Name##Iterator pos;\
	register size_t count = 0;\
	CSTL_ASSERT(self && "List_size");\
	CSTL_ASSERT(self->magic == self && "List_size");\
	for (pos = CSTL_LIST_BEGIN(self); pos != CSTL_LIST_END(self); pos = pos->next) {\
		count++;\
	}\
	return count;\
}\
\
void Name##_clear(Name *self)\
{\
	CSTL_ASSERT(self && "List_clear");\
	CSTL_ASSERT(self->magic == self && "List_clear");\
	Name##_erase_range(self, CSTL_LIST_BEGIN(self), CSTL_LIST_END(self));\
}\
\
Type *Name##_data(Name##Iterator pos)\
{\
	CSTL_ASSERT(pos && "List_data");\
	CSTL_ASSERT(pos->magic && "List_data");\
	CSTL_ASSERT(pos->magic == CSTL_MAGIC_LIST(Name) && "List_data");\
	return &pos->data;\
}\
\
Type *Name##_front(Name *self)\
{\
	CSTL_ASSERT(self && "List_front");\
	CSTL_ASSERT(self->magic == self && "List_front");\
	CSTL_ASSERT(!Name##_empty(self) && "List_front");\
	return &CSTL_LIST_BEGIN(self)->data;\
}\
\
Type *Name##_back(Name *self)\
{\
	CSTL_ASSERT(self && "List_back");\
	CSTL_ASSERT(self->magic == self && "List_back");\
	CSTL_ASSERT(!Name##_empty(self) && "List_back");\
	return &CSTL_LIST_RBEGIN(self)->data;\
}\
\
Name##Iterator Name##_begin(Name *self)\
{\
	CSTL_ASSERT(self && "List_begin");\
	CSTL_ASSERT(self->magic == self && "List_begin");\
	return CSTL_LIST_BEGIN(self);\
}\
\
Name##Iterator Name##_end(Name *self)\
{\
	CSTL_ASSERT(self && "List_end");\
	CSTL_ASSERT(self->magic == self && "List_end");\
	return CSTL_LIST_END(self);\
}\
\
Name##Iterator Name##_rbegin(Name *self)\
{\
	CSTL_ASSERT(self && "List_rbegin");\
	CSTL_ASSERT(self->magic == self && "List_rbegin");\
	return CSTL_LIST_RBEGIN(self);\
}\
\
Name##Iterator Name##_rend(Name *self)\
{\
	CSTL_ASSERT(self && "List_rend");\
	CSTL_ASSERT(self->magic == self && "List_rend");\
	return CSTL_LIST_REND(self);\
}\
\
Name##Iterator Name##_next(Name##Iterator pos)\
{\
	CSTL_ASSERT(pos && "List_next");\
	CSTL_ASSERT(pos->magic && "List_next");\
	CSTL_ASSERT(pos->magic == CSTL_MAGIC_LIST(Name) && "List_next");\
	return pos->next;\
}\
\
Name##Iterator Name##_prev(Name##Iterator pos)\
{\
	CSTL_ASSERT(pos && "List_prev");\
	CSTL_ASSERT(pos->magic && "List_prev");\
	CSTL_ASSERT(pos->magic == CSTL_MAGIC_LIST(Name) && "List_prev");\
	return pos->prev;\
}\
\
Name##Iterator Name##_insert(Name *self, Name##Iterator pos, Type data)\
{\
	CSTL_ASSERT(self && "List_insert");\
	CSTL_ASSERT(self->magic == self && "List_insert");\
	CSTL_ASSERT(pos && "List_insert");\
	CSTL_ASSERT((pos->magic == CSTL_MAGIC_LIST(Name) || pos->magic == self) && "List_insert");\
	return Name##_insert_ref(self, pos, &data);\
}\
\
Name##Iterator Name##_insert_ref(Name *self, Name##Iterator pos, Type const *data)\
{\
	Name *node;\
	CSTL_ASSERT(self && "List_insert_ref");\
	CSTL_ASSERT(self->magic == self && "List_insert_ref");\
	CSTL_ASSERT(pos && "List_insert_ref");\
	CSTL_ASSERT((pos->magic == CSTL_MAGIC_LIST(Name) || pos->magic == self) && "List_insert_ref");\
	CSTL_ASSERT(data && "List_insert_ref");\
	CSTL_UNUSED_PARAM(self);\
	node = (Name *) malloc(sizeof(Name));\
	if (!node) return 0;\
	node->data = *data;\
	node->next = pos;\
	node->prev = pos->prev;\
	pos->prev = node;\
	node->prev->next = node;\
	CSTL_MAGIC(node->magic = CSTL_MAGIC_LIST(Name));\
	return node;\
}\
\
int Name##_insert_n(Name *self, Name##Iterator pos, size_t n, Type data)\
{\
	CSTL_ASSERT(self && "List_insert_n");\
	CSTL_ASSERT(self->magic == self && "List_insert_n");\
	CSTL_ASSERT(pos && "List_insert_n");\
	CSTL_ASSERT((pos->magic == CSTL_MAGIC_LIST(Name) || pos->magic == self) && "List_insert_n");\
	return Name##_insert_n_ref(self, pos, n, &data);\
}\
\
int Name##_insert_n_ref(Name *self, Name##Iterator pos, size_t n, Type const *data)\
{\
	Name x;\
	register size_t i;\
	CSTL_ASSERT(self && "List_insert_n_ref");\
	CSTL_ASSERT(self->magic == self && "List_insert_n_ref");\
	CSTL_ASSERT(pos && "List_insert_n_ref");\
	CSTL_ASSERT((pos->magic == CSTL_MAGIC_LIST(Name) || pos->magic == self) && "List_insert_n_ref");\
	CSTL_ASSERT(data && "List_insert_n_ref");\
	x.next = &x;\
	x.prev = &x;\
	CSTL_MAGIC(x.magic = &x);\
	for (i = 0; i < n; i++) {\
		if (!Name##_insert_ref(&x, CSTL_LIST_END(&x), data)) {\
			Name##_clear(&x);\
			return 0;\
		}\
	}\
	Name##_splice(self, pos, &x, CSTL_LIST_BEGIN(&x), CSTL_LIST_END(&x));\
	return 1;\
}\
\
int Name##_insert_array(Name *self, Name##Iterator pos, Type const *data, size_t n)\
{\
	Name x;\
	register size_t i;\
	CSTL_ASSERT(self && "List_insert_array");\
	CSTL_ASSERT(self->magic == self && "List_insert_array");\
	CSTL_ASSERT(pos && "List_insert_array");\
	CSTL_ASSERT((pos->magic == CSTL_MAGIC_LIST(Name) || pos->magic == self) && "List_insert_array");\
	CSTL_ASSERT(data && "List_insert_array");\
	x.next = &x;\
	x.prev = &x;\
	CSTL_MAGIC(x.magic = &x);\
	for (i = 0; i < n; i++) {\
		if (!Name##_insert_ref(&x, CSTL_LIST_END(&x), &data[i])) {\
			Name##_clear(&x);\
			return 0;\
		}\
	}\
	Name##_splice(self, pos, &x, CSTL_LIST_BEGIN(&x), CSTL_LIST_END(&x));\
	return 1;\
}\
\
int Name##_insert_range(Name *self, Name##Iterator pos, Name##Iterator first, Name##Iterator last)\
{\
	Name x;\
	register Name##Iterator i;\
	CSTL_ASSERT(self && "List_insert_range");\
	CSTL_ASSERT(self->magic == self && "List_insert_range");\
	CSTL_ASSERT(pos && "List_insert_range");\
	CSTL_ASSERT((pos->magic == CSTL_MAGIC_LIST(Name) || pos->magic == self) && "List_insert_range");\
	CSTL_ASSERT(first && "List_insert_range");\
	CSTL_ASSERT(last && "List_insert_range");\
	CSTL_ASSERT(first->magic && "List_insert_range");\
	CSTL_ASSERT(last->magic && "List_insert_range");\
	x.next = &x;\
	x.prev = &x;\
	CSTL_MAGIC(x.magic = &x);\
	for (i = first; i != last; i = i->next) {\
		CSTL_ASSERT(i->magic && "List_insert_range");\
		if (!Name##_insert_ref(&x, CSTL_LIST_END(&x), &i->data)) {\
			Name##_clear(&x);\
			return 0;\
		}\
	}\
	Name##_splice(self, pos, &x, CSTL_LIST_BEGIN(&x), CSTL_LIST_END(&x));\
	return 1;\
}\
\
Name##Iterator Name##_erase(Name *self, Name##Iterator pos)\
{\
	Name *node;\
	CSTL_ASSERT(self && "List_erase");\
	CSTL_ASSERT(self->magic == self && "List_erase");\
	CSTL_ASSERT(pos && "List_erase");\
	CSTL_ASSERT(pos != self && "List_erase");\
	CSTL_ASSERT(pos->magic == CSTL_MAGIC_LIST(Name) && "List_erase");\
	CSTL_ASSERT(!Name##_empty(self) && "List_erase");\
	CSTL_UNUSED_PARAM(self);\
	node = pos->next;\
	pos->prev->next = pos->next;\
	pos->next->prev = pos->prev;\
	CSTL_MAGIC(pos->magic = 0);\
	free(pos);\
	return node;\
}\
\
Name##Iterator Name##_erase_range(Name *self, Name##Iterator first, Name##Iterator last)\
{\
	register Name##Iterator pos;\
	CSTL_ASSERT(self && "List_erase_range");\
	CSTL_ASSERT(self->magic == self && "List_erase_range");\
	CSTL_ASSERT(first && "List_erase_range");\
	CSTL_ASSERT(last && "List_erase_range");\
	CSTL_ASSERT((first->magic == CSTL_MAGIC_LIST(Name) || first->magic == self) && "List_erase_range");\
	CSTL_ASSERT((last->magic == CSTL_MAGIC_LIST(Name) || last->magic == self) && "List_erase_range");\
	pos = first;\
	while (pos != last) {\
		CSTL_ASSERT(!Name##_empty(self) && "List_erase_range");\
		pos = Name##_erase(self, pos);\
	}\
	return pos;\
}\
\
int Name##_resize(Name *self, size_t n, Type data)\
{\
	register size_t i;\
	size_t size;\
	CSTL_ASSERT(self && "List_resize");\
	CSTL_ASSERT(self->magic == self && "List_resize");\
	size = Name##_size(self);\
	if (size >= n) {\
		for (i = 0; i < size - n; i++) {\
			Name##_erase(self, CSTL_LIST_RBEGIN(self));\
		}\
	} else {\
		Name x;\
		x.next = &x;\
		x.prev = &x;\
		CSTL_MAGIC(x.magic = &x);\
		for (i = 0; i < n - size; i++) {\
			if (!Name##_insert_ref(&x, CSTL_LIST_END(&x), &data)) {\
				Name##_clear(&x);\
				return 0;\
			}\
		}\
		Name##_splice(self, CSTL_LIST_END(self), &x, CSTL_LIST_BEGIN(&x), CSTL_LIST_END(&x));\
	}\
	return 1;\
}\
\
void Name##_swap(Name *self, Name *x)\
{\
	Name *tmp_next;\
	Name *tmp_prev;\
	CSTL_ASSERT(self && "List_swap");\
	CSTL_ASSERT(x && "List_swap");\
	CSTL_ASSERT(self->magic == self && "List_swap");\
	CSTL_ASSERT(x->magic == x && "List_swap");\
	self->next->prev = x;\
	self->prev->next = x;\
	x->next->prev = self;\
	x->prev->next = self;\
	tmp_next = self->next;\
	tmp_prev = self->prev;\
	self->next = x->next;\
	self->prev = x->prev;\
	x->next = tmp_next;\
	x->prev = tmp_prev;\
}\
\
void Name##_splice(Name *self, Name##Iterator pos, Name *x, Name##Iterator first, Name##Iterator last)\
{\
	Name *tmp;\
	CSTL_ASSERT(self && "List_splice");\
	CSTL_ASSERT(self->magic == self && "List_splice");\
	CSTL_ASSERT(pos && "List_splice");\
	CSTL_ASSERT((pos->magic == CSTL_MAGIC_LIST(Name) || pos->magic == self) && "List_splice");\
	CSTL_ASSERT(x && "List_splice");\
	CSTL_ASSERT(x->magic == x && "List_splice");\
	CSTL_ASSERT(first && "List_splice");\
	CSTL_ASSERT(last && "List_splice");\
	CSTL_ASSERT((first->magic == CSTL_MAGIC_LIST(Name) || first->magic == x) && "List_splice");\
	CSTL_ASSERT((last->magic == CSTL_MAGIC_LIST(Name) || last->magic == x) && "List_splice");\
	CSTL_UNUSED_PARAM(self);\
	CSTL_UNUSED_PARAM(x);\
	if (first == last || pos == last) return;\
	pos->prev->next = first;\
	tmp = first->prev;\
	first->prev = pos->prev;\
	tmp->next = last;\
	pos->prev = last->prev;\
	last->prev->next = pos;\
	last->prev = tmp;\
}\
\
static Name *Name##_merge_list(Name *x, Name *y, int (*comp)(const void *, const void *))\
{\
	register Name *p;\
	Name *xlast;\
	Name *ylast;\
	Name head;\
	xlast = x->prev;\
	ylast = y->prev;\
	p = &head;\
	x->prev->next = &head;\
	y->prev->next = &head;\
	while (x != &head && y != &head) {\
		if (comp(&x->data, &y->data) <= 0) {\
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
	if (x == &head) {\
		p->next = y;\
		y->prev = p;\
		ylast->next = head.next;\
		head.next->prev = ylast;\
	} else {\
		p->next = x;\
		x->prev = p;\
		xlast->next = head.next;\
		head.next->prev = xlast;\
	}\
	return head.next;\
}\
\
static Name *Name##_merge_sort(Name *first, int (*comp)(const void *, const void *))\
{\
	register Name *x;\
	register Name *y;\
	Name *tmp;\
	if (first->next == first) {\
		return first;\
	}\
	x = first;\
	y = first;\
	do {\
		x = x->next;\
		y = y->next;\
		if (y != first) {\
			y = y->next;\
		}\
	} while (y != first);\
	tmp = x->prev;\
	x->prev->next = first;\
	x->prev = first->prev;\
	x->prev->next = x;\
	first->prev = tmp;\
	return Name##_merge_list(Name##_merge_sort(first, comp), Name##_merge_sort(x, comp), comp);\
}\
\
void Name##_sort(Name *self, int (*comp)(const void *, const void *))\
{\
	Name *first;\
	Name *last;\
	CSTL_ASSERT(self && "List_sort");\
	CSTL_ASSERT(self->magic == self && "List_sort");\
	CSTL_ASSERT(comp && "List_sort");\
	if (Name##_empty(self)) {\
		return;\
	}\
	/* 一時的に循環リストにする */\
	first = self->next;\
	last = self->prev;\
	first->prev = last;\
	last->next = first;\
	first = Name##_merge_sort(first, comp);\
	last = first->prev;\
	self->next = first;\
	first->prev = self;\
	last->next = self;\
	self->prev = last;\
}\
\
void Name##_merge(Name *self, Name *x, int (*comp)(const void *, const void *))\
{\
	Name *first1;\
	Name *last1;\
	Name *first2;\
	Name *last2;\
	CSTL_ASSERT(self && "List_merge");\
	CSTL_ASSERT(self->magic == self && "List_merge");\
	CSTL_ASSERT(x && "List_merge");\
	CSTL_ASSERT(x->magic == x && "List_merge");\
	CSTL_ASSERT(comp && "List_merge");\
	if (self == x || Name##_empty(x)) return;\
	if (Name##_empty(self)) {\
		Name##_splice(self, CSTL_LIST_END(self), x, CSTL_LIST_BEGIN(x), CSTL_LIST_END(x));\
		return;\
	}\
	/* 一時的に循環リストにする */\
	first1 = self->next;\
	last1 = self->prev;\
	first1->prev = last1;\
	last1->next = first1;\
	first2 = x->next;\
	last2 = x->prev;\
	first2->prev = last2;\
	last2->next = first2;\
	first1 = Name##_merge_list(first1, first2, comp);\
	last1 = first1->prev;\
	self->next = first1;\
	first1->prev = self;\
	last1->next = self;\
	self->prev = last1;\
	x->next = x;\
	x->prev = x;\
}\
\
void Name##_reverse(Name *self)\
{\
	register Name *p;\
	register Name *tmp;\
	CSTL_ASSERT(self && "List_reverse");\
	CSTL_ASSERT(self->magic == self && "List_reverse");\
	p = self;\
	do {\
		tmp = p->next;\
		p->next = p->prev;\
		p->prev = tmp;\
		p = tmp;\
	} while (p != self);\
}\
\


#endif /* CSTL_LIST_H_INCLUDED */
