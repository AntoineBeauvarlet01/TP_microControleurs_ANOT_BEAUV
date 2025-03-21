/*
 * driver_led.h
 *
 *  Created on: Mar 21, 2025
 *      Author: admin
 */

#ifndef INC_DRIVER_LED_H_
#define INC_DRIVER_LED_H_

#include "main.h"

typedef struct __DriverLED_HandleTypeDef
{
	SPI_HandleTypeDef* hspi;
	GPIO_TypeDef* CS_Port;
	uint16_t CS_Pin;
	GPIO_TypeDef* nRST_Port;
	uint16_t nRST_Pin;
	uint8_t led[2];
} DriverLED_HandleTypeDef;

extern DriverLED_HandleTypeDef hDriverLed;

HAL_StatusTypeDef DriverLED_Init(DriverLED_HandleTypeDef *hDriverLed, SPI_HandleTypeDef *hspi, GPIO_TypeDef* CS_Port, uint16_t CS_Pin, GPIO_TypeDef* nRST_Port, uint16_t nRST_Pin);
void DriverLED_WriteRegister(DriverLED_HandleTypeDef *hDriverLed, uint8_t reg, uint8_t data);
void DriverLED_ClignoterLED(DriverLED_HandleTypeDef *hDriverLed, uint8_t num_LED, uint32_t delay);
void DriverLED_WriteLED(DriverLED_HandleTypeDef *hDriverLed, uint16_t value);


#endif /* INC_DRIVER_LED_H_ */
