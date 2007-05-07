#include <stdio.h>
#include <cstl/deque.h>

DEQUE_INTERFACE(UCharDeque, unsigned char)
DEQUE_IMPLEMENT(UCharDeque, unsigned char)

int main(void)
{
	int i;
	UCharDeque *x;

	x = UCharDeque_new();
	UCharDeque_push_back(x, 0x01);
	UCharDeque_push_back(x, 0x02);
	UCharDeque_push_front(x, 0x03);
	UCharDeque_push_front(x, 0x04);
	UCharDeque_insert(x, 1, 0x05);
	UCharDeque_erase(x, 2, 1);
	*UCharDeque_at(x, 0) = 0x06;
	for (i = 0; i < UCharDeque_size(x); i++) {
		printf("%02x\n", *UCharDeque_at(x, i));
	}
	while (!UCharDeque_empty(x)) {
		printf("%02x\n", UCharDeque_pop_front(x));
	}
	UCharDeque_delete(x);
	return 0;
}

