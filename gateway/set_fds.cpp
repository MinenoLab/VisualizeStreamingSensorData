#include <sys/select.h>
#include "setting.h"

extern bool g_clients[MAX_DESCRIPTOR];

void set_fds(fd_set* pfds, int accept_sock){
  int i;

  FD_ZERO(pfds);
  //FD_SET(accept_sock, pfds);
  
  for(int i=0; i<=MAX_DESCRIPTOR; i++){
    if( g_clients[i] ){
      FD_SET(i, pfds);
    }
  }
}
