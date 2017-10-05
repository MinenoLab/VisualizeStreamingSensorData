## Sensor node program 

## 1.動作環境
* シングルボードコンピュータ ： Raspberry Pi2
* 加速度センサ：ADXL345
* OS：Linux(raspbian-jessie-lite)
* Cコンパイラ：gcc (Raspbian 4.9.2-10) 4.9.2

## 2.設定
センサノード設定

  | 設定項目 | 内容 |
|--------|--------|
|	  NODEID		|				センサノードに割り当てる固有ID		|
|	  DEFAULT_SAMPLING_RATE 		|		デフォルトサンプリングレート[Hz]				|
|	  OFFSET_COUNT  		|			オフセット算出の際に取得するデータ数 <br> プログラム起動後OFFSET_COUNTだけ加速度データを取得し、その平均をオフセットとして用いる			|
|	  USE_OFFSET		|				オフセットの使用の有無を指定 <br> 0:オフセットを使用しない <br> 1:起動時にオフセットを算出し適用		|
|	  DEBUG_MODE 		|				動作モードの指定 <br> 0:通常モード(センサから値を取得して送信) <br> 1:デバックモード(ダミーデータを送信)		|
|	  SENDBUFFSIZE 		|				送信データを格納するバッファサイズ		|
|	  USE_BINARY		|				送信時のデータ形式 <br>  0:テキスト形式 "id,valueX,valueY,valueZ" <br>  1:バイナリ形式 sensor_io_handle.h内BinaryDataFormat型		|
  
  
ゲートウェイ設定

| 設定項目 | 内容 |
|--------|--------|
|  GWIPADDR		|	ゲートウェイのIPアドレス|
|  GWPORT		|	ゲートウェイとの通信に用いるポート番号|


## 3.実行方法
### 3.1.コンパイル
* Makefileを用いてコンパイルを行う
```
   $ make clean
   $ make
```

### 3.2. 実行ファイル(nodemain)を実行
* コマンドライン引数にはサンプリングレート[Hz]を与える
```
   #サンプリングレート = 60Hz として実行
   $ ./nodemain 60 
```

* コマンド引数を与えない場合はDEFAULT_SAMPLING_RATEをサンプリングレートとして動作
```
   $ ./nodemain
   ```

