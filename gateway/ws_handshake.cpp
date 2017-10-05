#include <sys/socket.h>

#include "cwebsocket/lib/websocket.h"
#include "setting.h"

bool ws_handshake(int sock){
  /*********************************************
   返り値：
     ハンドシェイク成功時:true
     ハンドシェイク失敗時:false
  ********************************************/
  char buf[WS_BUFLEN];
  memset(buf, 0, WS_BUFLEN);
  size_t framesize;
  
  framesize = recv(sock, buf, WS_BUFLEN, 0);
  if(framesize < 0){
    fprintf(stderr,"recv failed\n");
    return false;
  }

  //受信フレーム表示
  //write(1,buf,framesize);

  struct handshake hs;
  nullHandshake(&hs);
  enum wsFrameType frameType;

  frameType = wsParseHandshake((uint8_t*)buf, framesize, &hs);

  if(frameType != WS_OPENING_FRAME){
    //リクエストフレームに不備がある:空のレスポンスを返す
    fprintf(stderr,"error in websocket incoming frame\n");
    wsMakeFrame(NULL, 0, (uint8_t*)buf, &framesize, WS_CLOSING_FRAME);
    send(sock, buf, strlen(buf), MSG_NOSIGNAL);

    return false;
  }

  if (strcmp(hs.resource, "/send") != 0) {
    sprintf((char *)buf, "HTTP/1.1 404 Not Found\r\n\r\n");
    send(sock, buf, strlen(buf), MSG_NOSIGNAL);
    fprintf(stderr,"error in websocket request resource\n");
    return false;
  }

  //レスポンス生成
  wsGetHandshakeAnswer(&hs, (uint8_t*)buf, &framesize);
  freeHandshake(&hs);
  //レスポンス送信
  send(sock, buf, strlen(buf), MSG_NOSIGNAL);

  return true;
}
