/*Display - ER-OLEDM024-1*/
/*Controller - SSD1305*/
/*Manufactura - "http://www.lcd-china.com/"*/

#include "stm32f10x.h"

//***************************************************************************************
//SSD1305 константы команд контроллера
#define 	MASTER_CONFIGURATION_SET    									0xAD 	// 0b10101101  
#define		ENABLE_INTERNAL_DCDC													0x8F  // 
#define		ENABLE_EXTERNAL_POWER_SUPPLY									0x8E  //

#define 	DISPLAY_ON_NORMAL_BRIGHTNESS									0xAF 	// 0b101011xx  
#define 	DISPLAY_ON_DIM_MODE         									0xAC 	// 0b101011xx  
#define 	DISPLAY_OFF 																	0xAE 	// 0b101011xx  

#define 	MEMORY_ADDRESSING_MODE_SET										0x20  // 0b00100000 режим адресации памяти
#define 	HORIZONTAL_ADDRESSING_MODE										0x00  // после заполнения первой страницы по горизонтали адрес столбцов сбрасывается в 0, адрес страницы инкрементируется
#define		VERTICAL_ADDRESSING_MODE											0x01  // после заполнения первой страницы по вертикали адрес страницы сбрасывается в 0, адрес столбца инкрементируется  
#define		PAGE_ADDRESSING_MODE													0x02  // после заполнения первой страницы её адрес остаётся нейзменным, адрес столбцов сбрасывается в 0 *установлен после сброса

#define 	DISPLAY_START_LINE_SET  											0x40 	// 0b01xxxxxx  x - изменяемое значение от 40h до 7Fh, установка стартовой строки 
#define 	PAGE_START_END_ADDRESS_SET										0x22	// установка начальной и конечной страницы
#define 	PAGE1_ADDRESS_SET  														0xB0	// установка указателя адреса на страницу 1 (для режима страничной адресации)
#define 	PAGE2_ADDRESS_SET  														0xB1	// установка указателя адреса на страницу 2 (для режима страничной адресации)
#define 	PAGE3_ADDRESS_SET  														0xB2	// установка указателя адреса на страницу 3 (для режима страничной адресации)
#define 	PAGE4_ADDRESS_SET  														0xB3	// установка указателя адреса на страницу 4 (для режима страничной адресации)
#define 	PAGE5_ADDRESS_SET  														0xB4	// установка указателя адреса на страницу 5 (для режима страничной адресации)
#define 	PAGE6_ADDRESS_SET  														0xB5	// установка указателя адреса на страницу 6 (для режима страничной адресации)
#define 	PAGE7_ADDRESS_SET  														0xB6	// установка указателя адреса на страницу 7 (для режима страничной адресации)
#define 	PAGE8_ADDRESS_SET  														0xB7	// установка указателя адреса на страницу 8 (для режима страничной адресации)

#define 	COLUMN_ADDRESS_SET														0x21  // установка начального и конечного адресов столбцов	
#define		COLUMN_START_ADDRESS													0x00  //  	
#define 	COLUMN_END_ADDRESS														0x1F  //
#define 	COLUMN_START_ADDRES_SET_HIGH  								0x10 	// установка указателя адреса столбцов, старший байт (для режима страничной адресации)
#define 	COLUMN_START_ADDRES_SET_LOW   								0x00 	// установка указателя адреса столбцов, младший байт (для режима страничной адресации)

#define 	CONTRAST_CONTROL_BANK0_SET										0x81  //
#define   CONTRAST_LEVEL_BANK0													0x80	//
#define 	BRIGHTNESS_AREA_COLOR_BANKS_SET								0x82  //
#define   CONTRAST_LEVEL_BANKCOLOR											0x80	// (в постраничном режиме может совпать с командой изменения стартовой строки)
#define 	LOOK_UP_TABLE_SET															0x91  // контраст-яркость
#define 	CURRENT_DRIVE_PULSE														0x1F  //
#define 	BANK_COLOR_BANK1_16_SET												0x92  //
#define 	BANK_COLOR_BANK17_32_SET											0x93  //

#define 	MULTIPLEX_RATIO_SET														0xA8  // 16 - 64 строкидля перебора, после сброса установлено 64
#define 	MULTIPLEX_64MUX																0x3F  //
#define 	SEGMENT_REMAP_COLUMN0_SEG0										0xA0  //
#define 	SEGMENT_REMAP_COLUMN131_SEG0									0xA1  // *
#define 	SCAN_DIRECTION_COM0_COMn_SET									0xC0  // установлено при сбросе, где n - MULTIPLEX_RATIO, кол-во строк сканирования
#define 	SCAN_DIRECTION_COMn_COM0_SET									0xC8  // *
#define 	COM_PINS_SET																	0x12  // после сброса установлено значение 0x12 (0b00xx0010)
#define 	DISPLAY_OFFSET_SET														0xD3  // сдвиг строк по горизонтали
#define 	OFFSET																				0x00  //

#define 	ENTIRE_DISPLAY_OFF														0xA4  //
#define 	ENTIRE_DISPLAY_ON															0xA5  //
#define 	NORMAL_DISPLAY_PIXEL_SET											0xA6  // пиксели дисплея устанавливаются записью 1
#define 	INVERSE_DISPLAY_PIXEL_SET											0xA7  // пиксели дисплея устанавливаются записью 0
#define 	DIM_MODE_SETTING															0xAB  // регулировка контраста и яркости в режиме DIM
#define		DIVIDE_RATIO_OSC_FREQUENCY										0xD5  //
#define		DIVIDE1_FREQMAX_SET														0xF0  //
#define		COLOR_AND_LOWPOWER_MODE_SET										0xD8  // после сброса установлен монохромный режим, режим потребления стандартный
#define		COLOR_AND_LOWPOWER_MODE												0x05  // режим малого потребления
#define		PRECHARGE_PERIOD_SET													0x91  // после сброса установлено значение 2 DCLKs  d9, 22 (в постраничном режиме может совпать с командой изменения стартовой строки)
#define		VCOMH_SET																			0x34  // после сброса установлено значение 0х34

#define		READ_MODIFY_WRITE_ON													0xE0  //
#define		READ_MODIFY_WRITE_OFF													0xEE  //
#define		NOP																						0xE3  //	

#define		SCROLL_ON																			0x2F  //
#define		SCROLL_OFF																		0x2E  //
#define		HORIZONTAL_SCROLL_SETUP_RIGHT									0x26  //		
#define		HORIZONTAL_SCROLL_SETUP_LEFT									0x27  //	
#define		CONTINUOUS_VERTICAL_RIGHTHORIZONTAL_SCROLL		0x29  //	
#define		CONTINUOUS_VERTICAL_LEFTHORIZONTAL_SCROLL			0x2A  //	
#define		VERTICAL_SCROLL_AREA_SET											0xA3  //

//************************************************************************************************
//Определение используемых портов ввода вывода

//#define WRPort				       GPIOB        //0 - запись, 1 - нет команды, вывод дисплея 5 
//#define RDPort				       GPIOB			    //0 - чтение, 1 - нет команды, вывод дисплея 6
//#define CEPort				       GPIOB			  //Порт ввода/вывода для вывода тактового сигнала для дисплея, вывод дисплея 7
//#define CDPort				       GPIOB					//0 - данные, 1 - команды, вывод дисплея 8
//#define ResetPort				     GPIOB				//0  - сброс дисплея, вывод дисплея 10
#define CommandScreenPort      GPIOA					//Порт управления команды/данные, чтение/запись, сброс
#define DataLineScreenPort     GPIOA				//Порт ввода/вывода для линий данных D0(MSB) - D7(LSB), выводы дисплея 11-18
//#define FontSelectScreenPort GPIOA					//Порт ввода/вывода для выбора формата шрифта, 6*8 - не подключён
																					    //или подключён к плюсу питания, 8*8 - подключён к земле (минус питания)
																					    //вывод дисплея 19
//************************************************************************************************
//Определение используемых выводов

#define WREnable      				GPIO_BSRR_BR10		//Запись выкл: 0 - запись, 1 - нет команды
#define WRDisable       			GPIO_BSRR_BS10		  //Запись вкл: 0 - запись, 1 - нет команды
#define RDEnable      				GPIO_BSRR_BR15		//Чтение выкл: 0 - чтение, 1 - нет команды
#define RDDisable       			GPIO_BSRR_BS15		  //Чтение вкл: 0 - чтение, 1 - нет команды
#define CSEnable      				GPIO_BSRR_BR11		//Тактовый сигнал вкл: 0 - активный уровень, 1 - неактивный уровень
#define CSDisable       			GPIO_BSRR_BS11		  //Тактовый сигнал выкл: 0 - активный уровень, 1 - неактивный уровень
#define DCCommandEnable 			GPIO_BSRR_BR9		//Режим работы - передача/чтение комманд
#define DCDataEnable    			GPIO_BSRR_BS9		  //Режим работы - передача/чтение данных
#define ResetDisable        	GPIO_BSRR_BS12		//Сброс дисплея не активен
#define ResetEnable           GPIO_BSRR_BR12		  //Сброс дисплея активен


#define ClockHigh      				GPIO_BSRR_BS0		//Установка в 1 линии данных DB5
#define ClockLow      				GPIO_BSRR_BR0		//Установка в 0 линии данных DB5
#define DataHigh      				GPIO_BSRR_BS1		//Установка в 1 линии данных DB4
#define DataLow      					GPIO_BSRR_BR1		//Установка в 0 линии данных DB4 


#define STATUS_COMMAND_READ_WRITE_CAPABILITY	0x80	//Флаг занятости
#define PageWidth															0x08		//толщина строки в точках (определяется моделью дисплея, аппаратный параметр)
#define CharPoints														0x05	//ширина символа в точках


	
void SetPinsToOut(void);							//функция установки выводов на выход (групповая, для порта дисплея)
void InitScreen (void);														//Инициализация дисплея

		
//void LcdBiasSet(uint8_t Bias);									//Смещение для дисплея (?)
void ScreenWriteByte(uint8_t Byte);							//Функция записи байта в порт МК
void ScreenWriteByteSPI (uint8_t Byte);					//Функция записи байта в порт МК 
void ScreenWriteCommand(uint8_t Command);					//Функция отпраки команды дисплею 
void ScreenWriteData(uint8_t Data);							//Функция отправки данных на дисплей 
void Delay_nop(void);															//Функция задержки из команды  NOP (зависит от частоты ядра)
//void ScreenStatusCheck(void);									//Функция проверки статуса дисплея 
void SetPinsToOut_(void);													//функция установки выводов на выход (групповая, для порта дисплея)	
void SetPinsToIn_(void);												//функция установки выводов на вход (групповая, для порта дисплея)	
void ScreenReset (void);													//Функция сброса дисплея после подачи питания 	
void ClearPageScreenProg (uint8_t Page);				//Очистка дисплея прогамная
void ClearPageScreenHard (uint8_t Page);					//Очистка дисплея аппаратная  - работает только с аппаратной заливкой
void GroundPageScreenProg (uint8_t Page);				//Заливка дисплея програмная	
void GroundPageScreenHard (uint8_t Page);					//Заливка дисплея	аппаратная  - работает только с аппаратной очисткой
		
void SendCharToScreen (uint8_t Page, uint8_t Position, uint8_t Quantity);		//Вывод символов на дисплей
void SendOneChar (uint8_t Page, uint8_t Position, uint8_t ASCII_Code);
void SetPage (uint8_t Page);											//Установка текущей страницы дисплея	
void SetColumn (uint8_t Column);										//Установка текущего столбца(вертикаль) дисплея
void SendByte (uint8_t Byte);											//Отправка байта данных

uint8_t ScreenDataRead (void);											//Функция записи байта в порт МК 

void PixelSetReset (uint8_t Horizontal, uint8_t Vertical, uint8_t SetClrBit);// Закрасить выбранный пиксель
void Line (uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);										//Рисование прямой линии по координатам начала и конца
							
void SendText(uint8_t *ASCII_Code, uint8_t Page, uint8_t Position);								//Отправка текстового сообщения на дисплей
