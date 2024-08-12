# stm32f0-iolink-master

このリポジトリは、C104で頒布するIO-Linkマスタ基板向けファームウェアです。

以下の特徴があります:

- IO-Link v1.1仕様を部分的に実装しています
- Direct Parameter、Process Dataのやり取りに対応しています
- On-request Data(ISDU、Event等)はサポートしていません
  - ハードウェア的には対応しているので、ご自身で実装していただくことは可能です
  - 余裕があれば今後このリポジトリでサポートしたい…
- IODDファイル(マスタに接続するセンサ・アクチュエータのデバイス定義ファイル)に読み込みには対応していません
  - Core/Inc/device ディレクトリ以下にデバイスの仕様をC++クラスとして定義する必要があります

## 使用方法

- このリポジトリをcloneし、STM32CubeIDEでプロジェクトとして開く
- .ioc ファイルを開き、スレーブ側デバイスの通信速度にUARTのbaudrateを合わせる
  - UART2: IOLINK1
  - UART3: IOLINK2
  - UART4: IOLINK3
  - UART6: IOLINK4
- デバイス定義クラスを Core/Inc/device 以下に定義する(後述)
- Core/Src/iolink_main.cpp 内にある iolink_main 関数を編集する
  - MasterCycleTimeの定義
  - デバイス定義クラスの実体化
  - デバイスの初期化
  - Preoperateモードへの移行
  - Operateモードへの移行
  - Process Dataのやり取り
  - UARTやCANへの出力(デフォルトでUARTに対して文字列を出力するようになっています)
  - …など
- ビルド、STLINK等を介して書き込み

## デバイス定義クラスの作り方

- iolink/iolink_port.hpp をincludeする
- iolink::IOLinkPort を継承したクラスを定義
- 関数 pre_operate, operate, pd_read, pd_reply をオーバーライドする
  - Wakeup requestの送信方法など、デバイス定義に関する処理は lr_tb2000.hpp (キーエンス社製レーザー測距計のデバイス定義クラス)を参考にしてみてください

# create_mseqでm-sequenceをつくる

執筆中、これも lr_tb2000.hpp を参考にすればうまく使えるはず…
