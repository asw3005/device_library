 /* Created: 25.07.2016
 * USART2.c
 * BaudRate - 
 * Parity - none
 * Bits - 1 start, 8 data, 1 stop
 */	

#include "stm32f10x.h"
#include "USART2.h"

uint8_t CounterUSART2RX;            //Счётчик кол-ва принятых байт, USART2
uint16_t CounterUSART2TX;           //Счётчик кол-ва переданных байт, USART2
uint16_t ByteNumberUSART2TX;        //Кол-во передаваемых байт, USART2
uint8_t TxBufferUart2[14];
uint8_t FlagUsart2RXComplete;       //Флаг "данные приняты" / "приёмный буфер занят"
uint8_t FlagUsart2TXComplete;       //Флаг "данные отправлены" / "передающий буфер занят"
uint8_t *USART2TxPointer_8;


//**********************************************************************************************************************************
//Настройка USART2
void USART2Config(void)
{ 
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN;                                             //Подать тактирование на модуль USART2
  
  GPIOA->CRL	&= ~GPIO_CRL_CNF2;                                                       //Сброс CNF в 00
  GPIOA->CRL	|= GPIO_CRL_CNF2_1;                                                  //Установка режима AF-PP
  GPIOA->CRL  |= GPIO_CRL_MODE2;                                                     //Установка режима outmax 10MHz 
	
	GPIOA->CRL	&= ~GPIO_CRL_CNF3;			                                						 //сброс CNF в 00								
  //GPIOA->CRL  |= GPIO_CRL_MODE3;	                                      						 //установка режима РР-GP
  
  USART2BaudRateRegister = USART2BRR;                                               //Установить бодрейт USART 
  //USART2ControlRegister1 |= USART_CR1_RXNEIE;                                           //Прерывание "регистр приёмника не пуст"
  
  //USART2ControlRegister1 |= USART_CR1_RXNEIE | USART_CR1_TCIE | USART_CR1_TXEIE ; //Прерывание "регистр приёмника не пуст"
                                                                                        //Прерывание "передача окончена"
                                                                                    //Прерывание "регистр передатчика пуст"  
  NVIC_EnableIRQ (USART2_IRQn);				  //включить прерываение USART2
}

//**********************************************************************************************************************************
//Включить USART2
void USART2Enable(void)
{
  uint16_t USARTEnable;
  
  USARTEnable = USART2ControlRegister1;               //Прочесть состояние первого регистра управления USART 
  USARTEnable |= USART_CR1_UE;                           //Установить бит включения передатчика USART в 1
  USART2ControlRegister1 = USARTEnable;              //Записать модифицированное значение обратно в регистр управления
}     

//**********************************************************************************************************************************
//Выключить USART2
void USART2Disable(void)
{
  uint16_t USARTDisable;
  
  USARTDisable = USART2ControlRegister1;              //Прочесть состояние первого регистра управления USART 
  USARTDisable &= ~USART_CR1_UE;                         //Установить бит включения передатчика USART в 1
  USART2ControlRegister1 = USARTDisable;             //Записать модифицированное значение обратно в регистр управления
}

//**********************************************************************************************************************************
//Включить передатчик USART2
void USART2TransmitterEnable(void)
{
  uint16_t TransmitterEnable;
  
  TransmitterEnable = USART2ControlRegister1;         //Прочесть состояние первого регистра управления USART 
  TransmitterEnable |= USART_CR1_TE;                      //Установить бит включения передатчика USART в 1
  USART2ControlRegister1 = TransmitterEnable;        //Записать модифицированное значение обратно в регистр управления
}

//**********************************************************************************************************************************
//Выключить передатчик USART2
void USART2TransmitterDisable(void)
{
  uint16_t TransmitterDisable;
  
  TransmitterDisable = USART2ControlRegister1;        //Прочесть состояние первого регистра управления USART
  TransmitterDisable &= ~USART_CR1_TE;                    //Установить бит включения передатчика USART в 1
  USART2ControlRegister1 = TransmitterDisable;       //Записать модифицированное значение обратно в регистр управления
}

//**********************************************************************************************************************************
//Включить приёмник USART2
void USART2ReceiverEnable(void)
{
  uint16_t ReceiverEnable;
  
  ReceiverEnable = USART2ControlRegister1;            //Прочесть состояние первого регистра управления USART 
  ReceiverEnable |= USART_CR1_RE;                         //Установить бит включения передатчика USART в 1
  USART2ControlRegister1 = ReceiverEnable;           //Записать модифицированное значение обратно в регистр управления
}

//**********************************************************************************************************************************
//Выключить приёмник USART2
void USART2ReceiverDisable(void)
{
  uint16_t ReceiverDisable;
  
  ReceiverDisable = USART2ControlRegister1;           //Прочесть состояние первого регистра управления USART 
  ReceiverDisable &= ~USART_CR1_RE;                       //Установить бит включения передатчика USART в 1
  USART2ControlRegister1 = ReceiverDisable;          //Записать модифицированное значение обратно в регистр управления
}

//**********************************************************************************************************************************
//Включить прерывание по событию "передающий регистр данных пуст", USART2
void USART2TXEInterruptEnable(void)
{
  uint16_t USARTTXEInterruptEnable;
  
  USARTTXEInterruptEnable = USART2ControlRegister1;   //Прочесть состояние первого регистра управления USART 
  USARTTXEInterruptEnable |= USART_CR1_TXEIE;          //Установить бит включения передатчика USART в 1
  USART2ControlRegister1 = USARTTXEInterruptEnable;  //Записать модифицированное значение обратно в регистр управления
}     

//**********************************************************************************************************************************
//Выключить прерывание по событию "передающий регистр данных пуст", USART2
void USART2TXEInterruptDisable(void)
{
  uint16_t USARTTXEInterruptDisable;
  
  USARTTXEInterruptDisable = USART2ControlRegister1;  //Прочесть состояние первого регистра управления USART 
  USARTTXEInterruptDisable &= ~USART_CR1_TXEIE;        //Установить бит включения передатчика USART в 1
  USART2ControlRegister1 = USARTTXEInterruptDisable; //Записать модифицированное значение обратно в регистр управления
}

//**********************************************************************************************************************************
//Включить прерывание по событию "передача окончена", USART2
void USART2TCInterruptEnable(void)
{
  uint16_t USARTTCInterruptEnable;
  
  USARTTCInterruptEnable = USART2ControlRegister1;    //Прочесть состояние первого регистра управления USART 
  USARTTCInterruptEnable |= USART_CR1_TCIE;         //Установить бит включения передатчика USART в 1
  USART2ControlRegister1 = USARTTCInterruptEnable;   //Записать модифицированное значение обратно в регистр управления
}     

//**********************************************************************************************************************************
//Выключить прерывание по событию "передача окончена", USART2
void USART2TCInterruptDisable(void)
{
  uint16_t USARTTCInterruptDisable;
  
  USARTTCInterruptDisable = USART2ControlRegister1;   //Прочесть состояние первого регистра управления USART 
  USARTTCInterruptDisable &= ~USART_CR1_TCIE;           //Установить бит включения передатчика USART в 1
  USART2ControlRegister1 = USARTTCInterruptDisable;  //Записать модифицированное значение обратно в регистр управления
}

//**********************************************************************************************************************************
//Включить прерывание по событию "передатчик готов к работе", USART2
void USART2IDLEInterruptEnable(void)
{
  uint16_t USARTIdleInterruptEnable;
  
  USARTIdleInterruptEnable = USART2ControlRegister1;    //Прочесть состояние первого регистра управления USART 
  USARTIdleInterruptEnable |= USART_CR1_IDLEIE;           //Установить бит включения передатчика USART в 1
  USART2ControlRegister1 = USARTIdleInterruptEnable; //Записать модифицированное значение обратно в регистр управления
} 

//**********************************************************************************************************************************
//Выключить прерывание по событию "передатчик готов к работе", USART2
void USART2IDLEInterruptDisable(void)
{
  uint16_t USARTIdleInterruptDisable;
  
  USARTIdleInterruptDisable = USART2ControlRegister1;    //Прочесть состояние первого регистра управления USART 
  USARTIdleInterruptDisable &= ~USART_CR1_IDLEIE;           //Установить бит включения передатчика USART в 1
  USART2ControlRegister1 &= USARTIdleInterruptDisable; //Записать модифицированное значение обратно в регистр управления
} 

//**********************************************************************************************************************************
//Получить статус состояния регистра передачи USART2
uint16_t getUSART2TXDataRegisterEmpty(void)
{
  uint16_t TXRegisterStatus;
  
  TXRegisterStatus = USART2StatusRegister;            //Прочесть состояние регистра статуса
  TXRegisterStatus &= USART_SR_TXE;                      //Выделить бит состояния передающего регистра
  TXRegisterStatus = TXRegisterStatus>>7;             //Сдвинуть на нулевую позицию
  return TXRegisterStatus;                               //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус состояния приёмного регистра USART2
uint16_t getUSART2RXDataRegisterNotEmpty(void)
{
  uint16_t RXRegisterStatus;
  
  RXRegisterStatus = USART2StatusRegister;            //Прочесть состояние регистра статуса
  RXRegisterStatus &= USART_SR_RXNE;                    //Выделить бит состояния принимающего регистра
  RXRegisterStatus = RXRegisterStatus>>5;             //Сдвинуть на нулевую позицию
  return RXRegisterStatus;                              //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус передачи USART2
uint16_t getUSART2TransmissionComplete(void)
{
  uint16_t TransmissionComplete;
  
  TransmissionComplete = USART2StatusRegister;        //Прочесть состояние регистра статуса
  TransmissionComplete &= USART_SR_TC;                  //Выделить бит окончания передачи
  TransmissionComplete = TransmissionComplete>>6;     //Сдвинуть на нулевую позицию
  return TransmissionComplete;                          //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус состояния бита "перезапись принятых данных" USART2
uint16_t getUSART2OverrunError(void)
{
  uint16_t OverrunError;
  
  OverrunError = USART2StatusRegister;                //Прочесть состояние регистра статуса
  OverrunError &= USART_SR_ORE;                         //Выделить бит окончания передачи
  OverrunError = OverrunError>>3;                     //Сдвинуть на нулевую позицию
  return OverrunError;                                  //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус состояния бита "передатчик включён" USART2
uint16_t getUSART2TXOn(void)
{
  uint16_t TXOn;
  
  TXOn = USART2ControlRegister1;                      //Прочесть состояние регистра статуса
  TXOn &= USART_CR1_TE;                                 //Выделить бит включения передатчика
  TXOn = TXOn>>3;                                     //Сдвинуть на нулевую позицию
  return TXOn;                                          //Передать значение флага 
}

//**********************************************************************************************************************************
//Получить статус состояния бита прерывание по событию "передающий буфер пуст" USART2
uint16_t getUSART2TXIOn(void)
{
  uint16_t TXIOn;
  
  TXIOn = USART2ControlRegister1;                     //Прочесть состояние регистра статуса
  TXIOn &= USART_CR1_TXEIE;                             //Выделить бит включения прерывания от событий передачи
  TXIOn = TXIOn>>7;                                   //Сдвинуть на нулевую позицию
  return TXIOn;                                         //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус состояния бита "состояние линии передачи IDLE" USART2
uint16_t getUSART2Idle(void)
{
  uint16_t Idle;
  
  Idle = USART2StatusRegister;                      //Прочесть состояние регистра статуса
  Idle &= USART_SR_IDLE;                                 //Выделить бит готовности передатчика
  Idle = Idle>>4;                                     //Сдвинуть на нулевую позицию
  return Idle;                                          //Передать значение флага 
}

//**********************************************************************************************************************************
//Получить статус состояния бита прерывание по событию "передатчик готов к работе" USART2
uint16_t getUSART2IdleI(void)
{
  uint16_t IdleI;
  
  IdleI = USART2ControlRegister1;                      //Прочесть состояние регистра статуса
  IdleI &= USART_CR1_IDLEIE;                                 //Выделить бит события готовности передатчика
  IdleI = IdleI>>4;                                     //Сдвинуть на нулевую позицию
  return IdleI;                                          //Передать значение флага 
}
