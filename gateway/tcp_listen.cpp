#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>


#include "setting.h"

int tcp_listen(int port){
  int sock;
  struct sockaddr_in addr;
  int yes = 1;
  int ret;
  struct timeval socket_timeout;

  socket_timeout.tv_sec  = (int)SOCKET_SEND_TIMEOUT;
  socket_timeout.tv_usec = (SOCKET_SEND_TIMEOUT-(int)SOCKET_SEND_TIMEOUT)*1000000;

  sock = socket(PF_INET, SOCK_STREAM, 0);//TCPí êM
  if(sock < 0) {
    perror("socket");
    exit(1);
  }

  bzero((char *) &addr, sizeof(addr));
  addr.sin_family = PF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&socket_timeout, sizeof(socket_timeout));
  setsockopt(sock, SOL_TCP, TCP_NODELAY, &yes, sizeof(yes));

  ret =  bind(sock, (struct sockaddr *)&addr, sizeof(addr));
  if(ret < 0) {
    perror("bind");
    exit(1);
  }

  ret = listen(sock, 10);
  if(ret < 0) {
    perror("reader: listen");
    close(sock);
    exit(1);
  }

  return sock;
}
