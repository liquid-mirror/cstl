#include <stdio.h>
#include <cstl/vector.h>

/* vectorのインターフェイスと実装を展開 */
CSTL_VECTOR_INTERFACE(IntVector, int)
CSTL_VECTOR_IMPLEMENT(IntVector, int)

int main(void)
{
	int i;
	/* intのvectorを生成。
	 * 型名・関数のプレフィックスはIntVectorとなる。 */
	IntVector *vec = IntVector_new();

	for (i = 0; i < 64; i++) {
		/* 末尾から要素を追加。自動的に拡張する */
		IntVector_push_back(vec, i);
	}

	/* 許容量の拡張 */
	printf("capacity: %d\n", IntVector_capacity(vec));
	IntVector_reserve(vec, 128);

	for (i = 0; i < 64; i++) {
		/* 先頭に要素を挿入 */
		IntVector_insert(vec, 0, i);
	}
	/* 要素数 */
	printf("size: %d\n", IntVector_size(vec));
	for (i = 0; i < IntVector_size(vec); i++) {
		/* インデックスによる要素の読み書き */
		printf("%d,", *IntVector_at(vec, i));
		*IntVector_at(vec, i) += 1;
		printf("%d\n", *IntVector_at(vec, i));
	}

	/* 先頭から要素数の半分削除 */
	IntVector_erase(vec, 0, IntVector_size(vec) / 2);
	printf("size: %d\n", IntVector_size(vec));

	/* 使い終わったら破棄 */
	IntVector_delete(vec);
	return 0;
}
