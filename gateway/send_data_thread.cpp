#include <cstdio>
#include <cstdlib>
#include <unistd.h>  
#include <pthread.h>
#include <sys/socket.h>
#include <errno.h>
#include <cstring>
#include <stdint.h>
#include "setting.h"
#include "send_data_thread.h"
#include "mygettimeofday.h"
#include "client_manager.h"

void* send_data_thread(void* param){
  SEND_INFO *psend_info;
  SEND_INFO send_info;
  int send_size;

  pthread_detach(pthread_self());
  psend_info = (SEND_INFO*)param;
  send_info = *psend_info;

  free(psend_info);
  //printf("call send()\n");
  send_size = send(send_info.sock,send_info.send_str, send_info.send_str_len, MSG_NOSIGNAL);
  //  fprintf(stderr,"sended to client:%d send size:%d\n", send_info.sock, send_size);
  if(send_size < 0 /*&& errno == EPIPE*/){
    //ƒNƒ‰ƒCƒAƒ“ƒg‘¤‚Åsocket‚ªclose‚³‚ê‚Ä‚¢‚½‚çØ’f
    // fprintf(stderr, "send failed: %s\n", strerror(errno) );
    // fprintf(stderr, "remove a client (SD:%d).\n",send_info.sock);
    printf("INFO: FAILED SEND TO CLIENT %d\n",send_info.sock);
    printf("INFO: REMOVE CLIENT %d\n",send_info.sock);
    remove_client(send_info.sock);//Ø’f
  }/*else{
    printf("success send()\n");
    }*/

}
