=begin
== list
listを使うには、list.hというヘッダファイルをインクルードする。
  #include <cstl/list.h>

以下のマクロを用いてコードを展開する必要がある。

  /* インターフェイスを展開 */
  #define CSTL_LIST_INTERFACE(Name, Type)

  /* 実装を展開 */
  #define CSTL_LIST_IMPLEMENT(Name, Type)

Nameに既存の型と重複しない任意の名前を、Typeに任意の要素の型を指定する。
<<< br

CSTL_LIST_INTERFACEの引数のNameにList, TypeにTを指定した場合、
以下のインターフェイスを提供する。

==== 型

  List
コンテナの型。抽象データ型となっており、以下の関数によってのみアクセスできる。

  ListIterator
イテレータの型。要素の位置を示す。
関数から返されたイテレータを有効なイテレータという。
宣言されただけのイテレータ、または削除された要素のイテレータを無効なイテレータという。

==== 関数
以下の関数において、List*型の引数はNULLでないことを事前条件に含める。

+ 生成
  List *List_new(void);
* listを生成する。
* 生成に成功した場合、そのオブジェクトへのポインタを返す。
* メモリ不足の場合、NULLを返す。
<<< br

+ 破棄
  void List_delete(List *self);
* selfのすべての要素を削除し、selfを破棄する。
<<< br

+ サイズ
  size_t List_size(List *self);
* selfの現在の要素数を返す。
<<< br

  int List_empty(List *self);
* selfが空の場合、0以外の値を返す。
* selfが空でない場合、0を返す。
<<< br

+ イテレータ
  ListIterator List_begin(List *self);
* selfの最初の要素のイテレータを返す。
<<< br

  ListIterator List_end(List *self);
* selfの最後の要素の次のイテレータを返す。
<<< br

  ListIterator List_rbegin(List *self);
* selfの最後の要素のイテレータを返す。
<<< br

  ListIterator List_rend(List *self);
* selfの最初の要素の前のイテレータを返す。
<<< br

  ListIterator List_next(ListIterator pos);
* posが示す位置の要素の次のイテレータを返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがList_end()またはList_rend()でないこと。
<<< br

  ListIterator List_prev(ListIterator pos);
* posが示す位置の要素の前のイテレータを返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがList_end()またはList_rend()でないこと。
<<< br

+ 要素のアクセス
  T *List_at(ListIterator pos);
* posが示す位置の要素へのポインタを返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがList_end()またはList_rend()でないこと。
<<< br

  T List_front(List *self);
* selfの最初の要素を返す。
* 事前条件
  * selfが空でないこと。
<<< br

  T List_back(List *self);
* selfの最後の要素を返す。
* 事前条件
  * selfが空でないこと。
<<< br

+ 挿入
  ListIterator List_insert(List *self, ListIterator pos, T elem);
* selfのposが示す位置にelemのコピーを挿入する。
* 挿入に成功した場合、新しい要素のイテレータを返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * posがselfの有効なイテレータであること。
<<< br

  int List_insert_n(List *self, ListIterator pos, size_t n, T elem);
* selfのposが示す位置にelemのコピーをn個挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * posがselfの有効なイテレータであること。
<<< br

  int List_insert_array(List *self, ListIterator pos, T *elems, size_t n);
* selfのposが示す位置にelemsという配列からn個の要素のコピーを挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * elemsがNULLでないこと。
  * posがselfの有効なイテレータであること。
<<< br

  int List_insert_range(List *self, ListIterator pos, ListIterator first, ListIterator last);
* selfのposが示す位置に[first, last)の範囲の要素のコピーを挿入する。
* [first, last)の要素はselfが持つ要素でもよい。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * posがselfの有効なイテレータであること。
  * [first, last)が有効なイテレータであること。
<<< br

  int List_push_back(List *self, T elem);
* elemのコピーをselfの最後の要素として追加する。
* 追加に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< br

  int List_push_front(List *self, T elem);
* elemのコピーをselfの最初の要素として追加する。
* 追加に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< br

+ 削除
  ListIterator List_erase(List *self, ListIterator pos);
* selfのposが示す位置の要素を削除し、その次のイテレータを返す。
* 事前条件
  * posがselfの有効なイテレータであること。
  * posがList_end()またはList_rend()でないこと。
<<< br

  ListIterator List_erase_range(List *self, ListIterator first, ListIterator last);
* selfの[first, last)の範囲の要素を削除し、削除した要素の次のイテレータを返す。
* 事前条件
  * [first, last)がselfの有効なイテレータであること。
<<< br

  T List_pop_front(List *self);
* selfの最初の要素を削除し、その要素を返す。
* 事前条件
  * selfが空でないこと。
<<< br

  T List_pop_back(List *self);
* selfの最後の要素を削除し、その要素を返す。
* 事前条件
  * selfが空でないこと。
<<< br

  void List_clear(List *self);
* selfのすべての要素を削除する。
<<< br

+ サイズの変更
  int List_resize(List *self, size_t n, T elem);
* selfの要素数をn個に変更する。
* nがselfの現在の要素数以下の場合、要素数がnになるまで末尾から要素が削除される。
* nがselfの現在の要素数より大きい場合、要素数がnになるまでelemのコピーが末尾から追加される。
* 要素数の変更に成功した場合、0以外の値を返す。
* メモリ不足の場合、0を返す。
<<< br

+ 交換
  void List_swap(List *self, List *x);
* selfとxの内容を交換する。
<<< br

+ つなぎ替え
  void List_splice(List *self, ListIterator pos, List *x, ListIterator first, ListIterator last);
* selfのposが示す位置にxの[first, last)の範囲の要素を移動する。
* [first, last)の要素の数だけselfの要素数が増加し、xの要素数が減少する。
* 事前条件
  * posがselfの有効なイテレータであること。
  * [first, last)がxの有効なイテレータであること。
  * selfとxが同一ならばposは[first, last)の範囲外であること。
<<< br

+ ソート
  void List_sort(List *self, int (*comp)(const T *p1, const T *p2));
* selfのすべての要素を比較関数compに従ってソートする。
* compには、*p1 == *p2ならば0を、*p1 < *p2ならば正または負の整数を、*p1 > *p2ならば*p1 < *p2の場合と逆の符号の整数を返す関数を指定する。
* このソートは安定である。
<<< br

+ 並べ替え
  void List_reverse(List *self);
* selfのすべての要素を逆順に並べ替える。
<<< br

+ マージ
  void List_merge(List *self, List *x, int (*comp)(const T *p1, const T *p2));
* ソートされた状態であるselfとxにおいて、xのすべての要素を比較関数compに従ってselfにマージする。
* selfはソートされた状態になり、xは空になる。
* compには、*p1 == *p2ならば0を、*p1 < *p2ならば正または負の整数を、*p1 > *p2ならば*p1 < *p2の場合と逆の符号の整数を返す関数を指定する。
* 事前条件
  * selfとxがcompに従ってソートされていること。
<<< br

=end
