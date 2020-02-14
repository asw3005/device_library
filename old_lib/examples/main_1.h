/* Created: 16.06.2016
 * main.h
 *
 *
 *
 */	
 
 
#ifndef main_H_
#define main_H_

//**********************************************************************************************************************************
//Константы
#define   ReadGeneralInfo   0x11                  //Информация о модуле
#define   ReadStateModule   0x21                  //Состояние модуля
#define   SetCmpThreshold   0x41                  //Порог компараторов
#define   SetTimeWindow     0x42                  //Временной сдвиг и временное окно
#define   StartCounter      0x43                  //Начать счёт
#define   StopCounter       0x44                  //Окончить счёт
#define   ClearCounter      0x45                  //Очистить счётчики
#define   ReadXYCounter     0x46                  //Считать значения общих счётчиков
#define   ReadXY            0x47                  //Считать значения попиксельных счётчиков


//**********************************************************************************************************************************
//Переменные	
extern uint8_t AddressPCB;                        //Адрес модуля (устанавливается на плате переключателями)
extern uint8_t FlagAddressCompare;                  //Флаг совпадения адреса устройства с принятым адресом от мастера
extern uint8_t CounterUSART1RX;                   //Счётчик кол-ва принятых байт, USART1
extern uint16_t CounterUSART1TX;                    //Счётчик кол-ва переданных байт, USART1
extern uint16_t ByteNumberUSART1TX;               //Кол-во передаваемых байт, USART1
extern uint8_t CounterTXSPI1;                       //Счётчик кол-ва переданных байт, SPI1
extern uint8_t FlagRXComplete;                    //Флаг окончания приёма данных
extern uint8_t FlagTXComplete;                      //Флаг "данные отправлены" / "передающий буфер занят"
extern uint8_t FlagRXOff;                         //Флаг "передатчик отключён"
extern uint8_t FlagRXTimeOut;                       //Флаг "таймаут приёма активен"
extern uint8_t FlagIdleState;                     //флаг готовности к передаче данных
extern uint8_t FlagTXFinishedSPI1;                //Передача окончена SPI1
extern uint8_t FlagDacDisable;					//Отключение ЦАП
extern uint8_t State;                               //Состояние модуля
extern uint8_t DataIn[30+1];                      //Буфер для приёма + CRC
extern uint8_t DataOut[30+1];                       //Буфер для передачи данных + CRC
extern uint8_t CountCRC8;                         //Посчитанная контрольная сумма
extern uint8_t *Pointer_8;
extern uint16_t *Pointer_16;
extern uint32_t CounterTimeOut, CounterRXOff;

//**********************************************************************************************************************************
//Используемые порты
#define     AddressPCBPort      GPIOA

//**********************************************************************************************************************************
//Структура данных заголовка приёма и передачи по USART
//typedef struct                        
//{
//  uint8_t Address;                            //Адрес устройства                    
//  uint8_t Function;                             //Выполняемое действие
//  uint8_t ReadOrWrite;                        //Команда запись или передача данных
//  uint16_t DataSize __attribute__((packed));    //Размер принимаемого или передаваемого блока данных
//  //uint16_t Data[];
//  uint8_t CRC8;                               //Контрольная сумма
//  
//} HeadTransferUSART_TypeDef; 

//**********************************************************************************************************************************
//Структура данных заголовка приёма и передачи по USART
typedef struct                        
{
  uint8_t Address;                            //Адрес устройства                    
  uint8_t Function;                             //Выполняемое действие
  uint8_t ReadOrWrite;                        //Команда запись или передача данных
  uint8_t DataSizeLow;                          //Размер принимаемого или передаваемого блока данных
  uint8_t DataSizeHigh;
  //uint8_t Data[30];
  uint8_t CRC8;                               //Контрольная сумма
  
} HeadTransferUSART_TypeDef; 

//**********************************************************************************************************************************
//Информация о модуле
const uint8_t GeneralInfo[8]  = 
{
  0x64, 0x00,                                     //идентификатор модуля 
  0x1A, 0x00,                                       //число
  0x07, 0x00,                                     //месяц 
  0xE0, 0x07                                        //год
};

//**********************************************************************************************************************************
//Прототипы функций
__INLINE void EnableClockPeriph(void);        //Включение тактирования перифирии	
__INLINE void EnDisJTAGSW(void);                //Конфигурирование JTAG и SWD
__INLINE void EnableClockOut(void);           //Вывод рабочей частоты на вывод 8 порта А	
__INLINE void InitPeriph(void); 					      //Инициализация перифирии
void InitPin(void);                           //Конфигурация внешних выводов
void InitVariable(void);                        //Инициализация используемых переменных, структур, указателей и массивов
void SendGeneralInfo(void);                   //Отправка информации о модуле
void SendStateModule(void);                     //Отправка состояния модуля
void SetThreshold(void);                      //Установка порога компараторов
void SendXYCounterInfo(void);                   //Отправка общего счёта каналов
void SendEcho(void);                          //Отправка эхо
  
#endif /* main_H_ */
