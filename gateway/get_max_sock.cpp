#include "setting.h"

extern bool g_clients[MAX_DESCRIPTOR];

int get_max_sock(){
  int ret = 0;

  for(int i=0; i<=MAX_DESCRIPTOR; i++){
    if( g_clients[i] ){
      ret = i;
    }
  }
  return ret;
}
