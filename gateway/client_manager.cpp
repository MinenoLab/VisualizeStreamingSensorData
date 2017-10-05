#include <unistd.h>

#include "setting.h"
#include "ws_handshake.h"


bool g_clients[MAX_DESCRIPTOR] = {false};

bool add_client(int sock){
  if(sock > MAX_DESCRIPTOR){
    return false;
  }

  if(ws_handshake(sock)){
    g_clients[sock] = true;
    return true;
  }else{
    return false;
  }
}

void remove_client(int sock){
  g_clients[sock] = false;
  close(sock);
}
