/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f1xx_it.c
 * @brief   Interrupt Service Routines.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "OLED.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUFFER_SIZE 256
#define MAX_BUFFER_SIZE 256
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void ParseAndDisplayData(void);
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
char rx_buffer[MAX_BUFFER_SIZE];
volatile uint16_t rx_index = 0;
volatile uint8_t receive_complete = 0;

// ����һ���ṹ�����洢�������ƶ�����
typedef struct {
    char city[20];
    float similarity;
} CityData;

#define MAX_CITIES 5
CityData city_data[MAX_CITIES];
volatile uint8_t city_count = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
    /* USER CODE BEGIN USART1_IRQn 0 */
    uint8_t received_data;
    
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) == SET)
    {
        received_data = (uint8_t)(huart1.Instance->DR & 0xFF);
        
        // �����ʼ��־
        if (received_data == 0xAA)
        {
            rx_index = 0;
            receive_complete = 0;
        }
        // ��������־
        else if (received_data == 0x55)
        {
            rx_buffer[rx_index] = '\0';
            receive_complete = 1;
            
            // �������ݲ���ʾ��OLED��
            ParseAndDisplayData();
        }
        // ��������
        else if (rx_index < MAX_BUFFER_SIZE - 1)
        {
            rx_buffer[rx_index++] = received_data;
        }
    }
    /* USER CODE END USART1_IRQn 0 */
    HAL_UART_IRQHandler(&huart1);
    /* USER CODE BEGIN USART1_IRQn 1 */
    /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void ParseAndDisplayData(void)
{
    char *saveptr1, *saveptr2;
    char *token;
    char *city_token;
    city_count = 0;
    
    // ���OLED��ʾ
    OLED_Clear();
    OLED_ShowString(1, 1, "Similar Cities:");
    
    // ��ӡ���յ���ԭʼ����
    printf("Received data: %s\r\n", rx_buffer);
    
    // ʹ��strtok_r���ָ��ַ�������������ȫ
    token = strtok_r(rx_buffer, ";", &saveptr1);
    while (token != NULL && city_count < 3)
    {
        printf("Processing token: %s\r\n", token);
        
        // �ָ�����������ƶ�
        city_token = strtok_r(token, ":", &saveptr2);
        if (city_token != NULL)
        {
            // ���������
            strncpy(city_data[city_count].city, city_token, 19);
            city_data[city_count].city[19] = '\0';
            
            // ��ȡ���ƶ�ֵ
            city_token = strtok_r(NULL, ":", &saveptr2);
            if (city_token != NULL)
            {
                city_data[city_count].similarity = atof(city_token);
                
                // ��OLED����ʾ����
                char display_buffer[21];
                snprintf(display_buffer, sizeof(display_buffer), 
                        "%s:%.2f", 
                        city_data[city_count].city,
                        city_data[city_count].similarity);
                
                // ֱ����ʾ�ڶ�Ӧ�У�ȷ���к���ȷ
                OLED_ShowString(city_count + 2, 1, display_buffer);
                
                printf("Displaying: %s on line %d\r\n", display_buffer, city_count + 2);
                
                city_count++;
            }
        }
        
        // ��ȡ��һ����������
        token = strtok_r(NULL, ";", &saveptr1);
    }
    
    printf("Total cities processed: %d\r\n", city_count);
}
/* USER CODE END 1 */
