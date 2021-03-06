   __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __  __
  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / Satellite - A Simplified Clock  / / / / / / / / / / / / / / / / / / / / / / / / / /
/_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/ /_/


====== 作成者： 岡田隼飛 ======

###### 概要 ######

  【 名前 】 Satellite - A Simplified Clock
  【 制作者 】 岡田隼飛
  【 開発環境 】 cygwin ver.3.0.7-1 | gcc ver.7.4.0 | GNU make ver.4.2.1
  【 動作環境 】 Windows10 ver.1909 (OS Build : 18363.1198)
  【 バージョン 】 ver.1.2.1
  【 最終更新日 】 2020年 11月 23日（勤労感謝の日）


###### 本ソフトウェアについて ######

  本ソフトウェアは, 後期プログラミング演習第一回の課題「アナログ時計」として作成したものです。

  今回の課題では、「人工衛星」をテーマとして作ってみました。アニメーションで動く線は航行する
  人工衛星を、文字盤は宇宙やSFっぽい近未来感のあるデザインをイメージしながら作成しています。


###### ソフトウェアの実行方法 ######

  ※本ソフトウェアはCygwinから実行しないと起動できません。

  ◆実行方法は、Cygwinにて本体のexeファイルが格納されているフォルダまで移動し、
  $ ./Satellite.exe
  とコマンドを入力して実行します。

  ◆実行ファイルが存在しない場合、本体のcファイルおよびMakefileが格納されているフォルダまで移動し、コマンドで
  $ make
  を実行してください（その際 GNU make が必要になります）。このとき警告が出ますがそのまま実行しても問題ありません。
  （glutの製作者が途中で作るのを放棄した関数にWarningが発生しているそうです）

  ◆ソフトウェアを終了する場合は、ウィンドウ右上のxボタンで終了します。このときCygwin上では未だに実行中の状態となることがあるようなので、
  その場合はCygwin上でCtrl + Cを長押しして強制終了してください。


###### 使い方 ######

  本ソフトウェアを起動すると、スタイリッシュなアニメーションとともにアナログ時計、デジタル時計で現在時刻が表示されます。
  また、右下のボタンをクリックすることでカレンダーを表示することができます。閉じるときはもう一度右下のボタンをクリックします。

  正直もっと機能を追加したかったのですが、自分のプログラミングのスキルとデザインセンスに限界があったため今回はここまでの機能となっています。。。
  というかアニメーションに凝りすぎた。。。


###### 必要動作環境および推奨動作環境 ######

  【 必要動作環境 】
  ・Windows 10以降
  ・cygwin
  ・gcc
  ・GNU make
  ・GLUTライブラリ
  ・glpngライブラリ

  【 推奨動作環境 】
  ・Windows 10以降
  ・cygwin    ver.3.0.7以降
  ・gcc       ver.7.4.0以降
  ・GNU make  ver.4.2.1以降
  ・GLUTライブラリ
  ・glpngライブラリ


###### フォルダおよびファイル構成 ######

  Manifesto.txtを参照


###### ヘルプ ######

  Help.htmlを参照


###### バージョン履歴 ######

  ◇ ver.1.0   ソフトウェアの初版リリース
  ◇ ver.1.1   すべての月でカレンダーに31日まで日付が表示されてしまう問題を修正
  ◇ ver.1.2   入れ忘れていたアイコンとReadme.txtを追加、人工衛星アニメーションの挙動を修正
  ◇ ver.1.2.1 ヘルプページ (Help.html)とフォルダおよびファイル一覧 (Manifesto.png)を追加、Readme.txtの内容を更新


###### 本ソフトウェアのライセンスおよび免責事項 ######

  本ソフトウェアはフリーソフトであり、どなたでも自由にお使いいただけます。

  著作権は、作成者にすべて帰属するものとします。

  本ソフトウェアを個人利用目的以外で改変し再配布したり、本ソフトウェアを売るなどして利益を得ることを固く禁じます。

  頑張って作ったので本ソフトウェアの製作者を偽ったり、本ソフトウェアに対する誹謗中傷をすること等はおやめください。

  本ソフトウェアを使用したことによって生じたすべての損害・不具合に関して、製作者および製作者の所属する組織は一切の責任を負いません．


###### 各ライブラリのライセンス ######

  本ソフトウェアで扱われているライブラリは作成者および配布元のライセンスにすべて従うものとします。

  -- 使用しているライブラリ --
  Silicon Graphics International Corp. (SGI), Mark J. Kilgard - OpenGL Utility Toolkit (GLUT) : https://www.opengl.org/resources/libraries/glut/
  Ben Wyatt - glpng : http://teacher.nagano-nct.ac.jp/ito/Springs_of_C/glpng.zip
                      http://teacher.nagano-nct.ac.jp/ito/Springs_of_C/gppngcygwin.zip （Cygwin用glpngコンパイル済みライブラリ）


###### 連絡先 ######

  Mail : chiramium@gmail.com
