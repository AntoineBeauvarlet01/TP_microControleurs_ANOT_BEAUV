/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "sai.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "driver_led.h"
#include "sgtl5000.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int chr)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)&chr, 1, HAL_MAX_DELAY);

	return chr;
}


void MCP23S17_Chenillard()
{
	uint16_t idx = 0;

	for(;;) {
		DriverLED_WriteLED(&hDriverLed, idx);
		idx = idx << 1;
		if(idx==0) idx = 1;
		HAL_Delay(500);
	}
}


/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* Configure the peripherals common clocks */
	PeriphCommonClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART2_UART_Init();
	MX_SPI3_Init();
	MX_SAI2_Init();
	MX_I2C2_Init();
	/* USER CODE BEGIN 2 */
	__HAL_SAI_ENABLE(&hsai_BlockA2); // Activation horloge MCLK

	uint8_t chip_id_value = 0;
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c2, CODEC_ADDR, CHIP_ID, I2C_MEMADD_SIZE_16BIT, &chip_id_value, sizeof(chip_id_value), HAL_MAX_DELAY);
	if (status == HAL_OK)
		printf("OK : %d\r\n", chip_id_value); // Affiche 160
	else
		printf("PAS OK : %d\r\n", chip_id_value);

	SGTL5000_Init(&hi2c2);

	//uint8_t sai;
	//HAL_SAI_Receive_DMA(&hsai_BlockA2, &sai, 1);
	//HAL_SAI_Transmit_DMA(&hsai_BlockA2, &sai, 1);


	if(HAL_OK != DriverLED_Init(&hDriverLed, &hspi3, VU_nCS_GPIO_Port, VU_nCS_Pin, VU_nRST_GPIO_Port, VU_nRST_Pin)){
		while(1);
	}

	//uint8_t uart2_chr;
	//HAL_UART_Receive(&huart2, &uart2_chr, 1, HAL_MAX_DELAY); // Pour autoriser la 1ère réception


	//char buffer[5];
	//uint8_t index = 0;
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		// ---------- Ecriture I2C ----------
		uint8_t reg_address = 0x0000;     // Adresse du registre à écrire
		uint16_t data_to_write = 0x1234;  // Données à écrire

		if (SGTL5000_WriteReg(&hi2c2, reg_address, data_to_write) == HAL_OK) {
			printf("Ecriture reussie\r\n");
		} else {
			printf("Erreur ecriture\r\n");
		}
		HAL_Delay(2000);

		// ---------- Q3 ----------
		// HAL_UART_Transmit(&huart2, "Hello world\r\n", 13, 100);

		// ----------Q4 ----------
		//printf("Hello world\r\n");
		//HAL_Delay(1000);

		//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		//HAL_Delay(1000);

		//MCP23S17_Chenillard(500);

		// ---------- DRIVER DE LEDS ----------
		/*
		HAL_UART_Transmit(&huart2, (uint8_t *)&uart2_chr, 1, HAL_MAX_DELAY); // écho
		HAL_UART_Receive(&huart2, &uart2_chr, 1, HAL_MAX_DELAY); // Pour ré-autoriser, à chaque fois, la réception

		if (uart2_chr == '\r') // Si l'utilisateur appuie sur "Entrée"
		{
			index = 0; // Réinitialise l'index pour la prochaine saisie

			uint16_t value_led = (uint16_t)strtol(buffer, NULL, 16);
			DriverLED_WriteLED(&hDriverLed, value_led);
		}
		else
		{
			buffer[index++] = uart2_chr; // Stocke le caractère
			if (index >= sizeof(buffer) - 1) index = 0; // Évite le dépassement de tableau
		}
		*/
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 10;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
void PeriphCommonClock_Config(void)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the peripherals clock
	 */
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SAI2;
	PeriphClkInit.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLLSAI1;
	PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSI;
	PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
	PeriphClkInit.PLLSAI1.PLLSAI1N = 13;
	PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV17;
	PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
