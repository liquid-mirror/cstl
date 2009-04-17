#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstl/list.h>

typedef struct {
	char *key;
	int value;
} Hoge;

Hoge *Hoge_new(char *key, int value)
{
	Hoge *self = malloc(sizeof(Hoge));
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

CSTL_LIST_INTERFACE(HogeList, Hoge *) /* インターフェイスを展開 */
CSTL_LIST_IMPLEMENT(HogeList, Hoge *) /* 実装を展開 */

int main(void)
{
	Hoge *hoge;
	/* イテレータ */
	HogeListIterator pos;
	/* (Hoge *)のlistを生成。
	 * 型名・関数のプレフィックスはHogeListとなる。 */
	HogeList *lst = HogeList_new();

	/* 末尾から追加 */
	hoge = Hoge_new("aaa", 1);
	HogeList_push_back(lst, hoge);
	hoge = Hoge_new("bbb", 2);
	HogeList_push_back(lst, hoge);

	/* 先頭から追加 */
	hoge = Hoge_new("ccc", 3);
	HogeList_push_front(lst, hoge);
	hoge = Hoge_new("ddd", 4);
	HogeList_push_front(lst, hoge);

	/* 要素数 */
	printf("size: %d\n", HogeList_size(lst));

	for (pos = HogeList_begin(lst); pos != HogeList_end(lst); pos = HogeList_next(pos)) {
		/* イテレータによる要素のアクセス */
		Hoge_print(*HogeList_data(pos));
		(*HogeList_data(pos))->value++;
		Hoge_print(*HogeList_data(pos));
	}
	for (pos = HogeList_begin(lst); pos != HogeList_end(lst); pos = HogeList_next(pos)) {
		Hoge *h = *HogeList_data(pos);
		if (!strcmp(h->key, "bbb")) {
			hoge = Hoge_new("eee", 5);
			/* 要素の挿入 */
			HogeList_insert(lst, pos, hoge);
		}
	}
	for (pos = HogeList_begin(lst); pos != HogeList_end(lst);) {
		Hoge *h = *HogeList_data(pos);
		if (!strcmp(h->key, "bbb")) {
			Hoge_delete(h);
			/* 要素の削除 */
			pos = HogeList_erase(lst, pos);
		} else {
			pos = HogeList_next(pos);
		}
	}

	/* 先頭から全要素を削除 */
	while (!HogeList_empty(lst)) {
		Hoge *h = *HogeList_front(lst);
		Hoge_print(h);
		Hoge_delete(h);
		HogeList_pop_front(lst);
	}

	/* 使い終わったら破棄 */
	HogeList_delete(lst);
	return 0;
}
