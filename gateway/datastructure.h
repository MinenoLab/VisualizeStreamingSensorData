/**
 * @file   datastructure.h
 * @brief  使用する列挙型,構造体等の定義
 * @author Kazumasa WAKAMORI
 */

#include <time.h>
#include <vector>
#include "setting.h"

/* 2重インクルード防止 */
#ifndef __DATASTRUCTURE_H__
#define __DATASTRUCTURE_H__

typedef struct{
  short node_id;
  float value_x;//x軸データ
  float value_y;//y軸データ
  float value_z;//z軸データ
}SensorData;

typedef struct{
  time_t timestamp;
  std::vector<SensorData> datas;
}SensorDataAll;


#endif /* __DATA_STRUCTURE_H__ */
