/* Created: 16.06.2016
 * DAC_AD5324.c
 *
 *
 *
 */ 

#include "DAC_AD5324.h"
//#include "stdlib.h"
#include "stm32f1xx_hal.h"

/**Private function prototype*/
static void DacConfigPin(void); 														//Init dac pins
static void CaseDACNumber(uint8_t DACNumber); 											//Select dac's number, if more then one
static void CaseDACChannel(uint8_t DACChannel); 										//Select dac's channel
static void DACChannelControl(uint8_t ChannelControl); 									//Dac channel control setting
static void DACOutControl(uint8_t OutControl); 											//Dac out control satting
static void DACData(int16_t Data); 													//Data for sending into dac 


//**********************************************************************************************************************************
//Set Dac Voltage
uint8_t * setDacVoltage(uint8_t DacChannel, int16_t Data)
{
	setDacSetting(PowerDownNormalState, LatchUpdateAllOut);
	Data += calibDacData[DacChannel - 1];
	if (DacChannel > 0 && DacChannel <= 4) CaseDACNumber(1); 
	else if (DacChannel > 4 && DacChannel <= 8)		{ CaseDACNumber(2); DacChannel -= 4; }
	else if (DacChannel > 8 && DacChannel <= 12)	{ CaseDACNumber(3); DacChannel -= 8; }
	else if (DacChannel > 12 && DacChannel <= 16)	{ CaseDACNumber(4); DacChannel -= 12; }
	else if (DacChannel > 16 && DacChannel <= 20)	{ CaseDACNumber(5); DacChannel -= 16; }
	else if (DacChannel > 20 && DacChannel <= 24)	{ CaseDACNumber(6); DacChannel -= 20; }
	else if (DacChannel > 24 && DacChannel <= 28)	{ CaseDACNumber(7); DacChannel -= 24; }
	else if (DacChannel > 28 && DacChannel <= 32)	{ CaseDACNumber(8); DacChannel -= 28; }
	else CaseDACNumber(10);

	CaseDACChannel(DacChannel);
	DACData(Data * 2);
	
	return (uint8_t *)&AD5324_Data.DacData;
}

//**********************************************************************************************************************************
//Set Dac Voltage all
uint16_t * setDacVoltageAll(uint8_t DacChannel, int16_t Data)
{
	setDacSetting(PowerDownNormalState, LatchUpdateAllOut);
	CaseDACNumber(9);
	DACData(Data * 2);
	
	CaseDACChannel(DacChannel);
	
	return (uint16_t *)&AD5324_Data.DacData;
}


//**********************************************************************************************************************************
//Set Dac Voltage
uint16_t setDacVoltageThrowNumbCh(uint8_t DacNumber, uint8_t DacChannel, uint16_t Data)
{
	CaseDACNumber(DacNumber);
	CaseDACChannel(DacChannel);
	DACData(Data);
	
	return AD5324_Data.DacData;
}

//**********************************************************************************************************************************
//Set control setting
action setDacSetting(uint8_t ChannelControl, uint8_t OutControl)
{
	DACChannelControl(ChannelControl);
	DACOutControl(OutControl);
	DacConfigPin();
	
	return actOK;
}

//**********************************************************************************************************************************
//Init pins
static void DacConfigPin(void)
{
//	GPIOA->CRL	&= ~(GPIO_CRL_CNF5 | GPIO_CRL_CNF7);                                      																	//Сброс CNF в 00
//  GPIOA->CRL	|= GPIO_CRL_CNF5_1 | GPIO_CRL_CNF7_1;                                 																		//Установка режима AF-PP
//  GPIOA->CRL  |= GPIO_CRL_MODE5 | GPIO_CRL_MODE7;                                       																	//Установка режима outmax 50MHz
  GPIOB->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1);			                                																//сброс CNF в 00								
  GPIOB->CRL  |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1);	                                      																//установка режима РР-GP
  GPIOC->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF5);			//сброс CNF в 00								
  GPIOC->CRL  |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5);	  //установка режима РР-GP
}
//**********************************************************************************************************************************
//Dac select
static void CaseDACNumber(uint8_t DACNumber)
{
  switch(DACNumber)
      {
        case 1: EnableDAC_X1; 
                break;  
        case 2: EnableDAC_X2; 
                break;  
        case 3: EnableDAC_X3; 
                break;  
        case 4: EnableDAC_X4; 
                break;  
        case 5: EnableDAC_Y1; 
                break;  
        case 6: EnableDAC_Y2; 
                break;  
        case 7: EnableDAC_Y3; 
                break;  
        case 8: EnableDAC_Y4; 
                break;  
        case 9: EnableDAC_XY; 
                break;
        case 10:DisableDAC_XY;  
                //break;
        default:__NOP();        
      } 
}

//**********************************************************************************************************************************
//Channel select
static void CaseDACChannel(uint8_t DACChannel)
{
  switch(DACChannel)
      {
        case 1: AD5324_Data.NumberChannelBits = Channel_4;                 
                break;                
        case 2: AD5324_Data.NumberChannelBits = Channel_3;                 
                break;              
        case 3: AD5324_Data.NumberChannelBits = Channel_2;                 
                break;              
        case 4: AD5324_Data.NumberChannelBits = Channel_1;                 
                break;                        
        default:__NOP();                                                   
      }
}

//**********************************************************************************************************************************
//
static void DACChannelControl(uint8_t ChannelControl)
{
  switch(ChannelControl)
      {
        case 0: AD5324_Data.PowerDownBits = PowerDownHighImpedanceOut;      
                break;  
        case 1: AD5324_Data.PowerDownBits = PowerDownNormalState;           
                break;
        default:__NOP();                                                    
      }  
}

//**********************************************************************************************************************************
//
static void DACOutControl(uint8_t OutControl)
{ 
  switch(OutControl)
      {       
        case 0: AD5324_Data.LatchBits = LatchUpdateAllOut;             			
                break;          
        case 1: AD5324_Data.LatchBits = LatchUpdateAddressedOut;               
                break;        
        default:__NOP();                                                        
      }  
}

//**********************************************************************************************************************************
//
static void DACData(int16_t Data)
{    
	AD5324_Data.DataBits = Data;                                         //
}

//**********************************************************************************************************************************
//
//uint16_t DACConfig(void)
//{    
//  uint16_t DataConfigDAC;
//  uint16_t *Pointer_16;
//  
//	Pointer_16 = (uint16_t *)& AD5324_Data.DacData;
//  DataConfigDAC = (*Pointer_16);                                                    //
//  return DataConfigDAC;
//}
