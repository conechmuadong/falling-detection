#include "MKL46Z4.h"
#include "GPIO.h"

void Init_Button(){
	// TODO: Add code to initialize the button switch  
	state = 0;
	SIM->SCGC5 |= 1<<11;
	//Chon che do GPIO
	PORTD->PCR[3] |= 1<<8;
	PORTD->PCR[12] |= 1<<8;
	//Chon input
	PTC->PDDR &= ~((uint32_t)(1 << 3));
	PTC->PDDR &= ~((uint32_t)(1 << 12));
	//Pin select
	PORTC->PCR[3] |= 1<<0;
	PORTC->PCR[3] |= 1<<1;
	
	PORTC->PCR[12] |= 1<<0;
	PORTC->PCR[12] |= 1<<1;
	//Bat interrupt pin
	PORTC->PCR[3] |= 1<<17 | 1<<19;
	PORTC->PCR[12] |= 1<<17 | 1<<19;
	//Interrupt Port
	NVIC_ClearPendingIRQ(PORTC_PORTD_IRQn);
	NVIC_EnableIRQ(PORTC_PORTD_IRQn);
	NVIC_SetPriority(PORTC_PORTD_IRQn,1);
	
}

void Init_LED(){
	// TODO: Add code to initialize the LEDs
	//Bat clock
	SIM->SCGC5 |= 1<<12;
	SIM->SCGC5 |= 1<<13;
	//Chon che do GPIO
	PORTD->PCR[5] |= 1<<8;
	PORTE->PCR[29] |= 1<<8;
	//Chon output
	PTD->PDDR |= 1<<5;
	PTE->PDDR |= 1<<29;
	//set1
	PTD->PSOR |= 1<<5;
	PTE->PSOR |= 1<<29;
}


void PORTC_PORTD_IRQHandler()
{
	if ((PTC->PDIR & (1<<3)) == 0)
	{
		if (state == 0)
		{
			state = 1;
			SysTick->CTRL |= 1<<0;
		}
		else if (state == 1)
		{
			state = 0;
			SysTick->CTRL &= ~((uint32_t)(1<<0));
		}
	}
	
	if ((PTC->PDIR & (1<<12)) == 0)
	{
		if (state != 0)
		{
			state = 1;
			SysTick->CTRL |= 1<<0;
		}
	}
}