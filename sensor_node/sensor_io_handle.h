/**
 * @file   sensor_io_handle.h
 * @brief  sensor_io_handle.cに関する宣言集
 * @author Kazumasa WAKAMORI
 */


/* 2重インクルード防止 */
#ifndef __SENSOR_IO_HANDLE__H__
#define __SENSOR_IO_HANDLE__H__

#if DEBUG_MODE == 0
#include <linux/i2c-dev.h> // I2C用インクルード
__s16 read_word_sensor(int fd, __u8 addr);
#endif

typedef struct{
  short node_id;//センサノードID
  //int accel_x;//x軸加速度
  //int accel_y;//y軸加速度
  //int accel_z;//z軸加速度
  float value_x;  //x軸変位
  float value_y;  //y軸変位
  float value_z;  //z軸変位
}BinaryDataFormat;

int open_sensor_io();
void close_sensor_io();
int make_send_data(char *data);
int make_send_data_binary(BinaryDataFormat *data);
int setup_accelaration_sensor();
void set_offset_value();
int HPF(float *displacement);

#endif /* __SENSOR_IO_HANDLE__H__ */
