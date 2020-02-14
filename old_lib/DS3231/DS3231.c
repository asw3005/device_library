/* Created: 27.11.2014 12:35:55
 * DS3231.c
 *
 * 
 *
 */ 

#include "stm32f10x.h"
#include "DS3231.h"
#include "DS3231Variable.h"
#include "i2c2.h"

uint8_t QuantityByteI2C2Tx;
uint8_t QuantityByteI2C2Rx;
uint8_t CounterByteI2C2Tx;
uint8_t CounterByteI2C2Rx;
uint8_t FlagConfigEnI2C2;
uint8_t FlagWriteDataI2C2;
uint8_t FlagReadDataI2C2;
uint8_t FlagTxEnableI2C2;
uint8_t FlagRxEnableI2C2;
uint8_t FlagRestartEnI2C2;
uint8_t FlagSwitchI2C2;
uint8_t RxBufferAddrI2C2;
uint8_t TxBufferI2C2[20];
uint8_t RxBufferI2C2[20];
uint8_t *PointerTxBufferI2C2;
uint8_t *PointerRxBufferI2C2;
uint8_t FlagTim2CmpI2C2;


//**********************************************************************************************************************************
//Подготовка конфигурационных данных (работа внутри помещения)
void DS3231PreparationConfig(void)                              
{
  //uint8_t *pointer_0;
  

}

//**********************************************************************************************************************************
//Настройка DS3231
void DS3231WriteConfig(void)                              
{
  FlagTxEnableI2C2 = 1;
  FlagWriteDataI2C2 = 1;
  FlagSwitchI2C2 = 1;
  QuantityByteI2C2Tx = 4;
  TxBufferI2C2[AddressControl] = DS3231_CONTROL_ADDR;
  DS3231WriteAddress(WRITE_MODE);
  //I2C1_Enable();
  I2C2_Start();
  
}

//**********************************************************************************************************************************
//Настройка времени
void DS3231WriteTime(void)                              
{
  FlagTxEnableI2C2 = 1;
  FlagWriteDataI2C2 = 1;
  FlagSwitchI2C2 = 1;
  QuantityByteI2C2Tx = 14;
  TxBufferI2C2[AddressSeconds] = DS3231_SECONDS_ADDR;
  DS3231WriteAddress(WRITE_MODE);
  //I2C1_Enable();
  I2C2_Start();
  
}

//**********************************************************************************************************************************
//Получить данные DS3231 
void getDS3231Data0(void)                              
{
  FlagTxEnableI2C2 = 1;
  FlagRxEnableI2C2 = 1;
  FlagRestartEnI2C2 = 1;
  FlagWriteDataI2C2 = 1;
  FlagReadDataI2C2 = 1;
  QuantityByteI2C2Tx = 1;
  QuantityByteI2C2Rx = 18;
  RxBufferAddrI2C2 = SECONDS;                     //Адрес заполнения буфера приёма
  TxBufferI2C2[1] = DS3231_SECONDS_ADDR;          //
  DS3231WriteAddress(WRITE_MODE);
  //I2C1_Enable();
  I2C2_Start();
}

//**********************************************************************************************************************************
//Получить данные о температуре
void getDS3231DataTemp(void)                              
{
  FlagTxEnableI2C2 = 1;
  FlagRxEnableI2C2 = 1;
  FlagRestartEnI2C2 = 1;
  FlagWriteDataI2C2 = 1;
  FlagReadDataI2C2 = 1;
  QuantityByteI2C2Tx = 1;
  QuantityByteI2C2Rx = 2;
  RxBufferAddrI2C2 = MSB_OF_TEMP;
  TxBufferI2C2[1] = DS3231_MSB_OF_TEMP_ADDR;
  DS3231WriteAddress(WRITE_MODE);
  //I2C1_Enable();
  I2C2_Start();
}



//**********************************************************************************************************************************
//Отправка адреса устройства 
void DS3231WriteAddress(uint8_t RxTxCondition)
{ 
  if (RxTxCondition == 0) {TxBufferI2C2[AddressDS3231] = ADDR_DS3231_WRITE_MODE;}    
  else                    {TxBufferI2C2[AddressDS3231] = ADDR_DS3231_READ_MODE;}
}



