=begin
== deque
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

+ 要素のアクセス
  T *Deque_at(Deque *self, size_t idx);
* selfのidx番目の要素へのポインタを返す。
* 戻り値はselfの変更により無効となる。
* 事前条件
  * idxがselfの現在の要素数より小さい値であること。
<<< br

  T Deque_front(Deque *self);
* selfの最初の要素を返す。
* 事前条件
  * selfが空でないこと。
<<< br

  T Deque_back(Deque *self);
* selfの最後の要素を返す。
* 事前条件
  * selfが空でないこと。
<<< br

+ 挿入
  int Deque_insert(Deque *self, size_t idx, T elem);
* selfのidx番目の位置にelemのコピーを挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * idxがselfの現在の要素数以下の値であること。
<<< br

  int Deque_insert_n(Deque *self, size_t idx, size_t n, T elem);
* selfのidx番目の位置にelemのコピーをn個挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * idxがselfの現在の要素数以下の値であること。
<<< br

  int Deque_insert_array(Deque *self, size_t idx, const T *elems, size_t n);
* selfのidx番目の位置にelemsという配列からn個の要素のコピーを挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * elemsがNULLでないこと。
  * idxがselfの現在の要素数以下の値であること。
<<< br

  int Deque_insert_range(Deque *self, size_t idx, Deque *x, size_t xidx, size_t n);
* selfのidx番目の位置にxのxidx番目からn個の要素のコピーを挿入する。
* selfとxは同じオブジェクトでもよい。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * xidx + nがxの現在の要素数以下の値であること。
  * idxがselfの現在の要素数以下の値であること。
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
* selfのidx番目からn個の要素を削除する。
* 事前条件
  * idx + nがselfの現在の要素数以下の値であること。
<<< br

  T Deque_pop_front(Deque *self);
* selfの最初の要素を削除し、その要素を返す。
* 事前条件
  * selfが空でないこと。
<<< br

  T Deque_pop_back(Deque *self);
* selfの最後の要素を削除し、その要素を返す。
* 事前条件
  * selfが空でないこと。
<<< br

  void Deque_clear(Deque *self);
* selfのすべての要素を削除する。
<<< br

+ サイズの変更
  int Deque_resize(Deque *self, size_t n, T elem);
* selfの要素数をn個に変更する。
* nがselfの現在の要素数以下の場合、要素数がn個になるまで末尾から要素が削除される。
* nがselfの現在の要素数より大きい場合、要素数がn個になるまでelemのコピーが末尾から追加される。
* 要素数の変更に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< br

+ 交換
  void Deque_swap(Deque *self, Deque *x);
* selfとxの内容を交換する。
<<< br

=end
