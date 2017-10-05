#include "recv_frame_from_client.h"

void* recv_frame_from_client_thread(void* param){
  int *psocket_listen;
  int socket_listen;

  psocket_listen = (int*)param;
  socket_listen = *psocket_listen;

  while(1){
    recv_frame_from_client(socket_listen);    
  }
}
