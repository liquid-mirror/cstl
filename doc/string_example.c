#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <cstl/string.h>

/* stringのインターフェイスと実装を展開 */
CSTL_STRING_INTERFACE(String, char)
CSTL_STRING_IMPLEMENT(String, char)

int main(void)
{
	size_t i;
	size_t pos;
	const char *p;
	/* stringを生成。
	 * 型名・関数のプレフィックスはStringとなる。 */
	String *str = String_new();

	/* 代入 */
	String_assign(str, "string");
	/* 末尾に追加 */
	String_append(str, " example");
	/* 先頭に挿入 */
	String_insert(str, 0, "a ");

	/* Cの文字列として出力 */
	printf("%s\n", String_c_str(str));

	for (i = 0; i < String_length(str); i++) {
		/* インデックスによる文字の読み書き */
		printf("%c, ", *String_at(str, i));
		*String_at(str, i) = toupper(*String_at(str, i));
		printf("%c\n", *String_at(str, i));
	}

	/* 検索して置換 */
	p = "EX";
	pos = String_find(str, p, 0);
	if (pos != CSTL_NPOS) {
		String_replace(str, pos, strlen(p), "S");
	}
	printf("%s\n", String_c_str(str));

	/* 使い終わったら破棄 */
	String_delete(str);
	return 0;
}
