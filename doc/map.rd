=begin
== map/multimap
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
関数から返されたイテレータを有効なイテレータという。
宣言されただけのイテレータ、または削除された要素のイテレータを無効なイテレータという。

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
* selfの最初の要素のイテレータを返す。
<<< br

  MapIterator Map_end(Map *self);
* selfの最後の要素の次のイテレータを返す。
<<< br

  MapIterator Map_rbegin(Map *self);
* selfの最後の要素のイテレータを返す。
<<< br

  MapIterator Map_rend(Map *self);
* selfの最初の要素の前のイテレータを返す。
<<< br

  MapIterator Map_next(MapIterator pos);
* posが示す位置の要素の次のイテレータを返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがMap_end()またはMap_rend()でないこと。
<<< br

  MapIterator Map_prev(MapIterator pos);
* posが示す位置の要素の前のイテレータを返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがMap_end()またはMap_rend()でないこと。
<<< br

+ 要素のアクセス
  KeyT Map_key(MapIterator pos);
* posが示す位置の要素のキーを返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがMap_end()またはMap_rend()でないこと。
<<< br

  ValueT *Map_value(MapIterator pos);
* posが示す位置の要素の値へのポインタを返す。
* 事前条件
  * posが有効なイテレータであること。
  * posがMap_end()またはMap_rend()でないこと。
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
* 挿入に成功した場合、*successに0以外の値を格納し、新しい要素のイテレータを返す。
* selfが既にkeyというキーの要素を持っている場合、挿入を行わず、*successに0を格納し、その要素のイテレータを返す。
* メモリ不足の場合、*successに0を格納し、selfの変更を行わず0を返す。
* successにNULLを指定した場合、*successにアクセスしない。
* この関数はmapのみで提供される。
<<< br

  MapIterator Map_insert(Map *self, KeyT key, ValueT value);
* keyとvalueのコピーのペアを要素としてselfに挿入する。
* 挿入に成功した場合、新しい要素のイテレータを返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* selfが既にkeyというキーの要素を持っている場合、そのキーの一番最後の位置に挿入される。
* この関数はmultimapのみで提供される。
<<< br

  int Map_insert_range(Map *self, MapIterator first, MapIterator last);
* [first, last)の範囲の要素のコピーをselfに挿入する。
* multimapの場合、[first, last)の要素はselfが持つ要素でもよい。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * [first, last)が有効なイテレータであること。
<<< br

+ 削除
  MapIterator Map_erase(Map *self, MapIterator pos);
* selfのposが示す位置の要素を削除し、その次のイテレータを返す。
* 事前条件
  * posがselfの有効なイテレータであること。
  * posがMap_end()またはMap_rend()でないこと。
<<< br

  MapIterator Map_erase_range(Map *self, MapIterator first, MapIterator last);
* selfの[first, last)の範囲の要素を削除し、削除した要素の次のイテレータを返す。
* 事前条件
  * [first, last)がselfの有効なイテレータであること。
<<< br

  size_t Map_erase_key(Map *self, KeyT key);
* selfのkeyというキーの要素をすべて削除し、削除した数を返す。
<<< br

  void Map_clear(Map *self);
* selfのすべての要素を削除する。
<<< br

+ 交換
  void Map_swap(Map *self, Map *x);
* selfとxの内容を交換する。
<<< br

+ 検索
  size_t Map_count(Map *self, KeyT key);
* selfのkeyというキーの要素の数を返す。
<<< br

  MapIterator Map_find(Map *self, KeyT key);
* selfのkeyというキーの要素を検索し、最初に見つかった要素のイテレータを返す。
* 見つからなければMap_end(self)を返す。
<<< br

  MapIterator Map_lower_bound(Map *self, KeyT key);
* ソートの基準に従い、selfのkey以上のキーの最初の要素のイテレータを返す。
* 見つからなければMap_end(self)を返す。
<<< br

  MapIterator Map_upper_bound(Map *self, KeyT key);
* ソートの基準に従い、selfのkeyより大きいキーの最初の要素のイテレータを返す。
* 見つからなければMap_end(self)を返す。
<<< br

=end
