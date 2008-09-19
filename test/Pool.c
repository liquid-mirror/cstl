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
 * \file Pool.c
 * \brief 固定長メモリプールからの動的メモリの確保と解放
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2006-03-22
 */

#include <assert.h>
#include "Pool.h"


typedef struct BlockHeader BlockHeader;

/* ヘッダサイズ */
#define HEADER_SIZE		(self->header_size)
/* アラインメントのサイズ */
#define ALIGN_SIZE		(self->align_size)
/* ALIGN_SIZE単位の切り上げ */
#define ALIGN_UP(s)		(((s) + (ALIGN_SIZE - 1)) & (~(ALIGN_SIZE - 1)))
/* ALIGN_SIZE単位の切り捨て */
#define ALIGN_DOWN(s)	((s) & (~(ALIGN_SIZE - 1)))

#define SPLIT_MIN		(ALIGN_SIZE*4)	/* 分割された空きブロックのヘッダサイズ分を除いた最小サイズ */
#define MAGIC_NO		0x17			/* マジックナンバー */

#ifdef POOL_DEBUG
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
/* バッファオーバーフローのチェック用の壁のサイズ */
#define WALL_SIZE		(self->wall_size)
#define WALL_CHAR		0xCC
static void clear_wall(Pool *self, BlockHeader *p);
static void debug_log(char *fmt, ...);
#define DEBUGLOG(x)		debug_log x
#else
#define DEBUGLOG(x)
#endif


/*! 
 * \brief メモリプールの初期化
 * \param self メモリプールへのポインタ
 * \param buf バッファへのポインタ
 * \param size バッファのサイズ
 * \param alignment アラインメント
 *
 * \pre bufはalignmentでアラインメントされていなければならない。
 * \pre alignmentは2のべき乗でなければならない。
 */
void Pool_init(Pool *self, void *buf, size_t size, size_t alignment)
{
	BlockHeader *p;
	self->align_size = alignment;
	self->header_size = ALIGN_UP(sizeof(BlockHeader));
#ifdef POOL_DEBUG
	self->wall_size = (alignment > 32) ? alignment : 32;
	self->fail_count = -1;
#endif
	p = (BlockHeader *) buf;

	self->list_term.size = 0;
	self->list_term.occupied = 1;
	self->list_term.magic = ~MAGIC_NO & 0xFF;
	self->list_term.next = p;
#ifndef POOL_SLIST
	self->list_term.prev = p;
#endif

	p->size = ALIGN_DOWN(size);
	p->occupied = 0;
	p->magic = MAGIC_NO;
	p->next = &self->list_term;
#ifndef POOL_SLIST
	p->prev = &self->list_term;
#endif
	self->loop_p = &self->list_term;

	self->init_flag = self;
}

/*! 
 * \brief メモリ確保
 * \param self メモリプールへのポインタ
 * \param size サイズ
 * \return 割り当てたメモリへのポインタ。失敗すると0が返る。
 *
 * mallocと同じ仕様
 */
#ifdef POOL_DEBUG
void *Pool_malloc_debug(Pool *self, size_t size, char *file, size_t line)
#else
void *Pool_malloc(Pool *self, size_t size)
#endif
{
	BlockHeader *p;
	BlockHeader *s;
#ifdef POOL_DEBUG
	size_t i;
	size_t alloc_block_size = ALIGN_UP(HEADER_SIZE + size + WALL_SIZE * 2);
#else
	size_t alloc_block_size = ALIGN_UP(HEADER_SIZE + size);
#endif

	if (self->init_flag != self) {
		DEBUGLOG(("Pool_malloc: pool is not initialized: %s(%d)\n", file, line));
		assert(0);
		return 0;
	}
#ifdef POOL_DEBUG
	if (!size) {
		DEBUGLOG(("Pool_malloc: alloc 0 bytes: %s(%d)\n", file, line));
	}
	if (self->fail_count >= 0) {
		if (self->fail_count > 0) {
			self->fail_count--;
		} else {
			DEBUGLOG(("Pool_malloc: return NULL by fail_count: %s(%d)\n", file, line));
			return 0;
		}
	}
#endif
	for (p = self->loop_p->next; p != self->loop_p; p = p->next) {
		if (!p->occupied && p->size >= alloc_block_size) {
			if (p->size >= alloc_block_size + HEADER_SIZE + SPLIT_MIN) {
				s = (BlockHeader *) ((char *) p + alloc_block_size);
				s->size = p->size - alloc_block_size;
				s->occupied = 0;
				s->magic = MAGIC_NO;
				p->size = alloc_block_size;
				s->next = p->next;
#ifndef POOL_SLIST
				s->prev = p;
				p->next->prev = s;
#endif
				p->next = s;
			}
			p->occupied = 1;
#ifdef POOL_DEBUG
			p->file = file;
			p->line = line;
			p->alloc_size = size;
			for (i = 0; i < WALL_SIZE; i++) {
				((char *) p)[HEADER_SIZE + i] = WALL_CHAR;
				((char *) p)[HEADER_SIZE + WALL_SIZE + size + i] = WALL_CHAR;
			}
#endif
			self->loop_p = p;
#ifdef POOL_DEBUG
			return (char *) p + HEADER_SIZE + WALL_SIZE;
#else
			return (char *) p + HEADER_SIZE;
#endif
		}
	}
	DEBUGLOG(("Pool_malloc: return NULL: %s(%d)\n", file, line));
	return 0;
}

/*! 
 * \brief 既に確保したメモリのサイズ変更
 * \param self メモリプールへのポインタ
 * \param ptr 既に確保したメモリへのポインタ
 * \param newsize 新しいサイズ
 * \return 新たに割り当てたメモリへのポインタ。失敗すると0が返る。
 *
 * reallocと同じ仕様
 */
#ifdef POOL_DEBUG
void *Pool_realloc_debug(Pool *self, void *ptr, size_t newsize, char *file, size_t line)
#else
void *Pool_realloc(Pool *self, void *ptr, size_t newsize)
#endif
{
	size_t i;
	BlockHeader *p;
	BlockHeader *s;
	size_t new_alloc_block_size;
	if (self->init_flag != self) {
		DEBUGLOG(("Pool_realloc: pool is not initialized: %s(%d)\n", file, line));
		assert(0);
		return 0;
	}
	if (!ptr) {
#ifdef POOL_DEBUG
		return Pool_malloc_debug(self, newsize, file, line);
#else
		return Pool_malloc(self, newsize);
#endif
	}
	if (!newsize) {
#ifdef POOL_DEBUG
		Pool_free_debug(self, ptr, file, line);
#else
		Pool_free(self, ptr);
#endif
		return 0;
	}
#ifdef POOL_DEBUG
	p = (BlockHeader *) ((char *) ptr - (HEADER_SIZE + WALL_SIZE));
#else
	p = (BlockHeader *) ((char *) ptr - HEADER_SIZE);
#endif
	if (p->magic != MAGIC_NO || !p->occupied) {
		DEBUGLOG(("Pool_realloc: NG pointer: %s(%d)\n", file, line));
		assert(0);
		return 0;
	}
#ifdef POOL_DEBUG
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
#ifndef POOL_SLIST
			s->prev = p;
#endif
			if (p->next->occupied) {
				s->size = p->size - new_alloc_block_size;
				s->next = p->next;
#ifndef POOL_SLIST
				p->next->prev = s;
#endif
			} else {
				if (self->loop_p == p->next) {
					self->loop_p = p->next->next;
				}
				s->size = p->size - new_alloc_block_size + p->next->size;
				s->next = p->next->next;
#ifndef POOL_SLIST
				p->next->next->prev = s;
#endif
			}
			p->size = new_alloc_block_size;
			p->next = s;
		}
#ifdef POOL_DEBUG
		p->alloc_size = newsize;
		for (i = 0; i < WALL_SIZE; i++) {
			((char *) ptr)[newsize + i] = WALL_CHAR;
		}
#endif
		return ptr;
	}
	/* 以下、拡張 */
	if (p->next->occupied || p->size + p->next->size < new_alloc_block_size) {
#ifdef POOL_DEBUG
		void *newptr = Pool_malloc_debug(self, newsize, file, line);
#else
		void *newptr = Pool_malloc(self, newsize);
#endif
		if (!newptr) {
			DEBUGLOG(("Pool_realloc: return NULL: %s(%d)\n", file, line));
			return 0;
		}
		for (i = 0;
#ifdef POOL_DEBUG
				i < p->alloc_size;
#else
				i < p->size - HEADER_SIZE;
#endif
				i++) {
			((char *) newptr)[i] = ((char *) ptr)[i];
		}
#ifdef POOL_DEBUG
		Pool_free_debug(self, ptr, file, line);
#else
		Pool_free(self, ptr);
#endif
		return newptr;
	}
#ifdef POOL_DEBUG
	if (self->fail_count >= 0) {
		if (self->fail_count > 0) {
			self->fail_count--;
		} else {
			DEBUGLOG(("Pool_malloc: return NULL by fail_count: %s(%d)\n", file, line));
			return 0;
		}
	}
#endif
	if (self->loop_p == p->next) {
		self->loop_p = p->next->next;
	}
	if (p->size + p->next->size >= new_alloc_block_size + HEADER_SIZE + SPLIT_MIN) {
		BlockHeader tmp = *p->next;
		s = (BlockHeader *) ((char *) p + new_alloc_block_size);
		s->size = p->size + p->next->size - new_alloc_block_size;
		s->occupied = 0;
		s->magic = MAGIC_NO;
#ifndef POOL_SLIST
		s->prev = p;
#endif
		s->next = tmp.next;
		p->size = new_alloc_block_size;
#ifndef POOL_SLIST
		tmp.next->prev = s;
#endif
		p->next = s;
	} else {
		p->size = p->size + p->next->size;
#ifndef POOL_SLIST
		p->next->next->prev = p;
#endif
		p->next = p->next->next;
	}
#ifdef POOL_DEBUG
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
 * \param self メモリプールへのポインタ
 * \param ptr 解放されるメモリへのポインタ
 *
 * freeと同じ仕様
 */
#ifdef POOL_SLIST
#ifdef POOL_DEBUG
void Pool_free_debug(Pool *self, void *ptr, char *file, size_t line)
#else
void Pool_free(Pool *self, void *ptr)
#endif
{
	BlockHeader *p;
	BlockHeader *prev;
	BlockHeader *pprev;

	if (self->init_flag != self) {
		DEBUGLOG(("Pool_free: pool is not initialized: %s(%d)\n", file, line));
		assert(0);
		return;
	}
	if (!ptr) {
		return;
	}
	prev = &self->list_term;
	pprev = &self->list_term;
	for (p = self->list_term.next; p != &self->list_term; p = p->next) {
		if (p == (BlockHeader *) ((char *) ptr - 
#ifdef POOL_DEBUG
										(HEADER_SIZE + WALL_SIZE)
#else
										HEADER_SIZE
#endif
									)) {
			if (p->magic != MAGIC_NO || !p->occupied) {
				DEBUGLOG(("Pool_free: NG pointer: %s(%d)\n", file, line));
				assert(0);
				return;
			}
#ifdef POOL_DEBUG
			if (!Pool_check_overflow(self, ptr)) {
				DEBUGLOG(("Pool_free: overflow: %s(%d), %s(%d)\n", p->file, p->line, file, line));
				assert(0);
			}
			clear_wall(self, p);
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
	DEBUGLOG(("Pool_free: NG pointer: %s(%d)\n", file, line));
	assert(0);
}
#else
#ifdef POOL_DEBUG
void Pool_free_debug(Pool *self, void *ptr, char *file, size_t line)
#else
void Pool_free(Pool *self, void *ptr)
#endif
{
	BlockHeader *p;
	BlockHeader *tmp;

	if (self->init_flag != self) {
		DEBUGLOG(("Pool_free: pool is not initialized: %s(%d)\n", file, line));
		assert(0);
		return;
	}
	if (!ptr) {
		return;
	}
#ifdef POOL_DEBUG
	p = (BlockHeader *) ((char *) ptr - (HEADER_SIZE + WALL_SIZE));
#else
	p = (BlockHeader *) ((char *) ptr - HEADER_SIZE);
#endif
	if (p->magic != MAGIC_NO || !p->occupied) {
		DEBUGLOG(("Pool_free: NG pointer: %s(%d)\n", file, line));
		assert(0);
		return;
	}
#ifdef POOL_DEBUG
	if (!Pool_check_overflow(self, ptr)) {
		DEBUGLOG(("Pool_free: overflow: %s(%d), %s(%d)\n", p->file, p->line, file, line));
		assert(0);
	}
	clear_wall(self, p);
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
#ifdef POOL_DEBUG
/*! 
 * \brief デバッグログ出力
 *
 * バッファオーバーフローに注意
 */
void debug_log(char *fmt, ...)
{
	va_list args;
	char buf[256];
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);
	fprintf(stderr, "%s", buf);
}

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
			debug_log("%p: ", &p[i]);
		}
		debug_log("%02x ", p[i]);
		tmp[i%16] = p[i];
		if (i == size -1) {
			for (j = i+1; j < i + (16 - i%16); j++) {
				tmp[j%16] = ' ';
				debug_log("   ");
			}
			i = j - 1;
		}
		if (i%16 == 15) {
			debug_log(" ");
			for (j = 0; j < 16; j++) {
				if (isprint(tmp[j])) {
					debug_log("%c", tmp[j]);
				} else {
					debug_log(".");
				}
			}
			debug_log("\n");
		}
	}
}

/*! 
 * \brief 壁をクリアする
 * \param self メモリプールへのポインタ
 * \param p メモリブロック
 */
static void clear_wall(Pool *self, BlockHeader *p)
{
	size_t i;
	for (i = 0; i < WALL_SIZE; i++) {
		((char *) p)[HEADER_SIZE + i] = ~WALL_CHAR & 0xFF;
		((char *) p)[HEADER_SIZE + WALL_SIZE + p->alloc_size + i] = ~WALL_CHAR & 0xFF;
	}
}

/*! 
 * \brief バッファオーバーフローをチェックする
 * \param self メモリプールへのポインタ
 * \param ptr Pool_malloc()で取得したポインタ
 * \return 確保したメモリサイズを超えて書き込みをしていなければ真を返す
 */
int Pool_check_overflow(Pool *self, void *ptr)
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
 * \brief バッファオーバーフローを検出する
 * \param self メモリプールへのポインタ
 */
void Pool_dump_overflow(Pool *self)
{
	BlockHeader *p;
	void *ptr;
	int flag = 0;
	if (self->init_flag != self) {
		debug_log("pool is not initialized\n");
		assert(0);
		return;
	}
	for (p = self->list_term.next; p != &self->list_term; p = p->next) {
		ptr = (char *) p + HEADER_SIZE + WALL_SIZE;
		if (p->occupied && !Pool_check_overflow(self, ptr)) {
			if (!flag) {
				debug_log("\ndetected overflow!\n");
				flag = 1;
			}
			debug_log("%s(%d): ", p->file, p->line);
			debug_log("ptr(%p): alloc(%d bytes)\n", ptr, p->alloc_size);
			hex_dump((char *) p + HEADER_SIZE, p->size - HEADER_SIZE);
		}
	}
}

/*! 
 * \brief メモリリークを検出する
 * \param self メモリプールへのポインタ
 * \param dump 真ならばリークをダンプする
 * \return メモリリークの検出個数
 */
size_t Pool_dump_leak(Pool *self, int dump)
{
	BlockHeader *p;
	size_t n = 0;
	size_t total = 0;
	int flag = 0;
	if (self->init_flag != self) {
		debug_log("pool is not initialized\n");
		assert(0);
		return 0;
	}
	for (p = self->list_term.next; p != &self->list_term; p = p->next) {
		if (p->occupied) {
			if (!flag) {
				debug_log("\ndetected memory leaks!\n");
				flag = 1;
			}
			if (dump || n < 10) {
				debug_log("%s(%d): ", p->file, p->line);
				debug_log("%d bytes\n", p->size - HEADER_SIZE);
			} else if (n == 10) {
				debug_log("...\n");
			}
			if (dump) {
				hex_dump((char *) p + HEADER_SIZE, p->size - HEADER_SIZE);
			}
			n++;
			total += p->size - HEADER_SIZE;
		}
	}
	debug_log("\ntotal memory leaks: %d bytes\n", total);
	return n;
}

/*! 
 * \brief メモリブロックをダンプする
 * \param self メモリプールへのポインタ
 * \param ptr Pool_malloc()で取得したポインタ
 */
void Pool_dump_block(Pool *self, void *ptr)
{
	BlockHeader *p;
	if (!ptr) goto NG_block;
	p = (BlockHeader *) ((char *) ptr - (HEADER_SIZE + WALL_SIZE));
	if (p->magic != MAGIC_NO) goto NG_block;

	if (p->occupied) {
		debug_log("%s(%d): ", p->file, p->line);
		debug_log("block(%d bytes): alloc(%d bytes)\n", p->size, p->alloc_size);
	} else {
		debug_log("*** free ***: ");
		debug_log("block(%d bytes)\n", p->size);
	}
#ifdef POOL_SLIST
	debug_log("next(%p)\n", p->next);
#else
	debug_log("prev(%p): next(%p)\n", p->prev, p->next);
#endif
	hex_dump((char *) p, HEADER_SIZE);
	hex_dump((char *) p + HEADER_SIZE, p->size - HEADER_SIZE);
	debug_log("\n");
	return;
NG_block:
	debug_log("NG memory block: %p\n", p);
}

/*! 
 * \brief 全メモリブロックのリストをダンプする
 * \param self メモリプールへのポインタ
 */
void Pool_dump_list(Pool *self)
{
	BlockHeader *p;
	if (self->init_flag != self) {
		debug_log("pool is not initialized\n");
		assert(0);
		return;
	}
	debug_log("\nPool_dump_list\n");
	debug_log("block header size: %d bytes\n", HEADER_SIZE);
	debug_log("block list terminator\n");
#ifdef POOL_SLIST
	debug_log("next(%p)\n", self->list_term.next);
#else
	debug_log("prev(%p): next(%p)\n", self->list_term.prev, self->list_term.next);
#endif
	hex_dump((char *) (&self->list_term), HEADER_SIZE);
	debug_log("\n");
	for (p = self->list_term.next; p != &self->list_term; p = p->next) {
		Pool_dump_block(self, (char *) p + (HEADER_SIZE + WALL_SIZE));
	}
}

#endif

