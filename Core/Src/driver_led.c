/*
 * driver_led.c
 *
 *  Created on: Mar 21, 2025
 *      Author: admin
 */

#include "driver_led.h"
#include "main.h"


void MCP23S17_WriteRegister(uint8_t reg, uint8_t data) {
	uint8_t buffer[3];
	buffer[0] = 0x40;  			// Adresse avec bit de commande en écriture
	buffer[1] = reg;            // Registre cible
	buffer[2] = data;           // Donnée à écrire

	HAL_GPIO_WritePin(VU_nCS_GPIO_Port, VU_nCS_Pin, RESET);
	HAL_SPI_Transmit(&hspi3, buffer, 3, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(VU_nCS_GPIO_Port, VU_nCS_Pin, SET);
}


void MCP23S17_ClignoterLED(uint8_t num_LED, uint32_t delay) {
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
    MCP23S17_WriteRegister(0x12, port_A_data);
    MCP23S17_WriteRegister(0x13, port_B_data);
    HAL_Delay(delay);

    // Eteindre la LED
    MCP23S17_WriteRegister(0x12, 0xFF);
    MCP23S17_WriteRegister(0x13, 0xFF);
    HAL_Delay(delay);
}


