/* C++標準ライブラリ チュートリアル&リファレンス
 * Nicolai M. Josuttis著, 吉川邦夫訳, アスキー, 2001
 * p.454の例を移植 */
#include <stdio.h>
#include <cstl/string.h>

CSTL_STRING_INTERFACE(String, char)
CSTL_STRING_IMPLEMENT(String, char)

/* 標準入力から個々の単語を抽出し、それぞれの文字を逆順に出力する */
int main(int argc, char *argv[])
{
	char buf[1024];
	String *line = String_new(1024);
	/* 区切り文字 */
	String *delims = String_new(8);
	String_assign(delims, " \t,.;");

	while (fgets(buf, 1024, stdin)) {
		size_t begIdx, endIdx;
		String_assign(line, buf);
		String_erase(line, String_length(line)-1, CSTL_NPOS); /* '\n'を取り除く */
		/* 最初の単語の先頭を検索する */
		begIdx = String_find_first_not_of(String_c_str(line), String_c_str(delims), 0);
		/* 単語の先頭が見つかるまで繰り返す */
		while (begIdx != CSTL_NPOS) {
			size_t i;
			/* 単語の末尾を検索する */
			endIdx = String_find_first_of(String_c_str(line), String_c_str(delims), begIdx);
			if (endIdx == CSTL_NPOS) {
				/* 単語の終わりは行末である */
				endIdx = String_length(line);
			}
			/* 単語の文字を逆順に出力する */
			for (i = endIdx; i > begIdx; --i) {
				putchar(*String_at(line, i-1));
			}
			putchar(' ');
			/* 次の単語の先頭を検索する */
			begIdx = String_find_first_not_of(String_c_str(line), String_c_str(delims), endIdx);
		}
		putchar('\n');
	}
	String_delete(line);
	String_delete(delims);
	return 0;
}
