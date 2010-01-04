/* 
 * Copyright (c) 2006-2009, KATO Noriaki
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
 * \file hashtable_debug.h
 * \brief unordered_set/unordered_mapデバッグ用
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2009-05-21
 * $URL$
 * $Id$
 *
 * ハッシュテーブルの表示とベリファイ
 */
#ifndef CSTL_HASHTABLE_DEBUG_H_INCLUDED
#define CSTL_HASHTABLE_DEBUG_H_INCLUDED

#include <stdio.h>
#include <math.h>


#define CSTL_HASHTABLE_DEBUG_IMPLEMENT(Name, KeyType, ValueType, Hasher, Compare, format1, format2, map, value)	\
\
CSTL_VECTOR_IMPLEMENT_PUSH_BACK(Name##Node_Vector, Name##Node *)\
CSTL_VECTOR_IMPLEMENT_CLEAR(Name##Node_Vector, Name##Node *)\
\
void Name##Node_print(Name##Node *list)\
{\
	register Name##Node *pos;\
	for (pos = list; pos != 0; pos = pos->next) {\
		if (map) {\
			printf("["#format1","#format2"] -> ", pos->key, pos->value);\
		} else {\
			printf("["#format1"] -> ", pos->key);\
		}\
	}\
	printf("NULL: size[%d]\n", Name##Node_size(list));\
}\
\
void Name##_print(Name *self)\
{\
	register size_t i;\
	int figure;\
	size_t bc = Name##_bucket_count(self);\
	figure = (int) log10(bc) + 1;\
	printf("size[%d], bucket_count[%d], load_factor[%g], max_load_factor[%g]\n", \
			Name##_size(self), bc, Name##_load_factor(self), Name##_get_max_load_factor(self));\
	for (i = 0; i < bc; i++) {\
		Name##Node **alias;\
		alias = Name##Node_Vector_at(self->buckets, i);\
		if (*alias) {\
			printf("hash[%*d]: ", figure, i);\
			Name##Node_print(*alias);\
		}\
	}\
}\
\
int Name##_verify(Name *self)\
{\
	int ret = 0;\
	register size_t i;\
	size_t bc = Name##_bucket_count(self);\
	Name##Node_Vector *v = Name##Node_Vector_new_reserve(1024);\
	CSTL_ASSERT(v && "Unordered(Set|Map)_verify");\
	for (i = 0; i < bc; i++) {\
		Name##Node **alias;\
		register Name##Iterator pos;\
		register size_t j, k;\
		size_t v_size;\
		alias = Name##Node_Vector_at(self->buckets, i);\
		for (pos = *alias; pos != 0; pos = pos->next) {\
			int ret;\
			size_t idx;\
			idx = Hasher(pos->key) % bc;\
			if (i != idx) {\
				goto end;\
			}\
			ret = Name##Node_Vector_push_back(v, pos);\
			CSTL_ASSERT(ret && "Unordered(Set|Map)_verify");\
		}\
		v_size = Name##Node_Vector_size(v);\
		for (j = 0; j < v_size; j++) {\
			for (k = j + 1; k < v_size; k++) {\
				if (Compare((*Name##Node_Vector_at(v, j))->key, (*Name##Node_Vector_at(v, k))->key) == 0) {\
					if (k == j + 1) {\
						/* 同じキーが並んでいるのでOK */\
						break;\
					} else {\
						/* 同じキーが並んでいないのでNG */\
						goto end;\
					}\
				}\
			}\
		}\
		Name##Node_Vector_clear(v);\
	}\
	if (*Name##Node_Vector_back(self->buckets) != &self->end_node) {\
		goto end;\
	}\
	if (self->end_node.next != 0) {\
		goto end;\
	}\
	if (self->end_node.bucket != 0) {\
		goto end;\
	}\
	ret = 1;\
end:\
	Name##Node_Vector_delete(v);\
	return ret;\
}\
\


#define CSTL_UNORDERED_SET_DEBUG_INTERFACE(Name)	\
void Name##_print(Name *self);\
int Name##_verify(Name *self);\

#define CSTL_UNORDERED_MAP_DEBUG_INTERFACE(Name)	\
void Name##_print(Name *self);\
int Name##_verify(Name *self);\

#define CSTL_UNORDERED_SET_DEBUG_IMPLEMENT(Name, Type, Hasher, Compare, format)	\
CSTL_HASHTABLE_DEBUG_IMPLEMENT(Name, Type, Type, Hasher, Compare, format, format, 0, key)	\

#define CSTL_UNORDERED_MAP_DEBUG_IMPLEMENT(Name, KeyType, ValueType, Hasher, Compare, format1, format2)	\
CSTL_HASHTABLE_DEBUG_IMPLEMENT(Name, KeyType, ValueType, Hasher, Compare, format1, format2, 1, value)	\

#endif /* CSTL_HASHTABLE_DEBUG_H_INCLUDED */
