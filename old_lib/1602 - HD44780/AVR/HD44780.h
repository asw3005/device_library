//************************************************************************************************
//Определения, функции и линии ввода/вывода для работы с дисплеем HD44780 (режим 4 и 8 бит)	

#include <avr/io.h>

//extern uint32_t Offset;
extern uint8_t FirstLine[16];
extern uint8_t SecondLine[16];
//************************************************************************************************
//Команды для работы с дисплеем на основе контроллера HD44780 и других совместимых

#define CLR_SCREEN				    0x01				//0b00000001	;очистка дисплея, счётчик адреса DDRAM на нулевую позицию
#define DDRAM_HOME_ADDR			    0x02				//0b00000010	;адресация в DDRAM, счётчик адреса 0, сдвиги сброшены
#define INC_CADDR					0x06				//0b00000110  ;инкремент счётчика адреса
#define DEC_CADDR					0x04				//0b00000100  ;декремент счётчика адреса
#define INC_CADDR_SHIFT				0x07				//0b00000111	;инкремент счётчика адреса, автосдвиг экрана
#define DEC_CADDR_SHIFT				0x05				//0b00000101	;декремент счётчика адреса, автосдвиг экрана
#define ENSCREEN_DISCUR_DISSQW      0x0C				//0b00001100  ;дисплей включён, курсор отключён
#define ENSCREEN_ENCUR_DISSQW	    0x0E				//0b00001110  ;дисплей включён, курсор включён, режим курсора мигающая чёрточка
#define ENSCREEN_ENCUR_ENSQW	    0x0F				//0b00001111	;дисплей включён, курсор включён, режим курсора мигающий квадрат
#define DISSCREEN_DISCUR_DISSQW		0x08				//0b00001000 	;дисплей отключён, курсор отключён
//#define DISSCREEN_ENCUR_DISSQW		0x0A				//0b00001010  ;дисплей отключён, курсор включён, режим курсора мигающая чёрточка  :)
//#define DISSCREEN_ENCUR_ENSQW	        0x0B				//0b00001011  ;дисплей отключён, курсор включён, режим курсора мигающий квадрат   :)
#define SCREEN_SHIFT_LEFT			0x18				//0b00011000	;сдвиг экрана влево
#define SCREEN_SHIFT_RIGHT		    0x1C				//0b00011100	;сдвиг экрана вправо
#define CUR_SHIFT_LEFT				0x10				//0b00010000	;сдвиг курсора влево
#define CUR_SHIFT_RIGHT			    0x14				//0b00010100	;сдвиг курсора вправо
#define DATA_8LINE				    0x38				//0b00111000	;8 линии данных, 2 строки, символ 5*8
#define DATA_4LINE				    0x28				//0b00101000  ;4 линии данных, 2 строки, символ 5*8
#define CGRAM_ADDR				    0x40				//0b01000000	;адресация на знакогенераторе собственных символов
#define DDRAM_ADDR				    0x80				//0b10000000	;адресация на знакогенераторе дисплея
#define SET_DDRAM_SCREEN_1          0x80				//0b10000000  ;запись команды, адресация на DDRAM, адресс первой строки первого символа
#define SET_DDRAM_SCREEN_2          0xC0				//0b11000000  ;запись команды, адресация на DDRAM, адресс второй строки первого символа


//************************************************************************************************
//Линии ввода/вывода для работы с дисплеем

#define DirectPortB             DDRB
#define DirectPortC             DDRC

#define PC0Out                  1<<DDC0
#define PC1Out                  1<<DDC1
#define PC2Out                  1<<DDC2
#define PC3Out                  1<<DDC3
#define PB3Out                  1<<DDB3
#define PB4Out                  1<<DDB4
#define PB5Out                  1<<DDB5

#define PC0In                   ~(1<<DDC0)
#define PC1In                   ~(1<<DDC1)
#define PC2In                   ~(1<<DDC2)
#define PC3In                   ~(1<<DDC3)
#define PB3In                   ~(1<<DDB3)
#define PB4In                   ~(1<<DDB4)
#define PB5In                   ~(1<<DDB5)

#define CommandScreenPort       PORTB					                //Порт управления команды/данные, чтение/запись, сброс
#define DataLineScreenPort      PORTC				                        //Порт ввода/вывода для линий данных D0(MSB) - D7(LSB), выводы дисплея 11-18

#define RSDataEnable	        CommandScreenPort |= (1<<PORTB3)		//Установка в 1 линии выбора типа передаваемых данных на дисплей: 0 - команды, 1 - данные
#define RSCommandEnable	        CommandScreenPort &= (~(1<<PORTB3))		//Установка в 0 линии выбора типа передаваемых данных на дисплей: 0 - команды, 1 - данные
#define RWReadEnable	      	CommandScreenPort |= (1<<PORTB4)		//Установка в 1 линии выбора режима работы с данными: 0 - запись, 1 - чтение
#define RWWriteEnable		    CommandScreenPort &= (~(1<<PORTB4))		//Установка в 0 линии выбора режима работы с данными: 0 - запись, 1 - чтение
#define EClockEnable        	CommandScreenPort |= (1<<PORTB5)		//Установка в 1 линии вывода тактового сигнала для дисплея
#define EClockDisable		    CommandScreenPort &= (~(1<<PORTB5))	    //Установка в 0 линии вывода тактового сигнала для дисплея

#define DataLineDB7Set      	DataLineScreenPort |= (1<<PORTC3)		//Установка в 1 линии данных DB7
#define DataLineDB7Reset        DataLineScreenPort &= (~(1<<PORTC3))		//Установка в 0 линии данных DB7
#define DataLineDB6Set      	DataLineScreenPort |= (1<<PORTC2)		//Установка в 1 линии данных DB6
#define DataLineDB6Reset        DataLineScreenPort &= (~(1<<PORTC2))		//Установка в 0 линии данных DB6
#define DataLineDB5Set      	DataLineScreenPort |= (1<<PORTC1)		//Установка в 1 линии данных DB5
#define DataLineDB5Reset        DataLineScreenPort &= (~(1<<PORTC1))		//Установка в 0 линии данных DB5
#define DataLineDB4Set      	DataLineScreenPort |= (1<<PORTC0)		//Установка в 1 линии данных DB4
#define DataLineDB4Reset        DataLineScreenPort &= (~(1<<PORTC0))		//Установка в 0 линии данных DB4

#define ClearMaskPort           0x0F    //




//#define ReadDataLineDB7			PORTC 
                      

void InitScreen(void);											    //функция инициализации дисплея
void SetPinsToOut (void);										        //функция установки выводов на выход (групповая, для порта дисплея)
void SendByteToScreen (uint8_t Data);			                    //функция передачи данных на дисплей
//void Delay_nop (void);											        //Функция задержки из команды  NOP (зависит от частоты ядра)


void ClearScreen(void);												//Очистка дисплея (видимой области)
void SendCommandToScreen(uint8_t Command);                             //функция передачи команды на дисплей, шина 4 бита
void SendDataToScreen(uint8_t Data);					            //функция передачи данных на дисплей, шина 4 бита
void SetAddrLine(uint8_t Line);						                //функция выбора строки дисплея (дисплей двустрочный)
void SendCharToFirstLine(void);				    		            //функция вывода символов на первую строку экрана
void SendCharToSecondLine(void);         		                        //функция вывода символов на вторую строку экрана
void SendMessageToScreen(void);           	                        //Функция отправки текста из памяти МК на дисплей
void WriteCharToScreen(void);									        //функция записи собственных символов (всего 8 штук) в ОЗУ дисплея
void SendCharToScreen(uint8_t Char, uint8_t Line, uint8_t Offset);//функция вывода символов в видимую часть экрана
void SendTest(void);

void PinScreenFlagIn(void);//Функция проверки флага занятости дисплея													//функция вывода символов из таблицы кодировки дисплея (тестовый вывод всех символов подряд)
void PinScreenFlagOut(void);//Функция проверки флага занятости дисплея
void CheckBusy(void);//Функция проверки флага занятости дисплея


void SendCommand4BitsAF (uint8_t Command);
void SendData4BitsAF (uint8_t Command);
 
void CheckBusy (void);										        //Функция проверки флага занятости дисплея
