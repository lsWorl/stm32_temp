#include "OLED.h"  // ���� OLED ͷ�ļ�
#include "OLED_Font.h"
// ���� OLED �� I2C ���
extern I2C_HandleTypeDef hi2c1;  // �� STM32CubeMX ����

/**
  * @brief  OLED д����
  * @param  Command Ҫд�������
  * @retval ��
  */
void OLED_WriteCommand(uint8_t Command)
{
    uint8_t data[2] = {0x00, Command};  // 0x00 ��ʾд����
    HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, data, 2, HAL_MAX_DELAY);
}

/**
  * @brief  OLED д����
  * @param  Data Ҫд�������
  * @retval ��
  */
void OLED_WriteData(uint8_t Data)
{
    uint8_t data[2] = {0x40, Data};  // 0x40 ��ʾд����
    HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, data, 2, HAL_MAX_DELAY);
}

/**
  * @brief  OLED ���ù��λ��
  * @param  Y �����Ͻ�Ϊԭ�㣬���·�������꣬��Χ��0~7
  * @param  X �����Ͻ�Ϊԭ�㣬���ҷ�������꣬��Χ��0~127
  * @retval ��
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
    OLED_WriteCommand(0xB0 | Y);  // ���� Y λ��
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));  // ���� X λ�ø� 4 λ
    OLED_WriteCommand(0x00 | (X & 0x0F));  // ���� X λ�õ� 4 λ
}

/**
  * @brief  OLED ����
  * @param  ��
  * @retval ��
  */
void OLED_Clear(void)
{
    uint8_t i, j;
    for (j = 0; j < OLED_PAGE_NUM; j++)
    {
        OLED_SetCursor(j, 0);
        for (i = 0; i < OLED_WIDTH; i++)
        {
            OLED_WriteData(0x00);  // �����ʾ
        }
    }
}

/**
  * @brief  OLED ��ʾһ���ַ�
  * @param  Line ��λ�ã���Χ��1~4
  * @param  Column ��λ�ã���Χ��1~16
  * @param  Char Ҫ��ʾ��һ���ַ�����Χ��ASCII �ɼ��ַ�
  * @retval ��
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{
    uint8_t i;
    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);  // ���ù��λ�����ϰ벿��
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F8x16[Char - ' '][i]);  // ��ʾ�ϰ벿������
    }
    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);  // ���ù��λ�����°벿��
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);  // ��ʾ�°벿������
    }
}

/**
  * @brief  OLED ��ʾ�ַ���
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  String Ҫ��ʾ���ַ�������Χ��ASCII �ɼ��ַ�
  * @retval ��
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        OLED_ShowChar(Line, Column + i, String[i]);
    }
}

/**
  * @brief  OLED �η�����
  * @param  X ����
  * @param  Y ָ��
  * @retval ����ֵ���� X �� Y �η�
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
    {
        Result *= X;
    }
    return Result;
}

/**
  * @brief  OLED ��ʾ���֣�ʮ���ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~4294967295
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
  * @retval ��
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
  * @brief  OLED ��ʾ���֣�ʮ���ƣ�����������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��-2147483648~2147483647
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
  * @retval ��
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
    uint8_t i;
    uint32_t Number1;
    if (Number >= 0)
    {
        OLED_ShowChar(Line, Column, '+');
        Number1 = Number;
    }
    else
    {
        OLED_ShowChar(Line, Column, '-');
        Number1 = -Number;
    }
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
  * @brief  OLED ��ʾ���֣�ʮ�����ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~0xFFFFFFFF
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~8
  * @retval ��
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++)
    {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
        if (SingleNumber < 10)
        {
            OLED_ShowChar(Line, Column + i, SingleNumber + '0');
        }
        else
        {
            OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
        }
    }
}

/**
  * @brief  OLED ��ʾ���֣������ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~1111 1111 1111 1111
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~16
  * @retval ��
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
    }
}

/**
  * @brief  OLED ��ʼ��
  * @param  ��
  * @retval ��
  */
void OLED_Init(void)
{

    // ��ʼ�� OLED
    OLED_WriteCommand(0xAE);  // �ر���ʾ
    OLED_WriteCommand(0xD5);  // ������ʾʱ�ӷ�Ƶ��/����Ƶ��
    OLED_WriteCommand(0x80);
    OLED_WriteCommand(0xA8);  // ���ö�·������
    OLED_WriteCommand(0x3F);
    OLED_WriteCommand(0xD3);  // ������ʾƫ��
    OLED_WriteCommand(0x00);
    OLED_WriteCommand(0x40);  // ������ʾ��ʼ��
    OLED_WriteCommand(0xA1);  // �������ҷ���0xA1 ����
    OLED_WriteCommand(0xC8);  // �������·���0xC8 ����
    OLED_WriteCommand(0xDA);  // ���� COM ����Ӳ������
    OLED_WriteCommand(0x12);
    OLED_WriteCommand(0x81);  // ���öԱȶȿ���
    OLED_WriteCommand(0xCF);
    OLED_WriteCommand(0xD9);  // ����Ԥ�������
    OLED_WriteCommand(0xF1);
    OLED_WriteCommand(0xDB);  // ���� VCOMH ȡ��ѡ�񼶱�
    OLED_WriteCommand(0x30);
    OLED_WriteCommand(0xA4);  // ����������ʾ��/�ر�
    OLED_WriteCommand(0xA6);  // ��������/��ת��ʾ
    OLED_WriteCommand(0x8D);  // ���ó���
    OLED_WriteCommand(0x14);
    OLED_WriteCommand(0xAF);  // ������ʾ

    OLED_Clear();  // ����
}
