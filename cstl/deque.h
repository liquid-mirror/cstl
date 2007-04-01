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

typedef struct Deque_t Deque;

DEQUE_BEGIN_EXTERN_C()
Deque *Deque_new(void);
void Deque_delete(Deque *self);
int Deque_assign(Deque *self, Deque *x, size_t idx, size_t n);
int Deque_push_back(Deque *self, Type elem);
int Deque_push_front(Deque *self, Type elem);
Type Deque_pop_front(Deque *self);
Type Deque_pop_back(Deque *self);
size_t Deque_size(Deque *self);
int Deque_empty(Deque *self);
void Deque_clear(Deque *self);
Type *Deque_at(Deque *self, size_t idx);
Type Deque_front(Deque *self);
Type Deque_back(Deque *self);
int Deque_insert(Deque *self, size_t idx, Type elem);
int Deque_insert_array(Deque *self, size_t idx, Type *elems, size_t n);
void Deque_erase(Deque *self, size_t idx, size_t n);
int Deque_resize(Deque *self, size_t n, Type elem);
void Deque_swap(Deque *self, Deque *x);
DEQUE_END_EXTERN_C()



/*! 
 * \brief deque構造体
 */
struct Deque_t {
	DEQUE_MAGIC(void *magic;)
};

static size_t Deque_forward(Deque *self, size_t idx, size_t n)
{
	return 0;
}

static size_t Deque_backward(Deque *self, size_t idx, size_t n)
{
	return 0;
}

static size_t Deque_next(Deque *self, size_t idx)
{
	return 0;
}

static size_t Deque_prev(Deque *self, size_t idx)
{
	return 0;
}

static size_t Deque_distance(Deque *self, size_t first, size_t last)
{
	return 0;
}

Deque *Deque_new(void)
{
	Deque *self;
	self = (Deque *) malloc(sizeof(Deque));
	if (!self) return 0;
	/* TODO */
	return self;
}

void Deque_delete(Deque *self)
{
	assert(self && "Deque_delete");
	assert(self->magic == self && "Deque_delete");
	DEQUE_MAGIC(self->magic = 0;)
}

int Deque_assign(Deque *self, Deque *x, size_t idx, size_t n)
{
	size_t i;
	assert(self && "Deque_assign");
	assert(self->magic == self && "Deque_assign");
	assert(x && "Deque_assign");
	assert(x->magic == x && "Deque_assign");
	assert(Deque_size(x) >= idx + n && "Deque_assign");
//    if (n > Deque_max_size(self)) return 0;
	if (self == x) {
		Deque_erase(self, idx + n, Deque_size(self) - (idx + n));
		Deque_erase(self, 0, idx);
	} else {
		Deque_clear(self);
		for (i = 0; i < n; i++) {
			Deque_push_back(self, *Deque_at(x, i));
		}
	}
	return 1;
}

int Deque_push_back(Deque *self, Type elem)
{
	assert(self && "Deque_push_back");
	assert(self->magic == self && "Deque_push_back");
	return 1;
}

int Deque_push_front(Deque *self, Type elem)
{
	assert(self && "Deque_push_front");
	assert(self->magic == self && "Deque_push_front");
	return 1;
}

Type Deque_pop_front(Deque *self)
{
	assert(self && "Deque_pop_front");
	assert(self->magic == self && "Deque_pop_front");
	assert(!Deque_empty(self) && "Deque_pop_front");
	return 0;
}

Type Deque_pop_back(Deque *self)
{
	assert(self && "Deque_pop_back");
	assert(self->magic == self && "Deque_pop_back");
	assert(!Deque_empty(self) && "Deque_pop_back");
	return 0;
}

size_t Deque_size(Deque *self)
{
	assert(self && "Deque_size");
	assert(self->magic == self && "Deque_size");
	return 0;
}

int Deque_empty(Deque *self)
{
	assert(self && "Deque_empty");
	assert(self->magic == self && "Deque_empty");
	return 0;
}

void Deque_clear(Deque *self)
{
	assert(self && "Deque_clear");
	assert(self->magic == self && "Deque_clear");
}

Type *Deque_at(Deque *self, size_t idx)
{
	assert(self && "Deque_at");
	assert(self->magic == self && "Deque_at");
	assert(Deque_size(self) > idx && "Deque_at");
	return 0;
}

Type Deque_front(Deque *self)
{
	assert(self && "Deque_front");
	assert(self->magic == self && "Deque_front");
	assert(!Deque_empty(self) && "Deque_front");
	return 0;
}

Type Deque_back(Deque *self)
{
	assert(self && "Deque_back");
	assert(self->magic == self && "Deque_back");
	assert(!Deque_empty(self) && "Deque_back");
	return 0;
}

static void Deque_move_forward(Deque *self, size_t first, size_t last, size_t n)
{
}

static void Deque_move_backward(Deque *self, size_t first, size_t last, size_t n)
{
}

int Deque_insert(Deque *self, size_t idx, Type elem)
{
	assert(self && "Deque_insert");
	assert(self->magic == self && "Deque_insert");
	assert(Deque_size(self) >= idx && "Deque_insert");
	return 0;
}

int Deque_insert_array(Deque *self, size_t idx, Type *elems, size_t n)
{
	assert(self && "Deque_insert_array");
	assert(self->magic == self && "Deque_insert_array");
	assert(Deque_size(self) >= idx && "Deque_insert_array");
	assert(elems && "Deque_insert_array");
	return 1;
}

void Deque_erase(Deque *self, size_t idx, size_t n)
{
	assert(self && "Deque_erase");
	assert(self->magic == self && "Deque_erase");
	assert(Deque_size(self) >= idx + n && "Deque_erase");
}

int Deque_resize(Deque *self, size_t n, Type elem)
{
	assert(self && "Deque_resize");
	assert(self->magic == self && "Deque_resize");
	return 1;
}

void Deque_swap(Deque *self, Deque *x)
{
	assert(self && "Deque_swap");
	assert(x && "Deque_swap");
	assert(self->magic == self && "Deque_swap");
	assert(x->magic == x && "Deque_swap");
}



#endif /* CSTL_DEQUE_H_INCLUDED */
