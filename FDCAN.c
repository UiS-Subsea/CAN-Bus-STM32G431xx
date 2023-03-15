/**
 * @file FDCAN.c
 *
 * @brief FDCAN
 *  
 */

#include "FDCAN.h"
#include "stm32g4xx_hal_fdcan.h"


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
        switch (RxHeader.Identifier) { // Leser ID på motatt melding. Casene må lages ut fra ID-er enn mottar. (Sjekk "Interface-Overview" under "Overordnet prosjekt") 
            case 32: 
                // Kjør kode 
                break; 

            case 33: 
                // Kjør kode 
                break; 

            case 34: 
                // Kjør kode 
                break; 

            case 63: // IKKE FJERN DENNE // Endres til 63 for Regulering, til 95 for Sensor og til 127 for Kraft. 

                memcpy(&TxData, (uint8_t *) &"polo!\n", 6); 

                FDCAN_Send(155, hfdcan); // Endres til 155 for Regulering, til 156 for Sensor, til 157/158/159 for Kraft1/2/3.
                break; 
                //        case 100: // Eksempel som kan fjernes 

                //                flagg = 1; // Kan sette et flagg her nede visst koden som skal kjøres er lang/krevende. 

                //                    break; 
        } 

    } 

} 

void FDCAN_Init(FDCAN_HandleTypeDef *canPort) 
{ 
    // Configure filter for FDCAN1 
    sFilterConfig.IdType = FDCAN_STANDARD_ID; 
    sFilterConfig.FilterIndex = 0; 
    sFilterConfig.FilterType = FDCAN_FILTER_MASK; 
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; 
    sFilterConfig.FilterID1 = 0x00; // FilterID1 fra listen nedenfor settes her. Definerer filterID 
    sFilterConfig.FilterID2 = 0x00; // FilterID2 fra listen nedenfor settes her. Definerer maskeID 

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
    TxHeader.DataLength = FDCAN_DLC_BYTES_8; // Antall byte som sendes 
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE; 
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF; 
    TxHeader.FDFormat = FDCAN_CLASSIC_CAN; // Bruker CAN og ikke FDCAN 
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS; 
    TxHeader.MessageMarker = 0; 

    HAL_FDCAN_ConfigFilter(canPort, &sFilterConfig); 

    HAL_FDCAN_Start(canPort); 

    HAL_FDCAN_ActivateNotification(canPort, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);

} 

void FDCAN_Send(uint16_t id, FDCAN_HandleTypeDef *hfdcan)
{
    TxHeader.Identifier = id; 
    HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &TxHeader, TxData);
}
