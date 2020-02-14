/*Display - ER-OLEDM028-1Y*/
/*Controller - SSD1322 - max. 480*128*/
/*Manufactura - "http://www.lcd-china.com/"*/

#include "stm32f10x.h"

//***************************************************************************************
//SSD1322 константы команд контроллера
#define		ENABLE_GRAY_SCALE_TABLE													0x00

#define 	SET_COLUMN_ADDRESS															0x15  // Установка адресов столбцов	(на один счёт адреса приходится 4 столбца, т.е. адрес 0 - столбец 0, адрес 1 - столбец 4
#define		COLUMN_START_ADDRESS														0x1C  // Установить начальный адрес столбца	
#define 	COLUMN_END_ADDRESS															0x5B  // Установить конечный адрес столбца

#define		WRITE_RAM_COMMAND																0x5C	// Команда активации записи данных в память дисплея
#define		READ_RAM_COMMAND																0x5D	// Команда активации чтения данных из памяти дисплея

#define		SET_ROW_ADDRESS																	0x75	// Установка адресов строк
#define		ROW_START_ADDRESS																0x00	// Установить адрес начальной строки
#define		ROW_END_ADDRESS																	0x3F	// Установить адрес конечной строки

#define		SET_REMAP_AND_DUAL_COM_LINE											0xA0	//
#define		ADDR_INC_COL_REMAP_COM_SCAN_COM_SPLIT_DUAL_COM	0x14	// Инкремент адреса горизонтальный, направление столбцов 0-256, ниббл ремап включён, сканирование  COM64-COM0, раделение на чётные нечётные номера строк отключено
#define		DUAL_COM_LINE_DISABLE														0x00	//
#define		DUAL_COM_LINE_ENABLE														0x11	//

#define		SET_DISPLAY_START_LINE													0xA1	// Команда установки начальной строки в памяти дисплея
#define		DEFINE_START_LINE																0x00	// Адрес начальной строки по умолчанию

#define		SET_DISPLAY_OFFSET															0xA2	// Команда установки смещения адреса начальной строки
#define		DEFINE_DISPLAY_OFFSET														0x00	// Смещение адреса начальной строки по умолчанию

#define		ENTIRE_DISPLAY_OFF															0xA4	// Содержимое памяти дисплея не отображается
#define		ENTIRE_DISPLAY_ON																0xA5	// Все пиксели закрашены
#define		NORMAL_DISPLAY_PIXEL_SET												0xA6	// Пиксели дисплея устанавливаются записью 1
#define		INVERSE_DISPLAY_PIXEL_SET												0xA7	// Пиксели дисплея устанавливаются записью 0

#define		ENABLE_PARTIAL_DISPLAY													0xA8	// Команда установки границ отображения информации на дисплее
#define		DISPLAY_AREA_START_ADDRESS											0x00	// Адрес начальной границы отображения
#define		DISPLAY_AREA_END_ADDRESS												0x3F	// Адрес конечной границы отображения

#define		DISABLE_PARTIAL_DISPLAY													0xA9	// Команда отключения режима ограничения отображения информации

#define 	FUNCTION_SELECTION			    										0xAB 	// Команда установки источника питания  
#define		ENABLE_INTERNAL_DCDC														0x01  // Выбрать внутренний источник питания
#define		ENABLE_EXTERNAL_POWER_SUPPLY										0x00  // Выбрать внешний источник питания

#define		SLEEP_ON																				0xAE	// Команда включения режима сна		
#define		SLEEP_OFF																				0xAF	// Команда выключения режима сна	

#define		SET_PHASE_LENGTH																0xB1	// Команда установки параметров фаз управления 
#define		DEFAULT_PHASE_TWO_ONE														0x74	// По умолчанию фаза 2 - 7, фаза 1 - 9
#define		PHASE_TWO_ONE																		0xE2	// По умолчанию фаза 2 - 7, фаза 1 - 9

#define		SET_CLOCK_DIVIDER_OSCILLATOR_FRAQUENCY					0xB3	// Команда установки делителя и частоты генератора
#define		DIVSET_OSCFREQ																	0x11	// Значение делителя и частоты

#define		SET_GPIO																				0xB5	//
#define		GPIO1_GPIO0																			0x0A	//

#define		SET_SECOND_PRECHARGE_PERIOD											0xB6	// Команда установки времени второго периода предзаряда ёмкости диода матрицы
#define		SECOND_PRECHARGE_PERIOD													0x08	// Время второго периода предзаряда

#define		SET_GRAY_SCALE_TABLE														0xB8	// Команда записи произвольных значений для таблицы яркости
#define		GAMMA_FOR_GS1																		0x01	// Значение 1 в таблице яркости
#define		GAMMA_FOR_GS2																		0x03	//
#define		GAMMA_FOR_GS3																		0x06	//
#define		GAMMA_FOR_GS4																		0x0A	//
#define		GAMMA_FOR_GS5																		0x10	//
#define		GAMMA_FOR_GS6																		0x1A	//
#define		GAMMA_FOR_GS7																		0x28	//
#define		GAMMA_FOR_GS8																		0x37	//
#define		GAMMA_FOR_GS9																		0x47	//
#define		GAMMA_FOR_GS10																	0x58	//
#define		GAMMA_FOR_GS11																	0x6A	//
#define		GAMMA_FOR_GS12																	0x7F	//
#define		GAMMA_FOR_GS13																	0x96	//
#define		GAMMA_FOR_GS14																	0xB4	//
#define		GAMMA_FOR_GS15																	0xFF	// Значение 15 в таблице яркости

#define		SELECT_LINEAR_GRAY_SCALE_TABLE									0xB9	// Команда включения предустановленной таблици яркости с линейным изменением

#define		SET_PRECHARGE_VOLTAGE														0xBB	// Команда установки напряжения предзаряда
#define		DEFAULT_PRECHARGE_VOLTAGE												0x17	// Значение напряжения предзаряда
#define		PRECHARGE_VOLTAGE																0x1F	// Значение напряжения предзаряда

#define		SET_VOLTAGE_COM_DESELECT												0xBE	//
#define		DEFAULT_VOLTAGE_COM_DESELECT										0x04	//
#define		VOLTAGE_COM_DESELECT														0x07	//

#define		SET_CONTRAST_CURRENT														0xC1	// Значение тока для установки контраста
#define 	DEFAULT_CONTRAST_CURRENT												0x4F	//

#define		MASTER_CONTRAST_CURRENT_CONTROL									0xC7	// Команда установки контраста

#define		SET_MUX_RATIO																		0xCA	// Команда установки диапазона сканируемых строк
#define		DEFAULT_MUX_RATIO																0x3F	// Кол-во сканируемых строк

#define		SET_COMMAND_LOCK																0xFD	// Команда установки блокировки на приём любых команд, кроме FD
#define		UNLOCK_OLED_DRIVER															0x12	// Блокировка отключена
#define		LOCK_OLED_DRIVER																0x16	// Блокировка включена


//************************************************************************************************
//Определение используемых портов ввода вывода

#define 	CommandScreenPort      													GPIOA									//Порт управления команды/данные, чтение/запись, сброс
#define 	DataLineScreenPort     													GPIOA										//Порт ввода/вывода для линий данных D0(MSB) - D7(LSB), выводы дисплея 11-18
//#define FontSelectScreenPort 														GPIOA									//Порт ввода/вывода для выбора формата шрифта, 6*8 - не подключён
																																								//или подключён к плюсу питания, 8*8 - подключён к земле (минус питания)
																																								//вывод дисплея 19
//************************************************************************************************
//Определение используемых выводов

#define 	WREnable      				(CommandScreenPort->BSRR = GPIO_BSRR_BR10)				//Запись выкл: 0 - запись, 1 - нет команды
#define 	WRDisable       			(CommandScreenPort->BSRR = GPIO_BSRR_BS10)		  		//Запись вкл: 0 - запись, 1 - нет команды
#define 	RDEnable      				(CommandScreenPort->BSRR = GPIO_BSRR_BR15)				//Чтение выкл: 0 - чтение, 1 - нет команды
#define 	RDDisable       			(CommandScreenPort->BSRR = GPIO_BSRR_BS15)		  		//Чтение вкл: 0 - чтение, 1 - нет команды
#define 	CSEnable      				(CommandScreenPort->BSRR = GPIO_BSRR_BR11)				//Тактовый сигнал вкл: 0 - активный уровень, 1 - неактивный уровень
#define 	CSDisable       			(CommandScreenPort->BSRR = GPIO_BSRR_BS11)		  		//Тактовый сигнал выкл: 0 - активный уровень, 1 - неактивный уровень
#define 	DCCommandEnable 			(CommandScreenPort->BSRR = GPIO_BSRR_BR9)					//Режим работы - передача/чтение комманд
#define 	DCDataEnable    			(CommandScreenPort->BSRR = GPIO_BSRR_BS9)		  			//Режим работы - передача/чтение данных
#define 	ResetDisable        	(CommandScreenPort->BSRR = GPIO_BSRR_BS12)				//Сброс дисплея не активен
#define 	ResetEnable           (CommandScreenPort->BSRR = GPIO_BSRR_BR12)		  		//Сброс дисплея активен


#define 	ClockHigh      				(DataLineScreenPort->BSRR = GPIO_BSRR_BS0)					//Установка в 1 линии данных DB5
#define 	ClockLow      				(DataLineScreenPort->BSRR = GPIO_BSRR_BR0)						//Установка в 0 линии данных DB5
#define 	DataHigh      				(DataLineScreenPort->BSRR = GPIO_BSRR_BS1)					//Установка в 1 линии данных DB4
#define 	DataLow      					(DataLineScreenPort->BSRR = GPIO_BSRR_BR1)						//Установка в 0 линии данных DB4 


#define 	STATUS_COMMAND_READ_WRITE_CAPABILITY						0x80									//Флаг занятости
#define 	PageWidth																				0x08										//толщина строки в точках (определяется моделью дисплея, аппаратный параметр)
#define 	CharPoints																			0x05									//ширина символа в точках
#define		PageHigh																				0x09																		


	
//************************************************************************************************
//Определение используемых выводов

void InitScreen (void);																													//Инициализация дисплея	
void SetScreen(void);																															//Заливка дисплея
void ClearScreen(void);																													//Очистка дисплея
void SendCharToScreen (uint8_t Page, uint8_t Position, uint8_t Quantity);					//Вывод группы символов на дисплей
void SendOneChar (uint8_t Page, uint8_t Position, uint8_t ASCII_Code);					//Вывод одного символа на дисплей
void PixelSetReset (uint16_t Horizontal, uint16_t Vertical, uint8_t SetClrBit);			// Закрасить выбранный пиксель
void Line (uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);											//Рисование прямой линии по координатам начала и конца				
void SendText(uint8_t *ASCII_Code, uint8_t Page, uint8_t Position);								//Отправка текстового сообщения на дисплей
void ClearPageScreenProg(uint8_t Page);																					//Очистка дисплея прогамная
void ClearPageScreenHard(void);																						//Очистка дисплея аппаратная  - работает только с аппаратной заливкой
void GroundPageScreenProg(uint8_t Page);																					//Заливка дисплея програмная	
void GroundPageScreenHard(void);																				//Заливка дисплея	аппаратная  - работает только с аппаратной очисткой

//void LcdBiasSet(uint8_t Bias);																								//Смещение для дисплея (?)
void ScreenWriteByte(uint8_t Byte);																								//Функция записи байта в порт МК 
void ScreenWriteByteSPI (uint8_t Byte);																					//Функция записи байта в порт МК 
void ScreenWriteCommand(uint8_t Command);																					//Функция отпраки команды дисплею 
void ScreenWriteData(uint8_t Data);																							//Функция отправки данных на дисплей 
void Delay_nop(void);																															//Функция задержки из команды  NOP (зависит от частоты ядра)
//void ScreenStatusCheck(void);																									//Функция проверки статуса дисплея
void SetPinsToOut(void);																													//функция установки выводов на выход (групповая, для порта дисплея)
void SetPinsToOut_(void);																												//функция установки выводов на выход (групповая, для порта дисплея)	
void SetPinsToIn_(void);																													//функция установки выводов на вход (групповая, для порта дисплея)	
void ScreenReset(void);																												//Функция сброса дисплея после подачи питания 	
void SetPage (uint8_t Page);																											//Установка текущей страницы дисплея	
void SetColumn (uint8_t Column);																								//Установка текущего столбца(вертикаль) дисплея
void SendByte (uint8_t Byte);																											//Отправка байта данных
uint8_t ScreenDataRead (void);																									//Функция записи байта в порт МК 


