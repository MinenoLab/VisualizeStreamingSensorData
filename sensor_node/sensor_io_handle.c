#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include"setting.h"
#include"sensor_io_handle.h"


#if DEBUG_MODE == 0
#include <linux/i2c-dev.h> // I2C用インクルード
#endif

#if DEBUG_MODE == 0

/*--Global Variable--*/
int i2c_fd;
/***USE ADXL345***/
const __u8 ACCEL_X_reg = 0x32;//加速度が格納されるレジスタの先頭アドレス
const __u8 ACCEL_Y_reg = 0x34;
const __u8 ACCEL_Z_reg = 0x36;
const char *i2cFileName = "/dev/i2c-1";
const unsigned char i2cAddress = 0x1d;//device address
const __u8 PWR_MGMT = 0x2D;//電源(POWER_CTL)
const float LSB_Sensitivity = 256.0;
/***USE ADXL345 END***/

const float GRAVITY =  9.80665;//重力加速度

int ACCEL_SENSOR_OFFSET[3] = {0};//加速度センサのオフセット値(生データの場合はlong long)

extern double sampling_cycle;

/*-------------------*/

/**
 * @fn openSensorIO
 * @brief センサのファイルディスクリプタを開く
 * @param なし
 * @return 0:成功 -1:失敗
 */
/***USE ADXL345***/
int open_sensor_io(){
  //register address
  __s32 ret;
  
  if((i2c_fd = open(i2cFileName,O_RDWR)) < 0){
    printf("Faild to open i2c port¥n");
    return -1;
  }
  if (ioctl(i2c_fd, I2C_SLAVE,i2cAddress) < 0) {
    printf("Unable to get bus access to talk to slave¥n");
    return -1;
  }

  
  ret = i2c_smbus_write_word_data(i2c_fd, PWR_MGMT, 0x00);
  if(ret != 0){
    printf("Failed i2c_smbus_write_word_data()\n");
    printf("%s\n",strerror(errno));
    return -1;
  }
  

  //  printf("sampling cycle = %lf\n",sampling_cycle);
  return 0;
}

/***USE ADXL345 END***/


/**
 * @fn closeSensorIO
 * @brief センサのファイルディスクリプタを閉じる
 * @param なし
 * @return なし
 */
/*MPU-6050, ADXL345共通*/
void close_sensor_io(){
  close(i2c_fd);
}

/**
 * @fn read_word_sensor
 * @brief センサデータをword単位(16bit)で読みだす
 * @param fd  :センサのファイルディスクリプタ
 * @param addr:読みだすレジスタの先頭アドレス
 * @return データ値(16bit)
 */
/***USE ADXL345***/
__s16 read_word_sensor(int fd,__u8 addr){
  __s16 res;
  
  res = i2c_smbus_read_word_data(fd,addr);

  return res;  
}
/***USE ADXL345 USE***/

/**
 * @fn setup_accelaration_sensor
 * @brief 加速度センサMPU-6050のセットアップを行う
 * @param センサのファイルディスクリプタ
 * @return 0:成功 -1:失敗
 * @memo : http://43zrtwysvxb2gf29r5o0athu.wpengine.netdna-cdn.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
 */

/***USE ADXL345***/
int setup_accelaration_sensor(){
  const __u8 DATA_FORMAT = 0x31;//setting data format addr
  const __u8 BW_RATE  = 0x2C;//setting band width addr
  //const __u8 FIFO_CTL = 0x38;
  
  //const __u8 BW_RATE_1600HZ      = 0x0F;
  //const __u8 BW_RATE_800HZ       = 0x0E;
  //const __u8 BW_RATE_400HZ       = 0x0D;
  //const __u8 BW_RATE_200HZ       = 0x0C;
  //const __u8 BW_RATE_100HZ       = 0x0B;
  const __u8 BW_RATE_50HZ        = 0x0A;
  //const __u8 BW_RATE_25HZ        = 0x09;

  const __u8 RANGE_2G            = 0x00;
  //const __u8 RANGE_4G            = 0x01;
  //const __u8 RANGE_8G            = 0x02;
  //const __u8 RANGE_16G           = 0x03;

  const __u8 MEASURE             = 0x08;
 
  __s32 ret,value;
  
  usleep(100e3);
  //Measureビットをセットし測定モードに
  ret = i2c_smbus_write_byte_data(i2c_fd, PWR_MGMT, MEASURE);//device reset
  if(ret != 0){
    printf("Failed to reset device.\n");
    return -1;
  }
   
  usleep(100e3);
  ret = i2c_smbus_write_byte_data(i2c_fd, BW_RATE, BW_RATE_50HZ);//BandWidth設定
  if(ret != 0){
    printf("Failed to set Band Width.\n");
    return -1;
  }

  usleep(100e3);
  value = i2c_smbus_read_byte_data(i2c_fd, DATA_FORMAT);
  value &= ~0x0F;
  value |= RANGE_2G;  
  value |= 0x08;
  printf("SET DATA_FORMAT:%X\n",value);
  ret = i2c_smbus_write_byte_data(i2c_fd, DATA_FORMAT, value);//set scale range = (+-)2g
  if(ret != 0){
    printf("Failed to set scale range.\n");
    return -1;
  }

  usleep(100e3);

  return 0;
}
/***USE ADXL345 END***/

/**
 * @fn set_offset_value
 * @brief 加速度センサのオフセット値を算出
 * @param *offset :オフセット値を格納するfloat型配列(offset[0]:X軸,offset[1]:Y軸,offset[2]:Z軸)
 * @return なし
 */
void set_offset_value(){
  int data_num = OFFSET_COUNT;
  int i;

  /*オフセット算出時に使用するバッファ*/
  long long accel_buff[3] = {0};
  
  for(i=0;i<data_num;i++){
    accel_buff[0] += read_word_sensor(i2c_fd, ACCEL_X_reg);
    accel_buff[1] += read_word_sensor(i2c_fd, ACCEL_Y_reg);
    accel_buff[2] += read_word_sensor(i2c_fd, ACCEL_Z_reg);
    usleep(1e3);
  }

  //平均算出
  for(i=0; i<3; i++){
    ACCEL_SENSOR_OFFSET[i] = accel_buff[i]/data_num;
  }

  printf("OFFSET: %d,%d,%d\n",ACCEL_SENSOR_OFFSET[0],ACCEL_SENSOR_OFFSET[1],ACCEL_SENSOR_OFFSET[2]);
  
  return;
}

/**
 * @fn make_send_data
 * @brief 送信するセンサデータを生成
 * @param *data :データ文字列を格納する文字配列のポインタ
 * @return 0:成功 -1:失敗
 */
int make_send_data(char *data){//センサデータ生成
  //[0]:X軸, [1]:Y軸, [2]:Z軸
  int accel_raw[3];      //加速度(生データ)
  float accel[3];       //加速度
  int i;//ループカウンタ
  
  accel_raw[0] = read_word_sensor(i2c_fd, ACCEL_X_reg); 
  accel_raw[1] = read_word_sensor(i2c_fd, ACCEL_Y_reg); 
  accel_raw[2] = read_word_sensor(i2c_fd, ACCEL_Z_reg);
  
  //オフセットを適用
  for(i=0; i<3; i++){
    accel_raw[i] -= ACCEL_SENSOR_OFFSET[i];
  }
    
  //生データ->[g]に変換
  for(i=0; i<3; i++){
    accel[i] = accel_raw[i] / LSB_Sensitivity;
  }
  
  //[g]->[m/s^2]に変換
  for(i=0; i<3; i++){
    accel[i] *= GRAVITY;  
  }

  sprintf(data,"%d,%f,%f,%f",NODEID,accel[0],accel[1],accel[2]);
  
  return 0;
}

/**
 * @fn make_send_data_binary
 * @brief 送信するセンサデータをバイナリ形式で生成
 * @param *data :データ文字列を格納する文字配列のポインタ
 * @return 0:成功 -1:失敗
 */
int make_send_data_binary(BinaryDataFormat *data){//センサデータ生成
   //[0]:X軸, [1]:Y軸, [2]:Z軸
  int accel_raw[3];      //加速度(生データ)
  float accel[3];       //加速度
  int i;//ループカウンタ
  accel_raw[0] = read_word_sensor(i2c_fd, ACCEL_X_reg); 
  accel_raw[1] = read_word_sensor(i2c_fd, ACCEL_Y_reg); 
  accel_raw[2] = read_word_sensor(i2c_fd, ACCEL_Z_reg);
  //printf("ACCEL(raw):   %d,%d,%d\n",accel_raw[0],accel_raw[1],accel_raw[2]);
  
  //オフセットを適用
  for(i=0; i<3; i++){
    accel_raw[i] -= ACCEL_SENSOR_OFFSET[i];
  }
     
  //生データ->[g]に変換
  for(i=0; i<3; i++){
    accel[i] = accel_raw[i] / LSB_Sensitivity;
  }
  
  //[g]->[m/s^2]に変換
  for(i=0; i<3; i++){
    accel[i] *= GRAVITY;  
  }

  data->node_id = NODEID;
  data->value_x = accel[0];
  data->value_y = accel[1];
  data->value_z = accel[2];
  return 0;
}

#else

/*---以下,デバック用---*/
int open_sensor_io(){//何もしない
  return 0;
}

void close_sensor_io(){//何もしない
}

int setup_accelaration_sensor()(){//何もしない
  return 0;
}

int make_send_data(char *data){//ダミーセンサデータを生成
  float accel_x,accel_y,accel_z;
  accel_x = 0.019 * (NODEID+1);
  accel_y = -0.011 * (NODEID+1);
  accel_z = -0.017 * (NODEID+1);
  //ダミーデータ生成
  sprintf(data,"%d,%f,%f,%f",NODEID,accel_x,accel_y,accel_z);
  
  return 0;
}

int make_send_data_binary(BinaryDataFormat *data){//センサデータ生成
  float value_x,value_y,value_z;
  value_x =  0.011 * (NODEID+1);
  value_y = -0.022 * (NODEID+1);
  value_z = -0.033 * (NODEID+1);

  data->node_id = NODEID;
  data->value_x = value_x;
  data->value_x = value_y;
  data->value_x = value_z;

  return 0;
}

void set_offset_value(){//何もしない
}
#endif
