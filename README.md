# english_notebook_for_engineer

自分がライブラリやフレームワークの公式リファレンスを読む時に英語力の無さを痛感したため建てたプロジェクト  
ITエンジニア特有の単語や高校英語以上レベルの単語を覚えるためのアプリケーションの作成を目指す  
(しれっと中学レベルの単語も入ってるのは復習の為です。嘘です普通に分かりませんでした。)

- [master に push、merge する前に(Merge Policy)](#master-に-pushmerge-する前にmerge-policy)
- [実行方法](#実行方法)
- [ファイル構成](#ファイル構成)
- [実装機能](#実装機能)
- [あったらいい機能](#あったらいい機能)
- [対応中の不具合](#対応中の不具合)
- [問題点](#問題点)
- [開発環境](#開発環境)

## master に push、merge する前に(Merge Policy)

- README.md 記載の実装機能が問題なく動作することを確認すること

## 実行方法

- 実行できる環境
  - os：windowsx64
<br/>

- exeファイルと同ディレクトリにenglish_notebook.jsonがある事を確認すること

1. `launch.bat`の実行（ダブルクリックでも可能）
1. モード選択画面が表示されるため、対応するキーを入力して各モードに移動
1. Escキー押下でプログラムの終了（mainモードでないと有効にならない）

## ファイル構成

- launch.bat : main.cを呼び出すバッチファイル
- english_notebook.json : 英単語をまとめたファイル
- main.c : 各関数を呼び出すメインのCファイル
  - parse_json.c : JSONをパースする関数
  - english_notebook.c : 英単語モード用の関数
  - question.c : 問題回答用モード用の関数（未実装）
- header.h : グローバル変数などシステム共通の設定をまとめたファイル

## 実装機能

- JSONファイルのパース
  - keyとvalueをそれぞれ行数ごとに取得する
- 単語帳モード
  - ランダムに英単語が出るので、キーボード入力で回答と次の英単語が表示される
- 解答モード(仮)
  - 4択を出力するだけ。キー入力で回答の選択肢と次の4択に移動する

## あったらいい機能

- 英検、TOEIC基準で難易度のコースを分ける
  - 進捗率の可視化
- 間違えた単語をまとめる

## 対応中の不具合

- nullポインタ（1行目のJSON）を参照して問題・回答が表示）されないことがある（代わりにnullと表示されている）

## 問題点

- パースする関数がJSONのフォーマット（文字コードや）に依存する

## 開発環境

- Visual Studio 2019/Git
- C
