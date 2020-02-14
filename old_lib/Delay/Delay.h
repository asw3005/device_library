/* Created: 21.07.2016
 * Delay.h
 *
 *
 *
 */	
 
#ifndef Delay_H_
#define Delay_H_

#include "stm32f10x.h"

//**********************************************************************************************************************************
//
#define     F_CPU           36000000
#define     PRESKALER       (F_CPU/1000)-1

#define     DelayTimer      TIM2



//**********************************************************************************************************************************
//Переменные
extern      uint16_t      Delay;
extern      uint8_t       Flag;

//**********************************************************************************************************************************
//Прототипы функций
void TimInitForSensor(void);            //Настройка таймера для циклического опроса датчиков (частота 1с)



void Delay_us(uint16_t Delay);
void Delay_ms(uint16_t Delay);
void DelayTimeInitForUSART (void);//функция задержки для ожидания передачи последнего байта по USART
void DelayTimInit (void);
void _delay_us (uint32_t __us);//
void _delay_ms (uint32_t __ms); //
void _delay_loop (uint32_t __count);

#endif /* Delay_H_ */
