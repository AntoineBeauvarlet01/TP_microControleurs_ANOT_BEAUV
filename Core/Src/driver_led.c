/*
 * driver_led.c
 *
 *  Created on: Mar 21, 2025
 *      Author: admin
 */

#include "driver_led.h"

DriverLED_HandleTypeDef hDriverLed;

HAL_StatusTypeDef DriverLED_Init(DriverLED_HandleTypeDef *hDriverLed, SPI_HandleTypeDef* hspi, GPIO_TypeDef* CS_Port, uint16_t CS_Pin, GPIO_TypeDef* nRST_Port, uint16_t nRST_Pin){
	hDriverLed->hspi = hspi;
	hDriverLed->CS_Port = CS_Port;
	hDriverLed->CS_Pin = CS_Pin;
	hDriverLed->nRST_Port = nRST_Port;
	hDriverLed->nRST_Pin = nRST_Pin;

	HAL_StatusTypeDef return_Status = HAL_OK;

	// Init GPIO Expander
	HAL_GPIO_WritePin(hDriverLed->CS_Port, hDriverLed->CS_Pin, SET);
	HAL_GPIO_WritePin(hDriverLed->nRST_Port, hDriverLed->nRST_Pin, SET);
	HAL_Delay(10);

	DriverLED_WriteRegister(hDriverLed, DIR_GPIOA, 0X00); // DIR A as OUTPUT
	DriverLED_WriteRegister(hDriverLed, DIR_GPIOB, 0X00); // DIR B as OUTPUT

	return return_Status;
}

void DriverLED_WriteRegister(DriverLED_HandleTypeDef *hDriverLed, uint8_t reg, uint8_t data) {
	uint8_t buffer[3];
	buffer[0] = 0x40;  			// Adresse avec bit de commande en écriture
	buffer[1] = reg;            // Registre cible
	buffer[2] = data;           // Donnée à écrire

	HAL_GPIO_WritePin(hDriverLed->CS_Port, hDriverLed->CS_Pin, RESET);
	HAL_SPI_Transmit(hDriverLed->hspi, buffer, 3, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(hDriverLed->CS_Port, hDriverLed->CS_Pin, SET);
}


void DriverLED_WriteLED(DriverLED_HandleTypeDef *hDriverLed, uint16_t value) {
    DriverLED_WriteRegister(hDriverLed, ADDR_GPIOA, (uint8_t)~(value & 0x00FF));
    DriverLED_WriteRegister(hDriverLed, ADDR_GPIOB, (uint8_t)~((value & 0xFF00)>>8));
}

