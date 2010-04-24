#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstl/list.h>


/* Hogeクラス */
typedef struct {
	const char *key;
	int value;
} Hoge;

Hoge *Hoge_new(const char *key, int value)
{
	Hoge *self = (Hoge *) malloc(sizeof(Hoge));
	self->key = key;
	self->value = value;
	return self;
}

void Hoge_delete(Hoge *self)
{
	free(self);
}

void Hoge_print(Hoge *self)
{
	printf("%s: %d\n", self->key, self->value);
}

/* 比較関数 */
int int_comp(const void *p1, const void *p2)
{
	return *(int*)p1 - *(int*)p2;
/*    return *(int*)p2 - *(int*)p1;*/
}

int comp(const void *p1, const void *p2)
{
	return strcmp((*((Hoge**)p1))->key, (*((Hoge**)p2))->key);
}

/* listのインターフェイスと実装を展開 */
CSTL_LIST_INTERFACE(HogeList, Hoge *)
CSTL_LIST_IMPLEMENT(HogeList, Hoge *)
CSTL_LIST_INTERFACE(IntList, int)
CSTL_LIST_IMPLEMENT(IntList, int)
CSTL_LIST_INTERFACE(Int2List, int) /* IntListとは違う型を定義(要素の型は同じ) */
CSTL_LIST_IMPLEMENT(Int2List, int)


int main(void)
{
	Hoge *hoge;

	HogeList *hl;
	IntList *il;
	Int2List *il2;

	HogeListIterator pos;
	IntListIterator iter;
	Int2ListIterator iter2;

	/* 生成は 型名_new() という関数になる */
	hl = HogeList_new(); /* C++イメージ: hl = new list<Hoge *>; */
	il = IntList_new();
	il2 = Int2List_new();

	/* 末尾から追加 */
	hoge = Hoge_new("aaa", 1);
	cstl_push_back(hl, hoge); /* C++イメージ: hl->push_back(hoge); */
	hoge = Hoge_new("bbb", 2);
	cstl_push_back(hl, hoge);

	cstl_push_back(il, 0);
	cstl_push_back(il, 1);
	cstl_push_back(il, 2);

	cstl_push_back(il2, 10);
	cstl_push_back(il2, 11);
	cstl_push_back(il2, 12);

	/* 先頭から追加 */
	hoge = Hoge_new("ccc", 3);
	cstl_push_front(hl, hoge);
	hoge = Hoge_new("ddd", 4);
	cstl_push_front(hl, hoge);

	cstl_push_front(il, 3);
	cstl_push_front(il, 4);
	cstl_push_front(il, 5);
	/* 要素数 */
	printf("size: %d\n", cstl_size(hl));
	printf("il: size: %d\n", cstl_size(il));
	printf("il2: size: %d\n", cstl_size(il2));

	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_incr(pos)) {
		/* C++イメージ: for (pos = hl->begin(); pos != hl->end(); ++pos) */
		/* イテレータによる要素のアクセス */
		Hoge_print(*cstl_iter_data(pos)); /* C++イメージ: Hoge_print(*pos); */
	}
	printf("\n");
	printf("sort\n");
	cstl_sort(hl, comp);
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_incr(pos)) {
		/* イテレータによる要素のアクセス */
		Hoge_print(*cstl_iter_data(pos));
	}
	printf("\n");

	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_incr(pos)) {
		/* イテレータによる要素のアクセス */
		Hoge_print(*cstl_iter_data(pos));
		(*cstl_iter_data(pos))->value++;
		Hoge_print(*cstl_iter_data(pos));
	}
	printf("\n");
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_incr(pos)) {
		Hoge *h = *cstl_iter_data(pos);
		if (!strcmp(h->key, "bbb")) {
			hoge = Hoge_new("eee", 5);
			/* 要素の挿入 */
			cstl_insert(hl, pos, hoge, NULL);
		}
	}
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); ) {
		Hoge *h = *cstl_iter_data(pos);
		if (!strcmp(h->key, "bbb")) {
			Hoge_delete(h);
			/* 要素の削除 */
			pos = cstl_erase(hl, pos);
		} else {
			cstl_iter_incr(pos);
		}
	}

	for (iter = cstl_begin(il); cstl_iter_ne(iter, cstl_end(il)); cstl_iter_incr(iter)) {
		printf("il: %d\n", *cstl_iter_data(iter));
	}
	for (iter2 = cstl_begin(il2); cstl_iter_ne(iter2, cstl_end(il2)); cstl_iter_incr(iter2)) {
		printf("il2: %d\n", *cstl_iter_data(iter2));
	}
	printf("\n");

	/* 違う型のオブジェクトの要素を挿入。要素の型は同じintなので可能 */
	cstl_insert_range(il, cstl_end(il), cstl_begin(il2), cstl_end(il2));
	printf("insert_range\n");

	for (iter = cstl_begin(il); cstl_iter_ne(iter, cstl_end(il)); cstl_iter_incr(iter)) {
		printf("il: %d\n", *cstl_iter_data(iter));
	}
	printf("\n");

	printf("sort\n");
	cstl_sort(il, int_comp);

	for (iter = cstl_begin(il); cstl_iter_ne(iter, cstl_end(il)); cstl_iter_incr(iter)) {
		printf("il: %d\n", *cstl_iter_data(iter));
	}
	printf("\n");

	/* 先頭から全要素を削除 */
	while (!cstl_empty(hl)) {
		Hoge *h = *cstl_front(hl);
		Hoge_print(h);
		Hoge_delete(h);
		cstl_pop_front(hl);
	}
	while (!cstl_empty(il)) {
		cstl_pop_front(il);
	}

	/* 使い終わったら破棄 */
	cstl_delete(hl); /* C++イメージ: delete hl; */
	cstl_delete(il);
	cstl_delete(il2);

	return 0;
}
