=begin
= CSTL

== 目次
* ((<CSTLとは>))
* ((<インストール>))
* ((<リファレンスマニュアル>))
  * ((<vector|URL:vector.html>))
  * ((<deque|URL:deque.html>))
  * ((<list|URL:list.html>))
  * ((<set, multiset|URL:set.html>))
  * ((<map, multimap|URL:map.html>))
  * ((<string|URL:string.html>))
  * ((<algorithm|URL:algorithm.html>))
* ((<STLとの主な違い>))
* ((<ライセンス>))
  * ((<日本語訳(参考)>))


== CSTLとは
CSTLは、C++のSTLを模倣したC言語用のコンテナライブラリである。
CSTLは以下の特長を持つ。
* STLと同様なデータ構造・アルゴリズムを提供。
* STLに似たインターフェイス。
* 任意の型の要素に対応。
* 大抵のC89準拠の処理系に移植可能。


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

ヘッダファイルのみなのでビルドする必要はない。


== リファレンスマニュアル
CSTLは以下のコンテナを提供する。

: ((<vector|URL:vector.html>))
  可変長配列。末尾での要素の挿入・削除が高速。インデックスによる要素のランダムアクセスが可能。

: ((<deque|URL:deque.html>))
  可変長両端キュー。先頭と末尾での要素の挿入・削除が高速。インデックスによる要素のランダムアクセスが可能。

: ((<list|URL:list.html>))
  双方向リンクリスト。任意の位置での要素の挿入・削除が高速だが、要素のランダムアクセスはできない。

: ((<set|URL:set.html>)) , ((<multiset|URL:set.html>))
  要素が値によって自動的にソートされるコンテナ。multisetは要素の重複を許す。

: ((<map|URL:map.html>)) , ((<multimap|URL:map.html>))
  キーと値のペアを要素とするコンテナ。要素はキーによって自動的にソートされる。multimapは要素のキーの重複を許す。

: ((<string|URL:string.html>))
  可変長文字列。

: ((<algorithm|URL:algorithm.html>))
  vector, deque, stringに共通なアルゴリズム。


== STLとの主な違い
* オブジェクトの生成/破棄は、必ずnew/deleteに相当する関数でしなければならない。
* 要素の挿入/削除時、自動的にコンストラクタ/デストラクタに相当する関数が呼ばれることはない。
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

  Copyright (c) 2006-2007, KATO Noriaki
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

  Copyright (c) 2006-2007, KATO Noriaki
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
