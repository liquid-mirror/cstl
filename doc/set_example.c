#include <stdio.h>
#include <cstl/set.h>

/* setのインターフェイスと実装を展開 */
CSTL_SET_INTERFACE(IntSet, int)
CSTL_SET_IMPLEMENT(IntSet, int, CSTL_LESS)

int main(void)
{
	int i;
	/* イテレータ */
	IntSetIterator pos;
	/* intのsetを生成。
	 * 型名・関数のプレフィックスはIntSetとなる。 */
	IntSet *set = IntSet_new();

	/* 要素を挿入 */
	for (i = 0; i < 64; i++) {
		IntSet_insert(set, i, NULL);
	}
	/* 要素数 */
	printf("size: %d\n", IntSet_size(set));
	for (pos = IntSet_begin(set); pos != IntSet_end(set); 
			pos = IntSet_next(pos)) {
		/* イテレータによる要素の読み出し(書き換えはできない) */
		printf("%d, ", *IntSet_data(pos));
	}
	printf("\n");

	/* 3以上の要素を削除 */
	IntSet_erase_range(set, IntSet_find(set, 3), IntSet_end(set));

	for (pos = IntSet_begin(set); pos != IntSet_end(set); 
			pos = IntSet_next(pos)) {
		/* イテレータによる要素の読み出し(書き換えはできない) */
		printf("%d, ", *IntSet_data(pos));
	}
	printf("\n");

	/* 使い終わったら破棄 */
	IntSet_delete(set);
	return 0;
}
