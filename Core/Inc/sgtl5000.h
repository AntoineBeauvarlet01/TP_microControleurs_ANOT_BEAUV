/*
 * sgtl5000.h
 *
 *  Created on: Apr 9, 2025
 *      Author: admin
 */

#ifndef INC_SGTL5000_H_
#define INC_SGTL5000_H_

#include "main.h"

// Adresses des registres du SGTL5000
#define CHIP_ID 0x0000
#define CODEC_ADDR 0x14
#define CHIP_LINREG_CTRL 0x0006
#define CHIP_ANA_POWER 0x0002
#define CHIP_CLK_TOP_CTRL 0x0004
#define CHIP_REF_CTRL 0x000A
#define CHIP_LINE_OUT_CTRL 0x0010
#define CHIP_SHORT_CTRL 0x0014
#define CHIP_ANA_CTRL 0x0016
#define CHIP_DIG_POWER 0x003C
#define CHIP_LINE_OUT_VOL 0x0020
#define CHIP_CLK_CTRL 0x0000
#define CHIP_I2S_CTRL 0x003A
#define CHIP_PLL_CTRL 0x0008
#define CHIP_SSS_CTRL 0x002E

// Fonction d'initialisation
void SGTL5000_Init(SPI_HandleTypeDef *hi2c);

// Fonction d'écriture de registre
HAL_StatusTypeDef SGTL5000_WriteReg(SPI_HandleTypeDef *hi2c, uint8_t reg, uint16_t data);

#endif /* INC_SGTL5000_H_ */
