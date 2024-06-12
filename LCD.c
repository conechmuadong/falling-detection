/**
 * @file LCD.c
 * @date May 31st 2024
 * @brief LCD driver
 * This file contains the implementation of functions for LCD display on FRDM-KL46Z board.
*/

#include "MKL46Z4.h"
#include "LCD.h"


uint32_t sLCD_Init(void)
{
	// Enabling MCGIRCLK clock source to give the LCD 32.768kHz clock for its operation.
	MCG->C1  |= MCG_C1_IRCLKEN_MASK | MCG_C1_IREFSTEN_MASK;
	// Enabling required ports' clocks
	SIM->SCGC5|=SIM_SCGC5_PORTD_MASK;
	SIM->SCGC5|=SIM_SCGC5_PORTE_MASK;
	SIM->SCGC5|=SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5|=SIM_SCGC5_PORTC_MASK;
	// Enabling LCD controller clock
	SIM->SCGC5|=SIM_SCGC5_SLCD_MASK; 
	// Turning on safe mode while initializing
	LCD->GCR|=LCD_GCR_PADSAFE_MASK; 
	LCD->GCR &= ~LCD_GCR_LCDEN_MASK; 
	
	// Setting pins alternative functions to operate with LCD
	PORTD->PCR[0] = PORT_PCR_MUX(0u);
	PORTE->PCR[4] = PORT_PCR_MUX(0u);
	PORTB->PCR[23] = PORT_PCR_MUX(0u);
	PORTB->PCR[22] = PORT_PCR_MUX(0u);
	PORTC->PCR[17] = PORT_PCR_MUX(0u);
	PORTB->PCR[21] = PORT_PCR_MUX(0u);
	PORTB->PCR[7] = PORT_PCR_MUX(0u);
	PORTB->PCR[8] = PORT_PCR_MUX(0u);
	PORTE->PCR[5] = PORT_PCR_MUX(0u);
	PORTC->PCR[18] = PORT_PCR_MUX(0u);
	PORTB->PCR[10] = PORT_PCR_MUX(0u);
	PORTB->PCR[11] = PORT_PCR_MUX(0u);
	
	// General LCD Configuration
	LCD->GCR = 
	LCD_GCR_RVTRIM(0x00) |
	LCD_GCR_CPSEL_MASK | // Selects the LCD charge pump to supply the LCD voltage
	LCD_GCR_LADJ(0x03) | // Choose slowest clock source for charge pump
	LCD_GCR_VSUPPLY_MASK | // Choose the voltage supply for the LCD
	LCD_GCR_ALTDIV(0x00) | // Keep original clock source frequency
	LCD_GCR_SOURCE_MASK | // Choose the alternative clock source for the LCD
	LCD_GCR_LCLK(0x01) | // Prescaler for LCD clock
	LCD_GCR_DUTY(0x03); // Choose 1/4 duty cycle for the LCD, 4 backplanes for 32 segments
	
	LCD->AR = LCD_AR_BRATE(0x03); // Set LCD blinking rate
	LCD->FDCR=0U; //Clearing Fault detect LCD register
	
	// Enabling LCD pins
	LCD->PEN[0] =
	LCD_PEN_PEN(1u<<19 )|	//P19
	LCD_PEN_PEN(1u<<18) |	//P18
	LCD_PEN_PEN(1u<<17) |	//P17
	LCD_PEN_PEN(1u<<7)  |	//P7
	LCD_PEN_PEN(1u<<8)  |	//P8
	LCD_PEN_PEN(1u<<10) |	//P10
	LCD_PEN_PEN(1u<<11);	//P11

	LCD->PEN[1]=
	LCD_PEN_PEN(1u<<8 )|	//P40
	LCD_PEN_PEN(1u<<20)|	//P52
	LCD_PEN_PEN(1u<<5) |	//P37
	LCD_PEN_PEN(1u<<21)|	//P53
	LCD_PEN_PEN(1u<<6);		//P38

	// Set 4 COMs as Blackplanes pin
	LCD->BPEN[0] = LCD_BPEN_BPEN(1U<<18) | LCD_BPEN_BPEN(1U<<19);
	LCD->BPEN[1] = LCD_BPEN_BPEN(1U<<20) | LCD_BPEN_BPEN(1U<<8);

	// Set COMn waveforms to enable all segments 	
	LCD->WF8B[40] = (1<<0)|(1<<4);	//COM0
	LCD->WF8B[52] = (1<<1)|(1<<5);	//COM1
	LCD->WF8B[19] = (1<<2)|(1<<6);	//COM2
	LCD->WF8B[18] = (1<<3)|(1<<7);	//COM3
	
	// Enable LCD and turn off safe mode
	LCD->GCR &= ~LCD_GCR_PADSAFE_MASK; 
	LCD->GCR |= LCD_GCR_LCDEN_MASK; 
	return 0;
}

// Macros for LCD segments
const volatile uint8_t ABC[10] = {0xEE,0x66,0xCC,0xEE,0x66,0xAA,0xAA,0xEE,0xEE,0xEE}; // A, B, C segments decoding marcos for 0:9 respectively 
const volatile uint8_t DEFG[10]= {0xBB,0x00,0x77,0x55,0xCC,0xDD,0xFF,0x00,0xFF,0xDD}; // D, E, F, G segments decoding marcos for 0:9 respectively 
const volatile uint8_t digit_ABC[4] = {11,38,8,17}; //WF8B indexes to refer to  the pin controlling ./:, C, B, A segments in digits consecutively
const volatile uint8_t digit_DEFG[4]= {10,53,7,37}; //WF8B indexes to refer to  the pin controlling D, E, G, F segments in digits consecutively

const volatile uint8_t FALL_ABC[3] = {0x88, 0xEE, 0x00}; // A, B, C segments decoding marcos for F, A, L respectively
const volatile uint8_t FALL_DEFG[3] = {0xEE, 0xEE, 0xBB}; // D, E, F, G segments decoding marcos for F, A, L respectively
	
uint8_t sLCD_Print(uint8_t digit, uint8_t number)
{
	// Check for valid inputs
	if(digit>3 && number>9) 
		return 1u; 
	else if(number>9) 
		return 2u; 
	else if(digit>3) 
		return 3u; 

	// Saving dots state
	int8_t ifdot = LCD->WF8B[digit_ABC[digit]]&((1<<0)|(1<<4)); 
	// Set decoded segment to the corresponding frontplane pins
	LCD->WF8B[digit_ABC[digit]] =ABC[number];  
	LCD->WF8B[digit_DEFG[digit]]=DEFG[number];
	// Restore dots state
	LCD->WF8B[digit_ABC[digit]] |=ifdot;
	return 0u; 	
}

uint8_t sLCD_DotSet(uint8_t dot)
{
	// Check for valid inputs
	if(dot>3) return 1;

	// Set the dot segment
	LCD->WF8B[digit_ABC[dot]] |= 0x11; //0x11 is the mask for the dot segment.
	return 0;
}

uint8_t sLCD_DotClear(uint8_t dot)
{	
	// Check for valid inputs
	if(dot>3) return 1; 

	// Clear the dot segment
	LCD->WF8B[digit_ABC[dot]]&=~0x11;
	return 0;
}


void sLCD_Clear(void)
{
	for(int i=0;i<4;i++)
	{
		//Clear frontplane pins output waveform.
		LCD->WF8B[digit_ABC[i]]=0x0;
		LCD->WF8B[digit_DEFG[i]]=0x0;
	}
}

void sLCD_Fall_Message_Print(){
	sLCD_Clear();
	// Set the segments to display "FALL" message
	LCD->WF8B[digit_ABC[3]] = FALL_ABC[0]; // F
	LCD->WF8B[digit_DEFG[3]] = FALL_DEFG[0];

	LCD->WF8B[digit_ABC[2]] = FALL_ABC[1]; // A
	LCD->WF8B[digit_DEFG[2]] = FALL_DEFG[1];
	
	LCD->WF8B[digit_ABC[1]] = FALL_ABC[2]; // L
	LCD->WF8B[digit_DEFG[1]] = FALL_DEFG[2];
	
	LCD->WF8B[digit_ABC[0]] = FALL_ABC[2]; // L
	LCD->WF8B[digit_DEFG[0]] = FALL_DEFG[2];
}
