#include "stm32f10x.h"
#include "I2C1.h"
//#include "BME280.h"

uint8_t FlagWriteData;
uint8_t FlagReadData;
static uint8_t FlagRestartEn;
static uint8_t FlagTxEnable;
static uint8_t FlagRxEnable;
static uint8_t CounterByteI2C1Tx;
static uint8_t CounterByteI2C1Rx;

//************************************************************************************************
//Transmit and Receive init structure definition  
typedef struct
{
	uint8_t DevicesSlaveAddress;		//don't shifted address
	uint8_t DevicesSlaveAddressRW;		//address + RW bit
	uint8_t *DataTx;
	uint8_t *DataRx;
	uint8_t QuantityByteTX;
	uint8_t QuantityByteRX;		
	
}TxRxI2CData;

static TxRxI2CData TxRxI2C1Data;


//************************************************************************************************
//Функция инициализации интерфейса I2C
void I2C1_Init(void)
{	
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;															//Подать тактирование на модуль I2C1
	GPIOB->CRL	&= ~(GPIO_CRL_CNF6 | GPIO_CRL_CNF7);											//сброс CNF бит (биты функций)									
	GPIOB->CRL  |= (GPIO_CRL_CNF6 | GPIO_CRL_MODE6_1 | GPIO_CRL_CNF7 | GPIO_CRL_MODE7_1);		//режим OD-AF (альтернативная функция, открытый сток), скорость выводов максимальная 2МГц						
	I2C1->CCR = CCR_VALUE;				                                                        //Регистр контроля частоты
	I2C1->TRISE = TRISE_VALUE;			                                                        //Регистр контроля времени нарастания (максимальное время в тактах)
	
	
	I2C1->CR2 |= I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN | I2C_CR2_FREQ_2 | I2C_CR2_FREQ_5;			//  Включено событие от буфера, 				
																								//передача приём, частота перефирии 
	NVIC_SetPriority (I2C1_EV_IRQn, 1);
	NVIC_EnableIRQ (I2C1_EV_IRQn);																//
	   
	I2C1_Enable();																				//включение I2C1				
}

//************************************************************************************************
//Transmit data from ISR
void TransmitI2C1DataISR(uint8_t AddrDevice, uint8_t *AddrTxBuffer, uint8_t TxQuantityByte)
{
	FlagTxEnable = 1;
	FlagRxEnable = 0;
	FlagRestartEn = 0;
	FlagWriteData = 1;
	FlagReadData = 0;
	
	TxRxI2C1Data.DevicesSlaveAddress = AddrDevice;
	TxRxI2C1Data.DevicesSlaveAddressRW = ADDR_WRITE_MODE(AddrDevice);
	TxRxI2C1Data.DataTx = AddrTxBuffer;
	TxRxI2C1Data.DataRx = 0;
	TxRxI2C1Data.QuantityByteTX = TxQuantityByte;
	TxRxI2C1Data.QuantityByteRX = 0;
	
	I2C1_Start();
}

//************************************************************************************************
//Receive data from ISR
void ReceiveI2C1DataISR(uint8_t AddrDevice, uint8_t *AddrTxBuffer, uint8_t *AddrRxBuffer, uint8_t RxQuantityByte)
{
	FlagTxEnable = 1;
	FlagRxEnable = 1;
	FlagRestartEn = 1;
	FlagWriteData = 1;
	FlagReadData = 1;
	
	TxRxI2C1Data.DevicesSlaveAddress = AddrDevice;
	TxRxI2C1Data.DevicesSlaveAddressRW = ADDR_WRITE_MODE(AddrDevice);
	TxRxI2C1Data.DataTx = AddrTxBuffer;
	TxRxI2C1Data.DataRx = AddrRxBuffer;
	TxRxI2C1Data.QuantityByteTX = 1;
	TxRxI2C1Data.QuantityByteRX = RxQuantityByte;
	
	I2C1_Start();
}

//************************************************************************************************
//Receive data from ISR, quantity write select
void ReceiveI2C1DataQSelectISR(uint8_t AddrDevice, uint8_t *AddrTxBuffer, uint8_t *AddrRxBuffer, uint8_t RxQuantityByte, uint8_t TxQuantityByte)
{
	FlagTxEnable = 1;
	FlagRxEnable = 1;
	FlagRestartEn = 1;
	FlagWriteData = 1;
	FlagReadData = 1;
	
	TxRxI2C1Data.DevicesSlaveAddress = AddrDevice;
	TxRxI2C1Data.DevicesSlaveAddressRW = ADDR_WRITE_MODE(AddrDevice);
	TxRxI2C1Data.DataTx = AddrTxBuffer;
	TxRxI2C1Data.DataRx = AddrRxBuffer;
	TxRxI2C1Data.QuantityByteTX = TxQuantityByte;
	TxRxI2C1Data.QuantityByteRX = RxQuantityByte;
	
	I2C1_Start();
}

//************************************************************************************************
//Функция включения I2C	
static void I2C1_Enable(void)
{
	I2C1->CR1 |= I2C_CR1_PE;
}

//************************************************************************************************
//Функция выключения I2C		
static void I2C1_Disable(void)
{
  I2C1->CR1 &= ~I2C_CR1_PE;
}			
			
//************************************************************************************************
//Функция генерации ссостояния СТАРТ на линии I2C				
static void I2C1_Start(void)
{
	I2C1->CR1 |= I2C_CR1_START;
}				
			
//************************************************************************************************
//Функция генерации состояния СТОП на линии I2C	
static void I2C1_Stop(void)
{
	I2C1->CR1 |= I2C_CR1_STOP;
}		

//************************************************************************************************
//Функция включения генерации подтверждения получения байта	
static void I2C1_EnAck(void)
{
	I2C1->CR1 |= I2C_CR1_ACK;
}	

//************************************************************************************************
//Функция выключения генерации подтверждения получения байта		
static void I2C1_DisAck(void)
{
	I2C1->CR1 &= ~I2C_CR1_ACK;
}			

//************************************************************************************************
//Функция перезагрузки модуля I2C1		
static void I2C1_RESET(void)
{
	I2C1->CR1 |= I2C_CR1_SWRST;
}			
				
//**********************************************************************************************************************************
//Получить статус состояния регистра передачи I2C1
static uint16_t getI2C1TXDataRegisterEmpty(void)
{
  uint16_t TXRegisterStatus;
  
  TXRegisterStatus = I2C1StatusRegister1;            //Прочесть состояние регистра статуса
  TXRegisterStatus &= I2C_SR1_TXE;                      //Выделить бит состояния передающего регистра
  TXRegisterStatus = TXRegisterStatus>>7;             //Сдвинуть на нулевую позицию
  return TXRegisterStatus;                               //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус состояния регистра приёма I2C1
static uint16_t getI2C1RXDataRegisterNotEmpty(void)
{
  uint16_t RXRegisterStatus;
  
  RXRegisterStatus = I2C1StatusRegister1;            //Прочесть состояние регистра статуса
  RXRegisterStatus &= I2C_SR1_RXNE;                      //Выделить бит состояния передающего регистра
  RXRegisterStatus = RXRegisterStatus>>6;             //Сдвинуть на нулевую позицию
  return RXRegisterStatus;                               //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус состояния СТОП I2C1
static uint16_t getI2C1StopDetection(void)
{
  uint16_t RegisterStatus;
  
  RegisterStatus = I2C1StatusRegister1;            //Прочесть состояние регистра статуса
  RegisterStatus &= I2C_SR1_STOPF;                      //Выделить бит состояния передающего регистра
  RegisterStatus = RegisterStatus>>4;             //Сдвинуть на нулевую позицию
  return RegisterStatus;                               //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус состояния передачи байта I2C1
static uint16_t getI2C1ByteTransferFinished(void)
{
  uint16_t RegisterStatus;
  
  RegisterStatus = I2C1StatusRegister1;            //Прочесть состояние регистра статуса
  RegisterStatus &= I2C_SR1_BTF;                      //Выделить бит состояния передающего регистра
  RegisterStatus = RegisterStatus>>2;             //Сдвинуть на нулевую позицию
  return RegisterStatus;                               //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус состояния приёма или передачи адреса I2C1
static uint16_t getI2C1AddrTxRx(void)
{
  uint16_t RegisterStatus;
  
  RegisterStatus = I2C1StatusRegister1;            //Прочесть состояние регистра статуса
  RegisterStatus &= I2C_SR1_ADDR;                      //Выделить бит состояния передающего регистра
  RegisterStatus = RegisterStatus>>1;             //Сдвинуть на нулевую позицию
  return RegisterStatus;                               //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус состояния СТАРТ I2C1
static uint16_t getI2C1StartDetection(void)
{
  uint16_t RegisterStatus;
  
  RegisterStatus = I2C1StatusRegister1;            //Прочесть состояние регистра статуса
  RegisterStatus &= I2C_SR1_SB;                      //Выделить бит состояния передающего регистра
  //RegisterStatus = RegisterStatus>>0;             //Сдвинуть на нулевую позицию
  return RegisterStatus;                               //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус состояния шины I2C1
static uint16_t getI2C1BusyDetection(void)
{
  uint16_t RegisterStatus;
  
  RegisterStatus = I2C1StatusRegister2;            //Прочесть состояние регистра статуса
  RegisterStatus &= I2C_SR2_BUSY;                      //Выделить бит состояния передающего регистра
  RegisterStatus = RegisterStatus>>1;             //Сдвинуть на нулевую позицию
  return RegisterStatus;                               //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус состояния шины I2C1
static uint16_t getI2C1MSLDetection(void)
{
  uint16_t RegisterStatus;
  
  RegisterStatus = I2C1StatusRegister2;            //Прочесть состояние регистра статуса
  RegisterStatus &= I2C_SR2_MSL;                      //Выделить бит состояния передающего регистра
  RegisterStatus = RegisterStatus>>0;             //Сдвинуть на нулевую позицию
  return RegisterStatus;                               //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус ответа шины I2C1
static uint16_t getI2C1AFDetection(void)
{
  uint16_t RegisterStatus;
  
  RegisterStatus = I2C1StatusRegister1;            //Прочесть состояние регистра статуса
  RegisterStatus &= I2C_SR1_AF;                      //Выделить бит состояния передающего регистра
  RegisterStatus = RegisterStatus>>10;             //Сдвинуть на нулевую позицию
  return RegisterStatus;                               //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус ошибка шины I2C1
static uint16_t getI2C1BERRDetection(void)
{
  uint16_t RegisterStatus;
  
  RegisterStatus = I2C1StatusRegister1;            //Прочесть состояние регистра статуса
  RegisterStatus &= I2C_SR1_BERR;                      //Выделить бит состояния передающего регистра
  RegisterStatus = RegisterStatus>>8;             //Сдвинуть на нулевую позицию
  return RegisterStatus;                               //Передать значение флага занятости
}

//**********************************************************************************************************************************
//Получить статус стоп флаг шины I2C1
static uint16_t getI2C1STOPDetection(void)
{
  uint16_t RegisterStatus;
  
  RegisterStatus = I2C1StatusRegister1;            //Прочесть состояние регистра статуса
  RegisterStatus &= I2C_SR1_STOPF;                      //Выделить бит состояния передающего регистра
  RegisterStatus = RegisterStatus>>4;             //Сдвинуть на нулевую позицию
  return RegisterStatus;                               //Передать значение флага занятости
}
		
//**********************************************************************************************************************************
//Обработчик прерывания от I2C1 		
void I2C1_EV_IRQHandler(void) 
{
	volatile  uint16_t StatusReadForClear;
   
	if(getI2C1StartDetection())                                                              //Проверка генерации состояния СТАРТ
	{     
		I2C1->DR = TxRxI2C1Data.DevicesSlaveAddressRW;                                       //Запись адреса ведомого
		//I2C1->CR1 &= ~I2C_SR1_AF;
		if(FlagRestartEn == 1){I2C1_EnAck(); }                                                //При считывании включить формирование ответа ведомому   
		__NOP();
		return;
	}
    
	if (getI2C1AddrTxRx() == 1)                                                               //Проверка совпадения адреса ведомого
		{ 
			StatusReadForClear = I2C1->SR2;
			if (TxRxI2C1Data.QuantityByteRX == 1)											//if just one byte transmit
				{ I2C1_DisAck(); }
			return;
		}
    
	if ((getI2C1AddrTxRx() == 0)&&(getI2C1TXDataRegisterEmpty())&&(FlagTxEnable == 1))        //Передача давнных ведомому
		{
			StatusReadForClear = I2C1->DR;
			if (CounterByteI2C1Tx < TxRxI2C1Data.QuantityByteTX)
			{    
				I2C1->DR = *(TxRxI2C1Data.DataTx + CounterByteI2C1Tx); //TxBufferI2C1[CounterByteI2C1Tx + 1];   Write data to Data register       
				CounterByteI2C1Tx++;
				return;
			}
			else 
			{ 
				FlagTxEnable = 0;                                                                  //Передача последнего байта
				FlagWriteData  = 0;
				CounterByteI2C1Tx = 0;                              
				if (FlagRestartEn == 0){I2C1_Stop(); }                                             //Генерация состояния СТОП при отправке данных ведомому
				else {TxRxI2C1Data.DevicesSlaveAddressRW = ADDR_READ_MODE(TxRxI2C1Data.DevicesSlaveAddress); I2C1_Start(); }                               //Иначе повторный старт 
				
				return;
			} 
      
		}
  
	if (getI2C1RXDataRegisterNotEmpty()&&FlagRxEnable == 1)                                   //Чтение данных ведомого
		{ 

			
			if (CounterByteI2C1Rx < TxRxI2C1Data.QuantityByteRX)
			{
				if (CounterByteI2C1Rx == TxRxI2C1Data.QuantityByteRX - 2)                                    //Чтение с N-2 байта происходит с отключённым ответом от мастера
					{ I2C1_DisAck(); }

              
				*(TxRxI2C1Data.DataRx + CounterByteI2C1Rx) = I2C1->DR ;  //RxBufferI2C1[CounterByteI2C1Rx + TxRxI2C1Data.RxBufferAddr] = I2C1->DR; Read data from data register
				CounterByteI2C1Rx++;   
				__NOP(); 
              
				if (CounterByteI2C1Rx == TxRxI2C1Data.QuantityByteRX)                                      //Чтение последнего байта
					{  
						FlagRestartEn = 0;             
						FlagRxEnable = 0;
						FlagReadData = 0;
						CounterByteI2C1Rx = 0;
						I2C1_Stop(); 
					}           
				return;
			}        
      
		}   
 
	  StatusReadForClear = I2C1->DR;  
}