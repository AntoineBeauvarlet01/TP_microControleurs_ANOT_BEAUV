/*
 * sgtl5000.c
 *
 *  Created on: Apr 9, 2025
 *      Author: admin
 */

#include "sgtl5000.h"

// Fonction d'écriture de registre
HAL_StatusTypeDef SGTL5000_WriteReg(SPI_HandleTypeDef *hi2c, uint8_t reg, uint16_t data) {
	uint8_t tx_buffer[3];
	tx_buffer[0] = (reg << 1); // Adresse du registre + bit d'écriture (0)
	tx_buffer[1] = (data >> 8) & 0xFF; // Octet de poids fort
	tx_buffer[2] = data & 0xFF; // Octet de poids faible
	return HAL_I2C_Master_Transmit(hi2c, 0x0A, tx_buffer, 3, HAL_MAX_DELAY); // adresse sgtl5000 = 0x0A
}


// Fonction d'initialisation
void SGTL5000_Init(SPI_HandleTypeDef *hi2c) {

	// Configuration de l'alimentation
	SGTL5000_WriteReg(hi2c, CHIP_LINREG_CTRL, 0x0008);
	SGTL5000_WriteReg(hi2c, CHIP_ANA_POWER, 0x7260);
	SGTL5000_WriteReg(hi2c, CHIP_CLK_TOP_CTRL, 0x0800);
	SGTL5000_WriteReg(hi2c, CHIP_ANA_POWER, 0x4A60);
	SGTL5000_WriteReg(hi2c, CHIP_LINREG_CTRL, 0x006C);

	// Tension de référence et configuration du courant de polarisation
	SGTL5000_WriteReg(hi2c, CHIP_REF_CTRL, 0x004E);
	SGTL5000_WriteReg(hi2c, CHIP_LINE_OUT_CTRL, 0x0322);

	// Autres configurations de blocs analogiques
	SGTL5000_WriteReg(hi2c, CHIP_REF_CTRL, 0x004F);
	SGTL5000_WriteReg(hi2c, CHIP_SHORT_CTRL, 0x1106);
	SGTL5000_WriteReg(hi2c, CHIP_ANA_CTRL, 0x0133);

	// Mise sous tension des entrées/sorties/blocs numériques
	SGTL5000_WriteReg(hi2c, CHIP_ANA_POWER, 0x6AFF);
	SGTL5000_WriteReg(hi2c, CHIP_DIG_POWER, 0x0073);

	// Définir le niveau de volume LINEOUT
	SGTL5000_WriteReg(hi2c, CHIP_LINE_OUT_VOL, 0x0505);

	// Horloge Système MCLK et Horloge d'Échantillonnage
	SGTL5000_WriteReg(hi2c, CHIP_CLK_CTRL, 0x0002);
	SGTL5000_WriteReg(hi2c, CHIP_CLK_CTRL, 0x0000);
	SGTL5000_WriteReg(hi2c, CHIP_I2S_CTRL, 0x0001);

	// Configuration de la PLL
	SGTL5000_WriteReg(hi2c, CHIP_ANA_POWER, 0x0400);
	SGTL5000_WriteReg(hi2c, CHIP_ANA_POWER, 0x0100);
	SGTL5000_WriteReg(hi2c, CHIP_CLK_TOP_CTRL, 0x0008);
	SGTL5000_WriteReg(hi2c, CHIP_PLL_CTRL, 0x6028);
	SGTL5000_WriteReg(hi2c, CHIP_PLL_CTRL, 0x07D0);

	// Routage des Entrées/Sorties
	SGTL5000_WriteReg(hi2c, CHIP_SSS_CTRL, 0x0001);
	SGTL5000_WriteReg(hi2c, CHIP_SSS_CTRL, 0x0003);
	SGTL5000_WriteReg(hi2c, CHIP_ANA_CTRL, 0x0000);
	SGTL5000_WriteReg(hi2c, CHIP_ANA_CTRL, 0x0000);
	SGTL5000_WriteReg(hi2c, CHIP_SSS_CTRL, 0x0000);
	SGTL5000_WriteReg(hi2c, CHIP_ANA_CTRL, 0x0040);
}
