/* Created: 27.05.2018
 * BME280.c
 *  
 * 
 * 
 */	

#include "stm32f10x.h"
#include "BME280.h"
#include "I2C1.h"


TempHumPressStruct TempHumPressBME280;

static int32_t t_fine;
static AddressStruct bme280Address;
static ConfigStruct bme280ConfigStruct;
static DataReceiveStruct bme280DataReceive;
static CompensationParameterStorageStruct bme280CompParamReceived;

//***************************************************************************************************************************************
//Private function prototype
static void BME280PreparationConfig(void);                   //Подготовка конфигурационных данных (работа внутри помещения)
static int32_t BME280TemperatureConv(void);                  //Преобразование температуры
static uint32_t BME280HumidityConv(void);                    //Преобразование влажности
static uint32_t BME280PressureConv(void); 					//Преобразование давления 


//**********************************************************************************************************************************
//Подготовка конфигурационных данных (работа внутри помещения)
void BME280PreparationConfig(void)                              
{
	bme280Address.AddressBME280 = ADDR_BME280_WRITE_MODE;
	bme280ConfigStruct.AddressRegisterCtrlHum = CTRL_HUM_ADDR;
	bme280ConfigStruct.bitsDataCtrlHum.osrs_h = OSRS_H_OVERSAMPLING_1;
	
	bme280ConfigStruct.AddressRegisterCtrlMeas = CTRL_MEAS_ADDR;
	bme280ConfigStruct.bitsDataCtrlMeas.mode = NORMAL_MODE;
	bme280ConfigStruct.bitsDataCtrlMeas.osrs_p = OSRS_P_OVERSAMPLING_16;
	bme280ConfigStruct.bitsDataCtrlMeas.osrs_t = OSRS_T_OVERSAMPLING_2;
	
	bme280ConfigStruct.AddressRegisterConfig = CONFIG_ADDR;
	bme280ConfigStruct.bitsDataConfig.spi3w_en = SPI3WIRE_DIS;
	bme280ConfigStruct.bitsDataConfig.t_sb = T_SB_500mS;
	bme280ConfigStruct.bitsDataConfig.filter = FILTER_COEFF16; 
}

//**********************************************************************************************************************************
//Запись конфигурационных данных 
void BME280WriteConfig(void)                              
{
	BME280PreparationConfig();
	TransmitI2C1DataISR(ADDR_BME280, (uint8_t *)(&bme280ConfigStruct), sizeof(bme280ConfigStruct));
}

//**********************************************************************************************************************************
//Получить данные о давлении, температуре и влажности					
void getBME280DataPressTempHum(void)                              
{
	static uint8_t PressMsbAddr = CTRL_HUM_ADDR;	// PRESS_MSB_ADDR;
	static uint8_t flagID = 0;
	
	if (!flagID)
		{
			TempHumPressBME280.ID = bme280CompParamReceived.bmeID;
			flagID = 1;
		}

	TempHumPressBME280.TemperatureC = BME280TemperatureConv();
	TempHumPressBME280.TemperatureF = ((BME280TemperatureConv() * 18) + 32000) / 10;
	TempHumPressBME280.HumidityRH = (BME280HumidityConv() * 1000) / 1024;
	TempHumPressBME280.PressurePa = BME280PressureConv();
	TempHumPressBME280.PressuremmHg = (((uint64_t)TempHumPressBME280.PressurePa * 1000000)) / 133322;
	
	ReceiveI2C1DataISR(ADDR_BME280, &PressMsbAddr, (uint8_t *)(&bme280DataReceive), sizeof(bme280DataReceive));
	
	//return (uint8_t *) &TempHumPress;
}

//**********************************************************************************************************************************
//Получить калибровочные данные первый диапазон 
void getBME280CalibrationData(void)                              
{
	//write receive's register address to bme280 Config Struct
	static uint8_t Calib00StartAddr = CALIB00_START_ADDR;	
	ReceiveI2C1DataISR(ADDR_BME280, &Calib00StartAddr, (uint8_t *)&bme280CompParamReceived, sizeof(bme280CompParamReceived));
}

//**********************************************************************************************************************************
//Получить ID 
void getBME280ID(void)                              
{
	//write receive's register address to bme280 Config Struct
	static uint8_t Calib26StartAddr = ID_ADDR;
	ReceiveI2C1DataISR(ADDR_BME280, &Calib26StartAddr, (uint8_t *)(&bme280CompParamReceived.bmeID), 1);
}

//**********************************************************************************************************************************
//Получить калибровочные данные первый диапазон 
void getBME280CalibrationData0(void)                              
{
	//write receive's register address to bme280 Config Struct
	static uint8_t Calib00StartAddr = CALIB00_START_ADDR;
	ReceiveI2C1DataISR(ADDR_BME280, &Calib00StartAddr, (uint8_t *)(&bme280CompParamReceived), 26);
}

//**********************************************************************************************************************************
//Получить калибровочные данные первый диапазон 
void getBME280CalibrationData1(void)                              
{
	//write receive's register address to bme280 Config Struct
	static uint8_t Calib26StartAddr = CALIB26_START_ADDR;
	ReceiveI2C1DataISR(ADDR_BME280, &Calib26StartAddr, (uint8_t *)(&bme280CompParamReceived.Dig_H2), 7);
}

//**********************************************************************************************************************************
//Преобразование давления 
// Returns pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa = 963.86 hPa
static uint32_t BME280PressureConv(void)                              
{
	int32_t var1, var2, var3, var4;  
	int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
	uint16_t dig_P1;
	uint32_t P, adc_P, P_min = 30000, P_max = 110000, var5;
  
	dig_P1 = bme280CompParamReceived.Dig_P1;
	dig_P2 = bme280CompParamReceived.Dig_P2;
	dig_P3 = bme280CompParamReceived.Dig_P3;
	dig_P4 = bme280CompParamReceived.Dig_P4;
	dig_P5 = bme280CompParamReceived.Dig_P5;
	dig_P6 = bme280CompParamReceived.Dig_P6;
	dig_P7 = bme280CompParamReceived.Dig_P7;
	dig_P8 = bme280CompParamReceived.Dig_P8;
	dig_P9 = bme280CompParamReceived.Dig_P9;
	
	adc_P = ((((bme280DataReceive.Press_msb << 8) | bme280DataReceive.Press_lsb) << 8) | bme280DataReceive.Press_xlsb_7_4) >> 4;

	var1 = (((int32_t)t_fine) / 2) - (int32_t)64000;
	var2 = (((var1 / 4) * (var1 / 4)) / 2048) * ((int32_t)dig_P6);
	var2 = var2 + ((var1 * ((int32_t)dig_P5)) * 2);
	var2 = (var2 / 4) + (((int32_t)dig_P4) * 65536);
	var3 = (dig_P3 * (((var1 / 4) * (var1 / 4)) / 8192)) / 8;
	var4 = (((int32_t)dig_P2) * var1) / 2;
	var1 = (var3 + var4) / 262144;
	var1 = (((32768 + var1)) * ((int32_t)dig_P1)) / 32768;
	 /* avoid exception caused by division by zero */
	if (var1) 
    {
      var5 = (uint32_t)((uint32_t)1048576) - adc_P;
      P = ((uint32_t)(var5 - (uint32_t)(var2 / 4096))) * 3125;
      if (P < 0x80000000)        
        {
          P = (P << 1) / ((uint32_t)var1);
        }
      else
        {
          P = (P / (uint32_t)var1) * 2;
        }
      var1 = (((int32_t)dig_P9) * ((int32_t)(((P / 8) * (P / 8)) / 8192))) / 4096;
      var2 = (((int32_t)(P / 4)) * ((int32_t)dig_P8)) / 8192;
      P = (uint32_t)((int32_t)P + ((var1 + var2 + dig_P7) / 16));

      if (P < P_min){P = P_min;}       
      else 
        {
         if (P > P_max)
            {P = P_max;}
        }
    } 
	else 
	{
		P = P_min;
	}

	return P;
}

//**********************************************************************************************************************************
//Преобразование температуры 
//Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
//t_fine carries fine temperature as global value
static int32_t BME280TemperatureConv(void)                              
{ 
	int32_t var1, var2, T, T_min = -4000, T_max = 8500;
	uint32_t adc_T;
	uint16_t dig_T1;
	int16_t dig_T2, dig_T3;
  
	dig_T1 = bme280CompParamReceived.Dig_T1;
	dig_T2 = bme280CompParamReceived.Dig_T2;
	dig_T3 = bme280CompParamReceived.Dig_T3;
	
	adc_T = ((((bme280DataReceive.Temp_msb << 8) | bme280DataReceive.Temp_lsb) << 8) | bme280DataReceive.Temp_xlsb_7_4) >> 4;
	
	var1 = (int32_t)((adc_T / 8) - ((int32_t)dig_T1 * 2));
	var1 = (var1 * ((int32_t)dig_T2)) / 2048;
	var2 = (int32_t)((adc_T / 16) - ((int32_t)dig_T1));
	var2 = (((var2 * var2) / 4096) * ((int32_t)dig_T3)) / 16384;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) / 256;

	if (T < T_min)
    {T = T_min;}
	else if (T > T_max)
    {T = T_max;}
  
	return T;
}

//**********************************************************************************************************************************
//Преобразование влажности 
//Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
//Output value of “47445” represents 47445/1024 = 46.333 %RH
static uint32_t BME280HumidityConv(void)                              
{
	int32_t  var1, var2, var3, var4, var5;
	int16_t  dig_H2, dig_H4, dig_H5, dig_H6;
	uint16_t dig_H1, dig_H3;
	uint32_t adc_H, Hum, Hum_max = 102400; 
  
	dig_H1 = bme280CompParamReceived.Dig_H1;
	dig_H2 = bme280CompParamReceived.Dig_H2;
	dig_H3 = bme280CompParamReceived.Dig_H3;
	dig_H4 = (bme280CompParamReceived.Dig_H4_11_4 << 4) | (0x0F & bme280CompParamReceived.Dig_H5_H4);
	dig_H5 = (bme280CompParamReceived.Dig_H5_11_4 << 4) | ((0xF0 & bme280CompParamReceived.Dig_H5_H4) >> 4);
	dig_H6 = bme280CompParamReceived.Dig_H6;
	
	adc_H = (bme280DataReceive.Hum_msb << 8) | bme280DataReceive.Hum_lsb;

	var1 = t_fine - ((int32_t)76800);
	var2 = (int32_t)(adc_H * 16384);
	var3 = (int32_t)(((int32_t)dig_H4) * 1048576);
	var4 = ((int32_t)dig_H5) * var1;
	var5 = (((var2 - var3) - var4) + (int32_t)16384) / 32768;
	var2 = (var1 * ((int32_t)dig_H6)) / 1024;
	var3 = (var1 * ((int32_t)dig_H3)) / 2048;
	var4 = ((var2 * (var3 + (int32_t)32768)) / 1024) + (int32_t)2097152;
	var2 = ((var4 * ((int32_t)dig_H2)) + 8192) / 16384;
	var3 = var5 * var2;
	var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
	var5 = var3 - ((var4 * ((int32_t)dig_H1)) / 16);
	var5 = (var5 < 0 ? 0 : var5);
	var5 = (var5 > 419430400 ? 419430400 : var5);
	Hum = (uint32_t)(var5 / 4096);
	if (Hum > Hum_max){Hum = Hum_max;}

	return Hum;
}




