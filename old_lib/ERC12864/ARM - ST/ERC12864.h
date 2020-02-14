/*Display - ERC12864*/
/*Controller - NT7538*/
/*Manufactura - "http://www.lcd-china.com/"*/

#include "stm32f10x.h"


//***************************************************************************************
//NT7538 константы команд контроллера
#define 	DISPLAY_ON 																		0xAF 	// 0b1010111x  x - изменяемое значение 1, дисплей включён
#define 	DISPLAY_OFF 																	0xAE 	// 0b1010111x  x - изменяемое значение 0, дисплей выключен 
#define 	DISPLAY_START_LINE_SET  											0x40 	// 0b01xxxxxx  x - изменяемое значение от 40h до 7Fh, установка стартовой строки 
#define 	PAGE_START_ADRESS_SET													0xB0	// 0b1011xxxx  x - изменяемое значение от B0h до B8h, установка страницы
#define 	COLUMN_START_ADDRES_SET_HIGH  								0x10 	// 0b0001xxxx  x - изменяемое значение от 10h до 18h, установка адреса столбца, старший полуайт
#define 	COLUMN_START_ADDRES_SET_LOW   								0x00 	// 0b0000xxxx  x - изменяемое значение от 00h до 0Fh, установка адреса столбца, младший полубайт
#define 	COLUMN_ADDRESS_NORMAL       									0xA0  // 0b1010000x  x - изменяемое значение от 0h или 1h, переключение направления отсчёта адресов столбцов
#define 	COLUMN_ADDRESS_REVERSE      									0xA1  // 0b1010000x  x - изменяемое значение от 0h или 1h, переключение направления отсчёта адресов столбцов
#define 	DISPLAY_NORMAL      													0xA6	// 0b1010011x  x - изменяемое значение от 0h или 1h, защёлка, содежимое РАМ невидимо 
#define 	DISPLAY_REVERSE     													0xA7	// 0b1010011x  x - изменяемое значение от 0h или 1h, защёлка, содержимое РАМ видимое
#define 	ENTIRE_DISPLAY_ON_NORMAL											0xA4	// 0b1010010x  x - изменяемое значение от 0h или 1h, аппаратная очистка экрана
#define 	ENTIRE_DISPLAY_ON_ALL_POINTS									0xA5	// 0b1010010x  x - изменяемое значение от 0h или 1h, аппаратная заливка экрана
#define 	LSD_BIAS_SET_19																0xA2	// 0b1010001x  x - изменяемое значение от 0h или 1h
#define 	LSD_BIAS_SET_17																0xA3	// 0b1010001x  x - изменяемое значение от 0h или 1h
#define 	READ_MODIFY_WRITE_EN													0xE0	// 0b11100000  вкючение режима чтение-модификация-запись
#define 	READ_MODIFY_WRITE_DIS													0xEE	// 0b11101110  выключение режима чтение-модификация-запись
#define		RESET_DISPLAY																	0xE2	// 0b11100010  сброс дисплея (регистры: адрес строки, адрес столбца, адрес страницы, режим выхода, V0 регулятор напряжения,
																															// регулировка контраста, режим индикатора, чтение-модификация-запись, тестовый режим
#define		OUTPUT_STATUS_SELECT_NORMAL										0xC0	// 0b1100x000
#define		OUTPUT_STATUS_SELECT_REVERSE    							0xC8	// 0b1100x000
#define		POWER_CONTROL_SET															0x2F  // регистр управления питанием, повышающий преобразователь включён, регулировка включена, питание включено (Power Control Set, 2F)
#define 	INTERNAL_RESISTOR_RATIO_SET										0x26	// регистр диапазона внутреннего резистора (установлено состояние по умолчанию)
#define 	ELECTRONIC_VOLUME_MODE_SET										0x81	// 0b10000001 включение режима регулировки контраста
#define 	ELECTRONIC_VOLUME_REGISTER_SET								0x19	// регистр установки контраста, начальное значение 50%
#define		STATIC_INDICATOR_OFF													0xAC	// 0b10101100 команда установка режима индикатора выключена
#define		STATIC_INDICATOR_ON														0xAD	// 0b10101101 команда установка режима индикатора включена
#define		STATIC_INDICATOR_REGISTER_SET_OFF							0x00  // 0b00000000 дисплей отключён
#define   STATIC_INDICATOR_REGISTER_SET_ON_BLINK_1S			0x01	// 0b00000001 дисплей включён, мигание 1с
#define		STATIC_INDICATOR_REGISTER_SET_ON_BLINK_05S		0x02	// 0b00000010 дисплей включён, мигание 0.5с
#define		STATIC_INDICATOR_REGISTER_SET_ON_CONSTANTLY		0x03	// 0b00000011 дисплей включён, мигание отключено
#define		NOP																						0xE3	// 0b11100011 нет операции
#define		TEST_IC																				0xF0	// 0b11110100 тест МК
#define		OSCILLATION_FREQUENCY_SELECT_31_4KHz					0xE4	// 0b11100100 рабочая частота 31.4 килогерца
#define		OSCILLATION_FREQUENCY_SELECT_26_3KHz					0xE5	// 0b11100101 рабочая частота 26.3 килогерца
#define		PARTIAL_DISPLAY_MODE_DISABLE 									0x82	// 0b10000010 режим настройки заполнения и смещения
#define		PARTIAL_DISPLAY_MODE_ENABLE										0x83	// 0b10000011 режим настройки заполнения и смещения



//************************************************************************************************
//Определение используемых портов ввода вывода

//#define WRPort				       GPIOB        //0 - запись, 1 - нет команды, вывод дисплея 5 
//#define RDPort				       GPIOB			    //0 - чтение, 1 - нет команды, вывод дисплея 6
//#define CEPort				       GPIOB			  //Порт ввода/вывода для вывода тактового сигнала для дисплея, вывод дисплея 7
//#define CDPort				       GPIOB					//0 - данные, 1 - команды, вывод дисплея 8
//#define ResetPort				     GPIOB				//0  - сброс дисплея, вывод дисплея 10
#define CommandScreenPort      GPIOB					//Порт управления команды/данные, чтение/запись, сброс
#define DataLineScreenPort     GPIOA				//Порт ввода/вывода для линий данных D0(MSB) - D7(LSB), выводы дисплея 11-18
//#define FontSelectScreenPort GPIOA					//Порт ввода/вывода для выбора формата шрифта, 6*8 - не подключён
																					    //или подключён к плюсу питания, 8*8 - подключён к земле (минус питания)
																					    //вывод дисплея 19
//************************************************************************************************
//Определение используемых выводов

#define WREnable      				GPIO_BSRR_BR0		//Запись выкл: 0 - запись, 1 - нет команды
#define WRDisable       			GPIO_BSRR_BS0		  //Запись вкл: 0 - запись, 1 - нет команды
#define RDEnable      				GPIO_BSRR_BR1		//Чтение выкл: 0 - чтение, 1 - нет команды
#define RDDisable       			GPIO_BSRR_BS1		  //Чтение вкл: 0 - чтение, 1 - нет команды
#define CEEnable      				GPIO_BSRR_BR2		//Тактовый сигнал вкл: 0 - активный уровень, 1 - неактивный уровень
#define CEDisable       			GPIO_BSRR_BS2		  //Тактовый сигнал выкл: 0 - активный уровень, 1 - неактивный уровень
#define A0CommandEnable 			GPIO_BSRR_BR3		//Режим работы - передача/чтение комманд
#define A0DataEnable    			GPIO_BSRR_BS3		  //Режим работы - передача/чтение данных
#define ResetDisable        	GPIO_BSRR_BS4		//Сброс дисплея не активен
#define ResetEnable           GPIO_BSRR_BR4		  //Сброс дисплея активен


#define ClockHigh      				GPIO_BSRR_BS6		//Установка в 1 линии данных DB5
#define ClockLow      				GPIO_BSRR_BR6		//Установка в 0 линии данных DB5
#define DataHigh      				GPIO_BSRR_BS7		//Установка в 1 линии данных DB4
#define DataLow      					GPIO_BSRR_BR7		//Установка в 0 линии данных DB4 MSB


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
