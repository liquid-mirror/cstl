=begin
== list
listは双方向リンクリストである。
任意の位置での要素の挿入・削除の計算量がO(1)であるが、要素のランダムアクセスはできない。

listを使うには、以下のマクロを用いてコードを展開する必要がある。

  #include <cstl/list.h>

  #define CSTL_LIST_INTERFACE(Name, Type)
  #define CSTL_LIST_IMPLEMENT(Name, Type)

((*CSTL_LIST_INTERFACE()*))は任意の名前と要素の型のlistのインターフェイスを展開する。
((*CSTL_LIST_IMPLEMENT()*))はその実装を展開する。
それぞれのマクロの引数は同じものを指定すること。

: Name
  既存の型と重複しない任意の名前。コンテナの型名と関数のプレフィックスになる
: Type
  任意の要素の型

=== 使用例
  #include <stdio.h>
  #include <cstl/list.h>
  
  CSTL_LIST_INTERFACE(IntList, int) /* インターフェイスを展開 */
  CSTL_LIST_IMPLEMENT(IntList, int) /* 実装を展開 */
  
  int main(void)
  {
      int i;
      /* イテレータ */
      IntListIterator pos;
      /* intのlistを生成 */
      IntList *lst = IntList_new();
  
      for (i = 0; i < 32; i++) {
          /* 末尾から追加 */
          IntList_push_back(lst, i);
      }
      for (i = 0; i < 32; i++) {
          /* 先頭から追加 */
          IntList_push_front(lst, i);
      }
      /* サイズ */
      printf("size: %d\n", IntList_size(lst));
      for (pos = IntList_begin(lst); pos != IntList_end(lst); pos = IntListIterator_next(pos)) {
          /* イテレータによる要素の読み書き */
          printf("%d,", *IntListIterator_ref(pos));
          *IntListIterator_ref(pos) += 1;
          printf("%d\n", *IntListIterator_ref(pos));
      }
  
      /* 使い終わったら破棄 */
      IntList_delete(lst);
      return 0;
  }

※複数のソースファイルから同じ型のコンテナを使用する場合は、
マクロ展開用のヘッダファイルとソースファイルを用意し、適宜インクルードやリンクをすればよい。

<<< hr

((*CSTL_LIST_INTERFACE(Name, Type)*))のNameにList, TypeにTを指定した場合、
以下のインターフェイスを提供する。

* 型
  * ((<List>))
  * ((<ListIterator>))
* 関数
  * 生成
    * ((<List_new()>))
  * 破棄
    * ((<List_delete()>))
  * サイズ
    * ((<List_size()>))
    * ((<List_empty()>))
  * イテレータ
    * ((<List_begin()>)) , ((<List_end()>))
    * ((<List_rbegin()>)) , ((<List_rend()>))
    * ((<ListIterator_next()>)) , ((<ListIterator_prev()>))
    * ((<ListIterator_ref()>))
  * 要素のアクセス
    * ((<List_front()>)) , ((<List_back()>))
  * 挿入
    * ((<List_insert()>)) , ((<List_insert_n()>)) , ((<List_insert_array()>)) , ((<List_insert_range()>))
    * ((<List_push_front()>)) , ((<List_push_back()>))
  * 削除
    * ((<List_erase()>)) , ((<List_erase_range()>))
    * ((<List_pop_front()>)) , ((<List_pop_back()>))
    * ((<List_clear()>))
  * サイズの変更
    * ((<List_resize()>))
  * 交換
    * ((<List_swap()>))
  * つなぎ替え
    * ((<List_splice()>))
  * ソート
    * ((<List_sort()>))
  * 並べ替え
    * ((<List_reverse()>))
  * マージ
    * ((<List_merge()>))
<<< hr

==== List
  typedef struct List List;
コンテナの型。抽象データ型となっており、以下の関数によってのみアクセスできる。
<<< hr

==== ListIterator
  typedef ... ListIterator;
イテレータの型。要素の位置を示す。
関数から返されたイテレータを有効なイテレータという。
宣言されただけのイテレータ、または削除された要素のイテレータを無効なイテレータという。
<<< hr

==== List_new()
  List *List_new(void);
* listを生成する。
* 生成に成功した場合、そのオブジェクトへのポインタを返す。
* メモリ不足の場合、NULLを返す。
<<< hr

==== List_delete()
  void List_delete(List *self);
* selfのすべての要素を削除し、selfを破棄する。
* selfがNULLの場合、何もしない。
<<< hr

==== List_size()
  size_t List_size(List *self);
* selfの現在の要素数を返す。
<<< hr

==== List_empty()
  int List_empty(List *self);
* selfが空の場合、0以外の値を返す。
* selfが空でない場合、0を返す。
<<< hr

==== List_begin()
  ListIterator List_begin(List *self);
* selfの最初の要素のイテレータを返す。
<<< hr

==== List_end()
  ListIterator List_end(List *self);
* selfの最後の要素の次のイテレータを返す。
<<< hr

==== List_rbegin()
  ListIterator List_rbegin(List *self);
* selfの最後の要素のイテレータを返す。
<<< hr

==== List_rend()
  ListIterator List_rend(List *self);
* selfの最初の要素の前のイテレータを返す。
<<< hr

==== ListIterator_next()
  ListIterator ListIterator_next(ListIterator pos);
* posが示す位置の要素の次のイテレータを返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがList_end()またはList_rend()でないこと。
<<< hr

==== ListIterator_prev()
  ListIterator ListIterator_prev(ListIterator pos);
* posが示す位置の要素の前のイテレータを返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがList_end()またはList_rend()でないこと。
<<< hr

==== ListIterator_ref()
  T *ListIterator_ref(ListIterator pos);
* posが示す位置の要素へのポインタを返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがList_end()またはList_rend()でないこと。
<<< hr

==== List_front()
  T *List_front(List *self);
* selfの最初の要素へのポインタを返す。
* 事前条件
  * selfが空でないこと。
<<< hr

==== List_back()
  T *List_back(List *self);
* selfの最後の要素へのポインタを返す。
* 事前条件
  * selfが空でないこと。
<<< hr

==== List_insert()
  ListIterator List_insert(List *self, ListIterator pos, T elem);
* selfのposが示す位置にelemのコピーを挿入する。
* 挿入に成功した場合、新しい要素のイテレータを返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * posがselfの有効なイテレータであること。
<<< hr

==== List_insert_n()
  int List_insert_n(List *self, ListIterator pos, size_t n, T elem);
* selfのposが示す位置にelemのコピーをn個挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * posがselfの有効なイテレータであること。
<<< hr

==== List_insert_array()
  int List_insert_array(List *self, ListIterator pos, T const *elems, size_t n);
* selfのposが示す位置にelemsという配列からn個の要素のコピーを挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * elemsがNULLでないこと。
  * posがselfの有効なイテレータであること。
<<< hr

==== List_insert_range()
  int List_insert_range(List *self, ListIterator pos, ListIterator first, ListIterator last);
* selfのposが示す位置に[first, last)の範囲の要素のコピーを挿入する。
* [first, last)の要素はselfが持つ要素でもよい。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * posがselfの有効なイテレータであること。
  * [first, last)が有効なイテレータであること。
<<< hr

==== List_push_front()
  int List_push_front(List *self, T elem);
* elemのコピーをselfの最初の要素として追加する。
* 追加に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< hr

==== List_push_back()
  int List_push_back(List *self, T elem);
* elemのコピーをselfの最後の要素として追加する。
* 追加に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< hr

==== List_erase()
  ListIterator List_erase(List *self, ListIterator pos);
* selfのposが示す位置の要素を削除し、その次のイテレータを返す。
* 事前条件
  * posがselfの有効なイテレータであること。
  * posがList_end()またはList_rend()でないこと。
<<< hr

==== List_erase_range()
  ListIterator List_erase_range(List *self, ListIterator first, ListIterator last);
* selfの[first, last)の範囲の要素を削除し、削除した要素の次のイテレータを返す。
* 事前条件
  * [first, last)がselfの有効なイテレータであること。
<<< hr

==== List_pop_front()
  void List_pop_front(List *self);
* selfの最初の要素を削除する。
* 事前条件
  * selfが空でないこと。
<<< hr

==== List_pop_back()
  void List_pop_back(List *self);
* selfの最後の要素を削除する。
* 事前条件
  * selfが空でないこと。
<<< hr

==== List_clear()
  void List_clear(List *self);
* selfのすべての要素を削除する。
<<< hr

==== List_resize()
  int List_resize(List *self, size_t n, T elem);
* selfの要素数をn個に変更する。
* nがselfの現在の要素数以下の場合、要素数がn個になるまで末尾から要素が削除される。
* nがselfの現在の要素数より大きい場合、要素数がn個になるまでelemのコピーが末尾から追加される。
* 要素数の変更に成功した場合、0以外の値を返す。
* メモリ不足の場合、0を返す。
<<< hr

==== List_swap()
  void List_swap(List *self, List *x);
* selfとxの内容を交換する。
<<< hr

==== List_splice()
  void List_splice(List *self, ListIterator pos, List *x, ListIterator first, ListIterator last);
* selfのposが示す位置にxの[first, last)の範囲の要素を移動する。
* [first, last)の要素の数だけselfの要素数が増加し、xの要素数が減少する。
* 事前条件
  * posがselfの有効なイテレータであること。
  * [first, last)がxの有効なイテレータであること。
  * selfとxが同一ならばposは[first, last)の範囲外であること。
<<< hr

==== List_sort()
  void List_sort(List *self, int (*comp)(const void *p1, const void *p2));
* selfのすべての要素を比較関数compに従ってソートする。
* compには、*p1 == *p2ならば0を、*p1 < *p2ならば正または負の整数を、*p1 > *p2ならば*p1 < *p2の場合と逆の符号の整数を返す関数を指定する。
* このソートは安定である。
<<< hr

==== List_reverse()
  void List_reverse(List *self);
* selfのすべての要素を逆順に並べ替える。
<<< hr

==== List_merge()
  void List_merge(List *self, List *x, int (*comp)(const void *p1, const void *p2));
* ソートされた状態であるselfとxにおいて、xのすべての要素を比較関数compに従ってselfにマージする。
* selfはソートされた状態になり、xは空になる。
* compには、*p1 == *p2ならば0を、*p1 < *p2ならば正または負の整数を、*p1 > *p2ならば*p1 < *p2の場合と逆の符号の整数を返す関数を指定する。
* 事前条件
  * selfとxがcompに従ってソートされていること。
<<< hr

=end
