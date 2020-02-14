/*
 * DS1821.c
 *
 *
 *
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include "DS18B20.h"

uint8_t ScratchPadBuff[9];
uint8_t ThausendCount, HundredCount, TensCount, UnitsCount = 0;

//******************************************************************************************************************
//Получение температуры с датчика
int16_t ReadTemperatureFromDS18B20(void)
{
	int16_t Temperature = 0;
	uint16_t SignBit = 0;
	uint8_t i;
	
	Reset1WireLine();									//Сброс устройств на линии и приём сигнала готовности
	WriteByteToDS18B20(SKIP_ROM);							//Пропустить адресацию
	WriteByteToDS18B20(START_CONVERT_T);				//Запустить преобразование температуры
	//В этой строке подтяжка линии к питанию, если используется паразитное питание
	_delay_ms(1000);										//Подождать окончания преобразования (не читая статус)
	Reset1WireLine();									//Сброс устройств на линии и приём сигнала готовности
	WriteByteToDS18B20(SKIP_ROM);							//Пропустить адресацию
	WriteByteToDS18B20(READ_SCRATCHPAD);				//Прочесть ячейки памяти, читается 9 штук, первые 2 байта младший и старший байты температуры
	for (i=0; i<2; i++)
		{
			ScratchPadBuff[i] = ReadByteFrom1Wire();		//Считывание памяти, 9 байт
		}

	Temperature |= ScratchPadBuff[1];					//Копировать старший байт температуры
	Temperature = Temperature<<8;							//сдвинуть байт на его место, т.к. при копировании находится на месте младшего байта					
	Temperature |= ScratchPadBuff[0];					//Копировать младший байт температуры
	SignBit = ((~(0x7FFF)) & Temperature);					//Сохранить знак температуры
	Temperature = ( (~(0xFF00)) & (Temperature>>3));	//сдвинуть байт на 3 единицы, т.к. используется 9-ти битный режим измерения температуры, стереть старшие 8 бит
	Temperature = Temperature*5;

	BCDtoDEC(Temperature);									//Подсчитать тысячи, сотни, десятки и единицы
	//Temperature = Temperature/10;
	
	
	Temperature |= SignBit;

	return(Temperature);	
}


//******************************************************************************************************************
//Cчётчик тысяч, сотен, десятков и единиц
void BCDtoDEC(int16_t BCDNumber)
{
	ThausendCount = 0;
	HundredCount = 0;
	TensCount = 0;
	UnitsCount = 0;
	//asm("nop");
	
	BCDNumber = BCDNumber-1000;
	while (BCDNumber>=0)
	{
		ThausendCount++;
		BCDNumber = BCDNumber-1000;
	}
	
	
	BCDNumber = BCDNumber+1000;
	//asm("nop");
	BCDNumber = BCDNumber-100;
	while (BCDNumber>=0)
	{
		HundredCount++;
		BCDNumber = BCDNumber-100;
	}
	
	
	BCDNumber = BCDNumber+100;
	//asm("nop");
	BCDNumber = BCDNumber-10;
	while (BCDNumber>=0)
	{
		TensCount++;
		BCDNumber = BCDNumber-10;
	}
	
	BCDNumber = BCDNumber+10;
	UnitsCount = BCDNumber;
	
}

//******************************************************************************************************************
//Получение информации о состоянии датчика температуры
uint8_t ReadScratchPadFromDS18B20( void )
{
	uint8_t ReadStatus, i;
	
	Reset1WireLine();									//Сброс устройств на линии и приём сигнала готовности
	WriteByteToDS18B20(SKIP_ROM);							//Пропустить адресацию
	WriteByteToDS18B20(READ_SCRATCHPAD);				//Прочесть ячейки памяти, читается 9 штук, 4й байт состояния
	for (i=0; i<9; i++)
	{
		ScratchPadBuff[i] = ReadByteFrom1Wire();		//Считывание памяти, 9 байт
	}
	//строка вызова проверки CRC
	ReadStatus = ScratchPadBuff[4];
	return(ReadStatus);					//
}



//******************************************************************************************************************
//Начальная настройка датчика температуры
void SetConfigRegisterDS18B20(void)
{
	uint8_t ConfigRegister = 0;
	
	ConfigRegister &= ~((1<<R1)|(1<<R0));			//Конфигурация для преобразования 9 бит
	Reset1WireLine();									//Сброс устройств на линии и приём сигнала готовности
	WriteByteToDS18B20(SKIP_ROM);					//Пропустить адресацию
	WriteByteToDS18B20(WRITE_SCRATCHPAD);				// ячейки памяти, читается 9 штук, 4й байт состояния
	WriteByteToDS18B20(TemperatureHigh);			//Верхняя точка сигнализации выхода текущей температуры за заданные пределы
	WriteByteToDS18B20(TemperatureLow);					//Нижняя точка сигнализации выхода текущей температуры за заданные пределы
	WriteByteToDS18B20(ConfigRegister);				//Установка разрядности преобразования
}


//******************************************************************************************************************
//Передача команды в 1-Wire устройство
//void SendCommandDS18B20(uint8_t Command)
//{
	//volatile uint8_t i,SendBit;
		//
	//SendBit = (~(0xFE) & Command);				//выделение младшего бита		
	////Reset1WireLine();								//Сброс устройств на линии и приём сигнала готовности
	//for (i=0; i<8; i++)							//цикл передачи 8-ми бит
		//{
			//
			//if (SendBit==1)
				//{
					//WriteBit1To1Wire();			//запись лог 1
				//}
			//else
				//{
					//WriteBit0To1Wire();		//запись лог 0
				//}
			//Command=Command>>1;						//обновление младшего бита
			//SendBit = (~(0xFE) & Command);		//выделение младшего бита
			//Delay_nop();							//пауза после тайм-слота передачи одного бита (необходимая пауза между тайм-слотами минимум 1мкс)
			//
		//}
			//
//}



//******************************************************************************************************************
//Передача байта в 1-Wire устройство
void WriteByteToDS18B20(uint8_t Byte)
{
	volatile uint8_t i, SendBit;
	
	SendBit = (~(0xFE) & Byte);					//выделение младшего бита	
	for (i=0; i<8; i++)						//цикл передачи 8-ми бит
	{
			
		if (SendBit==1)
			{
				WriteBit1To1Wire();				//запись лог 1
			}
		else
			{
				WriteBit0To1Wire();			//запись лог 0
			}
		Byte=Byte>>1;							//обновление младшего бита
		SendBit = (~(0xFE) & Byte);			//выделение младшего бита
		Delay_nop();							//пауза после тайм-слота передачи одного бита (необходимая пауза между тайм-слотами минимум 1мкс)
			
	}
	
}

//******************************************************************************************************************
//Считывание байта из 1-Wire устройства
uint8_t ReadByteFrom1Wire(void)
{
	uint8_t Temp_0;
	volatile uint8_t i, Byte;
	
	Byte = 0;								//обнуление переменной, принимающей байт с шины 1-Wire
	
	for (i=0; i<8; i++)							//цикл счёта приятых бит (всего 8)
		{
			Byte = Byte>>1;					//сдвиг вправо на 1 бит, байт принимается младшим битом вперёд, 1 сдвиг лишний в начале цикла
			Temp_0 = ReadBitFrom1Wire();		//считать бит с шины 1-Wire		
			if (Temp_0 == 1)				//если бит "1"
				{
					Byte |=0x80;				//записать на место старшего бита 1
				} 
			else  
				{
					Byte |=0x00;			//записать на место старшего бита 0 (эта часть условия может быть удалена)
				}
			Delay_nop();						//пауза после тайм-слота передачи одного бита (необходимая пауза между тайм-слотами минимум 1мкс)

		}	
		
	return(Byte);							//передать значение полученного байта
	
}



//******************************************************************************************************************
//Сброс шины 1-Wire и приём состояния готовности устройств
uint8_t Reset1WireLine(void)
{
	volatile uint8_t Error;
	
	asm("cli");																//Глобальное запрещение прерываний			
	DS1821_DIRECT_PORT |= (1<<DS1821_DIRECT_PIN);								//	
	DS1821_DATA_PORT |= (0<<DS1821_DATA_PIN_OUT);							//Подтяжка линии к 0
	_delay_us(480);//480
	DS1821_DIRECT_PORT &= ~(1<<DS1821_DIRECT_PIN);								//Отпустить линию
	_delay_us(70);															//Ожидание от 60 до 240мкс 70, ответ 120мкс
	Error = ((DS1821_DATA_PIN_IN & (~(READ_BIT_FROM_PORT)))>>ShiftingBit);		//Выделить 3й бит и сдвинуть на позицию нулевого бита	
	_delay_us(510);															//&&&
	asm("sei");																	//Глобальное разрешение прерываний
	return(Error);															//Передача кода ошибки (состояние линии в момент ожидания отклика от устройства)
}

//******************************************************************************************************************
//Запись нуля в шину 1-Wire 
void WriteBit0To1Wire(void)
{
	asm("cli");//Глобальное запрещение прерываний
		
	DS1821_DIRECT_PORT |= (1<<DS1821_DIRECT_PIN);					//
	DS1821_DATA_PORT |= (0<<DS1821_DATA_PIN_OUT);						//Подтяжка линии к 0
	_delay_us(60);	
	DS1821_DIRECT_PORT &= ~(1<<DS1821_DIRECT_PIN);					//Отпустить линию
	_delay_us(10);														//время, необходимое для восстановления линии в исходное состояние лог. 1		
	asm("sei");//Глобальное разрешение прерываний
}

//******************************************************************************************************************
//Запись единицы в шину 1-Wire
void WriteBit1To1Wire(void)
{
	asm("cli");//Глобальное запрещение прерываний
		
	DS1821_DIRECT_PORT |= (1<<DS1821_DIRECT_PIN);					//
	DS1821_DATA_PORT |= (0<<DS1821_DATA_PIN_OUT);						//Подтяжка линии к 0
	_delay_us(5);	
	DS1821_DIRECT_PORT &= ~(1<<DS1821_DIRECT_PIN);					//Отпустить линию
	_delay_us(64);	
	asm("sei");//Глобальное разрешение прерываний
}

//******************************************************************************************************************  
//Чтение бита с шины 1-Wire
uint8_t ReadBitFrom1Wire(void)
{
	uint8_t ReadBit;
	
	asm("cli");																//Глобальное запрещение прерываний	
	DS1821_DATA_PORT |= (0<<DS1821_DATA_PIN_OUT);								//Подтяжка линии к 0
	DS1821_DIRECT_PORT |= (1<<DS1821_DIRECT_PIN);							//
	_delay_us(5);//пауза 6 мкс
	DS1821_DIRECT_PORT &= ~(1<<DS1821_DIRECT_PIN);								//Отпустить линию
	_delay_us(9);//пауза 9 мкс
	ReadBit = ((DS1821_DATA_PIN_IN & (~(READ_BIT_FROM_PORT)))>>ShiftingBit);//Выделить 3й бит и сдвинуть на позицию нулевого бита
	_delay_us(55);	
	asm("sei");																	//Глобальное разрешение прерываний
	return(ReadBit);
}

//******************************************************************************************************************
//Задержка 2мкс (частота МК 4МГц)
void Delay_nop(void)
{
	asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");
	//asm("nop");asm("nop");asm("nop");asm("nop");
	//asm("nop");asm("nop");asm("nop");asm("nop");
}