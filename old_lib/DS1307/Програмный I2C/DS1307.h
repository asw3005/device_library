/* Created: 25.11.2014 10:22:58
 * DS1307.h
 *
 *
 *
 */ 



#ifndef DS1307_H_
#define DS1307_H_

#include <avr/io.h>

extern uint8_t DirectionData;
extern uint8_t OffsetAddrRW;
extern uint8_t NumberByteRW;

//*****************************************************************************************
//Определения для используемых портов и выводов

#define DS1307_PORT						PORTB	 //
#define DS1307_CLK						PORTB0			//
#define DS1307_DATA						PORTB2		 //
#define CLK_PIN							PINB0		//
#define DATA_PIN						PINB1	//


//*****************************************************************************************
// 

#define DATA_DOWN						(DS1307_PORT &= ~(1<<DS1307_DATA))
#define DATA_UP							(DS1307_PORT |= 1<<DS1307_DATA)
#define CLOCK_DOWN						(DS1307_PORT &= ~(1<<DS1307_CLK))
#define CLOCK_UP						(DS1307_PORT |= 1<<DS1307_CLK)


//*****************************************************************************************
//Адреса регистров RTC

#define	DS1307_SECONDS_ADDR_REG			0x00	// Единицы секунд и десятки секунд
#define	DS1307_MINUTES_ADDR_REG			0x01 // Единицы минут и десятки минут
#define	DS1307_HOURVIEW_ADDR_REG		0x02	// Единицы часов и десятки часов + формат 12/24
#define	DS1307_DAY_ADDR_REG				0x03 // День недели
#define	DS1307_DATE_ADDR_REG			0x04	// Число, единицы и десятки
#define	DS1307_MONTH_ADDR_REG			0x05 // Месяц, единицы и десятки 
#define	DS1307_YEAR_ADDR_REG			0x06	// Год, единицы и десятки
#define	DS1307_CONTROL_ADDR_REG			0x07 //	Регистр управления
#define	DS1307_STARTRAM_ADDR_REG		0x08	// Адрес начала области RAM
#define	DS1307_STOPRAM_ADDR_REG			0x3F // Адрес конца области RAM

//*****************************************************************************************
//Биты регистров RTC 

//Регистр управления

#define	DS1307_RS0						0 // Выбор частоты контрольного сигнала на выводе SQW/OUT (00 - 1Hz, 01 - 4096Hz, 10 - 8192Hz, *11 - 32768Hz, при включении 11), 
#define DS1307_RS1						1	// 
#define DS1307_SQWE						4 // Включение вывода контрольного сигнала SQW/OUT, при включении 0
#define DS1307_OUT						7	// Контроль логического уровна на выводе SQW/OUT, когда вывод контрольного сигнала отключён (на выходе 1 если бит OUT = 1), при включении 0

//*****************************************************************************************
//Адрес ведомого устройства

#define SlaveAddDS1307			0b11010000 //

//*****************************************************************************************
//Структуры данных в RTC

struct DS1307_RTC_Registers
{
	uint8_t Seconds			:4;
	uint8_t TenSeconds		:3;
	uint8_t ClockHalt		:1;
	uint8_t Minutes			:4;
	uint8_t TenMinutes		:3;
	uint8_t NULL_0			:1;
	uint8_t Hours			:4;
	uint8_t TenHour			:1;
	uint8_t TwentyHourPmAm	:1;
	uint8_t Format12_24		:1;
	uint8_t NULL_1			:1;
	uint8_t Day				:3;
	uint8_t NUUL_2			:5;
	uint8_t Date			:4;
	uint8_t TenDate			:2;
	uint8_t NUUL_3			:2;
	uint8_t Month			:4;
	uint8_t TenMonth		:1;
	uint8_t NULL_4			:3;
	uint8_t Year			:4;
	uint8_t TenYear			:4;
	
} DS1307_RTC_Registers;

struct
{
	uint8_t RS0				:1;
	uint8_t RS1				:1;
	uint8_t Null_0			:2;
	uint8_t SQWE			:1;
	uint8_t Null_1			:2;
	uint8_t Out				:1;
	
} DS1307_RTC_Control;

//*****************************************************************************************
//Используемые функции

//Функции высокого уровня





//Функции низкого уровня (функции-драйверы)


void ReadDS1307Byte(uint8_t Byte);											//Чтение из памяти ведомого устройства
void WriteDS1307Byte(uint8_t Byte);												//Запись в память ведомого
void StartConditionI2C(void);												//Старт условие на шине
void RStartConditionI2C(void);													//Повторный старт
void StopConditionI2C(void);												//Стоп условие на шине
void SendByteI2C(uint8_t Byte);													//Отправить байт
uint8_t ReadACKNACK(void);													//Приём бита подтверждения или неподтверждения приёма/передачи данных								
void AddressingRTC(uint8_t SlaveAddr, uint8_t Addr, uint8_t ReadWriteBit);		//Адресация ведомого устройства с битом признака чтения или записи


#endif /* DS1307_H_ */