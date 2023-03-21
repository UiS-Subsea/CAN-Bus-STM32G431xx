Kode til CAN-bus for STM32G431KB og STM32G431RB. Alt kan klippes og limes inn i "main.c" under de korrekte "USER CODE BEGIN...":



/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <string.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
FDCAN_HandleTypeDef hfdcan1;

UART_HandleTypeDef hlpuart1;

/* USER CODE BEGIN PV */

/* ----------------Startoppsett for CAN-Bus-----------------*/
FDCAN_FilterTypeDef sFilterConfig;
FDCAN_TxHeaderTypeDef TxHeader;
FDCAN_RxHeaderTypeDef RxHeader;
uint8_t TxData[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08}; // CAN-Bus Transmit Buffer
uint8_t RxData[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // CAN-Bus Receive Buffer

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_FDCAN1_Init(void);
void oppstartCAN(FDCAN_HandleTypeDef *canPort);
void sendCAN(uint16_t id, FDCAN_HandleTypeDef *canPort);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* ----------------Funksjoner for CAN-Bus-----------------*/

void oppstartCAN(FDCAN_HandleTypeDef *canPort) {			// Konfigurerer CAN-bussen
		// Configure filter for FDCAN1

		sFilterConfig.IdType = FDCAN_STANDARD_ID;
	  	sFilterConfig.FilterIndex = 0;
	  	sFilterConfig.FilterType = FDCAN_FILTER_MASK;
	  	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	  	sFilterConfig.FilterID1 = 0x00;						// FilterID1 fra listen nedenfor settes her. Definerer filterID
	  	sFilterConfig.FilterID2 = 0x00;						// FilterID2 fra listen nedenfor settes her. Definerer maskeID

	  	/* ---------FilterID1 og FilterID2 for de ulike gruppene--------- */

	  	// Reguleringskort: FilterID1 = 0x20, FilterID2 = 0xE0 --> Slipper gjennom ideer mellom 32-63 (0x20 - 0x3F)
	  	// Sensorskort: FilterID1 = 0x40, FilterID2 = 0xE0 --> Slipper gjennom ideer mellom 64-95 (0x40 - 0x5F)
	  	// Kraftkort: FilterID1 = 0x60, FilterID2 = 0xE0 --> Slipper gjennom ideer mellom 96-127 (0x60 - 0x7F)
	  	// Kommunikasjonskort: FilterID1 = 0x80, FilterID2 = 0xE0 --> Slipper gjennom ideer mellom 128-159 (0x80 - 0x9F)
	  	// Sett inn rett ID-er for din gruppe

	  	// Configure TX Header for FDCAN1
	  	TxHeader.Identifier = 0x00;
	    TxHeader.IdType = FDCAN_STANDARD_ID;
	    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	    TxHeader.DataLength = FDCAN_DLC_BYTES_8;			// Antall byte som sendes
	    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	    TxHeader.FDFormat = FDCAN_CLASSIC_CAN;				// Bruker CAN og ikke FDCAN
	    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	    TxHeader.MessageMarker = 0;

	    HAL_FDCAN_ConfigFilter(canPort, &sFilterConfig);
	    HAL_FDCAN_Start(canPort);
	    HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
}

void sendCAN(uint16_t id, FDCAN_HandleTypeDef *canPort) {	// For sending av melding på CAN-bussen
	TxHeader.Identifier = id;
	HAL_FDCAN_AddMessageToTxFifoQ(canPort, &TxHeader, TxData);
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

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LPUART1_UART_Init();
  MX_FDCAN1_Init();
  /* USER CODE BEGIN 2 */

  /* Starter CAN-buseen */
  oppstartCAN(&hfdcan1);			// FÅ MED DENNE LINJEN!!!

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

////	  Bygger datapaken som skal sendes. Datapakken består av 8 byte.
////	  Koden nedenfor er bare et forslag på hvordan en datapakke kan settes sammen.
//
//// Eksempel med 2x uint8, 1x uint16 og 1x uint32
//	  memcpy(TxData,&førsteuint8, 1); // Kopierer 1 byte fra førsteuint8 til første byte i TxData
//	  memcpy(&TxData[2],&andreuint8, 1); // Kopierer 1 byte fra andreuint8 til andre byte i TxData
//	  memcpy(&TxData[3],&førsteuint16, 2); // Kopierer 2 byte fra førsteuint16 til tredje og fjerde byte i TxData
//	  memcpy(&TxData[5],&førsteuint32, 4); // Kopierer 4 byte fra førsteuint32 til femte, sjette, syvende og åttende byte i TxData
//
//// Eksempel med	4x uint16
//	  memcpy(TxData, &førsteuint16, 2); // Kopierer 2 byte fra førsteuint16 til to første byte i TxData
//	  memcpy(&TxData[2], &andreuint16, 2); // Kopierer 2 byte fra andreuint16 til tredje og fjerde byte i TxData
//	  memcpy(&TxData[4], &tredjeuint16, 2); // Kopierer 2 byte fra tredjeuint16 til femte og sjette byte i TxData
//	  memcpy(&TxData[6], &fjerdeuint16, 2); // Kopierer 2 byte fra fjerdeuint16 til syvende og åttende byte i TxData
//
//// Etter å ha bygget pakken, kan meldingen sendes på bussen ved å kalle opp sendCAN-funksjonen:
//	  sendCAN(ID-på-mottaker, &hfdcan1); //   ID-på-mottaker settes til ID der meldingen skal gå

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_FDCAN;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief FDCAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN1_Init(void)
{

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = ENABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 20;
  hfdcan1.Init.NominalSyncJumpWidth = 1;
  hfdcan1.Init.NominalTimeSeg1 = 14;
  hfdcan1.Init.NominalTimeSeg2 = 2;
  hfdcan1.Init.DataPrescaler = 20;
  hfdcan1.Init.DataSyncJumpWidth = 1;
  hfdcan1.Init.DataTimeSeg1 = 14;
  hfdcan1.Init.DataTimeSeg2 = 2;
  hfdcan1.Init.StdFiltersNbr = 1;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */

  /* USER CODE END FDCAN1_Init 2 */

}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

// FDCAN1 Callback
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
  {
    /* Retreive Rx messages from RX FIFO0 */
    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
    {
    /* Reception Error */
    Error_Handler();
    }

    if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
    {
      /* Notification Error */
      Error_Handler();
    }

    switch (RxHeader.Identifier) {		// Leser ID på motatt melding. Casene må lages ut fra ID-er enn mottar. (Sjekk "Interface-Overview" under "Overordnet prosjekt")

    	case 32:
            // Kjør kode
            break;

        case 33:
            // Kjør kode
            break;

        case 34:
            // Kjør kode
            break;

        case 63: // IKKE FJERN DENNE					// Endres til 63 for Regulering, til 95 for Sensor og til 127 for Kraft.
             memcpy(&TxData, (uint8_t *) &"polo!\n", 6);
             sendCAN(155, hfdcan);							// Endres til 155 for Regulering, til 156 for Sensor, til 157/158/159 for Kraft1/2/3.
             break;
//        case 100:	// Eksempel som kan fjernes
//                	flagg = 1;		// Kan sette et flagg her nede visst koden som skal kjøres er lang/krevende.
//                    break;

    }
  }
}



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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
