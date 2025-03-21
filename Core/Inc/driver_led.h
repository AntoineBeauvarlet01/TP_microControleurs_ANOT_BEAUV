/*
 * driver_led.h
 *
 *  Created on: Mar 21, 2025
 *      Author: admin
 */

#ifndef INC_DRIVER_LED_H_
#define INC_DRIVER_LED_H_

#include <stdio.h>
#include <stdint.h>

void MCP23S17_WriteRegister(uint8_t reg, uint8_t data);
void MCP23S17_ClignoterLED(uint8_t num_LED, uint32_t delay);

#endif /* INC_DRIVER_LED_H_ */
