#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstl/vector.h>
#include <cstl/algorithm.h> /* CSTL_VECTOR_INTERFACE()の前にインクルード */

/* vectorのインターフェイスと実装を展開 */
CSTL_VECTOR_INTERFACE(IntVector, int)
CSTL_VECTOR_IMPLEMENT(IntVector, int)

/* intの比較関数 */
int int_less(const void *p1, const void *p2)
{
	if (*(int*)p1 < *(int*)p2) {
		return -1;
	} else if (*(int*)p1 > *(int*)p2) {
		return 1;
	} else {
		return 0;
	}
}

int main(void)
{
	int i;
	size_t idx;
	/* intのvectorを生成。
	 * 型名・関数のプレフィックスはIntVectorとなる。 */
	IntVector *vec = IntVector_new();

	srand(time(0));
	for (i = 0; i < 64; i++) {
		/* 末尾から100未満のランダムな値の要素を追加 */
		IntVector_push_back(vec, rand() % 100);
	}
	/* ソート */
	IntVector_sort(vec, 0, IntVector_size(vec), int_less);
	for (i = 0; i < IntVector_size(vec); i++) {
		printf("%d, ", *IntVector_at(vec, i));
	}
	printf("\n");
	/* 50以上の最初の要素のインデックス */
	idx = IntVector_lower_bound(vec, 0, IntVector_size(vec), 50, int_less);
	/* 先頭から50未満の要素までを逆順に並べ替え */
	IntVector_reverse(vec, 0, idx);
	for (i = 0; i < IntVector_size(vec); i++) {
		printf("%d, ", *IntVector_at(vec, i));
	}

	/* 使い終わったら破棄 */
	IntVector_delete(vec);
	return 0;
}
