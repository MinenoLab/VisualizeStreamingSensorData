#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>

#include "datastructure.h"
#include "setting.h"
#include "my_json_encode.h"
#include "tcp_listen.h"
#include "udp_listen.h"
#include "WS_accept_thread.h"
#include "recv_frame_from_client_thread.h"
#include "ringbuffer.h"
#include "create_thread_for_client.h"
#include "mygettimeofday.h"

#include "cwebsocket/lib/websocket.h"


using namespace std;

//int sampling_cycle = DEFAULT_SAMPLING_CYCLE;
double sampling_cycle = 1.0/DEFAULT_SAMPLING_RATE * 1000; //sampling cycle is msec

int main(int argc,char **argv){
  double sampling_rate = DEFAULT_SAMPLING_RATE;

  /*
    if(argc != 1 && argc != 2){
    //�K�v�ȃR�}���h���C���������^�����Ȃ�������G���[�Ƃ��ďI��
      printf("usage: %s [sampling cycle(ms)]\n",argv[0]);
    exit(1);
  }
  */
  
  /*
  if(argc >= 2){
    sampling_cycle = atoi(argv[1]);
    if( sampling_cycle <=  0 ){
      //�T���v�����O�����Ƃ��ėL���łȂ��l���^����ꂽ��G���[�Ƃ��ďI��
      printf("Do not use \"%s\" for sampling cycle(ms)\n\n",argv[1]);
      exit(1);
    }
  }
  printf("set sampling cycle:%d\n",sampling_cycle);
  */

  if(argc >= 2){//�R�}���h���C�������Ƃ��ė^����ꂽ�l���T���v�����O���[�g�Ƃ��ĕs�K��
    if((sampling_rate = atof(argv[1])) == 0){
      printf("Do not use \"%s\" for sampling rate\n",argv[1]);
      exit(1);
    }
    sampling_cycle = (1.0/sampling_rate) * 1000;//sampling cycle is [ms]
  }
  printf("Set sampling rate:%0.3lf[Hz]\n",sampling_rate);
  printf("Set sampling cycle:%0.3lf[msec]\n", sampling_cycle);
  
  
  initQueue();//�L���[������  
  printf("finished initializing ring buffer\n");

  //�Z���T�m�[�h����̎�M�\�P�b�g
  int  sock_listen = udp_listen(RECV_PORT);
  socklen_t addrlen;//���M��(�m�[�h)�̃A�h���X���i�[����\���̂̒���
  struct sockaddr_in senderinfo;
  char recv_buf[ RECV_BUFLEN ];
  
  //�N���C�A���g(�u���E�U)�֌W�X���b�h�N��
  create_thread_for_client();
  printf("finished creating threads for clients\n");
  
  long long time = gettimeofday_msec();
  long long time_temp = time;
  int recv_ret;
  
  fprintf(stderr,"READY\n");
  printf("start receiving data\n");

#if RECV_BINARY_DATA == 0 //�e�L�X�g�`���̃f�[�^��M
  
  while(1){
    //���M���A�h���X���擾����ꍇ
    //recv_ret = recvfrom(sock_listen, recv_buf, sizeof(recv_buf)-1, 0, (struct sockaddr*)&senderinfo, &addrlen);
    //���M���A�h���X���擾���Ȃ��ꍇ
    recv_ret = recvfrom(sock_listen, recv_buf, sizeof(recv_buf)-1, 0, NULL, NULL);

    if(recv_ret > 0){
      recv_buf[recv_ret] = '\0';
      printf("RECVDATA(T): %s\n",recv_buf);

      SensorData sdata;
      sscanf(recv_buf,"%d,%f,%f,%f",&sdata.node_id, &sdata.value_x, &sdata.value_y, &sdata.value_z);
      int enq_ret = enqueue(sdata);
      if(enq_ret < 0){
	printf("ERROR in main():Failed enqueue()\n");
      }
      
    }else{
      printf("ERROR in main():Failed recv()\n");
    }
  }

#else

  while(1){
    SensorData sdata;
    recv_ret = recvfrom(sock_listen, &sdata, sizeof(sdata), 0, NULL, NULL);
    
    if(recv_ret > 0){
      
      printf("RECVDATA(B): %d,%f,%f,%f\n",sdata.node_id, sdata.value_x, sdata.value_y, sdata.value_z);
      int enq_ret = enqueue(sdata);
      if(enq_ret < 0){
	printf("ERROR in main():Failed enqueue()\n");
      }

    }else{
      printf("ERROR in main():Failed recv()\n");
    }
  }
  
#endif

  close(sock_listen); 

  return 0;
}

