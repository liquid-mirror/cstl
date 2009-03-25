#include <stdio.h>
#include <cstl/set.h>

CSTL_SET_INTERFACE(IntSet, int)
CSTL_SET_IMPLEMENT(IntSet, int, CSTL_LESS)

int main(void)
{
	int success;
	IntSetIterator pos;
	IntSet *x = IntSet_new();

	IntSet_insert(x, 4, NULL);
	IntSet_insert(x, 3, NULL);
	IntSet_insert(x, 5, NULL);
	IntSet_insert(x, 1, NULL);
	IntSet_insert(x, 6, NULL);
	IntSet_insert(x, 2, NULL);
	IntSet_insert(x, 5, NULL);
	IntSet_insert(x, 8, NULL);
	IntSet_insert(x, 7, NULL);

	for (pos = IntSet_begin(x); pos != IntSet_end(x); pos = IntSetIterator_next(pos)) {
		printf("%d ", *IntSetIterator_elem(pos));
	}
	printf("\n");

	IntSet_insert(x, 4, &success);
	if (success) {
		printf("4 inserted as element\n");
	} else {
		printf("4 already exists\n");
	}

	IntSet_erase_range(x, IntSet_begin(x), IntSet_find(x, 3));
	IntSet_erase_key(x, 5);

	for (pos = IntSet_begin(x); pos != IntSet_end(x); pos = IntSetIterator_next(pos)) {
		printf("%d ", *IntSetIterator_elem(pos));
	}
	printf("\n");

	printf("lower_bound(5): %d\n", *IntSetIterator_elem(IntSet_lower_bound(x, 5)));
	printf("upper_bound(5): %d\n", *IntSetIterator_elem(IntSet_upper_bound(x, 5)));
	printf("lower_bound(6): %d\n", *IntSetIterator_elem(IntSet_lower_bound(x, 6)));
	printf("upper_bound(6): %d\n", *IntSetIterator_elem(IntSet_upper_bound(x, 6)));
	IntSet_delete(x);
	return 0;
}
