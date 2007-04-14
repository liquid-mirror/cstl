#include <stdio.h>
#include <cstl/ring.h>
#define NELEMS(array)	(sizeof(array) / sizeof(array[0]))

RING_INTERFACE(UCharRing, unsigned char)
RING_IMPLEMENT(UCharRing, unsigned char)

int main(void)
{
	int i;
	unsigned char buf[32];
	UCharRing *x;
	UCharRing y;

	x = UCharRing_new(31);
	UCharRing_push_back(x, 0x01);
	UCharRing_push_back(x, 0x02);
	UCharRing_push_front(x, 0x03);
	UCharRing_push_front(x, 0x04);
	UCharRing_insert(x, 1, 0x05);
	UCharRing_erase(x, 2, 1);
	*UCharRing_at(x, 0) = 0x06;
	for (i = 0; i < UCharRing_size(x); i++) {
		printf("%02x\n", *UCharRing_at(x, i));
	}
	while (!UCharRing_empty(x)) {
		printf("%02x\n", UCharRing_pop_front(x));
	}
	UCharRing_delete(x);

	/* 呼び出し側で用意したオブジェクトとバッファを利用する場合 */
	UCharRing_init(&y, buf, NELEMS(buf));
	UCharRing_push_back(&y, 0x01);
	UCharRing_push_back(&y, 0x02);
	while (!UCharRing_empty(&y)) {
		printf("%02x\n", UCharRing_pop_front(&y));
	}
	/* UCharRing_delete(&y)を呼び出してはいけない */
	return 0;
}

