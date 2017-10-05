#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/time.h>

#include "cwebsocket/lib/websocket.h"

#include "setting.h"
#include "set_fds.h"
#include "get_max_sock.h"

extern bool g_clients[MAX_DESCRIPTOR];

void recv_frame_from_client(int socket_listen){
  fd_set fds;
  int max_socket;
  char buf[WS_BUFLEN];
  uint8_t *recv_message = NULL;
  size_t message_size = 0;
  int recv_size;
  int close_message_len = strlen(close_message);

  struct timeval tv;//time outŽžŠÔ
  tv.tv_sec = 0;
  tv.tv_usec = 100000;

  set_fds(&fds, socket_listen);
  max_socket = get_max_sock();

  if(max_socket < socket_listen){
    max_socket = socket_listen;
  }

  select(max_socket+1, &fds, NULL, NULL, &tv);//using timeout
  //select(max_socket+1, &fds, NULL, NULL, NULL);//NOT using timeout

  for(int i=0; i< MAX_DESCRIPTOR; i++){

    if(g_clients[i] == false){
      continue;
    }

    if(FD_ISSET(i, &fds) != 0){

      recv_size = recv(i, buf, WS_BUFLEN, 0);
      if(recv_size > 0){
	//parse recv data
	wsParseInputFrame((uint8_t *)buf,(size_t) recv_size, &recv_message, &message_size);
	//write(1,recv_message, message_size);//stdout recv data
	
	for(int j=0; j<=(int)message_size-(int)close_message_len; j++){
	  //client‚ªØ’f‚ð•\‚·message‚ð‘—M‚µ‚Ä‚«‚½‚çsocket‚ðclose
	  if(memcmp((void*)close_message, (void*)(recv_message+j), close_message_len) ==0){
	    //	    fprintf(stderr, "\nremove a client (SD:%d).\n",i);
	    printf("INFO: REMOVE CLIENT %d\n",i);
	    g_clients[i] = false;
	    close(i);    
	  }
	}
	
      }
    }
  }
}
