CSTL
====

  CSTLは、C言語で使えるC++のSTLライクなコンテナライブラリです。vector, deque,
  list, set, multiset, map, multimap, unordered_set, unordered_multiset,
  unordered_map, unordered_multimap, stringを提供します。


SourceForge.JP CSTL
  http://sourceforge.jp/projects/cstl/


ディレクトリ・ファイル構成
  cstl/               CSTLのソースファイル
    vector.h            vector
    ring.h              リングバッファ
    deque.h             deque
    list.h              list
    rbtree.h            赤黒木
    set.h               set/multiset
    map.h               map/multimap
    hashtable.h         ハッシュテーブル
    unordered_set.h     unordered_set/unordered_multiset
    unordered_map.h     unordered_map/unordered_multimap
    string.h            string
    algorithm.h         アルゴリズム
  doc/                CSTLのドキュメント
    html/               ドキュメントをDoxygenでhtml化したもの(tarballのみに同梱)
    Doxyfile            Doxygen用設定ファイル
  test/               単体テスト
  sample/             CSTLを使用したサンプルやSTLと性能を比較したベンチマーク
  README.txt          このファイル


インストール
  cstlディレクトリをインクルードパスの通ったディレクトリにコピーするだけで使用
  可能です。


単体テストについて
  gcc3.4.4で動作確認をしています。testディレクトリでmakeを実行すればビルドから
  テスト実行までを行います。


ドキュメント生成について
  ドキュメントを生成するにはDoxygenのインストールが必要です。docディレクトリで
  doxygenを実行すればhtmlドキュメントが生成されます。



vim:set ts=8 sts=2 sw=2 tw=78 et ft=memo:
