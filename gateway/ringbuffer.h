/**
 * @file   ringbuffer.h
 * @brief  ringbuffer.cppに関する宣言集
 * @author Kazumasa WAKAMORI
 */
#include "datastructure.h"

/* 2重インクルード防止 */
#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__
/* ===========================宣言ここから=========================== */

void initQueue();       /* キューを初期化する */
int enqueue(SensorData data); /* キューにデータを入れる */
int dequeue(int node_id, SensorData* data); /* キューからデータを取り出す */
int isQueueEmpty(int node_id) ;    /* キューが空かどうかを調べる */
int getQueueSize(int node_id); /*キューのサイズを調べる*/

/* ===========================宣言ここまで=========================== */
#endif /* __RING_BUFFER_H__ */
