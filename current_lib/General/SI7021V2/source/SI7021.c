/* Created: 14.04.2019
 * SI7021.c
 * 
 * 
 * 
 */

#include "SI7021.h"
#include "stm32f10x.h"
#include "I2C1.h"

TempHumStruct TempHumSI7021;
//DeviceInfoStruct DeviceInfo;
static SerialNumberStructReceive SerialNumberReceive;
static AddressStructSI7021 SI7021Address;
static ConfigStructSI7021 SI7021ConfigStruct;
static DataReceiveStructSI7021 SI7021DataReceive;

//***************************************************************************************************************************************
//Private function prototype
static uint32_t SI7021HumidityConv(void); 			//Returns humidity in %RH 
static int32_t SI7021TemperatureConv(void); 			//Returns temperature in DegC


//**********************************************************************************************************************************
//Подготовка конфигурационных данных (работа внутри помещения)
void SI7021UserRegConfig(void)                              
{	
	SI7021ConfigStruct.WriteUserRegCmd = WriteRHTUserReg_1;
	SI7021ConfigStruct.bitsControlRegister.RES0 = 0;
	SI7021ConfigStruct.bitsControlRegister.RES1 = 0;
	SI7021ConfigStruct.bitsControlRegister.HTRE = 0;
	
	TransmitI2C1DataISR(ADDR_SI7021, (uint8_t *)(&SI7021ConfigStruct.WriteUserRegCmd), sizeof(SI7021ConfigStruct) - 2);
}

//**********************************************************************************************************************************
//Подготовка конфигурационных данных (работа внутри помещения)
void SI7021HeaterRegConfig(void)                              
{	
	SI7021ConfigStruct.WriteUserRegCmd = WriteHeaterCR;
	SI7021ConfigStruct.bitsHeaterControlRegister.Heater = 0;

	TransmitI2C1DataISR(ADDR_SI7021, (uint8_t *)(&SI7021ConfigStruct.WriteHeaterRegCmd), sizeof(SI7021ConfigStruct) - 2);
}

//**********************************************************************************************************************************
//Получить ID first part
void getSI7021IDFirstAccess(void)                              
{
	//write receive's register address to  Config Struct
	static uint8_t IDFirst[2] = { ReadElectronicIDFirstBytePart1, ReadElectronicIDFirstBytePart2 };
	
	ReceiveI2C1DataQSelectISR(ADDR_SI7021, (uint8_t *)&IDFirst, (uint8_t *)(&SerialNumberReceive.SNA_3), sizeof(SerialNumberReceive) - 6, 2);
}

//**********************************************************************************************************************************
//Получить ID second part
void getSI7021IDSecondAccess(void)                              
{
	//write receive's register address to  Config Struct
	static uint8_t IDSecond[2] = { ReadElectronicIDSecondBytePart1, ReadElectronicIDSecondBytePart2 };
	
	ReceiveI2C1DataQSelectISR(ADDR_SI7021, (uint8_t *)&IDSecond, (uint8_t *)(&SerialNumberReceive.SNB_3), sizeof(SerialNumberReceive) - 8, 2);
}

//**********************************************************************************************************************************
//Получить Firmware Revision
void getSI7021FirmwareRevision(void)                              
{
	//write receive's register address to  firmware revision Struct
	static uint8_t FirmwareRevision[2] = { ReadFirmwareRevisionPart1, ReadFirmwareRevisionPart2 };
	
	TempHumSI7021.SerialNumber = SerialNumberReceive.SNB_3;
	ReceiveI2C1DataQSelectISR(ADDR_SI7021, (uint8_t *)&FirmwareRevision, (uint8_t *)(&TempHumSI7021.FirmwareRevision), sizeof(FirmwareRevision) - 1, 2);
}

//**********************************************************************************************************************************
//Получить данные о влажности					
void getSI7021Hum(void)                              
{
	static uint8_t HumCmdMeasure = MeasureRHHoldMaster; 	// PRESS_MSB_ADDR;
	
	TempHumSI7021.HumidityRH = SI7021HumidityConv();

	ReceiveI2C1DataISR(ADDR_SI7021, &HumCmdMeasure, (uint8_t *)(&SI7021DataReceive.HumidityMSB), sizeof(SI7021DataReceive) - 3);
}

//**********************************************************************************************************************************
//Получить данные о температуре					
void getSI7021Temp(void)                              
{	
	static uint8_t TempCmdMeasure = MeasureTHoldMaster; 	// PRESS_MSB_ADDR;
	
	TempHumSI7021.TemperatureC = SI7021TemperatureConv() / 10;
	TempHumSI7021.TemperatureF = ((SI7021TemperatureConv() * 18) + 320000) / 100;
	
	ReceiveI2C1DataISR(ADDR_SI7021, &TempCmdMeasure, (uint8_t *)(&SI7021DataReceive.TemperatureMSB), sizeof(SI7021DataReceive) - 3);
}

//**********************************************************************************************************************************
//Преобразование температуры 
//Returns temperature in DegC
static int32_t SI7021TemperatureConv(void)                              
{ 
	uint16_t TempCode;
	float Temperature;
	
	TempCode = (SI7021DataReceive.TemperatureMSB << 8 | SI7021DataReceive.TemperatureLSB);
	
	Temperature = ((175.72f * TempCode) / 65536) - 46.85f;
	
	return (Temperature * 1000);
}

//**********************************************************************************************************************************
//Преобразование влажности 
//Returns humidity in %RH 
static uint32_t SI7021HumidityConv(void)                              
{
	uint32_t RH_Code;
	float RelativeHumidity;
	
	RH_Code = (SI7021DataReceive.HumidityMSB << 8 | SI7021DataReceive.HumidityLSB);
	
	RelativeHumidity = ((125.0f * RH_Code) / 65536) - 6;
	if (RelativeHumidity > 100)
		{
			RelativeHumidity = 100;
		}
	if (RelativeHumidity < 0)
		{
			RelativeHumidity = 0;
		}
	
	return RelativeHumidity * 1000;
}
