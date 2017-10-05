## Gateway program

### 1.動作環境
* 対応OS：Linux(Ubuntu 14.04 LTS 64bit)
* Cコンパイラ：gcc (Ubuntu 4.8.2-19ubuntu1) 4.8.2
* C\+\+コンパイラ： g++ (Ubuntu 4.8.2-19ubuntu1) 4.8.2


### 2.設定
* setting.h内の以下の設定項目を用途に合わせ変更してください。  
    Sensor nodeとの通信関係  
    
    | 設定項目 | 内容 |
    |--------|--------|
    |	  RECV_PORT	|	センサノードとのsocket通信に用いるポート番号	|
    |	  MAX_SENSOR_NUM	|	使用する最大のセンサノード数	|
    |	  RECV_BUFLEN	|	データ受信バッファサイズ	|
    |	  DEFAULT_SAMPLING_RATE	|	デフォルトサンプリングレート（このレートでGWに到着しているデータを集約しクライアントへ送信）	|
    |	  QUEUE_SIZE	|	センサデータを格納するキューサイズ（キューはセンサノード毎に用意され、各キューのサイズがQUEUE_SIZEとなる）	|
    |	  RECV_BINARY_DATA	|	ノードから送信されるデータ形式を指定（0:テキスト形式, 1:バイナリ形式）	|

    Clientとの通信関係

    | 設定項目 | 内容 |
    |--------|--------|
    |	  WS_PORT	|	可視化プログラムとのWebSocket通信に用いるポート番号	|
    |	  WS_BUFLEN	|	可視化プログラムとの通信におけるフレーム受信バッファサイズ	|
    |	  MAX_DESCRIPTOR	|	実行時に使用可能とする最大のディスクリプタ	|
    |	  SOCKET_SEND_TIMEOUT	|	WebSocket通信での送信時のタイムアウト時間	|
    |	  close_message	|	クライアントが切断時に送信するメッセージ	|


## 3.実行方法
#### 3.1.コンパイル
* Makefileを用いてコンパイルを行う
```
   $ make clean
   $ make
```
 
#### 3.2. 実行ファイル(gatewaymain)を実行
* コマンドライン引数にはサンプリングレート[Hz]を与える
```
   # サンプリングレート = 60Hz として実行
   $ ./gatewaymain 160
```

* コマンド引数を与えない場合はDEFAULT_SAMPLING_RATEをサンプリングレートとして動作
```
   $ ./gatewaymain
```
