#include "systick.h"
#include "MKL46Z4.h"
#include "gpio.h"

void Systick_Init()
{
	//Enable SysTick Interrupt
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	//Chon nguon Clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	
	//
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	//Set LOAD
	SysTick->LOAD = SystemCoreClock/1000 - 1;
}