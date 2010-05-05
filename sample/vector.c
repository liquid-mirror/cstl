#include <stdio.h>
#include <cstl/vector.h>

CSTL_VECTOR_INTERFACE(CharVector, char)
CSTL_VECTOR_IMPLEMENT(CharVector, char)

int main(void)
{
	CharVector *x = CharVector_new();
	CharVectorIterator iter;
	CharVectorReverseIterator riter;
	cstl_insert_array(x, cstl_end(x), "aaaa", 4);
	cstl_push_back(x, '1');
	cstl_push_back(x, '2');
	cstl_push_back(x, '3');
	cstl_push_back(x, '4');
	cstl_push_back(x, '5');
	cstl_push_back(x, '6');
	printf("diff beg - end:%d\n", cstl_iter_diff(cstl_begin(x), cstl_end(x)));
	printf("diff end - beg:%d\n", cstl_iter_diff(cstl_end(x), cstl_begin(x)));
	printf("size:%d\n", cstl_size(x));
	cstl_insert_array(x, cstl_iter_sub(cstl_end(x), 10), "bcde", 4);
	cstl_push_back(x, '\0');
	cstl_erase(x, cstl_begin(x));
	cstl_erase(x, cstl_iter_sub(cstl_end(x), 2));
	printf("%s\n", cstl_at(x, 0));
	printf("size:%d\n", cstl_size(x));
	for (iter = cstl_begin(x); cstl_iter_ne(iter, cstl_end(x)); cstl_iter_incr(&iter)) {
		CharVectorReverseIterator rpos;
		CharVectorIterator pos;
		CharVectorReverseIterator_init(&rpos, iter);
/*        printf("%c\n", *cstl_iter_data(iter));*/
		printf("%c, %c\n", *cstl_iter_data(iter), *cstl_iter_data(rpos));
		pos = cstl_riter_base(rpos);
		printf("%c\n", *cstl_iter_data(pos));
	}
	printf("reverse iter\n");
	for (riter = cstl_rbegin(x); cstl_iter_ne(riter, cstl_rend(x)); cstl_iter_incr(&riter)) {
		printf("%c\n", *cstl_iter_data(riter));
	}
/*    cstl_erase_range(x, cstl_begin(x), cstl_end(x));*/
/*    printf("size:%d\n", cstl_size(x));*/
#if 1

	cstl_pop_back(x);
	cstl_insert_array(x, cstl_end(x), "bbbb", 4);
	cstl_push_back(x, '\0');
	printf("%s\n", cstl_at(x, 0));
	printf("size:%d\n", cstl_size(x));

	*cstl_at(x, 0) = 'c';
	*cstl_at(x, 1) = 'd';
	*cstl_at(x, 4) = 'e';
	*cstl_at(x, 5) = 'f';
	printf("%s\n", cstl_at(x, 0));

/*    cstl_erase(x, 2, 4);*/
	iter = cstl_iter_add(cstl_begin(x), 2);
	cstl_erase_range(x, iter, cstl_iter_add(iter, 4));
	printf("%s\n", cstl_at(x, 0));

	cstl_insert(x, cstl_begin(x), 'g', NULL);
	cstl_insert(x, cstl_begin(x), 'h', NULL);
	cstl_insert(x, cstl_begin(x), 'i', NULL);
	cstl_insert_array(x, cstl_begin(x), "jklm", 4);
	printf("%s\n", cstl_at(x, 0));

#endif
	cstl_delete(x);
	return 0;
}
