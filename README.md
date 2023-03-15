# CAN-Bus-STM32G431xx
Kode og oppsett for CAN-bus på STM32G431KB og STM32G431RB

Det er to alternative måter og implementere kode for CAN-bus:

#1 Kode for CAN-bus i main.c:
Lim inn kode i main.c fra "Kode CAN-bus.c", https://github.com/UiS-Subsea/CAN-Bus-STM32G431xx/blob/main/Kode%20CAN-bus.c

#2 Kode for CAN-bus i main.c:
For å få CAN-bus i egen fil, sjå branch `syslak`, https://github.com/UiS-Subsea/CAN-Bus-STM32G431xx/tree/syslak



KODEN NEDENFOR ER ET EKSEMPEL PÅ HVORDAN EN MELDING KAN LAGES FOR DERETTER Å SENDE DEN: 

For å sende data: 

Endre i datapakken TxData og kall opp sendCAN med ID meldingen skal til. 

//   Bygger datapaken som skal sendes. Datapakken består av 8 byte. 
//   Koden nedenfor er bare et forslag på hvordan en datapakke kan settes sammen. 

// Eksempel med 2x uint8, 1x uint16 og 1x uint32 
  memcpy(TxData,&førsteuint8, 1); // Kopierer 1 byte fra førsteuint8 til første byte i TxData 
  memcpy(&TxData[2],&andreuint8, 1); // Kopierer 1 byte fra andreuint8 til andre byte i TxData 
  memcpy(&TxData[3],&førsteuint16, 2); // Kopierer 2 byte fra førsteuint16 til tredje og fjerde byte i TxData 
  memcpy(&TxData[5],&førsteuint32, 4); // Kopierer 4 byte fra førsteuint32 til femte, sjette, syvende og åttende byte i TxData 
  
// Eksempel med 4x uint16 
  memcpy(TxData, &førsteuint16, 2); // Kopierer 2 byte fra førsteuint16 til to første byte i TxData 
  memcpy(&TxData[2], &andreuint16, 2); // Kopierer 2 byte fra andreuint16 til tredje og fjerde byte i TxData 
  memcpy(&TxData[4], &tredjeuint16, 2); // Kopierer 2 byte fra tredjeuint16 til femte og sjette byte i TxData 
  memcpy(&TxData[6], &fjerdeuint16, 2); // Kopierer 2 byte fra fjerdeuint16 til syvende og åttende byte i TxData 

// Etter å ha bygget pakken, kan meldingen sendes på bussen ved å kalle opp sendCAN-funksjonen: 
  sendCAN(ID-på-mottaker, &hfdcan1); //   ID-på-mottaker settes til ID der meldingen skal gå 

    /* USER CODE END WHILE */ 

 

 
For å motta data: 

Endre ID-ene i casene i USER CODE 4 til ID-er du ønsker og motta frå. 
