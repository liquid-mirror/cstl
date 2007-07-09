CSTL
====

  CSTLは、C言語で使えるC++のSTLライクなコンテナライブラリです。vector, deque,
  list, set, multiset, map, multimap, stringを提供します。


SourceForge.jp CSTL
  http://sourceforge.jp/projects/cstl/


ディレクトリ・ファイル構成
  cstl/
    vector.h      vector
    ring.h        リングバッファ
    deque.h       deque
    list.h        list
    rbtree.h      赤黒木
    set.h         set/multiset
    map.h         map/multimap
    string.h      string
    algorithm.h   アルゴリズム
  doc/
    CSTL.rd       CSTLのドキュメント
    CSTL.html     CSTL.rdをhtml化したもの(tarballのみに同梱)
    Makefile      CSTL.htmlを生成するためのMakefile
  test/           単体テスト
  sample/         CSTLを使用したサンプル
  README.txt      このファイル


インストール
  CSTL.htmlをご覧下さい。


単体テストについて
  gcc3.4.4で動作確認をしています。testディレクトリでmakeを実行すればビルドから
  テスト実行までを行います。


ドキュメント生成について
  CSTL.rdからhtml等を生成するにはrubyとRDtoolのインストールが必要です。docディ
  レクトリでmakeを実行すればCSTL.htmlが生成されます。



vim:set ts=8 sts=2 sw=2 tw=78 et ft=memo:
