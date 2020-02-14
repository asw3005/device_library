/* Created: 27.11.2014 12:36:09
 * DS3231.h
 *
 * 
 *
 */ 


#ifndef DS3231Variable_H_
#define DS3231Variable_H_

#include "stm32f10x.h"
#include "DS3231.h"

//*****************************************************************************************
//Структуры данных в RTC

struct
{
	uint8_t Seconds					      :4;
	uint8_t TenSeconds			      :3;
	uint8_t RESERVED_0			      :1;
	uint8_t Minutes					      :4;
	uint8_t TenMinutes			      :3;
	uint8_t RESERVED_1			      :1;
	uint8_t Hours					        :4;
	uint8_t TenHour					      :1;
	uint8_t TwentyHourAmPm	      :1;
	uint8_t Format12_24			      :1;
	uint8_t RESERVED_2			      :1;
	uint8_t Day						        :3;
	uint8_t RESERVED_3			      :5;
	uint8_t Date					        :4;
	uint8_t TenDate					      :2;
	uint8_t RESERVED_4			      :2;
	uint8_t Month					        :4;
	uint8_t TenMonth				      :1;
	uint8_t RESERVED_5			      :2;
	uint8_t Century					      :1;
	uint8_t Year					        :4;
	uint8_t TenYear					      :4;
        
	uint8_t Alarm1Seconds		      :4;
	uint8_t Alarm1TenSeconds      :3;
	uint8_t A1M1					        :1;
	uint8_t Alarm1Minutes		      :4;
	uint8_t Alarm1TenMinutes      :3;
	uint8_t A1M2					        :1;
	uint8_t Alarm1Hour			      :4;
	uint8_t Alarm1TenHour		      :1;
	uint8_t Alarm1TwentyHourAmPm	:1;
  uint8_t Alarm1Format12_24     :1;
	uint8_t A1M3					        :1;
	uint8_t Alarm1DayDate			    :4;
	uint8_t Alarm1TenDate			    :2;
	uint8_t Alarm1DyDt				    :1;
	uint8_t A1M4					        :1;
	
	uint8_t Alarm2Minutes			    :4;
	uint8_t Alarm2TenMinutes		  :3;
	uint8_t A2M2					        :1;
	uint8_t Alarm2Hour				    :4;
	uint8_t Alarm2TenHour			    :1;
	uint8_t Alarm2TwentyHourAmPm	:1;
  uint8_t Alarm2Format12_24     :1;
	uint8_t A2M3					        :1;
	uint8_t Alarm2DayDate			    :4;
	uint8_t Alarm2TenDate			    :2;
	uint8_t Alarm2DyDt				    :1; 
	uint8_t A2M4					        :1;
		
	uint8_t A1IE					        :1;
	uint8_t A2IE					        :1;
	uint8_t INTCN					        :1;
	uint8_t RS1						        :1;
	uint8_t RS2						        :1;
	uint8_t CONV					        :1;
	uint8_t BBSQW					        :1;
	uint8_t EOSC					        :1;
  
  uint8_t A1F						        :1;
	uint8_t A2F						        :1;
	uint8_t BSY						        :1;
	uint8_t EN32KHZ				        :1;
	uint8_t RESERVED_6		        :3;
	uint8_t OSF						        :1;
  
  uint8_t AgingOffset           :8;
  uint8_t MSB_Of_Temp           :8;
  uint8_t LSB_Of_Temp           :8;
	
} DS3231_RTC_Registers;


#endif /* DS3231Variable_H_ */
