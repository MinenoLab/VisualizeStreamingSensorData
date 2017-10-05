#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>

/*ソケット関連*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>

/*スレッド関連*/
#include <pthread.h>

/* 割り込み関連 */
#include <signal.h>

/*自作ヘッダ*/
#include "setting.h"
#include "mygettimeofday.h"
#include "udp_setup.h"
#include "sensor_io_handle.h"

/*関数プロトタイプ宣言*/
void sigint_handler(int sig);

/*グローバル変数*/
double sampling_cycle = 1.0/DEFAULT_SAMPLING_RATE * 1000; //sampling cycle is msec

int main(int argc, char *argv[]){
  int sock;
  struct sockaddr_in addr;
  int send_ret; //sendto()の返り値格納用
  int msd_ret;  //make_send_data()の返り値格納用
  double sampling_rate = DEFAULT_SAMPLING_RATE;
  long long loop_start_time;//データ送信ループの時間計計測
  int loop_elapsed_time;////データ送信ループの時間計計測
  
  printf("nodemain START!!\n");
  
#if USE_BINARY == 0
  char sendbuff[SENDBUFFSIZE];//送信データ用バッファ(テキスト形式)
#else
  BinaryDataFormat sendbuff;//送信データ用バッファ(バイナリ形式)
#endif
  /*
  if(argc > 2 ){//コマンドライン引数が異常
    printf("usage: %s [sampling rate]\n",argv[0]);
    exit(1);
  }
  */

  if(argc >= 2){//コマンドライン引数として与えられた値がサンプリングレートとして不適切
    if((sampling_rate = atof(argv[1])) == 0){
      printf("Do not use \"%s\" for sampling rate\n",argv[1]);
      exit(1);
    }
    sampling_cycle = (1.0/sampling_rate) * 1000;//sampling cycle is [ms]
  }
  printf("Set sampling rate:%0.3lf[Hz]\n",sampling_rate);
  printf("Set sampling cycle:%0.3lf[msec]\n", sampling_cycle);
  
  if(open_sensor_io() < 0){//センサのファイルディスクリプタを開く
    printf("Failed to open sensor file descriptor.\n");
    exit(1);
  }
  printf("Successed to open sensor file descriptor.\n");
  
  /* シグナルハンドラを初期化 */
  if(signal(SIGINT, sigint_handler) == SIG_ERR) {
    printf("[Error] Failed to initialize signal function.\n");
    close_sensor_io();//close sensor file descriptor
    return -1;
  }
  printf("Successed to initialize signal function.\n");

  if(setup_accelaration_sensor() < 0){
    printf("Failed to setup sensor(MPU-6050).\n");
    close_sensor_io();//close sensor file descriptor
    exit(1);
  }
  printf("Successed to setup sensor(MPU-6050).\n");

#if USE_OFFSET == 1
  printf("Setting Offset Value...\n");
  set_offset_value();//オフセット値生成
  printf("Finish Setting Offset Value!\n");
#endif

  sock = udp_setup(GWIPADDR, GWPORT, &addr);
  if(sock < 0){
    printf("Failed setup udp socket.\n");
    close_sensor_io();//close sensor file descriptor
    exit(1);
  }
  printf("Successed setup udp socket.\n");

  printf("Start sending data\n");
  
#if USE_BINARY == 0
  
  while(1){
    loop_start_time = gettimeofday_usec();
    msd_ret = make_send_data(sendbuff);

    if(msd_ret < 0){
      //データ生成失敗
      printf( "[ERROR] fail to get sensor data\n");
    }else{
      //データ生成成功
      send_ret = sendto(sock, sendbuff, strlen(sendbuff), MSG_NOSIGNAL & MSG_NOSIGNAL, (struct sockaddr *)&addr, sizeof(addr));

      if(send_ret < 0){
	//送信失敗
	perror("sendto");
      }
      printf("DATASEND(T):%lld, %s\n",gettimeofday_msec(), sendbuff);

      loop_elapsed_time = gettimeofday_usec() - loop_start_time;//ループ処理時間を計算
      usleep(sampling_cycle*1000 - loop_elapsed_time);//サンプリング周期の分だけ停止(ループ処理時間を減算)
    }
  }

#else
  while(1){
    loop_start_time = gettimeofday_msec();
    msd_ret = make_send_data_binary(&sendbuff);

    if(msd_ret < 0){
      //データ生成失敗
      printf( "[ERROR] fail to get sensor data\n");
    }else{
      //データ生成成功
      send_ret = sendto(sock, &sendbuff, sizeof(sendbuff), MSG_NOSIGNAL & MSG_NOSIGNAL, (struct sockaddr *)&addr, sizeof(addr));

      if(send_ret < 0){
	//送信失敗
	perror("sendto");
      }
      
      //送信データ表示
      printf("DATASEND(B):%lld, %d, %f, %f, %f\n",gettimeofday_msec(), sendbuff.node_id, sendbuff.value_x, sendbuff.value_y, sendbuff.value_z);
    }

    loop_elapsed_time = gettimeofday_msec() - loop_start_time;//ループ処理時間を計算
    usleep((sampling_cycle - loop_elapsed_time)*1000);//サンプリング周期の分だけ停止(ループ処理時間を減算)
  }

#endif

  close(sock);
    
  return 0;
}


/**
 * @brief     Ctrl-Cのシグナルハンドラ
 * @param[in] sig シグナルの値
 * @return    なし
 */
void sigint_handler(int sig){
  printf("Got a signal -> SIGINT\n");
  close_sensor_io();//close sensor file descriptor
  exit(-1);
}
