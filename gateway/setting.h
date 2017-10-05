#include<vector>


#ifndef __SETTING_H__
#define __SETTING_H__


/*********PARAMETER****************************************************/
/*
 * RECV_PORT      :センサノードとのsocket通信に用いるポート番号
 * RECV_BUFFSIZE  :センサ毎の過去データの保持数
 * MAX_SENSOR_NUM   :最大のノード数
 *DEFAULT_SAMPLING_CYCLE:デフォルトサンプリング周期，この周期ごとにGWに到着して
 　　　　　　　　　　　　いるデータを集約しクライアントへ送信
 * WS_PORT        :可視化プログラムとのwebsocket通信に用いるポート番号
 * WS_BUFLEN      :可視化プログラムとのwebsocket通信のパケット受信用バッファ
 * MAX_DESCRIPTOR :実行時に使用可能とする最大ディスクリプタ
 * SOCKET_SEND_TIMEOUT :ソケット通信の送信時のタイムアウト時間
 * close_message[]:クライアントが切断時に送信してくるメッセージ(このメッセージを受信したらソケットを閉じる)
 */

/*-----センサノードとの通信関係-----*/
static const int RECV_PORT = 3000;
static const int MAX_SENSOR_NUM = 20;
static const int RECV_BUFLEN = 128;
static const double DEFAULT_SAMPLING_RATE = 60.0; /*デフォルトサンプリングレート*/
#define RECV_BINARY_DATA 1        

/*-----キュー関係----------*/
static const int QUEUE_SIZE = 5;


/*-----可視化プログラム(クライアント)との通信関係-----*/
static const int WS_PORT = 8000;
static const int WS_BUFLEN = 0xFFFF;
static const int MAX_DESCRIPTOR = 100;
static const double SOCKET_SEND_TIMEOUT = 0.1;
static const char close_message[] = "this_is_client_close_message";//クライアントが切断時に送ってくるメッセージ
/**********************************************************************/



#endif /*__SETTING_H__*/
