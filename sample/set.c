#include <stdio.h>
#include <cstl/set.h>

CSTL_SET_INTERFACE(IntSet, int)
CSTL_SET_IMPLEMENT(IntSet, int, CSTL_LESS)

int main(void)
{
	int success;
	IntSetIterator pos;
	IntSet *x = IntSet_new();

	cstl_insert_set(x, 4, NULL, NULL);
	cstl_insert_set(x, 3, NULL, NULL);
	cstl_insert_set(x, 5, NULL, NULL);
	cstl_insert_set(x, 1, NULL, NULL);
	cstl_insert_set(x, 6, NULL, NULL);
	cstl_insert_set(x, 2, NULL, NULL);
	cstl_insert_set(x, 5, NULL, NULL);
	cstl_insert_set(x, 8, NULL, NULL);
	cstl_insert_set(x, 7, NULL, NULL);

	for (pos = cstl_begin(x); cstl_iter_ne(pos, cstl_end(x)); cstl_iter_inc(&pos)) {
		printf("%d ", *cstl_iter_data(pos));
	}
	printf("\n");

	cstl_insert_set(x, 4, &pos, &success);
	if (success) {
		printf("4 inserted as element\n");
	} else {
		printf("4 already exists\n");
	}
	printf("%d\n", *cstl_iter_data(pos));

	cstl_erase_range(x, cstl_begin(x), cstl_find(x, 3));
	cstl_erase_key(x, 5);

	for (pos = cstl_begin(x); cstl_iter_ne(pos, cstl_end(x)); cstl_iter_inc(&pos)) {
		printf("%d ", *cstl_iter_data(pos));
	}
	printf("\n");

	printf("lower_bound(5): %d\n", *cstl_iter_data(cstl_lower_bound(x, 5)));
	printf("upper_bound(5): %d\n", *cstl_iter_data(cstl_upper_bound(x, 5)));
	printf("lower_bound(6): %d\n", *cstl_iter_data(cstl_lower_bound(x, 6)));
	printf("upper_bound(6): %d\n", *cstl_iter_data(cstl_upper_bound(x, 6)));
	IntSet_delete(x);
	return 0;
}
