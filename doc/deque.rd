=begin
== deque
dequeは可変長な両端キューである。
先頭と末尾での要素の挿入・削除の計算量がO(1)であり、それ以外の位置の要素の挿入・削除の計算量はO(N)である。
インデックスによる要素のランダムアクセスが可能。

dequeを使うには、以下のマクロを用いてコードを展開する必要がある。

  #include <cstl/deque.h>

  #define CSTL_DEQUE_INTERFACE(Name, Type)
  #define CSTL_DEQUE_IMPLEMENT(Name, Type)

((*CSTL_DEQUE_INTERFACE()*))は任意の名前と要素の型のdequeのインターフェイスを展開する。
((*CSTL_DEQUE_IMPLEMENT()*))はその実装を展開する。
それぞれのマクロの引数は同じものを指定すること。
また、((*CSTL_DEQUE_INTERFACE()*))を展開する前に、algorithm.hをインクルードすることにより、
((<アルゴリズム|URL:algorithm.html>))が使用可能となる。

: Name
  既存の型と重複しない任意の名前。コンテナの型名と関数のプレフィックスになる
: Type
  任意の要素の型

=== 使用例
  #include <stdio.h>
  #include <cstl/deque.h>
  
  CSTL_DEQUE_INTERFACE(IntDeque, int) /* インターフェイスを展開 */
  CSTL_DEQUE_IMPLEMENT(IntDeque, int) /* 実装を展開 */
  
  int main(void)
  {
      int i;
      /* intのdequeを生成 */
      IntDeque *deq = IntDeque_new();
  
      for (i = 0; i < 32; i++) {
          /* 末尾から追加 */
          IntDeque_push_back(deq, i);
      }
      for (i = 0; i < 32; i++) {
          /* 先頭から追加 */
          IntDeque_push_front(deq, i);
      }
      /* サイズ */
      printf("size: %d\n", IntDeque_size(deq));
      for (i = 0; i < 64; i++) {
          /* インデックスによる要素の読み書き */
          printf("%d,", *IntDeque_at(deq, i));
          *IntDeque_at(deq, i) += 1;
          printf("%d\n", *IntDeque_at(deq, i));
      }
  
      /* 使い終わったら破棄 */
      IntDeque_delete(deq);
      return 0;
  }

※複数のソースファイルから同じ型のコンテナを使用する場合は、
マクロ展開用のヘッダファイルとソースファイルを用意し、適宜インクルードやリンクをすればよい。

<<< hr

((*CSTL_DEQUE_INTERFACE(Name, Type)*))のNameにDeque, TypeにTを指定した場合、
以下のインターフェイスを提供する。

* 型
  * ((<Deque>))
* 関数
  * 生成
    * ((<Deque_new()>))
  * 破棄
    * ((<Deque_delete()>))
  * サイズ
    * ((<Deque_size()>))
    * ((<Deque_empty()>))
  * 要素のアクセス
    * ((<Deque_at()>))
    * ((<Deque_front()>)) , ((<Deque_back()>))
  * 挿入
    * ((<Deque_insert()>)) , ((<Deque_insert_n()>)) , ((<Deque_insert_array()>)) , ((<Deque_insert_range()>))
    * ((<Deque_push_front()>)) , ((<Deque_push_back()>))
  * 削除
    * ((<Deque_erase()>))
    * ((<Deque_pop_front()>)) , ((<Deque_pop_back()>))
    * ((<Deque_clear()>))
  * サイズの変更
    * ((<Deque_resize()>))
  * 交換
    * ((<Deque_swap()>))
<<< hr

==== Deque
  Deque
コンテナの型。抽象データ型となっており、以下の関数によってのみアクセスできる。
<<< hr

==== Deque_new()
  Deque *Deque_new(void);
* dequeを生成する。
* 生成に成功した場合、そのオブジェクトへのポインタを返す。
* メモリ不足の場合、NULLを返す。
<<< hr

==== Deque_delete()
  void Deque_delete(Deque *self);
* selfのすべての要素を削除し、selfを破棄する。
* selfがNULLの場合、何もしない。
<<< hr

==== Deque_size()
  size_t Deque_size(Deque *self);
* selfの現在の要素数を返す。
<<< hr

==== Deque_empty()
  int Deque_empty(Deque *self);
* selfが空の場合、0以外の値を返す。
* selfが空でない場合、0を返す。
<<< hr

==== Deque_at()
  T *Deque_at(Deque *self, size_t idx);
* selfのidx番目の要素へのポインタを返す。
* 戻り値はselfの変更により無効となる。
* 事前条件
  * idxがselfの現在の要素数より小さい値であること。
<<< hr

==== Deque_front()
  T Deque_front(Deque *self);
* selfの最初の要素を返す。
* 事前条件
  * selfが空でないこと。
<<< hr

==== Deque_back()
  T Deque_back(Deque *self);
* selfの最後の要素を返す。
* 事前条件
  * selfが空でないこと。
<<< hr

==== Deque_insert()
  int Deque_insert(Deque *self, size_t idx, T elem);
* selfのidx番目の位置にelemのコピーを挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * idxがselfの現在の要素数以下の値であること。
<<< hr

==== Deque_insert_n()
  int Deque_insert_n(Deque *self, size_t idx, size_t n, T elem);
* selfのidx番目の位置にelemのコピーをn個挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * idxがselfの現在の要素数以下の値であること。
<<< hr

==== Deque_insert_array()
  int Deque_insert_array(Deque *self, size_t idx, const T *elems, size_t n);
* selfのidx番目の位置にelemsという配列からn個の要素のコピーを挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * elemsがNULLでないこと。
  * idxがselfの現在の要素数以下の値であること。
<<< hr

==== Deque_insert_range()
  int Deque_insert_range(Deque *self, size_t idx, Deque *x, size_t xidx, size_t n);
* selfのidx番目の位置にxのxidx番目からn個の要素のコピーを挿入する。
* selfとxは同じオブジェクトでもよい。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * xidx + nがxの現在の要素数以下の値であること。
  * idxがselfの現在の要素数以下の値であること。
<<< hr

==== Deque_push_front()
  int Deque_push_front(Deque *self, T elem);
* elemのコピーをselfの最初の要素として追加する。
* 追加に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< hr

==== Deque_push_back()
  int Deque_push_back(Deque *self, T elem);
* elemのコピーをselfの最後の要素として追加する。
* 追加に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< hr

==== Deque_erase()
  void Deque_erase(Deque *self, size_t idx, size_t n);
* selfのidx番目からn個の要素を削除する。
* 事前条件
  * idx + nがselfの現在の要素数以下の値であること。
<<< hr

==== Deque_pop_front()
  T Deque_pop_front(Deque *self);
* selfの最初の要素を削除し、その要素を返す。
* 事前条件
  * selfが空でないこと。
<<< hr

==== Deque_pop_back()
  T Deque_pop_back(Deque *self);
* selfの最後の要素を削除し、その要素を返す。
* 事前条件
  * selfが空でないこと。
<<< hr

==== Deque_clear()
  void Deque_clear(Deque *self);
* selfのすべての要素を削除する。
<<< hr

==== Deque_resize()
  int Deque_resize(Deque *self, size_t n, T elem);
* selfの要素数をn個に変更する。
* nがselfの現在の要素数以下の場合、要素数がn個になるまで末尾から要素が削除される。
* nがselfの現在の要素数より大きい場合、要素数がn個になるまでelemのコピーが末尾から追加される。
* 要素数の変更に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< hr

==== Deque_swap()
  void Deque_swap(Deque *self, Deque *x);
* selfとxの内容を交換する。
<<< hr

=end
