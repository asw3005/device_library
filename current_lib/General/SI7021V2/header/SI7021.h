/* Created: 14.04.2019
 * SI7021.h
 * 
 * 
 * 
 */	
#pragma once
#include "stm32f10x.h"

//***************************************************************************************************************************************
//Константы
#define ADDR_SI7021_SHIFTED             0x80                                //7 bit MSB 0x76 (address) + 1 bit LSB 0x00 (read/write)
#define ADDR_SI7021						0x40								//It's not shifted address
#define WRITE_MODE						0x00								//режим записи в ведомое устройство				
#define READ_MODE						0x01								//режим чтения из ведомого устройства

//***************************************************************************************************************************************
//Internal command SI7021
static enum InternalCommandEnum {
	
	MeasureRHHoldMaster					= 0xE5,
	MeasureRHNoHoldMaster				= 0xF5,
	MeasureTHoldMaster					= 0xE3,
	MeasureTNoHoldMaster				= 0xF3,
	ReadTValuePrevious					= 0xE0,
	Reset								= 0xFE,
	WriteRHTUserReg_1					= 0xE6,
	ReadRHTUserReg_1					= 0xE7,
	WriteHeaterCR						= 0x51,
	ReadHeaterCR						= 0x11,
	ReadElectronicIDFirstBytePart1		= 0xFA,
	ReadElectronicIDFirstBytePart2		= 0x0F,
	ReadElectronicIDSecondBytePart1		= 0xFC,
	ReadElectronicIDSecondBytePart2		= 0xC9,
	ReadFirmwareRevisionPart1			= 0x84,
	ReadFirmwareRevisionPart2			= 0xB8,

} InternalCommand;

//***************************************************************************************************************************************
//Address struct 
typedef struct {
	uint8_t AddressSI7021;
	
} AddressStructSI7021;

//***************************************************************************************************************************************
//TempHumPress struct
typedef struct {
	uint32_t SerialNumber;
	uint32_t FirmwareRevision;
	int32_t TemperatureC;
	int32_t TemperatureF;
	uint32_t HumidityRH;
	
} TempHumStruct;

//***************************************************************************************************************************************
//Configuration struct 
typedef struct {
	uint8_t WriteUserRegCmd;
	union {
		uint8_t ControlRegister;
		struct {
			uint8_t RES0	: 1;
			uint8_t RSVD0	: 1;
			uint8_t HTRE	: 1;
			uint8_t RSVD1	: 1;
			uint8_t RSVD2	: 1;
			uint8_t RSVD3	: 1;
			uint8_t VDDS	: 1;
			uint8_t RES1	: 1;
						
		}bitsControlRegister;
	};
	
	uint8_t WriteHeaterRegCmd;
	union {
		uint8_t HeaterControlRegister;
		struct {
			uint8_t Heater	: 4;
			uint8_t RSVD	: 4;
			
		}bitsHeaterControlRegister;
	};
	
	
} ConfigStructSI7021;

//***************************************************************************************************************************************
//Electronic serial number struct receive
typedef struct {
	uint8_t SerialNumber;
	uint8_t FirmwareRevision;
	
} DeviceInfoStruct;

//***************************************************************************************************************************************
//Electronic serial number struct receive
typedef struct {
	uint8_t SNA_3;
	uint8_t SNA_3_CRC;
	uint8_t SNA_2;
	uint8_t SNA_2_CRC;
	uint8_t SNA_1;
	uint8_t SNA_1_CRC;
	uint8_t SNA_0;
	uint8_t SNA_0_CRC;
	uint8_t SNB_3;
	uint8_t SNB_2;
	uint8_t SNB_3_2_CRC;
	uint8_t SNB_1;
	uint8_t SNB_0;
	uint8_t SNB_1_0_CRC;
	
} SerialNumberStructReceive;

//***************************************************************************************************************************************
//
typedef struct {
	uint8_t ReadElectronicIDFirstBytePart1;		
	uint8_t ReadElectronicIDFirstBytePart2;
	uint8_t ReadElectronicIDSecondBytePart1;
	uint8_t ReadElectronicIDSecondBytePart2;
	uint8_t ReadFirmwareRevisionPart1;
	uint8_t ReadFirmwareRevisionPart2;	 
	
} IDRevisionSendCommandStruct;

//***************************************************************************************************************************************
//Data receive parameter
typedef struct {
	uint8_t TemperatureMSB;
	uint8_t TemperatureLSB;
	uint8_t TemperatureCRC8;
	uint8_t HumidityMSB;
	uint8_t HumidityLSB;
	uint8_t HumidityCRC8;
	
} DataReceiveStructSI7021;

//***************************************************************************************************************************************
//Public function prototype	
void getSI7021Hum(void);							//Получить данные о влажности					
void getSI7021Temp(void);							//Получить данные о температуре					
void SI7021UserRegConfig(void);    					//Запись конфигурационных данных 
void SI7021HeaterRegConfig(void);					//Подготовка конфигурационных данных (работа внутри помещения)
void getSI7021DataTempHum(void); 					//Получить данные о температуре и влажности
void getSI7021IDFirstAccess(void);					//Получить ID first part
void getSI7021IDSecondAccess(void);					//Получить ID second part 
void getSI7021FirmwareRevision(void);				//Получить Firmware Revision



