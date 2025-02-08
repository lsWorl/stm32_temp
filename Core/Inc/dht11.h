#ifndef __DHT11_H
#define __DHT11_H

#include "main.h"
#include "gpio.h"

// DHT11 数据结构体
typedef struct
{
    uint8_t humidity_int;      // 湿度整数部分
    uint8_t humidity_dec;      // 湿度小数部分
    uint8_t temperature_int;   // 温度整数部分
    uint8_t temperature_dec;   // 温度小数部分
    uint8_t check_sum;        // 校验和
} DHT11_Data_TypeDef;

// 函数声明
void DHT11_Init(void);
uint8_t DHT11_Read_Data(DHT11_Data_TypeDef *dht11_data);
void DHT11_GPIO_OUT(void);
void DHT11_GPIO_IN(void);

#endif
