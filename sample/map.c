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
	*StrFltMap_at(stocks, "BASF")    = 369.50;
	*StrFltMap_at(stocks, "VW")      = 413.50;
	*StrFltMap_at(stocks, "Daimler") = 819.00;
	*StrFltMap_at(stocks, "BMW")     = 834.00;
	*StrFltMap_at(stocks, "Siemens") = 842.20;

	/* すべての要素を出力する */
	for (pos = StrFltMap_begin(stocks); pos != StrFltMap_end(stocks); pos = StrFltMapIterator_next(pos)) {
		printf("stock: %s\tprice: %g\n", *StrFltMapIterator_key(pos), *StrFltMapIterator_value(pos));
	}
	printf("\n");

	/* 値が倍になった */
	for (pos = StrFltMap_begin(stocks); pos != StrFltMap_end(stocks); pos = StrFltMapIterator_next(pos)) {
		*StrFltMapIterator_value(pos) *= 2;
	}
	/* すべての要素を出力する */
	for (pos = StrFltMap_begin(stocks); pos != StrFltMap_end(stocks); pos = StrFltMapIterator_next(pos)) {
		printf("stock: %s\tprice: %g\n", *StrFltMapIterator_key(pos), *StrFltMapIterator_value(pos));
	}
	printf("\n");

	/* キーの"VW"を"Volkswagen"に変更する
	 * キーは変更不可なので値をコピーして古いキーを削除するしかない */
	*StrFltMap_at(stocks, "Volkswagen") = *StrFltMap_at(stocks, "VW");
	StrFltMap_erase_key(stocks, "VW");

	/* すべての要素を出力する */
	for (pos = StrFltMap_begin(stocks); pos != StrFltMap_end(stocks); pos = StrFltMapIterator_next(pos)) {
		printf("stock: %s\tprice: %g\n", *StrFltMapIterator_key(pos), *StrFltMapIterator_value(pos));
	}
	printf("\n");

	StrFltMap_delete(stocks);
	return 0;
}
