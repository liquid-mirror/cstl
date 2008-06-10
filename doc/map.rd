=begin
== map/multimap
map/multimapを使うには、以下のマクロを用いてコードを展開する必要がある。

* mapの場合
    #include <cstl/map.h>

    /* インターフェイスを展開 */
    #define CSTL_MAP_INTERFACE(Name, KeyType, ValueType)

    /* 実装を展開 */
    #define CSTL_MAP_IMPLEMENT(Name, KeyType, ValueType, Compare)

* multimapの場合
    #include <cstl/map.h>

    /* インターフェイスを展開 */
    #define CSTL_MULTIMAP_INTERFACE(Name, KeyType, ValueType)

    /* 実装を展開 */
    #define CSTL_MULTIMAP_IMPLEMENT(Name, KeyType, ValueType, Compare)

: Name
  既存の型と重複しない任意の名前。コンテナの型名と関数のプレフィックスになる
: KeyType
  任意の要素のキーの型
: ValueType
  任意の要素の値の型
: Compare
  要素を比較する関数またはマクロ
  * KeyTypeが整数型、小数型、ポインタ型など、2つの値を単純に比較できる型の場合、
    要素のソートの順序を昇順にするならばCSTL_LESSマクロを、降順にするならばCSTL_GREATERマクロをCompareに指定する。
    CSTL_LESS/CSTL_GREATERマクロはヘッダで以下のように定義されている。
      #define CSTL_LESS(x, y)     ((x) == (y) ? 0 : (x) < (y) ? -1 : 1)
      #define CSTL_GREATER(x, y)  ((x) == (y) ? 0 : (x) > (y) ? -1 : 1)
  * KeyTypeがその他の型の場合、以下の関数のような引数と戻り値を持ち、
    x == yならば0を、x < yならば正または負の整数を、x > yならばx < yの場合と逆の符号の整数を
    返す比較関数またはマクロをCompareに指定する。
    尚、KeyTypeが文字列型(char*)ならば、C標準関数のstrcmpが指定可能である。
      int comp(KeyType x, KeyType y);

<<< br

NameにMap, KeyTypeにKeyT, ValueTypeにValueTを指定した場合、
以下のインターフェイスを提供する。

* 型
  * ((<Map>))
  * ((<MapIterator>))
* 関数
  * 生成
    * ((<Map_new()>))
  * 破棄
    * ((<Map_delete()>))
  * サイズ
    * ((<Map_size()>))
    * ((<Map_empty()>))
  * イテレータ
    * ((<Map_begin()>)) , ((<Map_end()>))
    * ((<Map_rbegin()>)) , ((<Map_rend()>))
    * ((<Map_next()>)) , ((<Map_prev()>))
  * 要素のアクセス
    * ((<Map_key()>)) , ((<Map_value()>))
    * ((<Map_lookup() map用>))
  * 挿入
    * ((<Map_insert() map用>)) , ((<Map_insert() multimap用>)) , ((<Map_insert_range()>))
  * 削除
    * ((<Map_erase()>)) , ((<Map_erase_range()>)) , ((<Map_erase_key()>))
    * ((<Map_clear()>))
  * 交換
    * ((<Map_swap()>))
  * 検索
    * ((<Map_count()>))
    * ((<Map_find()>))
    * ((<Map_lower_bound()>)) , ((<Map_upper_bound()>))
<<< hr

==== Map
  Map
コンテナの型。抽象データ型となっており、以下の関数によってのみアクセスできる。
<<< hr

==== MapIterator
  MapIterator
イテレータの型。要素の位置を示す。
関数から返されたイテレータを有効なイテレータという。
宣言されただけのイテレータ、または削除された要素のイテレータを無効なイテレータという。
<<< hr

==== Map_new()
  Map *Map_new(void);
* map/multimapを生成する。
* 生成に成功した場合、そのオブジェクトへのポインタを返す。
* メモリ不足の場合、NULLを返す。
<<< hr

==== Map_delete()
  void Map_delete(Map *self);
* selfのすべての要素を削除し、selfを破棄する。
* selfがNULLの場合、何もしない。
<<< hr

==== Map_size()
  size_t Map_size(Map *self);
* selfの現在の要素数を返す。
<<< hr

==== Map_empty()
  int Map_empty(Map *self);
* selfが空の場合、0以外の値を返す。
* selfが空でない場合、0を返す。
<<< hr

==== Map_begin()
  MapIterator Map_begin(Map *self);
* selfの最初の要素のイテレータを返す。
<<< hr

==== Map_end()
  MapIterator Map_end(Map *self);
* selfの最後の要素の次のイテレータを返す。
<<< hr

==== Map_rbegin()
  MapIterator Map_rbegin(Map *self);
* selfの最後の要素のイテレータを返す。
<<< hr

==== Map_rend()
  MapIterator Map_rend(Map *self);
* selfの最初の要素の前のイテレータを返す。
<<< hr

==== Map_next()
  MapIterator Map_next(MapIterator pos);
* posが示す位置の要素の次のイテレータを返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがMap_end()またはMap_rend()でないこと。
<<< hr

==== Map_prev()
  MapIterator Map_prev(MapIterator pos);
* posが示す位置の要素の前のイテレータを返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがMap_end()またはMap_rend()でないこと。
<<< hr

==== Map_key()
  KeyT Map_key(MapIterator pos);
* posが示す位置の要素のキーを返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがMap_end()またはMap_rend()でないこと。
<<< hr

==== Map_value()
  ValueT *Map_value(MapIterator pos);
* posが示す位置の要素の値へのポインタを返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがMap_end()またはMap_rend()でないこと。
<<< hr

==== Map_lookup() map用
  ValueT *Map_lookup(Map *self, KeyT key);
* selfのkeyというキーの要素の値へのポインタを返す。
* selfがkeyというキーの要素を持っていない場合、keyというキーの新しい要素(値は不定)を挿入し、その要素の値へのポインタを返す。
* メモリ不足の場合、NDEBUGマクロが未定義ならばアサーションに失敗し、定義済みならばselfの変更を行わずNULLを返す。
* この関数はmapのみで提供される。
<<< hr

==== Map_insert() map用
  MapIterator Map_insert(Map *self, KeyT key, ValueT value, int *success);
* keyとvalueのコピーのペアを要素としてselfに挿入する。
* 挿入に成功した場合、*successに0以外の値を格納し、新しい要素のイテレータを返す。
* selfが既にkeyというキーの要素を持っている場合、挿入を行わず、*successに0を格納し、その要素のイテレータを返す。
* メモリ不足の場合、*successに0を格納し、selfの変更を行わず0を返す。
* successにNULLを指定した場合、*successにアクセスしない。
* この関数はmapのみで提供される。
<<< hr

==== Map_insert() multimap用
  MapIterator Map_insert(Map *self, KeyT key, ValueT value);
* keyとvalueのコピーのペアを要素としてselfに挿入する。
* 挿入に成功した場合、新しい要素のイテレータを返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* selfが既にkeyというキーの要素を持っている場合、そのキーの一番最後の位置に挿入される。
* この関数はmultimapのみで提供される。
<<< hr

==== Map_insert_range()
  int Map_insert_range(Map *self, MapIterator first, MapIterator last);
* [first, last)の範囲の要素のコピーをselfに挿入する。
* multimapの場合、[first, last)の要素はselfが持つ要素でもよい。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * [first, last)が有効なイテレータであること。
<<< hr

==== Map_erase()
  MapIterator Map_erase(Map *self, MapIterator pos);
* selfのposが示す位置の要素を削除し、その次のイテレータを返す。
* 事前条件
  * posがselfの有効なイテレータであること。
  * posがMap_end()またはMap_rend()でないこと。
<<< hr

==== Map_erase_range()
  MapIterator Map_erase_range(Map *self, MapIterator first, MapIterator last);
* selfの[first, last)の範囲の要素を削除し、削除した要素の次のイテレータを返す。
* 事前条件
  * [first, last)がselfの有効なイテレータであること。
<<< hr

==== Map_erase_key()
  size_t Map_erase_key(Map *self, KeyT key);
* selfのkeyというキーの要素をすべて削除し、削除した数を返す。
<<< hr

==== Map_clear()
  void Map_clear(Map *self);
* selfのすべての要素を削除する。
<<< hr

==== Map_swap()
  void Map_swap(Map *self, Map *x);
* selfとxの内容を交換する。
<<< hr

==== Map_count()
  size_t Map_count(Map *self, KeyT key);
* selfのkeyというキーの要素の数を返す。
<<< hr

==== Map_find()
  MapIterator Map_find(Map *self, KeyT key);
* selfのkeyというキーの要素を検索し、最初に見つかった要素のイテレータを返す。
* 見つからなければMap_end(self)を返す。
<<< hr

==== Map_lower_bound()
  MapIterator Map_lower_bound(Map *self, KeyT key);
* ソートの基準に従い、selfのkey以上のキーの最初の要素のイテレータを返す。
* 見つからなければMap_end(self)を返す。
<<< hr

==== Map_upper_bound()
  MapIterator Map_upper_bound(Map *self, KeyT key);
* ソートの基準に従い、selfのkeyより大きいキーの最初の要素のイテレータを返す。
* 見つからなければMap_end(self)を返す。
<<< hr

=end
