#include <stdio.h>
#include <string.h>
#include <cstl/map.h>

CSTL_MAP_INTERFACE(StrFltMap, char *, float)
CSTL_MAP_IMPLEMENT(StrFltMap, char *, float, strcmp)

/* 連想配列として使う */
int main(void)
{
	StrFltMapIterator pos;
	StrFltMap *stocks = StrFltMap_new();

	/* 要素の挿入 */
	*cstl_at(stocks, "BASF")    = 369.50;
	*cstl_at(stocks, "VW")      = 413.50;
	*cstl_at(stocks, "Daimler") = 819.00;
	*cstl_at(stocks, "BMW")     = 834.00;
	*cstl_at(stocks, "Siemens") = 842.20;

	/* すべての要素を出力する */
	for (pos = cstl_begin(stocks); cstl_iter_ne(pos, cstl_end(stocks)); cstl_iter_inc(&pos)) {
		printf("stock: %s\tprice: %g\n", *cstl_iter_key(pos), *cstl_iter_val(pos));
	}
	printf("\n");

	/* 値が倍になった */
	for (pos = cstl_begin(stocks); cstl_iter_ne(pos, cstl_end(stocks)); cstl_iter_inc(&pos)) {
		*cstl_iter_val(pos) *= 2;
	}
	/* すべての要素を出力する */
	for (pos = cstl_begin(stocks); cstl_iter_ne(pos, cstl_end(stocks)); cstl_iter_inc(&pos)) {
		printf("stock: %s\tprice: %g\n", *cstl_iter_key(pos), *cstl_iter_val(pos));
	}
	printf("\n");

	/* キーの"VW"を"Volkswagen"に変更する
	 * キーは変更不可なので値をコピーして古いキーを削除するしかない */
	*cstl_at(stocks, "Volkswagen") = *cstl_at(stocks, "VW");
	cstl_erase_key(stocks, "VW");

	/* すべての要素を出力する */
	for (pos = cstl_begin(stocks); cstl_iter_ne(pos, cstl_end(stocks)); cstl_iter_inc(&pos)) {
		printf("stock: %s\tprice: %g\n", *cstl_iter_key(pos), *cstl_iter_val(pos));
	}
	printf("\n");

	cstl_delete(stocks);
	return 0;
}
