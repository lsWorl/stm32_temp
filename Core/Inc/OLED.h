#ifndef __OLED_H
#define __OLED_H

#include "stm32f1xx_hal.h"  // 包含 HAL 库头文件

// OLED 的 I2C 地址
#define OLED_ADDRESS 0x78  // 7 位 I2C 地址，左移 1 位后为 0xF0

// OLED 屏幕尺寸定义
#define OLED_WIDTH  128    // OLED 宽度
#define OLED_HEIGHT 64     // OLED 高度
#define OLED_PAGE_NUM 8    // OLED 页数（每页 8 行）

// OLED 函数声明
void OLED_Init(void);                            // OLED 初始化
void OLED_Clear(void);                           // OLED 清屏
void OLED_SetCursor(uint8_t Y, uint8_t X);       // 设置光标位置
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);  // 显示一个字符
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);  // 显示字符串
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);  // 显示数字（十进制）
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);  // 显示有符号数字
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);  // 显示十六进制数字
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);  // 显示二进制数字

// 内部函数声明（仅供模块内部使用）
void OLED_WriteCommand(uint8_t Command);  // 写命令
void OLED_WriteData(uint8_t Data);        // 写数据

#endif
