#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <pthread.h>
#include <unistd.h>  
#include <sys/socket.h>
#include <sys/time.h>
#include <errno.h>
#include <poll.h>

#include "setting.h"
#include "datastructure.h"
#include "my_json_encode.h"
#include "set_fds.h"
#include "get_max_sock.h"
#include "cwebsocket/lib/websocket.h" 
#include "send_data_thread.h"
#include "ringbuffer.h"
#include "mygettimeofday.h"

extern bool g_clients[MAX_DESCRIPTOR];

using namespace std;
//extern int sampling_cycle;
extern double sampling_cycle;

void* send_to_client_thread(void* param){
  //�N���C�A���g(�u���E�U)�ւ̃f�[�^���M�X���b�h
  char send_data[4096];
  int *psocket_listen;
  int socket_listen;
  long long loop_start_time;//�f�[�^���M���[�v�̎��Ԍv�v��
  //int loop_elapsed_time;////�f�[�^���M���[�v�̎��Ԍv�v��
  int usleep_time;

  psocket_listen = (int*)param;
  socket_listen = *psocket_listen;

  while(1){
    loop_start_time = gettimeofday_usec();
    /*�o�b�t�@�T�C�Y�m�F(�f�o�b�N�p)*/
    /*
    for(int i=0;i<4;i++){
      fprintf(stderr,"%d, ",getQueueSize(i));
    }
    fprintf(stderr,"\n");
    */
    char accel_x_str[128],accel_y_str[128],accel_z_str[128];
    char node_id_str[128];
    SensorData data;

    sprintf(send_data,"{\"sensingtime\":%lld,", gettimeofday_msec());
    sprintf(send_data+strlen(send_data),"\"data\":[");
    
    //���M�f�[�^����
    bool firstdata = false;
    for(int i=0; i<MAX_SENSOR_NUM; i++){
      vector<pair<string,string> > piece_of_data;
      int deq_ret = dequeue(i,&data);
      if(deq_ret >= 0){//���o������
	if(firstdata){//2��߈ȍ~�͐擪�ɃR���}��t��
	  sprintf(send_data+strlen(send_data),",");
	}else{
	  firstdata = true;
	}
	sprintf(accel_x_str,"%f",data.value_x);//�����x����
	sprintf(accel_y_str,"%f",data.value_y);
	sprintf(accel_z_str,"%f",data.value_z);
	sprintf(node_id_str,"%d",data.node_id);//�Z���TID

	piece_of_data.push_back(make_pair("id",node_id_str));
	piece_of_data.push_back(make_pair("x",accel_x_str));
	piece_of_data.push_back(make_pair("y",accel_y_str));
	piece_of_data.push_back(make_pair("z",accel_z_str));
      
	//string piece_of_data_str = my_json_encode(piece_of_data);
	string piece_of_data_str = my_json_encode_noindent(piece_of_data);

	sprintf(send_data+strlen(send_data),"%s",piece_of_data_str.c_str());
      }
    }
  
    sprintf(send_data+strlen(send_data),"]}");

    enum wsFrameType frameType;
    int send_size;
    int max_sock;
    fd_set fds;
    struct timeval tv;//time out����
    SEND_INFO send_info;
    pthread_t send_data_th;
    size_t info_size = (size_t)sizeof(SEND_INFO);

    send_info.send_str_len = sizeof(send_info.send_str);

    wsMakeFrame( (uint8_t*)send_data, strlen(send_data), send_info.send_str, &(send_info.send_str_len), WS_TEXT_FRAME);
    printf("%s\n",send_data);
      
    set_fds(&fds, socket_listen);
    max_sock = get_max_sock();
    if(max_sock < socket_listen){
      max_sock = socket_listen;
    }
    
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    select(max_sock+1, NULL, &fds, NULL, &tv);

    for(int sd = 0; sd<=max_sock; sd++){
      if(!g_clients[sd]){
	continue;       
      }

      if(FD_ISSET(sd, &fds)!=0){
	//���M�����X���b�h�ɓn�����M���,�J���͑��M�����X���b�h�ōs��
	SEND_INFO *psend_info = (SEND_INFO*)malloc(info_size);
	memcpy(psend_info, &send_info, info_size);
	(*psend_info).sock = sd;
	
	//���M�����X���b�h�𐶐�
	pthread_create(&send_data_th, NULL, send_data_thread, psend_info);
      }
    }


    //loop_elapsed_time = gettimeofday_usec() - loop_start_time;//���[�v�������Ԃ��v�Z
    usleep_time = sampling_cycle*1000 - (gettimeofday_usec() - loop_start_time);
    //printf("usleep time %d\n",usleep_time);
    if(usleep_time >= 0){
      usleep(usleep_time);//�T���v�����O�����̕�������~(���[�v�������Ԃ����Z)
    }
  }
}
