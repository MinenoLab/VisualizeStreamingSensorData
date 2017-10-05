#include<stdio.h>
#include<unistd.h>

/*ソケット関連*/
#include <sys/socket.h>
#include <arpa/inet.h>

/*-----自作ヘッダ-----*/
#include "udp_setup.h"

/**
 * @fn udp_setup
 * @brief UDP通信の設定(ソケット生成)
 * @param *ip_addr 通信先のIPアドレス
 * @param port 通信ポート番号
 * @param *addr sockaddr_in構造体のポインタ
 * @return 生成したソケットディスクリプタ
 */
int udp_setup(const char *ip_addr, int port, struct sockaddr_in* addr){
  int sock;

  sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//socket生成
  
  addr->sin_family = AF_INET;
  addr->sin_addr.s_addr = inet_addr(ip_addr);
  addr->sin_port = htons(port);

  return sock;
  
}

