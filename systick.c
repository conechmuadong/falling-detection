#include "systick.h"
#include "MKL46Z4.h"
#include "gpio.h"

void Systick_Init()
{
	//Enable SysTick Interrupt
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	//Select Clock Source for SysTick
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	
	//Enable SysTick Timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	//Set LOAD value
	SysTick->LOAD = SystemCoreClock/1000 - 1;
}