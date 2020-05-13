# english_notebook_for_engineer

自分がライブラリやフレームワークの公式リファレンスを読む時に英語力の無さを痛感したため建てたプロジェクト  
ITエンジニア特有の単語や高校英語以上レベルの単語を覚えるためのアプリケーションの作成を目指す  
(しれっと中学レベルの単語も入ってるのは復習の為です。嘘です普通に分かりませんでした。)

- [english_notebook_for_engineer](#englishnotebookforengineer)
  - [master に push、merge する前に(Merge Policy)](#master-%e3%81%ab-pushmerge-%e3%81%99%e3%82%8b%e5%89%8d%e3%81%abmerge-policy)
  - [実行方法](#%e5%ae%9f%e8%a1%8c%e6%96%b9%e6%b3%95)
  - [ファイル構成](#%e3%83%95%e3%82%a1%e3%82%a4%e3%83%ab%e6%a7%8b%e6%88%90)
  - [実装機能](#%e5%ae%9f%e8%a3%85%e6%a9%9f%e8%83%bd)
  - [あったらいい機能](#%e3%81%82%e3%81%a3%e3%81%9f%e3%82%89%e3%81%84%e3%81%84%e6%a9%9f%e8%83%bd)
  - [対応中の不具合](#%e5%af%be%e5%bf%9c%e4%b8%ad%e3%81%ae%e4%b8%8d%e5%85%b7%e5%90%88)
  - [問題点](#%e5%95%8f%e9%a1%8c%e7%82%b9)
  - [開発環境](#%e9%96%8b%e7%99%ba%e7%92%b0%e5%a2%83)

## master に push、merge する前に(Merge Policy)

- README.md 記載の実装機能が問題なく動作することを確認すること

## 実行方法

1. `gcc parse_json.c -o [arbitrary file name]`
2. `[arbitrary file name].exe`を実行

## ファイル構成

- english_notebook.json : 英単語をまとめたファイル
- parse_json.c : JSONをパースするためのCファイル

## 実装機能

- JSONファイルのパース
  - 現段階では行数を読み込み、読み込んだ文字のアドレス情報等にCLI上に出力する

## あったらいい機能

- 英検、TOEIC基準で難易度のコースを分ける
  - 進捗率の可視化
- 間違えた単語をまとめる

## 対応中の不具合

- 環境によってはパースされたマルチバイト文字が文字化けする

## 問題点

## 開発環境

- VSCode/Git
- C(gcc)
