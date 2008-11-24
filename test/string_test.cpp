#include "../cstl/string.h"
#include "../cstl/algorithm.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <string>

#include "Pool.h"
#ifdef MY_MALLOC
double buf[1024*1024/sizeof(double)];
Pool pool;
#define malloc(s)		Pool_malloc(&pool, s)
#define realloc(p, s)	Pool_realloc(&pool, p, s)
#define free(p)			Pool_free(&pool, p)
#endif

#ifdef CSTLGEN
#include "String.h"
#else
CSTL_STRING_INTERFACE(String, char)
CSTL_STRING_IMPLEMENT(String, char)
#endif

#define SIZE	16

using namespace std;


void StringTest_test_1_1(void)
{
	String *x;
	string s;
	size_t i;
	printf("***** test_1_1 *****\n");
	/* 初期状態 */
	x = String_new(SIZE);
	assert(String_empty(x));
	assert(String_size(x) == 0);
	assert(String_capacity(x) == SIZE);
	/* c_str */
	String_assign_len(x, "abcdefghijklmn", 7);
//    printf("%s\n", String_c_str(x));
	s = String_c_str(x);
	assert(strcmp(s.c_str(), String_c_str(x)) == 0);
	/* at */
	for (i = 0; i < String_length(x); i++) {
		assert(s.at(i) == *String_at(x, i));
	}
	/* clear */
	String_clear(x);
	assert(String_empty(x));
	assert(String_size(x) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	String_delete(x);
}

void StringTest_test_1_2(void)
{
	String *x;
	String *y;
	printf("***** test_1_2 *****\n");
	x = String_new(SIZE);
	/* capacity */
	/* reserve */
	String_reserve(x, 100);
	assert(String_size(x) == 0);
	assert(String_capacity(x) == 100);
	String_reserve(x, 99);
	assert(String_capacity(x) == 100);
	String_reserve(x, 101);
	assert(String_capacity(x) == 101);
	/* shrink */
	String_shrink(x, 10);
	assert(String_capacity(x) == 10);
	String_shrink(x, 11);
	assert(String_capacity(x) == 10);
	String_shrink(x, 9);
	assert(String_capacity(x) == 9);
	/* resize */
	String_resize(x, 10, 'a');
	assert(String_size(x) == 10);
	assert(strcmp("aaaaaaaaaa", String_c_str(x)) == 0);
	String_resize(x, 15, 'b');
	assert(String_size(x) == 15);
	assert(strcmp("aaaaaaaaaabbbbb", String_c_str(x)) == 0);
	String_resize(x, 5, 'b');
	assert(String_size(x) == 5);
	assert(strcmp("aaaaa", String_c_str(x)) == 0);
	String_resize(x, 0, 'b');
	assert(String_size(x) == 0);
	assert(strcmp("", String_c_str(x)) == 0);
	/* assign */
	String_assign(x, "abcdefghijklmnopqrstuvwxyz");
	assert(String_size(x) == 26);
	assert(strcmp("abcdefghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	/* erase */
	assert(String_erase(x, 0, 3) == x);
	assert(String_size(x) == 23);
	assert(strcmp("defghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	assert(String_erase(x, 0, 0) == x);
	assert(String_size(x) == 23);
	assert(strcmp("defghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	assert(String_erase(x, 1, 1) == x);
	assert(String_size(x) == 22);
	assert(strcmp("dfghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	assert(String_erase(x, 5, 10) == x);
	assert(String_size(x) == 12);
	assert(strcmp("dfghituvwxyz", String_c_str(x)) == 0);
	assert(String_erase(x, 11, 1) == x);
	assert(String_size(x) == 11);
	assert(strcmp("dfghituvwxy", String_c_str(x)) == 0);
	assert(String_erase(x, 5, CSTL_NPOS) == x);
	assert(String_size(x) == 5);
	assert(strcmp("dfghi", String_c_str(x)) == 0);
	assert(String_erase(x, 0, CSTL_NPOS) == x);
	assert(String_size(x) == 0);
	assert(strcmp("", String_c_str(x)) == 0);
	/* compare */
	String_assign(x, "abcdefg");
	y = String_new(SIZE);
	String_assign(y, "abcdefgh");
	assert(String_compare(x, y) < 0);
	String_erase(y, String_size(y)-2, 2);
	assert(String_compare(x, y) > 0);
	String_erase(x, String_size(x)-1, 1);
	assert(String_compare(x, y) == 0);
	*String_at(x, String_size(x)-1) = 'z';
	assert(String_compare(x, y) > 0);
	*String_at(x, String_size(x)-1) = 'a';
	assert(String_compare(x, y) < 0);
	/* swap */
	String_assign(x, "abcdefg");
	String_assign(y, "abcdefgh");
	String_shrink(x, 0);
	String_shrink(y, 0);
	assert(strcmp("abcdefg", String_c_str(x)) == 0);
	assert(strcmp("abcdefgh", String_c_str(y)) == 0);
	assert(String_size(x) == 7);
	assert(String_size(y) == 8);
	String_swap(x, y);
	assert(strcmp("abcdefg", String_c_str(y)) == 0);
	assert(strcmp("abcdefgh", String_c_str(x)) == 0);
	assert(String_size(y) == 7);
	assert(String_size(x) == 8);
	/* pop_back */
	String_pop_back(x);
	assert(strcmp("abcdefg", String_c_str(x)) == 0);
	assert(String_size(x) == 7);
	String_pop_back(x);
	assert(strcmp("abcdef", String_c_str(x)) == 0);
	assert(String_size(x) == 6);
	String_pop_back(x);
	assert(strcmp("abcde", String_c_str(x)) == 0);
	assert(String_size(x) == 5);
	String_pop_back(x);
	assert(strcmp("abcd", String_c_str(x)) == 0);
	assert(String_size(x) == 4);
	String_pop_back(x);
	assert(strcmp("abc", String_c_str(x)) == 0);
	assert(String_size(x) == 3);
	String_pop_back(x);
	assert(strcmp("ab", String_c_str(x)) == 0);
	assert(String_size(x) == 2);
	String_pop_back(x);
	assert(strcmp("a", String_c_str(x)) == 0);
	assert(String_size(x) == 1);
	String_pop_back(x);
	assert(strcmp("", String_c_str(x)) == 0);
	assert(String_size(x) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	String_delete(x);
	String_delete(y);
}

void StringTest_test_1_3(void)
{
	String *x;
	printf("***** test_1_3 *****\n");
	x = String_new(SIZE);
	/* assign */
	assert(String_assign(x, "abcdefghijklmnopqrstuvwxyz") == x);
	assert(String_size(x) == 26);
	assert(strcmp("abcdefghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	assert(String_assign(x, String_c_str(x)) == x);
	assert(String_size(x) == 26);
	assert(strcmp("abcdefghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	assert(String_assign_len(x, "abcdefghijklmnopqrstuvwxyz", 10) == x);
	assert(String_size(x) == 10);
	assert(strcmp("abcdefghij", String_c_str(x)) == 0);
	assert(String_assign_len(x, "abcdefghijklmnopqrstuvwxyz", 0) == x);
	assert(String_size(x) == 0);
	assert(strcmp("", String_c_str(x)) == 0);
	assert(String_assign_len(x, "", 0) == x);
	assert(String_size(x) == 0);
	assert(strcmp("", String_c_str(x)) == 0);
	/* assign_c */
	assert(String_assign_c(x, 10, 'a') == x);
	assert(String_size(x) == 10);
	assert(strcmp("aaaaaaaaaa", String_c_str(x)) == 0);
	assert(String_assign_c(x, 1, 'b') == x);
	assert(String_size(x) == 1);
	assert(strcmp("b", String_c_str(x)) == 0);
	assert(String_assign_c(x, 0, 'c') == x);
	assert(String_size(x) == 0);
	assert(strcmp("", String_c_str(x)) == 0);
	/* append */
	assert(String_append(x, "abcdef") == x);
	assert(String_size(x) == 6);
	assert(strcmp("abcdef", String_c_str(x)) == 0);
	assert(String_append(x, "gh") == x);
	assert(String_size(x) == 8);
	assert(strcmp("abcdefgh", String_c_str(x)) == 0);
	assert(String_append_len(x, "ijkl", 3) == x);
	assert(String_size(x) == 11);
	assert(strcmp("abcdefghijk", String_c_str(x)) == 0);
	assert(String_append_len(x, "l", 1) == x);
	assert(String_size(x) == 12);
	assert(strcmp("abcdefghijkl", String_c_str(x)) == 0);
	assert(String_append_len(x, "mno", 0) == x);
	assert(String_size(x) == 12);
	assert(strcmp("abcdefghijkl", String_c_str(x)) == 0);
	assert(String_append(x, "m") == x);
	assert(String_size(x) == 13);
	assert(strcmp("abcdefghijklm", String_c_str(x)) == 0);
	assert(String_append(x, String_c_str(x) + 1) == x);
	assert(String_size(x) == 25);
	assert(strcmp("abcdefghijklmbcdefghijklm", String_c_str(x)) == 0);
	String_clear(x);
	assert(String_size(x) == 0);
	/* append_c */
	assert(String_append_c(x, 5, 'a') == x);
	assert(String_size(x) == 5);
	assert(strcmp("aaaaa", String_c_str(x)) == 0);
	assert(String_append_c(x, 2, 'b') == x);
	assert(String_size(x) == 7);
	assert(strcmp("aaaaabb", String_c_str(x)) == 0);
	assert(String_append_c(x, 4, 'c') == x);
	assert(String_size(x) == 11);
	assert(strcmp("aaaaabbcccc", String_c_str(x)) == 0);
	assert(String_append_c(x, 0, 'd') == x);
	assert(String_size(x) == 11);
	assert(strcmp("aaaaabbcccc", String_c_str(x)) == 0);
	/* push_back */
	assert(String_push_back(x, 'd') == x);
	assert(String_push_back(x, 'e') == x);
	assert(String_push_back(x, 'f') == x);
	assert(String_size(x) == 14);
	assert(strcmp("aaaaabbccccdef", String_c_str(x)) == 0);
	String_clear(x);
	assert(String_size(x) == 0);
	/* insert */
	assert(String_insert(x, 0, "abcdef") == x);
	assert(String_size(x) == 6);
	assert(strcmp("abcdef", String_c_str(x)) == 0);
	assert(String_insert(x, 0, "gh") == x);
	assert(String_size(x) == 8);
	assert(strcmp("ghabcdef", String_c_str(x)) == 0);
	assert(String_insert(x, 1, "ijk") == x);
	assert(String_size(x) == 11);
	assert(strcmp("gijkhabcdef", String_c_str(x)) == 0);
	assert(String_insert(x, String_size(x), "lmn") == x);
	assert(String_size(x) == 14);
	assert(strcmp("gijkhabcdeflmn", String_c_str(x)) == 0);
	assert(String_insert_len(x, String_size(x)-1, "opqr", 3) == x);
	assert(String_size(x) == 17);
	assert(strcmp("gijkhabcdeflmopqn", String_c_str(x)) == 0);
	assert(String_insert_len(x, 5, "rst", 0) == x);
	assert(String_size(x) == 17);
	assert(strcmp("gijkhabcdeflmopqn", String_c_str(x)) == 0);
	assert(String_insert(x, 1, String_c_str(x)) == x);
	assert(String_size(x) == 34);
	assert(strcmp("ggijkhabcdeflmopqnijkhabcdeflmopqn", String_c_str(x)) == 0);
	String_erase(x, 1, 17);
	assert(String_size(x) == 17);
	assert(strcmp("gijkhabcdeflmopqn", String_c_str(x)) == 0);

	assert(String_insert_len(x, 0, String_c_str(x) + 1, 4) == x);
	assert(String_size(x) == 21);
	assert(strcmp("ijkhgijkhabcdeflmopqn", String_c_str(x)) == 0);
	String_erase(x, 0, 4);
	assert(String_size(x) == 17);
	assert(strcmp("gijkhabcdeflmopqn", String_c_str(x)) == 0);

	assert(String_insert_len(x, 3, String_c_str(x) + 1, 4) == x);
	assert(String_size(x) == 21);
	assert(strcmp("gijijkhkhabcdeflmopqn", String_c_str(x)) == 0);
	String_erase(x, 3, 4);
	assert(String_size(x) == 17);
	assert(strcmp("gijkhabcdeflmopqn", String_c_str(x)) == 0);

	assert(String_insert_len(x, 5, String_c_str(x) + 1, 4) == x);
	assert(String_size(x) == 21);
	assert(strcmp("gijkhijkhabcdeflmopqn", String_c_str(x)) == 0);

	String_clear(x);
	assert(String_size(x) == 0);
	/* insert_c */
	assert(String_insert_c(x, 0, 5, 'a') == x);
	assert(String_size(x) == 5);
	assert(strcmp("aaaaa", String_c_str(x)) == 0);
	assert(String_insert_c(x, 0, 3, 'b') == x);
	assert(String_size(x) == 8);
	assert(strcmp("bbbaaaaa", String_c_str(x)) == 0);
	assert(String_insert_c(x, 1, 2, 'c') == x);
	assert(String_size(x) == 10);
	assert(strcmp("bccbbaaaaa", String_c_str(x)) == 0);
	assert(String_insert_c(x, 2, 1, 'd') == x);
	assert(String_size(x) == 11);
	assert(strcmp("bcdcbbaaaaa", String_c_str(x)) == 0);
	assert(String_insert_c(x, String_size(x), 3, 'e') == x);
	assert(String_size(x) == 14);
	assert(strcmp("bcdcbbaaaaaeee", String_c_str(x)) == 0);
	assert(String_insert_c(x, String_size(x)-1, 2, 'f') == x);
	assert(String_size(x) == 16);
	assert(strcmp("bcdcbbaaaaaeeffe", String_c_str(x)) == 0);
	assert(String_insert_c(x, 5, 0, 'g') == x);
	assert(String_size(x) == 16);
	assert(strcmp("bcdcbbaaaaaeeffe", String_c_str(x)) == 0);
	assert(String_insert_c(x, 0, 17, 'g') == x);
	assert(String_size(x) == 33);
	assert(strcmp("gggggggggggggggggbcdcbbaaaaaeeffe", String_c_str(x)) == 0);
	String_clear(x);
	assert(String_size(x) == 0);


#if 0
	string s;
	s.insert(0, "abc", 3);
	printf("%s\n", s.c_str());
	s.insert(0, "def", 3);
	printf("%s\n", s.c_str());
	s.erase(0, string::npos);
	printf("%s\n", s.c_str());
#endif

	String_delete(x);
}

int cmp(const void *x, const void *y)
{
	if (*(char*)x < *(char*)y) {
		return -1;
	} else if (*(char*)x > *(char*)y) {
		return 1;
	} else {
		return 0;
	}
}

void StringTest_test_1_4(void)
{
	String *x;
	printf("***** test_1_4 *****\n");
	x = String_new(SIZE);
	String_assign(x, "abcdefghijklmnopqrstuvwxyz");
	assert(String_size(x) == 26);
	assert(strcmp("abcdefghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	/* replace */
	assert(String_replace(x, 0, 3, "ABC") == x);
	assert(String_size(x) == 26);
	assert(strcmp("ABCdefghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	assert(String_replace(x, 6, 3, "GHI") == x);
	assert(String_size(x) == 26);
	assert(strcmp("ABCdefGHIjklmnopqrstuvwxyz", String_c_str(x)) == 0);
	assert(String_replace(x, String_size(x)-3, CSTL_NPOS, "XYZ") == x);
	assert(String_size(x) == 26);
	assert(strcmp("ABCdefGHIjklmnopqrstuvwXYZ", String_c_str(x)) == 0);
	String_shrink(x, 0);
	assert(String_replace(x, 3, 0, "DEF") == x);
	assert(String_size(x) == 29);
	assert(strcmp("ABCDEFdefGHIjklmnopqrstuvwXYZ", String_c_str(x)) == 0);
	assert(String_replace(x, 6, 3, "") == x);
	assert(String_size(x) == 26);
	assert(strcmp("ABCDEFGHIjklmnopqrstuvwXYZ", String_c_str(x)) == 0);
	assert(String_replace_len(x, 0, 3, "hogehoge", 4) == x);
	assert(String_size(x) == 27);
	assert(strcmp("hogeDEFGHIjklmnopqrstuvwXYZ", String_c_str(x)) == 0);

	assert(String_replace_len(x, 3, 4, String_c_str(x), 4) == x);
	assert(String_size(x) == 27);
	printf("%s\n", String_c_str(x));
	assert(strcmp("hoghogeGHIjklmnopqrstuvwXYZ", String_c_str(x)) == 0);

	assert(String_replace_len(x, 2, 4, String_c_str(x), 3) == x);
	assert(String_size(x) == 26);
	printf("%s\n", String_c_str(x));
	assert(strcmp("hohogeGHIjklmnopqrstuvwXYZ", String_c_str(x)) == 0);

	assert(String_replace_len(x, 4, 4, String_c_str(x) + 6, 3) == x);
	assert(String_size(x) == 25);
	printf("%s\n", String_c_str(x));
	assert(strcmp("hohoGHIIjklmnopqrstuvwXYZ", String_c_str(x)) == 0);

	String_erase(x, 0, 8);
	assert(String_size(x) == 17);
	printf("%s\n", String_c_str(x));
	assert(strcmp("jklmnopqrstuvwXYZ", String_c_str(x)) == 0);

	assert(String_replace_len(x, 2, 4, String_c_str(x), 5) == x);
	assert(String_size(x) == 18);
	printf("%s\n", String_c_str(x));
	assert(strcmp("jkjklmnpqrstuvwXYZ", String_c_str(x)) == 0);

	assert(String_replace_len(x, 4, 4, String_c_str(x) + 6, 5) == x);
	assert(String_size(x) == 19);
	printf("%s\n", String_c_str(x));
	assert(strcmp("jkjknpqrsqrstuvwXYZ", String_c_str(x)) == 0);


	assert(String_replace(x, 0, CSTL_NPOS, "") == x);
	assert(String_size(x) == 0);
	assert(strcmp("", String_c_str(x)) == 0);

	assert(String_replace(x, 0, CSTL_NPOS, "abc") == x);
	assert(String_size(x) == 3);
	assert(strcmp("abc", String_c_str(x)) == 0);

	/* replace_c */
	String_assign(x, "abcdefghijklmnopqrstuvwxyz");
	assert(String_size(x) == 26);
	assert(strcmp("abcdefghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	assert(String_replace_c(x, 0, 3, 3, 'A') == x);
	assert(String_size(x) == 26);
	assert(strcmp("AAAdefghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	assert(String_replace_c(x, 6, 3, 3, 'G') == x);
	assert(String_size(x) == 26);
	assert(strcmp("AAAdefGGGjklmnopqrstuvwxyz", String_c_str(x)) == 0);
	assert(String_replace_c(x, String_size(x)-3, CSTL_NPOS, 3, 'X') == x);
	assert(String_size(x) == 26);
	assert(strcmp("AAAdefGGGjklmnopqrstuvwXXX", String_c_str(x)) == 0);
	String_shrink(x, 0);
	assert(String_replace_c(x, 3, 0, 3, 'D') == x);
	assert(String_size(x) == 29);
	assert(strcmp("AAADDDdefGGGjklmnopqrstuvwXXX", String_c_str(x)) == 0);
	assert(String_replace_c(x, 6, 3, 0, 'E') == x);
	assert(String_size(x) == 26);
	assert(strcmp("AAADDDGGGjklmnopqrstuvwXXX", String_c_str(x)) == 0);
	assert(String_replace_c(x, 0, 3, 4, '0') == x);
	assert(String_size(x) == 27);
	assert(strcmp("0000DDDGGGjklmnopqrstuvwXXX", String_c_str(x)) == 0);
	assert(String_replace_c(x, 0, CSTL_NPOS, 0, '0') == x);
	assert(String_size(x) == 0);
	assert(strcmp("", String_c_str(x)) == 0);
	assert(String_replace_c(x, 0, CSTL_NPOS, 10, '0') == x);
	assert(String_size(x) == 10);
	assert(strcmp("0000000000", String_c_str(x)) == 0);
	/* sort */
	String_assign(x, "asortingexample");
	printf("String_sort\n");
	printf("%s\n", String_c_str(x));
	String_sort(x, 0, String_size(x), cmp);
	printf("%s\n", String_c_str(x));

	assert(String_insert(String_append(String_assign(x, "hoge"), "piyo"), 0, "foo") == x);
	printf("%s\n", String_c_str(x));

#if 0
	string s = "abcdefghijklmnopqrstuvwxyz";
	s.replace(1, string::npos, "hoge");
	printf("%s\n", s.c_str());
#endif

	POOL_DUMP_OVERFLOW(&pool);
	String_delete(x);
}

void print_find_test(const char *cstr, string &t, size_t i, size_t j, size_t k, size_t idx1, size_t idx2, const char *f)
{
	if (idx1 != idx2) {
		if (k == CSTL_NPOS) {
			t.assign(cstr);
		} else {
			t.assign(cstr, k);
		}
		printf("cstr[%s]\ni[%3d], j[%3d], k[%3d], idx1[%3d], idx2[%3d] :%s\n", t.c_str(), i, j, k, idx1, idx2, f);
	}
}

void find_test(String *x, const string &s, size_t i, size_t j, size_t k, const char *cstr)
{
	string t;
	size_t idx1, idx2;
	/* find */
	idx1 = String_find_len(String_c_str(x), &cstr[j], i, k);
	idx2 = (k == CSTL_NPOS) ? s.find(&cstr[j], i) : s.find(&cstr[j], i, k);
	/* STLの実装によって動作が違うことがある */
	if (i == CSTL_NPOS) {
		assert(idx1 == CSTL_NPOS);
	} else {
		print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find");
		assert(idx1 == idx2);
	}
	/* find_c */
	idx1 = String_find_c(String_c_str(x), cstr[j], i);
	idx2 = s.find(cstr[j], i);
	print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find_c");
	assert(idx1 == idx2);
	assert(String_find_c(String_c_str(x), cstr[j], i) == String_find_first_of_c(String_c_str(x), cstr[j], i));

	/* rfind */
	idx1 = String_rfind_len(String_c_str(x), &cstr[j], i, k);
	idx2 = (k == CSTL_NPOS) ? s.rfind(&cstr[j], i) : s.rfind(&cstr[j], i, k);
	print_find_test(&cstr[j], t, i, j, k, idx2, idx2, "rfind");
	assert(idx1 == idx2);
	/* rfind_c */
	idx1 = String_rfind_c(String_c_str(x), cstr[j], i);
	idx2 = s.rfind(cstr[j], i);
	print_find_test(&cstr[j], t, i, j, k, idx2, idx2, "rfind_c");
	assert(idx1 == idx2);
	assert(String_rfind_c(String_c_str(x), cstr[j], i) == String_find_last_of_c(String_c_str(x), cstr[j], i));

	/* find_first_of */
	idx1 = String_find_first_of_len(String_c_str(x), &cstr[j], i, k);
	idx2 = (k == CSTL_NPOS) ? s.find_first_of(&cstr[j], i) : s.find_first_of(&cstr[j], i, k);
	print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find_first_of");
	assert(idx1 == idx2);
	/* find_first_of_c */
	idx1 = String_find_first_of_c(String_c_str(x), cstr[j], i);
	idx2 = s.find_first_of(cstr[j], i);
	print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find_first_of_c");
	assert(idx1 == idx2);

	/* find_last_of */
	idx1 = String_find_last_of_len(String_c_str(x), &cstr[j], i, k);
	idx2 = (k == CSTL_NPOS) ? s.find_last_of(&cstr[j], i) : s.find_last_of(&cstr[j], i, k);
	print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find_last_of");
	assert(idx1 == idx2);
	/* find_last_of_c */
	idx1 = String_find_last_of_c(String_c_str(x), cstr[j], i);
	idx2 = s.find_last_of(cstr[j], i);
	print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find_last_of_c");
	assert(idx1 == idx2);

	/* find_first_not_of */
	idx1 = String_find_first_not_of_len(String_c_str(x), &cstr[j], i, k);
	idx2 = (k == CSTL_NPOS) ? s.find_first_not_of(&cstr[j], i) : s.find_first_not_of(&cstr[j], i, k);
	print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find_first_not_of");
	assert(idx1 == idx2);
	/* find_first_not_of_c */
	idx1 = String_find_first_not_of_c(String_c_str(x), cstr[j], i);
	idx2 = s.find_first_not_of(cstr[j], i);
	print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find_first_not_of_c");
	assert(idx1 == idx2);

	/* find_last_not_of */
	idx1 = String_find_last_not_of_len(String_c_str(x), &cstr[j], i, k);
	idx2 = (k == CSTL_NPOS) ? s.find_last_not_of(&cstr[j], i) : s.find_last_not_of(&cstr[j], i, k);
	print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find_last_not_of");
	assert(idx1 == idx2);
	/* find_last_not_of_c */
	idx1 = String_find_last_not_of_c(String_c_str(x), cstr[j], i);
	idx2 = s.find_last_not_of(cstr[j], i);
	print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find_last_not_of_c");
	assert(idx1 == idx2);
}

void StringTest_test_1_5(void)
{
	String *x;
	String *y;
	string s;
	size_t i, j, k;
	printf("***** test_1_5 *****\n");
	x = String_new(SIZE);
	y = String_new(SIZE);
	String_assign(x, "abc abcd abcde abcdef abcdefg abcdefgh abcdefghi");
	String_assign(y, "abcdefghijklmnopqrstuvwxyz");
	s = String_c_str(x);
	assert(strcmp(s.c_str(), String_c_str(x)) == 0);
	for (i = 0; i < String_size(x); i++) {
		for (j = 0; j < String_size(x); j++) {
			for (k = 0; k < String_size(x) -j +1; k++) {
				find_test(x, s, i, j, k, String_c_str(x));
			}
			if (k == String_size(x) -j +1) {
				find_test(x, s, i, j, CSTL_NPOS, String_c_str(x));
			}
			if (j <= String_size(y)) {
				for (k = 0; k < String_size(y) -j +1; k++) {
					find_test(x, s, i, j, k, String_c_str(y));
				}
				if (k == String_size(y) -j +1) {
					find_test(x, s, i, j, CSTL_NPOS, String_c_str(y));
				}
			}
		}
	}
	for (j = 0; j < String_size(x); j++) {
		for (k = 0; k < String_size(x) -j +1; k++) {
			find_test(x, s, CSTL_NPOS, j, k, String_c_str(x));
		}
		if (k == String_size(x) -j +1) {
			find_test(x, s, CSTL_NPOS, j, CSTL_NPOS, String_c_str(x));
		}
		if (j <= String_size(y)) {
			for (k = 0; k < String_size(y) -j +1; k++) {
				find_test(x, s, CSTL_NPOS, j, k, String_c_str(y));
			}
			if (k == String_size(y) -j +1) {
				find_test(x, s, CSTL_NPOS, j, CSTL_NPOS, String_c_str(y));
			}
		}
	}

	String_delete(x);
	String_delete(y);
}


void StringTest_run(void)
{
	printf("\n===== string test =====\n");
	StringTest_test_1_1();
	StringTest_test_1_2();
	StringTest_test_1_3();
	StringTest_test_1_4();
	StringTest_test_1_5();
}


int main(void)
{
#ifdef MY_MALLOC
	Pool_init(&pool, buf, sizeof buf, sizeof buf[0]);
#endif
	StringTest_run();
#ifdef MY_MALLOC
	POOL_DUMP_LEAK(&pool, 0);
#endif
	return 0;
}
