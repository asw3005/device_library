/** Created: 27.05.2018
 ** BME280.h
 **/	
#pragma once
//#ifndef BME280_H_
//#define BME280_H_

#include "stm32l4xx_hal.h"
/**
 ** @brief Number device profiles UNUSED
 **/
//#define NUMBER_DEVICE_PROFILES			0x03

/**
 ** @brief Device address
 **/
#define ADDR_BME280_SHIFTED             0xEC                                //7 bit MSB 0x76 (address) + 1 bit LSB 0x00 (read/write)
#define ADDR_BME280						0x76								//It's not shifted address
#define ADDR_BME280_SHIFTED_ALTERNATIVE 0xEE                                //
#define ADDR_BME280_ALTERNATIVE			0x77								//It's not shifted address
#define WRITE_MODE						0x00								//режим записи в ведомое устройство				
#define READ_MODE						0x01								//режим чтения из ведомого устройства

#define ADDR_BME280_WRITE_MODE	        (ADDR_BME280 | WRITE_MODE)		    //Отправить адрес ведомого устройства, режим запись 
#define ADDR_BME280_READ_MODE			(ADDR_BME280 | READ_MODE)			//Отправить адрес ведомого устройства, режим чтение

/**
 ** @brief Internal registers addresses
 **/
#define CALIB00_START_ADDR              0x88
#define CALIB25_FINISH_ADDR             0xA1
#define ID_ADDR                         0xD0
#define RESET_ADDR                      0xE0
#define CALIB26_START_ADDR              0xE1
#define CALIB41_FINISH_ADDR             0xF0
#define CTRL_HUM_ADDR                   0xF2
#define STATUS_ADDR                     0xF3
#define CTRL_MEAS_ADDR                  0xF4
#define CONFIG_ADDR                     0xF5
#define PRESS_MSB_ADDR                  0xF7
#define PRESS_LSB_ADDR                  0xF8
#define PRESS_XLSB_ADDR                 0xF9
#define TEMP_MSB_ADDR                   0xFA
#define TEMP_LSB_ADDR                   0xFB
#define TEMP_XLSB_ADDR                  0xFC
#define HUM_MSB_ADDR                    0xFD
#define HUM_LSB_ADDR                    0xFE

/**
 ** @brief Software reset
 **/
#define RESET_STATE                     0xB6

/**
 ** @brief Data aquisition control humidity. Default condition is 0x8000.
 **/
//***************************************************************************************************************************************
//Настройка режима сбора данных о влажности CTRL_HUM (в состоянии SKIPPED влажность имеет значение 0x8000)
#define OSRS_H_SKIPPED                  0x00
#define OSRS_H_OVERSAMPLING_1           0x01
#define OSRS_H_OVERSAMPLING_2           0x02
#define OSRS_H_OVERSAMPLING_4           0x03
#define OSRS_H_OVERSAMPLING_8           0x04
#define OSRS_H_OVERSAMPLING_16          0x05

/**
 ** @brief Data aquisition control temperature, pressure, measurement. Default condition is 0x8000
 **/
#define OSRS_P_SKIPPED                  0x00
#define OSRS_P_OVERSAMPLING_1           0x01
#define OSRS_P_OVERSAMPLING_2           0x02
#define OSRS_P_OVERSAMPLING_4           0x03
#define OSRS_P_OVERSAMPLING_8           0x04
#define OSRS_P_OVERSAMPLING_16          0x05

#define OSRS_T_SKIPPED                  0x00
#define OSRS_T_OVERSAMPLING_1           0x01
#define OSRS_T_OVERSAMPLING_2           0x02
#define OSRS_T_OVERSAMPLING_4           0x03
#define OSRS_T_OVERSAMPLING_8           0x04
#define OSRS_T_OVERSAMPLING_16          0x05

#define SLEEP_MODE                      0x00
#define FORCED_MODE                     0x01
//#define FORCED_MODE                     0x02
#define NORMAL_MODE                     0x03

/**
 ** @brief Configuration speed, filter, interface
 **/
#define T_SB_500uS                      0x00
#define T_SB_62500uS                    0x01
#define T_SB_125mS                      0x02
#define T_SB_250mS                      0x03
#define T_SB_500mS                      0x04
#define T_SB_1000mS                     0x05
#define T_SB_10mS                       0x06
#define T_SB_20mS                       0x07

#define FILTER_OFF                      0x00
#define FILTER_COEFF2                   0x01
#define FILTER_COEFF4                   0x02
#define FILTER_COEFF8                   0x03
#define FILTER_COEFF16                  0x04

#define SPI3WIRE_EN                     0x01
#define SPI3WIRE_DIS                    0x00

/**
 ** @brief  
 ** 
 ** 
 **
 **/
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
typedef int8_t(*bme280_communication_fptr)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
typedef void(*bme280_delay_fptr)(uint32_t period);

/**
 ** @brief Device measurement profiles
 **/
typedef enum 
{ 
	WEATHER_MONITORING,
	HUMIDITY_SENSING,
	INDOOR_NAVIGATION,
	GAMING,
	CUSTOM_PROFILE_0,
	
} PROFILES_enum;

/**
 ** @brief Compensated data
 **/
typedef struct {
	int32_t TemperatureC;
	int32_t TemperatureF;
	uint32_t HumidityRH;
	uint32_t PressurePa;
	uint32_t PressuremmHg;
	
} TempHumPressStruct_typedef;

/**
 ** @brief Configuration registers
 **/
typedef struct {
	uint8_t AddressRegisterCtrlHum;
	union {	
		uint8_t DataCtrlHum;
		struct {
			uint8_t   osrs_h			: 3;   
			uint8_t   RESERVED_0		: 5;			
		} bitsDataCtrlHum;				
	};
	uint8_t AddressRegisterCtrlMeas;
	union {
		uint8_t DataCtrlMeas;
		struct {
			uint8_t   mode				: 2;   
			uint8_t   osrs_p			: 3;
			uint8_t   osrs_t			: 3; 
		} bitsDataCtrlMeas;
	};
	uint8_t AddressRegisterConfig;
	union {
		uint8_t DataConfig;
		struct {
			uint8_t   spi3w_en			: 1;   
			uint8_t   RESERVED_1		: 1;
			uint8_t   filter			: 3;   
			uint8_t   t_sb				: 3; 
		} bitsDataConfig;
	};
	
} ConfigStruct_typedef;

/**
 ** @brief Data receive parametr
 **/
typedef struct {
	uint8_t	Ctrl_Hum;		                        
	uint8_t	Status;			                          
	uint8_t	Ctrl_Meas;		                       
	uint8_t	Config;			                          
	uint8_t Reserved_0;	
	uint8_t	Press_msb;		                       
	uint8_t	Press_lsb;		                       
	uint8_t	Press_xlsb_7_4;	                      
	uint8_t	Temp_msb;		                        
	uint8_t	Temp_lsb;		                        
	uint8_t	Temp_xlsb_7_4;	                       
	uint8_t	Hum_msb;			                         
	uint8_t	Hum_lsb;			 
	
} DataReceiveStruct_typedef;

/**
 ** @brief Calibration data
 **/
//typedef struct __attribute__((aligned(1), packed)) {
#pragma pack(push, 1)
typedef struct {
	uint16_t Dig_T1;
	int16_t Dig_T2;
	int16_t Dig_T3;
	uint16_t Dig_P1;
	int16_t Dig_P2;
	int16_t Dig_P3;
	int16_t Dig_P4;
	int16_t Dig_P5;
	int16_t Dig_P6;
	int16_t Dig_P7;
	int16_t Dig_P8;
	int16_t Dig_P9;
	uint8_t Reserved_0;
	uint8_t Dig_H1;
	uint8_t Reserved_1[46];
	uint8_t bmeID;
	uint8_t Reserved_2[15];
	uint8_t Reset;
	int16_t Dig_H2;
	uint8_t Dig_H3;
	int8_t Dig_H4_11_4;		
	uint8_t Dig_H5_H4;
	int8_t Dig_H5_11_4;
	int8_t Dig_H6;
	
} CompensationParameterStorageStruct_typedef;
#pragma pack(pop)

/**
 ** @brief BME280 instance struct
 **/
typedef struct {
	
	uint8_t dev_address;
	uint8_t dev_id;
	ConfigStruct_typedef dev_configuration;
	DataReceiveStruct_typedef dev_uncompensated_data;
	CompensationParameterStorageStruct_typedef dev_calibration_data;
	bme280_communication_fptr read_data_i2c;
	bme280_communication_fptr write_data_i2c;
	bme280_delay_fptr delay;	
	TempHumPressStruct_typedef dev_compensated_data;
	
	
} BME280_typedef;

/**
 ** @brief Public function prototype
 **/
TempHumPressStruct_typedef* getBME280DataPressTempHum(BME280_typedef *dev_bme280); 
void BME280WriteConfig(void); 								//Запись конфигурационных данных  
void getBME280CalibrationData(BME280_typedef *dev_bme280); 
void getBME280CalibrationData0(BME280_typedef *dev_bme280); 
void getBME280CalibrationData1(BME280_typedef *dev_bme280);  
void getBME280ID(BME280_typedef *dev_bme280); 
 
//#endif /* BME280_H_ */


