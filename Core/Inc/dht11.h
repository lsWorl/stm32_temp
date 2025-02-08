#ifndef __DHT11_H
#define __DHT11_H

#include "main.h"
#include "gpio.h"

// DHT11 ���ݽṹ��
typedef struct
{
    uint8_t humidity_int;      // ʪ����������
    uint8_t humidity_dec;      // ʪ��С������
    uint8_t temperature_int;   // �¶���������
    uint8_t temperature_dec;   // �¶�С������
    uint8_t check_sum;        // У���
} DHT11_Data_TypeDef;

// ��������
void DHT11_Init(void);
uint8_t DHT11_Read_Data(DHT11_Data_TypeDef *dht11_data);
void DHT11_GPIO_OUT(void);
void DHT11_GPIO_IN(void);

#endif
