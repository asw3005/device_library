/* Created: 16.06.2016
 * main.c
 *
 *
 *
 */

#include "stm32f10x.h"
#include "main.h"
#include "Delay.h"
#include "DAC_AD5324.h"
#include "USART1.h"
#include "SPI1.h"
#include "MultiplexerControl.h"
	
uint8_t AddressPCB;           //Адрес модуля (устанавливается на плате переключателями)
uint8_t FlagAddressCompare;     //Флаг совпадения адреса устройства с принятым адресом от мастера/флаг выбора
uint8_t CounterUSART1RX;      //Счётчик кол-ва принятых байт, USART1
uint16_t CounterUSART1TX;       //Счётчик кол-ва переданных байт, USART1
uint16_t ByteNumberUSART1TX;  //Кол-во передаваемых байт, USART1
uint8_t CounterTXSPI1;        //Счётчик кол-ва переданных байт, SPI1
uint8_t FlagRXComplete;       //Флаг "данные приняты" / "приёмный буфер занят"
uint8_t FlagTXComplete;       //Флаг "данные отправлены" / "передающий буфер занят"
uint8_t FlagRXOff;            //Флаг "приёмник отключён"
uint8_t FlagRXTimeOut;          //Флаг "таймаут приёма активен"
uint8_t FlagIdleState;        //флаг готовности к передаче данных
uint8_t FlagTXFinishedSPI1;   //Передача окончена, SPI1
uint8_t FlagDacDisable;					//Отключение ЦАП
uint8_t State;                  //Состояние модуля
uint8_t DataIn[30+1];         //Буфер для приёма + CRC
uint8_t DataOut[30+1];          //Буфер для передачи данных + CRC
uint8_t CountCRC8;            //Посчитанная контрольная сумма
uint8_t *Pointer_8;
uint16_t *Pointer_16;
uint32_t CounterTimeOut, CounterRXOff;

HeadTransferUSART_TypeDef HeadTransferUSART1;

//**********************************************************************************************************************************
//Основная программа							
int main(void)
{
 volatile uint8_t ReadWriteBuffer; 
 uint16_t i;
 
  
	InitPeriph();															//настройка портов ввода/вывода, перефирии МК		
	InitPin();	
	DisableDAC_XY;		                       	//Отключить все ЦАП
	InitVariable();
	USART1Config();
	SPI1Config();
	USART1Enable();
	USART1ReceiverEnable();
	USART1TransmitterEnable();  
	SPI1Enable();  
  __NOP();
	
	
  DACChannelControl(1);                   //Конфигурация режима энегросбережения ЦАП
  DACOutControl(0);                       //Конфигурация способа обновления выходов ЦАП
  DACData(0x0148);                         //Запись данных для отправки
  CounterTXSPI1 = 0;
	
	for (i=1;i<5;i++)
		{
			EnableDAC_XY;
			CaseDACChannel(i);                      //Установка необходимого номера канала ЦАП
			SPI1TXEInterruptEnable();
			while(!FlagDacDisable)
				{}
			FlagDacDisable = 0;
//			for (i=1;i<10000;i++)
//				{}	
			DisableDAC_XY;		                       	//Отключить все ЦАП
		}
		
	DisableDAC_XY;		                       	//Отключить все ЦАП

while(1)
	{		
    if (FlagRXComplete)// Если Флаг "данные приняты" / "приёмный буфер занят" установлен
      {
        //Проверка CRC, если ошибка, игнорировать полученные данные
        switch(HeadTransferUSART1.Function)
            {
              case ReadGeneralInfo: SendGeneralInfo();  //Отправить информацию о модуле
                                    break;
              case ReadStateModule: SendStateModule();   //Отправить информацию о состоянии модуля   
                                    break;
              case SetCmpThreshold: SetThreshold();  //Установить порог компараторов
                                    SendEcho();     //Отправить эхо, USART1
                                    break;
              case SetTimeWindow:   CaseTimeControl(DataIn[0]);//Установить параметры временного сдвига и окна 
                                    SendEcho();//Отправить эхо, USART1
                                    break;
              case StartCounter:    SendXYCounterInfo();
                                    SendEcho();//Отправить эхо, USART1
                                    break;
              case StopCounter:     
                                    SendEcho();//Отправить эхо, USART1
                                    break;
              case ClearCounter:   
                                    SendEcho();//Отправить эхо, USART1
                                    break;
              case ReadXYCounter:     
                                    break;
              case ReadXY:          //CounterUSART1TX = 1536; 
                                    break;
              default: //InitVariable();
                       FlagRXComplete = 0;//Установить флаг "данные приняты" / "приёмный буфер занят"
                       __NOP();           //Выход во всех остальных случаях, ошибка выбора действия
            }

      }
			
			if (FlagDacDisable)
				{
					DisableDAC_XY;
					FlagDacDisable = 0;
				}
      
//    if (FlagRXTimeOut) //Тайм аут приёма данных, контроль количества принятых байт
//      {
//        if (CounterTimeOut == 5000000) //Пауза 70ms при частоте 72МГц
//          {
//            //ReadWriteBuffer = USART1->DR; //Сброс флага по событию "приёмный буфер не пуст"
//            FlagRXTimeOut = 0;                                    //Сброс флага тайм аута
//            FlagAddressCompare = 0;                               //Сброс флага выбора
//            FlagRXComplete = 0;                                   //Установить флаг "данные приняты" / "приёмный буфер занят"
//            CounterTimeOut = 0; 
//          }
//         else
//          {           
//            CounterTimeOut++;  
//          }
//      }

	}
  
}

//**********************************************************************************************************************************
//Инициализация перифирии	
void InitPeriph (void)
{                
  EnableClockPeriph();			                                //Включение тактирования перифирии					
  EnDisJTAGSW(); 							                              //Конфигурирование JTAG и SWD					
  EnableClockOut();   					                            //Вывод рабочей частоты на вывод 8 порта А	
                            
  //__ASM volatile ("cpsid i");                         
  __enable_irq();									                          //глобальное разрешение прерываний
}
			
//**********************************************************************************************************************************
//Включение тактирования перифирии	
void EnableClockPeriph(void)			
{
  RCC->APB2ENR	|= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;   //включить тактирование порта А, альтернативных функций
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;		                                          	                    //включить тактирование таймера 2	 
  //RCC->APB2ENR |= RCC_APB2ENR_SPI1EN | RCC_APB2ENR_USART1EN;
}
			
//**********************************************************************************************************************************
//Конфигурирование JTAG и SWD
void EnDisJTAGSW(void)			
{
  AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1;  											//отключить JTAG, оставить включённым SW-DP 
}		
	
//**********************************************************************************************************************************
//Вывод рабочей частоты на вывод 8 порта А	
void EnableClockOut(void)			
{
  GPIOA->CRH	&= ~GPIO_CRH_CNF8;											      //сброс CNF									
  GPIOA->CRH  |= GPIO_CRH_CNF8_1 | GPIO_CRH_MODE8;		        //режим PP-AF  
  RCC->CFGR  |= RCC_CFGR_MCO_SYSCLK;                        // вывод MCO включён, источник SysClk
}
           
//**********************************************************************************************************************************
//Конфигурация внешних выводов
void InitPin(void)			
{ 
	DacConfigPin();
	MultiplexerConfig();
  //GPIOA->CRL	&= ~(GPIO_CRL_CNF2 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4);			                                                                      //сброс CNF в 00								
  //GPIOA->CRL  |= (GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4);	                                                                          //установка режима РР-GP
  //GPIOC->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);			      //сброс CNF в 00								
 //GPIOC->CRL  |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7);	      //установка режима РР-GP
  //GPIOC->CRH	&= ~(GPIO_CRH_CNF8 | GPIO_CRH_CNF9);			                                                                                      //сброс CNF в 00								
  //GPIOC->CRH  |= (GPIO_CRH_MODE8 | GPIO_CRH_MODE9);	                                                                                            //установка режима РР-GP                                                                                                          
}     

//**********************************************************************************************************************************
//Инициализация используемых переменных, структур, указателей и массивов
void InitVariable(void)			
{ 
  //AddressPCB = AddressPCBPort->IDR;
  AddressPCB = 0x09;
  //HeadTransferUSART1.Address = AddressPCB;
  HeadTransferUSART1.Address = 0;
  CounterUSART1RX = 0;      
  CounterUSART1TX = 0; 
  CounterTXSPI1 = 0;
  FlagAddressCompare = 0;  
  FlagRXComplete = 0;       
  FlagTXComplete = 0;    
  FlagRXOff = 0;
  FlagRXTimeOut = 0;
	FlagDacDisable = 0;
  State = 0;   
  CounterTimeOut = 0;
  CounterRXOff = 0;
  CountCRC8 = 0;
  //Pointer_8 = 0;
  //Pointer_16 = 0;
  FlagIdleState = 0;
}

//**********************************************************************************************************************************
//Отправка информации о модуле
void SendGeneralInfo(void)
{
  uint8_t *HeadPointer, *DataOutPointer, *GeneralInfoPointer;
  uint8_t Counter;
  uint8_t HeadValue;                                                  //Кол-во байт в заголовке
  
  HeadValue = 5;
  
  if (!FlagTXComplete)                                                //Флаг "данные отправлены" / "передающий буфер занят"
    {
      ByteNumberUSART1TX = 13+1;                                      //Кол-во передаваемых байт + CRC
      HeadPointer = (uint8_t *)& HeadTransferUSART1;                  //Определение адреса буфера заголовка
      DataOutPointer = (uint8_t *)& DataOut;                          //Определение адреса буфера отправки данных
      for (Counter=0;Counter<3;Counter++)
        {      
          (*DataOutPointer) = (*HeadPointer);
          DataOutPointer++;
          HeadPointer++;
        }
      DataOut[3] = 8;                                                 //Кол-во передаваемых байт данных, LSB
      DataOut[4] = 0;                                                 //MSB
      //FlagTXComplete = 1;
      GeneralInfoPointer = (uint8_t *)& GeneralInfo;                  //Определение адреса буфера заголовка
      DataOutPointer = (uint8_t *)& DataOut;                          //Определение адреса буфера отправки данных
      DataOutPointer = DataOutPointer + HeadValue;                    //Прибавить смещение к адресу на размер байт заголовка
      for (Counter=0;Counter<8;Counter++)
        {      
          (*DataOutPointer) = (*GeneralInfoPointer);
          DataOutPointer++;
          GeneralInfoPointer++;
        }
      //Вызов функции подсчёта контрольной суммы для передаваемых данных
      (*DataOutPointer) = CountCRC8;
      (*DataOutPointer) = 0xFF;
      CounterUSART1TX = 0;                                            //Обнулить счётчик переданных байт, USART1
      FlagRXComplete = 0;                                             //Сбросить флаг "данные приняты" / "приёмный буфер занят"  
      FlagTXComplete = 1;                                             //Установить флаг "данные отправлены" / "передающий буфер занят"
      //USART1TransmitterEnable();                                      //Включить передатчик
      RS422TXEnable;
      USART1TXEInterruptEnable();                                     //Включить прерывание по событию "передающий регистр данных пуст", USART1
    } 
}

//**********************************************************************************************************************************
//Отправка состояния модуля
void SendStateModule(void)
{
  uint8_t *HeadPointer, *DataOutPointer;
  uint8_t Counter;
  
  if (!FlagTXComplete)                                                 //Флаг "данные отправлены" / "передающий буфер занят"
    {
      ByteNumberUSART1TX = 7+1;                                       //Кол-во передаваемых байт + CRC
      HeadPointer = (uint8_t *)& HeadTransferUSART1;                  //Определение адреса буфера заголовка
      DataOutPointer = (uint8_t *)& DataOut;                          //Определение адреса буфера отправки данных
      for (Counter=0;Counter<3;Counter++)
        {      
          (*DataOutPointer) = (*HeadPointer);
          DataOutPointer++;
          HeadPointer++;
        }
      DataOut[3] = 1;                                                 //Кол-во передаваемых байт данных, LSB
      DataOut[4] = 0;                                                 //MSB
      DataOut[5] = State;                                             //Текущее состояние модуля
      //Вызов функции подсчёта контрольной суммы для передаваемых данных
      DataOutPointer = DataOutPointer + 4;
      (*DataOutPointer) = CountCRC8;
      (*DataOutPointer) = 0xFF;
      CounterUSART1TX = 0;                                            //Обнулить счётчик переданных байт, USART1
      FlagRXComplete = 0;                                             //Сбросить флаг "данные приняты" / "приёмный буфер занят"  
      FlagTXComplete = 1;                                             //Установить флаг "данные отправлены" / "передающий буфер занят"  
      //USART1TransmitterEnable();                                      //Включить передатчик
      RS422TXEnable;
      USART1TXEInterruptEnable();                                     //Включить прерывание по событию "передающий регистр данных пуст", USART1  
    }
}

//**********************************************************************************************************************************
//Установка порога компараторов
void SetThreshold(void)
{
  if (!FlagTXComplete)                                 //Флаг "данные отправлены" / "передающий буфер занят"
    {
      
      CaseDACChannel(DataIn[0]);                      //Установка необходимого номера канала ЦАП
      DACChannelControl(DataIn[3]);                   //Конфигурация режима энегросбережения ЦАП
      DACOutControl(DataIn[2]);                       //Конфигурация способа обновления выходов ЦАП
      Pointer_16 = (uint16_t *)& DataIn[4];           //Определение адреса буфера для приёма данных   
      DACData((*Pointer_16));                         //Запись данных для отправки
      CounterTXSPI1 = 0;
      CaseDACNumber(DataIn[1]);                       //Установка необходимого номера ЦАП
      SPI1TXEInterruptEnable();
      //SPI1Enable();
			//ReadWriteBuffer = DACConfig();
      //SPI1->DR = DACConfig();//Запись подготовленных данных ЦАП в регистр передачи SPI1 и сброс флага события "передающий буфер пуст"
    }
}

//**********************************************************************************************************************************
//Отправка общего счёта каналов
void SendXYCounterInfo(void)
{
  CounterUSART1TX = 18;
}

//**********************************************************************************************************************************
//Отправка эхо
void SendEcho(void)
{
  uint8_t *HeadPointer, *DataOutPointer;
  uint8_t Counter;
  
  if (!FlagTXComplete)                                                 //Флаг "данные отправлены" / "передающий буфер занят"
    {
      ByteNumberUSART1TX = 5+1;                                       //Кол-во передаваемых байт + CRC
      HeadPointer = (uint8_t *)& HeadTransferUSART1;                  //Определение адреса буфера заголовка
      DataOutPointer = (uint8_t *)& DataOut;                          //Определение адреса буфера отправки данных
      for (Counter=0;Counter<3;Counter++)
        {      
          (*DataOutPointer) = (*HeadPointer);
          DataOutPointer++;
          HeadPointer++;
        }
      DataOut[3] = 0;                                                 //Кол-во передаваемых байт данных, LSB
      DataOut[4] = 0;                                                 //MSB
      DataOutPointer = DataOutPointer + 2;
      //Вызов функции подсчёта контрольной суммы для передаваемых данных
      (*DataOutPointer) = CountCRC8;
      (*DataOutPointer) = 0xFF;
      CounterUSART1TX = 0;                                            //Обнулить счётчик переданных байт, USART1
      FlagRXComplete = 0;                                             //Сбросить флаг "данные приняты" / "приёмный буфер занят"  
      FlagTXComplete = 1;                                             //Установить флаг "данные отправлены" / "передающий буфер занят"  
      //USART1TransmitterEnable();                                      //Включить передатчик
      RS422TXEnable;
      USART1TXEInterruptEnable();                                     //Включить прерывание по событию "передающий регистр данных пуст", USART1 
    }
}
  
//**********************************************************************************************************************************
//Обработчик прерывания от TIM2 (события)			
void TIM2_IRQHandler(void) 
{
  TIM2->SR &= ~TIM_SR_UIF;									                //сброс флага прерывания | TIM_SR_CC1IF
  //NVIC_ClearPendingIRQ(TIM2_IRQn);
  //Flag=1;
}	
 
//**********************************************************************************************************************************
//Обработчик прерывания от USART1 (события)		
void USART1_IRQHandler(void) 
{
  uint8_t ReadWriteBuffer;
 
  if (getUSART1RXDataRegisterNotEmpty())                                       //Приёмный регистр не пуст
    {  
      ReadWriteBuffer = USART1DataRegister;                                            //Прочесть значение регистра приёмника, сброс флага события "приёмный регистр не пуст"
      if (!FlagRXComplete)                                                     //Проверка флага "данные приняты" / "приёмный буфер занят" 
      {
      //Добавить проверку флага "данные успешно приняты" / "приёмный буфер занят" 
      //предохраняет от затирания буфера приёма, если действия с его данными длятся большой промежуток времени
      switch(FlagAddressCompare)
          {
						case 0: Pointer_8 = (uint8_t *)& HeadTransferUSART1;               //Определение адреса буфера для приёма заголовка
										Pointer_8 = Pointer_8 + CounterUSART1RX;                   //Сложить адрес буфера с необходимым смещением
										(*Pointer_8) = ReadWriteBuffer;                            //Записать данные по получившемуся адресу
										CounterUSART1RX++;    
										if (CounterUSART1RX == 5)                                  //Если весь заголовок принят
											{
												FlagAddressCompare = 1;
												CounterUSART1RX = 0;
											}	 
										break;   

					  case 1:	 
										if (HeadTransferUSART1.Address == AddressPCB)
											{
												if (HeadTransferUSART1.DataSizeLow == 0)
													{
														HeadTransferUSART1.CRC8 = ReadWriteBuffer;									//Записать данные по получившемуся адресу 
														CounterTimeOut = 0;                                        	//Сброс счётчика тайм аута 
														FlagAddressCompare = 0;                                    	//Сбросить флаг совпадения адреса/выбора
														FlagRXTimeOut = 0;                                         	//Отключение таймера таймаута приёма                                 
														FlagRXComplete = 1;                                        	//Установить флаг "данные приняты" / "приёмный буфер занят"
													}
												else
													{
														DataIn[0] = ReadWriteBuffer;
														FlagAddressCompare = 3; 
														CounterUSART1RX = 1;
													}
											}
										else
											{
												CounterUSART1RX = 0;																		//Сбросить счётчик принятых байт данных
												CounterTimeOut = 0;                                   	//Сброс счётчика тайм аута 
												FlagAddressCompare = 0;                               	//Сбросить флаг совпадения адреса/выбора
												FlagRXTimeOut = 0;                                    	//Отключение таймера таймаута приёма                                 
												FlagRXComplete = 0;                                   	//Сброс флага "данные приняты" / "приёмный буфер занят"
											} 
										break; 
         
            case 3: Pointer_8 = (uint8_t *)& DataIn;                             //Определение адреса буфера для приёма данных
                    Pointer_8 = Pointer_8 + CounterUSART1RX;                     //Сложить адрес буфера с необходимым смещением
                    (*Pointer_8) = ReadWriteBuffer;                              //Записать данные по получившемуся адресу
                    CounterUSART1RX++; 
                    if (CounterUSART1RX == HeadTransferUSART1.DataSizeLow+1)     //Данные + CRC
                      {
                        HeadTransferUSART1.CRC8 = ReadWriteBuffer;
                        CounterUSART1RX = 0;                                     //Сбросить счётчик принятых байт данных
                        CounterTimeOut = 0;                                      //Сброс счётчика тайм аута
                        FlagAddressCompare = 0;                                  //Сбросить флаг совпадения адреса/выбора                         
                        FlagRXTimeOut = 0;                                       //Отключение таймера таймаута приёма
                        FlagRXComplete = 1;                                      //Установить флаг "данные приняты" / "приёмный буфер занят"
                      }  
                    break; 
                       
            default: __NOP();                                                     //Выход во всех остальных случаях, ошибка выбора действия
          }  
      } 
                                                                                  //Сброс флага по событию "передающий буфер пуст" 
    }

  if (getUSART1TXDataRegisterEmpty()&&getUSART1TXIOn())                          //Передающий регистр пуст
    {  
      Pointer_8 = (uint8_t *)& DataOut;                                           //Определение адреса буфера для приёма данных
      Pointer_8 = Pointer_8 + CounterUSART1TX;                                    //Сложить адрес буфера с необходимым смещением
      USART1DataRegister = (*Pointer_8);                                          //Записать передаваемый байт в регистр данных, сброс флага события "передающий буфер пуст" 
      CounterUSART1TX++;  
      if (CounterUSART1TX == ByteNumberUSART1TX)
        {
          USART1TXEInterruptDisable();                                            //Выключить прерывание по событию "передающий регистр данных пуст", USART1
          CounterUSART1TX = 0;
          FlagTXComplete = 0;                                                     //Сбросить флаг "данные отправлены" / "передающий буфер занят"  
        } 
    }
    
//  if (getUSART1TransmissionComplete()&&(!getUSART1TXIOn()))                                           //Проверить флаг "передатчик готов к работе"
//    {
//      //RS422TXDisable;                                                         //
//      //USART1TransmitterDisable();                                             //Выключить передатчик
//    }  
    
  if (getUSART1OverrunError())                                                    //Ошибка перезаписи данных
    {
      
    }
 
}	
	
	
//**********************************************************************************************************************************
//Обработчик прерывания от SPI1 		
void SPI1_IRQHandler(void) 
{
  if (getSPITXBufferEmpty()&&getSPITXIBufferEmpty())
    {
      SPI1->DR = DACConfig();//Запись подготовленных данных ЦАП в регистр передачи SPI1 и сброс флага события "передающий буфер пуст"
      CounterTXSPI1++;
      if (CounterTXSPI1 == 1)
        {
          SPI1TXEInterruptDisable();
          CounterTXSPI1 = 0;
					//DisableDAC_XY;
          //SPI1Disable(); 
					FlagDacDisable = 1;
        }
    }
}	
 
