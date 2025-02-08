#include "dht11.h"
#include "main.h"
#include <stdio.h>

void delay_us(uint32_t us);

/**
 * @brief  ��ʼ��DHT11
 * @param  ��
 * @retval ��
 */
void DHT11_Init(void)
{
    DHT11_GPIO_OUT();
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
    HAL_Delay(1000);  // �ϵ�ȴ�1s
}

/**
 * @brief  ����DHT11��GPIOΪ���ģʽ
 * @param  ��
 * @retval ��
 */
void DHT11_GPIO_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    GPIO_InitStruct.Pin = DHT11_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

/**
 * @brief  ����DHT11��GPIOΪ����ģʽ
 * @param  ��
 * @retval ��
 */
void DHT11_GPIO_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    GPIO_InitStruct.Pin = DHT11_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

/**
 * @brief  ��ȡDHT11������
 * @param  dht11_data: DHT11���ݽṹ��ָ��
 * @retval 0: ��ȡ�ɹ�, 1: ��ȡʧ��
 */
uint8_t DHT11_Read_Data(DHT11_Data_TypeDef *dht11_data)
{
    uint8_t i, j, temp;
    uint8_t buf[5];
    uint8_t retry = 0;
    
    // �������Ϳ�ʼ�ź�
    DHT11_GPIO_OUT();
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET);
    delay_us(20000);
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
    delay_us(30); //�ӳ�30us
    
    // �л�Ϊ����ģʽ,�ȴ�DHT11��Ӧ
    DHT11_GPIO_IN();
    
    // �ȴ�DHT11����(��Ӧ�ź�) �ȴ�100us
    retry = 0;
    while(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) && retry < 100)
    {
        retry++;
        delay_us(1);
    }
    if(retry >= 100) return 1;
    
    // �ȴ�DHT11����
    retry = 0;
    while(!HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) && retry < 100)
    {
        retry++;
        delay_us(1);
    }
    if(retry >= 100) return 1;
    
    // �ȴ�DHT11����(׼����������)
    retry = 0;
    while(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) && retry < 100)
    {
        retry++;
        delay_us(1);
    }
    if(retry >= 100) return 1;
    
    // ��ʼ����40bit����
    for(i = 0; i < 5; i++)
    {
        temp = 0;
        for(j = 0; j < 8; j++)
        {
            // �ȴ�50us�͵�ƽ����
            retry = 0;
            while(!HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) && retry < 100)
            {
                retry++;
                delay_us(1);
            }
            if(retry >= 100) return 1;
            
            // ��ʱ40us
            delay_us(40);
            
            // �ж�����λ��1����0
            temp <<= 1;
            if(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin))
            {
                temp |= 1;
                // �ȴ��ߵ�ƽ����
                retry = 0;
                while(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) && retry < 100)
                {
                    retry++;
                    delay_us(1);
                }
                if(retry >= 100) return 1;
            }
        }
        buf[i] = temp;
    }
    
    // ��֤У���
    if(buf[4] == (buf[0] + buf[1] + buf[2] + buf[3]))
    {
        dht11_data->humidity_int = buf[0];
        dht11_data->humidity_dec = buf[1];
        dht11_data->temperature_int = buf[2];
        dht11_data->temperature_dec = buf[3];
        dht11_data->check_sum = buf[4];
        return 0;
    }
    
    return 1;
}
