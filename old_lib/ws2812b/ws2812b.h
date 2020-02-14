/**\brief Heder files.
 ***/
#pragma once

/**\brief Includes.
 **
 ***/
#include "stm32f1xx_hal.h"

/**\brief Definitions.
 **
 ***/

///Define parameters of led strip and buffer
#define THREE_COLORS				///<THREE_COLORS  - strip colors, if not def, the colors set to four colors led.
#define LED_NUMBERS_OF_STRIP 8		///<LED_NUMBERS_OF_STRIP - led numbers in the led strip. 
#define COLORS_DATA_BITS 24			///<BUFFER_SIZE_STRIP - compute the buffer size.    
/// \cond
#ifdef THREE_COLORS
#define COLOR_NUMBERS_OF_LED 3	///<Numbers of led in strip.
    //#define DATA_BYTES_QUANTITY 3 
#else  
#define COLOR_NUMBERS_OF_LED 4
    //#define DATA_BYTES_QUANTITY 4
#endif
/// \endcond
#define BUFFER_SIZE_STRIP (LED_NUMBERS_OF_STRIP * COLOR_NUMBERS_OF_LED * 8)///<Size for data buffer.  

///Define parameters for timer and timer's PWM channel
#define TIM_PRESCALER       1   ///<Prescaler for input clock, output clock 36MHz, period 27,7nS.
#define TIM_PWM_PERIOD      44  ///<PWM period constant, PWM frequency is 800kHz, step PWM is 27,7nS (determined timer's input clock).      
#define TIM_0LEVEL          12  ///<Low level constant, PWM signal set to 0.35uS HL + 0.9uS LL, tolerance +-150nS.   
#define TIM_1LEVEL          32  ///<High level constant, PWM signal set to 0.9uS HL + 0.35uS LL, tolerance +-150nS.      
#define LOAD_FRAME_TIME     0   ///<Low level frame constant, it's no less 50uS PWM signal shold be 0.

/// Enum.
enum error_led_param
{
	InputParam_Ok,
	NumbersOfLed_Err,
	ColorParam_Err
		
} error_led_param_enum;


///Data typedef.
///Buffer typedef struct.
typedef struct 
{
	uint8_t loadFirstFrame[300]; 			///<Start reset frame, length is 40 * 1.25uS = 50uS (operation frequency's period)
	uint8_t bitBuffer[BUFFER_SIZE_STRIP]; 	///<It's data buffer, GRB format, 24 bytes to 3 bytes of color. 
	uint8_t loadSecondFrame; 				///<Reset frame at end of transmission.

} PWM_BUFFER_Typedef;

///Buffer typedef struct for CIRC DMA mode.
///why 300 instead of 40 - my leds don't work, if  reset code less then 250uS, buf in datasheet this value is >=50uS
typedef struct 
{
	uint8_t loadFirstFrame[300];  			///<Start reset frame, length is 300 * 1.25uS = 375uS (operation frequency's period), in CIRC DMA mode
	uint8_t bitBuffer[BUFFER_SIZE_STRIP];  	///<It's data buffer, GRB format, 24 bytes to 3 bytes of color. 
	uint8_t loadSecondFrame;  				///<Reset frame at end of transmission.

} PWM_BUFFER_CIRC_Typedef;

/**\brief Public function prototype.
 **
 ***/
void initPwmChannel(TIM_TypeDef *timer, DMA_Channel_TypeDef *dma_channel, GPIO_TypeDef *gpio);
void fillLed(void);
enum error_led_param updatePositionLed(uint8_t green, uint8_t red, uint8_t blue, uint8_t position);
enum error_led_param updateAllLeds(uint8_t green, uint8_t red, uint8_t blue);
void chengeColorSmootly(void);
