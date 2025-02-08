#include "dht11.h"
#include "main.h"
#include <stdio.h>

void delay_us(uint32_t us);

/**
 * @brief  初始化DHT11
 * @param  无
 * @retval 无
 */
void DHT11_Init(void)
{
    DHT11_GPIO_OUT();
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
    HAL_Delay(1000);  // 上电等待1s
}

/**
 * @brief  配置DHT11的GPIO为输出模式
 * @param  无
 * @retval 无
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
 * @brief  配置DHT11的GPIO为输入模式
 * @param  无
 * @retval 无
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
 * @brief  读取DHT11的数据
 * @param  dht11_data: DHT11数据结构体指针
 * @retval 0: 读取成功, 1: 读取失败
 */
uint8_t DHT11_Read_Data(DHT11_Data_TypeDef *dht11_data)
{
    uint8_t i, j, temp;
    uint8_t buf[5];
    uint8_t retry = 0;
    
    // 主机发送开始信号
    DHT11_GPIO_OUT();
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET);
    delay_us(20000);
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
    delay_us(30); //延迟30us
    
    // 切换为输入模式,等待DHT11响应
    DHT11_GPIO_IN();
    
    // 等待DHT11拉低(响应信号) 等待100us
    retry = 0;
    while(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) && retry < 100)
    {
        retry++;
        delay_us(1);
    }
    if(retry >= 100) return 1;
    
    // 等待DHT11拉高
    retry = 0;
    while(!HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) && retry < 100)
    {
        retry++;
        delay_us(1);
    }
    if(retry >= 100) return 1;
    
    // 等待DHT11拉低(准备发送数据)
    retry = 0;
    while(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) && retry < 100)
    {
        retry++;
        delay_us(1);
    }
    if(retry >= 100) return 1;
    
    // 开始接收40bit数据
    for(i = 0; i < 5; i++)
    {
        temp = 0;
        for(j = 0; j < 8; j++)
        {
            // 等待50us低电平结束
            retry = 0;
            while(!HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) && retry < 100)
            {
                retry++;
                delay_us(1);
            }
            if(retry >= 100) return 1;
            
            // 延时40us
            delay_us(40);
            
            // 判断数据位是1还是0
            temp <<= 1;
            if(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin))
            {
                temp |= 1;
                // 等待高电平结束
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
    
    // 验证校验和
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
