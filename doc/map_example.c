#include <stdio.h>
#include <string.h>
#include <cstl/map.h>

CSTL_MAP_INTERFACE(StrIntMap, const char *, int)			/* インターフェイスを展開 */
CSTL_MAP_IMPLEMENT(StrIntMap, const char *, int, strcmp)	/* 実装を展開 */

CSTL_MULTIMAP_INTERFACE(IntIntMMap, int, int)				/* インターフェイスを展開 */
CSTL_MULTIMAP_IMPLEMENT(IntIntMMap, int, int, CSTL_LESS)	/* 実装を展開 */

int main(void)
{
	{ /* map */
		/* イテレータ */
		StrIntMapIterator pos;
		/* キーが文字列、値がintのmapを生成。
		 * 型名・関数のプレフィックスはStrIntMapとなる。 */
		StrIntMap *map = StrIntMap_new();

		/* 要素を挿入 */
		StrIntMap_insert(map, "aaa", 1, NULL);
		StrIntMap_insert(map, "bbb", 2, NULL);
		/* キーによる値の読み書き */
		printf("%d\n", *StrIntMap_at(map, "aaa"));
		*StrIntMap_at(map, "bbb") = 3;
		*StrIntMap_at(map, "ccc") = 4; /* 存在しないキーの要素は自動的に挿入 */
		/* 要素数 */
		printf("size: %d\n", StrIntMap_size(map));
		for (pos = StrIntMap_begin(map); pos != StrIntMap_end(map); pos = StrIntMap_next(pos)) {
			/* イテレータによる要素の読み書き */
			printf("%s: %d,", *StrIntMap_key(pos), *StrIntMap_value(pos));
			*StrIntMap_value(pos) += 1;
			printf("%d\n", *StrIntMap_value(pos));
		}

		/* 使い終わったら破棄 */
		StrIntMap_delete(map);
	}
	{ /* multimap */
		/* イテレータ */
		IntIntMMapIterator pos;
		/* キーがint、値がintのmultimapを生成。
		 * 型名・関数のプレフィックスはIntIntMMapとなる。 */
		IntIntMMap *map = IntIntMMap_new();

		/* 要素を挿入 */
		IntIntMMap_insert(map, 1, 1);
		IntIntMMap_insert(map, 2, 2);
		IntIntMMap_insert(map, 1, 3); /* 重複したキーを挿入できる */
		/* 要素数 */
		printf("size: %d\n", IntIntMMap_size(map));

		/* キーが1の要素を探索 */
		for (pos = IntIntMMap_lower_bound(map, 1); pos != IntIntMMap_upper_bound(map, 1); pos = IntIntMMap_next(pos)) {
			/* イテレータによる要素の読み書き */
			printf("%d: %d,", *IntIntMMap_key(pos), *IntIntMMap_value(pos));
			*IntIntMMap_value(pos) += 1;
			printf("%d\n", *IntIntMMap_value(pos));
		}

		/* 使い終わったら破棄 */
		IntIntMMap_delete(map);
	}
	return 0;
}
