/**
 **\file ws2812B.c
 **\brief Module for WS2812B leds
 **
***/

#include "ws2812b.h"
//#include "cmsis_os.h"
#include "stm32f1xx_hal.h"

/**\brief Private function prototype
 ***/
static void circDmaEnable(DMA_Channel_TypeDef *dma_channel);
static void circDmaDisable(DMA_Channel_TypeDef *dma_channel);
static void updateLedColors(uint8_t green, uint8_t red, uint8_t blue,const uint8_t position);

/**\brief Instance of PWM_BUFFER_Typedef
 ***/
static PWM_BUFFER_Typedef pwmLedBuffer = 
{ 		
	.loadFirstFrame = { 0 }, ///<reset frame init	
	.bitBuffer = { 0 },
	.loadSecondFrame = 0
};        

/**\brief Init GPIO, Timer, DMA
 **
 ** RCC->AHBENR
 ** RCC->APB1ENR
 ** Enable dma channel for use in timer, enable timer.
 ** 
 ** DMA1_Channel6->CCR
 ** Enable transport complete interrupt, dma direction set to "read from memory",
 ** increment memory address, peripheral data size is 16 bit (timer CCR register),
 ** memory size is 8 bit.
 ** 
 ** DMA1_Channel6->CNDTR
 ** Set number of data transfer.
 ** 
 ** DMA1_Channel6->CPAR
 ** Set the address where data will be written
 **
 ** Set the address from where data will be read
 **
 ** NVIC_SetPriority, NVIC_EnableIRQ - set timer priority, enable timer interrupt
 ** GPIOx->CRx - config GPIO for timer channel
 ** 
 ** DMA1_Channelx - config DMA, prereset DMA EN bit.
 ** Direction M2P, memory address increment, MEM size 8 bit, Periph size 16 bit.
 ** DMA1_Channelx->CNDTR - number of data bytes to transfer.
 ** DMA1_Channelx->CPAR - set peripheral address.
 ** DMA1_Channelx->CMAR - set memory buffer address.
 ** 
 ** GPIOx->CRx - configuration output pin, reset CNF to 0, set AF-PP, out speed max 10MHz
 ** 
 ** TIM-> - set input clock prescaler to TIM_PRESCALER, set period PWM to TIM_PWM_PERIOD(look define upper),  
 ** set compare register first to 0, PWM mode 1, enable PWM channel X, enable DMA request, 
 **
***/
void initPwmChannel(TIM_TypeDef *timer, DMA_Channel_TypeDef *dma_channel, GPIO_TypeDef *gpio) 
{
	UNUSED(timer);
	UNUSED(dma_channel);
	UNUSED(gpio);
	
	for (volatile uint8_t i = 0; i < sizeof(pwmLedBuffer.bitBuffer); i++) pwmLedBuffer.bitBuffer[i] = TIM_0LEVEL;
	
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;	
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	
	DMA1_Channel6->CCR &= (uint16_t)~DMA_CCR_EN;
	DMA1_Channel6->CCR |= (uint16_t)(DMA_CCR_DIR | DMA_CCR_MINC |/* DMA_CCR_CIRC |*/ /*DMA_CCR_MSIZE_0 |*/ DMA_CCR_PSIZE_0 /*| DMA_CCR_PL*/);
	
	DMA1_Channel6->CNDTR = (uint16_t)sizeof(pwmLedBuffer);
	DMA1_Channel6->CPAR = (uint32_t)(&TIM3->CCR1);
	//DMA1_Channel6->CPAR = (uint32_t)(&TIM3->DMAR);
	DMA1_Channel6->CMAR = (uint32_t)((uint8_t *)&pwmLedBuffer);		
  
	GPIOA->CRL	&= ~GPIO_CRL_CNF6;                                
	GPIOA->CRL	|= GPIO_CRL_CNF6_1;                               
	GPIOA->CRL  |= GPIO_CRL_MODE6_0;       
  	  		                  
	TIM3->PSC = TIM_PRESCALER;     				                  
	TIM3->ARR = TIM_PWM_PERIOD;    					  		          
	TIM3->CCR1 = 0;
    
	TIM3->CCMR1 |= (uint32_t)(TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE);                //Режим PWM2, включить выход сигнала, PA6
    TIM3->CCER |= (uint32_t)TIM_CCER_CC1E;

	TIM3->CR1 |= (uint32_t)(TIM_CR1_URS /*| TIM_CR1_ARPE*/);    			          //обновление с помощью бита UG не устанавливает флаг прерывания
    TIM3->DIER |= (uint32_t)TIM_DIER_CC1DE;
	//TIM3->CR2 |= TIM_CR2_CCDS;
	
	TIM3->CR1 |= (uint32_t)TIM_CR1_CEN; 
}

/**\brief Function for fill led array, data buffer transmitted to leds.
 **
 ***/
void fillLed(void)
{ 	
	DMA1_Channel6->CCR &= (uint32_t)~DMA_CCR_EN;
	DMA1_Channel6->CNDTR = (uint32_t)(sizeof(pwmLedBuffer));
	DMA1_Channel6->CCR |= (uint32_t)DMA_CCR_EN;
}

/**\brief Function enable DMA CIRC mode.
 **
 **\warning Before using, make sure that there is an instance of PWM_BUFFER_CIRC_Typedef
 **
 ***/
static void circDmaEnable(DMA_Channel_TypeDef *dma_channel)
{ 	
	UNUSED(dma_channel);	
	DMA1_Channel6->CCR |= (uint16_t)(DMA_CCR_CIRC);
}

/**\brief Function enable DMA CIRC mode.
 **
 ***/
static void circDmaDisable(DMA_Channel_TypeDef *dma_channel)
{ 	
	UNUSED(dma_channel);	
	DMA1_Channel6->CCR &= (uint16_t)(~DMA_CCR_CIRC);
}

/**\brief Function position led.
 **
 ***/
enum error_led_param updatePositionLed(uint8_t green, uint8_t red, uint8_t blue, uint8_t position)
{ 
	if (position > LED_NUMBERS_OF_STRIP) return NumbersOfLed_Err;
	if (green > 255 || red > 255 || blue > 255) return ColorParam_Err;
	
	updateLedColors(green, red, blue, position);
	fillLed();
	
	return InputParam_Ok;
}

/**\brief Function fill whole buffer.
 **
 ***/
enum error_led_param updateAllLeds(uint8_t green, uint8_t red, uint8_t blue)
{ 	
	if (green > 255 || red > 255 || blue > 255) return ColorParam_Err;
	
	for (volatile uint16_t i = 0; i < LED_NUMBERS_OF_STRIP; i++)
	updateLedColors(green, red, blue, i);
	fillLed();
	
	return InputParam_Ok;
}

/**\brief Function smootly chenges color.
 **
 ***/
void chengeColorSmootly()
{
	static uint8_t green = 0, red = 0, blue = 0; 
	static uint8_t 	flagGreenUpDown = 1, flagRedUpDown = 1, flagBlueUpDown = 1;	
		
	if (flagGreenUpDown == 0) green -= 1;
	else if (flagGreenUpDown == 1)  green += 1;
		
	if (flagRedUpDown == 0) red -= 1;
	else if (flagRedUpDown == 1)  red += 1;
		
	if (flagBlueUpDown == 0) blue -= 1;
	else if (flagBlueUpDown == 1)  blue += 1;
		
	if (green == 20) flagGreenUpDown = 1;
	else if (green == 150) flagGreenUpDown = 0;
		
	if (red == 20) flagRedUpDown = 1;
	else if (red == 210) flagRedUpDown = 0;
		
	if (blue == 20) flagBlueUpDown = 1;
	else if (blue == 140) flagBlueUpDown = 0;
	updateAllLeds(green, red, blue);
}

/**\brief Function "updateLedColors" send data into led massive (strip, pcb led and other).
 **
 ***/
static void updateLedColors(uint8_t green, uint8_t red, uint8_t blue, const uint8_t position)
{
	volatile uint8_t i;
	static uint8_t prepColors;	
	
	prepColors = green;	
	for (i = 0; i < 8 ; i++)
	{
		if ((prepColors & 0x80) != 0) pwmLedBuffer.bitBuffer[i + position*COLORS_DATA_BITS] = TIM_1LEVEL;
		else pwmLedBuffer.bitBuffer[i + position*COLORS_DATA_BITS] = TIM_0LEVEL;
		prepColors <<= 1;
	}

	prepColors = red;
	for (i = 0; i < 8; i++)
	{
		if ((prepColors & 0x80) != 0) pwmLedBuffer.bitBuffer[i + 8 + position*COLORS_DATA_BITS] = TIM_1LEVEL;
		else pwmLedBuffer.bitBuffer[i + 8 + position*COLORS_DATA_BITS] = TIM_0LEVEL;
		prepColors <<= 1;
	}

	prepColors = blue;
	for (i = 0; i < 8; i++)
	{
		if ((prepColors & 0x80) != 0) pwmLedBuffer.bitBuffer[i + 16 + position*COLORS_DATA_BITS] = TIM_1LEVEL;
		else pwmLedBuffer.bitBuffer[i + 16 + position*COLORS_DATA_BITS] = TIM_0LEVEL;
		prepColors <<= 1;
	}
		
	pwmLedBuffer.loadSecondFrame = LOAD_FRAME_TIME;
}
