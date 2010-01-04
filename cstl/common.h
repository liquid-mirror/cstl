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
 * \file common.h
 * \brief 共通マクロ定義
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2009-12-21
 * $URL$
 * $Id$
 */
#ifndef CSTL_COMMON_H_INCLUDED
#define CSTL_COMMON_H_INCLUDED


#ifdef __cplusplus
#define CSTL_EXTERN_C_BEGIN()	extern "C" {
#define CSTL_EXTERN_C_END()		}
#else
#define CSTL_EXTERN_C_BEGIN()
#define CSTL_EXTERN_C_END()
#endif

#if !defined(NDEBUG) && defined(CSTL_DEBUG)
#include <assert.h>
#define CSTL_MAGIC(x)	x
#define CSTL_ASSERT(x)	assert(x)
#else
#define CSTL_MAGIC(x)
#define CSTL_ASSERT(x)
#endif

#define CSTL_UNUSED_PARAM(x)	(void) x


#endif /* CSTL_COMMON_H_INCLUDED */
