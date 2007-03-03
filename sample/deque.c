#include <stdio.h>
#include "../cstl/deque.h"
#define NELEMS(array)	(sizeof(array) / sizeof(array[0]))

DEQUE_INTERFACE(UCharDeque, unsigned char)
DEQUE_IMPLEMENT(UCharDeque, unsigned char)

int main(void)
{
	int i;
	unsigned char buf[32];
	UCharDeque *x;
	UCharDeque y;

	x = UCharDeque_new(31);
	UCharDeque_push_back(x, 0x01);
	UCharDeque_push_back(x, 0x02);
	UCharDeque_push_front(x, 0x03);
	UCharDeque_push_front(x, 0x04);
	UCharDeque_insert(x, 1, 0x05);
	UCharDeque_erase(x, 2);
	*UCharDeque_at(x, 0) = 0x06;
	for (i = 0; i < UCharDeque_size(x); i++) {
		printf("%02x\n", *UCharDeque_at(x, i));
	}
	while (!UCharDeque_empty(x)) {
		printf("%02x\n", UCharDeque_pop_front(x));
	}
	UCharDeque_delete(x);

	/* 呼び出し側で用意したオブジェクトとバッファを利用する場合 */
	UCharDeque_init(&y, buf, NELEMS(buf));
	UCharDeque_push_back(&y, 0x01);
	UCharDeque_push_back(&y, 0x02);
	while (!UCharDeque_empty(&y)) {
		printf("%02x\n", UCharDeque_pop_front(&y));
	}
	/* UCharDeque_delete(&y)を呼び出してはいけない */
	return 0;
}

