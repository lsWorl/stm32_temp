#ifndef __W25Q64_H
#define __W25Q64_H

#include "main.h"
#include <stdio.h>
#include "usart.h"
#include "spi.h"

void W25Q64_WritePage(uint32_t block, uint8_t sector, uint32_t page, uint8_t *data, uint32_t len);
void W25Q64_ReadPage(uint32_t block, uint8_t sector, uint32_t page,uint8_t innerAddr, uint8_t *buffer, uint32_t len);
void W25Q64_Erase(uint32_t addr, uint32_t len);
void W25Q64_ReadID(uint8_t *mid,uint16_t *did);
void W25Q64_ChipErase(void);

void W25Q64_WriteEnable(void);
void W25Q64_WriteDisable(void);

void W25Q64_WaitNotBusy(void);

void W25Q64_SectorErase(uint8_t block, uint8_t sector);

#endif
