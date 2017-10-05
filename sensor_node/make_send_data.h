/**
 * @file   make_send_data.h
 * @brief  make_send_data.cに関する宣言集
 * @author Kazumasa WAKAMORI
 */


/* 2重インクルード防止 */
#ifndef __MAKE_SEND_DATA__H__
#define __MAKE_SEND_DATA__H__

#if DEBUG_MODE == 0
#include <linux/i2c-dev.h> // I2C用インクルード
__s16 read_word_sensor(int fd, __u8 addr);
#endif

int make_send_data(char *data);

#endif /* __MAKE_SEND_DATA__H__ */
