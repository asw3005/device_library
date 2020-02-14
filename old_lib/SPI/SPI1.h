/* Created: 13.07.2016
 * SPI1.h
 *
 *
 *
 */	
 
#ifndef SPI1_H_
#define SPI1_H_

#include "stm32f10x.h"

//**********************************************************************************************************************************
//

//#define     EnableBit               SPI_CR1_SPE
//#define     BusyBit                 SPI_SR_BSY
//#define     TXBufferBit             SPI_SR_TXE
//#define     TXEInterruptBit         SPI_CR2_TXEIE
#define     SPI1ControlRegister1    (SPI1->CR1)
#define     SPI1ControlRegister2    (SPI1->CR2)
#define     SPI1StatusRegister      (SPI1->SR)

//**********************************************************************************************************************************
//Прототипы функций
void SPI1Config(void);              //Настройка приёмопередатчика SPI1
void SPI1Enable(void);                //Включить приёмопередатчик SPI1
void SPI1Disable(void);             //Выключить приёмопередатчик SPI1 
void SPI1TXEInterruptEnable(void);    //Включить прерывание по событию "передающий буфер пуст", SPI1
void SPI1TXEInterruptDisable(void); //Выключить прерывание по событию "передающий буфер  пуст", SPI1
uint8_t getSPIBusy(void);             //Получить статус состояния SPI1
uint8_t getSPITXBufferEmpty(void);  //Получить статус состояния передающего буфера SPI1
uint8_t getSPITXIBufferEmpty(void);		//Получить статус состояния бита события "передающий буфер пуст" SPI1

#endif /* SPI1_H_ */
