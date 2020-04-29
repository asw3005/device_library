/* Created: 14.04.2019
 * si7021v2.c
 * 
 * 
 * 
 */

#include "si7021v2.h"
#include "stm32l4xx_hal.h"

/**
 ** @brief Private function prototype
 **/
static int32_t SI7021TemperatureConv(SI7021_typedef *dev_si7021); 
static uint32_t SI7021HumidityConv(SI7021_typedef *dev_si7021);  

/**
 ** @brief Get temperature
 **/				
TempHumStruct_typedef* getSI7021Temp(SI7021_typedef *dev_si7021)                              
{	
	dev_si7021->read_data_i2c(dev_si7021->dev_address, MeasureTHoldMaster, 1, &dev_si7021->dev_uncompensated_data.TemperatureMSB, 3);
	dev_si7021->delay(15);
	dev_si7021->dev_compensated_data.TemperatureC = SI7021TemperatureConv(dev_si7021) / 10;
	dev_si7021->dev_compensated_data.TemperatureF = ((dev_si7021->dev_compensated_data.TemperatureC * 18 * 10) + 320000) / 100;
	return &dev_si7021->dev_compensated_data;
}

/**
 ** @brief Get humidity
 **/					
TempHumStruct_typedef* getSI7021Hum(SI7021_typedef *dev_si7021)                              
{
	dev_si7021->read_data_i2c(dev_si7021->dev_address, MeasureRHHoldMaster, 1, &dev_si7021->dev_uncompensated_data.HumidityMSB, 3);
	dev_si7021->delay(15);
	dev_si7021->dev_compensated_data.HumidityRH = SI7021HumidityConv(dev_si7021);
	return &dev_si7021->dev_compensated_data;
}

/**
 ** @brief Set configuration register
 **/
void setConfugurationSI7021(uint8_t measurement_resolution, uint8_t heater_en, uint8_t heater_value, SI7021_typedef *dev_si7021)                              
{	
	dev_si7021->dev_configuration.bitsControlRegister.RES0 = (measurement_resolution & 0x01);
	dev_si7021->dev_configuration.bitsControlRegister.RES1 = (measurement_resolution & 0x02);
	dev_si7021->dev_configuration.bitsControlRegister.HTRE = heater_en;	/// On-chip heater disable
	
	if(heater_en)
	{
		dev_si7021->dev_configuration.HeaterControlRegister = heater_value;
		dev_si7021->write_data_i2c(dev_si7021->dev_address, WriteHeaterCR, 1, &dev_si7021->dev_configuration.HeaterControlRegister, 1);
		dev_si7021->delay(1);
	}
	
	dev_si7021->write_data_i2c(dev_si7021->dev_address, WriteRHTUserReg_1, 1, &dev_si7021->dev_configuration.ControlRegister, 1);
	dev_si7021->delay(1);	
}

/**
 ** @brief Get firmware revision
 **/
void getSI7021ElectronicSerialNumber(SI7021_typedef *dev_si7021)                              
{
	//write receive's register address to  firmware revision Struct
	dev_si7021->dev_twin_cmd = ((uint16_t)ReadElectronicIDFirstBytePart1) << 8;
	dev_si7021->dev_twin_cmd |= ReadElectronicIDFirstBytePart2;
	
	dev_si7021->read_data_i2c(dev_si7021->dev_address, dev_si7021->dev_twin_cmd, 2, &dev_si7021->dev_electronic_serial_number.SNA_3, 8);
	dev_si7021->delay(1);
	
	dev_si7021->dev_twin_cmd = ((uint16_t)ReadElectronicIDSecondBytePart1) << 8;
	dev_si7021->dev_twin_cmd |= ReadElectronicIDSecondBytePart2;
	
	dev_si7021->read_data_i2c(dev_si7021->dev_address, dev_si7021->dev_twin_cmd, 2, &dev_si7021->dev_electronic_serial_number.SNB_3, 6);
	dev_si7021->delay(1);
	
	dev_si7021->dev_twin_cmd = ((uint16_t)ReadFirmwareRevisionPart1) << 8;
	dev_si7021->dev_twin_cmd |= ReadFirmwareRevisionPart2;
	
	dev_si7021->read_data_i2c(dev_si7021->dev_address, dev_si7021->dev_twin_cmd, 2, &dev_si7021->dev_compensated_data.FirmwareRevision, 1);
	dev_si7021->delay(1);	
	
	dev_si7021->dev_compensated_data.SerialNumber = ((uint64_t)dev_si7021->dev_electronic_serial_number.SNA_3) << 56;
	dev_si7021->dev_compensated_data.SerialNumber |= ((uint64_t)dev_si7021->dev_electronic_serial_number.SNA_2) << 48;
	dev_si7021->dev_compensated_data.SerialNumber |= ((uint64_t)dev_si7021->dev_electronic_serial_number.SNA_1) << 40;
	dev_si7021->dev_compensated_data.SerialNumber |= ((uint64_t)dev_si7021->dev_electronic_serial_number.SNA_0) << 32;
	dev_si7021->dev_compensated_data.SerialNumber |= ((uint64_t)dev_si7021->dev_electronic_serial_number.SNB_3) << 24;
	dev_si7021->dev_compensated_data.SerialNumber |= ((uint64_t)dev_si7021->dev_electronic_serial_number.SNB_2) << 16;
	dev_si7021->dev_compensated_data.SerialNumber |= ((uint64_t)dev_si7021->dev_electronic_serial_number.SNB_1) << 8;
	dev_si7021->dev_compensated_data.SerialNumber |= ((uint64_t)dev_si7021->dev_electronic_serial_number.SNB_0);
}

/**
 ** @brief Returns temperature in DegC
 **/
static int32_t SI7021TemperatureConv(SI7021_typedef *dev_si7021)                              
{ 
	uint16_t TempCode;
	float Temperature;
	
	TempCode = (dev_si7021->dev_uncompensated_data.TemperatureMSB << 8 | dev_si7021->dev_uncompensated_data.TemperatureLSB);
	
	Temperature = ((175.72f * TempCode) / 65536) - 46.85f;
	
	return (Temperature * 1000);
}

/**
 ** @brief Returns humidity in %RH 
 **/
static uint32_t SI7021HumidityConv(SI7021_typedef *dev_si7021)                              
{
	uint32_t RH_Code;
	float RelativeHumidity;
	
	RH_Code = (dev_si7021->dev_uncompensated_data.HumidityMSB << 8 | dev_si7021->dev_uncompensated_data.HumidityLSB);
	
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