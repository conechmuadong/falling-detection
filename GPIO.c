#include "MKL46Z4.h"
#include "GPIO.h"

void Init_Button(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
  
	PORTC->PCR[3] |= PORT_PCR_MUX(0x1);
	PORTC->PCR[3] |= PORT_PCR_PE_MASK;
	PORTC->PCR[3] |= PORT_PCR_PS_MASK;
	PORTC->PCR[3] |= PORT_PCR_IRQC(0xA);
	
	PORTC->PCR[12] |= PORT_PCR_MUX(0x1);
	PORTC->PCR[12] |= PORT_PCR_PE_MASK;
	PORTC->PCR[12] |= PORT_PCR_PS_MASK;
	PORTC->PCR[12] |= PORT_PCR_IRQC(0xA);
	PTC->PDDR &= ~((uint32_t)1<<12);
	
	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);
}

//Enable PTC5 interrupt for enable fall detect
void Init_FreeFall_IRQ(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	PORTC->PCR[5] |= PORT_PCR_MUX(0x1);
	PORTC->PCR[5] |= PORT_PCR_IRQC(0xA);
	PTC->PDDR &= ~((uint32_t)1<<5);
	
	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);
}
void Init_LED(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	PORTD->PCR[5] |= PORT_PCR_MUX(0x1);
	PORTE->PCR[29] |= PORT_PCR_MUX(0x1);
	PTD->PDDR |= 1 << 5;
	PTE->PDDR |= 1 << 29;
	
	PTD->PSOR |= 1 << 5;
	PTE->PSOR |= 1 << 29;
}