/* Created: 16.06.2016
 * DAC_AD5324.h
 *
 * 
 * 
 */ 


#ifndef DAC_AD5324Variable_H_
#define DAC_AD5324Variable_H_

#include "stm32f10x.h"
#include "DAC_AD5324.h"

//**********************************************************************************************************************************
//Структуры данных для ЦАП
struct 
{
  uint16_t  DataBits                  :12;    //Биты данных
  uint8_t   Latch                     :1;   //Управление обновлением выходов
  uint8_t   PowerDown                 :1;     //Режим энергосбережения
  uint8_t   NumberChannel 						:2;   //Номер канала
  
} AD5324_InputShiftRegister;

#endif /* DAC_AD5324Variable_H_ */
