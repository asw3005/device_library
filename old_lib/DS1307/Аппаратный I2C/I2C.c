/* Created: 03.01.2015 18:27:17
 * I2C.c
 *
 *
 *
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include "I2C.h"
#include "DS1307.h"

uint8_t DirectionData;
uint8_t OffsetAddrRW;
uint8_t NumberByteRW;
uint8_t CurrentAddressRW;
uint8_t RWDeviceFlag;
uint8_t *RWBytePointer;

//*********************************************************************************************
//Настройка автомата I2C (частоты передачи данных SCL)
void I2C_Init(void)
{
	TWSR = TWSR_PRESCALER_VALUE;		// Установить предделитель тактовой частоты I2C в регистре TWSR
	TWBR = TWBR_BIT_VALUE;				// Установить значение бит рейта в регистре TWBR
}


//*********************************************************************************************
//Отправить определённое количество байт, по окончанию сгенерировать СТОП
void SendByteI2C(void)
{
	uint8_t CounterByte = 0;
	
	if (CounterByte<NumberByteRW)
		{
			RWBytePointer = RWBytePointer + OffsetAddrRW;	//Определить начальный байт, прибавив смещение
			TWDR = (*RWBytePointer);							//Записать через указатель очередной байт
			OffsetAddrRW++;									//Увеличить смещение на 1 (получить адрес следующего байта)
			CounterByte++;										//увеличить счётчик передаваемых байт на 1
			SEND_BYTE;										//Запустить автомат I2C для передачи данных
			//return;
		}
	else
		{
			//CounterByte = 0;
			SEND_STOP;										//Сгенерировать состояние STOP
		}

}


//*********************************************************************************************
//Принять байт
uint8_t ReadByteI2C(void)
{
		uint8_t CounterByte = 0;
		
		if (CounterByte<NumberByteRW)
		{
			
			OffsetAddrRW++;									//Увеличить смещение на 1 (получить адрес следующего байта)
			CounterByte++;										//увеличить счётчик передаваемых байт на 1
			SEND_BYTE;										//Запустить автомат I2C для передачи данных
			//return;
		}
		else
		{
			//CounterByte = 0;
			SEND_STOP;										//Сгенерировать состояние STOP
		}
}