/* Created: 27.05.2018
 * BME280.h
 * 
 * 
 * 
 */	
 
#ifndef BME280_H_
#define BME280_H_

#include "stm32f10x.h"

//***************************************************************************************************************************************
//Константы
#define ADDR_BME280_SHIFTED             0xEC                                //7 bit MSB 0x76 (address) + 1 bit LSB 0x00 (read/write)
#define ADDR_BME280						0x76								//It's not shifted address
#define WRITE_MODE						0x00								//режим записи в ведомое устройство				
#define READ_MODE						0x01								//режим чтения из ведомого устройства

#define ADDR_BME280_WRITE_MODE	        (ADDR_BME280 | WRITE_MODE)		    //Отправить адрес ведомого устройства, режим запись 
#define ADDR_BME280_READ_MODE			(ADDR_BME280 | READ_MODE)			//Отправить адрес ведомого устройства, режим чтение

//***************************************************************************************************************************************
//Internal registers addresses
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

//***************************************************************************************************************************************
//Состояние СБРОС (остальные коды не имеют воздействия)
#define RESET_STATE                     0xB6

//***************************************************************************************************************************************
//Настройка режима сбора данных о влажности CTRL_HUM (в состоянии SKIPPED влажность имеет значение 0x8000)
#define OSRS_H_SKIPPED                  0x00
#define OSRS_H_OVERSAMPLING_1           0x01
#define OSRS_H_OVERSAMPLING_2           0x02
#define OSRS_H_OVERSAMPLING_4           0x03
#define OSRS_H_OVERSAMPLING_8           0x04
#define OSRS_H_OVERSAMPLING_16          0x05

//***************************************************************************************************************************************
//Настройка режима сбора данных о давлении и температуре, режима работы датчика CTRL_MEAS (в состоянии SKIPPED давление и температура 
//имеют значения 0x8000)
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

//***************************************************************************************************************************************
//Настройка скорости, фильтрации и интерфейса обмена в регистре CONFIG
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

//***************************************************************************************************************************************
//Address struct 
typedef struct {
	uint8_t AddressBME280;
	
} AddressStruct;

//***************************************************************************************************************************************
//TempHumPress struct
typedef struct {
	int32_t ID;
	int32_t TemperatureC;
	int32_t TemperatureF;
	uint32_t HumidityRH;
	uint32_t PressurePa;
	uint32_t PressuremmHg;
	
} TempHumPressStruct;

//***************************************************************************************************************************************
//Configuration struct 
typedef struct {
	uint8_t AddressRegisterCtrlHum;
	union {	
		uint8_t DataCtrlHum;
		struct {
			uint8_t   osrs_h			:3;   
			uint8_t   RESERVED_0		:5;			
		} bitsDataCtrlHum;				
	} DataCtrlHum;
	uint8_t AddressRegisterCtrlMeas;
	union {
		uint8_t DataCtrlMeas;
		struct {
			uint8_t   mode				:2;   
			uint8_t   osrs_p			:3;
			uint8_t   osrs_t			:3; 
		} bitsDataCtrlMeas;
	} DataCtrlMeas;
	uint8_t AddressRegisterConfig;
	union {
		uint8_t DataConfig;
		struct {
			uint8_t   spi3w_en			:1;   
			uint8_t   RESERVED_1		:1;
			uint8_t   filter			:3;   
			uint8_t   t_sb				:3; 
		} bitsDataConfig;
	} DataConfig;
	
} ConfigStruct;

//***************************************************************************************************************************************
//Data receive parameter
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
	
} DataReceiveStruct;

//***************************************************************************************************************************************
//Compensation Parameter Storage
typedef struct __attribute__((aligned(1), packed)) {
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
	
} CompensationParameterStorageStruct;

//***************************************************************************************************************************************
//Public function	
void getBME280DataPressTempHum(void);					//Получить данные о давлении, температуре и влажности
void BME280WriteConfig(void);								//Запись конфигурационных данных  
void getBME280CalibrationData(void); 						//Получить калибровочные данные первый диапазон 
void getBME280CalibrationData0(void);                       //Получить калибровочные данные первый диапазон
void getBME280CalibrationData1(void);                       //Получить калибровочные данные первый диапазон 
void getBME280ID(void);										//Получить ID 

//***************************************************************************************************************************************
//Прототипы функций
static void BME280PreparationConfig(void);                  //Подготовка конфигурационных данных (работа внутри помещения)
static int32_t BME280TemperatureConv(void);                 //Преобразование температуры
static uint32_t BME280HumidityConv(void);                   //Преобразование влажности
static uint32_t BME280PressureConv(void);					//Преобразование давления 
 
#endif /* BME280_H_ */


