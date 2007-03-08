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
 *     Heap_init(&heap, buf, sizeof buf);
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
 *     DUMP_MEMORY_LEAK(&heap, 1);
 *     return 0;
 * }
 * \endcode
 */
#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED

#include <stddef.h>

/* 
 * SLIST_BLOCKをプリプロセッサシンボル定義した場合、メモリブロックのリストが
 * 単方向リストとなる。メモリブロックヘッダのサイズがポインタ一つ分小さくな
 * り、freeの計算量がO(n)になる。
 * SLIST_BLOCKを定義しない場合、メモリブロックのリストが双方向リストとなる。
 * メモリブロックヘッダのサイズがポインタ一つ分大きくなり、freeの計算量が
 * O(1)になる。
 */
/*#define SLIST_BLOCK*/

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
	void *init_flag;
};

#ifdef __cplusplus
extern "C" {
#endif
void Heap_init(Heap *self, double *buf, size_t size);
void Heap_free(Heap *self, void *ptr);
#ifdef HEAP_DEBUG
void *Heap_alloc_debug(Heap *self, size_t size, char *file, size_t line);
void *Heap_realloc_debug(Heap *self, void *ptr, size_t newsize, char *file, size_t line);
void hex_dump(void *buf, size_t size);
size_t dump_memory_leak(Heap *self, int dump);
void dump_memory_block(Heap *self, void *ptr);
void dump_memory_list(Heap *self);
#define Heap_alloc(h, s)		Heap_alloc_debug(h, s, __FILE__, __LINE__)
#define Heap_realloc(h, p, s)	Heap_realloc_debug(h, p, s, __FILE__, __LINE__)
#define DUMP_MEMORY_LEAK(h, d)	dump_memory_leak(h, d)
#define DUMP_MEMORY_BLOCK(h, p)	dump_memory_block(h, p)
#define DUMP_MEMORY_LIST(h)		dump_memory_list(h)
#else
void *Heap_alloc(Heap *self, size_t size);
void *Heap_realloc(Heap *self, void *ptr, size_t newsize);
#define DUMP_MEMORY_LEAK(h, d)
#define DUMP_MEMORY_BLOCK(h, p)
#define DUMP_MEMORY_LIST(h)
#endif
#ifdef __cplusplus
}
#endif


#endif /* HEAP_H_INCLUDED */
