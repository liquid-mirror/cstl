/* C++標準ライブラリ チュートリアル&リファレンス
 * Nicolai M. Josuttis著, 吉川邦夫訳, アスキー, 2001
 * p.212の例を移植 */
#include <stdio.h>
#include <string.h>
#include "../cstl/map.h"

MULTIMAP_INTERFACE(StrStrMMap, char *, char *)
MULTIMAP_IMPLEMENT(StrStrMMap, char *, char *, strcmp, ASC)

/* 辞書として使う */
int main(void)
{
	StrStrMMapIterator pos;
	StrStrMMap *dict = StrStrMMap_new();
	char *word;

	StrStrMMap_insert(dict , "day"     , "Tag");
	StrStrMMap_insert(dict , "strange" , "fremd");
	StrStrMMap_insert(dict , "car"     , "Auto");
	StrStrMMap_insert(dict , "smart"   , "elegant");
	StrStrMMap_insert(dict , "trait"   , "Merkmal");
	StrStrMMap_insert(dict , "strange" , "seltsam");
	StrStrMMap_insert(dict , "smart"   , "raffiniert");
	StrStrMMap_insert(dict , "smart"   , "klug");
	StrStrMMap_insert(dict , "clever"  , "raffiniert");

	/* すべての要素を出力する */
	printf("english\t\tgerman\n");
	printf("-------------------------------\n");
	for (pos = StrStrMMap_begin(dict); pos != StrStrMMap_end(dict); pos = StrStrMMap_next(pos)) {
		printf("%s\t\t%s\n", StrStrMMap_key(pos), *StrStrMMap_value(pos));
	}
	printf("\n");

	/* "smart"というキーに対応するすべての値を出力する */
	word = "smart";
	printf("%s: \n", word);
	for (pos = StrStrMMap_lower_bound(dict, word); pos != StrStrMMap_upper_bound(dict, word); pos = StrStrMMap_next(pos)) {
		printf("\t%s\n", *StrStrMMap_value(pos));
	}
	printf("\n");

	/* "raffiniert"という値に対応するすべてのキーを出力する */
	word = "raffiniert";
	printf("%s: \n", word);
	for (pos = StrStrMMap_begin(dict); pos != StrStrMMap_end(dict); pos = StrStrMMap_next(pos)) {
		if (strcmp(word, *StrStrMMap_value(pos)) == 0) {
			printf("\t%s\n", StrStrMMap_key(pos));
		}
	}

	StrStrMMap_delete(dict);
	return 0;
}
