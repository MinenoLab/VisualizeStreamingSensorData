#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>

#include "datastructure.h"
#include "setting.h"
#include "my_json_encode.h"
#include "tcp_listen.h"
#include "udp_listen.h"
#include "WS_accept_thread.h"
#include "recv_frame_from_client_thread.h"
#include "ringbuffer.h"
#include "create_thread_for_client.h"
#include "mygettimeofday.h"

#include "cwebsocket/lib/websocket.h"


using namespace std;

//int sampling_cycle = DEFAULT_SAMPLING_CYCLE;
double sampling_cycle = 1.0/DEFAULT_SAMPLING_RATE * 1000; //sampling cycle is msec

int main(int argc,char **argv){
  double sampling_rate = DEFAULT_SAMPLING_RATE;

  /*
    if(argc != 1 && argc != 2){
    //必要なコマンドライン引数が与えられなかったらエラーとして終了
      printf("usage: %s [sampling cycle(ms)]\n",argv[0]);
    exit(1);
  }
  */
  
  /*
  if(argc >= 2){
    sampling_cycle = atoi(argv[1]);
    if( sampling_cycle <=  0 ){
      //サンプリング周期として有効でない値が与えられたらエラーとして終了
      printf("Do not use \"%s\" for sampling cycle(ms)\n\n",argv[1]);
      exit(1);
    }
  }
  printf("set sampling cycle:%d\n",sampling_cycle);
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
  
  
  initQueue();//キュー初期化  
  printf("finished initializing ring buffer\n");

  //センサノードからの受信ソケット
  int  sock_listen = udp_listen(RECV_PORT);
  socklen_t addrlen;//送信元(ノード)のアドレスを格納する構造体の長さ
  struct sockaddr_in senderinfo;
  char recv_buf[ RECV_BUFLEN ];
  
  //クライアント(ブラウザ)関係スレッド起動
  create_thread_for_client();
  printf("finished creating threads for clients\n");
  
  long long time = gettimeofday_msec();
  long long time_temp = time;
  int recv_ret;
  
  fprintf(stderr,"READY\n");
  printf("start receiving data\n");

#if RECV_BINARY_DATA == 0 //テキスト形式のデータ受信
  
  while(1){
    //送信元アドレスを取得する場合
    //recv_ret = recvfrom(sock_listen, recv_buf, sizeof(recv_buf)-1, 0, (struct sockaddr*)&senderinfo, &addrlen);
    //送信元アドレスを取得しない場合
    recv_ret = recvfrom(sock_listen, recv_buf, sizeof(recv_buf)-1, 0, NULL, NULL);

    if(recv_ret > 0){
      recv_buf[recv_ret] = '\0';
      printf("RECVDATA(T): %s\n",recv_buf);

      SensorData sdata;
      sscanf(recv_buf,"%d,%f,%f,%f",&sdata.node_id, &sdata.value_x, &sdata.value_y, &sdata.value_z);
      int enq_ret = enqueue(sdata);
      if(enq_ret < 0){
	printf("ERROR in main():Failed enqueue()\n");
      }
      
    }else{
      printf("ERROR in main():Failed recv()\n");
    }
  }

#else

  while(1){
    SensorData sdata;
    recv_ret = recvfrom(sock_listen, &sdata, sizeof(sdata), 0, NULL, NULL);
    
    if(recv_ret > 0){
      
      printf("RECVDATA(B): %d,%f,%f,%f\n",sdata.node_id, sdata.value_x, sdata.value_y, sdata.value_z);
      int enq_ret = enqueue(sdata);
      if(enq_ret < 0){
	printf("ERROR in main():Failed enqueue()\n");
      }

    }else{
      printf("ERROR in main():Failed recv()\n");
    }
  }
  
#endif

  close(sock_listen); 

  return 0;
}

