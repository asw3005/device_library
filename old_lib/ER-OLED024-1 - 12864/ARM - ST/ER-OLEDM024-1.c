/*Display - ERC12864*/
/*Controller - NT7538*/
#include "stm32f10x.h"
#include "Delay.h"
#include "ER-OLEDM024-1.h"
#include "ER-OLEDM024-1-12864SUMBOL.h"

			
//uint8_t i=0;	
volatile uint8_t PageCash = 0;


//************************************************************************************************
//Инициализация дисплея
void InitScreen (void)
		{

			SetPinsToOut(); 																//настройка используемых дисплеем выводов
			ScreenReset();																		//Дисплей RESET
			Delay_nop();			
			CommandScreenPort->BSRR = DCCommandEnable;			//Режим записи комманд
			
			ScreenWriteByte(DISPLAY_OFF);									//отключить дисплей (?)			
			Delay_nop();																		//пауза
				
			ScreenWriteByte(DIVIDE_RATIO_OSC_FREQUENCY);	//делитель 1, частота работы генератора максимум
			ScreenWriteByte(DIVIDE1_FREQMAX_SET);						//
			Delay_nop();																	//пауза
		
			ScreenWriteByte(MULTIPLEX_RATIO_SET);						//
			ScreenWriteByte(MULTIPLEX_64MUX);							//управление 64 строками
			Delay_nop();																		//пауза
			
			ScreenWriteByte(DISPLAY_OFFSET_SET); 					//
			ScreenWriteByte(OFFSET); 												//
			Delay_nop();																	//пауза
			
			ScreenWriteByte(MASTER_CONFIGURATION_SET);			//режим настройки	
			ScreenWriteByte(ENABLE_EXTERNAL_POWER_SUPPLY);//включить питание от внешнего источника питания
			Delay_nop();																			//пауза
			
			ScreenWriteByte(COLOR_AND_LOWPOWER_MODE_SET);//повышающий преобразователь включён, регулировка включена, питание включено
			ScreenWriteByte(COLOR_AND_LOWPOWER_MODE);			//включен режим малого потребления
			Delay_nop();																			//пауза
			
			ScreenWriteByte(SEGMENT_REMAP_COLUMN131_SEG0);	//
			ScreenWriteByte(SCAN_DIRECTION_COMn_COM0_SET);
			Delay_nop();																			//пауза
			
			ScreenWriteByte(COM_PINS_SET);									//
			Delay_nop();																			//пауза
			
			ScreenWriteByte(LOOK_UP_TABLE_SET);						//
			ScreenWriteByte(CURRENT_DRIVE_PULSE);						//
			ScreenWriteByte(CURRENT_DRIVE_PULSE);					//
			ScreenWriteByte(CURRENT_DRIVE_PULSE);						//
			ScreenWriteByte(CURRENT_DRIVE_PULSE);					//
			Delay_nop();																		//пауза
					
			ScreenWriteByte(CONTRAST_CONTROL_BANK0_SET);				//
			ScreenWriteByte(CONTRAST_LEVEL_BANK0);									//
			Delay_nop();																				//пауза
			ScreenWriteByte(BRIGHTNESS_AREA_COLOR_BANKS_SET);		//
			ScreenWriteByte(CONTRAST_LEVEL_BANKCOLOR);				//
			Delay_nop();																				//пауза
			
			ScreenWriteByte(PRECHARGE_PERIOD_SET);				//
			Delay_nop();
			
			ScreenWriteByte(VCOMH_SET);										//установка напряжения логиского нуля
			
			ScreenWriteByte(PAGE_START_END_ADDRESS_SET);
			ScreenWriteByte(0);
			ScreenWriteByte(7);
			Delay_nop();
			
			//ScreenWriteByte(PAGE_ADDRESSING_MODE);				//
			ScreenWriteByte(INVERSE_DISPLAY_PIXEL_SET);				//
			ScreenWriteByte(ENTIRE_DISPLAY_OFF);					//
			ScreenWriteByte(DISPLAY_ON_NORMAL_BRIGHTNESS);		//
	

			

		}

//************************************************************************************************
//Очистка дисплея	програмная постраничная	
void ClearPageScreenProg (uint8_t Page)
		{

			uint8_t k=0;
			
			SetPage(Page);
			SetColumn(1);

					for (k=0;k<=127;k++) 
							
						{
							ScreenWriteData(0xFF);													
						}						
											
		}

//************************************************************************************************
//Заливка дисплея	програмная постраничная	
void GroundPageScreenProg (uint8_t Page)
		{
					
			uint8_t k=0;
	
			SetPage(Page);
			SetColumn(1);
						
					for (k=0;k<=127;k++) //кол-во точек в строке 
							
						{
							ScreenWriteData(0x00);																					
						}
						
		}	
		
		
//************************************************************************************************
//Заливка дисплея	аппаратная  - работает только с аппаратной очисткой	
void GroundPageScreenHard (uint8_t Page)
		{
			
			ScreenWriteByte(ENTIRE_DISPLAY_ON);
			Delay_nop();				

		}
		
		
		
//************************************************************************************************
//Очистка дисплея	аппаратная  - работает только с аппаратной заливкой	
void ClearPageScreenHard (uint8_t Page)
		{

			ScreenWriteByte(ENTIRE_DISPLAY_OFF);
			Delay_nop();

		}			


//************************************************************************************************
//Вывод символов на дисплей, Страница(строка широкая, в точках, 5 ширина, 8 высота);  0-63 точки; 
//позиция - координата икс, 0 - 127 точек; кол-во символов на строке 5 на 8 точек	
void SendCharToScreen (uint8_t Page, uint8_t Position, uint8_t Quantity)
		{
					
			uint8_t k = 0;
			uint8_t QSymbol = 0;
			uint8_t i = 0;
			
			SetPage(Page);
			SetColumn(Position);
			
			
			
			QSymbol = (CharPoints*Quantity)-1;				//кол-во точек, приходящееся на выводимое кол-во символов
			if (Quantity==0){QSymbol=0;}
				
					for (k=0;k<=QSymbol;k++) //кол-во точек в строке 
							
						{

							ScreenWriteData(Font[i++]);
					
							if (i==54) {i=0;}													//кол-во элементов в массиве
																
						}

						SetColumn(1);

		}		
		
//************************************************************************************************
//Отправка текстового сообщения на дисплей							
void SendText(uint8_t *ASCII_Code, uint8_t Page, uint8_t Position)
{
		uint8_t i,j;
	
   for(i=0;i<9;i++)
  {
    //LCDSendChar(ASCII_Code[i]);
		SendOneChar(Page, Position, ASCII_Code[i]);
		j++;
  }
}		
		
//************************************************************************************************
//Вывод символа на дисплей 
//позиция - местоположение по горизонтали 	
void SendOneChar (uint8_t Page, uint8_t Position, uint8_t ASCII_Code)
		{
					
			uint8_t k = 0;
			uint8_t Temp_0 = 0;
			uint8_t QSymbol = 0;
		
			SetPage(Page);																			//установка страницы вывода символа	
			SetColumn(Position);																	//установка начальной позиции (по горизонтали)
			
			//QSymbol = CharPoints - 1;			
			QSymbol = 4;																				//кол-во точек, приходящееся на выводимое кол-во символов (ширина символа)
			Temp_0 = SCREEN_EN_RUS_ASCI_WIN1251_v1[ASCII_Code];		//получение смещения для таблицы символов из пришедшего кода ASCII WIN1251																		

						for (k=0;k<=QSymbol;k++) 											//кол-во точек, из которого состоит символ (кол-во байт по вертикали, т.к. у дисплея вертикальная адресации) 
								
								{										
									ScreenWriteData(Font[Temp_0++]); 					//из таблицы аски кодов получаем смещение, по полученному смещению из таблицы символов
								}

			SetColumn(1);																				//вернуть положение по горизонтали в исходное состояние

		}
		
		

		
//************************************************************************************************
//Установка текущей страницы дисплея		
void SetPage (uint8_t Page)
		{
			
			ScreenWriteCommand(PAGE1_ADDRESS_SET+(Page-1));
			
			//CommandScreenPort->BSRR = DCCommandEnable;			//Режим записи комманд
			//ScreenWriteByte(PAGE1_ADDRESS_SET+(Page-1));			//Установить стартовую страницу, минус 1 для приведения номеров страниц к удобному виду
			//Delay_nop();//при использовании закоментированной верхней строки эта задержка не нужна
		}
	
//************************************************************************************************
//Установка текущего столбца(вертикаль) дисплея		
void SetColumn (uint8_t Column)
		{
			CommandScreenPort->BSRR = DCCommandEnable;												//Режим комманд
			ScreenWriteByte(COLUMN_START_ADDRES_SET_HIGH+((Column+1)>>4));				//формирование адреса столбцов, координата Х
			ScreenWriteByte(COLUMN_START_ADDRES_SET_LOW+((Column+1) & (~0xF0)));		
			Delay_nop();//при использовании закоментированной верхней строки эта задержка не нужна	
		}
		

		
//************************************************************************************************
//функция установки выводов на выход (групповая, для порта дисплея)	
void SetPinsToOut (void)			
			{

      DataLineScreenPort->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);			//сброс CNF в 00								
	    DataLineScreenPort->CRL  |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7);	//установка режима РР-GP

      CommandScreenPort->CRH	&= ~(GPIO_CRH_CNF9 | GPIO_CRH_CNF10 | GPIO_CRH_CNF11 | GPIO_CRH_CNF12 | GPIO_CRH_CNF15);			//сброс CNF в 00								
	    CommandScreenPort->CRH  |= GPIO_CRH_MODE9 | GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_MODE12| GPIO_CRH_MODE15;	//установка режима РР-GP
			
			CommandScreenPort->BSRR = ResetDisable;						//Сброс дисплея не активен
			CommandScreenPort->BSRR = CSDisable;					  //Дисплей не активен
			CommandScreenPort->BSRR = WRDisable;					  	//Режим записи не активен
			CommandScreenPort->BSRR = RDDisable;					  //Режим чтения не активен
			
				
			}
			
			
//************************************************************************************************
//Функция сброса дисплея после подачи питания 
void ScreenReset (void)
		{
			
			CommandScreenPort->BSRR = ResetDisable;						//Сброс дисплея не активен
			Delay_ms(50);																	//пауза после подачи питания на дисплей
			CommandScreenPort->BSRR = ResetEnable;						//Сброс дисплея активен
			Delay_ms(1);																	//пауза после подачи питания на дисплей
			CommandScreenPort->BSRR = ResetDisable;						//Сброс дисплея не активен	
			Delay_ms(1);																		//пауза после сигнала ресет		
			CommandScreenPort->BSRR = CSEnable;					  		//Включение дисплея
			Delay_us(1);																		//Функция задержки из команды  NOP (зависит от частоты ядра)
     			
		}
			
//************************************************************************************************
//Функция отпраки команды дисплею 	
void ScreenWriteCommand (uint8_t Command)
		{		
				CommandScreenPort->BSRR = DCCommandEnable;//Режим комманд
				ScreenWriteByte(Command);
				//Delay_nop();
		}	

//************************************************************************************************
//Функция отправки данных на дисплей 
void ScreenWriteData (uint8_t Data)
		{		
				CommandScreenPort->BSRR = DCDataEnable;//Режим комманд
				ScreenWriteByte(Data);
				//Delay_nop();
		}


//************************************************************************************************
//Функция записи байта в порт МК (раскоментировать, если используется связь по i8080) 
void ScreenWriteByte (uint8_t Byte)
		{
		
			DataLineScreenPort->ODR &= ~0x00FF;									//Стереть младшие 8 бит порта
			DataLineScreenPort->ODR |= Byte;									//Записать в порт  
			
			CommandScreenPort->BSRR = WREnable;					  			//Режим записи активен
			//CommandScreenPort->BSRR |= CEEnable;					  		//Дисплей активен
			Delay_nop();																			//Функция задержки из команды  NOP (зависит от частоты ядра)	
			//Delay_us(10);
			//CommandScreenPort->BSRR |= CEDisable;					  		//Дисплей не активен
			CommandScreenPort->BSRR = WRDisable;					  	//Режим записи не активен
			Delay_nop();
			DataLineScreenPort->ODR &= ~0x00FF;									//Стереть младшие 8 бит порта
			
			
		}


////************************************************************************************************
////Функция записи байта в порт МК (раскоментировать, если используется связь по SPI)
//void ScreenWriteByte (uint8_t Byte)
//		{
//		
//				volatile uint8_t i;
//				uint8_t SendByte;
//			
//				SendByte = (~(0x7F) & Byte);
//					
//				for (i=0; i<8; i++)
//						{
//							if (SendByte == 0x80)
//									{
//										DataLineScreenPort->BSRR = DataHigh;
//									}
//							else
//									{										
//										DataLineScreenPort->BSRR = DataLow;
//										
//									}
//				
//							DataLineScreenPort->BSRR = ClockHigh;
//							Delay_nop ();
//							DataLineScreenPort->BSRR = ClockLow;
//							Delay_nop ();
//							//DataLineScreenPort->BSRR = DataLow;
//							
//							Byte = Byte<<1;					
//							SendByte = (~(0x7F) & Byte);			
//													
//						}
//		
//		}

//************************************************************************************************
//Функция задержки из команды  NOP (зависит от частоты ядра)
void Delay_nop (void)
		{
		
		__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();
		//__NOP();__NOP();__NOP();
		//__NOP();__NOP();__NOP();
		//__NOP();__NOP();__NOP();
		//__NOP();__NOP();__NOP();
		
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
			DataLineScreenPort->CRL	|=  (GPIO_CRL_CNF0_1 | GPIO_CRL_CNF1_1 | GPIO_CRL_CNF2_1 | GPIO_CRL_CNF3_1 | GPIO_CRL_CNF4_1 | GPIO_CRL_CNF5_1 | GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1);							//сброс CNF в 00	
	    DataLineScreenPort->CRL  &= ~(GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7);	//установка режима РР-GP
		
			}	

//************************************************************************************************
//Функция записи байта в порт МК 

uint8_t ScreenDataRead (void)
		{
			uint8_t Temp_0;
			
			CommandScreenPort->BSRR = DCCommandEnable;	//Режим комманд
			ScreenWriteByte(READ_MODIFY_WRITE_ON);		//включить режим "чтение-модификация-запись"
			SetPinsToIn_();															//установка выводов МК на вход для приёма данных
			CommandScreenPort->BSRR = DCDataEnable;		//Режим данных
			CommandScreenPort->BSRR = RDEnable;					//режим чтения данных активен   //два чтения подряд, первое чтение не используется
			//__NOP();
			CommandScreenPort->BSRR = RDDisable;			//режим чтения данных не активен	//в рам хранится предыдущее значение байта относительно того, что показано на экране
			CommandScreenPort->BSRR = RDEnable;					//режим чтения данных активен
			
			Delay_nop ();															//пауза
			Temp_0 = (DataLineScreenPort->IDR);						//приём данных от дисплея
			CommandScreenPort->BSRR = RDDisable;			//режим чтения данных не активен				
			SetPinsToOut_();														//установка выводов МК на выход для передачи данных
			
			return(Temp_0);
	
		}

//************************************************************************************************
// Закрасить выбранный пиксель
void PixelSetReset (uint8_t Horizontal, uint8_t Vertical, uint8_t SetClrBit)
		{
			uint8_t BitNumber =0, PageNumber =0, NumbPicsel =0, Temp_0 = 0;
		
			
			if (Vertical<PageWidth)
					{
						BitNumber = Vertical;																//номер бита
						PageNumber = 1;																				//номер страницы					
					}
			else
					{
									
							BitNumber = (Vertical - (8*(Vertical/PageWidth)));//вычисление номера бита, PageWidth = 8
							
							if (BitNumber == 0)																	//если 0, то число получаемых страниц целое
									{
										BitNumber = PageWidth;											//номер бита = координата по вертикали (y) делить на ширину страницы
										PageNumber = Vertical/PageWidth;							//номер страницы
									}
									
							else 
									{
										Temp_0 = Vertical/PageWidth;								//номер бита = координата по вертикали (y) делить на ширину страницы
										PageNumber = 	Temp_0 + 1;											//номер страницы							
									}
						}
						
			NumbPicsel = NumberPicselArr[BitNumber-1];								//определение байта с нужным пикселем по номеру найденного бита
				
			SetColumn(Horizontal);																			//установить адрес столбцов
			SetPage(PageNumber);																			//установить адрес страницы					

			if (SetClrBit == 1)																				//если единица, то пиксель устанавливается
					{	
						Temp_0 = (ScreenDataRead() & (~NumbPicsel));				//наложение считанных данных и устанавливаемого бита
					}
			else																												//иначе пиксель стирается 
					{
						Temp_0 = (ScreenDataRead() | NumbPicsel);						//наложение считанных данных и стираемого бита
					}
			
			ScreenWriteByte(Temp_0);																		//запись модифицированного пикселя
			ScreenWriteCommand (READ_MODIFY_WRITE_OFF);								//отключить режим "чтение-модификация-запись"
					
		}	 

//************************************************************************************************
//	
void Line (uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2)
		{
			
			uint8_t x = 0, y =0;
			
			
			
				for (y=Y1;y<=Y2;y++)
							{
									for (x=X1;x<=X2;x++)
								
											{
												
												Delay_ms(100);
												PixelSetReset (x,y,1);
																						
											}
											
//										for (x=X1;x<=X2;x++)
//								
//											{
//												
//												Delay_ms(100);
//												PixelSetReset (x,y,0);
//																						
//											}		
							//if (y == Y2) { y = Y1;}							
							
							}
		}		
		



