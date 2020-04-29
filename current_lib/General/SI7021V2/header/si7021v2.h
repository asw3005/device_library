/* Created: 14.04.2019
 * si7021v2.h
 * 
 */	
#pragma once
#include "stm32l4xx_hal.h"

/**
 ** @brief Constants
 **/
#define ADDR_SI7021_SHIFTED             0x80                                //7 bit MSB 0x76 (address) + 1 bit LSB 0x00 (read/write)
#define ADDR_SI7021						0x40								//It's not shifted address
#define WRITE_MODE						0x00								//режим записи в ведомое устройство				
#define READ_MODE						0x01								//режим чтения из ведомого устройства

/**
 ** @brief Internal command SI7021
 **/
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

/**
 ** @brief Device specific function type
 ** 
 ** @param[in] dev_addr : Device address on the I2C bus
 ** @param[in] reg_addr : Register address for device
 ** @param[in] *data : Pointer on data struct instance
 ** @param[in] len : Length of transmition/reception data
 **
 ** @return Result of API execution status
 ** @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 **/
typedef int8_t(*si7021_communication_fptr)(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);
typedef void(*si7021_delay_fptr)(uint32_t period);

/**
 ** @brief TempHumPress struct
 **/
typedef struct {
	uint64_t SerialNumber;
	uint8_t FirmwareRevision;
	int32_t TemperatureC;
	int32_t TemperatureF;
	uint32_t HumidityRH;
	
} TempHumStruct_typedef;

/**
 ** @brief Configuration struct
 **/
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
	
	
} ConfigStructSI7021_typedef;

/**
 ** @brief Electronic serial number struct receive
 **/
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
	
} SerialNumberStructReceive_typedef;

/**
 ** @brief Data receive parameter
 **/
typedef struct {
	uint8_t TemperatureMSB;
	uint8_t TemperatureLSB;
	uint8_t TemperatureCRC8;
	uint8_t HumidityMSB;
	uint8_t HumidityLSB;
	uint8_t HumidityCRC8;
	
} DataReceiveStructSI7021_typedef;

/**
 ** @brief BME280 instance struct
 **/
typedef struct {
	
	uint8_t dev_address;
	uint8_t dev_id;
	uint16_t dev_twin_cmd;
	ConfigStructSI7021_typedef dev_configuration;
	SerialNumberStructReceive_typedef dev_electronic_serial_number;
	DataReceiveStructSI7021_typedef dev_uncompensated_data;
	si7021_communication_fptr read_data_i2c;
	si7021_communication_fptr write_data_i2c;
	si7021_delay_fptr delay;	
	TempHumStruct_typedef dev_compensated_data;
	
	
} SI7021_typedef;

/**
 ** @brief Public function prototype
 **/

//Get temperature
TempHumStruct_typedef* getSI7021Temp(SI7021_typedef *dev_si7021);	
//Get humidity
TempHumStruct_typedef* getSI7021Hum(SI7021_typedef *dev_si7021); 		
//Set configuration register
void setConfugurationSI7021(uint8_t measurement_resolution, uint8_t heater_en, uint8_t heater_value, SI7021_typedef *dev_si7021); 
//Get firmware revision
void getSI7021ElectronicSerialNumber(SI7021_typedef *dev_si7021);





