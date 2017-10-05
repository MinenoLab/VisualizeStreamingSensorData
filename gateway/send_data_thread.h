#ifndef _SEND_DATA_THREAD_H_ //2èdíËã`ÇÃñhé~
#define _SEND_DATA_THREAD_H_

void* send_data_thread(void* param);

typedef struct{
  int sock;
  uint8_t send_str[2048];
  size_t send_str_len;
}SEND_INFO;

#endif //_SEND_DATA_THREAD_H_
