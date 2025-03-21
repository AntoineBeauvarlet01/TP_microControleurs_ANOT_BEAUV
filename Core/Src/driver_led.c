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

	HAL_GPIO_WritePin(hDriverLed->CS_Port, hDriverLed->CS_Pin, SET);
	HAL_GPIO_WritePin(hDriverLed->nRST_Port, hDriverLed->nRST_Pin, SET);
	HAL_Delay(10);

	DriverLED_WriteRegister(hDriverLed, 0X00, 0X00); // DIR A as OUTPUT
	DriverLED_WriteRegister(hDriverLed, 0X01, 0X00); // DIR B as OUTPUT

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
    DriverLED_WriteRegister(hDriverLed, 0x12, (uint8_t)~(value & 0x00FF));
    DriverLED_WriteRegister(hDriverLed, 0x13, (uint8_t)~((value & 0xFF00)>>8));
}



void DriverLED_ClignoterLED(DriverLED_HandleTypeDef *hDriverLed, uint8_t num_LED, uint32_t delay) {
    uint8_t port_A_data = 0xFF; // Toutes les LEDs éteintes par défaut
    uint8_t port_B_data = 0xFF;

    switch (num_LED) {
        case 1:  port_A_data &= ~(1 << 0); break;
        case 2:  port_A_data &= ~(1 << 1); break;
        case 3:  port_A_data &= ~(1 << 2); break;
        case 4:  port_A_data &= ~(1 << 3); break;
        case 5:  port_A_data &= ~(1 << 4); break;
        case 6:  port_A_data &= ~(1 << 5); break;
        case 7:  port_A_data &= ~(1 << 6); break;
        case 8:  port_A_data &= ~(1 << 7); break;
        case 9:  port_B_data &= ~(1 << 0); break;
        case 10: port_B_data &= ~(1 << 1); break;
        case 11: port_B_data &= ~(1 << 2); break;
        case 12: port_B_data &= ~(1 << 3); break;
        case 13: port_B_data &= ~(1 << 4); break;
        case 14: port_B_data &= ~(1 << 5); break;
        case 15: port_B_data &= ~(1 << 6); break;
        case 16: port_B_data &= ~(1 << 7); break;
        default: break; //print("N° DE PIN INVALIDE (1-16)\r\n"); break;
    }

    // Allumer la LED spécifiée
    DriverLED_WriteRegister(hDriverLed, 0x12, port_A_data);
    DriverLED_WriteRegister(hDriverLed, 0x13, port_B_data);
    HAL_Delay(delay);

    // Eteindre la LED
    DriverLED_WriteRegister(hDriverLed, 0x12, 0xFF);
    DriverLED_WriteRegister(hDriverLed, 0x13, 0xFF);
    HAL_Delay(delay);
}


