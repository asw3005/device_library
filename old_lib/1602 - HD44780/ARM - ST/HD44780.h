//************************************************************************************************
//Определения, функции и линии ввода/вывода для работы с дисплеем HD44780 (режим 4 и 8 бит)	

#include "stm32f10x.h"

//extern uint32_t Offset;
extern uint32_t FirstLine[16];
extern uint32_t SecondLine[16];
//************************************************************************************************
//Команды для работы с дисплеем на основе контроллера HD44780 и других совместимых

#define CLR_SCREEN				        0x01				//0b00000001	;очистка дисплея, счётчик адреса DDRAM на нулевую позицию
#define DDRAM_HOME_ADDR				    0x02				//0b00000010	;адресация в DDRAM, счётчик адреса 0, сдвиги сброшены
#define INC_CADDR					        0x06				//0b00000110  ;инкремент счётчика адреса
#define DEC_CADDR					        0x04				//0b00000100  ;декремент счётчика адреса
#define INC_CADDR_SHIFT				    0x07				//0b00000111	;инкремент счётчика адреса, автосдвиг экрана
#define DEC_CADDR_SHIFT				    0x05				//0b00000101	;декремент счётчика адреса, автосдвиг экрана
#define ENSCREEN_DISCUR_DISSQW    0x0C				//0b00001100  ;дисплей включён, курсор отключён
#define ENSCREEN_ENCUR_DISSQW	    0x0E				//0b00001110  ;дисплей включён, курсор включён, режим курсора мигающая чёрточка
#define ENSCREEN_ENCUR_ENSQW	    0x0F				//0b00001111	;дисплей включён, курсор включён, режим курсора мигающий квадрат
#define DISSCREEN_DISCUR_DISSQW		0x08				//0b00001000 	;дисплей отключён, курсор отключён
//#define DISSCREEN_ENCUR_DISSQW		0x0A				//0b00001010  ;дисплей отключён, курсор включён, режим курсора мигающая чёрточка  :)
//#define DISSCREEN_ENCUR_ENSQW	    0x0B				//0b00001011  ;дисплей отключён, курсор включён, режим курсора мигающий квадрат   :)
#define SCREEN_SHIFT_LEFT			    0x18				//0b00011000	;сдвиг экрана влево
#define SCREEN_SHIFT_RIGHT		    0x1C				//0b00011100	;сдвиг экрана вправо
#define CUR_SHIFT_LEFT				    0x10				//0b00010000	;сдвиг курсора влево
#define CUR_SHIFT_RIGHT				    0x14				//0b00010100	;сдвиг курсора вправо
#define DATA_8LINE				        0x38				//0b00111000	;8 линии данных, 2 строки, символ 5*8
#define DATA_4LINE				        0x28				//0b00101000  ;4 линии данных, 2 строки, символ 5*8
#define CGRAM_ADDR				        0x40				//0b01000000	;адресация на знакогенераторе собственных символов
#define DDRAM_ADDR				        0x80				//0b10000000	;адресация на знакогенераторе дисплея
#define SET_DDRAM_SCREEN_1        0x80				//0b10000000  ;запись команды, адресация на DDRAM, адресс первой строки первого символа
#define SET_DDRAM_SCREEN_2        0xC0				//0b11000000  ;запись команды, адресация на DDRAM, адресс второй строки первого символа


//************************************************************************************************
//Линии ввода/вывода для работы с дисплеем

#define CommandScreenPort      GPIOB					//Порт управления команды/данные, чтение/запись, сброс
#define DataLineScreenPort     GPIOA				//Порт ввода/вывода для линий данных D0(MSB) - D7(LSB), выводы дисплея 11-18

#define RSDataEnable	      	GPIO_BSRR_BS0		 //Установка в 1 линии выбора типа передаваемых данных на дисплей: 0 - команды, 1 - данные
#define RSCommandEnable	      GPIO_BSRR_BR0		  //Установка в 0 линии выбора типа передаваемых данных на дисплей: 0 - команды, 1 - данные
#define RWReadEnable	      	GPIO_BSRR_BS1		 //Установка в 1 линии выбора режима работы с данными: 0 - запись, 1 - чтение
#define RWWriteEnable		      GPIO_BSRR_BR1		  //Установка в 0 линии выбора режима работы с данными: 0 - запись, 1 - чтение
#define EClockEnable        	GPIO_BSRR_BS2		//Установка в 1 линии вывода тактового сигнала для дисплея
#define EClockDisable		      GPIO_BSRR_BR2			//Установка в 0 линии вывода тактового сигнала для дисплея

#define DataLineDB7Set      	GPIO_BSRR_BS7		//Установка в 1 линии данных DB7
#define DataLineDB7Reset      GPIO_BSRR_BR7		//Установка в 0 линии данных DB7
#define DataLineDB6Set      	GPIO_BSRR_BS6		//Установка в 1 линии данных DB6
#define DataLineDB6Reset      GPIO_BSRR_BR6		//Установка в 0 линии данных DB6
#define DataLineDB5Set      	GPIO_BSRR_BS5		//Установка в 1 линии данных DB5
#define DataLineDB5Reset      GPIO_BSRR_BR5		//Установка в 0 линии данных DB5
#define DataLineDB4Set      	GPIO_BSRR_BS4		//Установка в 1 линии данных DB4
#define DataLineDB4Reset      GPIO_BSRR_BR4		//Установка в 0 линии данных DB4

#define ReadDataLineDB7				GPIO_IDR_IDR7 
                      

void InitScreen(void);											//функция инициализации дисплея
void SetPinsToOut (void);										//функция установки выводов на выход (групповая, для порта дисплея)
void SendByteToScreen (uint32_t Data);			//функция передачи данных на дисплей
void Delay_nop (void);											//Функция задержки из команды  NOP (зависит от частоты ядра)


void ClearScreen(void);												//Очистка дисплея (видимой области)
void SendCommandToScreen(uint32_t Command); //функция передачи команды на дисплей, шина 4 бита
void SendDataToScreen(uint32_t Data);					//функция передачи данных на дисплей, шина 4 бита
void SetAddrLine(uint32_t Line);						//функция выбора строки дисплея (дисплей двустрочный)
void SendCharToFirstLine(void);				    		//функция вывода символов на первую строку экрана
void SendCharToSecondLine(void);         		//функция вывода символов на вторую строку экрана
void SendMessageToScreen(void);           	//Функция отправки текста из памяти МК на дисплей
void WriteCharToScreen(void);									//функция записи собственных символов (всего 8 штук) в ОЗУ дисплея
void SendCharToScreen(uint32_t Char, uint32_t Line, uint8_t Offset);//функция вывода символов в видимую часть экрана
void SendTest(void);													//функция вывода символов из таблицы кодировки дисплея (тестовый вывод всех символов подряд)

void SendCommand4BitsAF (uint32_t Command);
void SendData4BitsAF (uint32_t Command);
 
void CheckBusy (void);										//Функция проверки флага занятости дисплея
