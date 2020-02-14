#include "stm32f10x.h"


#define CLK_TIM 72000000
#define CLK_DIV   ((CLK_TIM/1000000)-1)

#define DelayTimer TIM2

extern uint32_t Delay;

void Delay_us (uint32_t Delay);
void Delay_ms (uint32_t Delay);
void DelayTimInit (void);
