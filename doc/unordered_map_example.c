#include <stdio.h>
#include <string.h>
#include <cstl/unordered_map.h>

CSTL_UNORDERED_MAP_INTERFACE(StrIntUMap, const char *, int)			/* インターフェイスを展開 */
CSTL_UNORDERED_MAP_IMPLEMENT(StrIntUMap, const char *, int, StrIntUMap_hash_string, strcmp)		/* 実装を展開 */

CSTL_UNORDERED_MULTIMAP_INTERFACE(IntIntUMMap, int, int)			/* インターフェイスを展開 */
CSTL_UNORDERED_MULTIMAP_IMPLEMENT(IntIntUMMap, int, int, StrIntUMap_hash_int, CSTL_EQUAL_TO)	/* 実装を展開 */

int main(void)
{
	{ /* unordered_map */
		/* イテレータ */
		StrIntUMapIterator pos;
		/* キーが文字列、値がintのunordered_mapを生成。
		 * 型名・関数のプレフィックスはStrIntUMapとなる。 */
		StrIntUMap *umap = StrIntUMap_new();

		/* 要素を挿入 */
		StrIntUMap_insert(umap, "aaa", 1, NULL);
		StrIntUMap_insert(umap, "bbb", 2, NULL);
		/* キーによる値の読み書き */
		printf("%d\n", *StrIntUMap_at(umap, "aaa"));
		*StrIntUMap_at(umap, "bbb") = 3;
		*StrIntUMap_at(umap, "ccc") = 4; /* 存在しないキーの要素は自動的に挿入 */
		/* 要素数 */
		printf("size: %d\n", StrIntUMap_size(umap));
		for (pos = StrIntUMap_begin(umap); pos != StrIntUMap_end(umap); pos = StrIntUMap_next(pos)) {
			/* イテレータによる要素の読み書き */
			printf("%s: %d,", *StrIntUMap_key(pos), *StrIntUMap_value(pos));
			*StrIntUMap_value(pos) += 1;
			printf("%d\n", *StrIntUMap_value(pos));
		}

		/* 使い終わったら破棄 */
		StrIntUMap_delete(umap);
	}
	{ /* unordered_multimap */
		/* イテレータ */
		IntIntUMMapIterator pos;
		IntIntUMMapIterator first, last;
		/* キーがint、値がintのunordered_multimapを生成。
		 * 型名・関数のプレフィックスはIntIntUMMapとなる。 */
		IntIntUMMap *umap = IntIntUMMap_new();

		/* 要素を挿入 */
		IntIntUMMap_insert(umap, 1, 1);
		IntIntUMMap_insert(umap, 2, 2);
		IntIntUMMap_insert(umap, 1, 3); /* 重複したキーを挿入できる */
		/* 要素数 */
		printf("size: %d\n", IntIntUMMap_size(umap));

		/* キーが1の要素を探索 */
		IntIntUMMap_equal_range(umap, 1, &first, &last);
		for (pos = first; pos != last; pos = IntIntUMMap_next(pos)) {
			/* イテレータによる要素の読み書き */
			printf("%d: %d,", *IntIntUMMap_key(pos), *IntIntUMMap_value(pos));
			*IntIntUMMap_value(pos) += 1;
			printf("%d\n", *IntIntUMMap_value(pos));
		}

		/* 使い終わったら破棄 */
		IntIntUMMap_delete(umap);
	}
	return 0;
}
