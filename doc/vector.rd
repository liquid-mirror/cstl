=begin
== vector
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

+ 要素のアクセス
  T *Vector_at(Vector *self, size_t idx);
* selfのidx番目の要素へのポインタを返す。
* 事前条件
  * idxがselfの現在の要素数より小さい値であること。
<<< br

  T Vector_front(Vector *self);
* selfの最初の要素を返す。
* 事前条件
  * selfが空でないこと。
<<< br

  T Vector_back(Vector *self);
* selfの最後の要素を返す。
* 事前条件
  * selfが空でないこと。
<<< br

+ 挿入
  int Vector_insert(Vector *self, size_t idx, T elem);
* selfのidx番目の位置にelemのコピーを挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * idxがselfの現在の要素数以下の値であること。
<<< br

  int Vector_insert_n(Vector *self, size_t idx, size_t n, T elem);
* selfのidx番目の位置にelemのコピーをn個挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * idxがselfの現在の要素数以下の値であること。
<<< br

  int Vector_insert_array(Vector *self, size_t idx, T *elems, size_t n);
* selfのidx番目の位置にelemsという配列からn個の要素のコピーを挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * elemsがNULLでないこと。
  * idxがselfの現在の要素数以下の値であること。
<<< br

  int Vector_insert_range(Vector *self, size_t idx, Vector *x, size_t xidx, size_t n);
* selfのidx番目の位置にxのxidx番目からn個の要素のコピーを挿入する。
* selfとxは同じオブジェクトでもよい。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * xidx + nがxの現在の要素数以下の値であること。
  * idxがselfの現在の要素数以下の値であること。
<<< br

  int Vector_push_back(Vector *self, T elem);
* elemのコピーをselfの最後の要素として追加する。
* 追加に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< br

+ 削除
  void Vector_erase(Vector *self, size_t idx, size_t n);
* selfのidx番目からn個の要素を削除する。
* 事前条件
  * idx + nがselfの現在の要素数以下の値であること。
<<< br

  T Vector_pop_back(Vector *self);
* selfの最後の要素を削除し、その要素を返す。
* 事前条件
  * selfが空でないこと。
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

+ 交換
  void Vector_swap(Vector *self, Vector *x);
* selfとxの内容を交換する。
<<< br

=end
