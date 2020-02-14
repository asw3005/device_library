/*
 * WaterControl.c
 * 
 * Created: 17.11.2014 22:39:28
 *
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "WaterControl.h"
#include "D:\ATMEL_STUDIO6\WaterControl\WaterControl\Library\I2C.h"
#include "D:\ATMEL_STUDIO6\WaterControl\WaterControl\Library\DS1307.h"
#include "D:\ATMEL_STUDIO6\WaterControl\WaterControl\Library\HD44780.h"
#include "D:\ATMEL_STUDIO6\WaterControl\WaterControl\Library\DS18B20.h"

//*********************************************************************************************
//Основная программа
int main(void)
{
	
	    //SetPinsToOut();
	    //InitScreen();
	    //SendMessageToScreen();
	    //ReadTemperatureFromDS18B20();
		I2C_Init();
		DS1307_Init();
	
	
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}

//*********************************************************************************************
//Прерывание автомата I2C
ISR (TWI_vect)
{
	uint8_t ParameterCase;
	
	ParameterCase = (TWSR & (~TWSR_MASK));
	
	switch(ParameterCase)
	{
	//***********************************************
	//Cгенерировано состояние СТАРТ на шине I2C
		case START_CONDITION_I2C1:
		
			TWCR &= ~CLEAR_START_BIT_MASK;			// Cтереть бит старта в регистре управления автоматом I2C
			TWDR = (SlaveAddDS1307|0x01);		// Записать текущий адрес + бит напрвлен6ия данных в регистр TWDR
			SEND_BYTE;								// Дать команду отправить данные			
		
		break;
		
	//***********************************************
	//Сгенерировано состояние повторный старт		
		case REPEATED_START_I2C1:
		
			TWCR &= ~CLEAR_START_BIT_MASK;			// Cтереть бит старта в регистре управления автоматом I2C
			TWDR = (SlaveAddDS1307|0x00);		// Записать текущий адрес + бит напрвлен6ия данных в регистр TWDR	
			SEND_BYTE;								// Дать команду отправить данные
							
		break;

	//***********************************************
	//		
		case ARBITRATION_LOST_I2C1:					
		break;

	//***********************************************
	//Адрес ведомого устройства + бит на запись отправлены		
		case SLADDR_W_RECEIVED_ACK:
			
			TWDR = CurrentAddressRW;		// Записать текущий адрес+бит напрвления данных в регистр TWDR
			SEND_BYTE;								// Дать команду отправить данные
			
		break;

	//***********************************************
	//Ведомое устройство не ответило, адрес + бит на запись не приняты		
		case SLADDR_W_RECEIVED_NACK:		
		
		break;

	//***********************************************
	//Передан очередной байт ведомому устройству, ответ получен		
		case DATA_TRANSMITTED_ACK_RECEIVED:	
		
			if (RWDeviceFlag == 1)
				{
					SEND_START;
				}
			else
				{
					SendByteI2C();
				}
		
		break;

	//***********************************************
	//Ведомое устройство не получило текущий отправленный байт	
		case DATA_TRANSMITTED_NACK_RECEIVED:
		
		break;

	//***********************************************
	//		
		case SLADDR_R_RECEIVED_ACK:
		
				if (NumberByteRW > 1)
					{
						READ_BYTE_ACK;						// Дать команду принять данные, на принятый байт отправить подтверждение ACK
					} 
				else
					{
						READ_BYTE;							// Дать команду принять данные, на принятый байтотправить NACK
					}
	
		break;

	//***********************************************
	//	
		case SLADDR_R_RECEIVED_NACK:		
		
		break;

	//***********************************************
	//		
		case DATA_RECEIVED_ACK_RETURNED:
		
				ReadByteI2C();
		
		break;

	//***********************************************
	//		
		case DATA_RECEIVED_NACK_RETURNED:	
			
			//Считать один принятый байт
			SEND_STOP; //Сгенерировать состояние стоп 
						
		break;

	//***********************************************
	//Действия при считывании не описанного кода приёмо/передачи		
		default:
		asm("nop");
		break;
	}
	
	
	
}
