/*Display - PG128128*/
/*Controller - T6963C*/

#include "stm32f10x.h"
#include "PG128128.h"
#include "Delay.h"

//uint8_t Command;




//************************************************************************************************
//функция установки выводов на выход (групповая, для порта дисплея)	
void InitScreen (void)			
			{

			ScreenReset();																															//сброс дисплея после подачи питания									
			GraphicHomeAddr(GRAPHICS_HOME_ADDRESS_LOW, GRAPHICS_HOME_ADDRESS_HIGH);				//Функция установки начального адреса вывода графики
			GraphicArea(GRAPHICS_AREA_LOW, GRAPHICS_AREA_HIGH);													//Функция установки границ вывода графики			
			TextHomeAddr(TEXT_HOME_ADDRESS_LOW, TEXT_HOME_ADDRESS_HIGH);									//Функция установки начального адреса вывода текста
			TextArea(TEXT_AREA_LOW, TEXT_AREA_HIGH);																		//Функция установки границ вывода текста
			ScreenWriteCommand(SCREEN_MODE);																							//Функция записи команды в регистр дисплея, OR+INTERNAL_CGROM 
			SetAddrPointer (0x01,0x08);																									//Функция установки указателя адреса
			ScreenWriteCommand(TEXT_MODE);																							//Функция записи команды в регистр дисплея	
//Set ‘TEXT HOME ADDRESS’ младший байт данных, старший +команда
//Set ‘TEXT AREA SET’ младший байт данных, старший +команда
//Set ‘GRAPHICS HOME ADDRESS’ младший байт данных, старший +команда
//Set ‘GRAPHICS AREA SET’ младший байт данных, старший +команда
//Set ‘MODE SET’ команда
//Set ‘DISPLAY MODE’ команда
				
			}

//************************************************************************************************
//функция установки выводов на выход (групповая, для порта дисплея)	
void InitScreenV1 (void)			
			{

			ScreenReset();																																											//сброс дисплея после подачи питания
			ScreenWriteCommand(SCREEN_MODE);																																	//Функция записи команды в регистр дисплея, OR+INTERNAL_CGROM 
			SendAddrCommand (GRAPHICS_HOME_ADDRESS_LOW, GRAPHICS_HOME_ADDRESS_HIGH, GRAPHIC_HOME_ADDRESS_SET);//Функция установки начального адреса вывода графики	
			//GraphicHomeAddr(GRAPHICS_HOME_ADDRESS_LOW, GRAPHICS_HOME_ADDRESS_HIGH);													//Функция установки начального адреса вывода графики
			SendAddrCommand (GRAPHICS_AREA_LOW, GRAPHICS_AREA_HIGH, GRAPHIC_AREA_SET);													//Функция установки границ вывода графики	
			//GraphicArea(GRAPHICS_AREA_LOW, GRAPHICS_AREA_HIGH);																								//Функция установки границ вывода графики	
			SendAddrCommand (TEXT_HOME_ADDRESS_LOW, TEXT_HOME_ADDRESS_HIGH, TEXT_HOME_ADDRESS_SET);						//Функция установки начального адреса вывода текста	
			//TextHomeAddr(TEXT_HOME_ADDRESS_LOW, TEXT_HOME_ADDRESS_HIGH);																		//Функция установки начального адреса вывода текста
			SendAddrCommand (TEXT_AREA_LOW, TEXT_AREA_HIGH, TEXT_AREA_SET);																			//Функция установки начального границ вывода графики	
			//TextArea(TEXT_AREA_LOW, TEXT_AREA_HIGH);																													//Функция установки границ вывода текста
			
			ScreenWriteCommand(TEXT_MODE);																																		//Функция записи команды в регистр дисплея
				
			SendAddrCommand (0x00, 0x18, CURSOR_POINTER_SET);
				
			
				
			SendAddrCommand (0x00, 0x18, ADDRESS_POINTER_SET);																									//установка указателя адреса на позицию начала текстовой страницы	
			
			ScreenWriteData(0x15);
				
//			SendAddrCommand (0x01, 0x18, ADDRESS_POINTER_SET);																									//установка указателя адреса на позицию начала текстовой страницы	
//			
//			ScreenWriteData(0x16);
//			
//			SendAddrCommand (0x02, 0x18, ADDRESS_POINTER_SET);																									//установка указателя адреса на позицию начала текстовой страницы	
//			
	//		ScreenWriteData(0x17);
//			
//			SendAddrCommand (0x03, 0x18, ADDRESS_POINTER_SET);																									//установка указателя адреса на позицию начала текстовой страницы	
//			
//			ScreenWriteData(0x18);
//Set ‘TEXT HOME ADDRESS’ младший байт данных, старший +команда
//Set ‘TEXT AREA SET’ младший байт данных, старший +команда
//Set ‘GRAPHICS HOME ADDRESS’ младший байт данных, старший +команда
//Set ‘GRAPHICS AREA SET’ младший байт данных, старший +команда
//Set ‘MODE SET’ команда
//Set ‘DISPLAY MODE’ команда				
																
			}




//************************************************************************************************
//функция установки выводов на выход (групповая, для порта дисплея)	
void SetPinsToOut (void)			
			{

      DataLineScreenPort->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);			//сброс CNF в 00								
	    DataLineScreenPort->CRL  |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7);	//установка режима РР-GP

      CommandScreenPort->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4);			//сброс CNF в 00								
	    CommandScreenPort->CRL  |= GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3| GPIO_CRL_MODE4;	//установка режима РР-GP
			
			CommandScreenPort->BSRR = ResetDisable;						//Сброс дисплея не активен
			CommandScreenPort->BSRR = CEDisable;					  //Дисплей не активен
			CommandScreenPort->BSRR = WRDisable;					  	//Режим записи не активен
			CommandScreenPort->BSRR = RDDisable;					  //Режим чтения не активен
			
				
			}
	
//************************************************************************************************
//функция установки выводов на выход (групповая, для порта дисплея)	
void SetPinsToOut_ (void)			
			{

      DataLineScreenPort->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);			//сброс CNF в 00								
	    DataLineScreenPort->CRL  |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7);	//установка режима РР-GP
																
			}
			
//************************************************************************************************
//функция установки выводов на вход (групповая, для порта дисплея)	
void SetPinsToIn_ (void)			
			{

      DataLineScreenPort->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);			//сброс CNF в 00
			DataLineScreenPort->CRL	|=  (GPIO_CRL_CNF0_1 | GPIO_CRL_CNF1_1 | GPIO_CRL_CNF2_1 | GPIO_CRL_CNF3_1 | GPIO_CRL_CNF4_1 | GPIO_CRL_CNF5_1 | GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1);			//сброс CNF в 00	
	    DataLineScreenPort->CRL  &= ~(GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7);	//установка режима РР-GP

      //CommandScreenPort->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3 );			//сброс CNF в 00								
	    //CommandScreenPort->CRL  |= GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 ;	//установка режима РР-GP
				
			}			

//************************************************************************************************
//Функция сброса дисплея после подачи питания 

void ScreenReset (void)
		{

			Delay_ms(100);																		//пауза после подачи питания на дисплей
			CommandScreenPort->BSRR = ResetEnable;						//Сброс дисплея активен
			Delay_us(10);				  													//пауза 
			CommandScreenPort->BSRR = ResetDisable;					//Сброс дисплея не активен	
			Delay_ms(10);																		//пауза после сигнала ресет		
			CommandScreenPort->BSRR = CEEnable;					  	//Включение дисплея
			Delay_us(5);																		//Функция задержки из команды  NOP (зависит от частоты ядра)
     			
		}
//************************************************************************************************
//Функция записи байта в порт МК 

void ScreenWriteByte (uint8_t Byte)
		{
		
			DataLineScreenPort->ODR &= ~0x00FF;									//Стереть младшие 8 бит порта
			DataLineScreenPort->ODR |= Byte;									//Записать в порт младшую часть адреса 
			
			CommandScreenPort->BSRR = WREnable;					  			//Режим записи активен
			//CommandScreenPort->BSRR |= CEEnable;					  		//Дисплей активен
			Delay_nop();																			//Функция задержки из команды  NOP (зависит от частоты ядра)	
			//Delay_us(10);
			//CommandScreenPort->BSRR |= CEDisable;					  		//Дисплей не активен
			CommandScreenPort->BSRR = WRDisable;					  	//Режим записи не активен

		}
		
//************************************************************************************************
//Функция записи команды в регистр дисплея 

void ScreenWriteCommand (uint8_t Command)
		{
			
			
			ScreenStatusCheck();
			CommandScreenPort->BSRR = CDCommandEnable;					//Режим записи комманд
			ScreenWriteByte(Command);
    
		}
		
//************************************************************************************************
//Функция записи данных в регистр дисплея 

void ScreenWriteData (uint8_t Data)
		{
		
			 
			ScreenStatusCheck();	
			CommandScreenPort->BSRR = CDDataEnable;						//Режим записи данных
			ScreenWriteByte(Data);

		}

//************************************************************************************************
//Функция проверки статуса дисплея 

void ScreenStatusCheck (void)
		{
			
			volatile uint16_t Status = 0;
			
		  DataLineScreenPort->ODR &= ~0x00FF;									//Стереть младшие 8 бит порта
			SetPinsToIn_();			
			CommandScreenPort->BSRR = CDCommandEnable;								//Режим команда
			//Delay_us(5);
		  CommandScreenPort->BSRR = RDEnable;					  				//Режим чтения активен
			//CommandScreenPort->BSRR |= CEEnable;					  					//Дисплей активен
			Delay_nop();
			//Delay_us(1);																						//Функция задержки из команды  NOP (зависит от частоты ядра)			
			Status = (DataLineScreenPort->IDR&(~0xFFFC));													//Считывание данных из входного регистра МК   ??? (подумать над оптимальным считыванием)
				
			while(Status!=STATUS_COMMAND_READ_WRITE_CAPABILITY) 		//Ожидание считывания статуса готовности дисплея
					{
						Status = (DataLineScreenPort->IDR&(~0xFFFC));										//Считывание данных из входного регистра МК
																																//Здесь выход по таймауту
					}	
			
			//DataLineScreenPort->ODR &= ~0x00FF;									//Стереть младшие 8 бит порта
			
		  //CommandScreenPort->BSRR |= CEDisable;					  				//Дисплей не активен
      CommandScreenPort->BSRR = RDDisable;					  					//Режим чтения не активен
			SetPinsToOut_();
			
		}		
		
//************************************************************************************************
//Функция задержки из команды  NOP (зависит от частоты ядра)

void Delay_nop (void)
		{
		
		__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();
		
		}		
		
//************************************************************************************************
//Функция универсальная, отправка младших и старших байт данных/адресов + команда
		
void SendAddrCommand (uint8_t LowByte, uint8_t HighByte, uint8_t Command)
		{
		
			ScreenStatusCheck();															//проверить готовность дисплея (в режиме команда) 
			CommandScreenPort->BSRR = CDDataEnable;				//Режим данные
			//Delay_us(5);
			//CommandScreenPort->BSRR |= WREnable;					  	//Режим записи активен
			ScreenWriteByte (LowByte);												//Записать в порт младшую часть адреса
			ScreenStatusCheck();														//проверить готовность дисплея
			CommandScreenPort->BSRR = CDDataEnable;					//Режим данные
			//Delay_us(5);
			ScreenWriteByte (HighByte);											//Записать в порт старшую часть адреса
			ScreenStatusCheck();															//проверить готовность дисплея
			CommandScreenPort->BSRR |= CDCommandEnable;			//Режим команда
			//Delay_us(5);
			ScreenWriteByte (Command);											//Записать в порт команду записи начального адреса графики
					
		}
		
//************************************************************************************************
//Функция установки начального адреса вывода графики
		
void GraphicHomeAddr (uint8_t GraphicHomeAddrLow, uint8_t GraphicHomeAddrHigh)
		{
		
			ScreenStatusCheck();															//проверить готовность дисплея (в режиме команда) 
			CommandScreenPort->BSRR |= CDDataEnable;				//Режим данные
			//CommandScreenPort->BSRR |= WREnable;					  	//Режим записи активен
			ScreenWriteByte (GraphicHomeAddrLow);						//Записать в порт младшую часть адреса
			ScreenStatusCheck();															//проверить готовность дисплея
			CommandScreenPort->BSRR |= CDDataEnable;				//Режим данные
			ScreenWriteByte (GraphicHomeAddrHigh);						//Записать в порт старшую часть адреса
			ScreenStatusCheck();														//проверить готовность дисплея
			//CommandScreenPort->BSRR |= CDCommandEnable;			//Режим команда
			ScreenWriteByte (GRAPHIC_HOME_ADDRESS_SET);				//Записать в порт команду записи начального адреса графики
					
		}

//************************************************************************************************
//Функция установки границ вывода графики
		
void GraphicArea (uint8_t GraphicAreaLow, uint8_t GraphicAreaHigh)
		{
		
			ScreenStatusCheck();															//проверить готовность дисплея (в режиме команда) 
			CommandScreenPort->BSRR |= CDDataEnable;				//Режим данные
			//CommandScreenPort->BSRR |= WREnable;					  	//Режим записи активен
			ScreenWriteByte (GraphicAreaLow);									//Записать в порт младшую часть адреса
			ScreenStatusCheck();														//проверить готовность дисплея
			CommandScreenPort->BSRR |= CDDataEnable;					//Режим данные
			ScreenWriteByte (GraphicAreaHigh);							//Записать в порт старшую часть адреса
			ScreenStatusCheck();															//проверить готовность дисплея
			//CommandScreenPort->BSRR |= CDCommandEnable;			//Режим команда
			ScreenWriteByte (GRAPHIC_AREA_SET);							//Записать в порт команду записи начального адреса графики
			
		}
		
//************************************************************************************************
//Функция установки начального адреса вывода текста
		
void TextHomeAddr (uint8_t TextHomeAddrLow, uint8_t TextHomeAddrHigh)
		{
			
			ScreenStatusCheck();															//проверить готовность дисплея (в режиме команда) 
			CommandScreenPort->BSRR |= CDDataEnable;				//Режим данные
			//CommandScreenPort->BSRR |= WREnable;					  	//Режим записи активен
			ScreenWriteByte (TextHomeAddrLow);								//Записать в порт младшую часть адреса
			ScreenStatusCheck();														//проверить готовность дисплея
			CommandScreenPort->BSRR |= CDDataEnable;					//Режим данные
			ScreenWriteByte (TextHomeAddrHigh);							//Записать в порт старшую часть адреса
			ScreenStatusCheck();															//проверить готовность дисплея
			//CommandScreenPort->BSRR |= CDCommandEnable;			//Режим команда
			ScreenWriteByte (TEXT_HOME_ADDRESS_SET);				//Записать в порт команду записи начального адреса графики
			
		}

//************************************************************************************************
//Функция установки границ вывода текста
		
void TextArea (uint8_t TextAreaLow, uint8_t TextAreaHigh)
		{
						
			ScreenStatusCheck();															//проверить готовность дисплея (в режиме команда) 
			CommandScreenPort->BSRR |= CDDataEnable;				//Режим данные
			//CommandScreenPort->BSRR |= WREnable;					  	//Режим записи активен
			ScreenWriteByte (TextAreaLow);										//Записать в порт младшую часть адреса
			ScreenStatusCheck();														//проверить готовность дисплея
			CommandScreenPort->BSRR |= CDDataEnable;					//Режим данные
			ScreenWriteByte (TextAreaHigh);									//Записать в порт старшую часть адреса
			ScreenStatusCheck();															//проверить готовность дисплея
			//CommandScreenPort->BSRR |= CDCommandEnable;			//Режим команда
			ScreenWriteByte (TEXT_AREA_SET);								//Записать в порт команду записи начального адреса графики
			
		}
		
//************************************************************************************************
//Функция установки указателя адреса
		
void SetAddrPointer (uint8_t AddrPointerLow, uint8_t AddrPointerHigh)
		{
						
			ScreenStatusCheck();															//проверить готовность дисплея (в режиме команда) 
			CommandScreenPort->BSRR |= CDDataEnable;				//Режим данные
			//CommandScreenPort->BSRR |= WREnable;					  	//Режим записи активен
			ScreenWriteByte (AddrPointerLow);										//Записать в порт младшую часть адреса
			ScreenStatusCheck();														//проверить готовность дисплея
			CommandScreenPort->BSRR |= CDDataEnable;					//Режим данные
			ScreenWriteByte (AddrPointerHigh);									//Записать в порт старшую часть адреса
			ScreenStatusCheck();															//проверить готовность дисплея
			//CommandScreenPort->BSRR |= CDCommandEnable;			//Режим команда
			ScreenWriteByte (ADDRESS_POINTER_SET);							//Записать в порт команду записи начального адреса графики
			
		}		





