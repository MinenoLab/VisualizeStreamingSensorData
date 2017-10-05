/**
 * @file   ringbuffer.c
 * @brief  リングバッファに関するプログラム
 * @author Kazumasa WAKAMORI
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include <iostream>
#include <queue>

#include "datastructure.h"
#include "setting.h"
#include "ringbuffer.h"

using namespace std;

queue<SensorData> que[MAX_SENSOR_NUM];
pthread_mutex_t mutex[MAX_SENSOR_NUM];

/**
 * @brief  キューを初期化する
 * @param  なし
 * @return なし
 */
void initQueue() {
  //キュー初期化
  for(int i=0;i<MAX_SENSOR_NUM;i++){
    queue<SensorData> empty;
    swap( que[i], empty );
  }

  //mutex初期化
  for(int i=0;i<MAX_SENSOR_NUM;i++){
    int mtx_d_ret = pthread_mutex_destroy( &mutex[i] );

    if(mtx_d_ret < 0){
      fprintf(stderr,"[ERROR]failed to do pthread_mutex_destroy().\n");
      exit(1);
    }
    pthread_mutex_init(&mutex[i], NULL);//返り値は常に0
  }
}

/**
 * @brief     キューにデータ(文字列)を入れる
 * @param[in] data キューに入れるセンサデータ
 * @retval    0  成功
 * @retval    -1 失敗
 */
int enqueue(SensorData data) {
  //int mutex_ret;
  if(data.node_id < 0 || data.node_id >= MAX_SENSOR_NUM){/*ノードIDが無効*/
    printf("ERROR in enqueue(): Illegal NodeID\n");
    return -1;
  }
  if ( que[data.node_id].size() >= QUEUE_SIZE) { /* キューに空きがない */
    //printf("ERROR in enqueue(): Queue[%d] is full\n",data.node_id);
    //return -1;
    que[data.node_id].pop();
  }

  pthread_mutex_lock( &mutex[data.node_id] );//mutexロック
  que[data.node_id].push(data);
  pthread_mutex_unlock( &mutex[data.node_id] );//mutexアンロック
  /*
  mutex_ret = pthread_mutex_unlock( &mutex[data.node_id] );//mutexアンロック
  if(mutex_ret < 0){
    printf("ERROR: Failed pthread_mutex_unlock()%d\n",data.node_id);
  }
  */

  //  fprintf(stderr,"queue[%2d] size=%d\n",data.node_id,que[data.node_id].size());
  return 0;
}

/**
 * @brief      キューからデータ(文字列)を取り出す
 * @param[out] node_id ノードID(どのノードのキューからデータをとりだすかを表す)
 * @param[out] data キューから取り出したセンサデータ
 * @retval     0  成功
 * @retval     -1 失敗
 */
int dequeue(int node_id, SensorData* data) {
  //int  mutex_ret;
  if(node_id < 0 || node_id >= MAX_SENSOR_NUM){/*ノードIDが無効*/
    //printf("ERROR in dequeue(): Illegal Node ID\n");
    return -1;
  }
  if (que[node_id].empty()) {      /* キューが空 */
    //printf("ERROR in dequeue(): Queue[%d] is empty\n",node_id);
    return -1;
  }

  pthread_mutex_lock( &mutex[node_id] );//mutexロック
  SensorData d = que[node_id].front();
  *data = d;
  que[node_id].pop();
  pthread_mutex_unlock( &mutex[node_id] );//mutexアンロック
  /*
  mutex_ret = pthread_mutex_unlock( &mutex[node_id] );//mutexアンロック
  
  if(mutex_ret < 0){
    printf("ERROR: Failed pthread_mutex_unlock()%d\n",node_id);
  }
  */

  return 0;
}

/**
 * @brief  キューが空かどうかを調べる
 * @param  node_id どのノード用のキューを調べるか
 * @retval 1 空
 * @retval 0 空でない
 */
int isQueueEmpty(int node_id) {
  if(que[node_id].empty()){
    return 1;
  }else{
    return 0;
  }
}

/**
 * @brief  キューのサイズを調べる
 * @param  node_id どのノード用のキューを調べるか
 * @retval 0以上 キューサイズ
 * @retval 0未満 失敗
 */
int getQueueSize(int node_id) {
  if(node_id < 0 || node_id >= MAX_SENSOR_NUM){/*ノードIDが無効*/
    return -1;
  }
  return que[node_id].size();
}

