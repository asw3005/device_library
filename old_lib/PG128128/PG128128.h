/*Display - PG128128*/
/*Controller - T6963C*/

#include "stm32f10x.h"


//**************************************************************************************************************************************
//Комманды дисплея
//
#define CURSOR_POINTER_SET							0x21 //00100001	Установить указатель курсора				
#define OFFSET_REGISTER_SET							0x22 //00100010 Установить регистр смещения
#define ADDRESS_POINTER_SET							0x24 //00100100 Установить указатель адреса 
#define TEXT_HOME_ADDRESS_SET						0x40 //01000000 Установить адрес начала текста
#define TEXT_AREA_SET										0x41 //01000001 Установить границы текста
#define GRAPHIC_HOME_ADDRESS_SET				0x42 //01000010 Установить начальный адрес графики
#define GRAPHIC_AREA_SET								0x43 //01000011 Установить границы графики
//************************************************************************************************
//Данную группу использовать с командой ИЛИ
#define OR_MODE_SET											0x80 //1000x000 Режим ИЛИ
#define EXOR_MODE_SET										0x81 //1000x001 Режим ЕКСОР
#define AND_MODE_SET										0x83 //1000x011 Режим И
#define TEXT_ATTRIBUTE_MODE_SET					0x84 //1000x100 Режим текст

#define INTERNAL_CGROM_MODE_SET					0x80 //10000xxx Режим работы с внутренней памятью
#define EXTERNAL_CGRAM_MODE_SET					0x88 //10001xxx Режим работы с внешней памятью

//************************************************************************************************
//Данную группу использовать с командой ИЛИ
#define DISPLAY_OFF_MODE_SET						0x90 //10010000 Дисплей отключён

#define CURSOR_ON_BLINK_OFF_MODE_SET		0x92 //1001xx10 Курсор включён
#define CURSOR_ON_BLINK_ON_MODE_SET			0x93 //1001xx11 Мигающий курсор включён

#define TEXT_ON_GRAPHIC_OFF_MODE_SET		0x94 //100101xx Текстовый режим включён
#define TEXT_OFF_GRAFIC_ON_MODE_SET			0x98 //100110xx Графический режим включён
#define TEXT_ON_GRAPHIC_ON_MODE_SET			0x9С //100111xx Смешанный режим включён

//************************************************************************************************
//
#define ONE_LINE_CURSOR_MODE_SET				0xA0 //10100000 Размер курсора одна линия
#define EIGTH_LINE_CURSOR_MODE_SET			0xA7 //10100111 Размер курсора восемь линий

#define DATA_AUTO_WRITE_MODE_SET				0xB0 //10110000 Установить автозапись данных 
#define DATA_AUTO_READ_MODE_SET					0xB1 //10110001 Установить авточтение данных
#define AUTO_RESET_MODE_SET							0xB2 //10110010 Установить автосброс

#define DATA_WRITE_ADP_INC_MODE_SET			0xC0 //11000000 Записать данные и инкрементировать указатель адреса
#define DATA_READ_ADP_INC_MODE_SET			0xC1 //11000001 Прочитать данные и инкрементировать указатель адреса
#define DATA_WRITE_ADP_DEC_MODE_SET   	0xC2 //11000010 Записать данные и декрементировать указатель адреса
#define DATA_READ_ADP_DEC_MODE_SET			0xC3 //11000011 Прочитать данные и декрементировать указатель адреса
#define DATA_WRITE_ADP_NONVAR_MODE_SET	0xC4 //11000100 Записать данные и не менять состояние указателя адреса
#define DATA_READ_ADP_NONVAR_MODE_SET		0xC5 //11000101 Прочитать данные и не менять состояние указателя адреса

#define SCREEN_PEEK_MODE_SET						0xE0 //11100000 Помещение данных в стек ?

#define SCREEN_COPY_MODE_SET						0xE8 //11101000 Копия экрана ?

//************************************************************************************************
//Данную группу использовать с командой ИЛИ
#define BIT_RESET_MODE_SET							0xF7 //11110xxx Режим сброса бита
#define BIT_SET_MODE_SET								0xF8 //11111xxx Режим установки бита

#define BIT_0														0xF0 //1111x000 Бит 0
#define BIT_1														0xF9 //1111x001 Бит 1
#define BIT_2														0xFA //1111x010 Бит 2
#define BIT_3														0xFB //1111x011 Бит 3
#define BIT_4														0xFC //1111x100 Бит 4
#define BIT_5														0xFD //1111x101 Бит 5
#define BIT_6														0xFE //1111x110 Бит 6
#define BIT_7														0xFF //1111x111 Бит 7

//************************************************************************************************
//Определения для инициализации дисплея

#define STATUS_COMMAND_READ_WRITE_CAPABILITY		0x03																										//Статутс готовности посли операций выполнения команд, 
																																																				//чтения, записи

#define SCREEN_MODE 										(OR_MODE_SET | INTERNAL_CGROM_MODE_SET) 											//Выбор рабочей памяти и режима отображения информации 

#define TEXT_MODE												(TEXT_ON_GRAPHIC_OFF_MODE_SET | CURSOR_ON_BLINK_OFF_MODE_SET)		//Режим текст со включённым курсором
#define GRAPHIC_MODE										(TEXT_OFF_GRAFIC_ON_MODE_SET)																	//Графический режим
#define UNIVERSAL_MODE									(TEXT_ON_GRAPHIC_ON_MODE_SET | CURSOR_ON_BLINK_OFF_MODE_SET)			//Режим текст + графика

#define TEXT_HOME_ADDRESS_LOW						0x00 			//младший байт данных, старший +команда
#define TEXT_HOME_ADDRESS_HIGH					0x18 			//младший байт данных, старший +команда
#define TEXT_AREA_LOW										0x10 		//младший байт данных, старший +команда
#define TEXT_AREA_HIGH									0x00 		//младший байт данных, старший +команда
#define GRAPHICS_HOME_ADDRESS_LOW				0x00	 	//младший байт данных, старший +команда
#define GRAPHICS_HOME_ADDRESS_HIGH			0x00	 	//младший байт данных, старший +команда
#define GRAPHICS_AREA_LOW		 						0x10 		//младший байт данных, старший +команда
#define GRAPHICS_AREA_HIGH		 					0x00 		//младший байт данных, старший +команда

//************************************************************************************************
//Определение используемых портов ввода вывода

//#define WRPort				       GPIOB        //0 - запись, 1 - нет команды, вывод дисплея 5 
//#define RDPort				       GPIOB			    //0 - чтение, 1 - нет команды, вывод дисплея 6
//#define CEPort				       GPIOB			  //Порт ввода/вывода для вывода тактового сигнала для дисплея, вывод дисплея 7
//#define CDPort				       GPIOB					//0 - данные, 1 - команды, вывод дисплея 8
//#define ResetPort				     GPIOB				//0  - сброс дисплея, вывод дисплея 10
#define CommandScreenPort      GPIOB					//Порт управления команды/данные, чтение/запись, сброс
#define DataLineScreenPort     GPIOE				//Порт ввода/вывода для линий данных D0(MSB) - D7(LSB), выводы дисплея 11-18
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
#define CDCommandEnable 			GPIO_BSRR_BS3		//Режим работы - передача/чтение комманд
#define CDDataEnable    			GPIO_BSRR_BR3		  //Режим работы - передача/чтение данных
#define ResetDisable        	GPIO_BSRR_BS4		//Сброс дисплея не активен
#define ResetEnable           GPIO_BSRR_BR4		  //Сброс дисплея активен

#define DataLineDB0Set      	GPIO_BSRR_BS0		//Установка в 1 линии данных DB7 LSB
#define DataLineDB0Reset      GPIO_BSRR_BR0		//Установка в 0 линии данных DB7
#define DataLineDB1Set      	GPIO_BSRR_BS1		//Установка в 1 линии данных DB6
#define DataLineDB1Reset      GPIO_BSRR_BR1		//Установка в 0 линии данных DB6
#define DataLineDB2Set      	GPIO_BSRR_BS2		//Установка в 1 линии данных DB5
#define DataLineDB2Reset      GPIO_BSRR_BR2		//Установка в 0 линии данных DB5
#define DataLineDB3Set      	GPIO_BSRR_BS3		//Установка в 1 линии данных DB4
#define DataLineDB3Reset      GPIO_BSRR_BR3		//Установка в 0 линии данных DB4
#define DataLineDB4Set      	GPIO_BSRR_BS4		//Установка в 1 линии данных DB7
#define DataLineDB4Reset      GPIO_BSRR_BR4		//Установка в 0 линии данных DB7
#define DataLineDB5Set      	GPIO_BSRR_BS5		//Установка в 1 линии данных DB6
#define DataLineDB5Reset      GPIO_BSRR_BR5		//Установка в 0 линии данных DB6
#define DataLineDB6Set      	GPIO_BSRR_BS6		//Установка в 1 линии данных DB5
#define DataLineDB6Reset      GPIO_BSRR_BR6		//Установка в 0 линии данных DB5
#define DataLineDB7Set      	GPIO_BSRR_BS7		//Установка в 1 линии данных DB4
#define DataLineDB7Reset      GPIO_BSRR_BR7		//Установка в 0 линии данных DB4 MSB

#define WrireDataLineDB0			GPIO_ODR_ODR0   //Линия данных 7 LSB                        
#define WrireDataLineDB1			GPIO_ODR_ODR1   //Линия данных 6                        
#define WrireDataLineDB2			GPIO_ODR_ODR2   //Линия данных 5                        
#define WrireDataLineDB3			GPIO_ODR_ODR3   //Линия данных 4                        
#define WrireDataLineDB4			GPIO_ODR_ODR4   //Линия данных 3                        
#define WrireDataLineDB5			GPIO_ODR_ODR5   //Линия данных 2                        
#define WrireDataLineDB6			GPIO_ODR_ODR6   //Линия данных 1                        
#define WrireDataLineDB7			GPIO_ODR_ODR7   //Линия данных 0 MSB 

 
#define ReadDataLineDB0 			GPIO_ODR_IDR0   //Линия данных 7 LSB                        
#define ReadDataLineDB1				GPIO_ODR_IDR1   //Линия данных 6                        
#define ReadDataLineDB2				GPIO_ODR_IDR2   //Линия данных 5                        
#define ReadDataLineDB3				GPIO_ODR_IDR3   //Линия данных 4                        
#define ReadDataLineDB4				GPIO_ODR_IDR4   //Линия данных 3                        
#define ReadDataLineDB5				GPIO_ODR_IDR5   //Линия данных 2                        
#define ReadDataLineDB6				GPIO_ODR_IDR6   //Линия данных 1                        
#define ReadDataLineDB7				GPIO_ODR_IDR7   //Линия данных 0 MSB




	
void InitScreen (void);															//функция установки выводов на выход (групповая, для порта дисплея)
void InitScreenV1 (void);															//функция установки выводов на выход (групповая, для порта дисплея)
void SetPinsToOut (void); 													//функция установки выводов на выход (групповая, для порта дисплея)
void SetPinsToIn_ (void);    													//функция установки выводов на вход (групповая, для порта дисплея)
	
void SetPinsToOut_ (void);													//функция установки выводов на выход (групповая, для порта дисплея)



 

void ScreenWriteData (uint8_t Data);																									//Функция записи данных в регистр дисплея		
void SendAddrCommand (uint8_t LowByte, uint8_t HighByte, uint8_t Command);					//Функция универсальная, отправка младших и старших байт данных/адресов + команда		
void GraphicHomeAddr (uint8_t GraphicHomeAddrLow, uint8_t GraphicHomeAddrHigh);				//Функция установки начального адреса вывода графики	
void GraphicArea (uint8_t GraphicAreaLow, uint8_t GraphicAreaHigh);									//Функция установки границ вывода графики	
void TextHomeAddr (uint8_t TextHomeAddrLow, uint8_t TextHomeAddrHigh);								//Функция установки начального адреса вывода текста	
void TextArea (uint8_t TextAreaLow, uint8_t TextAreaHigh);													//Функция установки границ вывода текста
void SetAddrPointer (uint8_t AddrPointerLow, uint8_t AddrPointerHigh);								//Функция установки указателя адреса

void Delay_nop (void);																//Функция задержки из команды  NOP (зависит от частоты ядра) 
void ScreenReset (void);														//Функция сброса дисплея после подачи питания
void ScreenStatusCheck (void);												//Функция проверки статуса дисплея 
void ScreenWriteByte (uint8_t Byte);								//Функция записи байта в порт МК 
void ScreenWriteCommand (uint8_t Command);						//Функция записи команды в регистр дисплея 


