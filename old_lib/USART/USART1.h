/* Created: 25.07.2016
 * USART2.h
 * 
 * 
 * 
 */	
 
#ifndef USART1_H_
#define USART1_H_

#include "stm32f10x.h"

//**********************************************************************************************************************************
//
#define     USART1ControlRegister1    (USART1->CR1)
#define     USART1ControlRegister2    (USART1->CR2)
#define     USART1ControlRegister3    (USART1->CR3)
#define     USART1DataRegister        (USART1->DR)
#define     USART1StatusRegister      (USART1->SR)
#define     USART1BaudRateRegister    (USART1->BRR)         

//**********************************************************************************************************************************
//Бодрейт для модуля USART
#define     USART1BRR                 (Mantissa | Fraction)

//***********************
//Тактирование шины 36МГц
//#define     Mantissa                  0x3A90 												//2.400Kbps, 12 старших бит
//#define     Fraction                  0x0008												//4 младших бита
//#define     Mantissa                  0x0EA0 												//9.6Kbps, 12 старших бит
//#define     Fraction                  0x0006												//4 младших бита
//#define     Mantissa                  0x0750												//19.2Kbps, 12 старших бит
//#define     Fraction                  0x0003												//4 младших бита
//#define     Mantissa                  0x0270												//57.6Kbps, 12 старших бит
//#define     Fraction                  0x0001												//4 младших бита
#define     Mantissa                  0x0130												//115.2Kbps, 115.384, 12 старших бит
#define     Fraction                  0x0008												//4 младших бита
//#define     Mantissa                  0x0090												//230.4Kbps, 230.769, 12 старших бит
//#define     Fraction                  0x000C												//4 младших бита
//#define     Mantissa                  0x0040												//460.8Kbps, 461.538, 12 старших бит
//#define     Fraction                  0x000E												//4 младших бита
//#define     Mantissa                  0x0020												//921.6Kbps, 923.076, 12 старших бит
//#define     Fraction                  0x0007												//4 младших бита
//#define     Mantissa                  0x0010												//2250Kbps, 12 старших бит
//#define     Fraction                  0x0000												//4 младших бита

//***********************
//Тактирование шины 72МГц
//#define     Mantissa                  0x7530 												//2.400Kbps, 12 старших бит
//#define     Fraction                  0x0000												//4 младших бита
//#define     Mantissa                  0x1D40 												//9.6Kbps, 12 старших бит
//#define     Fraction                  0x000C												//4 младших бита
//#define     Mantissa                  0x0EA0												//19.2Kbps, 12 старших бит
//#define     Fraction                  0x0006												//4 младших бита
//#define     Mantissa                  0x04E0												//57.6Kbps, 12 старших бит
//#define     Fraction                  0x0002												//4 младших бита
//#define     Mantissa                  0x0270												//115.2Kbps, 115.384, 12 старших бит
//#define     Fraction                  0x0001												//4 младших бита
//#define     Mantissa                  0x0130												//230.4Kbps, 230.769, 12 старших бит
//#define     Fraction                  0x0008												//4 младших бита
//#define     Mantissa                  0x0090												//460.8Kbps, 461.538, 12 старших бит
//#define     Fraction                  0x000C												//4 младших бита
//#define     Mantissa                  0x0040												//921.6Kbps, 923.076, 12 старших бит
//#define     Fraction                  0x000E												//4 младших бита
//#define     Mantissa                  0x0020												//2250Kbps, 12 старших бит
//#define     Fraction                  0x0000												//4 младших бита
//#define     Mantissa                  0x0010												//4500Kbps, 12 старших бит
//#define     Fraction                  0x0000												//4 младших бита

//**********************************************************************************************************************************
//Расположение данных в буфере передачи
#define       ID            0x00
#define       Tmsb          0x01
#define       Tlsb          0x02
#define       RHmsb         0x03
#define       RHlsb         0x04
#define       Pmsb          0x05
#define       Pxmsb         0x06
#define       Plsb          0x07
#define       Pxlsb         0x08
#define       VIBmsb        0x09
#define       VIBlsb        0x0A
#define       DUSTmsb       0x0B
#define       DUSTlsb       0x0C
#define       CRC8          0x0D

//**********************************************************************************************************************************
//Прототипы функций
void USART1Config(void);                                //Настройка USART1
void USART1Enable(void);                                    //Включить USART1
void USART1Disable(void);                               //Выключить USART1
void USART1TransmitterEnable(void);                         //Включить передатчик USART1
void USART1TransmitterDisable(void);                    //Выключить передатчик USART1
void USART1ReceiverEnable(void);                            //Включить приёмник USART1
void USART1ReceiverDisable(void);                       //Выключить приёмник USART1
void USART1TXEInterruptEnable(void);                        //Включить прерывание по событию "передающий регистр данных пуст", USART1
void USART1TXEInterruptDisable(void);                   //Выключить прерывание по событию "передающий регистр данных пуст", USART1
void USART1TCInterruptEnable(void);                         //Включить прерывание по событию "передача окончена", USART1
void USART1TCInterruptDisable(void);                    //Выключить прерывание по событию "передача окончена", USART1
void USART1IDLEInterruptEnable(void);                       //Включить прерывание по событию "передатчик готов к работе", USART1
void USART1IDLEInterruptDisable(void);                  //Выключить прерывание по событию "передатчик готов к работе", USART1
uint16_t getUSART1IdleI(void);                          //Получить статус состояния бита прерывание по событию "передатчик готов к работе" USART1
uint16_t getUSART1TXDataRegisterEmpty(void);                //Получить статус состояния регистра передачи USART1
uint16_t getUSART1RXDataRegisterNotEmpty(void);         //Получить статус состояния приёмного регистра USART1
uint16_t getUSART1TransmissionComplete(void);               //Получить статус передачи USART1
uint16_t getUSART1OverrunError(void);                   //Получить статус состояния бита "перезапись принятых данных" USART1
uint16_t getUSART1TXOn(void);                               //Получить статус состояния бита "передатчик включён" USART1
uint16_t getUSART1TXIOn(void);                          //Получить статус состояния бита прерывание по событию "передающий буфер пуст" USART1
uint16_t getUSART1Idle(void);                               //Получить статус состояния бита "состояние линии передачи IDLE" USART1

#endif /* USART1_H_ */
