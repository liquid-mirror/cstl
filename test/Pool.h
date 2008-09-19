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
 * \file Pool.h
 * \brief 固定長メモリプールからの動的メモリの確保と解放
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2006-03-22
 *
 * 必要ならば排他制御をすること。
 *
 * \par 使用例
 *
 * \code
 * #include "Pool.h"
 * Pool pool;
 * int buf[256];
 * #define malloc(s)      Pool_malloc(&pool, s)
 * #define realloc(p, s)  Pool_realloc(&pool, p, s)
 * #define free(p)        Pool_free(&pool, p)
 *
 * int main(void)
 * {
 *     int i;
 *     char *hoge;
 *     Pool_init(&pool, buf, sizeof buf, sizeof buf[0]);
 *     hoge = Pool_malloc(&pool, 16);
 *     for (i = 0; i < 16; i++) {
 *         hoge[i] = i;
 *     }
 *     Pool_free(&pool, hoge);
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
 *     POOL_DUMP_LEAK(&pool, 1);
 *     return 0;
 * }
 * \endcode
 */
#ifndef POOL_H_INCLUDED
#define POOL_H_INCLUDED

#include <stddef.h>

/* 
 * POOL_SLISTをマクロ定義した場合(メモリブロックリストが単方向リスト)
 * - メモリブロックヘッダのサイズがポインタ一つ分小さくなる。
 * - freeの計算量がO(n)になる。
 *
 * POOL_SLISTをマクロ定義しない場合(メモリブロックリストが双方向リスト)
 * - メモリブロックヘッダのサイズがポインタ一つ分大きくなる。
 * - freeの計算量がO(1)になる。
 *
 * POOL_DEBUGをマクロ定義した場合、メモリリーク、バッファオーバーフロー、
 * メモリ確保失敗、不正なポインタの解放などの検出ができる。
 */

/*! 
 * \brief メモリブロックヘッダ構造体
 */
struct BlockHeader {
#ifndef POOL_SLIST
	struct BlockHeader *prev;
#endif
	struct BlockHeader *next;
	size_t size;			/* ブロックのサイズ(ヘッダのサイズも含む) */
	unsigned char occupied;	/* 使用中フラグ */
	unsigned char magic;	/* マジックナンバー */
#ifdef POOL_DEBUG
	char *file;				/* ファイル名 */
	size_t line;			/* ファイルの行番号 */
	size_t alloc_size;		/* ユーザが指定したサイズ */
#endif
};

/*! 
 * \brief メモリプール構造体
 *
 * メモリブロックヘッダのリストを持つ
 */
typedef struct Pool {
	struct BlockHeader list_term;	/* メモリブロックリストターミネータ */
	struct BlockHeader *loop_p;		/* ループ用ポインタ */
	struct Pool *init_flag;
	size_t align_size;
	size_t header_size;
#ifdef POOL_DEBUG
	size_t wall_size;
	int fail_count;
#endif
} Pool;

#ifdef __cplusplus
extern "C" {
#endif
void Pool_init(Pool *self, void *buf, size_t size, size_t alignment);
#ifdef POOL_DEBUG
void *Pool_malloc_debug(Pool *self, size_t size, char *file, size_t line);
void *Pool_realloc_debug(Pool *self, void *ptr, size_t newsize, char *file, size_t line);
void Pool_free_debug(Pool *self, void *ptr, char *file, size_t line);
void hex_dump(void *buf, size_t size);
size_t Pool_dump_leak(Pool *self, int dump);
void Pool_dump_block(Pool *self, void *ptr);
void Pool_dump_list(Pool *self);
int Pool_check_overflow(Pool *self, void *ptr);
void Pool_dump_overflow(Pool *self);
#define Pool_malloc(self, s)			Pool_malloc_debug(self, s, __FILE__, __LINE__)
#define Pool_realloc(self, p, s)		Pool_realloc_debug(self, p, s, __FILE__, __LINE__)
#define Pool_free(self, p)				Pool_free_debug(self, p, __FILE__, __LINE__)
#define POOL_DUMP_LEAK(self, d)			Pool_dump_leak(self, d)
#define POOL_DUMP_BLOCK(self, p)		Pool_dump_block(self, p)
#define POOL_DUMP_LIST(self)			Pool_dump_list(self)
#define POOL_DUMP_OVERFLOW(self)		Pool_dump_overflow(self)
#define POOL_SET_FAIL_COUNT(self, c)	(self)->fail_count = (c)
#define POOL_RESET_FAIL_COUNT(self)		(self)->fail_count = -1
#else
void *Pool_malloc(Pool *self, size_t size);
void *Pool_realloc(Pool *self, void *ptr, size_t newsize);
void Pool_free(Pool *self, void *ptr);
#define POOL_DUMP_LEAK(self, d)
#define POOL_DUMP_BLOCK(self, p)
#define POOL_DUMP_LIST(self)
#define POOL_DUMP_OVERFLOW(self)
#define POOL_SET_FAIL_COUNT(self, c)
#define POOL_RESET_FAIL_COUNT(self)
#endif
#ifdef __cplusplus
}
#endif


#endif /* POOL_H_INCLUDED */
