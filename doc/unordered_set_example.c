#include <stdio.h>
#include <cstl/unordered_set.h>

CSTL_UNORDERED_SET_INTERFACE(IntUSet, int)				/* インターフェイスを展開 */
CSTL_UNORDERED_SET_IMPLEMENT(IntUSet, int, IntUSet_hash_int, CSTL_EQUAL_TO)	/* 実装を展開 */

int main(void)
{
	int i;
	/* イテレータ */
	IntUSetIterator pos;
	/* intのunordered_setを生成。
	 * 型名・関数のプレフィックスはIntUSetとなる。 */
	IntUSet *uset = IntUSet_new();

	/* 要素を挿入 */
	for (i = 0; i < 64; i++) {
		IntUSet_insert(uset, i, NULL);
	}
	/* 要素数 */
	printf("size: %d\n", IntUSet_size(uset));
	for (pos = IntUSet_begin(uset); pos != IntUSet_end(uset); pos = IntUSet_next(pos)) {
		/* イテレータによる要素の読み出し(書き換えはできない) */
		printf("%d, ", *IntUSet_data(pos));
	}
	printf("\n");

	/* 3以上の要素を削除 */
	IntUSet_erase_range(uset, IntUSet_find(uset, 3), IntUSet_end(uset));

	for (pos = IntUSet_begin(uset); pos != IntUSet_end(uset); pos = IntUSet_next(pos)) {
		/* イテレータによる要素の読み出し(書き換えはできない) */
		printf("%d, ", *IntUSet_data(pos));
	}
	printf("\n");

	/* 使い終わったら破棄 */
	IntUSet_delete(uset);
	return 0;
}
