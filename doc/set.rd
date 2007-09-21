=begin
== set/multiset
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
関数から返されたイテレータを有効なイテレータという。
宣言されただけのイテレータ、または削除された要素のイテレータを無効なイテレータという。

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
* selfの最初の要素のイテレータを返す。
<<< br

  SetIterator Set_end(Set *self);
* selfの最後の要素の次のイテレータを返す。
<<< br

  SetIterator Set_rbegin(Set *self);
* selfの最後の要素のイテレータを返す。
<<< br

  SetIterator Set_rend(Set *self);
* selfの最初の要素の前のイテレータを返す。
<<< br

  SetIterator Set_next(SetIterator pos);
* posが示す位置の要素の次のイテレータを返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがSet_end()またはSet_rend()でないこと。
<<< br

  SetIterator Set_prev(SetIterator pos);
* posが示す位置の要素の前のイテレータを返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがSet_end()またはSet_rend()でないこと。
<<< br

+ 要素のアクセス
  T Set_key(SetIterator pos);
* posが示す位置の要素を返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがSet_end()またはSet_rend()でないこと。
<<< br

+ 挿入
  SetIterator Set_insert(Set *self, T elem, int *success);
* elemのコピーをselfに挿入する。
* 挿入に成功した場合、*successに0以外の値を格納し、新しい要素のイテレータを返す。
* selfが既にelemという要素を持っている場合、挿入を行わず、*successに0を格納し、その要素のイテレータを返す。
* メモリ不足の場合、*successに0を格納し、selfの変更を行わず0を返す。
* successにNULLを指定した場合、*successにアクセスしない。
* この関数はsetのみで提供される。
<<< br

  SetIterator Set_insert(Set *self, T elem);
* elemのコピーをselfに挿入する。
* 挿入に成功した場合、新しい要素のイテレータを返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* selfが既にelemという要素を持っている場合、その値の一番最後の位置に挿入される。
* この関数はmultisetのみで提供される。
<<< br

  int Set_insert_range(Set *self, SetIterator first, SetIterator last);
* [first, last)の範囲の要素のコピーをselfに挿入する。
* multisetの場合、[first, last)の要素はselfが持つ要素でもよい。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * [first, last)が有効なイテレータであること。
<<< br

+ 削除
  SetIterator Set_erase(Set *self, SetIterator pos);
* selfのposが示す位置の要素を削除し、その次のイテレータを返す。
* 事前条件
  * posがselfの有効なイテレータであること。
  * posがSet_end()またはSet_rend()でないこと。
<<< br

  SetIterator Set_erase_range(Set *self, SetIterator first, SetIterator last);
* selfの[first, last)の範囲の要素を削除し、削除した要素の次のイテレータを返す。
* 事前条件
  * [first, last)がselfの有効なイテレータであること。
<<< br

  size_t Set_erase_key(Set *self, T elem);
* selfのelemという要素をすべて削除し、削除した数を返す。
<<< br

  void Set_clear(Set *self);
* selfのすべての要素を削除する。
<<< br

+ 交換
  void Set_swap(Set *self, Set *x);
* selfとxの内容を交換する。
<<< br

+ 検索
  size_t Set_count(Set *self, T elem);
* selfのelemという要素の数を返す。
<<< br

  SetIterator Set_find(Set *self, T elem);
* selfのelemという要素を検索し、最初に見つかった要素のイテレータを返す。
* 見つからなければSet_end(self)を返す。
<<< br

  SetIterator Set_lower_bound(Set *self, T elem);
* ソートの基準に従い、selfのelem以上の最初の要素のイテレータを返す。
* 見つからなければSet_end(self)を返す。
<<< br

  SetIterator Set_upper_bound(Set *self, T elem);
* ソートの基準に従い、selfのelemより大きい最初の要素のイテレータを返す。
* 見つからなければSet_end(self)を返す。
<<< br

=end
