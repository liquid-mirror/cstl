=begin
= CSTL

== 目次
* ((<概要>))
* ((<動作条件>))
* ((<インストール>))
* ((<提供コンテナ>))
* ((<リファレンスマニュアル>))
  * ((<vector>))
  * ((<deque>))
  * ((<list>))
  * ((<set|"set/multiset">))
  * ((<multiset|"set/multiset">))
  * ((<map|"map/multimap">))
  * ((<multimap|"map/multimap">))
  * ((<string>))
  * ((<algorithm>))
* ((<STLとの主な違い>))
* ((<ライセンス>))
  * ((<日本語訳(参考)>))


== 概要
CSTLは、C++のSTLコンテナに似たインターフェイスを持つC/C++用のコンテナライブラリである。
マクロを使うことにより、任意の型の要素を格納することが可能なコンテナを提供する。


== 動作条件
* ANSI C準拠のC/C++コンパイラ
* stdlib.h, assert.hが使用可能


== インストール
* 手動でコピーする方法

  ((<SourceForge.jp|URL:http://sourceforge.jp/projects/cstl/files/>))からtarballをダウンロードする。
  tarballを展開し、cstlディレクトリをインクルードパスの通ったディレクトリにコピーする。

* ((<SVNリポジトリ|URL:http://svn.sourceforge.jp/svnroot/cstl/>))からチェックアウトする方法

  インクルードパスの通ったディレクトリに移動し、以下のコマンドを実行する。
    最新スナップショットをチェックアウト
    > svn checkout http://svn.sourceforge.jp/svnroot/cstl/trunk/cstl ./cstl
    特定のバージョンをチェックアウト(x.x.xはバージョン番号)
    > svn checkout http://svn.sourceforge.jp/svnroot/cstl/tags/rel-x.x.x/cstl ./cstl


== 提供コンテナ
CSTLは以下のコンテナを提供する。

* ((<vector>))

  可変長配列。許容量を超えた要素の追加をした場合、自動的に拡張する。
  末尾での要素の挿入・削除が高速であり、それ以外の位置の要素の挿入・削除と要素の検索は遅い。
  インデックスによる要素のランダムアクセスが可能。
  ((<アルゴリズム|algorithm>))が使用可能。

* ((<deque>))

  両端キュー。
  先頭と末尾での要素の挿入・削除が高速であり、それ以外の位置の要素の挿入・削除と要素の検索は遅い。
  インデックスによる要素のランダムアクセスが可能。
  ((<アルゴリズム|algorithm>))が使用可能。

* ((<list>))

  双方向リンクリスト。
  要素の挿入・削除が高速であり、要素の検索は遅い。

* ((<set|"set/multiset">))

  要素が値によって自動的にソートされるコンテナ。要素は定数となる。
  要素の重複は許されない。
  要素の挿入・削除・検索が速い。

* ((<multiset|"set/multiset">))

  要素の重複が許されることを除き、setと同じである。

* ((<map|"map/multimap">))

  キーと値のペアを要素とするコンテナ。要素はキーによって自動的にソートされる。キーは定数となる。
  キーの重複は許されない。
  要素の挿入・削除が速い。キーの検索は速く、値の検索は遅い。
  キーによる要素のランダムアクセスが可能。
  連想配列として使用可能。

* ((<multimap|"map/multimap">))

  キーの重複が許されることと、キーによる要素のランダムアクセスが不可能であることを除き、mapと同じである。
  辞書として使用可能。

* ((<string>))

  可変長文字列。許容量を超えた文字の追加をした場合、自動的に拡張する。
  ((<アルゴリズム|algorithm>))が使用可能。


== リファレンスマニュアル
=== vector
vectorを使うには、vector.hというヘッダファイルをインクルードする。
  #include <cstl/vector.h>

以下のマクロを用いてコードを展開する必要がある。

  /* インターフェイスを展開 */
  #define CSTL_VECTOR_INTERFACE(Name, Type)

  /* 実装を展開 */
  #define CSTL_VECTOR_IMPLEMENT(Name, Type)

Nameに既存の型と重複しない任意の名前を、Typeに任意の要素の型を指定する。
<<< br

CSTL_VECTOR_INTERFACEの引数のNameにVector, TypeにTを指定した場合、
以下のインターフェイスを提供する。

==== 型

  Vector
コンテナの型。抽象データ型となっており、以下の関数によってのみアクセスできる。

==== 関数
以下の関数において、Vector*型の引数はNULLでないことを事前条件に含める。

+ 生成
  Vector *Vector_new(size_t n);
* 許容量(内部メモリの再割り当てを行わずに格納できる要素数)がn個のvectorを生成する。
* 生成に成功した場合、そのオブジェクトへのポインタを返す。
* メモリ不足の場合、NULLを返す。
<<< br

+ 破棄
  void Vector_delete(Vector *self);
* selfのすべての要素を削除し、selfを破棄する。
<<< br

+ サイズ
  size_t Vector_size(Vector *self);
* selfの現在の要素数を返す。
<<< br

  int Vector_empty(Vector *self);
* selfが空の場合、0以外の値を返す。
* selfが空でない場合、0を返す。
<<< br

+ 許容量
  size_t Vector_capacity(Vector *self);
* selfの許容量を返す。
<<< br

  int Vector_reserve(Vector *self, size_t n);
* selfの許容量を要素n個の領域に拡張する。
* selfが持つ要素は維持され、拡張した領域の初期化はしない。
* 拡張に成功した場合、0以外の値を返す。
* nがselfの現在の許容量以下の場合、selfの変更を行わず0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< br

  void Vector_shrink(Vector *self, size_t n);
* selfの許容量を要素n個の領域に縮小する。
* nがselfの現在の要素数以下の場合、selfの許容量を要素数と同じにする。
* nがselfの現在の許容量以上の場合、何もしない。
<<< br

+ 代入
  int Vector_assign(Vector *self, Vector *x, size_t idx, size_t n);
* xのidxが示すインデックスの位置からn個の要素のコピーをselfの要素として代入する。
* selfとxは同じオブジェクトでもよい。
* 代入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、idx + nがxの現在の要素数以下の値であること。
<<< br

  void Vector_swap(Vector *self, Vector *x);
* selfとxの内容を交換する。
* 要素のコピーをしないので、Vector_assign(self, x, 0, Vector_size(x))よりも速い。
  xが不要になる場合、こちらを使用するべきである。
<<< br

+ 要素のアクセス
  T *Vector_at(Vector *self, size_t idx);
* selfのidxが示すインデックスの要素へのポインタを返す。
* 事前条件は、idxがselfの現在の要素数より小さい値であること。
<<< br

  T Vector_front(Vector *self);
* selfの最初の要素を返す。
* 事前条件は、selfが空でないこと。
<<< br

  T Vector_back(Vector *self);
* selfの最後の要素を返す。
* 事前条件は、selfが空でないこと。
<<< br

+ 挿入
  int Vector_insert(Vector *self, size_t idx, T elem);
* selfのidxが示すインデックスの位置にelemのコピーを挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、idxがselfの現在の要素数以下の値であること。
<<< br

  int Vector_insert_n(Vector *self, size_t idx, size_t n, T elem);
* selfのidxが示すインデックスの位置にelemのコピーをn個挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、idxがselfの現在の要素数以下の値であること。
<<< br

  int Vector_insert_array(Vector *self, size_t idx, T *elems, size_t n);
* selfのidxが示すインデックスの位置にelemsという配列からn個の要素のコピーを挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、elemsがNULLでないこと、かつidxがselfの現在の要素数以下の値であること。
<<< br

  int Vector_insert_range(Vector *self, size_t idx, Vector *x, size_t xidx, size_t n);
* selfのidxが示すインデックスの位置にxのxidxが示すインデックスの位置からn個の要素のコピーを挿入する。
* selfとxは同じオブジェクトでもよい。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、xidx + nがxの現在の要素数以下の値であること、かつidxがselfの現在の要素数以下の値であること。
<<< br

  int Vector_push_back(Vector *self, T elem);
* elemのコピーをselfの最後の要素として追加する。
* 追加に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< br

+ 削除
  void Vector_erase(Vector *self, size_t idx, size_t n);
* selfのidxが示すインデックスの位置からn個の要素を削除する。
* 事前条件は、idx + nがselfの現在の要素数以下の値であること。
<<< br

  T Vector_pop_back(Vector *self);
* selfの最後の要素を削除し、その要素を返す。
* 事前条件は、selfが空でないこと。
<<< br

  void Vector_clear(Vector *self);
* selfのすべての要素を削除する。
<<< br

+ サイズの変更
  int Vector_resize(Vector *self, size_t n, T elem);
* selfの要素数をn個に変更する。
* nがselfの現在の要素数以下の場合、要素数がnになるまで末尾から要素が削除される。
* nがselfの現在の要素数より大きい場合、要素数がnになるまでelemのコピーが末尾から追加される。
* 要素数の変更に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< br


=== deque
dequeを使うには、deque.hというヘッダファイルをインクルードする。
  #include <cstl/deque.h>

以下のマクロを用いてコードを展開する必要がある。

  /* インターフェイスを展開 */
  #define CSTL_DEQUE_INTERFACE(Name, Type)

  /* 実装を展開 */
  #define CSTL_DEQUE_IMPLEMENT(Name, Type)

Nameに既存の型と重複しない任意の名前を、Typeに任意の要素の型を指定する。
<<< br

CSTL_DEQUE_INTERFACEの引数のNameにDeque, TypeにTを指定した場合、
以下のインターフェイスを提供する。

==== 型

  Deque
コンテナの型。抽象データ型となっており、以下の関数によってのみアクセスできる。

==== 関数
以下の関数において、Deque*型の引数はNULLでないことを事前条件に含める。

+ 生成
  Deque *Deque_new(void);
* dequeを生成する。
* 生成に成功した場合、そのオブジェクトへのポインタを返す。
* メモリ不足の場合、NULLを返す。
<<< br

+ 破棄
  void Deque_delete(Deque *self);
* selfのすべての要素を削除し、selfを破棄する。
<<< br

+ サイズ
  size_t Deque_size(Deque *self);
* selfの現在の要素数を返す。
<<< br

  int Deque_empty(Deque *self);
* selfが空の場合、0以外の値を返す。
* selfが空でない場合、0を返す。
<<< br

+ 代入
  int Deque_assign(Deque *self, Deque *x, size_t idx, size_t n);
* xのidxが示すインデックスの位置からn個の要素のコピーをselfの要素として代入する。
* selfとxは同じオブジェクトでもよい。
* 代入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、idx + nがxの現在の要素数以下の値であること。
<<< br

  void Deque_swap(Deque *self, Deque *x);
* selfとxの内容を交換する。
* 要素のコピーをしないので、Deque_assign(self, x, 0, Deque_size(x))よりも速い。
  xが不要になる場合、こちらを使用するべきである。
<<< br

+ 要素のアクセス
  T *Deque_at(Deque *self, size_t idx);
* selfのidxが示すインデックスの要素へのポインタを返す。
* 事前条件は、idxがselfの現在の要素数より小さい値であること。
<<< br

  T Deque_front(Deque *self);
* selfの最初の要素を返す。
* 事前条件は、selfが空でないこと。
<<< br

  T Deque_back(Deque *self);
* selfの最後の要素を返す。
* 事前条件は、selfが空でないこと。
<<< br

+ 挿入
  int Deque_insert(Deque *self, size_t idx, T elem);
* selfのidxが示すインデックスの位置にelemのコピーを挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、idxがselfの現在の要素数以下の値であること。
<<< br

  int Deque_insert_n(Deque *self, size_t idx, size_t n, T elem);
* selfのidxが示すインデックスの位置にelemのコピーをn個挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、idxがselfの現在の要素数以下の値であること。
<<< br

  int Deque_insert_array(Deque *self, size_t idx, T *elems, size_t n);
* selfのidxが示すインデックスの位置にelemsという配列からn個の要素のコピーを挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、elemsがNULLでないこと、かつidxがselfの現在の要素数以下の値であること。
<<< br

  int Deque_insert_range(Deque *self, size_t idx, Deque *x, size_t xidx, size_t n);
* selfのidxが示すインデックスの位置にxのxidxが示すインデックスの位置からn個の要素のコピーを挿入する。
* selfとxは同じオブジェクトでもよい。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、xidx + nがxの現在の要素数以下の値であること、かつidxがselfの現在の要素数以下の値であること。
<<< br

  int Deque_push_back(Deque *self, T elem);
* elemのコピーをselfの最後の要素として追加する。
* 追加に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< br

  int Deque_push_front(Deque *self, T elem);
* elemのコピーをselfの最初の要素として追加する。
* 追加に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< br

+ 削除
  void Deque_erase(Deque *self, size_t idx, size_t n);
* selfのidxが示すインデックスの位置からn個の要素を削除する。
* 事前条件は、idx + nがselfの現在の要素数以下の値であること。
<<< br

  T Deque_pop_front(Deque *self);
* selfの最初の要素を削除し、その要素を返す。
* 事前条件は、selfが空でないこと。
<<< br

  T Deque_pop_back(Deque *self);
* selfの最後の要素を削除し、その要素を返す。
* 事前条件は、selfが空でないこと。
<<< br

  void Deque_clear(Deque *self);
* selfのすべての要素を削除する。
<<< br

+ サイズの変更
  int Deque_resize(Deque *self, size_t n, T elem);
* selfの要素数をn個に変更する。
* nがselfの現在の要素数以下の場合、要素数がnになるまで末尾から要素が削除される。
* nがselfの現在の要素数より大きい場合、要素数がnになるまでelemのコピーが末尾から追加される。
* 要素数の変更に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< br


=== list
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
削除された要素を示すイテレータは無効となる。

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
* selfの最初の要素の位置を示すイテレータを返す。
<<< br

  ListIterator List_end(List *self);
* selfの最後の要素の次の位置を示すイテレータを返す。
<<< br

  ListIterator List_rbegin(List *self);
* selfの最後の要素の位置を示すイテレータを返す。
<<< br

  ListIterator List_rend(List *self);
* selfの最初の要素の前の位置を示すイテレータを返す。
<<< br

  ListIterator List_next(ListIterator pos);
* posが示す位置の要素の次のイテレータを返す。
* 事前条件は、posが有効なイテレータであり、かつList_end()またはList_rend()でないこと。
<<< br

  ListIterator List_prev(ListIterator pos);
* posが示す位置の要素の前のイテレータを返す。
* 事前条件は、posが有効なイテレータであり、かつList_end()またはList_rend()でないこと。
<<< br

+ 代入
  int List_assign(List *self, ListIterator first, ListIterator last);
* [first, last)の範囲の要素のコピーをselfの要素として代入する。
* [first, last)の要素はselfが持つ要素でもよい。
* 代入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、[first, last)が有効なイテレータであること。
<<< br

  void List_swap(List *self, List *x);
* selfとxの内容を交換する。
* 要素のコピーをしないので、List_assign(self, List_begin(x), List_end(x))よりも速い。
  xが不要になる場合、こちらを使用するべきである。
<<< br

+ 要素のアクセス
  T *List_at(ListIterator pos);
* posが示す位置の要素へのポインタを返す。
* 事前条件は、posが有効なイテレータであり、かつList_end()またはList_rend()でないこと。
<<< br

  T List_front(List *self);
* selfの最初の要素を返す。
* 事前条件は、selfが空でないこと。
<<< br

  T List_back(List *self);
* selfの最後の要素を返す。
* 事前条件は、selfが空でないこと。
<<< br

+ 挿入
  ListIterator List_insert(List *self, ListIterator pos, T elem);
* selfのposが示す位置にelemのコピーを挿入する。
* 挿入に成功した場合、新しい要素の位置を示すイテレータを返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、posがselfの有効なイテレータであること。
<<< br

  int List_insert_n(List *self, ListIterator pos, size_t n, T elem);
* selfのposが示す位置にelemのコピーをn個挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、posがselfの有効なイテレータであること。
<<< br

  int List_insert_array(List *self, ListIterator pos, T *elems, size_t n);
* selfのposが示す位置にelemsという配列からn個の要素のコピーを挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、elemsがNULLでないこと、かつposがselfの有効なイテレータであること。
<<< br

  int List_insert_range(List *self, ListIterator pos, ListIterator first, ListIterator last);
* selfのposが示す位置に[first, last)の範囲の要素のコピーを挿入する。
* [first, last)の要素はselfが持つ要素でもよい。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、posと[first, last)がselfの有効なイテレータであること。
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
* selfのposが示す位置の要素を削除し、その次の位置を示すイテレータを返す。
* 事前条件は、posがselfの有効なイテレータであり、かつList_end()またはList_rend()でないこと。
<<< br

  ListIterator List_erase_range(List *self, ListIterator first, ListIterator last);
* selfの[first, last)の範囲の要素を削除し、削除した要素の次の位置を示すイテレータを返す。
* 事前条件は、[first, last)がselfの有効なイテレータであること。
<<< br

  T List_pop_front(List *self);
* selfの最初の要素を削除し、その要素を返す。
* 事前条件は、selfが空でないこと。
<<< br

  T List_pop_back(List *self);
* selfの最後の要素を削除し、その要素を返す。
* 事前条件は、selfが空でないこと。
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

+ つなぎ替え
  void List_splice(List *self, ListIterator pos, List *x, ListIterator first, ListIterator last);
* selfのposが示す位置にxの[first, last)の範囲の要素を移動する。
* [first, last)の要素の数だけselfの要素数が増加し、xの要素数が減少する。
* 事前条件は、posがselfの有効なイテレータであり、かつ[first, last)がxの有効なイテレータであり、
  かつselfとxが同一ならばposは[first, last)の範囲外であること。
<<< br

+ ソート
  void List_sort(List *self, int (*comp)(const T *p1, const T *p2));
* selfのすべての要素を比較関数compに従ってソートする。
* compには、*p1 = *p2ならば0を、*p1 < *p2ならば正または負の整数を、*p1 > *p2ならば*p1 < *p2の場合と逆の符号の整数を返す関数を指定する。
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
* compには、*p1 = *p2ならば0を、*p1 < *p2ならば正または負の整数を、*p1 > *p2ならば*p1 < *p2の場合と逆の符号の整数を返す関数を指定する。
* selfとxがソートされていない場合の動作は未定義である。
<<< br


=== set/multiset
set/multisetを使うには、set.hというヘッダファイルをインクルードする。
  #include <cstl/set.h>

以下のマクロを用いてコードを展開する必要がある。

* setの場合
    /* インターフェイスを展開 */
    #define CSTL_SET_INTERFACE(Name, Type)

    /* 実装を展開 */
    #define CSTL_SET_IMPLEMENT(Name, Type, Compare)

* multisetの場合
    /* インターフェイスを展開 */
    #define CSTL_MULTISET_INTERFACE(Name, Type)

    /* 実装を展開 */
    #define CSTL_MULTISET_IMPLEMENT(Name, Type, Compare)

Nameに既存の型と重複しない任意の名前を、Typeに任意の要素の型を指定する。

Compareに要素の比較ルーチンを指定する。
  * Typeが整数型、小数型、ポインタ型など、2つの値を単純に比較できる型の場合、
    要素のソートの順序を昇順にするならばCSTL_LESSマクロを、降順にするならばCSTL_GREATERマクロを指定する。
    CSTL_LESS/CSTL_GREATERマクロはヘッダで以下のように定義されている。
      #define CSTL_LESS(x, y)     ((x) == (y) ? 0 : (x) < (y) ? -1 : 1)
      #define CSTL_GREATER(x, y)  ((x) == (y) ? 0 : (x) > (y) ? -1 : 1)
  * Typeがその他の型の場合、以下のプロトタイプのような引数と戻り値を持ち、
    x = yならば0を、x < yならば正または負の整数を、x > yならばx < yの場合と逆の符号の整数を
    返す比較ルーチンを指定する。
    尚、Typeが文字列型(char*)ならば、C標準関数のstrcmpが指定可能である。
      int Compare(Type x, Type y);

<<< br

CSTL_SET_INTERFACE/CSTL_MULTISET_INTERFACEの引数のNameにSet, TypeにTを指定した場合、
以下のインターフェイスを提供する。

==== 型

  Set
コンテナの型。抽象データ型となっており、以下の関数によってのみアクセスできる。

  SetIterator
イテレータの型。要素の位置を示す。
削除された要素を示すイテレータは無効となる。

==== 関数
以下の関数において、Set*型の引数はNULLでないことを事前条件に含める。

+ 生成
  Set *Set_new(void);
* set/multisetを生成する。
* 生成に成功した場合、そのオブジェクトへのポインタを返す。
* メモリ不足の場合、NULLを返す。
<<< br

+ 破棄
  void Set_delete(Set *self);
* selfのすべての要素を削除し、selfを破棄する。
<<< br

+ サイズ
  size_t Set_size(Set *self);
* selfの現在の要素数を返す。
<<< br

  int Set_empty(Set *self);
* selfが空の場合、0以外の値を返す。
* selfが空でない場合、0を返す。
<<< br

+ イテレータ
  SetIterator Set_begin(Set *self);
* selfの最初の要素の位置を示すイテレータを返す。
<<< br

  SetIterator Set_end(Set *self);
* selfの最後の要素の次の位置を示すイテレータを返す。
<<< br

  SetIterator Set_rbegin(Set *self);
* selfの最後の要素の位置を示すイテレータを返す。
<<< br

  SetIterator Set_rend(Set *self);
* selfの最初の要素の前の位置を示すイテレータを返す。
<<< br

  SetIterator Set_next(SetIterator pos);
* posが示す位置の要素の次のイテレータを返す。
* 事前条件は、posが有効なイテレータであり、かつSet_end()またはSet_rend()でないこと。
<<< br

  SetIterator Set_prev(SetIterator pos);
* posが示す位置の要素の前のイテレータを返す。
* 事前条件は、posが有効なイテレータであり、かつSet_end()またはSet_rend()でないこと。
<<< br

+ 代入
  int Set_assign(Set *self, SetIterator first, SetIterator last);
* [first, last)の範囲の要素のコピーをselfの要素として代入する。
* [first, last)の要素はselfが持つ要素でもよい。
* 代入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、[first, last)が有効なイテレータであること。
<<< br

  void Set_swap(Set *self, Set *x);
* selfとxの内容を交換する。
* 要素のコピーをしないので、Set_assign(self, Set_begin(x), Set_end(x))よりも速い。
  xが不要になる場合、こちらを使用するべきである。
<<< br

+ 要素のアクセス
  T Set_key(SetIterator pos);
* posが示す位置の要素を返す。
* 事前条件は、posが有効なイテレータであり、かつSet_end()またはSet_rend()でないこと。
<<< br

+ 挿入
  SetIterator Set_insert(Set *self, T elem, int *success);
* elemのコピーをselfに挿入する。
* 挿入に成功した場合、*successに0以外の値を格納し、新しい要素の位置を示すイテレータを返す。
* selfが既にelemという要素を持っている場合、挿入を行わず、*successに0を格納し、その要素の位置を示すイテレータを返す。
* メモリ不足の場合、*successに0を格納し、selfの変更を行わず0を返す。
* successにNULLを指定した場合、*successにアクセスしない。
* この関数はsetのみで提供される。
<<< br

  SetIterator Set_insert(Set *self, T elem);
* elemのコピーをselfに挿入する。
* 挿入に成功した場合、新しい要素の位置を示すイテレータを返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* selfが既にelemという要素を持っている場合、その値の一番最後の位置に挿入される。
* この関数はmultisetのみで提供される。
<<< br

+ 削除
  SetIterator Set_erase(Set *self, SetIterator pos);
* selfのposが示す位置の要素を削除し、その次の位置を示すイテレータを返す。
* 事前条件は、posがselfの有効なイテレータであり、かつSet_end()またはSet_rend()でないこと。
<<< br

  SetIterator Set_erase_range(Set *self, SetIterator first, SetIterator last);
* selfの[first, last)の範囲の要素を削除し、削除した要素の次の位置を示すイテレータを返す。
* 事前条件は、[first, last)がselfの有効なイテレータであること。
<<< br

  size_t Set_erase_key(Set *self, T elem);
* selfのelemという要素をすべて削除し、削除した数を返す。
<<< br

  void Set_clear(Set *self);
* selfのすべての要素を削除する。
<<< br

+ 検索
  size_t Set_count(Set *self, T elem);
* selfのelemという要素の数を返す。
<<< br

  SetIterator Set_find(Set *self, T elem);
* selfのelemという要素を検索し、最初に見つかった要素の位置を示すイテレータを返す。
* 見つからなければSet_end(self)を返す。
<<< br

  SetIterator Set_lower_bound(Set *self, T elem);
* ソートの基準に従い、selfのelem以上の要素の最初の位置を示すイテレータを返す。
* 見つからなければSet_end(self)を返す。
<<< br

  SetIterator Set_upper_bound(Set *self, T elem);
* ソートの基準に従い、selfのelemより大きい要素の最初の位置を示すイテレータを返す。
* 見つからなければSet_end(self)を返す。
<<< br


=== map/multimap
map/multimapを使うには、map.hというヘッダファイルをインクルードする。
  #include <cstl/map.h>

以下のマクロを用いてコードを展開する必要がある。

* mapの場合
    /* インターフェイスを展開 */
    #define CSTL_MAP_INTERFACE(Name, KeyType, ValueType)

    /* 実装を展開 */
    #define CSTL_MAP_IMPLEMENT(Name, KeyType, ValueType, Compare)

* multimapの場合
    /* インターフェイスを展開 */
    #define CSTL_MULTIMAP_INTERFACE(Name, KeyType, ValueType)

    /* 実装を展開 */
    #define CSTL_MULTIMAP_IMPLEMENT(Name, KeyType, ValueType, Compare)

Nameに既存の型と重複しない任意の名前を、KeyTypeに任意の要素のキーの型を、ValueTypeに任意の要素の値の型を指定する。

Compareに要素の比較ルーチンを指定する。
  * KeyTypeが整数型、小数型、ポインタ型など、2つの値を単純に比較できる型の場合、
    要素のソートの順序を昇順にするならばCSTL_LESSマクロを、降順にするならばCSTL_GREATERマクロを指定する。
    CSTL_LESS/CSTL_GREATERマクロはヘッダで以下のように定義されている。
      #define CSTL_LESS(x, y)     ((x) == (y) ? 0 : (x) < (y) ? -1 : 1)
      #define CSTL_GREATER(x, y)  ((x) == (y) ? 0 : (x) > (y) ? -1 : 1)
  * KeyTypeがその他の型の場合、以下のプロトタイプのような引数と戻り値を持ち、
    x = yならば0を、x < yならば正または負の整数を、x > yならばx < yの場合と逆の符号の整数を
    返す比較ルーチンを指定する。
    尚、KeyTypeが文字列型(char*)ならば、C標準関数のstrcmpが指定可能である。
      int Compare(KeyType x, KeyType y);

<<< br

CSTL_MAP_INTERFACE/CSTL_MULTIMAP_INTERFACEの引数のNameにMap, KeyTypeにKeyT, ValueTypeにValueTを指定した場合、
以下のインターフェイスを提供する。

==== 型

  Map
コンテナの型。抽象データ型となっており、以下の関数によってのみアクセスできる。

  MapIterator
イテレータの型。要素の位置を示す。
削除された要素を示すイテレータは無効となる。

==== 関数
以下の関数において、Map*型の引数はNULLでないことを事前条件に含める。

+ 生成
  Map *Map_new(void);
* map/multimapを生成する。
* 生成に成功した場合、そのオブジェクトへのポインタを返す。
* メモリ不足の場合、NULLを返す。
<<< br

+ 破棄
  void Map_delete(Map *self);
* selfのすべての要素を削除し、selfを破棄する。
<<< br

+ サイズ
  size_t Map_size(Map *self);
* selfの現在の要素数を返す。
<<< br

  int Map_empty(Map *self);
* selfが空の場合、0以外の値を返す。
* selfが空でない場合、0を返す。
<<< br

+ イテレータ
  MapIterator Map_begin(Map *self);
* selfの最初の要素の位置を示すイテレータを返す。
<<< br

  MapIterator Map_end(Map *self);
* selfの最後の要素の次の位置を示すイテレータを返す。
<<< br

  MapIterator Map_rbegin(Map *self);
* selfの最後の要素の位置を示すイテレータを返す。
<<< br

  MapIterator Map_rend(Map *self);
* selfの最初の要素の前の位置を示すイテレータを返す。
<<< br

  MapIterator Map_next(MapIterator pos);
* posが示す位置の要素の次のイテレータを返す。
* 事前条件は、posが有効なイテレータであり、かつMap_end()またはMap_rend()でないこと。
<<< br

  MapIterator Map_prev(MapIterator pos);
* posが示す位置の要素の前のイテレータを返す。
* 事前条件は、posが有効なイテレータであり、かつMap_end()またはMap_rend()でないこと。
<<< br

+ 代入
  int Map_assign(Map *self, MapIterator first, MapIterator last);
* [first, last)の範囲の要素のコピーをselfの要素として代入する。
* [first, last)の要素はselfが持つ要素でもよい。
* 代入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、[first, last)が有効なイテレータであること。
<<< br

  void Map_swap(Map *self, Map *x);
* selfとxの内容を交換する。
* 要素のコピーをしないので、Map_assign(self, Map_begin(x), Map_end(x))よりも速い。
  xが不要になる場合、こちらを使用するべきである。
<<< br

+ 要素のアクセス
  KeyT Map_key(MapIterator pos);
* posが示す位置の要素のキーを返す。
* 事前条件は、posが有効なイテレータであり、かつMap_end()またはMap_rend()でないこと。
<<< br

  ValueT *Map_value(MapIterator pos);
* posが示す位置の要素の値へのポインタを返す。
* 事前条件は、posが有効なイテレータであり、かつMap_end()またはMap_rend()でないこと。
<<< br

  ValueT *Map_lookup(Map *self, KeyT key);
* selfのkeyというキーの要素の値へのポインタを返す。
* selfがkeyというキーの要素を持っていない場合、keyというキーの新しい要素(値は不定)を挿入し、その要素の値へのポインタを返す。
* メモリ不足の場合、NDEBUGマクロが未定義ならばアサーションに失敗し、定義済みならばselfの変更を行わずNULLを返す。
* この関数はmapのみで提供される。
<<< br

+ 挿入
  MapIterator Map_insert(Map *self, KeyT key, ValueT value, int *success);
* keyとvalueのコピーのペアを要素としてselfに挿入する。
* 挿入に成功した場合、*successに0以外の値を格納し、新しい要素の位置を示すイテレータを返す。
* selfが既にkeyというキーの要素を持っている場合、挿入を行わず、*successに0を格納し、その要素の位置を示すイテレータを返す。
* メモリ不足の場合、*successに0を格納し、selfの変更を行わず0を返す。
* successにNULLを指定した場合、*successにアクセスしない。
* この関数はmapのみで提供される。
<<< br

  MapIterator Map_insert(Map *self, KeyT key, ValueT value);
* keyとvalueのコピーのペアを要素としてselfに挿入する。
* 挿入に成功した場合、新しい要素の位置を示すイテレータを返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* selfが既にkeyというキーの要素を持っている場合、そのキーの一番最後の位置に挿入される。
* この関数はmultimapのみで提供される。
<<< br

+ 削除
  MapIterator Map_erase(Map *self, MapIterator pos);
* selfのposが示す位置の要素を削除し、その次の位置を示すイテレータを返す。
* 事前条件は、posがselfの有効なイテレータであり、かつMap_end()またはMap_rend()でないこと。
<<< br

  MapIterator Map_erase_range(Map *self, MapIterator first, MapIterator last);
* selfの[first, last)の範囲の要素を削除し、削除した要素の次の位置を示すイテレータを返す。
* 事前条件は、[first, last)がselfの有効なイテレータであること。
<<< br

  size_t Map_erase_key(Map *self, KeyT key);
* selfのkeyというキーの要素をすべて削除し、削除した数を返す。
<<< br

  void Map_clear(Map *self);
* selfのすべての要素を削除する。
<<< br

+ 検索
  size_t Map_count(Map *self, KeyT key);
* selfのkeyというキーの要素の数を返す。
<<< br

  MapIterator Map_find(Map *self, KeyT key);
* selfのkeyというキーの要素を検索し、最初に見つかった要素の位置を示すイテレータを返す。
* 見つからなければMap_end(self)を返す。
<<< br

  MapIterator Map_lower_bound(Map *self, KeyT key);
* ソートの基準に従い、selfのkey以上のキーの要素の最初の位置を示すイテレータを返す。
* 見つからなければMap_end(self)を返す。
<<< br

  MapIterator Map_upper_bound(Map *self, KeyT key);
* ソートの基準に従い、selfのkeyより大きいキーの要素の最初の位置を示すイテレータを返す。
* 見つからなければMap_end(self)を返す。
<<< br


=== string
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
* CharT *cstr, size_t cstr_lenという関数の引数において、cstr_lenにCSTL_NPOSを指定した場合、cstrはCの文字列として扱われる。

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
* selfのidxが示すインデックスの文字へのポインタを返す。
* 事前条件は、idxがselfの現在の文字数より小さい値であること。
<<< br

  CharT *String_c_str(String *self);
* selfの文字列を'\0'で終端したCの文字列として返す。
<<< br

+ 代入
  int String_assign(String *self, CharT *cstr, size_t cstr_len);
* selfにcstrという文字の配列からcstr_len個の文字を代入する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列を代入する。ただしcstrは'\0'で終端していなければならない。
* cstrはself内の文字列でもよい。
* 代入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、cstrがNULLでないこと。
<<< br

  int String_assign_c(String *self, size_t n, CharT c);
* selfにcという文字をn個代入する。
* 代入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< br

  void String_swap(String *self, String *x);
* selfとxの内容を交換する。
* 文字のコピーをしないので、String_assign(self, String_c_str(x), String_size(x))よりも速い。
  xが不要になる場合、こちらを使用するべきである。
<<< br

+ 追加
  int String_append(String *self, CharT *cstr, size_t cstr_len);
* selfにcstrという文字の配列からcstr_len個の文字を追加する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列を追加する。ただしcstrは'\0'で終端していなければならない。
* cstrはself内の文字列でもよい。
* 追加に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、cstrがNULLでないこと。
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
  int String_insert(String *self, size_t idx, CharT *cstr, size_t cstr_len);
* selfのidxが示すインデックスにcstrという文字の配列からcstr_len個の文字を挿入する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列を挿入する。ただしcstrは'\0'で終端していなければならない。
* cstrはself内の文字列でもよい。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、cstrがNULLでないこと、かつidxがselfの文字数以下の値であること。
<<< br

  int String_insert_c(String *self, size_t idx, size_t n, CharT c);
* selfのidxが示すインデックスにcという文字をn個挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、idxがselfの文字数以下の値であること。
<<< br

+ 置換
  int String_replace(String *self, size_t idx, size_t len, CharT *cstr, size_t cstr_len);
* selfのidxが示すインデックスから最大len個の文字を、cstrという文字の配列のcstr_len個の文字で置換する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列で置換する。ただしcstrは'\0'で終端していなければならない。
* cstrはself内の文字列でもよい。
* 置換に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、cstrがNULLでないこと、かつidxがselfの文字数以下の値であること。
<<< br

  int String_replace_c(String *self, size_t idx, size_t len, size_t n, CharT c);
* selfのidxが示すインデックスから最大len個の文字を、cという文字n個で置換する。
* 置換に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件は、idxがselfの文字数以下の値であること。
<<< br

+ 削除
  void String_erase(String *self, size_t idx, size_t len);
* selfのidxが示すインデックスの文字から最大len個の文字を削除する。
* 事前条件は、idxがselfの文字数以下の値であること。
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

+ 検索
  size_t String_find(CharT *x, CharT *cstr, size_t idx, size_t cstr_len);
* xというCの文字列において、idxが示すインデックスから、cstrという文字の配列のcstr_len個の文字が現れる最初の位置を前進して検索する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列を検索する。ただしcstrは'\0'で終端していなければならない。
* 検索に成功した場合、見つかった部分文字列の最初の文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件は、x, cstrが共にNULLでないこと。
<<< br

  size_t String_find_c(CharT *x, CharT c, size_t idx);
* xというCの文字列において、idxが示すインデックスから、cという文字が現れる最初の位置を前進して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件は、xがNULLでないこと。
<<< br

  size_t String_rfind(CharT *x, CharT *cstr, size_t idx, size_t cstr_len);
* xというCの文字列において、idxが示すインデックスから、cstrという文字の配列のcstr_len個の文字が現れる最後の位置を後退して検索する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列を検索する。ただしcstrは'\0'で終端していなければならない。
* 検索に成功した場合、見つかった部分文字列の最初の文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件は、x, cstrが共にNULLでないこと。
<<< br

  size_t String_rfind_c(CharT *x, CharT c, size_t idx);
* xというCの文字列において、idxが示すインデックスから、cという文字が現れる最後の位置を後退して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件は、xがNULLでないこと。
<<< br

  size_t String_find_first_of(CharT *x, CharT *cstr, size_t idx, size_t cstr_len);
* xというCの文字列において、idxが示すインデックスから、cstrという文字の配列のcstr_len個の文字に含まれる最初の文字が現れる位置を前進して検索する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列に含まれる文字を検索する。ただしcstrは'\0'で終端していなければならない。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件は、x, cstrが共にNULLでないこと。
<<< br

  size_t String_find_first_of_c(CharT *x, CharT c, size_t idx);
* xというCの文字列において、idxが示すインデックスから、cという文字が現れる最初の位置を前進して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件は、xがNULLでないこと。
<<< br

  size_t String_find_last_of(CharT *x, CharT *cstr, size_t idx, size_t cstr_len);
* xというCの文字列において、idxが示すインデックスから、cstrという文字の配列のcstr_len個の文字に含まれる最後の文字が現れる位置を後退して検索する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列に含まれる文字を検索する。ただしcstrは'\0'で終端していなければならない。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件は、x, cstrが共にNULLでないこと。
<<< br

  size_t String_find_last_of_c(CharT *x, CharT c, size_t idx);
* xというCの文字列において、idxが示すインデックスから、cという文字が現れる最後の位置を後退して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件は、xがNULLでないこと。
<<< br

  size_t String_find_first_not_of(CharT *x, CharT *cstr, size_t idx, size_t cstr_len);
* xというCの文字列において、idxが示すインデックスから、cstrという文字の配列のcstr_len個の文字に含まれない最初の文字が現れる位置を前進して検索する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列に含まれない文字を検索する。ただしcstrは'\0'で終端していなければならない。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件は、x, cstrが共にNULLでないこと。
<<< br

  size_t String_find_first_not_of_c(CharT *x, CharT c, size_t idx);
* xというCの文字列において、idxが示すインデックスから、c以外の文字が現れる最初の位置を前進して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件は、xがNULLでないこと。
<<< br

  size_t String_find_last_not_of(CharT *x, CharT *cstr, size_t idx, size_t cstr_len);
* xというCの文字列において、idxが示すインデックスから、cstrという文字の配列のcstr_len個の文字に含まれない最後の文字が現れる位置を後退して検索する。
* cstr_lenがCSTL_NPOSと等しい場合、cstrというCの文字列に含まれない文字を検索する。ただしcstrは'\0'で終端していなければならない。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件は、x, cstrが共にNULLでないこと。
<<< br

  size_t String_find_last_not_of_c(CharT *x, CharT c, size_t idx);
* xというCの文字列において、idxが示すインデックスから、c以外の文字が現れる最後の位置を後退して検索する。
* 検索に成功した場合、その文字のインデックスを返す。
* 検索に失敗した場合、CSTL_NPOSを返す。
* 事前条件は、xがNULLでないこと。
<<< br


=== algorithm
CSTLは、vector, deque, stringにおいて、共通なアルゴリズムを提供する。

アルゴリズムを使うには、実装マクロCSTL_XXX_IMPLEMENT(XXXは、VECTOR, DEQUE, STRINGのいずれか)を展開する前に、
algorithm.hというヘッダファイルをインクルードする。

  #include <cstl/algorithm.h>

ここではvectorを例として説明する。
deque, stringの場合もインターフェイスは同じである。

CSTL_VECTOR_INTERFACEの引数のNameにVector, TypeにTを指定した場合、
以下のインターフェイスを提供する。

==== 関数
以下の関数において、Vector*型の引数はNULLでないことを事前条件に含める。

また、int (*comp)(const T *p1, const T *p2)という関数ポインタの引数には、*p1 = *p2ならば0を、*p1 < *p2ならば正または負の整数を、*p1 > *p2ならば*p1 < *p2の場合と逆の符号の整数を返す関数を指定すること。

+ ソート
  void Vector_sort(Vector *self, size_t idx, size_t n, int (*comp)(const T *p1, const T *p2));
* selfのidxが示すインデックスの位置からn個の要素を比較関数compに従ってソートする。
* このソートは安定でない。
* 事前条件は、idx + nがselfの現在の要素数以下の値であること。
<<< br

  void Vector_stable_sort(Vector *self, size_t idx, size_t n, int (*comp)(const T *p1, const T *p2));
* selfのidxが示すインデックスの位置からn個の要素を比較関数compに従ってソートする。
* このソートは安定である。
* 事前条件は、idx + nがselfの現在の要素数以下の値であること。
<<< br

+ 二分探索
  size_t Vector_binary_search(Vector *self, size_t idx, size_t n, T value, int (*comp)(const T *p1, const T *p2));
* selfのidxが示すインデックスの位置からn個の要素において、valueに一致する要素のインデックスを返す。
* 一致する要素が複数ある場合、最初の要素のインデックスを返す。
* 見つからない場合、idx + nを返す。
* selfの[idx, idx + n)の範囲は比較関数compに従ってソートされていなければならない。ソートされていない場合の動作は未定義である。
* 事前条件は、idx + nがselfの現在の要素数以下の値であること。
<<< br

  size_t Vector_lower_bound(Vector *self, size_t idx, size_t n, T value, int (*comp)(const T *p1, const T *p2));
* selfのidxが示すインデックスの位置からn個の要素において、value以上の最初の要素のインデックスを返す。
* 見つからない場合、idx + nを返す。
* selfの[idx, idx + n)の範囲は比較関数compに従ってソートされていなければならない。ソートされていない場合の動作は未定義である。
* 事前条件は、idx + nがselfの現在の要素数以下の値であること。
<<< br

  size_t Vector_upper_bound(Vector *self, size_t idx, size_t n, T value, int (*comp)(const T *p1, const T *p2));
* selfのidxが示すインデックスの位置からn個の要素において、valueより大きい最初の要素のインデックスを返す。
* 見つからない場合、idx + nを返す。
* selfの[idx, idx + n)の範囲は比較関数compに従ってソートされていなければならない。ソートされていない場合の動作は未定義である。
* 事前条件は、idx + nがselfの現在の要素数以下の値であること。
<<< br

+ 並べ替え
  void Vector_reverse(Vector *self, size_t idx, size_t n);
* selfのidxが示すインデックスの位置からn個の要素を逆順に並べ替える。
* 事前条件は、idx + nがselfの現在の要素数以下の値であること。
<<< br

  void Vector_rotate(Vector *self, size_t first, size_t middle, size_t last);
* selfの[middle, last)のインデックスの範囲の要素をfirstが示すインデックスの位置に移動する。
* 事前条件は、first <= middle <= last <= selfの現在の要素数であること。
<<< br

+ マージ
  int Vector_merge(Vector *self, size_t idx, Vector *x, size_t xidx, size_t xn, 
                    Vector *y, size_t yidx, size_t yn, int (*comp)(const T *p1, const T *p2));
<<< br

  void Vector_inplace_merge(Vector *self, size_t first, size_t middle, size_t last, 
                              int (*comp)(const T *p1, const T *p2));
<<< br

+ ヒープ
  void Vector_push_heap(Vector *self, size_t idx, size_t n, int (*comp)(const T *p1, const T *p2));
<<< br

  void Vector_pop_heap(Vector *self, size_t idx, size_t n, int (*comp)(const T *p1, const T *p2));
<<< br

  void Vector_make_heap(Vector *self, size_t idx, size_t n, int (*comp)(const T *p1, const T *p2));
<<< br

  void Vector_sort_heap(Vector *self, size_t idx, size_t n, int (*comp)(const T *p1, const T *p2));
<<< br







== STLとの主な違い
* vector, deque, stringはインデックスで要素にアクセスするため、イテレータをサポートしない。
* vector, deque, listのpop_back(), pop_front()は削除した値を返す。
* vector, stringのshrink()は許容量の縮小ができる。
* stringのreserve()は許容量の縮小ができない。
* set, mapのinsert()は重複による成否を引数で返す。
* set, multiset, map, multimapのerase()は削除した次の位置のイテレータを返す。
* set, multiset, map, multimapのerase_range()は削除した次の位置のイテレータを返す。
* 未対応なメンバ関数あり。
* vector, deque, stringのみアルゴリズムをサポートする。
* アルゴリズムのbinary_search()は見つかったインデックスを返す。
* 未対応なアルゴリズムあり。


== ライセンス
本ソフトウェアのライセンスは、修正BSDライセンスに従う。

  Copyright (c) 2006, KATO Noriaki
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


=== 日本語訳(参考)

  Copyright (c) 2006, KATO Noriaki
  All rights reserved.

  ソースコード形式かバイナリ形式か、変更するかしないかを問わず、以下の条件を満た
  す場合に限り、再頒布および使用が許可されます。

  1. ソースコードを再頒布する場合、上記の著作権表示、本条件一覧、および下記免責
     条項を含めること。
  2. バイナリ形式で再頒布する場合、頒布物に付属のドキュメント等の資料に、上記の
     著作権表示、本条件一覧、および下記免責条項を含めること。

  本ソフトウェアは、著作権者によって「現状のまま」提供されており、明示黙示を問わ
  ず、商業的な使用可能性、および特定の目的に対する適合性に関する暗黙の保証も含
  め、またそれに限定されない、いかなる保証もありません。著作権者は、事由のいかん
  を問わず、損害発生の原因いかんを問わず、かつ責任の根拠が契約であるか厳格責任で
  あるか（過失その他の）不法行為であるかを問わず、仮にそのような損害が発生する可
  能性を知らされていたとしても、本ソフトウェアの使用によって発生した（代替品また
  は代用サービスの調達、使用の喪失、データの喪失、利益の喪失、業務の中断も含め、
  またそれに限定されない）直接損害、間接損害、偶発的な損害、特別損害、懲罰的損
  害、または結果損害について、一切責任を負わないものとします。

<<< sfjp
=end
