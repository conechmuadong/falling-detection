#include "GPIO.h"

void Init_Button(){
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	PORTC->PCR[3] |= PORT_PCR_MUX(0x1);
	PORTC->PCR[3] |= PORT_PCR_PE_MASK;
	PORTC->PCR[3] |= PORT_PCR_PS_MASK;
	PORTC->PCR[3] |= PORT_PCR_IRQC(0xA);
	PTC->PDDR &= ~((uint32_t)1<<3);
	
	PORTC->PCR[12] |= PORT_PCR_MUX(0x1);
	PORTC->PCR[12] |= PORT_PCR_PE_MASK;
	PORTC->PCR[12] |= PORT_PCR_PS_MASK;
	PORTC->PCR[12] |= PORT_PCR_IRQC(0xA);
	PTC->PDDR &= ~((uint32_t)1<<12);
	
	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);
}

void Init_FreeFall_IRQ_Input(){
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	PORTC->PCR[5] |= PORT_PCR_MUX(0x1);
	PORTC->PCR[5] |= PORT_PCR_IRQC(0xA);
	PTC->PDDR &= ~((uint32_t)1<<5);
	
	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);
}

void InitLED(){
	// TODO: Add code to initialize the LEDs
}

void setGreenLED(){
	//TODO: Add code to turn on Green LED on PB9
}

void setRedLED(){
	//TODO: Add code to turn on RED LED on PE26
}

void turnOffLED(){
	//TODO: Add code to turn off the LEDs
}