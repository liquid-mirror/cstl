CSTL
====

  CSTLは、C++のSTLライクなC/C++用のコンテナライブラリです。vector, deque,
  list, set, multiset, map, multimap, stringを提供します。


SourceForge.jp CSTL
  http://sourceforge.jp/projects/cstl/


ディレクトリ・ファイル構成
  cstl/
    vector.h      vector
    deque.h       deque
    list.h        list
    rbtree.h      赤黒木(set/mapのデータ構造)
    set.h         set
    map.h         map
    string.h      string
  doc/
    CSTL.rd       CSTLのドキュメント(RDファイル)
    CSTL.html     CSTL.rdをhtml化したもの
    Makefile      CSTL.htmlを生成するためのMakefile
  test/           単体テスト
  sample/         CSTLを使用したサンプル
  heap.h          独自アロケータのヘッダ
  heap.c          独自アロケータのソース
  README.txt      このファイル


インストール
  CSTL.htmlをご覧下さい。


独自アロケータについて
  stdlib.hが使えない環境でmalloc/realloc/freeの代わりに使用することができます。
  詳しくはheap.hの使用例をご覧下さい。


単体テストについて
  gcc3.4.4で動作確認をしています。(stringのみstd::stringを使用してテストしてい
  るのでg++を使用) makeを実行すればビルドからテスト実行までを行います。


ドキュメント生成について
  CSTL.rdからhtml等を生成するにはrubyとRDtoolのインストールが必要です。docディ
  レクトリでmakeを実行すればCSTL.htmlが生成されます。



vim:set ts=8 sts=2 sw=2 tw=78 et ft=memo:
