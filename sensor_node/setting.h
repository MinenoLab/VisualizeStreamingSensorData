/**
 * @file   setting.h
 * @brief  接続先等の設定の定義
 * @author Kazumasa Wakamori
 **/


#ifndef __SETTING_H__
#define __SETTING_H__

/*-----ノード設定-----*/
#define NODEID 0
#define DEFAULT_SAMPLING_RATE 60 /*デフォルトサンプリングレート[Hz]*/
#define OFFSET_COUNT 1000          /*オフセット算出において使用するデータ数*/
#define USE_OFFSET 1               /*0:オフセットを用いない
				     1:起動時にオフセットを算出し適用*/
#define DEBUG_MODE 0               /*0:実センサを用いて動作
				     1:デバックモード(ダミーデータを生成)*/
#define SENDBUFFSIZE 128
#define USE_BINARY  1              /*0:テキスト形式でデータ送信
				     1:バイナリ形式でデータ送信*/

/*-----ゲートウェイ設定-----*/
#define GWIPADDR "192.168.11.100"
//#define GWIPADDR "127.0.0.1"     /*local host*/
#define GWPORT 3000


#endif /*__SETTING_H__*/
