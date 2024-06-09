#include "systick.h"
#include "MKL46Z4.h"
#include "gpio.h"

volatile uint32_t redLed = 0;
volatile uint32_t greenLed = 0;

void Systick_Init()
{
	//Enable SysTick Interrupt
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	//Chon nguon Clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	//Set LOAD
	SysTick->LOAD = SystemCoreClock/1000 - 1;
}

void Systick_Handler()
{
	greenLed++;
	if (greenLed == 500)
	{
		PTD->PTOR |= 1<<5;
		greenLed = 0;
	}
	if (state == 2)
	{
		redLed++;
		if (redLed == 250)
		{
			PTE->PTOR |= 1<<29;
			redLed = 0;
		}
	}
}