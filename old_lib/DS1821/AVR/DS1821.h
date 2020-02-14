/*
 * DS1821.h
 *
 *
 *
 */ 
#include <avr/io.h>


#ifndef DS1821_H_
#define DS1821_H_

#define READ_ROM			0x33 //Identification
#define SKIP_ROM			0xCC	//Skip addressing
#define MATCH_ROM			0x55 //Address specific device
#define SEARCH_ROM			0xF0	//Obtain IDs of all devices on the bus
#define OVERDRIVE_SKIP		0xRO //3CH Overdrive version of SKIP ROM
#define OVERDRIVE_MATCH_ROM 0x69	//Overdriver version of MATCH ROM

//**********************************************************************************************************************************
//TEMPERATURE CONVERSION COMMANDS
#define READ_TEMPERATURE	0xAA //Reads last converted temperature value from temperature register. Master receives 8-bit temperature value from DS1821.
#define START_CONVERT_T		0xEE	//Initiates temperature conversions.
#define STOP_CONVERT_T		0x22 //Halts temperature conversions.

//**********************************************************************************************************************************
//THERMOSTAT and STATUS/CONFIGURATION COMMANDS
#define WRITE_TH			0x01 //Writes data to the TH register. Master transmits 8-bit TH value to DS1821.
#define WRITE_TL			0x02	//Writes data to the TL register. Master transmits 8-bit TL value to DS1821.
#define READ_TH				0xA1 //Reads data from the TH register. Master receives 8-bit TH value from DS1821.
#define READ_TL				0xA2	//Reads data from the TL register. Master receives 8-bit TL value from DS1821.
#define WRITE_STATUS		0x0C //Writes data to the status/configuration register. Master transmits 8-bit status/configuration value to DS1821.
#define READ_STATUS			0xAC	//Reads data from the status/configuration register. Master receives 8-bit status/configuration value from DS1821.

#define DONE				7	//1 - преобразование температуры закончено
#define NVB					5		//1 - выполняется запись в энергонезависимую память
#define THF					4	//1 - превышен порог максимума температуры
#define TLF					3		//1 - температура ниже порога минимума температуры
#define TR					2	//1 - ражим работы термостат, 0 - режим работы 1-WIRE, после сброса 0
#define POL					1		//1 - выход термостата активная единица, 0 - выход термостата активный 0, после сброса 1
#define ONESHOT				0	//1 - преобразование температуры происходит 1 раз после передачи команды "начать преобразование температуры", после сброса 1	

//#define  StatusRegConfig   	

//**********************************************************************************************************************************
//HIGH-RESOLUTION COMMANDS
#define READ_COUNTER		0xA0 //Reads data from the counter register Master receives 9-bit counter value from DS1821.
#define LOAD_COUNTER		0x41	//Loads slope accumulator data into the counter register None


//************************************************************************************************************
//Определения для портов ввода-вывода

#define DS1821_DATA_PORT	PORTB
#define DS1821_DIRECT_PORT	DDRB
#define DS1821_DATA_PIN_IN	PINB
#define DS1821_DIRECT_PIN	DDB4
#define DS1821_DATA_PIN_OUT	PB4		//вывод подключения датчика температуры


//************************************************************************************************************
//Прототипы функций

int8_t ReadTemperatureFromDS1821(void);			//Получение температуры с датчика
uint8_t ReadStatusFromDS1821(void);				//Получение информации о состоянии датчика температуры
void StartConvertTemperatureDS1821(void);			//Запустить преобразование температуры температуры
void StopConvertTemperatureDS1821(void);		//Остановить преобразование температуры температуры
void SendCommandDS1821(uint8_t Command);			//Передача команды в 1-Wire устройство
void WriteByteToDS1821(uint8_t Byte);			//Передача байта в 1-Wire устройство
uint8_t ReadByteFrom1Wire(void);					//Считывание байта из 1-Wire устройства
uint8_t  Reset1WireLine (void);						//Сброс шины 1-Wire и приём состояния готовности устройств
void WriteBit0To1Wire (void);						//Запись нуля в шину 1-Wire
void WriteBit1To1Wire (void);					//Запись единицы в шину 1-Wire
uint8_t ReadBitFrom1Wire (void);					//Чтение бита с шины 1-Wire
void SetConfigRegisterDS1821(void);				//Начальная настройка датчика температуры
void WriteStatusToDS1821(uint8_t WriteStatus);		//Отправка конфигурации датчику температуры
void Delay_nop(void);								//Задержка 2мкс (частота МК 4МГц)

#endif /* DS1821_H_ */