#include<stdio.h>
#include<pthread.h>
#include <unistd.h>

#include "WS_accept_thread.h"
#include "send_to_client_thread.h"
#include "recv_frame_from_client_thread.h"
#include "create_thread_for_client.h"
#include "tcp_listen.h"
#include "setting.h"

void create_thread_for_client(){
  const int usleep_time = 10e3;
  int socket_listen = tcp_listen(WS_PORT);//WebSocket listen socket

  pthread_t WS_accept_th;
  pthread_t send_to_client_th;
  pthread_t recv_frame_from_client_th;
  int ret;

  /*
    同じ変数socket_listenを複数のスレッドへポインタ渡しするので、
    各スレッドの起動間にsueep()関数を実行することで、
    スレッド内で値を受け取るのを待つ
   */
  
  ret = pthread_create(&WS_accept_th, NULL, WS_accept_thread, &socket_listen);
  if(ret < 0 ){
    fprintf(stderr,"[ERROR]failed to create thread:WS_accept_thread().\n");
  }

  usleep(usleep_time);
  ret = pthread_create(&send_to_client_th, NULL, send_to_client_thread, &socket_listen);
  if(ret < 0 ){
    fprintf(stderr,"[ERROR]failed to create thread:send_to_client_th().\n");
  }

  usleep(usleep_time);
  ret =pthread_create(&recv_frame_from_client_th, NULL, recv_frame_from_client_thread, &socket_listen);
  if(ret < 0 ){
    fprintf(stderr,"[ERROR]failed to create thread:recv_frame_from_client_th().\n");
  }

  usleep(usleep_time);
}
