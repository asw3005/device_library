/* Created: 16.06.2016
 * DAC_AD5324.h
 *
 * 
 * 
 */ 


#ifndef DAC_AD5324_H_
#define DAC_AD5324_H_

#include "stm32f10x.h"

//**********************************************************************************************************************************
//Номера ЦАП
#define     EnableDAC_X1                (GPIOC->BSRR = GPIO_BSRR_BR4)  
#define     DisableDAC_X1               (GPIOC->BSRR = GPIO_BSRR_BS4)
#define     EnableDAC_X2                (GPIOC->BSRR = GPIO_BSRR_BR5) 
#define     DisableDAC_X2               (GPIOC->BSRR = GPIO_BSRR_BS5)
#define     EnableDAC_X3                (GPIOB->BSRR = GPIO_BSRR_BR0) 
#define     DisableDAC_X3               (GPIOB->BSRR = GPIO_BSRR_BS0)
#define     EnableDAC_X4                (GPIOB->BSRR = GPIO_BSRR_BR1) 
#define     DisableDAC_X4               (GPIOB->BSRR = GPIO_BSRR_BS1)
      
#define     EnableDAC_Y1                (GPIOC->BSRR = GPIO_BSRR_BR0)  
#define     DisableDAC_Y1               (GPIOC->BSRR = GPIO_BSRR_BS0)
#define     EnableDAC_Y2                (GPIOC->BSRR = GPIO_BSRR_BR1) 
#define     DisableDAC_Y2               (GPIOC->BSRR = GPIO_BSRR_BS1)
#define     EnableDAC_Y3                (GPIOC->BSRR = GPIO_BSRR_BR2) 
#define     DisableDAC_Y3               (GPIOC->BSRR = GPIO_BSRR_BS2)
#define     EnableDAC_Y4                (GPIOC->BSRR = GPIO_BSRR_BR3) 
#define     DisableDAC_Y4               (GPIOC->BSRR = GPIO_BSRR_BS3)
  
#define     EnableDAC_XY                ((GPIOB->BSRR = GPIO_BSRR_BR0 | GPIO_BSRR_BR1)&(GPIOC->BSRR = GPIO_BSRR_BR0 | GPIO_BSRR_BR1 | GPIO_BSRR_BR2| GPIO_BSRR_BR3 | GPIO_BSRR_BR4 | GPIO_BSRR_BR5))             
#define     DisableDAC_XY               ((GPIOB->BSRR = GPIO_BSRR_BS0 | GPIO_BSRR_BS1)&(GPIOC->BSRR = GPIO_BSRR_BS0 | GPIO_BSRR_BS1 | GPIO_BSRR_BS2| GPIO_BSRR_BS3 | GPIO_BSRR_BS4 | GPIO_BSRR_BS5))

//**********************************************************************************************************************************
//Сигналы стробирования и данных
#define     ClockHighDAC                (GPIOA->BSRR = GPIO_BSRR_BS5)
#define     ClockLowDAC                 (GPIOA->BSRR = GPIO_BSRR_BR5)
#define     DataHighDAC                 (GPIOA->BSRR = GPIO_BSRR_BS6)
#define     DataLowDAC                  (GPIOA->BSRR = GPIO_BSRR_BR6)

//**********************************************************************************************************************************
//Номера каналов ЦАП
#define     Channel_1                   0x00
#define     Channel_2                   0x01
#define     Channel_3                   0x02
#define     Channel_4                   0x03

//**********************************************************************************************************************************
//Управление режимом энергосбережения
#define     PowerDownHighImpedanceOut   0x00
#define     PowerDownNormalState        0x01

//**********************************************************************************************************************************
//Управление выходами
#define     LatchUpdateAllOut           0x00
#define     LatchUpdateAddressedOut     0x01


//**********************************************************************************************************************************
//Прототипы функций
void DacConfigPin(void);													//Настройка выводов управления ЦАП
void CaseDACNumber(uint8_t DACNumber);          //Выбор ЦАП
void CaseDACChannel(uint8_t DACChannel);          //Выбор канала ЦАП
void DACChannelControl(uint8_t ChannelControl); //Выбор режима работы ЦАП (энергосбережение)
void DACOutControl(uint8_t OutControl);           //Выбор режима обновления данных на выходах
void DACData(uint16_t Data);                    //Запись данных для ЦАП
uint16_t DACConfig(void);                         //Чтение готовых данных для записи в ЦАП


#endif /* DAC_AD5324_H_ */
