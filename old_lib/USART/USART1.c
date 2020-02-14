 /* Created: 25.07.2016
 * USART2.c
 * BaudRate - 
 * Parity - none
 * Bits - 1 start, 8 data, 1 stop
 */	

#include "stm32f10x.h"
#include "USART1.h"

uint8_t CounterUSART1RX;            //Счётчик кол-ва принятых байт, USART2
uint16_t CounterUSART1TX;           //Счётчик кол-ва переданных байт, USART2
uint16_t ByteNumberUSART1TX;        //Кол-во передаваемых байт, USART2
uint8_t TxBufferUsart1[14];
uint8_t FlagUsart1RXComplete;       //Флаг "данные приняты" / "приёмный буфер занят"
uint8_t FlagUsart1TXComplete;       //Флаг "данные отправлены" / "передающий буфер занят"
uint8_t *USART1TxPointer_8;
                                    

//**********************************************************************************************************************************
//Настройка USART1
void USART1Config(void)
{ 
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;                                             //Подать тактирование на модуль USART2
  
  GPIOA->CRH	&= ~GPIO_CRH_CNF9;                                                       //Сброс CNF в 00
  GPIOA->CRH	|= GPIO_CRH_CNF9_1;                                                  //Установка режима AF-PP
  GPIOA->CRH  |= GPIO_CRH_MODE9;                                                     //Установка режима outmax 10MHz 
	
	GPIOA->CRH	&= ~GPIO_CRH_CNF11;			                                						 //сброс CNF в 00								
  GPIOA->CRH  |= GPIO_CRH_MODE11;	                                      						 //установка режима РР-GP
  
  USART1BaudRateRegister = USART1BRR;                                               //Установить бодрейт USART 
  //USART1ControlRegister1 |= USART_CR1_RXNEIE;                                           //Прерывание "регистр приёмника не пуст"
  
  //USART1ControlRegister1 |= USART_CR1_RXNEIE | USART_CR1_TCIE | USART_CR1_TXEIE ; //Прерывание "регистр приёмника не пуст"
                                                                                        //Прерывание "передача окончена"
                                                                                    //Прерывание "регистр передатчика пуст"  
  NVIC_SetPriority (USART1_IRQn, 7);
  NVIC_EnableIRQ (USART1_IRQn);				  //включить прерываение USART1
}

//**********************************************************************************************************************************
//Включить USART2
void USART1Enable(void)
{
  uint16_t USARTEnable;
  
  USARTEnable = USART1ControlRegister1;               //Прочесть состояние первого регистра управления USART 
  USARTEnable |= USART_CR1_UE;                           //Установить бит включения передатчика USART в 1
  USART1ControlRegister1 = USARTEnable;              //Записать модифицированное значение обратно в регистр управления
}     

//**********************************************************************************************************************************
//Выключить USART2
void USART1Disable(void)
{
  uint16_t USARTDisable;
  
  USARTDisable = USART1ControlRegister1;              //Прочесть состояние первого регистра управления USART 
  USARTDisable &= ~USART_CR1_UE;                         //Установить бит включения передатчика USART в 1
  USART1ControlRegister1 = USARTDisable;             //Записать модифицированное значение обратно в регистр управления
}

//**********************************************************************************************************************************
//Включить передатчик USART2
void USART1TransmitterEnable(void)
{
  uint16_t TransmitterEnable;
  
  TransmitterEnable = USART1ControlRegister1;         //Прочесть состояние первого регистра управления USART 
  TransmitterEnable |= USART_CR1_TE;                      //Установить бит включения передатчика USART в 1
  USART1ControlRegister1 = TransmitterEnable;        //Записать модифицированное значение обратно в регистр управления
}

//**********************************************************************************************************************************
//Выключить передатчик USART2
void USART1TransmitterDisable(void)
{
  uint16_t TransmitterDisable;
  
  TransmitterDisable = USART1ControlRegister1;        //Прочесть состояние первого регистра управления USART
  TransmitterDisable &= ~USART_CR1_TE;                    //Установить бит включения передатчика USART в 1
  USART1ControlRegister1 = TransmitterDisable;       //Записать модифицированное значение обратно в регистр управления
}

//**********************************************************************************************************************************
//Включить приёмник USART2
void USART1ReceiverEnable(void)
{
  uint16_t ReceiverEnable;
  
  ReceiverEnable = USART1ControlRegister1;            //Прочесть состояние первого регистра управления USART 
  ReceiverEnable |= USART_CR1_RE;                         //Установить бит включения передатчика USART в 1
  USART1ControlRegister1 = ReceiverEnable;           //Записать модифицированное значение обратно в регистр управления
}

//**********************************************************************************************************************************
//Выключить приёмник USART2
void USART1ReceiverDisable(void)
{
  uint16_t ReceiverDisable;
  
  ReceiverDisable = USART1ControlRegister1;           //Прочесть состояние первого регистра управления USART 
  ReceiverDisable &= ~USART_CR1_RE;                       //Установить бит включения передатчика USART в 1
  USART1ControlRegister1 = ReceiverDisable;          //Записать модифицированное значение обратно в регистр управления
}

//**********************************************************************************************************************************
//Включить прерывание по событию "передающий регистр данных пуст", USART2
void USART1TXEInterruptEnable(void)
{
  uint16_t USARTTXEInterruptEnable;
  
  USARTTXEInterruptEnable = USART1ControlRegister1;   //Прочесть состояние первого регистра управления USART 
  USARTTXEInterruptEnable |= USART_CR1_TXEIE;          //Установить бит включения передатчика USART в 1
  USART1ControlRegister1 = USARTTXEInterruptEnable;  //Записать модифицированное значение обратно в регистр управления
}     

//**********************************************************************************************************************************
//Выключить прерывание по событию "передающий регистр данных пуст", USART2
void USART1TXEInterruptDisable(void)
{
  uint16_t USARTTXEInterruptDisable;
  
  USARTTXEInterruptDisable = USART1ControlRegister1;  //Прочесть состояние первого регистра управления USART 
  USARTTXEInterruptDisable &= ~USART_CR1_TXEIE;        //Установить бит включения передатчика USART в 1
  USART1ControlRegister1 = USARTTXEInterruptDisable; //Записать модифицированное значение обратно в регистр управления
}

//**********************************************************************************************************************************
//Включить прерывание по событию "передача окончена", USART2
void USART1TCInterruptEnable(void)
{
  uint16_t USARTTCInterruptEnable;
  
  USARTTCInterruptEnable = USART1ControlRegister1;    //Прочесть состояние первого регистра управления USART 
  USARTTCInterruptEnable |= USART_CR1_TCIE;         //Установить бит включения передатчика USART в 1
  USART1ControlRegister1 = USARTTCInterruptEnable;   //Записать модифицированное значение обратно в регистр управления
}     

//**********************************************************************************************************************************
//Выключить прерывание по событию "передача окончена", USART2
void USART1TCInterruptDisable(void)
{
  uint16_t USARTTCInterruptDisable;
  
  USARTTCInterruptDisable = USART1ControlRegister1;   //Прочесть состояние первого регистра управления USART 
  USARTTCInterruptDisable &= ~USART_CR1_TCIE;           //Установить бит включения передатчика USART в 1
  USART1ControlRegister1 = USARTTCInterruptDisable;  //Записать модифицированное значение обратно в регистр управления
}

//**********************************************************************************************************************************
//Включить прерывание по событию "передатчик готов к работе", USART2
void USART1IDLEInterruptEnable(void)
{
  uint16_t USARTIdleInterruptEnable;
  
  USARTIdleInterruptEnable = USART1ControlRegister1;    //Прочесть состояние первого регистра управления USART 
  USARTIdleInterruptEnable |= USART_CR1_IDLEIE;           //Установить бит включения передатчика USART в 1
  USART1ControlRegister1 = USARTIdleInterruptEnable; //Записать модифицированное значение обратно в регистр управления
} 

//**********************************************************************************************************************************
//Выключить прерывание по событию "передатчик готов к работе", USART2
void USART1IDLEInterruptDisable(void)
{
  uint16_t USARTIdleInterruptDisable;
  
  USARTIdleInterruptDisable = USART1ControlRegister1;    //Прочесть состояние первого регистра управления USART 
  USARTIdleInterruptDisable &= ~USART_CR1_IDLEIE;           //Установить бит включения передатчика USART в 1
  USART1ControlRegister1 &= USARTIdleInterruptDisable; //Записать модифицированное значение обратно в регистр управления
} 

//**********************************************************************************************************************************
//Получить статус состояния регистра передачи USART2
uint16_t getUSART1TXDataRegisterEmpty(void)
{
  uint16_t TXRegisterStatus;
  
  TXRegisterStatus = USART1StatusRegister;            //Прочесть состояние регистра статуса
  TXRegisterStatus &= USART_SR_TXE;                      //Выделить бит состояния передающего регистра
  TXRegisterStatus = TXRegisterStatus>>7;             //Сдвинуть на нулевую позицию
  return TXRegisterStatus;                               //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус состояния приёмного регистра USART2
uint16_t getUSART1RXDataRegisterNotEmpty(void)
{
  uint16_t RXRegisterStatus;
  
  RXRegisterStatus = USART1StatusRegister;            //Прочесть состояние регистра статуса
  RXRegisterStatus &= USART_SR_RXNE;                    //Выделить бит состояния принимающего регистра
  RXRegisterStatus = RXRegisterStatus>>5;             //Сдвинуть на нулевую позицию
  return RXRegisterStatus;                              //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус передачи USART2
uint16_t getUSART1TransmissionComplete(void)
{
  uint16_t TransmissionComplete;
  
  TransmissionComplete = USART1StatusRegister;        //Прочесть состояние регистра статуса
  TransmissionComplete &= USART_SR_TC;                  //Выделить бит окончания передачи
  TransmissionComplete = TransmissionComplete>>6;     //Сдвинуть на нулевую позицию
  return TransmissionComplete;                          //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус состояния бита "перезапись принятых данных" USART2
uint16_t getUSART1OverrunError(void)
{
  uint16_t OverrunError;
  
  OverrunError = USART1StatusRegister;                //Прочесть состояние регистра статуса
  OverrunError &= USART_SR_ORE;                         //Выделить бит окончания передачи
  OverrunError = OverrunError>>3;                     //Сдвинуть на нулевую позицию
  return OverrunError;                                  //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус состояния бита "передатчик включён" USART2
uint16_t getUSART1TXOn(void)
{
  uint16_t TXOn;
  
  TXOn = USART1ControlRegister1;                      //Прочесть состояние регистра статуса
  TXOn &= USART_CR1_TE;                                 //Выделить бит включения передатчика
  TXOn = TXOn>>3;                                     //Сдвинуть на нулевую позицию
  return TXOn;                                          //Передать значение флага 
}

//**********************************************************************************************************************************
//Получить статус состояния бита прерывание по событию "передающий буфер пуст" USART2
uint16_t getUSART1TXIOn(void)
{
  uint16_t TXIOn;
  
  TXIOn = USART1ControlRegister1;                     //Прочесть состояние регистра статуса
  TXIOn &= USART_CR1_TXEIE;                             //Выделить бит включения прерывания от событий передачи
  TXIOn = TXIOn>>7;                                   //Сдвинуть на нулевую позицию
  return TXIOn;                                         //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус состояния бита "состояние линии передачи IDLE" USART2
uint16_t getUSART1Idle(void)
{
  uint16_t Idle;
  
  Idle = USART1StatusRegister;                      //Прочесть состояние регистра статуса
  Idle &= USART_SR_IDLE;                                 //Выделить бит готовности передатчика
  Idle = Idle>>4;                                     //Сдвинуть на нулевую позицию
  return Idle;                                          //Передать значение флага 
}

//**********************************************************************************************************************************
//Получить статус состояния бита прерывание по событию "передатчик готов к работе" USART2
uint16_t getUSART1IdleI(void)
{
  uint16_t IdleI;
  
  IdleI = USART1ControlRegister1;                      //Прочесть состояние регистра статуса
  IdleI &= USART_CR1_IDLEIE;                                 //Выделить бит события готовности передатчика
  IdleI = IdleI>>4;                                     //Сдвинуть на нулевую позицию
  return IdleI;                                          //Передать значение флага 
}
