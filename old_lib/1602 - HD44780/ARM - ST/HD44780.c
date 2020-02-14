#include "stm32f10x.h"
#include "main.h"
#include "HD44780.h"
#include "HD44780SUMBOL.h"
#include "Delay.h"


uint32_t FirstLine[16];
uint32_t SecondLine[16];

//*************************************************************************************************
//функция инициализации дисплея
		
void InitScreen(void)
     {
			 
			 Delay_us(50000); 															//пауза 50 миллисекунд после подачи питания на дисплей
			 SendCommandToScreen(DATA_8LINE);							//вызов подпрограммы передачи команды дисплею, 4 или 8 линий данных, 2 строки, символ 5*8; при использовании интерфейса 4 бит, заменить 8 на 4
			 Delay_us(40);																	//пауза ожидания выполнения команды
			 SendCommandToScreen(ENSCREEN_DISCUR_DISSQW);	//вызов подпрограммы передачи команды дисплею, дисплей включён, курсор включён, режим курсора мигающая чёрточка
			 Delay_us(40);
			 SendCommandToScreen(CLR_SCREEN);						      //вызов подпрограммы передачи команды дисплею, очистка дисплея, счётчик адреса DDRAM на нулевую позицию
			 Delay_us(1530);
			 SendCommandToScreen(INC_CADDR);						  	//вызов подпрограммы передачи команды дисплею, инкремент счётчика адреса
			 Delay_us(40);
	
		 }
		
//************************************************************************************************
//функция установки выводов на выход (групповая, для порта дисплея)	
void SetPinsToOut (void)			
			{

      DataLineScreenPort->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);			//сброс CNF в 00								
	    DataLineScreenPort->CRL  |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7);	//установка режима РР-GP

      CommandScreenPort->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2);			//сброс CNF в 00								
	    CommandScreenPort->CRL  |= GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2;	//установка режима РР-GP
				
			}		
		
//*************************************************************************************************
//функция передачи команды на дисплей		
void SendCommandToScreen (uint32_t Command)
			{

				CommandScreenPort->BSRR = RSCommandEnable;  //данные на дисплей: 0 - команды
				CommandScreenPort->BSRR = RWWriteEnable;  //режим работы с данными: 0 - запись				
				SendByteToScreen(Command);
				
			}
			
//*************************************************************************************************
//функция передачи данных на дисплей		
void SendDataToScreen (uint32_t Data)
			{

				CommandScreenPort->BSRR = RSDataEnable;  //данные на дисплей: 0 - команды
				CommandScreenPort->BSRR = RWWriteEnable;  //режим работы с данными: 0 - запись				
				SendByteToScreen(Data);
				
			}

//*************************************************************************************************
//функция передачи данных на дисплей, шина 8 бит
void SendByteToScreen (uint32_t Data)
			{
				

				DataLineScreenPort->ODR &= ~(0x00FF);		//очистить линию данных
				DataLineScreenPort->ODR  = Data;			 		//записать старшую тетраду команды
				
				CommandScreenPort->BSRR  = EClockEnable;		//строб 1
				//Delay_us(2);										         	//пауза 1 микросекунда
				__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();			
				CommandScreenPort->BSRR  = EClockDisable;		//строб 0 
				__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
				Delay_us(40);

			}			
			
////*************************************************************************************************
////функция передачи данных на дисплей, шина 4 бита
//void SendByteToScreen(uint32_t Data)
//			{
//				
//				uint32_t temp_0;
//				uint32_t temp_1;
//				
//				temp_0 = (Data & 0x000000F0);										 //старшая тетрада команды
//				temp_1 = Data<<4;				 //младшая тетрада команды
//			
//				
//				DataLineScreenPort->ODR &= ~(0x00F0);//очистить линию данных
//				DataLineScreenPort->ODR |= temp_0;			 //записать старшую тетраду команды (лог ИЛИ лишняя?)
//				CommandScreenPort->BSRR = EClockEnable;			 //строб 1
//				//Delay_us(2);										         //пауза 1 микросекунда
//				__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();			
//				CommandScreenPort->BSRR = EClockDisable;		 //строб 0 
//				__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//				
//				
//				DataLineScreenPort->ODR &= ~(0x00F0);//очистить линию данных
//				DataLineScreenPort->ODR |= temp_1;			 //записать младшую тетраду команды (лог ИЛИ лишняя?)
//				CommandScreenPort->BSRR = EClockEnable;			 //строб 1
//				//Delay_us(2);										         //пауза 1 микросекунда
//				__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();					
//				CommandScreenPort->BSRR = EClockDisable;		 //строб 0
//				__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//				Delay_us(40);
//				
//			}			
			
//************************************************************************************************
//Функция задержки из команды  NOP (зависит от частоты ядра)
void Delay_nop (void)
		{
		
		__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();
	//	__NOP();__NOP();__NOP();
	
		
		}			

//*************************************************************************************************
//функция выбора строки дисплея (дисплей двустрочный)
			
void SetAddrLine (uint32_t Line)
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
			
void SetAddrLineChar (uint32_t Line, uint8_t OffsetChar)
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
		 
void SendCharToScreen (uint32_t Char, uint32_t Line, uint8_t OffsetChar)
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
						
			uint32_t i;
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
			uint32_t i;
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

void SendTest (void)
		{
			
			uint8_t i = 0;				//счётчик символово в таблице 
			uint32_t j = 0;				//счётчик символово строки индикатора
			
			SetAddrLine(1);
			
			while(1)
			{
							 																			
				for (j=1;j<=16;j++)//цикл выполняется пока не будет выведено 16 символов (1я строка)
							
						{
														
							SendDataToScreen(SCREEN_EN_RUS_ASCI_WIN1251_v1[i++]);//отправить символ из таблицы (его ASCI код) на индикатор							
							Delay_ms(250); 																		//пауза 1 секунда, символы выводятся раз в секунду
																										
						}
						
					SetAddrLine(2);	
					//Delay_us(40);
						
					for (j=1;j<=16;j++)//цикл выполняется пока не будет выведено 16 символов (2я строка)
							
						{
							
							SendDataToScreen(SCREEN_EN_RUS_ASCI_WIN1251_v1[i++]);//отправить символ из таблицы (его ASCI код) на индикатор							
							Delay_ms(250); 																		//пауза 1 секунда, символы выводятся раз в секунду
																																									
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
			
			uint8_t j;
						
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
			uint8_t i;
			
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
			uint8_t i;
			
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
void CheckBusy (void) 
{

}
		

