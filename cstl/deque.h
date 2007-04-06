/* 
 * Copyright (c) 2007, KATO Noriaki
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
 * \date 2007-04-01
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

#define Type unsigned char

typedef struct UCharDeque_t UCharDeque;

DEQUE_BEGIN_EXTERN_C()
UCharDeque *UCharDeque_new(void);
void UCharDeque_delete(UCharDeque *self);
int UCharDeque_assign(UCharDeque *self, UCharDeque *x, size_t idx, size_t n);
int UCharDeque_push_back(UCharDeque *self, Type elem);
int UCharDeque_push_front(UCharDeque *self, Type elem);
Type UCharDeque_pop_front(UCharDeque *self);
Type UCharDeque_pop_back(UCharDeque *self);
size_t UCharDeque_size(UCharDeque *self);
int UCharDeque_empty(UCharDeque *self);
void UCharDeque_clear(UCharDeque *self);
Type *UCharDeque_at(UCharDeque *self, size_t idx);
Type UCharDeque_front(UCharDeque *self);
Type UCharDeque_back(UCharDeque *self);
int UCharDeque_insert(UCharDeque *self, size_t idx, Type elem);
int UCharDeque_insert_array(UCharDeque *self, size_t idx, Type *elems, size_t n);
void UCharDeque_erase(UCharDeque *self, size_t idx, size_t n);
int UCharDeque_resize(UCharDeque *self, size_t n, Type elem);
void UCharDeque_swap(UCharDeque *self, UCharDeque *x);
DEQUE_END_EXTERN_C()

#define DEQUE_BUF_SIZE(Type)	(sizeof(Type) < 512 ? 512 / sizeof(Type) : 1)

#define malloc(s)        Heap_alloc(&heap, s)
#define realloc(p, s)    Heap_realloc(&heap, p, s)
#define free(p)          Heap_free(&heap, p)

#include "ring.h"
#include "vector.h"
RING_INTERFACE(Ring, Type)
RING_IMPLEMENT(Ring, Type)
VECTOR_INTERFACE(RingVector, Ring*)
VECTOR_IMPLEMENT(RingVector, Ring*)

/*! 
 * \brief deque構造体
 */
struct UCharDeque_t {
	size_t begin;
	size_t end;
	size_t nelems;
	RingVector *map;
	DEQUE_MAGIC(void *magic;)
};

enum {
	DEQUE_INITIAL_MAP_SIZE = 8
};

static size_t UCharDeque_forward(UCharDeque *self, size_t idx, size_t n)
{
	return 0;
}

static size_t UCharDeque_backward(UCharDeque *self, size_t idx, size_t n)
{
	return 0;
}

static size_t UCharDeque_next(UCharDeque *self, size_t idx)
{
	return 0;
}

static size_t UCharDeque_prev(UCharDeque *self, size_t idx)
{
	return 0;
}

static size_t UCharDeque_distance(UCharDeque *self, size_t first, size_t last)
{
	return 0;
}

UCharDeque *UCharDeque_new(void)
{
	UCharDeque *self;
	self = (UCharDeque *) malloc(sizeof(UCharDeque));
	if (!self) return 0;
	/* TODO */
	self->map = RingVector_new(DEQUE_INITIAL_MAP_SIZE);
	if (!self->map) {
		free(self);
		return 0;
	}
	RingVector_resize(self->map, DEQUE_INITIAL_MAP_SIZE, 0);
	self->begin = RingVector_size(self->map) / 2;
	self->end = self->begin + 1;
	self->nelems = 0;
	*RingVector_at(self->map, self->begin) = Ring_new(DEQUE_BUF_SIZE(Type));
	if (!*RingVector_at(self->map, self->begin)) {
		RingVector_delete(self->map);
		free(self);
		return 0;
	}
	DEQUE_MAGIC(self->magic = self;)
	return self;
}

void UCharDeque_delete(UCharDeque *self)
{
	size_t i;
	assert(self && "Deque_delete");
	assert(self->magic == self && "Deque_delete");
	for (i = 0; i < RingVector_size(self->map); i++) {
		Ring *r = *RingVector_at(self->map, i);
		if (r) {
			Ring_delete(r);
		}
	}
	RingVector_delete(self->map);
	DEQUE_MAGIC(self->magic = 0;)
	free(self);
}

int UCharDeque_assign(UCharDeque *self, UCharDeque *x, size_t idx, size_t n)
{
	size_t i;
	assert(self && "Deque_assign");
	assert(self->magic == self && "Deque_assign");
	assert(x && "Deque_assign");
	assert(x->magic == x && "Deque_assign");
	assert(UCharDeque_size(x) >= idx + n && "Deque_assign");
//    if (n > UCharDeque_max_size(self)) return 0;
	if (self == x) {
		UCharDeque_erase(self, idx + n, UCharDeque_size(self) - (idx + n));
		UCharDeque_erase(self, 0, idx);
	} else {
		UCharDeque_clear(self);
		for (i = 0; i < n; i++) {
			UCharDeque_push_back(self, *UCharDeque_at(x, i));
		}
	}
	return 1;
}

int UCharDeque_push_back(UCharDeque *self, Type elem)
{
	assert(self && "Deque_push_back");
	assert(self->magic == self && "Deque_push_back");
	return 1;
}

int UCharDeque_push_front(UCharDeque *self, Type elem)
{
	assert(self && "Deque_push_front");
	assert(self->magic == self && "Deque_push_front");
	return 1;
}

Type UCharDeque_pop_front(UCharDeque *self)
{
	assert(self && "Deque_pop_front");
	assert(self->magic == self && "Deque_pop_front");
	assert(!UCharDeque_empty(self) && "Deque_pop_front");
	return 0;
}

Type UCharDeque_pop_back(UCharDeque *self)
{
	assert(self && "Deque_pop_back");
	assert(self->magic == self && "Deque_pop_back");
	assert(!UCharDeque_empty(self) && "Deque_pop_back");
	return 0;
}

size_t UCharDeque_size(UCharDeque *self)
{
	assert(self && "Deque_size");
	assert(self->magic == self && "Deque_size");
	return self->nelems;
}

int UCharDeque_empty(UCharDeque *self)
{
	assert(self && "Deque_empty");
	assert(self->magic == self && "Deque_empty");
	return (self->nelems == 0);
}

void UCharDeque_clear(UCharDeque *self)
{
	assert(self && "Deque_clear");
	assert(self->magic == self && "Deque_clear");
	self->begin = RingVector_size(self->map) / 2;
	self->end = self->begin + 1;
	self->nelems = 0;
	Ring_clear(*RingVector_at(self->map, self->begin));
}

Type *UCharDeque_at(UCharDeque *self, size_t idx)
{
	assert(self && "Deque_at");
	assert(self->magic == self && "Deque_at");
	assert(UCharDeque_size(self) > idx && "Deque_at");
	return 0;
}

Type UCharDeque_front(UCharDeque *self)
{
	assert(self && "Deque_front");
	assert(self->magic == self && "Deque_front");
	assert(!UCharDeque_empty(self) && "Deque_front");
	return 0;
}

Type UCharDeque_back(UCharDeque *self)
{
	assert(self && "Deque_back");
	assert(self->magic == self && "Deque_back");
	assert(!UCharDeque_empty(self) && "Deque_back");
	return 0;
}

static void UCharDeque_move_forward(UCharDeque *self, size_t first, size_t last, size_t n)
{
}

static void UCharDeque_move_backward(UCharDeque *self, size_t first, size_t last, size_t n)
{
}

int UCharDeque_insert(UCharDeque *self, size_t idx, Type elem)
{
	assert(self && "Deque_insert");
	assert(self->magic == self && "Deque_insert");
	assert(UCharDeque_size(self) >= idx && "Deque_insert");
	return 0;
}

int UCharDeque_insert_array(UCharDeque *self, size_t idx, Type *elems, size_t n)
{
	assert(self && "Deque_insert_array");
	assert(self->magic == self && "Deque_insert_array");
	assert(UCharDeque_size(self) >= idx && "Deque_insert_array");
	assert(elems && "Deque_insert_array");
	return 1;
}

void UCharDeque_erase(UCharDeque *self, size_t idx, size_t n)
{
	assert(self && "Deque_erase");
	assert(self->magic == self && "Deque_erase");
	assert(UCharDeque_size(self) >= idx + n && "Deque_erase");
}

int UCharDeque_resize(UCharDeque *self, size_t n, Type elem)
{
	assert(self && "Deque_resize");
	assert(self->magic == self && "Deque_resize");
	return 1;
}

void UCharDeque_swap(UCharDeque *self, UCharDeque *x)
{
	assert(self && "Deque_swap");
	assert(x && "Deque_swap");
	assert(self->magic == self && "Deque_swap");
	assert(x->magic == x && "Deque_swap");
}

#undef malloc
#undef realloc
#undef free


#endif /* CSTL_DEQUE_H_INCLUDED */
