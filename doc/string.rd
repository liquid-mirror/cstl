=begin
== string
stringを使うには、string.hというヘッダファイルをインクルードする。
  #include <cstl/string.h>
以下のマクロを用いてコードを展開する必要がある。

  /* インターフェイスを展開 */
  #define CSTL_STRING_INTERFACE(Name, Type)

  /* 実装を展開 */
  #define CSTL_STRING_IMPLEMENT(Name, Type)

Nameに既存の型と重複しない任意の名前を、Typeに任意の文字の型を指定する。
<<< br

CSTL_STRING_INTERFACEの引数のNameにString, TypeにCharTを指定した場合、
以下のインターフェイスを提供する。

==== 定数
  #define CSTL_NPOS  ((size_t)-1)
* size_t型の最大値。
* 検索関数において、検索に失敗した時に返される。
* const CharT *cstr, size_t cstr_lenという関数の引数において、cstr_lenにCSTL_NPOSを指定した場合、cstrはCの文字列として扱われる。

==== 型

  String
コンテナの型。抽象データ型となっており、以下の関数によってのみアクセスできる。

==== 関数
以下の関数において、String*型の引数はNULLでないことを事前条件に含める。

+ 生成
  String *String_new(size_t n);
* 許容量(内部メモリの再割り当てを行わずに格納できる文字数)がn個のstringを生成する。
* 生成に成功した場合、そのオブジェクトへのポインタを返す。
* メモリ不足の場合、NULLを返す。
<<< br

+ 破棄
  void String_delete(String *self);
* selfのすべての文字を削除し、selfを破棄する。
<<< br

+ サイズ
  size_t String_size(String *self);
* selfの現在の文字数を返す。String_length(self)と等価である。
<<< br

  size_t String_length(String *self);
* selfの現在の文字数を返す。String_size(self)と等価である。
<<< br

  int String_empty(String *self);
* selfが空の場合、0以外の値を返す。
* selfが空でない場合、0を返す。
<<< br

+ 許容量
  size_t String_capacity(String *self);
* selfの許容量を返す。
<<< br

  int String_reserve(String *self, size_t n);
* selfの許容量を文字n個の領域に拡張する。
* selfが持つ文字は維持され、拡張した領域の初期化はしない。
* 拡張に成功した場合、0以外の値を返す。
* nがselfの現在の許容量以下の場合、selfの変更を行わず0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< br

  void String_shrink(String *self, size_t n);
* selfの許容量を文字n個の領域に縮小する。
* nがselfの現在の文字数以下の場合、selfの許容量を文字数と同じにする。
* nがselfの現在の許容量以上の場合、何もしない。
<<< br

+ 比較
  int String_compare(String *self, String *x);
* selfとxの文字列を比較する。
* 文字列が等しい場合、0を返す。
* selfがxより辞書順位で小さい場合、負の値を返す。
* selfがxより辞書順位で大きい場合、正の値を返す。
<<< br

+ 文字のアクセス
  CharT *String_at(String *self, size_t idx);
* selfのidx番目の文字へのポインタを返す。
* 事前条件
  * idxがselfの現在の文字数より小さい値であること。
<<< br

  const CharT *String_c_str(String *self);
* selfの文字列を'\0'で終端したCの文字列として返す。
<<< br

+ 代入
  int String_assign(String *self, const CharT *cstr, size_t cstr_len);
* selfにcstrという文字の配列からcstr_len個の文字を代入する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列を代入する。ただしcstrは'\0'で終端していなければならない。
* cstrはself内の文字列でもよい。
* 代入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * cstrがNULLでないこと。
<<< br

  int String_assign_c(String *self, size_t n, CharT c);
* selfにcという文字をn個代入する。
* 代入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< br

+ 追加
  int String_append(String *self, const CharT *cstr, size_t cstr_len);
* selfにcstrという文字の配列からcstr_len個の文字を追加する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列を追加する。ただしcstrは'\0'で終端していなければならない。
* cstrはself内の文字列でもよい。
* 追加に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * cstrがNULLでないこと。
<<< br

  int String_append_c(String *self, size_t n, CharT c);
* selfにcという文字をn個追加する。
* 追加に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< br

  int String_push_back(String *self, CharT c);
* selfにcという文字を追加する。
* 追加に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< br

+ 挿入
  int String_insert(String *self, size_t idx, const CharT *cstr, size_t cstr_len);
* selfのidx番目の位置にcstrという文字の配列からcstr_len個の文字を挿入する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列を挿入する。ただしcstrは'\0'で終端していなければならない。
* cstrはself内の文字列でもよい。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * cstrがNULLでないこと。
  * idxがselfの文字数以下の値であること。
<<< br

  int String_insert_c(String *self, size_t idx, size_t n, CharT c);
* selfのidx番目の位置にcという文字をn個挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * idxがselfの文字数以下の値であること。
<<< br

+ 置換
  int String_replace(String *self, size_t idx, size_t len, const CharT *cstr, size_t cstr_len);
* selfのidx番目から最大len個の文字を、cstrという文字の配列のcstr_len個の文字で置換する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列で置換する。ただしcstrは'\0'で終端していなければならない。
* cstrはself内の文字列でもよい。
* 置換に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * cstrがNULLでないこと。
  * idxがselfの文字数以下の値であること。
<<< br

  int String_replace_c(String *self, size_t idx, size_t len, size_t n, CharT c);
* selfのidx番目から最大len個の文字を、cという文字n個で置換する。
* 置換に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * idxがselfの文字数以下の値であること。
<<< br

+ 削除
  void String_erase(String *self, size_t idx, size_t len);
* selfのidx番目の文字から最大len個の文字を削除する。
* 事前条件
  * idxがselfの文字数以下の値であること。
<<< br

  void String_clear(String *self);
* selfのすべての文字を削除する。
<<< br

+ サイズの変更
  int String_resize(String *self, size_t n, CharT c);
* selfの文字数をn個に変更する。
* nがselfの現在の文字数以下の場合、文字数がnになるまで末尾から文字が削除される。
* nがselfの現在の文字数より大きい場合、文字数がnになるまでcという文字が末尾から追加される。
* 要素数の変更に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< br

+ 交換
  void String_swap(String *self, String *x);
* selfとxの内容を交換する。
* 文字のコピーをしないので、String_assign(self, String_c_str(x), String_size(x))よりも速い。
  xが不要になる場合、こちらを使用するべきである。
<<< br

+ 検索
  size_t String_find(const CharT *x, const CharT *cstr, size_t idx, size_t cstr_len);
* xというCの文字列のidx番目から、cstrという文字の配列のcstr_len個の文字が現れる最初の位置を前進して検索する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列を検索する。ただしcstrは'\0'で終端していなければならない。
* 検索に成功した場合、見つかった部分文字列の最初の文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
  * cstrがNULLでないこと。
<<< br

  size_t String_find_c(const CharT *x, CharT c, size_t idx);
* xというCの文字列のidx番目から、cという文字が現れる最初の位置を前進して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
<<< br

  size_t String_rfind(const CharT *x, const CharT *cstr, size_t idx, size_t cstr_len);
* xというCの文字列のidx番目から、cstrという文字の配列のcstr_len個の文字が現れる最後の位置を後退して検索する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列を検索する。ただしcstrは'\0'で終端していなければならない。
* 検索に成功した場合、見つかった部分文字列の最初の文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
  * cstrがNULLでないこと。
<<< br

  size_t String_rfind_c(const CharT *x, CharT c, size_t idx);
* xというCの文字列のidx番目から、cという文字が現れる最後の位置を後退して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
<<< br

  size_t String_find_first_of(const CharT *x, const CharT *cstr, size_t idx, size_t cstr_len);
* xというCの文字列のidx番目から、cstrという文字の配列のcstr_len個の文字に含まれる最初の文字が現れる位置を前進して検索する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列に含まれる文字を検索する。ただしcstrは'\0'で終端していなければならない。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
  * cstrがNULLでないこと。
<<< br

  size_t String_find_first_of_c(const CharT *x, CharT c, size_t idx);
* xというCの文字列のidx番目から、cという文字が現れる最初の位置を前進して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
<<< br

  size_t String_find_last_of(const CharT *x, const CharT *cstr, size_t idx, size_t cstr_len);
* xというCの文字列のidx番目から、cstrという文字の配列のcstr_len個の文字に含まれる最後の文字が現れる位置を後退して検索する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列に含まれる文字を検索する。ただしcstrは'\0'で終端していなければならない。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
  * cstrがNULLでないこと。
<<< br

  size_t String_find_last_of_c(const CharT *x, CharT c, size_t idx);
* xというCの文字列のidx番目から、cという文字が現れる最後の位置を後退して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
<<< br

  size_t String_find_first_not_of(const CharT *x, const CharT *cstr, size_t idx, size_t cstr_len);
* xというCの文字列のidx番目から、cstrという文字の配列のcstr_len個の文字に含まれない最初の文字が現れる位置を前進して検索する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列に含まれない文字を検索する。ただしcstrは'\0'で終端していなければならない。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
  * cstrがNULLでないこと。
<<< br

  size_t String_find_first_not_of_c(const CharT *x, CharT c, size_t idx);
* xというCの文字列のidx番目から、c以外の文字が現れる最初の位置を前進して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
<<< br

  size_t String_find_last_not_of(const CharT *x, const CharT *cstr, size_t idx, size_t cstr_len);
* xというCの文字列のidx番目から、cstrという文字の配列のcstr_len個の文字に含まれない最後の文字が現れる位置を後退して検索する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列に含まれない文字を検索する。ただしcstrは'\0'で終端していなければならない。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
  * cstrがNULLでないこと。
<<< br

  size_t String_find_last_not_of_c(const CharT *x, CharT c, size_t idx);
* xというCの文字列のidx番目から、c以外の文字が現れる最後の位置を後退して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
<<< br

=end
