/**
 * @file FDCAN.c
 *
 * @brief FDCAN Headder file
 *  
 */

#ifndef SRC_FDCAN_H_
#define SRC_FDCAN_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "stm32g431xx.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_fdcan.h"


// extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_FilterTypeDef sFilterConfig;
extern FDCAN_TxHeaderTypeDef TxHeader;
extern FDCAN_RxHeaderTypeDef RxHeader;

extern uint8_t TxData[8];
extern uint8_t RxData[8];


void FDCAN_Init(FDCAN_HandleTypeDef *hfdcan);
void FDCAN_Send(uint16_t id, FDCAN_HandleTypeDef *hfdcan);

#endif /* ifndef SRC_FDCAN_H_ */
