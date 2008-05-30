=begin
== algorithm
CSTLは、((<vector|URL:vector.html>)), ((<deque|URL:deque.html>)), ((<string|URL:string.html>))において、共通なアルゴリズムを提供する。

アルゴリズムを使うには、CSTL_XXX_INTERFACE(Name, Type)((-XXXは、VECTOR, DEQUE, STRINGのいずれか-))を展開する前に、
algorithm.hというヘッダファイルをインクルードする必要がある。

  #include <cstl/algorithm.h>

CSTL_XXX_INTERFACE(Name, Type)のNameにContainer, TypeにTを指定した場合、
以下のインターフェイスを提供する。

* 関数
  * ソート
    * ((<Container_sort()>)) , ((<Container_stable_sort()>))
  * 二分探索
    * ((<Container_binary_search()>))
    * ((<Container_lower_bound()>)) , ((<Container_upper_bound()>))
  * 並べ替え
    * ((<Container_reverse()>))
    * ((<Container_rotate()>))
  * マージ
    * ((<Container_merge()>))
    * ((<Container_inplace_merge()>))
  * ヒープ
    * ((<Container_make_heap()>)) , ((<Container_sort_heap()>))
    * ((<Container_push_heap()>)) , ((<Container_pop_heap()>))
<<< hr

以下の関数において、 int (*comp)(const void *p1, const void *p2)という関数ポインタには、*p1 == *p2ならば0を、*p1 < *p2ならば正または負の整数を、*p1 > *p2ならば*p1 < *p2の場合と逆の符号の整数を返す関数を指定すること。
<<< hr

==== Container_sort()
  void Container_sort(Container *self, size_t idx, size_t n, int (*comp)(const void *p1, const void *p2));
* selfのidx番目からn個の要素を比較関数compに従ってソートする。
* このソートは安定でない。
* 事前条件
  * idx + nがselfの現在の要素数以下の値であること。
<<< hr

==== Container_stable_sort()
  void Container_stable_sort(Container *self, size_t idx, size_t n, int (*comp)(const void *p1, const void *p2));
* selfのidx番目からn個の要素を比較関数compに従ってソートする。
* このソートは安定である。
* 事前条件
  * idx + nがselfの現在の要素数以下の値であること。
<<< hr

==== Container_binary_search()
  size_t Container_binary_search(Container *self, size_t idx, size_t n, T value, int (*comp)(const void *p1, const void *p2));
* selfのidx番目からn個の要素において、比較関数compに従ってvalueに一致する要素のインデックスを返す。
* 一致する要素が複数ある場合、最初の要素のインデックスを返す。
* 見つからない場合、idx + nを返す。
* 事前条件
  * idx + nがselfの現在の要素数以下の値であること。
  * selfのidx番目からn個の要素が比較関数compに従ってソートされていること。
<<< hr

==== Container_lower_bound()
  size_t Container_lower_bound(Container *self, size_t idx, size_t n, T value, int (*comp)(const void *p1, const void *p2));
* selfのidx番目からn個の要素において、比較関数compに従ってvalue以上の最初の要素のインデックスを返す。
* 見つからない場合、idx + nを返す。
* 事前条件
  * idx + nがselfの現在の要素数以下の値であること。
  * selfのidx番目からn個の要素が比較関数compに従ってソートされていること。
<<< hr

==== Container_upper_bound()
  size_t Container_upper_bound(Container *self, size_t idx, size_t n, T value, int (*comp)(const void *p1, const void *p2));
* selfのidx番目からn個の要素において、比較関数compに従ってvalueより大きい最初の要素のインデックスを返す。
* 見つからない場合、idx + nを返す。
* 事前条件
  * idx + nがselfの現在の要素数以下の値であること。
  * selfのidx番目からn個の要素が比較関数compに従ってソートされていること。
<<< hr

==== Container_reverse()
  void Container_reverse(Container *self, size_t idx, size_t n);
* selfのidx番目からn個の要素を逆順に並べ替える。
* 事前条件
  * idx + nがselfの現在の要素数以下の値であること。
<<< hr

==== Container_rotate()
  void Container_rotate(Container *self, size_t first, size_t middle, size_t last);
* selfのmiddle番目からlast - 1番目までの要素をfirst番目の位置に移動する。
* first番目からmiddle - 1番目までにあった要素は後ろにずらされる。
* 事前条件
  * first <= middle <= last <= selfの現在の要素数、であること。
<<< hr

==== Container_merge()
  int Container_merge(Container *self, size_t idx, 
                      Container *x, size_t xidx, size_t xn, 
                      Container *y, size_t yidx, size_t yn, 
                      int (*comp)(const void *p1, const void *p2));
* xのxidx番目からxn個の要素とyのyidx番目からyn個の要素のコピーを比較関数compに従ってマージし、selfのidx番目の位置に挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* selfのidx番目からxn + yn個の要素はソートされた状態になる。
* 事前条件
  * idxがselfの現在の要素数以下の値であること。
  * xidx + xnがxの現在の要素数以下の値であること。
  * yidx + ynがyの現在の要素数以下の値であること。
  * selfとx、selfとyは同じオブジェクトでないこと。
  * xのxidx番目からxn個の要素が比較関数compに従ってソートされていること。
  * yのyidx番目からyn個の要素が比較関数compに従ってソートされていること。
<<< hr

==== Container_inplace_merge()
  void Container_inplace_merge(Container *self, size_t first, size_t middle, size_t last, 
                              int (*comp)(const void *p1, const void *p2));
* selfの連続する2つの範囲first番目からmiddle - 1番目までとmiddle番目からlast - 1番目までの要素を比較関数compに従ってマージする。
* selfのfirst番目からlast - 1番目までの要素はソートされた状態になる。
* 事前条件
  * first <= middle <= last <= selfの現在の要素数、であること。
  * selfのfirst番目からmiddle - 1番目までの要素は比較関数compに従ってソートされていること。
  * selfのmiddle番目からlast - 1番目までの要素は比較関数compに従ってソートされていること。
<<< hr

==== Container_make_heap()
  void Container_make_heap(Container *self, size_t idx, size_t n, int (*comp)(const void *p1, const void *p2));
* selfのidx番目からn個の要素を比較関数compに従ってヒープに変換する。
* 事前条件
  * idx + nがselfの現在の要素数以下の値であること。
<<< hr

==== Container_sort_heap()
  void Container_sort_heap(Container *self, size_t idx, size_t n, int (*comp)(const void *p1, const void *p2));
* selfのidx番目からn個の要素を比較関数compに従ってソートする。
* このソートは安定でない。
* 事前条件
  * idx + nがselfの現在の要素数以下の値であること。
  * selfのidx番目からn個の要素が比較関数compに従ってヒープになっていること。
<<< hr

==== Container_push_heap()
  void Container_push_heap(Container *self, size_t idx, size_t n, int (*comp)(const void *p1, const void *p2));
* selfのidx + n - 1番目の要素を、selfのidx番目からn - 1個の範囲のヒープに追加して、idx番目からn個の要素を一つのヒープとして再構成する。
* 事前条件
  * idx + nがselfの現在の要素数以下の値であること。
  * selfのidx番目からn - 1個の要素が比較関数compに従ってヒープになっていること。
<<< hr

==== Container_pop_heap()
  void Container_pop_heap(Container *self, size_t idx, size_t n, int (*comp)(const void *p1, const void *p2));
* selfのidx番目からn個の範囲のヒープから、ヒープの最初の要素とヒープの最後の要素を交換し、selfのidx番目からn - 1個の要素を一つのヒープとして再構成する。
* 事前条件
  * idx + nがselfの現在の要素数以下の値であること。
  * selfのidx番目からn個の要素が比較関数compに従ってヒープになっていること。
  * nが1以上であること。

=end
