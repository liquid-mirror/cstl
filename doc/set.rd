=begin
== set/multiset
set/multisetを使うには、 以下のマクロを用いてコードを展開する必要がある。

* setの場合
    #include <cstl/set.h>

    /* インターフェイスを展開 */
    #define CSTL_SET_INTERFACE(Name, Type)

    /* 実装を展開 */
    #define CSTL_SET_IMPLEMENT(Name, Type, Compare)

* multisetの場合
    #include <cstl/set.h>

    /* インターフェイスを展開 */
    #define CSTL_MULTISET_INTERFACE(Name, Type)

    /* 実装を展開 */
    #define CSTL_MULTISET_IMPLEMENT(Name, Type, Compare)

: Name
  既存の型と重複しない任意の名前。コンテナの型名と関数のプレフィックスになる
: Type
  任意の要素の型
: Compare
  要素を比較する関数またはマクロ
  * Typeが整数型、小数型、ポインタ型など、2つの値を単純に比較できる型の場合、
    要素のソートの順序を昇順にするならばCSTL_LESSマクロを、降順にするならばCSTL_GREATERマクロをCompareに指定する。
    CSTL_LESS/CSTL_GREATERマクロはヘッダで以下のように定義されている。
      #define CSTL_LESS(x, y)     ((x) == (y) ? 0 : (x) < (y) ? -1 : 1)
      #define CSTL_GREATER(x, y)  ((x) == (y) ? 0 : (x) > (y) ? -1 : 1)
  * Typeがその他の型の場合、以下の関数のような引数と戻り値を持ち、
    x == yならば0を、x < yならば正または負の整数を、x > yならばx < yの場合と逆の符号の整数を
    返す比較関数またはマクロをCompareに指定する。
    尚、Typeが文字列型(char*)ならば、C標準関数のstrcmpが指定可能である。
      int comp(Type x, Type y);

<<< br

NameにSet, TypeにTを指定した場合、
以下のインターフェイスを提供する。

* 型
  * ((<Set>))
  * ((<SetIterator>))
* 関数
  * 生成
    * ((<Set_new()>))
  * 破棄
    * ((<Set_delete()>))
  * サイズ
    * ((<Set_size()>))
    * ((<Set_empty()>))
  * イテレータ
    * ((<Set_begin()>)) , ((<Set_end()>))
    * ((<Set_rbegin()>)) , ((<Set_rend()>))
    * ((<Set_next()>)) , ((<Set_prev()>))
  * 要素のアクセス
    * ((<Set_key()>))
  * 挿入
    * ((<Set_insert() set用>)) , ((<Set_insert() multiset用>)) , ((<Set_insert_range()>))
  * 削除
    * ((<Set_erase()>)) , ((<Set_erase_range()>)) , ((<Set_erase_key()>))
    * ((<Set_clear()>))
  * 交換
    * ((<Set_swap()>))
  * 検索
    * ((<Set_count()>))
    * ((<Set_find()>))
    * ((<Set_lower_bound()>)) , ((<Set_upper_bound()>))
<<< hr

==== Set
  Set
コンテナの型。抽象データ型となっており、以下の関数によってのみアクセスできる。
<<< hr

==== SetIterator
  SetIterator
イテレータの型。要素の位置を示す。
関数から返されたイテレータを有効なイテレータという。
宣言されただけのイテレータ、または削除された要素のイテレータを無効なイテレータという。
<<< hr

==== Set_new()
  Set *Set_new(void);
* set/multisetを生成する。
* 生成に成功した場合、そのオブジェクトへのポインタを返す。
* メモリ不足の場合、NULLを返す。
<<< hr

==== Set_delete()
  void Set_delete(Set *self);
* selfのすべての要素を削除し、selfを破棄する。
* selfがNULLの場合、何もしない。
<<< hr

==== Set_size()
  size_t Set_size(Set *self);
* selfの現在の要素数を返す。
<<< hr

==== Set_empty()
  int Set_empty(Set *self);
* selfが空の場合、0以外の値を返す。
* selfが空でない場合、0を返す。
<<< hr

==== Set_begin()
  SetIterator Set_begin(Set *self);
* selfの最初の要素のイテレータを返す。
<<< hr

==== Set_end()
  SetIterator Set_end(Set *self);
* selfの最後の要素の次のイテレータを返す。
<<< hr

==== Set_rbegin()
  SetIterator Set_rbegin(Set *self);
* selfの最後の要素のイテレータを返す。
<<< hr

==== Set_rend()
  SetIterator Set_rend(Set *self);
* selfの最初の要素の前のイテレータを返す。
<<< hr

==== Set_next()
  SetIterator Set_next(SetIterator pos);
* posが示す位置の要素の次のイテレータを返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがSet_end()またはSet_rend()でないこと。
<<< hr

==== Set_prev()
  SetIterator Set_prev(SetIterator pos);
* posが示す位置の要素の前のイテレータを返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがSet_end()またはSet_rend()でないこと。
<<< hr

==== Set_key()
  T Set_key(SetIterator pos);
* posが示す位置の要素を返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがSet_end()またはSet_rend()でないこと。
<<< hr

==== Set_insert() set用
  SetIterator Set_insert(Set *self, T elem, int *success);
* elemのコピーをselfに挿入する。
* 挿入に成功した場合、*successに0以外の値を格納し、新しい要素のイテレータを返す。
* selfが既にelemという要素を持っている場合、挿入を行わず、*successに0を格納し、その要素のイテレータを返す。
* メモリ不足の場合、*successに0を格納し、selfの変更を行わず0を返す。
* successにNULLを指定した場合、*successにアクセスしない。
* この関数はsetのみで提供される。
<<< hr

==== Set_insert() multiset用
  SetIterator Set_insert(Set *self, T elem);
* elemのコピーをselfに挿入する。
* 挿入に成功した場合、新しい要素のイテレータを返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* selfが既にelemという要素を持っている場合、その値の一番最後の位置に挿入される。
* この関数はmultisetのみで提供される。
<<< hr

==== Set_insert_range()
  int Set_insert_range(Set *self, SetIterator first, SetIterator last);
* [first, last)の範囲の要素のコピーをselfに挿入する。
* multisetの場合、[first, last)の要素はselfが持つ要素でもよい。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * [first, last)が有効なイテレータであること。
<<< hr

==== Set_erase()
  SetIterator Set_erase(Set *self, SetIterator pos);
* selfのposが示す位置の要素を削除し、その次のイテレータを返す。
* 事前条件
  * posがselfの有効なイテレータであること。
  * posがSet_end()またはSet_rend()でないこと。
<<< hr

==== Set_erase_range()
  SetIterator Set_erase_range(Set *self, SetIterator first, SetIterator last);
* selfの[first, last)の範囲の要素を削除し、削除した要素の次のイテレータを返す。
* 事前条件
  * [first, last)がselfの有効なイテレータであること。
<<< hr

==== Set_erase_key()
  size_t Set_erase_key(Set *self, T elem);
* selfのelemという要素をすべて削除し、削除した数を返す。
<<< hr

==== Set_clear()
  void Set_clear(Set *self);
* selfのすべての要素を削除する。
<<< hr

==== Set_swap()
  void Set_swap(Set *self, Set *x);
* selfとxの内容を交換する。
<<< hr

==== Set_count()
  size_t Set_count(Set *self, T elem);
* selfのelemという要素の数を返す。
<<< hr

==== Set_find()
  SetIterator Set_find(Set *self, T elem);
* selfのelemという要素を検索し、最初に見つかった要素のイテレータを返す。
* 見つからなければSet_end(self)を返す。
<<< hr

==== Set_lower_bound()
  SetIterator Set_lower_bound(Set *self, T elem);
* ソートの基準に従い、selfのelem以上の最初の要素のイテレータを返す。
* 見つからなければSet_end(self)を返す。
<<< hr

==== Set_upper_bound()
  SetIterator Set_upper_bound(Set *self, T elem);
* ソートの基準に従い、selfのelemより大きい最初の要素のイテレータを返す。
* 見つからなければSet_end(self)を返す。
<<< hr

=end
