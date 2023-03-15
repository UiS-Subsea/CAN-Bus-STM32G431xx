# CAN buss kommunikasjon

CAN kommunikasjon kode implementeres i FDCAN.c

## Endringer i main.c

Øverst legg til

```c
#include "FDCAN.h"
```
Under `/* USER CODE BEGIN PV */` Legg til

```c

FDCAN_FilterTypeDef sFilterConfig;
FDCAN_TxHeaderTypeDef TxHeader;
FDCAN_RxHeaderTypeDef RxHeader;
uint8_t TxData[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};  // CAN-Bus Transmit Buffer 
uint8_t RxData[8] = {0, 0, 0, 0, 0, 0, 0, 0};                   // CAN-Bus Receive Buffer 

```

I funksjonen `int main(void)` må du legge til følgende før while løkka starter

```c
FDCAN_Init(&hfdcan1);

```


