#ifndef __OLED_H
#define __OLED_H

#include "stm32f1xx_hal.h"  // ���� HAL ��ͷ�ļ�

// OLED �� I2C ��ַ
#define OLED_ADDRESS 0x78  // 7 λ I2C ��ַ������ 1 λ��Ϊ 0xF0

// OLED ��Ļ�ߴ綨��
#define OLED_WIDTH  128    // OLED ���
#define OLED_HEIGHT 64     // OLED �߶�
#define OLED_PAGE_NUM 8    // OLED ҳ����ÿҳ 8 �У�

// OLED ��������
void OLED_Init(void);                            // OLED ��ʼ��
void OLED_Clear(void);                           // OLED ����
void OLED_SetCursor(uint8_t Y, uint8_t X);       // ���ù��λ��
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);  // ��ʾһ���ַ�
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);  // ��ʾ�ַ���
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);  // ��ʾ���֣�ʮ���ƣ�
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);  // ��ʾ�з�������
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);  // ��ʾʮ����������
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);  // ��ʾ����������

// �ڲ���������������ģ���ڲ�ʹ�ã�
void OLED_WriteCommand(uint8_t Command);  // д����
void OLED_WriteData(uint8_t Data);        // д����

#endif
