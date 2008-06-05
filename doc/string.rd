=begin
== string
stringを使うには、以下のマクロを用いてコードを展開する必要がある。

  #include <cstl/string.h>

  /* インターフェイスを展開 */
  #define CSTL_STRING_INTERFACE(Name, Type)

  /* 実装を展開 */
  #define CSTL_STRING_IMPLEMENT(Name, Type)

: Name
  既存の型と重複しない任意の名前。コンテナの型名と関数のプレフィックスになる
: Type
  任意の文字の型

<<< br

NameにString, TypeにCharTを指定した場合、
以下のインターフェイスを提供する。

* 定数
  * ((<CSTL_NPOS>))
* 型
  * ((<String>))
* 関数
  * 生成
    * ((<String_new()>))
  * 破棄
    * ((<String_delete()>))
  * サイズ
    * ((<String_size()>)) , ((<String_length()>))
    * ((<String_empty()>))
  * 許容量
    * ((<String_capacity()>))
    * ((<String_reserve()>)) , ((<String_shrink()>))
  * 比較
    * ((<String_compare()>))
  * 文字のアクセス
    * ((<String_at()>))
    * ((<String_c_str()>))
    * ((<String_data()>))
  * 代入
    * ((<String_assign()>)) , ((<String_assign_len()>)) , ((<String_assign_c()>))
  * 追加
    * ((<String_append()>)) , ((<String_append_len()>)) , ((<String_append_c()>)) , ((<String_push_back()>))
  * 挿入
    * ((<String_insert()>)) , ((<String_insert_len()>)) , ((<String_insert_c()>))
  * 置換
    * ((<String_replace()>)) , ((<String_replace_len()>)) , ((<String_replace_c()>))
  * 削除
    * ((<String_erase()>))
    * ((<String_pop_back()>))
    * ((<String_clear()>))
  * サイズの変更
    * ((<String_resize()>))
  * 交換
    * ((<String_swap()>))
  * 検索
    * ((<String_find()>)) , ((<String_find_len()>)) , ((<String_find_c()>))
    * ((<String_rfind()>)) , ((<String_rfind_len()>)) , ((<String_rfind_c()>))
    * ((<String_find_first_of()>)) , ((<String_find_first_of_len()>)) , ((<String_find_first_of_c()>))
    * ((<String_find_last_of()>)) , ((<String_find_last_of_len()>)) , ((<String_find_last_of_c()>))
    * ((<String_find_first_not_of()>)) , ((<String_find_first_not_of_len()>)) , ((<String_find_first_not_of_c()>))
    * ((<String_find_last_not_of()>)) , ((<String_find_last_not_of_len()>)) , ((<String_find_last_not_of_c()>))
<<< hr

==== CSTL_NPOS
  #define CSTL_NPOS  ((size_t)-1)
* size_t型の最大値。
* 検索関数において、検索に失敗した時に返される。
<<< hr

==== String
  String
コンテナの型。抽象データ型となっており、以下の関数によってのみアクセスできる。
<<< hr

==== String_new()
  String *String_new(size_t n);
* 許容量(内部メモリの再割り当てを行わずに格納できる文字数)がn個のstringを生成する。
* 生成に成功した場合、そのオブジェクトへのポインタを返す。
* メモリ不足の場合、NULLを返す。
<<< hr

==== String_delete()
  void String_delete(String *self);
* selfのすべての文字を削除し、selfを破棄する。
<<< hr

==== String_size()
  size_t String_size(String *self);
* selfの現在の文字数を返す。String_length(self)と等価である。
<<< hr

==== String_length()
  size_t String_length(String *self);
* selfの現在の文字数を返す。String_size(self)と等価である。
<<< hr

==== String_empty()
  int String_empty(String *self);
* selfが空の場合、0以外の値を返す。
* selfが空でない場合、0を返す。
<<< hr

==== String_capacity()
  size_t String_capacity(String *self);
* selfの許容量を返す。
<<< hr

==== String_reserve()
  int String_reserve(String *self, size_t n);
* selfの許容量を文字n個の領域に拡張する。
* selfが持つ文字は維持され、拡張した領域の初期化はしない。
* 拡張に成功した場合、0以外の値を返す。
* nがselfの現在の許容量以下の場合、selfの変更を行わず0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< hr

==== String_shrink()
  void String_shrink(String *self, size_t n);
* selfの許容量を文字n個の領域に縮小する。
* nがselfの現在の文字数以下の場合、selfの許容量を文字数と同じにする。
* nがselfの現在の許容量以上の場合、何もしない。
<<< hr

==== String_compare()
  int String_compare(String *self, String *x);
* selfとxの文字列を比較する。
* 文字列が等しい場合、0を返す。
* selfがxより辞書順位で小さい場合、負の値を返す。
* selfがxより辞書順位で大きい場合、正の値を返す。
<<< hr

==== String_at()
  CharT *String_at(String *self, size_t idx);
* selfのidx番目の文字へのポインタを返す。
* 戻り値はselfの変更により無効となる。
* 事前条件
  * idxがselfの現在の文字数より小さい値であること。
<<< hr

==== String_c_str()
  const CharT *String_c_str(String *self);
* selfの文字列を'\0'で終端したCの文字列として返す。
* 戻り値はselfの変更により無効となる。
<<< hr

==== String_data()
  const CharT *String_data(String *self);
* selfの文字列を文字の配列として返す。
* 戻り値の配列が'\0'で終端していることは保証されない。
* 戻り値はselfの変更により無効となる。
<<< hr

==== String_assign()
  String *String_assign(String *self, const CharT *cstr);
* selfにcstrというCの文字列を代入する。
* cstrはself内の文字列でもよい。
* 代入に成功した場合、selfを返す。
* メモリ不足の場合、selfの変更を行わずNULLを返す。
* 事前条件
  * cstrがNULLでないこと。
<<< hr

==== String_assign_len()
  String *String_assign_len(String *self, const CharT *chars, size_t chars_len);
* selfにcharsという文字の配列からchars_len個の文字を代入する。
* chars_lenがCSTL_NPOSと等しい場合、String_assign()と等価となる。ただしcharsは'\0'で終端していなければならない。
* charsはself内の文字列でもよい。
* 代入に成功した場合、selfを返す。
* メモリ不足の場合、selfの変更を行わずNULLを返す。
* 事前条件
  * charsがNULLでないこと。
<<< hr

==== String_assign_c()
  String *String_assign_c(String *self, size_t n, CharT c);
* selfにcという文字をn個代入する。
* 代入に成功した場合、selfを返す。
* メモリ不足の場合、selfの変更を行わずNULLを返す。
<<< hr

==== String_append()
  String *String_append(String *self, const CharT *cstr);
* selfにcstrというCの文字列を追加する。
* cstrはself内の文字列でもよい。
* 追加に成功した場合、selfを返す。
* メモリ不足の場合、selfの変更を行わずNULLを返す。
* 事前条件
  * cstrがNULLでないこと。
<<< hr

==== String_append_len()
  String *String_append_len(String *self, const CharT *chars, size_t chars_len);
* selfにcharsという文字の配列からchars_len個の文字を追加する。
* chars_lenがCSTL_NPOSと等しい場合、String_append()と等価となる。ただしcharsは'\0'で終端していなければならない。
* charsはself内の文字列でもよい。
* 追加に成功した場合、selfを返す。
* メモリ不足の場合、selfの変更を行わずNULLを返す。
* 事前条件
  * charsがNULLでないこと。
<<< hr

==== String_append_c()
  String *String_append_c(String *self, size_t n, CharT c);
* selfにcという文字をn個追加する。
* 追加に成功した場合、selfを返す。
* メモリ不足の場合、selfの変更を行わずNULLを返す。
<<< hr

==== String_push_back()
  String *String_push_back(String *self, CharT c);
* selfにcという文字を追加する。
* 追加に成功した場合、selfを返す。
* メモリ不足の場合、selfの変更を行わずNULLを返す。
<<< hr

==== String_insert()
  String *String_insert(String *self, size_t idx, const CharT *cstr);
* selfのidx番目の位置にcstrというCの文字列を挿入する。
* cstrはself内の文字列でもよい。
* 挿入に成功した場合、selfを返す。
* メモリ不足の場合、selfの変更を行わずNULLを返す。
* 事前条件
  * cstrがNULLでないこと。
  * idxがselfの文字数以下の値であること。
<<< hr

==== String_insert_len()
  String *String_insert_len(String *self, size_t idx, const CharT *chars, size_t chars_len);
* selfのidx番目の位置にcharsという文字の配列からchars_len個の文字を挿入する。
* chars_lenがCSTL_NPOSと等しい場合、String_insert()と等価となる。ただしcharsは'\0'で終端していなければならない。
* charsはself内の文字列でもよい。
* 挿入に成功した場合、selfを返す。
* メモリ不足の場合、selfの変更を行わずNULLを返す。
* 事前条件
  * charsがNULLでないこと。
  * idxがselfの文字数以下の値であること。
<<< hr

==== String_insert_c()
  String *String_insert_c(String *self, size_t idx, size_t n, CharT c);
* selfのidx番目の位置にcという文字をn個挿入する。
* 挿入に成功した場合、selfを返す。
* メモリ不足の場合、selfの変更を行わずNULLを返す。
* 事前条件
  * idxがselfの文字数以下の値であること。
<<< hr

==== String_replace()
  String *String_replace(String *self, size_t idx, size_t len, const CharT *cstr);
* selfのidx番目から最大len個の文字を、cstrというCの文字列で置換する。
* cstrはself内の文字列でもよい。
* 置換に成功した場合、selfを返す。
* メモリ不足の場合、selfの変更を行わずNULLを返す。
* 事前条件
  * cstrがNULLでないこと。
  * idxがselfの文字数以下の値であること。
<<< hr

==== String_replace_len()
  String *String_replace_len(String *self, size_t idx, size_t len, const CharT *chars, size_t chars_len);
* selfのidx番目から最大len個の文字を、charsという文字の配列のchars_len個の文字で置換する。
* chars_lenがCSTL_NPOSと等しい場合、String_replace()と等価となる。ただしcharsは'\0'で終端していなければならない。
* charsはself内の文字列でもよい。
* 置換に成功した場合、selfを返す。
* メモリ不足の場合、selfの変更を行わずNULLを返す。
* 事前条件
  * charsがNULLでないこと。
  * idxがselfの文字数以下の値であること。
<<< hr

==== String_replace_c()
  String *String_replace_c(String *self, size_t idx, size_t len, size_t n, CharT c);
* selfのidx番目から最大len個の文字を、cという文字n個で置換する。
* 置換に成功した場合、selfを返す。
* メモリ不足の場合、selfの変更を行わずNULLを返す。
* 事前条件
  * idxがselfの文字数以下の値であること。
<<< hr

==== String_erase()
  String *String_erase(String *self, size_t idx, size_t len);
* selfのidx番目の文字から最大len個の文字を削除する。
* selfを返す。
* 事前条件
  * idxがselfの文字数以下の値であること。
<<< hr

==== String_pop_back()
  CharT String_pop_back(String *self);
* selfの最後の文字を削除し、その文字を返す。
* 事前条件
  * selfが空でないこと。
<<< hr

==== String_clear()
  void String_clear(String *self);
* selfのすべての文字を削除する。
<<< hr

==== String_resize()
  int String_resize(String *self, size_t n, CharT c);
* selfの文字数をn個に変更する。
* nがselfの現在の文字数以下の場合、文字数がn個になるまで末尾から文字が削除される。
* nがselfの現在の文字数より大きい場合、文字数がn個になるまでcという文字が末尾から追加される。
* 要素数の変更に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< hr

==== String_swap()
  void String_swap(String *self, String *x);
* selfとxの内容を交換する。
<<< hr

==== String_find()
  size_t String_find(const CharT *x, const CharT *cstr, size_t idx);
* xというCの文字列のidx番目から、cstrというCの文字列が現れる最初の位置を前進して検索する。
* 検索に成功した場合、見つかった部分文字列の最初の文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
  * cstrがNULLでないこと。
<<< hr

==== String_find_len()
  size_t String_find_len(const CharT *x, const CharT *chars, size_t idx, size_t chars_len);
* xというCの文字列のidx番目から、charsという文字の配列のchars_len個の文字が現れる最初の位置を前進して検索する。
* chars_lenがCSTL_NPOSと等しい場合、String_find()と等価となる。ただしcharsは'\0'で終端していなければならない。
* 検索に成功した場合、見つかった部分文字列の最初の文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
  * charsがNULLでないこと。
<<< hr

==== String_find_c()
  size_t String_find_c(const CharT *x, CharT c, size_t idx);
* xというCの文字列のidx番目から、cという文字が現れる最初の位置を前進して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
<<< hr

==== String_rfind()
  size_t String_rfind(const CharT *x, const CharT *cstr, size_t idx);
* xというCの文字列のidx番目から、cstrというCの文字列が現れる最後の位置を後退して検索する。
* 検索に成功した場合、見つかった部分文字列の最初の文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
  * cstrがNULLでないこと。
<<< hr

==== String_rfind_len()
  size_t String_rfind_len(const CharT *x, const CharT *chars, size_t idx, size_t chars_len);
* xというCの文字列のidx番目から、charsという文字の配列のchars_len個の文字が現れる最後の位置を後退して検索する。
* chars_lenがCSTL_NPOSと等しい場合、String_rfind()と等価となる。ただしcharsは'\0'で終端していなければならない。
* 検索に成功した場合、見つかった部分文字列の最初の文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
  * charsがNULLでないこと。
<<< hr

==== String_rfind_c()
  size_t String_rfind_c(const CharT *x, CharT c, size_t idx);
* xというCの文字列のidx番目から、cという文字が現れる最後の位置を後退して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
<<< hr

==== String_find_first_of()
  size_t String_find_first_of(const CharT *x, const CharT *cstr, size_t idx);
* xというCの文字列のidx番目から、cstrというCの文字列に含まれる最初の文字が現れる位置を前進して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
  * cstrがNULLでないこと。
<<< hr

==== String_find_first_of_len()
  size_t String_find_first_of_len(const CharT *x, const CharT *chars, size_t idx, size_t chars_len);
* xというCの文字列のidx番目から、charsという文字の配列のchars_len個の文字に含まれる最初の文字が現れる位置を前進して検索する。
* chars_lenがCSTL_NPOSと等しい場合、String_find_first_of()と等価となる。ただしcharsは'\0'で終端していなければならない。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
  * charsがNULLでないこと。
<<< hr

==== String_find_first_of_c()
  size_t String_find_first_of_c(const CharT *x, CharT c, size_t idx);
* xというCの文字列のidx番目から、cという文字が現れる最初の位置を前進して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
<<< hr

==== String_find_last_of()
  size_t String_find_last_of(const CharT *x, const CharT *cstr, size_t idx);
* xというCの文字列のidx番目から、cstrというCの文字列に含まれる最後の文字が現れる位置を後退して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
  * cstrがNULLでないこと。
<<< hr

==== String_find_last_of_len()
  size_t String_find_last_of_len(const CharT *x, const CharT *chars, size_t idx, size_t chars_len);
* xというCの文字列のidx番目から、charsという文字の配列のchars_len個の文字に含まれる最後の文字が現れる位置を後退して検索する。
* chars_lenがCSTL_NPOSと等しい場合、String_find_last_of()と等価となる。ただしcharsは'\0'で終端していなければならない。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
  * charsがNULLでないこと。
<<< hr

==== String_find_last_of_c()
  size_t String_find_last_of_c(const CharT *x, CharT c, size_t idx);
* xというCの文字列のidx番目から、cという文字が現れる最後の位置を後退して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
<<< hr

==== String_find_first_not_of()
  size_t String_find_first_not_of(const CharT *x, const CharT *cstr, size_t idx);
* xというCの文字列のidx番目から、cstrというCの文字列に含まれない最初の文字が現れる位置を前進して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
  * cstrがNULLでないこと。
<<< hr

==== String_find_first_not_of_len()
  size_t String_find_first_not_of_len(const CharT *x, const CharT *chars, size_t idx, size_t chars_len);
* xというCの文字列のidx番目から、charsという文字の配列のchars_len個の文字に含まれない最初の文字が現れる位置を前進して検索する。
* chars_lenがCSTL_NPOSと等しい場合、String_find_first_not_of()と等価となる。ただしcharsは'\0'で終端していなければならない。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
  * charsがNULLでないこと。
<<< hr

==== String_find_first_not_of_c()
  size_t String_find_first_not_of_c(const CharT *x, CharT c, size_t idx);
* xというCの文字列のidx番目から、c以外の文字が現れる最初の位置を前進して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
<<< hr

==== String_find_last_not_of()
  size_t String_find_last_not_of(const CharT *x, const CharT *cstr, size_t idx);
* xというCの文字列のidx番目から、cstrというCの文字列に含まれない最後の文字が現れる位置を後退して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
  * cstrがNULLでないこと。
<<< hr

==== String_find_last_not_of_len()
  size_t String_find_last_not_of_len(const CharT *x, const CharT *chars, size_t idx, size_t chars_len);
* xというCの文字列のidx番目から、charsという文字の配列のchars_len個の文字に含まれない最後の文字が現れる位置を後退して検索する。
* chars_lenがCSTL_NPOSと等しい場合、String_find_last_not_of()と等価となる。ただしcharsは'\0'で終端していなければならない。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
  * charsがNULLでないこと。
<<< hr

==== String_find_last_not_of_c()
  size_t String_find_last_not_of_c(const CharT *x, CharT c, size_t idx);
* xというCの文字列のidx番目から、c以外の文字が現れる最後の位置を後退して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件
  * xがNULLでないこと。
<<< hr

=end
