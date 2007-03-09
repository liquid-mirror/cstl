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
 * \file deque.h
 * \brief dequeコンテナ
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2006-02-25
 */
#ifndef CSTL_DEQUE_H_INCLUDED
#define CSTL_DEQUE_H_INCLUDED

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#ifdef __cplusplus
#define DEQUE_BEGIN_EXTERN_C()	extern "C" {
#define DEQUE_END_EXTERN_C()	}
#else
#define DEQUE_BEGIN_EXTERN_C()
#define DEQUE_END_EXTERN_C()
#endif

#ifndef NDEBUG
#define DEQUE_MAGIC(x) x
#else
#define DEQUE_MAGIC(x)
#endif

/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define DEQUE_INTERFACE(Name, Type)	\
typedef struct Name##_t Name;\
/*! 
 * \brief deque構造体
 */\
struct Name##_t {\
	size_t begin;\
	size_t end;\
	size_t nelems;\
	Type *buf;\
	DEQUE_MAGIC(void *magic;)\
};\
\
DEQUE_BEGIN_EXTERN_C()\
Name *Name##_new(size_t n);\
Name *Name##_new_copy(Name *x);\
void Name##_init(Name *self, Type *buf, size_t n);\
void Name##_delete(Name *self);\
int Name##_assign(Name *self, Type *elems, size_t n);\
int Name##_push_back(Name *self, Type elem);\
int Name##_push_front(Name *self, Type elem);\
Type Name##_pop_front(Name *self);\
Type Name##_pop_back(Name *self);\
size_t Name##_size(Name *self);\
size_t Name##_max_size(Name *self);\
int Name##_empty(Name *self);\
int Name##_full(Name *self);\
void Name##_clear(Name *self);\
Type *Name##_at(Name *self, size_t idx);\
Type Name##_front(Name *self);\
Type Name##_back(Name *self);\
int Name##_insert(Name *self, size_t idx, Type elem);\
int Name##_insert_n(Name *self, size_t idx, Type *elems, size_t n);\
void Name##_erase(Name *self, size_t idx);\
void Name##_erase_n(Name *self, size_t idx, size_t n);\
int Name##_resize(Name *self, size_t n, Type elem);\
DEQUE_END_EXTERN_C()\


/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define DEQUE_IMPLEMENT(Name, Type)	\
\
static size_t Name##_forward(Name *self, size_t idx, size_t n)\
{\
	return (idx + n) % self->nelems;\
}\
\
static size_t Name##_backward(Name *self, size_t idx, size_t n)\
{\
	return (idx >= n) ? idx - n : self->nelems + idx - n;\
}\
\
static size_t Name##_next(Name *self, size_t idx)\
{\
	return Name##_forward(self, idx, 1);\
}\
\
static size_t Name##_prev(Name *self, size_t idx)\
{\
	return Name##_backward(self, idx, 1);\
}\
\
static size_t Name##_distance(Name *self, size_t first, size_t last)\
{\
	return (first <= last) ? last - first : self->nelems - first + last;\
}\
\
Name *Name##_new(size_t n)\
{\
	Name *self;\
	Type *buf;\
	if (!n) return 0;\
	self = (Name *) malloc(sizeof(Name));\
	if (!self) return 0;\
	buf = (Type *) malloc(sizeof(Type) * (n+1));\
	if (!buf) {\
		free(self);\
		return 0;\
	}\
	Name##_init(self, buf, (n+1));\
	return self;\
}\
\
Name *Name##_new_copy(Name *x)\
{\
	Name *self;\
	assert(x && "Deque_new_copy");\
	assert(x->magic == x && "Deque_new_copy");\
	self = Name##_new(Name##_max_size(x));\
	if (!self) return 0;\
	Name##_insert_n(self, 0, Name##_at(x, 0), Name##_size(x));\
	return self;\
}\
\
void Name##_delete(Name *self)\
{\
	assert(self && "Deque_delete");\
	assert(self->magic == self && "Deque_delete");\
	DEQUE_MAGIC(self->magic = 0);\
	free(self->buf);\
	free(self);\
}\
\
void Name##_init(Name *self, Type *buf, size_t n)\
{\
	assert(self && "Deque_init");\
	assert(buf && "Deque_init");\
	self->begin = 0;\
	self->end = 0;\
	self->buf = buf;\
	self->nelems = n;\
	DEQUE_MAGIC(self->magic = self);\
}\
\
int Name##_assign(Name *self, Type *elems, size_t n)\
{\
	size_t i;\
	assert(self && "Deque_assign");\
	assert(self->magic == self && "Deque_assign");\
	assert(elems && "Deque_assign");\
	if (n > Name##_max_size(self)) return 0;\
	for (i = 0; i < n; i++) {\
		self->buf[i] = elems[i];\
	}\
	self->begin = 0;\
	self->end = n;\
	return 1;\
}\
\
int Name##_push_back(Name *self, Type elem)\
{\
	assert(self && "Deque_push_back");\
	assert(self->magic == self && "Deque_push_back");\
	if (Name##_full(self)) return 0;\
	self->buf[self->end] = elem;\
	self->end = Name##_next(self, self->end);\
	return 1;\
}\
\
int Name##_push_front(Name *self, Type elem)\
{\
	assert(self && "Deque_push_front");\
	assert(self->magic == self && "Deque_push_front");\
	if (Name##_full(self)) return 0;\
	self->begin = Name##_prev(self, self->begin);\
	self->buf[self->begin] = elem;\
	return 1;\
}\
\
Type Name##_pop_front(Name *self)\
{\
	size_t idx;\
	assert(self && "Deque_pop_front");\
	assert(self->magic == self && "Deque_pop_front");\
	assert(!Name##_empty(self) && "Deque_pop_front");\
	idx = self->begin;\
	self->begin = Name##_next(self, self->begin);\
	return self->buf[idx];\
}\
\
Type Name##_pop_back(Name *self)\
{\
	assert(self && "Deque_pop_back");\
	assert(self->magic == self && "Deque_pop_back");\
	assert(!Name##_empty(self) && "Deque_pop_back");\
	self->end = Name##_prev(self, self->end);\
	return self->buf[self->end];\
}\
\
size_t Name##_size(Name *self)\
{\
	assert(self && "Deque_size");\
	assert(self->magic == self && "Deque_size");\
	return Name##_distance(self, self->begin, self->end);\
}\
\
size_t Name##_max_size(Name *self)\
{\
	assert(self && "Deque_max_size");\
	assert(self->magic == self && "Deque_max_size");\
	return (self->nelems - 1);\
}\
\
int Name##_empty(Name *self)\
{\
	assert(self && "Deque_empty");\
	assert(self->magic == self && "Deque_empty");\
	return (self->begin == self->end);\
}\
\
int Name##_full(Name *self)\
{\
	assert(self && "Deque_full");\
	assert(self->magic == self && "Deque_full");\
	return (Name##_next(self, self->end) == self->begin);\
}\
\
void Name##_clear(Name *self)\
{\
	assert(self && "Deque_clear");\
	assert(self->magic == self && "Deque_clear");\
	self->end = self->begin;\
}\
\
Type *Name##_at(Name *self, size_t idx)\
{\
	assert(self && "Deque_at");\
	assert(self->magic == self && "Deque_at");\
	assert(Name##_size(self) > idx && "Deque_at");\
	return &self->buf[Name##_forward(self, self->begin, idx)];\
}\
\
Type Name##_front(Name *self)\
{\
	assert(self && "Deque_front");\
	assert(self->magic == self && "Deque_front");\
	assert(!Name##_empty(self) && "Deque_front");\
	return self->buf[self->begin];\
}\
\
Type Name##_back(Name *self)\
{\
	assert(self && "Deque_back");\
	assert(self->magic == self && "Deque_back");\
	assert(!Name##_empty(self) && "Deque_back");\
	return self->buf[Name##_prev(self, self->end)];\
}\
\
static void Name##_move_forward(Name *self, size_t first, size_t last, size_t n)\
{\
	size_t i;\
	for (i = Name##_prev(self, last); i != Name##_prev(self, first); i = Name##_prev(self, i)) {\
		self->buf[Name##_forward(self, i, n)] = self->buf[i];\
	}\
}\
\
static void Name##_move_backward(Name *self, size_t first, size_t last, size_t n)\
{\
	size_t i;\
	for (i = first; i != last; i = Name##_next(self, i)) {\
		self->buf[Name##_backward(self, i, n)] = self->buf[i];\
	}\
}\
\
int Name##_insert(Name *self, size_t idx, Type elem)\
{\
	assert(self && "Deque_insert");\
	assert(self->magic == self && "Deque_insert");\
	assert(Name##_size(self) >= idx && "Deque_insert");\
	return Name##_insert_n(self, idx, &elem, 1);\
}\
\
int Name##_insert_n(Name *self, size_t idx, Type *elems, size_t n)\
{\
	size_t i, j;\
	size_t pos;\
	assert(self && "Deque_insert_n");\
	assert(self->magic == self && "Deque_insert_n");\
	assert(Name##_size(self) >= idx && "Deque_insert_n");\
	assert(elems && "Deque_insert_n");\
	if (Name##_size(self) + n > Name##_max_size(self)) return 0;\
	pos = Name##_forward(self, self->begin, idx);\
	if (Name##_size(self) / 2 < idx) {\
		/* end側を移動 */\
		Name##_move_forward(self, pos, self->end, n);\
		self->end = Name##_forward(self, self->end, n);\
	} else {\
		/* begin側を移動 */\
		Name##_move_backward(self, self->begin, pos, n);\
		self->begin = Name##_backward(self, self->begin, n);\
		pos = Name##_backward(self, pos, n);\
	}\
	for (i = pos, j = 0; j < n; i = Name##_next(self, i), j++) {\
		self->buf[i] = elems[j];\
	}\
	return 1;\
}\
\
void Name##_erase(Name *self, size_t idx)\
{\
	assert(self && "Deque_erase");\
	assert(self->magic == self && "Deque_erase");\
	assert(Name##_size(self) >= idx + 1 && "Deque_erase");\
	Name##_erase_n(self, idx, 1);\
}\
\
void Name##_erase_n(Name *self, size_t idx, size_t n)\
{\
	size_t pos1;\
	size_t pos2;\
	assert(self && "Deque_erase_n");\
	assert(self->magic == self && "Deque_erase_n");\
	assert(Name##_size(self) >= idx + n && "Deque_erase_n");\
	pos1 = Name##_forward(self, self->begin, idx);\
	pos2 = Name##_forward(self, pos1, n);\
	if (Name##_distance(self, self->begin, pos1) >= \
		Name##_distance(self, pos2, self->end)) {\
		/* end側を移動 */\
		Name##_move_backward(self, pos2, self->end, n);\
		self->end = Name##_backward(self, self->end, n);\
	} else {\
		/* begin側を移動 */\
		Name##_move_forward(self, self->begin, pos1, n);\
		self->begin = Name##_forward(self, self->begin, n);\
	}\
}\
\
int Name##_resize(Name *self, size_t n, Type elem)\
{\
	size_t size;\
	assert(self && "Deque_resize");\
	assert(self->magic == self && "Deque_resize");\
	size = Name##_size(self);\
	if (size >= n) {\
		self->end = Name##_backward(self, self->end, size - n);\
	} else {\
		size_t i;\
		if (Name##_max_size(self) < n) {\
			return 0;\
		}\
		for (i = 0; i < n - size; i++) {\
			Name##_push_back(self, elem);\
		}\
	}\
	return 1;\
}\
\

#endif /* CSTL_DEQUE_H_INCLUDED */
