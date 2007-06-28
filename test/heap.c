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
 * \file heap.c
 * \brief 動的メモリの確保と解放
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2006-03-22
 */

#include <assert.h>
#include "heap.h"


/* アラインメントのサイズ。環境に合わせて変更してもよい。 */
#define ALIGN_SIZE		(sizeof(double))
/* ALIGN_SIZE単位の切り上げ */
#define ALIGN_UP(s)		(((s) + (ALIGN_SIZE - 1)) & (~(ALIGN_SIZE - 1)))
/* ALIGN_SIZE単位の切り捨て */
#define ALIGN_DOWN(s)	((s) & (~(ALIGN_SIZE - 1)))

#define SPLIT_MIN		(ALIGN_SIZE*4)	/* 分割された空きブロックのヘッダサイズ分を除いた最小サイズ */
#define MAGIC_NO		0x17			/* マジックナンバー */

#ifdef HEAP_DEBUG
#define WALL_SIZE	32		/* ヒープオーバーフローのチェック用の壁のサイズ */
#define WALL_CHAR	0xCC
static void clear_wall(BlockHeader *p);
#endif


/* ヘッダサイズ */
static const size_t HEADER_SIZE = ALIGN_UP(sizeof(BlockHeader));

/*! 
 * \brief ヒープの初期化
 * \param self ヒープへのポインタ
 * \param buf バッファへのポインタ
 * \param size バッファのサイズ(バイト数)
 */
void Heap_init(Heap *self, double *buf, size_t size)
{
	BlockHeader *p = (BlockHeader *) buf;

	self->list_term.size = 0;
	self->list_term.occupied = 1;
	self->list_term.magic = ~MAGIC_NO & 0xFF;
	self->list_term.next = p;
#ifndef SLIST_BLOCK
	self->list_term.prev = p;
#endif

	p->size = ALIGN_DOWN(size);
	p->occupied = 0;
	p->magic = MAGIC_NO;
	p->next = &self->list_term;
#ifndef SLIST_BLOCK
	p->prev = &self->list_term;
#endif
	self->loop_p = &self->list_term;

	self->init_flag = self;
}

/*! 
 * \brief メモリ確保
 * \param self ヒープへのポインタ
 * \param size サイズ
 * \param file 呼び出し側のファイル名
 * \param line 呼び出し側のファイルの行
 * \return 割り当てたメモリへのポインタ。失敗すると0が返る。
 */
#ifdef HEAP_DEBUG
void *Heap_alloc_debug(Heap *self, size_t size, char *file, size_t line)
#else
void *Heap_alloc(Heap *self, size_t size)
#endif
{
	BlockHeader *p;
	BlockHeader *s;
#ifdef HEAP_DEBUG
	size_t i;
	size_t alloc_block_size = ALIGN_UP(HEADER_SIZE + size + WALL_SIZE * 2);
#else
	size_t alloc_block_size = ALIGN_UP(HEADER_SIZE + size);
#endif

	if (self->init_flag != self) {
		assert(0);
		return 0;
	}
	for (p = self->loop_p->next; p != self->loop_p; p = p->next) {
		if (!p->occupied && p->size >= alloc_block_size) {
			if (p->size >= alloc_block_size + HEADER_SIZE + SPLIT_MIN) {
				s = (BlockHeader *) ((char *) p + alloc_block_size);
				s->size = p->size - alloc_block_size;
				s->occupied = 0;
				s->magic = MAGIC_NO;
				p->size = alloc_block_size;
				s->next = p->next;
#ifndef SLIST_BLOCK
				s->prev = p;
				p->next->prev = s;
#endif
				p->next = s;
			}
			p->occupied = 1;
#ifdef HEAP_DEBUG
			p->file = file;
			p->line = line;
			p->alloc_size = size;
			for (i = 0; i < WALL_SIZE; i++) {
				((char *) p)[HEADER_SIZE + i] = WALL_CHAR;
				((char *) p)[HEADER_SIZE + WALL_SIZE + size + i] = WALL_CHAR;
			}
#endif
			self->loop_p = p;
#ifdef HEAP_DEBUG
			return (char *) p + HEADER_SIZE + WALL_SIZE;
#else
			return (char *) p + HEADER_SIZE;
#endif
		}
	}
	return 0;
}

/*! 
 * \brief 既に確保したメモリのサイズ変更
 * \param self ヒープへのポインタ
 * \param ptr 既に確保したメモリへのポインタ
 * \param newsize 新しいサイズ
 * \param file 呼び出し側のファイル名
 * \param line 呼び出し側のファイルの行
 * \return 新たに割り当てたメモリへのポインタ。失敗すると0が返る。
 */
#ifdef HEAP_DEBUG
void *Heap_realloc_debug(Heap *self, void *ptr, size_t newsize, char *file, size_t line)
#else
void *Heap_realloc(Heap *self, void *ptr, size_t newsize)
#endif
{
	size_t i;
	BlockHeader *p;
	BlockHeader *s;
	size_t new_alloc_block_size;
	if (self->init_flag != self) {
		assert(0);
		return 0;
	}
	if (!ptr) {
#ifdef HEAP_DEBUG
		return Heap_alloc_debug(self, newsize, file, line);
#else
		return Heap_alloc(self, newsize);
#endif
	}
	if (!newsize) {
		Heap_free(self, ptr);
		return 0;
	}
#ifdef HEAP_DEBUG
	p = (BlockHeader *) ((char *) ptr - (HEADER_SIZE + WALL_SIZE));
#else
	p = (BlockHeader *) ((char *) ptr - HEADER_SIZE);
#endif
	if (p->magic != MAGIC_NO || !p->occupied) return 0;
#ifdef HEAP_DEBUG
	new_alloc_block_size = ALIGN_UP(HEADER_SIZE + newsize + WALL_SIZE * 2);
#else
	new_alloc_block_size = ALIGN_UP(HEADER_SIZE + newsize);
#endif
	if (p->size >= new_alloc_block_size) {
		if (p->size >= 2 * new_alloc_block_size && new_alloc_block_size >= HEADER_SIZE + SPLIT_MIN) {
			/* 縮んだ分を空き領域として回収 */
			s = (BlockHeader *) ((char *) p + new_alloc_block_size);
			s->occupied = 0;
			s->magic = MAGIC_NO;
#ifndef SLIST_BLOCK
			s->prev = p;
#endif
			if (p->next->occupied) {
				s->size = p->size - new_alloc_block_size;
				s->next = p->next;
#ifndef SLIST_BLOCK
				p->next->prev = s;
#endif
			} else {
				if (self->loop_p == p->next) {
					self->loop_p = p->next->next;
				}
				s->size = p->size - new_alloc_block_size + p->next->size;
				s->next = p->next->next;
#ifndef SLIST_BLOCK
				p->next->next->prev = s;
#endif
			}
			p->size = new_alloc_block_size;
			p->next = s;
		}
#ifdef HEAP_DEBUG
		p->alloc_size = newsize;
		for (i = 0; i < WALL_SIZE; i++) {
			((char *) ptr)[newsize + i] = WALL_CHAR;
		}
#endif
		return ptr;
	}
	/* 以下、拡張 */
	if (p->next->occupied || p->size + p->next->size < new_alloc_block_size) {
#ifdef HEAP_DEBUG
		void *newptr = Heap_alloc_debug(self, newsize, file, line);
#else
		void *newptr = Heap_alloc(self, newsize);
#endif
		if (!newptr) {
			return 0;
		}
		for (i = 0;
#ifdef HEAP_DEBUG
				i < p->alloc_size;
#else
				i < p->size - HEADER_SIZE;
#endif
				i++) {
			((char *) newptr)[i] = ((char *) ptr)[i];
		}
		Heap_free(self, ptr);
		return newptr;
	}
	if (self->loop_p == p->next) {
		self->loop_p = p->next->next;
	}
	if (p->size + p->next->size >= new_alloc_block_size + HEADER_SIZE + SPLIT_MIN) {
		BlockHeader tmp = *p->next;
		s = (BlockHeader *) ((char *) p + new_alloc_block_size);
		s->size = p->size + p->next->size - new_alloc_block_size;
		s->occupied = 0;
		s->magic = MAGIC_NO;
#ifndef SLIST_BLOCK
		s->prev = p;
#endif
		s->next = tmp.next;
		p->size = new_alloc_block_size;
#ifndef SLIST_BLOCK
		tmp.next->prev = s;
#endif
		p->next = s;
	} else {
		p->size = p->size + p->next->size;
#ifndef SLIST_BLOCK
		p->next->next->prev = p;
#endif
		p->next = p->next->next;
	}
#ifdef HEAP_DEBUG
	p->file = file;
	p->line = line;
	p->alloc_size = newsize;
	for (i = 0; i < WALL_SIZE; i++) {
		((char *) ptr)[newsize + i] = WALL_CHAR;
	}
#endif
	return ptr;
}

/*! 
 * \brief メモリ解放
 * \param self ヒープへのポインタ
 * \param ptr 解放されるメモリへのポインタ
 */
#ifdef SLIST_BLOCK
void Heap_free(Heap *self, void *ptr)
{
	BlockHeader *p;
	BlockHeader *prev;
	BlockHeader *pprev;

	if (self->init_flag != self) {
		assert(0);
		return;
	}
	if (!ptr) {
		return;
	}
	prev = &self->list_term;
	pprev = &self->list_term;
	for (p = self->list_term.next; p != &self->list_term; p = p->next) {
#ifdef HEAP_DEBUG
		if (p == (BlockHeader *) ((char *) ptr - 
										(HEADER_SIZE + WALL_SIZE)
#else
										HEADER_SIZE
#endif
									)) {
			if (p->magic != MAGIC_NO || !p->occupied) {
				assert(0);
				return;
			}
#ifdef HEAP_DEBUG
			assert(check_heap_overflow(ptr));
			clear_wall(p);
#endif
			p->occupied = 0;
			if (p == self->loop_p) {
				self->loop_p = prev;
			}
			if (!prev->occupied) {
				prev->next = p->next;
				prev->size += p->size;
				p = prev;
				if (p == self->loop_p) {
					self->loop_p = pprev;
				}
			}
			if (!p->next->occupied) {
				p->size += p->next->size;
				p->next = p->next->next;
			}
			return;
		}
		pprev = prev;
		prev = p;
	}
	assert(0);
}
#else
void Heap_free(Heap *self, void *ptr)
{
	BlockHeader *p;
	BlockHeader *tmp;

	if (self->init_flag != self) {
		assert(0);
		return;
	}
	if (!ptr) {
		return;
	}
#ifdef HEAP_DEBUG
	p = (BlockHeader *) ((char *) ptr - (HEADER_SIZE + WALL_SIZE));
#else
	p = (BlockHeader *) ((char *) ptr - HEADER_SIZE);
#endif
	if (p->magic != MAGIC_NO || !p->occupied) {
		assert(0);
		return;
	}
#ifdef HEAP_DEBUG
	assert(check_heap_overflow(ptr));
	clear_wall(p);
#endif
	p->occupied = 0;
	if (p == self->loop_p) {
		self->loop_p = p->prev;
	}
	if (!p->prev->occupied) {
		tmp = p->prev;
		tmp->next = p->next;
		p->next->prev = tmp;
		tmp->size += p->size;
		p = tmp;
		if (p == self->loop_p) {
			self->loop_p = p->prev;
		}
	}
	if (!p->next->occupied) {
		tmp = p->next;
		p->next = tmp->next;
		tmp->next->prev = p;
		p->size += tmp->size;
	}
}
#endif


/* 以下、デバッグ用 */
#ifdef HEAP_DEBUG
#include <stdio.h>
#include <ctype.h>
/*! 
 * \brief 16進ダンプ
 * \param buf バッファ
 * \param size バッファサイズ
 */
void hex_dump(void *buf, size_t size)
{
	size_t i;
	size_t j;
	unsigned char tmp[16];
	unsigned char *p;
	p = (unsigned char *) buf;
	for (i = 0; i < size; i++) {
		if (i%16 == 0) {
			printf("%p: ", &p[i]);
		}
		printf("%02x ", p[i]);
		tmp[i%16] = p[i];
		if (i == size -1) {
			for (j = i+1; j < i + (16 - i%16); j++) {
				tmp[j%16] = ' ';
				printf("   ");
			}
			i = j - 1;
		}
		if (i%16 == 15) {
			printf(" ");
			for (j = 0; j < 16; j++) {
				if (isprint(tmp[j])) {
					printf("%c", tmp[j]);
				} else {
					printf(".");
				}
			}
			printf("\n");
		}
	}
}

/*! 
 * \brief 壁をクリアする
 * \param p メモリブロック
 */
static void clear_wall(BlockHeader *p)
{
	size_t i;
	for (i = 0; i < WALL_SIZE; i++) {
		((char *) p)[HEADER_SIZE + i] = ~WALL_CHAR & 0xFF;
		((char *) p)[HEADER_SIZE + WALL_SIZE + p->alloc_size + i] = ~WALL_CHAR & 0xFF;
	}
}

/*! 
 * \brief ヒープオーバーフローをチェックする
 * \param ptr Heap_alloc()で取得したポインタ
 * \return 確保したメモリサイズを超えて書き込みをしていなければ真を返す
 */
int check_heap_overflow(void *ptr)
{
	size_t i;
	BlockHeader *p = (BlockHeader *) ((char *) ptr - (HEADER_SIZE + WALL_SIZE));
	assert(p->occupied);
	for (i = 0; i < WALL_SIZE; i++) {
		if (((unsigned char *) p)[HEADER_SIZE + i] != WALL_CHAR) {
			return 0;
		}
		if (((unsigned char *) p)[HEADER_SIZE + WALL_SIZE + p->alloc_size + i] != WALL_CHAR) {
			return 0;
		}
	}
	return 1;
}

/*! 
 * \brief ヒープオーバーフローを検出する
 * \param self ヒープへのポインタ
 */
void dump_heap_overflow(Heap *self)
{
	BlockHeader *p;
	void *ptr;
	int flag = 0;
	if (self->init_flag != self) {
		assert(0);
		return;
	}
	for (p = self->list_term.next; p != &self->list_term; p = p->next) {
		ptr = (char *) p + HEADER_SIZE + WALL_SIZE;
		if (p->occupied && !check_heap_overflow(ptr)) {
			if (!flag) {
				printf("\ndetected heap overflow!\n");
				flag = 1;
			}
			printf("%s(%d): ", p->file, p->line);
			printf("ptr(%p): alloc(%d bytes)\n", ptr, p->alloc_size);
			hex_dump((char *) p + HEADER_SIZE, p->size - HEADER_SIZE);
		}
	}
}

/*! 
 * \brief メモリリークを検出する
 * \param self ヒープへのポインタ
 * \param dump 真ならばリークをダンプする
 * \return メモリリークの検出個数
 */
size_t dump_memory_leak(Heap *self, int dump)
{
	BlockHeader *p;
	size_t n = 0;
	size_t total = 0;
	int flag = 0;
	if (self->init_flag != self) {
		assert(0);
		return 0;
	}
	for (p = self->list_term.next; p != &self->list_term; p = p->next) {
		if (p->occupied) {
			if (!flag) {
				printf("\ndetected memory leaks!\n");
				flag = 1;
			}
			if (dump || n < 10) {
				printf("%s(%d): ", p->file, p->line);
				printf("%d bytes\n", p->size - HEADER_SIZE);
			} else if (n == 10) {
				printf("...\n");
			}
			if (dump) {
				hex_dump((char *) p + HEADER_SIZE, p->size - HEADER_SIZE);
			}
			n++;
			total += p->size - HEADER_SIZE;
		}
	}
	printf("\ntotal memory leaks: %d bytes\n", total);
	return n;
}

/*! 
 * \brief メモリブロックをダンプする
 * \param ptr Heap_alloc()で取得したポインタ
 */
void dump_memory_block(void *ptr)
{
	BlockHeader *p;
	if (!ptr) goto NG_block;
	p = (BlockHeader *) ((char *) ptr - (HEADER_SIZE + WALL_SIZE));
	if (p->magic != MAGIC_NO) goto NG_block;

	if (p->occupied) {
		printf("%s(%d): ", p->file, p->line);
		printf("block(%d bytes): alloc(%d bytes)\n", p->size, p->alloc_size);
	} else {
		printf("*** free ***: ");
		printf("block(%d bytes)\n", p->size);
	}
#ifdef SLIST_BLOCK
	printf("next(%p)\n", p->next);
#else
	printf("prev(%p): next(%p)\n", p->prev, p->next);
#endif
	hex_dump((char *) p, HEADER_SIZE);
	hex_dump((char *) p + HEADER_SIZE, p->size - HEADER_SIZE);
	printf("\n");
	return;
NG_block:
	printf("NG memory block: %p\n", p);
}

/*! 
 * \brief 全メモリブロックのリストをダンプする
 * \param self ヒープへのポインタ
 */
void dump_memory_list(Heap *self)
{
	BlockHeader *p;
	if (self->init_flag != self) {
		assert(0);
		return;
	}
	printf("\ndump_memory_list()\n");
	printf("block header size: %d bytes\n", HEADER_SIZE);
	printf("block list terminator\n");
#ifdef SLIST_BLOCK
	printf("next(%p)\n", self->list_term.next);
#else
	printf("prev(%p): next(%p)\n", self->list_term.prev, self->list_term.next);
#endif
	hex_dump((char *) (&self->list_term), HEADER_SIZE);
	printf("\n");
	for (p = self->list_term.next; p != &self->list_term; p = p->next) {
		dump_memory_block((char *) p + (HEADER_SIZE + WALL_SIZE));
	}
}
#endif

