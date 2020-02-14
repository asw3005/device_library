/* Created: 16.06.2016
 * DAC_AD5324.c
 *
 *
 *
 */ 

#include "DAC_AD5324.h"
#include "DAC_AD5324Variable.h"

//**********************************************************************************************************************************
//Настройка управления ЦАП
void DacConfigPin(void)
{
  GPIOB->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1);			                                																//сброс CNF в 00								
  GPIOB->CRL  |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1);	                                      																//установка режима РР-GP
  GPIOC->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF5);			//сброс CNF в 00								
  GPIOC->CRL  |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5);	  //установка режима РР-GP
}
//**********************************************************************************************************************************
//Выбор ЦАП
void CaseDACNumber(uint8_t DACNumber)
{
  switch(DACNumber)
      {
        case 1: EnableDAC_X1; //Активировать первый ЦАП Х 
                break;  
        case 2: EnableDAC_X2;   //Активировать второй ЦАП Х 
                break;  
        case 3: EnableDAC_X3; //Активировать третий ЦАП Х 
                break;  
        case 4: EnableDAC_X4;   //Активировать четвёртый ЦАП Х 
                break;  
        case 5: EnableDAC_Y1; //Активировать первый ЦАП Y 
                break;  
        case 6: EnableDAC_Y2;   //Активировать второй ЦАП Y 
                break;  
        case 7: EnableDAC_Y3; //Активировать третий ЦАП Y 
                break;  
        case 8: EnableDAC_Y4;   //Активировать четвёртый ЦАП Y 
                break;  
        case 9: EnableDAC_XY; //Активировать ЦАПы ХY 
                break;
        //case 10:DisableDAC_XY;  //Деактивировать ЦАПы ХY 
                //break;
        default:__NOP();        //Выход во всех остальных случаях
      } 
}

//**********************************************************************************************************************************
//Выбор канала ЦАП
void CaseDACChannel(uint8_t DACChannel)
{
  switch(DACChannel)
      {
        case 1: AD5324_InputShiftRegister.NumberChannel = Channel_4;               //Активировать первый канал ЦАП 
                break;                
        case 2: AD5324_InputShiftRegister.NumberChannel = Channel_3;                 //Активировать второй канал ЦАП 
                break;              
        case 3: AD5324_InputShiftRegister.NumberChannel = Channel_2;               //Активировать третий канал ЦАП 
                break;              
        case 4: AD5324_InputShiftRegister.NumberChannel = Channel_1;                 //Активировать четвёртый канал ЦАП 
                break;                        
        default:__NOP();                                                           //Выход во всех остальных случаях
      }
}

//**********************************************************************************************************************************
//Выбор режима работы ЦАП (энергосбережение)
void DACChannelControl(uint8_t ChannelControl)
{
  switch(ChannelControl)
      {
        case 0: AD5324_InputShiftRegister.PowerDown = PowerDownHighImpedanceOut;    //Выходы находятся в высокоимпедансном состоянии 
                break;  
        case 1: AD5324_InputShiftRegister.PowerDown = PowerDownNormalState;           //Выходы работают в нормальном режиме
                break;
        default:__NOP();                                                            //Выход во всех остальных случаях
      }  
}

//**********************************************************************************************************************************
//Выбор режима обновления данных на выходах
void DACOutControl(uint8_t OutControl)
{ 
  switch(OutControl)
      {       
        case 0: AD5324_InputShiftRegister.Latch = LatchUpdateAllOut;           			//Данные попадают на все каналы одновременно
                break;          
        case 1: AD5324_InputShiftRegister.Latch = LatchUpdateAddressedOut;             //Данные попадают только на адресуемые входные регистры каналы
                break;        
        default:__NOP();                                                           //Выход во всех остальных случаях
      }  
}

//**********************************************************************************************************************************
//Запись данных для ЦАП
void DACData(uint16_t Data)
{    
  AD5324_InputShiftRegister.DataBits = Data;                                        //Запись данных для ЦАП
}

//**********************************************************************************************************************************
//Чтение готовых данных для записи в ЦАП
uint16_t DACConfig(void)
{    
  uint16_t DataConfigDAC;
  uint16_t *Pointer_16;
  
  Pointer_16 = (uint16_t *)& AD5324_InputShiftRegister;
  DataConfigDAC = (*Pointer_16);                                                    //Считать конфигурацию и данные для отправки в ЦАП
  return DataConfigDAC;
}
