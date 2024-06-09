#include "MKL46Z4.h"
#include "GPIO.h"

void Init_Button(){
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
  //Button Init
	PORTC->PCR[12] |= PORT_PCR_MUX(0x1);
	PORTC->PCR[12] |= PORT_PCR_PE_MASK;
	PORTC->PCR[12] |= PORT_PCR_PS_MASK;
	PORTC->PCR[12] |= PORT_PCR_IRQC(0xA);
	PTC->PDDR &= ~((uint32_t)1<<12);
	
	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);
}

//Enable PTC5 interrupt for enable fall detect
void Init_FreeFall_IRQ(){
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	PORTC->PCR[5] |= PORT_PCR_MUX(0x1);
	PORTC->PCR[5] |= PORT_PCR_IRQC(0xA);
	PTC->PDDR &= ~((uint32_t)1<<5);
	
	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);

void Init_LED(){
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	PORTD->PCR[5] |= PORT_PCR_MUX(0x1);
	PORTE->PCR[29] |= PORT_PCR_MUX(0x1);
	PTD->PDDR |= 1 << 5;
	PTE->PDDR |= 1 << 29;
	
	PTD->PSOR |= 1 << 5;
	PTE->PSOR |= 1 << 29;
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