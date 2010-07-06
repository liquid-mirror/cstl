#include <stdio.h>
#include <cstl/deque.h>

CSTL_DEQUE_INTERFACE(UCharDeque, unsigned char)
CSTL_DEQUE_IMPLEMENT(UCharDeque, unsigned char)

int main(void)
{
	int i;
	UCharDeque *x;
	UCharDequeIterator iter;

	x = UCharDeque_new();
	cstl_push_back(x, 0x01);
	cstl_push_back(x, 0x02);
	cstl_push_front(x, 0x03);
	cstl_push_front(x, 0x04);
	for (i = 0; i < cstl_size(x); i++) { printf("%02x\n", *cstl_at(x, i)); }
	printf("\n");

	iter = cstl_begin(x);
	cstl_insert(x, cstl_iter_add(iter, 1), 0x05, NULL);
	for (i = 0; i < cstl_size(x); i++) { printf("%02x\n", *cstl_at(x, i)); }
	printf("\n");

	cstl_erase_range(x, cstl_iter_add(iter, 2), cstl_iter_add(iter, 3));
	for (i = 0; i < cstl_size(x); i++) { printf("%02x\n", *cstl_at(x, i)); }
	printf("\n");

	*cstl_at(x, 0) = 0x06;
	for (i = 0; i < cstl_size(x); i++) { printf("%02x\n", *cstl_at(x, i)); }
	printf("\n");

	while (!cstl_empty(x)) {
		printf("%02x\n", *cstl_front(x));
		cstl_pop_front(x);
	}
	cstl_delete(x);
	return 0;
}

