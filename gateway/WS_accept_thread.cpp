#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>

#include "setting.h"
#include "tcp_listen.h"
#include "client_manager.h"

void* WS_accept_thread(void* param){
  //�N���C�A���g�i�u���E�U�j�����websocket�̐ڑ��v�����󂯂�

  int clientSocket;
  struct sockaddr addr;
  int len = sizeof(addr);
  int ret;
  int send_size;
  
  int *psocket_listen;
  int listenSocket;

  psocket_listen = (int*)param;
  listenSocket = *psocket_listen;
  
  while(1){
    clientSocket = accept(listenSocket,&addr,(socklen_t*) &len);
    //�N���C�A���g�ǉ�
    if( clientSocket < 0 || !add_client(clientSocket) ){
      //���s
      //fprintf(stderr, "Failed adding a client (SD:%d).\n",clientSocket);
      printf("[INFO]FAILED ADDING CLIENT %d\n",clientSocket);
      close(clientSocket);
    }else{
      //����
      //fprintf(stderr, "Successed adding a client (SD:%d).\n",clientSocket);
      printf("[INFO]SUCCESSED ADDING CLIENT %d\n",clientSocket);
    }
  }
  
  //close(clientSocket);
  close(listenSocket);	 
}
