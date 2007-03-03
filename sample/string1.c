/* C++標準ライブラリ チュートリアル&リファレンス
 * Nicolai M. Josuttis著, 吉川邦夫訳, アスキー, 2001
 * p.450の例を移植 */
#include <stdio.h>
#include "../cstl/string.h"

STRING_INTERFACE(String, char)
STRING_IMPLEMENT(String, char)

/* コマンドライン引数に.tmpという拡張子をつける
 * 拡張子が付いていれば.tmpに変える
 * 最初から.tmpという拡張子ならば.xxxという拡張子をつける */
int main(int argc, char *argv[])
{
	int i;
	String *filename = String_new();
	String *basename = String_new();
	String *extname = String_new();
	String *tmpname = String_new();
	String *suffix = String_new_cstr("tmp", NPOS);

	for (i = 1; i < argc; i++) {
		size_t idx;
		/* コマンドライン引数をファイル名とする */
		String_assign(filename, argv[i], NPOS);
		/* ファイル名から'.'を探す */
		idx = String_find_c(String_c_str(filename), '.', 0);
		if (idx == NPOS) {
			/* ファイル名に'.'が含まれない
			 * ".tmp"を追加 */
			String_assign(tmpname, String_c_str(filename), NPOS);
			String_push_back(tmpname, '.');
			String_append(tmpname, String_c_str(suffix), NPOS);
		} else {
			/* ファイル名を基本名と拡張子に分ける */
			String_assign(basename, String_c_str(filename), idx);
			String_assign(extname, String_c_str(filename) + idx+1, NPOS);
			if (String_empty(extname)) {
				/* '.'のあとに拡張子がない場合、"tmp"を付ける */
				String_assign(tmpname, String_c_str(filename), NPOS);
				String_append(tmpname, String_c_str(suffix), NPOS);
			} else if (String_compare(extname, suffix) == 0) {
				/* "tmp"を"xxx"に置換 */
				String_assign(tmpname, String_c_str(filename), NPOS);
				String_replace(tmpname, idx+1, String_size(extname), "xxx", NPOS);
			} else {
				/* その他の拡張子を"tmp"に置換 */
				String_assign(tmpname, String_c_str(filename), NPOS);
				String_replace(tmpname, idx+1, NPOS, String_c_str(suffix), NPOS);
			}
		}
		printf("%s => %s\n", String_c_str(filename), String_c_str(tmpname));
	}

	String_delete(filename);
	String_delete(basename);
	String_delete(extname);
	String_delete(tmpname);
	String_delete(suffix);
	return 0;
}
