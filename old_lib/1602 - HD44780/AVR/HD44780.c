

#include <avr/io.h>
#include <util/delay.h>
//#include "WaterManager1.0.h"
#include "HD44780.h"
#include "HD44780SUMBOL.h"

//#include "Delay.h"
//#include <C:\Program Files\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\avr\include\avr\pgmspace.h>

uint8_t FirstLine[16];
uint8_t SecondLine[16];

//*************************************************************************************************
//функция инициализации дисплея
		
void InitScreen(void)
{
			 
	_delay_ms(50); 								//пауза 50 миллисекунд после подачи питания на дисплей
    //CheckBusy();
	SendCommandToScreen(DATA_4LINE);					//вызов подпрограммы передачи команды дисплею, 4 или 8 линий данных, 2 строки, символ 5*8; при использовании интерфейса 4 бит, заменить 8 на 4
    //CheckBusy();
	_delay_us(40);									//пауза ожидания выполнения команды
	SendCommandToScreen(ENSCREEN_DISCUR_DISSQW);	    //вызов подпрограммы передачи команды дисплею, дисплей включён, курсор включён, режим курсора мигающая чёрточка
    //CheckBusy();
	_delay_us(40);
	SendCommandToScreen(CLR_SCREEN);				//вызов подпрограммы передачи команды дисплею, очистка дисплея, счётчик адреса DDRAM на нулевую позицию
    //CheckBusy();
	_delay_ms(1530);
	SendCommandToScreen(INC_CADDR);				    //вызов подпрограммы передачи команды дисплею, инкремент счётчика адреса
    //CheckBusy();
	_delay_us(100);
	
}
		
//************************************************************************************************
//функция установки выводов на выход (групповая, для порта дисплея)	
inline void SetPinsToOut (void)			
{
    DirectPortB = (PB3Out)|(PB4Out)|(PB5Out);
    DirectPortC = (PC0Out)|(PC1Out)|(PC2Out)|(PC3Out);

    CommandScreenPort  = (0<<PORTB3)|(0<<PORTB4)|(0<<PORTB5);
	DataLineScreenPort = (0<<PORTC0)|(0<<PORTC1)|(0<<PORTC2)|(0<<PORTC3);			
}		
		
//*************************************************************************************************
//функция передачи команды на дисплей		
void SendCommandToScreen (uint8_t Command)
{

    RSCommandEnable;
    RWWriteEnable;				
	SendByteToScreen(Command);
				
}
			
//*************************************************************************************************
//функция передачи данных на дисплей		
void SendDataToScreen (uint8_t Data)
{

    RSDataEnable;  //данные на дисплей: 0 - команды
    RWWriteEnable;  //режим работы с данными: 0 - запись			
	SendByteToScreen(Data);
				
}

////*************************************************************************************************
////функция передачи данных на дисплей, шина 8 бит
//void SendByteToScreen (uint8_t Data)
//{
				//
//
	//DataLineScreenPort &= ~(0xFF);		//очистить линию данных
	//DataLineScreenPort  = Data;			 		//записать старшую тетраду команды
				//
	//CommandScreenPort->BSRR  = EClockEnable;		//строб 1
	////Delay_us(2);										         	//пауза 1 микросекунда
	//asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");			
	//CommandScreenPort->BSRR  = EClockDisable;		//строб 0 
	//asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	//Delay_us(40);
//
//}			
			
//*************************************************************************************************
//функция передачи данных на дисплей, шина 4 бита
void SendByteToScreen(uint8_t Data)
			{
				
				uint8_t temp_0;
				uint8_t temp_1;

                temp_1 = (Data & (~0xF0));				 //младшая тетрада команды
                temp_0 = (Data>>4);										 //старшая тетрада команды
			
				DataLineScreenPort &= ~(ClearMaskPort);//очистить линию данных
				DataLineScreenPort |= temp_0;			 //записать старшую тетраду команды (лог ИЛИ лишняя?)
				EClockEnable;			 //строб 1
				//Delay_us(2);										         //пауза 1 микросекунда
				asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");	
				EClockDisable;		 //строб 0 
				asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
			
				DataLineScreenPort &= ~(ClearMaskPort);//очистить линию данных
				DataLineScreenPort |= temp_1;			 //записать младшую тетраду команды (лог ИЛИ лишняя?)
				EClockEnable;			 //строб 1
				//Delay_us(2);										         //пауза 1 микросекунда
				asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");			
				EClockDisable;		 //строб 0
				asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
                
				//_delay_us(40);//!!!!
                CheckBusy();
				
			}			
			
//************************************************************************************************
//Функция задержки из команды  NOP (зависит от частоты ядра)
//void Delay_nop (void)
//{		
    //asm("nop");asm("nop");asm("nop");
    //asm("nop");asm("nop");asm("nop");
    //asm("nop");asm("nop");asm("nop");
    //asm("nop");asm("nop");asm("nop");
   	//asm("nop");asm("nop");asm("nop");
//}			

//*************************************************************************************************
//функция выбора строки дисплея (дисплей двустрочный)
			
void SetAddrLine (uint8_t Line)
{
				
    if (Line == 1)
	    {					
	    SendCommandToScreen(SET_DDRAM_SCREEN_1);	//вызов подпрограммы передачи команды дисплею, адресация на DDRAM, адресс первой строки первого символа							
	    }
    else
	    {						
	    SendCommandToScreen(SET_DDRAM_SCREEN_2);	//вызов подпрограммы передачи команды дисплею, адресация на DDRAM, адресс второй строки первого символа							
	    }
				
}
			
//*************************************************************************************************
//функция выбора строки и положения выводимого символа на дисплее (дисплей двустрочный)
			
void SetAddrLineChar (uint8_t Line, uint8_t OffsetChar)
{
				
    if (Line == 1)
	    {						
	    SendCommandToScreen (SET_DDRAM_SCREEN_1+OffsetChar);	//вызов подпрограммы передачи команды дисплею, адресация на DDRAM, адресс первой строки первого символа							
	    }
    else
	    {						
	    SendCommandToScreen (SET_DDRAM_SCREEN_2+OffsetChar);	//вызов подпрограммы передачи команды дисплею, адресация на DDRAM, адресс второй строки первого символа							
	    }
				
}


//*************************************************************************************************
//функция вывода символов в видимую часть экрана
		 
void SendCharToScreen (uint8_t Char, uint8_t Line, uint8_t OffsetChar)
		{
						
			SetAddrLineChar(Line, OffsetChar);
			//Delay_us(40);
			SendDataToScreen(Char);
			//Delay_us(40);
			
		}
		
//*************************************************************************************************
//функция вывода символов на первую строку экрана
		
void SendCharToFirstLine (void)
{
						
	volatile uint8_t i;
	SetAddrLine(1);
	//Delay_us(40);																	//пауза ожидания выполнения команды
			
	for (i=0;i<=15;i++)
		{
						
		    SendDataToScreen(FirstLine[i]);										//отправить 1й символ из массива
		    //Delay_us(40);																	//пауза ожидания выполнения команды
						
		}
			
}

//*************************************************************************************************
//функция вывода символов на вторую строку экрана
		
void SendCharToSecondLine (void)
{
	volatile uint8_t i;
	SetAddrLine(2);
	//Delay_us(40);																	//пауза ожидания выполнения команды
			
	for (i=0;i<=15;i++)
		{
						
		    SendDataToScreen(SecondLine[i]);										//отправить 1й символ из массива
		    //Delay_us(40);																	//пауза ожидания выполнения команды
						
		}
																					
}	
		
		
//*************************************************************************************************
//функция вывода символов из таблицы кодировки дисплея (тестовый вывод всех символов подряд)

void SendTest(void)
{
			
	volatile uint8_t i = 0;				//счётчик символово в таблице 
	volatile uint8_t j = 0;				//счётчик символово строки индикатора
			
	SetAddrLine(1);
			
	while(1)
	{
							 																			
		for (j=1;j<=16;j++)//цикл выполняется пока не будет выведено 16 символов (1я строка)							
			{														
				SendDataToScreen(SCREEN_EN_RUS_ASCI_WIN1251_v1[i++]);//отправить символ из таблицы (его ASCI код) на индикатор							
				_delay_ms(250);// 	!!!																	//пауза 1 секунда, символы выводятся раз в секунду																										
			}
						
			SetAddrLine(2);	
			//Delay_us(40);
						
		for (j=1;j<=16;j++)//цикл выполняется пока не будет выведено 16 символов (2я строка)							
			{							
				SendDataToScreen(SCREEN_EN_RUS_ASCI_WIN1251_v1[i++]);//отправить символ из таблицы (его ASCI код) на индикатор							
				_delay_ms(250); //!!!!!																		//пауза 1 секунда, символы выводятся раз в секунду																																									
			}
						
			SetAddrLine(1);
			//Delay_us(40);
						
		if (i == 255)//если все символы таблицы выведены, то
			{							
				i=0;//сбросить индекс символов таблицы на первый								
			}	
			
		}
						
}

//*************************************************************************************************
//функция записи собственных символов (всего 8 штук) в ОЗУ дисплея
		
void WriteCharToScreen (void)
{
			
	volatile uint8_t j;
						
	SendCommandToScreen (CGRAM_ADDR);						  //установить адрес на внутреннее ОЗУ (для записи своих символов)
	//Delay_us(40);													  //пауза ожидания выполнения команды
														
	for (j=0;j<=63;j++)								   //счёт записи символа, один символ состоит из 8-ми байт (5 младших значащих бит)			
		{
								
			SendDataToScreen(RamScreenChar[j]);
			//Delay_us(40);													  //пауза ожидания выполнения команды
																
		}
								
	SendCommandToScreen (SET_DDRAM_SCREEN_1);						  //установить адрес на внутреннее ОЗУ
	//Delay_us(40);													  //пауза ожидания выполнения команды
						
}
		
//************************************************************************************************
//Функция отправки текста из памяти МК на дисплей
void SendMessageToScreen(void)
{
	volatile uint8_t i;
			
	for (i=0;i<=15;i++)
		{						
		FirstLine[i] = MessageLine_1[i];	//копировать текст для первой строки в буфер				
		}
			
	for (i=0;i<=15;i++)
		{					
		SecondLine[i] = MessageLine_2[i];	//копировать текст для второй строки в буфер				
		}
			
	SendCharToFirstLine();						//отправка данных из буфера
	SendCharToSecondLine();
			
}
		
//************************************************************************************************
//Очистка дисплея (видимой области)
void ClearScreen(void)
{
	volatile uint8_t i;
			
	SetAddrLine(1);
						
	for (i=0;i<=15;i++)
		{						
		SendDataToScreen(0x99);		//отправить символ пробела				
		}
			
	SetAddrLine(2);
			
	for (i=0;i<=15;i++)
		{						
		SendDataToScreen(0x99);		//отправить символ пробела				
		}
			
}		

//*************************************************************************************************
//Функция проверки флага занятости дисплея 
inline void PinScreenFlagIn(void) 
{
    DirectPortC &= PC3In;
    DataLineScreenPort = 1<<PORTC3; 
}

//*************************************************************************************************
//Функция проверки флага занятости дисплея 
inline void PinScreenFlagOut(void) 
{
    DirectPortC |= PC3Out;
    DataLineScreenPort = 0<<PORTC3;   
}
	
//*************************************************************************************************
//Функция проверки флага занятости дисплея 
void CheckBusy(void) 
{
    volatile uint8_t BusyFlag;
    
    PinScreenFlagIn();
    RSCommandEnable;
    RWReadEnable;

    EClockEnable;                       //строб 1
    //Delay_nop();
    asm("nop");asm("nop");asm("nop");
    asm("nop");asm("nop");asm("nop");
    BusyFlag = PINC & (~(0xF7));
    EClockDisable;		                //строб 0
    //asm("nop");asm("nop");asm("nop");
    //asm("nop");asm("nop");asm("nop");
    EClockEnable;			            //строб 1
    //Delay_nop();
    asm("nop");asm("nop");asm("nop");
    asm("nop");asm("nop");asm("nop");
    EClockDisable;  
        
    while (BusyFlag == 0x08)
        {

            EClockEnable;			    //строб 1
            //Delay_nop();
            asm("nop");asm("nop");asm("nop");
            asm("nop");asm("nop");asm("nop");
            BusyFlag = PINC & (~(0xF7));
            EClockDisable;		        //строб 0
            //asm("nop");asm("nop");asm("nop");
            //asm("nop");asm("nop");asm("nop");
            EClockEnable;			            //строб 1
            //Delay_nop();
            asm("nop");asm("nop");asm("nop");
            asm("nop");asm("nop");asm("nop");
            EClockDisable;

        }    
    
    PinScreenFlagOut();
}
		

