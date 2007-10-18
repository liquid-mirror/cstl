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
 * \file heap.h
 * \brief 動的メモリの確保と解放
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2006-03-22
 *
 * 必要ならば排他制御をすること。
 *
 * \par 使用例
 *
 * \code
 * #include "heap.h"
 * Heap heap;
 * double buf[256];
 * #define malloc(s)        Heap_alloc(&heap, s)
 * #define realloc(p, s)    Heap_realloc(&heap, p, s)
 * #define free(p)          Heap_free(&heap, p)
 *
 * int main(void)
 * {
 *     int i;
 *     char *hoge;
 *     Heap_init(&heap, buf, sizeof buf, sizeof buf[0]);
 *     hoge = Heap_alloc(&heap, 16);
 *     for (i = 0; i < 16; i++) {
 *         hoge[i] = i;
 *     }
 *     Heap_free(&heap, hoge);
 *
 *     hoge = malloc(64);
 *     for (i = 0; i < 64; i++) {
 *         hoge[i] = i;
 *     }
 *     hoge = realloc(hoge, 128);
 *     for (i = 64; i < 128; i++) {
 *         hoge[i] = i;
 *     }
 *     free(hoge);
 *
 *     HEAP_DUMP_LEAK(&heap, 1);
 *     return 0;
 * }
 * \endcode
 */
#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED

#include <stddef.h>

/* 
 * SLIST_BLOCKをマクロ定義した場合(メモリブロックリストが単方向リスト)
 * - メモリブロックヘッダのサイズがポインタ一つ分小さくなる。
 * - freeの計算量がO(n)になる。
 *
 * SLIST_BLOCKをマクロ定義しない場合(メモリブロックリストが双方向リスト)
 * - メモリブロックヘッダのサイズがポインタ一つ分大きくなる。
 * - freeの計算量がO(1)になる。
 *
 * HEAP_DEBUGをマクロ定義した場合、メモリリーク、ヒープオーバーフロー、メモ
 * リ確保失敗、不正なポインタの解放などの検出ができる。
 */

/*! 
 * \brief メモリブロックヘッダ構造体
 */
typedef struct BlockHeader_t BlockHeader;
struct BlockHeader_t {
#ifndef SLIST_BLOCK
	BlockHeader *prev;
#endif
	BlockHeader *next;
	size_t size;			/* ブロックのサイズ(ヘッダのサイズも含む) */
	unsigned char occupied;	/* 使用中フラグ */
	unsigned char magic;	/* マジックナンバー */
#ifdef HEAP_DEBUG
	char *file;
	size_t line;
	size_t alloc_size;		/* ユーザが指定したサイズ */
#endif
};

/*! 
 * \brief ヒープ構造体
 *
 * メモリブロックヘッダのリストを持つ
 */
typedef struct Heap_t Heap;
struct Heap_t {
	BlockHeader list_term;	/* メモリブロックリストターミネータ */
	BlockHeader *loop_p;	/* ループ用ポインタ */
	Heap *init_flag;
	size_t align_size;
	size_t header_size;
#ifdef HEAP_DEBUG
	size_t wall_size;
	int fail_count;
#endif
};

#ifdef __cplusplus
extern "C" {
#endif
void Heap_init(Heap *self, void *buf, size_t size, size_t alignment);
#ifdef HEAP_DEBUG
void *Heap_alloc_debug(Heap *self, size_t size, char *file, size_t line);
void *Heap_realloc_debug(Heap *self, void *ptr, size_t newsize, char *file, size_t line);
void Heap_free_debug(Heap *self, void *ptr, char *file, size_t line);
void hex_dump(void *buf, size_t size);
size_t Heap_dump_leak(Heap *self, int dump);
void Heap_dump_block(Heap *self, void *ptr);
void Heap_dump_list(Heap *self);
int Heap_check_overflow(Heap *self, void *ptr);
void Heap_dump_overflow(Heap *self);
#define Heap_alloc(h, s)		Heap_alloc_debug(h, s, __FILE__, __LINE__)
#define Heap_realloc(h, p, s)	Heap_realloc_debug(h, p, s, __FILE__, __LINE__)
#define Heap_free(h, p)			Heap_free_debug(h, p, __FILE__, __LINE__)
#define HEAP_DUMP_LEAK(h, d)	Heap_dump_leak(h, d)
#define HEAP_DUMP_BLOCK(h, p)	Heap_dump_block(h, p)
#define HEAP_DUMP_LIST(h)		Heap_dump_list(h)
#define HEAP_DUMP_OVERFLOW(h)	Heap_dump_overflow(h)
#define HEAP_SET_FAIL_COUNT(h, c)	(h)->fail_count = (c)
#define HEAP_RESET_FAIL_COUNT(h)	(h)->fail_count = -1
#else
void *Heap_alloc(Heap *self, size_t size);
void *Heap_realloc(Heap *self, void *ptr, size_t newsize);
void Heap_free(Heap *self, void *ptr);
#define HEAP_DUMP_LEAK(h, d)
#define HEAP_DUMP_BLOCK(h, p)
#define HEAP_DUMP_LIST(h)
#define HEAP_DUMP_OVERFLOW(h)
#define HEAP_SET_FAIL_COUNT(h, c)
#define HEAP_RESET_FAIL_COUNT(h)
#endif
#ifdef __cplusplus
}
#endif


#endif /* HEAP_H_INCLUDED */
