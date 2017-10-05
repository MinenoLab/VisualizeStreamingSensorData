/**
 * @file   make_udp_socket.h
 * @brief  make_udp_socket.cに関する宣言集
 * @author Kazumasa WAKAMORI
 */


#ifndef __UDP_SETUP_H__
#define __UDP_SETUP_H__

int udp_setup(const char *ip_addr, int port, struct sockaddr_in* addr);

#endif /* __UDP_SETUP_H__*/
