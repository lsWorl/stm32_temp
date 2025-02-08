#include "W25Q64.h"



void W25Q64_ReadID(uint8_t *mid, uint16_t *did)
{

  // ��ȡID
  // ����Ƭѡ�ź�
  SPI_Start();
  // ���Ͷ�ȡID����
  SPI_SwapByte(0x9f);
  // ��ȡ������id
  *mid = SPI_SwapByte(0xff);
  // ��ȡ�豸id
  *did = 0;
  *did |= SPI_SwapByte(0xff) << 8;   // ��8λ
  *did |= SPI_SwapByte(0xff) & 0xff; // ��8λ
  // �ر�Ƭѡ�ź�
  SPI_Stop();

  // ���͵�����Ϣ������
  char debugMsg[50];
  int msgLength = snprintf(debugMsg, sizeof(debugMsg), "Manufacturer ID: 0x%02X, Device ID: 0x%04X\r\n", *mid, *did);
  HAL_UART_Transmit(&huart1, (uint8_t *)debugMsg, msgLength, HAL_MAX_DELAY);
}

void W25Q64_WritePage(uint32_t block, uint8_t sector, uint32_t page, uint8_t *data, uint32_t len) {}
void W25Q64_ReadPage(uint32_t block, uint8_t sector, uint32_t page, uint8_t innerAddr, uint8_t *buffer, uint32_t len) {}
void W25Q64_Erase(uint32_t addr, uint32_t len) {}
void W25Q64_ChipErase(void) {}

void W25Q64_WriteEnable(void) {}
void W25Q64_WriteDisable(void) {}

void W25Q64_WaitNotBusy(void) {}

void W25Q64_SectorErase(uint8_t block, uint8_t sector) {}
