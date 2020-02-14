/* Created: 27.05.2018
 * I2C1.h
 * 
 * 
 * 
 */	
 
#ifndef I2C1_H_
#define I2C1_H_

#include "stm32f10x.h"

//***************************************************************************************************************************************
//Константы

#define FREQ_PERIPH 								    36000000										//Входная частота перефирии (AHB1)
#define FREQ_I2C    								    100000											//Частота передачи-приёма данных на шине I2C
#define FREQ_RISE   								    1000000  										//Максимальное время нарастания фронта на шине
#define THIGH											1                             					//Длитильность высокого уровня сигнала 
#define TLOW        								    1                           					//Длительность низкого уровня сигнала 
          
#define CCR_VALUE   								    (FREQ_PERIPH/((THIGH+TLOW)*FREQ_I2C))			//Значение для регистра контроля частоты
#define TRISE_VALUE 								    ((FREQ_PERIPH/FREQ_RISE)+1) 					//Значение для регистра времени нарастания
          
#define ADDR_STM32F103									0x00
#define WRITE_MODE									    0x00											//режим записи в ведомое устройство				
#define READ_MODE										0x01											//режим чтения из ведомого устройства

#define ADDR_STM32F103_WRITE_MODE						(ADDR_STM32F103 | WRITE_MODE)					//Отправить адрес ведомого устройства, режим запись 
#define ADDR_STM32F103_READ_MODE						(ADDR_STM32F103 | READ_MODE)					//Отправить адрес ведомого устройства, режим чтение



#define ADDR_WRITE_MODE(__DEVICES_ADDRESS__)	        ((((uint8_t) __DEVICES_ADDRESS__) << 1U) | WRITE_MODE)				//Отправить адрес ведомого устройства, режим запись 
#define ADDR_READ_MODE(__DEVICES_ADDRESS__)				((((uint8_t) __DEVICES_ADDRESS__) << 1U) | READ_MODE)				//Отправить адрес ведомого устройства, режим чтение


//***************************************************************************************************************************************
//

#define I2C1StatusRegister1              (I2C1->SR1)
#define I2C1StatusRegister2              (I2C1->SR2)

//***************************************************************************************************************************************
//Прототипы функций
static void I2C1_Enable(void); 													  //Функция включения I2C			
static void I2C1_Disable(void); 														//Функция выключения I2C			
static void I2C1_Start(void); 														//Функция генерации ссостояния СТАРТ на линии I2C			
static void I2C1_Stop(void); 															//Функция генерации состояния СТОП на линии I2C			
static void I2C1_EnAck(void); 														//Функция включения генерации подтверждения получения байта		
static void I2C1_DisAck(void); 														//Функция выключения генерации подтверждения получения байта		
static void I2C1_RESET(void);                             //Функция перезагрузки модуля I2C1
static uint16_t getI2C1TXDataRegisterEmpty(void);         //Получить статус состояния регистра передачи I2C1
static uint16_t getI2C1RXDataRegisterNotEmpty(void);        //Получить статус состояния регистра приёма I2C1
static uint16_t getI2C1StopDetection(void);               //Получить статус состояния СТОП I2C1
static uint16_t getI2C1ByteTransferFinished(void);          //Получить статус состояния передачи байта I2C1
static uint16_t getI2C1AddrTxRx(void);                    //Получить статус состояния приёма или передачи адреса I2C1
static uint16_t getI2C1StartDetection(void);                //Получить статус состояния СТАРТ I2C1
static uint16_t getI2C1BusyDetection(void);               //Получить статус состояния шины I2C1
static uint16_t getI2C1MSLDetection(void); 								//Получить статус состояния шины I2C1
static uint16_t getI2C1AFDetection(void);                   //Получить статус ответа шины I2C1
static uint16_t getI2C1BERRDetection(void);               //Получить статус ошибка шины I2C1
static uint16_t getI2C1STOPDetection(void);                 //Получить статус стоп флаг шины I2C1
			
//***************************************************************************************************************************************
//Public function		
void I2C1_Init(void); 																								//Функция инициализации I2C
void TransmitI2C1DataISR(uint8_t AddrDevice, uint8_t *AddrTxBuffer, uint8_t TxQuantityByte); 						//Transmit data from ISR
void ReceiveI2C1DataISR(uint8_t AddrDevice, uint8_t *AddrTxBuffer, uint8_t *AddrRxBuffer, uint8_t RxQuantityByte);   	//Receive data from ISR
void ReceiveI2C1DataQSelectISR(uint8_t AddrDevice, uint8_t *AddrTxBuffer, uint8_t *AddrRxBuffer, uint8_t RxQuantityByte, uint8_t TxQuantityByte);//Receive data from ISR, quantity write select

#endif /* I2C1_H_ */


