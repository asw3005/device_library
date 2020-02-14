/*
 * DS18B20.h
 *
 *
 *
 */ 

#include <avr/io.h>

#ifndef DS18B20_H_
#define DS18B20_H_
extern uint8_t ThausendCount, HundredCount, TensCount, UnitsCount;

#define READ_ROM			0x33 //Identification
#define SKIP_ROM			0xCC	//Skip addressing
#define MATCH_ROM			0x55 //Address specific device
#define SEARCH_ROM			0xF0	//Obtain IDs of all devices on the bus
#define ALARM_SEARCH        0xEC //
#define OVERDRIVE_SKIP		0xRO    //3CH Overdrive version of SKIP ROM
#define OVERDRIVE_MATCH_ROM 0x69 //Overdriver version of MATCH ROM

//**********************************************************************************************************************************
//TEMPERATURE CONVERSION COMMANDS
#define START_CONVERT_T		0x44	//Initiates temperature conversions.

//**********************************************************************************************************************************
//Configuration register, default R0 = 1, R1 = 1
#define R0                  5
#define R1                  6

//**********************************************************************************************************************************
//MEMORY COMMANDS
#define READ_SCRATCHPAD		0xBE //Reads the entire scratchpad including the CRC byte. DS18B20 transmits up to 9 data bytes to master.
#define WRITE_SCRATCHPAD	0x4E	//Master transmits 3 data bytes to DS18B20.Writes data into scratchpad bytes 2, 3, and 4 (TH, TL, and configuration registers).
#define COPY_SCRATCHPAD		0x48 //Copies TH, TL, and configuration register data from the scratchpad to EEPROM.
#define RECALL_E2			0xB8	//DS18B20 transmits recall status to master.Recalls TH, TL, and configuration register data from EEPROM to the scratchpad.
#define READ_POWER_SUPPLU	0xB4 //DS18B20 transmits supply status to master.Signals DS18B20 power supply mode to the master.



//**********************************************************************************************************************************
//
#define TemperatureHigh		0x2D //45
#define TemperatureLow		0x12  //18


#define DS1821_DATA_PORT	PORTB
#define DS1821_DIRECT_PORT	DDRB
#define DS1821_DATA_PIN_IN	PINB
#define DS1821_DIRECT_PIN	DDB3
#define DS1821_DATA_PIN_OUT	PB3		//вывод подключения датчика температуры
#define READ_BIT_FROM_PORT  0xF7	//считываемый бит порта
#define ShiftingBit			0x03	//кол-ви сдвигов в битах, необходимых для перемещения на позицию нулевого бита


int16_t ReadTemperatureFromDS18B20(void);		//Получение температуры с датчика
void SendCommandDS18B20(uint8_t Command);			//Передача команды в 1-Wire устройство
void WriteByteToDS18B20(uint8_t Byte);			//Передача байта в 1-Wire устройство
uint8_t ReadByteFrom1Wire(void);					//Считывание байта из 1-Wire устройства
uint8_t Reset1WireLine (void);						//Сброс шины 1-Wire и приём состояния готовности устройств
void WriteBit0To1Wire (void);						//Запись нуля в шину 1-Wire
void WriteBit1To1Wire(void);					//Запись единицы в шину 1-Wire
uint8_t ReadBitFrom1Wire (void);					//Чтение бита с шины 1-Wire
void SetConfigRegisterDS18B20(void);					//Начальная настройка датчика температуры
uint8_t ReadScratchPadFromDS18B20(void);				//Получение информации о состоянии датчика температуры
void BCDtoDEC(int16_t BCDNumber);				//счётчик тысяч, сотен, десятков и единиц


void Delay_nop(void);							//Задержка 2мкс (частота МК 4МГц)



#endif /* DS18B20_H_ */