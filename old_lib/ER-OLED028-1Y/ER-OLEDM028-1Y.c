/*Display - ER-OLEDM028-1Y*/
/*Controller - SSD1322*/
#include "stm32f10x.h"
#include "Delay.h"
#include "ER-OLEDM028-1Y.h"
#include "ER-OLEDM028-1Y-25664SUMBOL.h"

uint8_t FourPixel[2];
			
//************************************************************************************************
//Инициализация дисплея
void InitScreen (void)
		{

			SetPinsToOut(); 																										// Настройка используемых дисплеем выводов
			ScreenReset();																												// Дисплей RESET		
		
			ScreenWriteCommand(SET_COMMAND_LOCK);																// Команда настройки режима питания
			ScreenWriteData(UNLOCK_OLED_DRIVER);																	// Включить режим питания от внешнего источника

			ScreenWriteCommand(SLEEP_ON); 																			//Команда включения режима сна

			ScreenWriteCommand(SET_CLOCK_DIVIDER_OSCILLATOR_FRAQUENCY);						// Команда установки делителя и частоты генератора
			ScreenWriteData(DIVSET_OSCFREQ);																		//Значение делителя и частоты 

			ScreenWriteCommand(SET_MUX_RATIO); 																		// Команда установки диапазона сканируемых строк
			ScreenWriteData(DEFAULT_MUX_RATIO); 																// Кол-во сканируемых строк, 64 

			ScreenWriteCommand(SET_DISPLAY_OFFSET); 															// Команда установки смещения адреса начальной строки 
			ScreenWriteData(DEFINE_DISPLAY_OFFSET);															// Смещение адреса начальной строки по умолчанию

			ScreenWriteCommand(SET_DISPLAY_START_LINE); 													// Команда установки начальной строки в памяти дисплея
			ScreenWriteData(DEFINE_START_LINE); 																// Адрес начальной строки по умолчанию

			ScreenWriteCommand(SET_REMAP_AND_DUAL_COM_LINE); 											// Инкремент адреса горизонтальный, направление столбцов 0-256, 
																																						//ниббл ремап отключён, сканирование  COM64-COM0, раделение на чётные нечётные номера строк отключено
			//ScreenWriteData(ADDR_INC_COL_REMAP_COM_SCAN_COM_SPLIT_DUAL_COM);
      ScreenWriteData(ADDR_INC_COL_REMAP_COM_SCAN_COM_SPLIT_DUAL_COM);			
			ScreenWriteData(DUAL_COM_LINE_ENABLE);

			/*Write_Instruction(0xB5); //GPIO Write_Instruction(0x00); */
			ScreenWriteCommand(FUNCTION_SELECTION); 														// Команда установки источника питания 
			ScreenWriteData(ENABLE_EXTERNAL_POWER_SUPPLY);												// Выбрать внешний источник питания

			ScreenWriteCommand(0xB4); 																					//Display Enhancement A 
			ScreenWriteData(0xA0);
			ScreenWriteData(0xfd); 

			ScreenWriteCommand(SET_CONTRAST_CURRENT); 														// Значение тока для установки контраста
			ScreenWriteData(DEFAULT_CONTRAST_CURRENT); 

//			ScreenWriteCommand(MASTER_CONTRAST_CURRENT_CONTROL); /*master contrast current control*/ 
//			ScreenWriteData(0x0F); 

			ScreenWriteCommand(SELECT_LINEAR_GRAY_SCALE_TABLE);									// Команда включения предустановленной таблици яркости с линейным изменением 
			ScreenWriteCommand(SET_PHASE_LENGTH); 																// Команда установки параметров фаз управления
			ScreenWriteData(PHASE_TWO_ONE);																			// По умолчанию фаза 2 - 7, фаза 1 - 9 

			ScreenWriteCommand(0xD1); 																						//Set Display Enhancement B
			ScreenWriteData(0x82); 
			ScreenWriteData(0x20); 

			ScreenWriteCommand(SET_PRECHARGE_VOLTAGE); 													// Команда установки напряжения предзаряда 
			ScreenWriteData(PRECHARGE_VOLTAGE);																		// Значение напряжения предзаряда

			ScreenWriteCommand(SET_SECOND_PRECHARGE_PERIOD);									 // Команда установки времени второго периода предзаряда ёмкости диода матрицы
			ScreenWriteData(SECOND_PRECHARGE_PERIOD); 														// Время второго периода предзаряда

			ScreenWriteCommand(SET_VOLTAGE_COM_DESELECT); 									   // SET VCOMH */ 
			ScreenWriteData(VOLTAGE_COM_DESELECT); 

			ScreenWriteCommand(NORMAL_DISPLAY_PIXEL_SET); 												// Пиксели дисплея устанавливаются записью 1
			ClearScreen();																										 // Очистка видеопамяти
			ScreenWriteCommand(SLEEP_OFF); 																				// Команда выключения режима сна
            
		
		}

//************************************************************************************************
//Заливка дисплея		
void SetScreen(void)
{
	volatile uint8_t NumberLines, NumberBytes;		

	SetColumn(1); 
	SetPage(1);
	ScreenWriteCommand(WRITE_RAM_COMMAND);																					// 
	DCDataEnable;//Режим данных
	for(NumberLines = 0; NumberLines < 1; NumberLines++) 
			{
			 for(NumberBytes = 0; NumberBytes < 128; NumberBytes++) 			
					{ 
						ScreenWriteByte(0xFF);
						//Delay_ms(1);
					} 
			}					
					
}		
		
//************************************************************************************************
//Очистка дисплея	
void ClearScreen(void)
{
	volatile uint8_t NumberLines, NumberBytes;		

	SetColumn(1);
	SetPage(1);
	ScreenWriteCommand(WRITE_RAM_COMMAND);																									//
	DCDataEnable;//Режим данных
	for(NumberLines = 0; NumberLines < 64; NumberLines++) 
			{
			 for(NumberBytes = 0; NumberBytes < 128; NumberBytes++) 		
					{ 
						ScreenWriteByte(0x00);
						Delay_us(10);
					} 
			}					
						
}		
		
//************************************************************************************************
//Очистка дисплея	програмная постраничная	
void ClearPageScreenProg(uint8_t Page)
{

	uint8_t NumberLines, NumberBytes;		

	SetColumn(1); 
	SetPage(Page); 
	ScreenWriteCommand(WRITE_RAM_COMMAND); 
	DCDataEnable;//Режим данных
	for(NumberLines = 0; NumberLines < 8; NumberLines++) 
			{
			 for(NumberBytes = 0; NumberBytes < 128; NumberBytes++) 		
					{ 				
						ScreenWriteByte(0x00); 
						Delay_ms(1);
					} 
			}					
							
}

//************************************************************************************************
//Заливка дисплея	програмная постраничная	
void GroundPageScreenProg(uint8_t Page)
{
	uint8_t NumberLines, NumberBytes;		

	SetColumn(1);
	SetPage(Page); 
	ScreenWriteCommand(WRITE_RAM_COMMAND); 
	DCDataEnable;//Режим данных
	for(NumberLines = 0; NumberLines < 8; NumberLines++) 
			{
			 for(NumberBytes = 0; NumberBytes < 128; NumberBytes++) 	
					{ 				
						ScreenWriteByte(0xFF); 
						Delay_ms(1);
					} 
			}
		
}			

//************************************************************************************************
//Вывод символа на дисплей 
//позиция - местоположение по горизонтали 	
void SendOneChar (uint8_t Page, uint8_t Position, uint8_t ASCII_Code)
		{
					
			uint8_t i, j, k, Temp_1, SimbolOffsetRow, FontByte, QSymbol ;
			uint16_t Temp_0;
			uint8_t RowOffset,SendByte = 0;
			//uint8_t QSymbol = 0;
			//uint8_t SimbolOffsetRow, FontByte;
			SimbolOffsetRow = 0;
			
			SetPage(Page);																					//установка страницы вывода символа	
			SetColumn(Position);																			//установка начальной позиции (по горизонтали)

			QSymbol = 8;																						//кол-во байт, приходящееся на выводимый символ
			Temp_0 = SCREEN_EN_RUS_ASCI_WIN1251_v1[ASCII_Code];				//получение смещения для таблицы символов из пришедшего кода ASCII WIN1251																		

						for (k=0;k<QSymbol;k++) 													//кол-во байт, из которого состоит символ (кол-во байт по горизонтали, т.к. у дисплея горизонтальная адресация) 
								
								{	
									ScreenWriteCommand(WRITE_RAM_COMMAND); 
									DCDataEnable;//Режим данных
									FontByte = ASCII_Font[Temp_0]; 										//из таблицы аски кодов получаем смещение, по полученному смещению из таблицы символов	
									Temp_0++;
									for (i=0; i<4; i++)													// Передача 4-х байт данных 8-ми пикселей (2 пикселя - 1 байт)
											{	
												SendByte = 0;												
												for (j=0; j<2;j++)											// Формирование и передача значения 2-х пикселей
														{
															SendByte = SendByte<<4;					// Сдвинуть влево байт данных состояния пикселей на 4
															Temp_1 = (FontByte & (~0x7F));	// Выделить старший бит с байта символа
															
															if (Temp_1 == 0x80)								// Если бит  равен 1, то
																	{
																		SendByte |= 0x0F; 				// Записать в ниббл значение F (может быть переменной от 0 до 15, т.к. является значением яркости отдельного пикселя)
																	}
															else															//Иначе
																	{
																		SendByte |= 0x00;					// Записать в ниббл значение 0
																	}
															FontByte = FontByte<<1;						// Сдвинуть влево байт данных шрифта на 1

														}		
														//SendByte = 0;
														ScreenWriteByte(SendByte);					// Записать 2 пикселя в память дисплея
											}						
									//SetPage(Page+1);																			//установка строк вывода символа
								  RowOffset = ((Page-1)*PageHigh);
									ScreenWriteCommand(SET_ROW_ADDRESS); 
									ScreenWriteData(ROW_START_ADDRESS+RowOffset+(++SimbolOffsetRow)); 
									ScreenWriteData(ROW_END_ADDRESS);
								  SetColumn(Position);
											
								}
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
//Заливка дисплея	аппаратная  - работает только с аппаратной очисткой	
void GroundPageScreenHard(void)
{
	ScreenWriteByte(ENTIRE_DISPLAY_ON);
	Delay_nop();				
}
		
		
		
//************************************************************************************************
//Очистка дисплея	аппаратная  - работает только с аппаратной заливкой	
void ClearPageScreenHard(void)
{
	ScreenWriteByte(ENTIRE_DISPLAY_OFF);
	Delay_nop();
}	

//************************************************************************************************
//Установка текущей страницы дисплея		
void SetPage (uint8_t Page)
{
	uint8_t RowOffset;
	
	RowOffset = ((Page-1)*PageHigh);
	ScreenWriteCommand(SET_ROW_ADDRESS); 
	ScreenWriteData(ROW_START_ADDRESS+RowOffset); 
	ScreenWriteData(ROW_END_ADDRESS);
}
	
//************************************************************************************************
//Установка текущего столбца(вертикаль) дисплея		
void SetColumn (uint8_t Column)
{
	uint8_t ColumnOffset;
	
	ColumnOffset = (Column-1);
	ScreenWriteCommand(SET_COLUMN_ADDRESS); 
	ScreenWriteData(COLUMN_START_ADDRESS+ColumnOffset); 
	ScreenWriteData(COLUMN_END_ADDRESS);	
}
			
//************************************************************************************************
//функция установки выводов на выход (групповая, для порта дисплея)	
void SetPinsToOut(void)			
{
	DataLineScreenPort->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);			//сброс CNF в 00								
	DataLineScreenPort->CRL  |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7);	//установка режима РР-GP

	CommandScreenPort->CRH	&= ~(GPIO_CRH_CNF9 | GPIO_CRH_CNF10 | GPIO_CRH_CNF11 | GPIO_CRH_CNF12 | GPIO_CRH_CNF15);			//сброс CNF в 00								
	CommandScreenPort->CRH  |= GPIO_CRH_MODE9 | GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_MODE12| GPIO_CRH_MODE15;	//установка режима РР-GP

	ResetDisable;						//Сброс дисплея не активен
	CSDisable;					  //Дисплей не активен
	WRDisable;					  	//Режим записи не активен
	RDDisable;					  //Режим чтения не активен
}
			
			
//************************************************************************************************
//Функция сброса дисплея после подачи питания 
void ScreenReset(void)
{
	ResetDisable;						//Сброс дисплея не активен
	Delay_ms(50);																	//пауза после подачи питания на дисплей
	ResetEnable;						//Сброс дисплея активен
	Delay_ms(1);																	//пауза после подачи питания на дисплей
	ResetDisable;						//Сброс дисплея не активен	
	Delay_ms(1);																		//пауза после сигнала ресет		
	CSEnable;					  		//Включение дисплея
	Delay_us(1);																		//Функция задержки 	
}
			
//************************************************************************************************
//Функция отпраки команды дисплею 	
void ScreenWriteCommand(uint8_t Command)
{		
	DCCommandEnable;//Режим комманд
	ScreenWriteByte(Command);
	//Delay_nop();
}	

//************************************************************************************************
//Функция отправки данных на дисплей 
void ScreenWriteData(uint8_t Data)
{		
	DCDataEnable;//Режим данных
	ScreenWriteByte(Data);
	//Delay_nop();
}


//************************************************************************************************
//Функция записи байта в порт МК (раскоментировать, если используется связь по i8080) 
void ScreenWriteByte(uint8_t Byte)
{	
	DataLineScreenPort->ODR &= ~0x00FF;									//Стереть младшие 8 бит порта
	DataLineScreenPort->ODR |= Byte;									//Записать в порт  			
	WREnable;					  																//Режим записи активен
	Delay_nop();																			//Функция задержки из команды  NOP (зависит от частоты ядра)
	WRDisable;					  															//Режим записи не активен
	Delay_nop();
	DataLineScreenPort->ODR &= ~0x00FF; 							//Стереть младшие 8 бит порта	
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
//										DataHigh;
//									}
//							else
//									{										
//										DataLow;				
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
//Функция чтения байта 
//void ScreenReadByte (uint8_t Byte)
//{	
//	DataLineScreenPort->ODR &= ~0x00FF;									//Стереть младшие 8 бит порта
//	DataLineScreenPort->ODR |= Byte;									//Записать в порт  			
//	RDEnable;					  																//Режим записи активен
//	Delay_nop();																			//Функция задержки из команды  NOP (зависит от частоты ядра)
//	RDDisable;					  															//Режим записи не активен
//	Delay_nop();
//	DataLineScreenPort->ODR &= ~0x00FF; 							//Стереть младшие 8 бит порта	
//}

//************************************************************************************************
//Функция задержки из команды  NOP (зависит от частоты ядра)
void Delay_nop (void)
{
	__NOP();__NOP();__NOP();// по факту вставлено 5 нопов (2 штуки на 1) 
	__NOP();__NOP();__NOP();//
	//__NOP();__NOP();__NOP();//
	//__NOP();__NOP();__NOP();
	//__NOP();__NOP();__NOP();
}	
		


//************************************************************************************************
//функция установки выводов на выход (групповая, для порта дисплея)	
void SetPinsToOut_(void)			
{
	DataLineScreenPort->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);			//сброс CNF в 00								
	DataLineScreenPort->CRL  |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7);	//установка режима РР-GP												
}
			
//************************************************************************************************
//функция установки выводов на вход (групповая, для порта дисплея)	
void SetPinsToIn_(void)			
{
	DataLineScreenPort->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);			//сброс CNF в 00
	DataLineScreenPort->CRL	|=  (GPIO_CRL_CNF0_1 | GPIO_CRL_CNF1_1 | GPIO_CRL_CNF2_1 | GPIO_CRL_CNF3_1 | GPIO_CRL_CNF4_1 | GPIO_CRL_CNF5_1 | GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1);							//сброс CNF в 00	
	DataLineScreenPort->CRL  &= ~(GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7);	//установка режима РР-GP
}	

//************************************************************************************************
//Функция записи байта в порт МК 

uint8_t ScreenDataRead(void)
		{	
			uint8_t Data;

			ScreenWriteCommand(READ_RAM_COMMAND);				// Команда включения режима чтения данных из памяти дисплея		
			SetPinsToIn_();																//установка выводов МК на вход для приёма данных
			DCDataEnable;																//Режим данных
			
			RDEnable;																		//два чтения подряд, первое чтение не используется
			Delay_nop();																	//пауза
			RDDisable;				//режим чтения данных не активен	//в рам хранится предыдущее значение байта относительно того, что показано на экране
			
			RDEnable;																		//режим чтения данных активен
			Delay_nop();																	//пауза
			FourPixel[0] = (DataLineScreenPort->IDR);			//приём данных от дисплея
			RDDisable;																	//режим чтения данных не активен
			__NOP();
			RDEnable;																			//режим чтения данных активен
			Delay_nop();																//пауза
			FourPixel[1] = (DataLineScreenPort->IDR);			//приём данных от дисплея
			RDDisable;																	//режим чтения данных не активен			
			SetPinsToOut_();															//установка выводов МК на выход для передачи данных	
			
			return(Data);
		}

//************************************************************************************************
// Закрасить выбранный пиксель
void PixelSetReset (uint16_t Horizontal, uint16_t Vertical, uint8_t SetClrBit)
		{
			uint8_t ColumnAddrCounter = 0, SendByte_0 = 0, SendByte_1 = 0;
			int16_t NumberBit = 0;
		
			SendByte_0 =0;
			SendByte_1 =0;
			
			NumberBit = Horizontal;
			
			while(NumberBit >= 0)
					{
						NumberBit = NumberBit - 4;
						ColumnAddrCounter++;
					}
			NumberBit = (2*(NumberBit + 4)); 	// Остаток
			//*************************************************************
			// Установка адресов считывания текущего состояния 4-х пикселей	
					
			ScreenWriteCommand(SET_ROW_ADDRESS); 
			ScreenWriteData(ROW_START_ADDRESS+(Vertical-1)); 
			ScreenWriteData(ROW_END_ADDRESS);		
					
			if(NumberBit > 0)																		
					{					
						ScreenWriteCommand(SET_COLUMN_ADDRESS); 
						ScreenWriteData(COLUMN_START_ADDRESS+(ColumnAddrCounter-1)); 
						ScreenWriteData(COLUMN_END_ADDRESS); 	
					}
			else																							// 
					{
						ColumnAddrCounter = ColumnAddrCounter - 1;
						ScreenWriteCommand(SET_COLUMN_ADDRESS); 
						ScreenWriteData(COLUMN_START_ADDRESS+(ColumnAddrCounter-1)); 
						ScreenWriteData(COLUMN_END_ADDRESS);
						ColumnAddrCounter++;
					}
					
			//ScreenDataRead();																	// Считать состояние 4х пикселей по адресу закраски нужного пикселя
			//SendByte_0 = ScreenDataRead();		// Пиксели 1-3
			//SendByte_1 = ScreenDataRead();		// Пиксели 1-3
					
			SendByte_0 = PixelArr[NumberBit];
		  SendByte_1 = PixelArr[NumberBit+1];		
			ScreenDataRead();		
					
			if(SetClrBit == 1)
					{
						SendByte_0 = (FourPixel[0] | SendByte_0);		// 
						SendByte_1 = (FourPixel[1] | SendByte_1);		//
					}
			else	
					{			
						SendByte_0 =  (FourPixel[0] & (~SendByte_0));		// 
						SendByte_1 =  (FourPixel[1] & (~SendByte_1));		//
					}
								
			// Склеить текущее состояние с необходимым (преобразованные полубайты?)
					
			//*****************************************************************
			// Установка адресов записи нового состояния одного из 4-х пикселей	
					
			ScreenWriteCommand(SET_ROW_ADDRESS); // После считывания по адресу столбца 64 произодёт инкремент строки, потому нужно восстановить адрес строки и столбца перед записью
			ScreenWriteData(ROW_START_ADDRESS+(Vertical-1)); 
			ScreenWriteData(ROW_END_ADDRESS);		
					
			if(NumberBit > 0)																		// Закраска пикселей 1-3 в полубайте
					{					
						ScreenWriteCommand(SET_COLUMN_ADDRESS); 
						ScreenWriteData(COLUMN_START_ADDRESS+(ColumnAddrCounter-1)); 
						ScreenWriteData(COLUMN_END_ADDRESS);
					}
			else																							// Закраска 4-го пикселя в полубайте
					{
						ColumnAddrCounter = ColumnAddrCounter - 1;
						ScreenWriteCommand(SET_COLUMN_ADDRESS); 
						ScreenWriteData(COLUMN_START_ADDRESS+(ColumnAddrCounter-1)); 
						ScreenWriteData(COLUMN_END_ADDRESS);
					}				
						
			ScreenWriteCommand(WRITE_RAM_COMMAND); 
			ScreenWriteData(SendByte_0); 
			ScreenWriteData(SendByte_1);		


					
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
		



