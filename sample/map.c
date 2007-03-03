/* C++標準ライブラリ チュートリアル&リファレンス
 * Nicolai M. Josuttis著, 吉川邦夫訳, アスキー, 2001
 * p.211の例を移植 */
#include <stdio.h>
#include <string.h>
#include "../cstl/map.h"

MAP_INTERFACE(StrFltMap, char *, float)
MAP_IMPLEMENT(StrFltMap, char *, float, strcmp, ASC)

/* 連想配列として使う */
int main(void)
{
	StrFltMapIterator pos;
	StrFltMap *stocks = StrFltMap_new();

	/* 要素の挿入 */
	*StrFltMap_lookup(stocks, "BASF")    = 369.50;
	*StrFltMap_lookup(stocks, "VW")      = 413.50;
	*StrFltMap_lookup(stocks, "Daimler") = 819.00;
	*StrFltMap_lookup(stocks, "BMW")     = 834.00;
	*StrFltMap_lookup(stocks, "Siemens") = 842.20;

	/* すべての要素を出力する */
	for (pos = StrFltMap_begin(stocks); pos != StrFltMap_end(stocks); pos = StrFltMap_next(pos)) {
		printf("stock: %s\tprice: %g\n", StrFltMap_key(pos), *StrFltMap_value(pos));
	}
	printf("\n");

	/* 値が倍になった */
	for (pos = StrFltMap_begin(stocks); pos != StrFltMap_end(stocks); pos = StrFltMap_next(pos)) {
		*StrFltMap_value(pos) *= 2;
	}
	/* すべての要素を出力する */
	for (pos = StrFltMap_begin(stocks); pos != StrFltMap_end(stocks); pos = StrFltMap_next(pos)) {
		printf("stock: %s\tprice: %g\n", StrFltMap_key(pos), *StrFltMap_value(pos));
	}
	printf("\n");

	/* キーの"VW"を"Volkswagen"に変更する
	 * キーは変更不可なので値をコピーして古いキーを削除するしかない */
	*StrFltMap_lookup(stocks, "Volkswagen") = *StrFltMap_lookup(stocks, "VW");
	StrFltMap_erase_key(stocks, "VW");

	/* すべての要素を出力する */
	for (pos = StrFltMap_begin(stocks); pos != StrFltMap_end(stocks); pos = StrFltMap_next(pos)) {
		printf("stock: %s\tprice: %g\n", StrFltMap_key(pos), *StrFltMap_value(pos));
	}
	printf("\n");

	StrFltMap_delete(stocks);
	return 0;
}
