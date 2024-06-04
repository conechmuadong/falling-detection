/**
 * @file timer.c
 * @date June 2th 2024
 * @brief PIT timer setup
 * This file contains the implementation of functions for PIT timer setup
 * for displaying the time on the LCD.
*/

#include "timer.h"

void Timer_Init(void){
	// Enable PIT clock
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

	// Enable PIT module
	PIT->MCR = 0x00U;

	// Set PIT counter value of channel 0 to 13.929.999 pulse (0x00d551f3), as 1s timer.
	// This timer is used for updating the time on the LCD.
	PIT->CHANNEL[0].LDVAL = 0x00d551f3U;
	PIT->CHANNEL[0].TCTRL = 0x00U;
	// Clear interrupt flag
	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
	// Enable PIT timer channel 0 and interrupt
	PIT->CHANNEL[0].TCTRL = (PIT_TCTRL_TIE_MASK|PIT_TCTRL_TEN_MASK);
	
	// Set PIT counter value of channel 1 to 6.464.999 pulse (0x000f4240), as 0.5s timer.
	// This timer is used for blinking the colon on the LCD.
	PIT->CHANNEL[1].LDVAL = 0x006aa8afU;
	PIT->CHANNEL[1].TCTRL = 0x00U;
	// Clear interrupt flag
	PIT->CHANNEL[1].TFLG = PIT_TFLG_TIF_MASK;
	// Enable PIT timer channel 1 and interrupt
	PIT->CHANNEL[1].TCTRL = (PIT_TCTRL_TIE_MASK|PIT_TCTRL_TEN_MASK);

	// Configure NVIC for PIT
	NVIC_SetPriority(22,2);
	NVIC_EnableIRQ(22);
}