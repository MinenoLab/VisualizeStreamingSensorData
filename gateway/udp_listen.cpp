#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "udp_listen.h"

int udp_listen(int port){
  int sock;
  struct sockaddr_in addr;
  
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  
  bind(sock, (struct sockaddr *)&addr, sizeof(addr));

  return sock;
}
