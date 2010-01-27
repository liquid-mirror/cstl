#include <stdio.h>
#include <cstl/deque.h>

/* dequeのインターフェイスと実装を展開 */
CSTL_DEQUE_INTERFACE(IntDeque, int)
CSTL_DEQUE_IMPLEMENT(IntDeque, int)

int main(void)
{
	int i;
	/* intのdequeを生成。
	 * 型名・関数のプレフィックスはIntDequeとなる。 */
	IntDeque *deq = IntDeque_new();

	for (i = 0; i < 32; i++) {
		/* 末尾から追加 */
		IntDeque_push_back(deq, i);
	}
	for (i = 0; i < 32; i++) {
		/* 先頭から追加 */
		IntDeque_push_front(deq, i);
	}
	/* 要素数 */
	printf("size: %d\n", IntDeque_size(deq));
	for (i = 0; i < IntDeque_size(deq); i++) {
		/* インデックスによる要素の読み書き */
		printf("%d,", *IntDeque_at(deq, i));
		*IntDeque_at(deq, i) += 1;
		printf("%d\n", *IntDeque_at(deq, i));
	}

	/* 先頭から全要素を取り出して削除 */
	while (!IntDeque_empty(deq)) {
		printf("%d\n", *IntDeque_front(deq));
		IntDeque_pop_front(deq);
	}
	printf("size: %d\n", IntDeque_size(deq));

	/* 使い終わったら破棄 */
	IntDeque_delete(deq);
	return 0;
}

