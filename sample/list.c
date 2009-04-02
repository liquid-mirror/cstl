#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstl/list.h>

typedef struct Hoge_t {
	char *key;
	int value;
} Hoge;

Hoge *Hoge_new(char *key, int value)
{
	Hoge *self = malloc(sizeof(Hoge));
	self->key = key; self->value = value;
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

CSTL_LIST_INTERFACE(HogeList, Hoge *)
CSTL_LIST_IMPLEMENT(HogeList, Hoge *)

int main(void)
{
	Hoge *hoge;
	HogeListIterator pos;
	HogeList *x = HogeList_new();

	hoge = Hoge_new("aaa", 1);
	HogeList_push_back(x, hoge);
	hoge = Hoge_new("bbb", 2);
	HogeList_push_back(x, hoge);
	hoge = Hoge_new("ccc", 3);
	HogeList_push_front(x, hoge);
	hoge = Hoge_new("ddd", 4);
	HogeList_push_front(x, hoge);

	for (pos = HogeList_begin(x); pos != HogeList_end(x); pos = HogeList_next(pos)) {
		Hoge_print(*HogeList_data(pos));
	}
	for (pos = HogeList_begin(x); pos != HogeList_end(x); pos = HogeList_next(pos)) {
		Hoge *h = *HogeList_data(pos);
		if (!strcmp(h->key, "bbb")) {
			hoge = Hoge_new("eee", 5);
			HogeList_insert(x, pos, hoge);
		}
	}
	for (pos = HogeList_begin(x); pos != HogeList_end(x);) {
		Hoge *h = *HogeList_data(pos);
		if (!strcmp(h->key, "bbb")) {
			pos = HogeList_erase(x, pos);
			Hoge_delete(h);
		} else {
			pos = HogeList_next(pos);
		}
	}
	while (!HogeList_empty(x)) {
		Hoge *h = *HogeList_front(x);
		HogeList_pop_front(x);
		Hoge_print(h);
		Hoge_delete(h);
	}
	HogeList_delete(x);
	return 0;
}

