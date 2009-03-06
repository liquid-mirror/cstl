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
/*! 
 * \brief イテレータ
 */\
typedef struct Name *Name##Iterator;\
\
CSTL_LIST_BEGIN_EXTERN_C()\
Name *Name##_new(void);\
void Name##_delete(Name *self);\
int Name##_push_back(Name *self, Type elem);\
int Name##_push_front(Name *self, Type elem);\
void Name##_pop_front(Name *self);\
void Name##_pop_back(Name *self);\
int Name##_empty(Name *self);\
size_t Name##_size(Name *self);\
void Name##_clear(Name *self);\
Type *Name##Iterator_ref(Name##Iterator pos);\
Type *Name##_front(Name *self);\
Type *Name##_back(Name *self);\
Name##Iterator Name##_begin(Name *self);\
Name##Iterator Name##_end(Name *self);\
Name##Iterator Name##_rbegin(Name *self);\
Name##Iterator Name##_rend(Name *self);\
Name##Iterator Name##Iterator_next(Name##Iterator pos);\
Name##Iterator Name##Iterator_prev(Name##Iterator pos);\
Name##Iterator Name##_insert(Name *self, Name##Iterator pos, Type elem);\
int Name##_insert_n(Name *self, Name##Iterator pos, size_t n, Type elem);\
int Name##_insert_array(Name *self, Name##Iterator pos, Type const *elems, size_t n);\
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
/*! 
 * \brief list構造体
 */\
struct Name {\
	Name *prev;\
	Name *next;\
	Type elem;\
	CSTL_LIST_MAGIC(Name *magic;)\
};\
\
Name *Name##_new(void)\
{\
	Name *self;\
	self = (Name *) malloc(sizeof(Name));\
	if (!self) return 0;\
	self->next = self;\
	self->prev = self;\
	CSTL_LIST_MAGIC(self->magic = self);\
	return self;\
}\
\
void Name##_delete(Name *self)\
{\
	if (!self) return;\
	assert(self->magic == self && "List_delete");\
	Name##_clear(self);\
	CSTL_LIST_MAGIC(self->magic = 0);\
	free(self);\
}\
\
static Name##Iterator Name##_insert_ref(Name *self, Name##Iterator pos, Type const *elem)\
{\
	Name *node;\
	node = (Name *) malloc(sizeof(Name));\
	if (!node) return 0;\
	node->elem = *elem;\
	node->next = pos;\
	node->prev = pos->prev;\
	pos->prev = node;\
	node->prev->next = node;\
	CSTL_LIST_MAGIC(node->magic = self);\
	return node;\
}\
\
int Name##_push_back(Name *self, Type elem)\
{\
	assert(self && "List_push_back");\
	assert(self->magic == self && "List_push_back");\
	return Name##_insert_ref(self, CSTL_LIST_END(self), &elem) ? 1 : 0;\
}\
\
int Name##_push_front(Name *self, Type elem)\
{\
	assert(self && "List_push_front");\
	assert(self->magic == self && "List_push_front");\
	return Name##_insert_ref(self, CSTL_LIST_BEGIN(self), &elem) ? 1 : 0;\
}\
\
void Name##_pop_front(Name *self)\
{\
	assert(self && "List_pop_front");\
	assert(self->magic == self && "List_pop_front");\
	assert(!Name##_empty(self) && "List_pop_front");\
	Name##_erase(self, CSTL_LIST_BEGIN(self));\
}\
\
void Name##_pop_back(Name *self)\
{\
	assert(self && "List_pop_back");\
	assert(self->magic == self && "List_pop_back");\
	assert(!Name##_empty(self) && "List_pop_back");\
	Name##_erase(self, CSTL_LIST_RBEGIN(self));\
}\
\
int Name##_empty(Name *self)\
{\
	assert(self && "List_empty");\
	assert(self->magic == self && "List_empty");\
	return self->next == self;\
}\
\
size_t Name##_size(Name *self)\
{\
	register Name##Iterator pos;\
	register size_t count = 0;\
	assert(self && "List_size");\
	assert(self->magic == self && "List_size");\
	for (pos = CSTL_LIST_BEGIN(self); pos != CSTL_LIST_END(self); pos = pos->next) {\
		count++;\
	}\
	return count;\
}\
\
void Name##_clear(Name *self)\
{\
	assert(self && "List_clear");\
	assert(self->magic == self && "List_clear");\
	Name##_erase_range(self, CSTL_LIST_BEGIN(self), CSTL_LIST_END(self));\
}\
\
Type *Name##Iterator_ref(Name##Iterator pos)\
{\
	assert(pos && "ListIterator_ref");\
	assert(pos->magic && "ListIterator_ref");\
	assert(pos->magic != pos && "ListIterator_ref");\
	return &pos->elem;\
}\
\
Type *Name##_front(Name *self)\
{\
	assert(self && "List_front");\
	assert(self->magic == self && "List_front");\
	assert(!Name##_empty(self) && "List_front");\
	return &CSTL_LIST_BEGIN(self)->elem;\
}\
\
Type *Name##_back(Name *self)\
{\
	assert(self && "List_back");\
	assert(self->magic == self && "List_back");\
	assert(!Name##_empty(self) && "List_back");\
	return &CSTL_LIST_RBEGIN(self)->elem;\
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
Name##Iterator Name##Iterator_next(Name##Iterator pos)\
{\
	assert(pos && "ListIterator_next");\
	assert(pos->magic && "ListIterator_next");\
	assert(pos->magic != pos && "ListIterator_next");\
	return pos->next;\
}\
\
Name##Iterator Name##Iterator_prev(Name##Iterator pos)\
{\
	assert(pos && "ListIterator_prev");\
	assert(pos->magic && "ListIterator_prev");\
	assert(pos->magic != pos && "ListIterator_prev");\
	return pos->prev;\
}\
\
Name##Iterator Name##_insert(Name *self, Name##Iterator pos, Type elem)\
{\
	assert(self && "List_insert");\
	assert(self->magic == self && "List_insert");\
	assert(pos && "List_insert");\
	assert(pos->magic == self && "List_insert");\
	return Name##_insert_ref(self, pos, &elem);\
}\
\
int Name##_insert_n(Name *self, Name##Iterator pos, size_t n, Type elem)\
{\
	Name x;\
	register size_t i;\
	assert(self && "List_insert_n");\
	assert(self->magic == self && "List_insert_n");\
	assert(pos && "List_insert_n");\
	assert(pos->magic == self && "List_insert_n");\
	x.next = &x;\
	x.prev = &x;\
	CSTL_LIST_MAGIC(x.magic = &x);\
	for (i = 0; i < n; i++) {\
		if (!Name##_insert_ref(&x, CSTL_LIST_END(&x), &elem)) {\
			Name##_clear(&x);\
			return 0;\
		}\
	}\
	Name##_splice(self, pos, &x, CSTL_LIST_BEGIN(&x), CSTL_LIST_END(&x));\
	return 1;\
}\
\
int Name##_insert_array(Name *self, Name##Iterator pos, Type const *elems, size_t n)\
{\
	Name x;\
	register size_t i;\
	assert(self && "List_insert_array");\
	assert(self->magic == self && "List_insert_array");\
	assert(pos && "List_insert_array");\
	assert(pos->magic == self && "List_insert_array");\
	assert(elems && "List_insert_array");\
	x.next = &x;\
	x.prev = &x;\
	CSTL_LIST_MAGIC(x.magic = &x);\
	for (i = 0; i < n; i++) {\
		if (!Name##_insert_ref(&x, CSTL_LIST_END(&x), &elems[i])) {\
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
	assert(self && "List_insert_range");\
	assert(self->magic == self && "List_insert_range");\
	assert(pos && "List_insert_range");\
	assert(pos->magic == self && "List_insert_range");\
	assert(first && "List_insert_range");\
	assert(last && "List_insert_range");\
	assert(first->magic && "List_insert_range");\
	assert(last->magic && "List_insert_range");\
	x.next = &x;\
	x.prev = &x;\
	CSTL_LIST_MAGIC(x.magic = &x);\
	for (i = first; i != last; i = i->next) {\
		assert(i->magic && "List_insert_range");\
		if (!Name##_insert_ref(&x, CSTL_LIST_END(&x), &i->elem)) {\
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
	assert(self && "List_erase");\
	assert(self->magic == self && "List_erase");\
	assert(pos && "List_erase");\
	assert(pos != self && "List_erase");\
	assert(pos->magic == self && "List_erase");\
	assert(!Name##_empty(self) && "List_erase");\
	node = pos->next;\
	pos->prev->next = pos->next;\
	pos->next->prev = pos->prev;\
	CSTL_LIST_MAGIC(pos->magic = 0);\
	free(pos);\
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
	assert(first->magic == self && "List_erase_range");\
	assert(last->magic == self && "List_erase_range");\
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
		Name x;\
		x.next = &x;\
		x.prev = &x;\
		CSTL_LIST_MAGIC(x.magic = &x);\
		for (i = 0; i < n - size; i++) {\
			if (!Name##_insert_ref(&x, CSTL_LIST_END(&x), &elem)) {\
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
	CSTL_LIST_MAGIC(register Name##Iterator pos);\
	assert(self && "List_swap");\
	assert(x && "List_swap");\
	assert(self->magic == self && "List_swap");\
	assert(x->magic == x && "List_swap");\
	CSTL_LIST_MAGIC(\
		for (pos = CSTL_LIST_BEGIN(self); pos != CSTL_LIST_END(self); pos = pos->next) {\
			assert(pos->magic == self && "List_swap");\
			pos->magic = x;\
		}\
		for (pos = CSTL_LIST_BEGIN(x); pos != CSTL_LIST_END(x); pos = pos->next) {\
			assert(pos->magic == x && "List_swap");\
			pos->magic = self;\
		}\
	);\
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
	CSTL_LIST_MAGIC(register Name##Iterator i);\
	assert(self && "List_splice");\
	assert(self->magic == self && "List_splice");\
	assert(pos && "List_splice");\
	assert(pos->magic == self && "List_splice");\
	assert(x && "List_splice");\
	assert(x->magic == x && "List_splice");\
	assert(first && "List_splice");\
	assert(last && "List_splice");\
	assert(first->magic == x && "List_splice");\
	assert(last->magic == x && "List_splice");\
	if (first == last || pos == last) return;\
	CSTL_LIST_MAGIC(\
		for (i = first; i != last; i = i->next) {\
			assert(i != pos && "List_splice");\
			assert(i->magic == x && "List_splice");\
			i->magic = self;\
		}\
	);\
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
		if (comp(&x->elem, &y->elem) <= 0) {\
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
	assert(self && "List_sort");\
	assert(self->magic == self && "List_sort");\
	assert(comp && "List_sort");\
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
	CSTL_LIST_MAGIC(register Name##Iterator pos);\
	assert(self && "List_merge");\
	assert(self->magic == self && "List_merge");\
	assert(x && "List_merge");\
	assert(x->magic == x && "List_merge");\
	assert(comp && "List_merge");\
	if (self == x || Name##_empty(x)) return;\
	if (Name##_empty(self)) {\
		Name##_splice(self, CSTL_LIST_END(self), x, CSTL_LIST_BEGIN(x), CSTL_LIST_END(x));\
		return;\
	}\
	CSTL_LIST_MAGIC(\
		for (pos = CSTL_LIST_BEGIN(x); pos != CSTL_LIST_END(x); pos = pos->next) {\
			assert(pos->magic == x && "List_merge");\
			pos->magic = self;\
		}\
	);\
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
	assert(self && "List_reverse");\
	assert(self->magic == self && "List_reverse");\
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
