/* Created: 16.06.2016
 * DAC_AD5324.h
 *
 * 
 * 
 */ 


//#ifndef DAC_AD5324_H_
//#define DAC_AD5324_H_
#pragma once

//#include "stm32f10x.h"
#include <stdint.h>

//**********************************************************************************************************************************
//PCB select
//#define PCB_01
#define PCB_02

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
//
#define     ClockHighDAC                (GPIOA->BSRR = GPIO_BSRR_BS5)
#define     ClockLowDAC                 (GPIOA->BSRR = GPIO_BSRR_BR5)
#define     DataHighDAC                 (GPIOA->BSRR = GPIO_BSRR_BS6)
#define     DataLowDAC                  (GPIOA->BSRR = GPIO_BSRR_BR6)

//**********************************************************************************************************************************
//
#define     Channel_1                   0x00
#define     Channel_2                   0x01
#define     Channel_3                   0x02
#define     Channel_4                   0x03

//**********************************************************************************************************************************
//
#define     PowerDownHighImpedanceOut   0x00
#define     PowerDownNormalState        0x01

//**********************************************************************************************************************************
//
#define     LatchUpdateAllOut           0x00
#define     LatchUpdateAddressedOut     0x01

#define		voutDefault					(uint16_t)0x0148

//**********************************************************************************************************************************
//Enum action status
typedef enum actionStatus
{
	actOK,
	actFalse
} action;

//**********************************************************************************************************************************
//Config struct
static union 
{
	uint16_t DacData;
	struct
	{
		uint16_t  DataBits				: 12;    
		uint8_t   LatchBits             : 1;     
		uint8_t   PowerDownBits         : 1;     
		uint8_t   NumberChannelBits 	: 2;     
	} /*anonim struct*/ ;

} AD5324_Data;

//**********************************************************************************************************************/
//Calibration data for dac channels
//PCB_01
#ifdef PCB_01
static const int8_t calibDacData[32] =
{
	-3, 0, 6, -21,
	14, -17, 6, 12,
	13, -15, 1, 8,
	-11, 7, -3, -14,
	
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0		
};
#endif
//PCB_02
#ifdef PCB_02
static const int8_t calibDacData[32] =
{
	15, -9, -11, 11,
	8, -9, 11, -6,
	-14, 32, -5, 3,
	-4, 4, -5, -35,
	
	9, -15, -3, 0,
	-12, -7, 3, -2,
	-24, -5, 1, 3,
	-6, -6, -4, -12
	
};
#endif


//**********************************************************************************************************************************
//Function prototipy
uint8_t * setDacVoltage(uint8_t DacChannel, int16_t Data); 		//Set Dac Voltage
uint16_t * setDacVoltageAll(uint8_t DacChannel, int16_t Data); 		//Set Dac Voltage all
action setDacSetting(uint8_t ChannelControl, uint8_t OutControl);	//Set control setting

//#endif /* DAC_AD5324_H_ */
