/* Created: 27.11.2014 12:36:09
 * DS3231.h
 *
 * 
 *
 */ 


#ifndef DS3231_H_
#define DS3231_H_

//*****************************************************************************************
//Определения для используемых портов и выводов

#define DS3231_PORT				PORTB	 //
#define DS3231_CLK			    PB0			//
#define DS3231_DATA				PB2		 //


//*****************************************************************************************
//Адреса регистров RTC

#define	DS3231_SECONDS_ADDR_REG			0x00	// Единицы секунд и десятки секунд
#define	DS3231_MINUTES_ADDR_REG			0x01 // Единицы минут и десятки минут
#define	DS3231_HOURVIEW_ADDR_REG		0x02	// Единицы часов и десятки часов + формат 12/24
#define	DS3231_DAY_ADDR_REG				0x03 // День недели
#define	DS3231_DATE_ADDR_REG			0x04	// Число, единицы и десятки
#define	DS3231_MONTH_ADDR_REG			0x05 // Месяц, единицы и десятки
#define	DS3231_YEAR_ADDR_REG			0x06	// Год, единицы и десятки
#define DS3231_ALARM1_SECONDS			0x07 //Будильник 1, секунды и десятки секунд
#define DS3231_ALARM1_MINUTES			0x08	//Будильник 1, минуты и десятки минут
#define DS3231_ALARM1_HOURS				0x09 //Будильник 1, часы и десятки часов +  формат 12/24
#define DS3231_ALARM1_DAY_DATA			0x0A	//Будильник 1, день недели или единицы числа + десятки числа, + режим срабатывания день (1) или дата (0)
#define DS3231_ALARM2_MINUTES			0x0B //Будильник 2, минуты и десятки минут
#define DS3231_ALARM2_HOURS				0x0C	//Будильник 2, часы и десятки часов +  формат 12/24
#define DS3231_ALARM2_DAY_DATA			0x0D //Будильник 2, день недели или единицы числа + десятки числа, + режим срабатывания день (1) или дата (0)
#define	DS3231_CONTROL_ADDR_REG			0x0E	//	Регистр управления
#define	DS3231_CONTROL_STATUS_ADDR_REG	0x0F //	Регистр управления и статуса
#define	DS3231_AGING_OFFSET				0x10	//
#define	DS3231_MSB_OF_TEMP				0x11 // 
#define	DS3231_LSB_OF_TEMP				0x12	// 

//*****************************************************************************************
//Биты регистров RTC

//Регистр управления
#define	DS3231_A1IE			0		
#define DS3231_A2IE			1
#define DS3231_INTCN		2
#define DS3231_RS1			3
#define DS3231_RS2			4
#define DS3231_CONV			5
#define DS3231_BBSQW		6
#define DS3231_EOSC			7

//Регистр управления и статуса
#define DS3231_A1F			0
#define DS3231_A2F			1
#define DS3231_BSY			2
#define DS3231_EN32KHZ		3
#define DS3231_OSF			7

//*****************************************************************************************
//Структуры данных в RTC

struct
{
	uint8_t Seconds					:4;
	uint8_t TenSeconds				:3;
	uint8_t NULL_0					:1;
	uint8_t Minutes					:4;
	uint8_t TenMinutes				:3;
	uint8_t NULL_1					:1;
	uint8_t Hours					:4;
	uint8_t TenHour					:1;
	uint8_t TwentyHourPmAm			:1;
	uint8_t Format12_24				:1;
	uint8_t NULL_2					:1;
	uint8_t Day						:3;
	uint8_t NULL_3					:5;
	uint8_t Date					:4;
	uint8_t TenDate					:2;
	uint8_t NULL_4					:2;
	uint8_t Month					:4;
	uint8_t TenMonth				:1;
	uint8_t NULL_5					:2;
	uint8_t Century					:1;
	uint8_t Year					:4;
	uint8_t TenYear					:4;
	
	uint8_t Alarm1Seconds			:4;
	uint8_t Alarm1TenSeconds		:3;
	uint8_t A1M1					:1;
	uint8_t Alarm1Minutes			:4;
	uint8_t Alarm1TenMinutes		:3;
	uint8_t A1M2					:1;
	uint8_t Alarm1Hour				:4;
	uint8_t Alarm1TenHour			:1;
	uint8_t Alarm1TwentyHourAmPm	:1;
	uint8_t A1M3					:1;
	uint8_t Alarm1DayDate			:4;
	uint8_t Alarm1TenDate			:3;
	uint8_t Alarm1DyDt				:1;
	uint8_t A1M4					:1;
	
	uint8_t Alarm2Minutes			:4;
	uint8_t Alarm2TenMinutes		:3;
	uint8_t A2M2					:1;
	uint8_t Alarm2Hour				:4;
	uint8_t Alarm2TenHour			:1;
	uint8_t Alarm2TwentyHourAmPm	:1;
	uint8_t A2M3					:1;
	uint8_t Alarm2DayDate			:4;
	uint8_t Alarm2TenDate			:3;
	uint8_t Alarm2DyDt				:1;
	uint8_t A2M4					:1;
		
	
	
} DS3231_RTC_Registers;

struct
{
	uint8_t A1IE					:1;
	uint8_t A2IE					:1;
	uint8_t INTCN					:1;
	uint8_t RS1						:1;
	uint8_t RS2						:1;
	uint8_t CONV					:1;
	uint8_t BBSQW					:1;
	uint8_t EOSC					:1;
	
} DS3231_RTC_Control;

struct
{
	uint8_t A1F						:1;
	uint8_t A2F						:1;
	uint8_t BSY						:1;
	uint8_t EN32KHZ					:1;
	uint8_t NULL_0					:3;
	uint8_t OSF						:1;
	
} DS3231_RTC_ControlStatus;

//*****************************************************************************************
//Используемые функции

//Функции высокого уровня
void StartConditionI2C(void);	//Старт условие на шине
void StopConditionI2C(void);	//Стоп условие на шине

//Функции низкого уровня (функции-драйверы)






#endif /* DS3231_H_ */