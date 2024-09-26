/**
 * @file main.c
 * @brief Main source file
 * This file contains the main code for the project.
 * The project is a digital clock that can be set the time by pressing the button.
 * The clock will display the time in the format of HH:MM.
 * The clock will also have a fall detection feature, when user wearing this device
 * falls, this will display an alert message on sLCD.
 * @author Nguyen Duy Hung, Vu Thu Huyen
 * @date June 10th 2024
*/

#include "MKL46Z4.h"
#include "i2c.h"
#include "timer.h"
#include "LCD.h"
#include "GPIO.h"
#include "systick.h"

typedef enum {
	LCD_NORMAL,
	LCD_FALL,
	LCD_CHANGE_MINUTE,
	LCD_CHANGE_HOUR,
	LCD_SYSTEM_OFF
} LCD_Stage;

LCD_Stage stage = LCD_NORMAL;
uint8_t second;
uint8_t minute;
uint8_t hour;
uint8_t dot = 0;

uint32_t redLed = 0;
uint32_t greenLed = 0;

/**
 * @brief Show current time on sLCD
 * This function shows the current time on sLCD in the format of HH:MM.
 * @param void
 * @return none
*/
void show_current_time();

/**
 * @brief Initialize accelerometer
 * This function initializes the accelerometer MMA8451Q.
 * @param void
 * @return none
*/
void accelerometer_init();

int main(){
	I2C_Init();
	
	Init_Button();
	Init_FreeFall_IRQ();
	
	accelerometer_init();
	
	sLCD_Init();
	Timer_Init();
	Systick_Init();
	
	Init_LED();
	show_current_time();
	
	while (1){
			if (stage != LCD_SYSTEM_OFF){
				show_current_time();
			}
			else{
				sLCD_Clear();
				PTD->PDOR|= 1<<5;
				PTE->PDOR|= 1<<29;
			}
			while(stage == LCD_FALL){
				PTD->PDOR |= 1<<5;
				sLCD_Fall_Message_Print();
				for (int i = 0; i<3000000; i++);
				sLCD_Clear();
				for (int i = 0; i<3000000; i++);
			}
	}
}
void PIT_IRQHandler(){
	if (PIT->CHANNEL[0].TFLG & (uint32_t)1u){
		if (++second == 60){
			if (++minute == 60){
				if(++hour == 24){
					hour = 0;
				}
				minute = 0;
			}
			second = 0;
		}
		PIT->CHANNEL[0].TFLG |= 1;
	}		
	else if ((PIT->CHANNEL[1].TFLG & (uint32_t)1u)){
		if(stage != LCD_FALL && stage != LCD_SYSTEM_OFF){
			if (dot == 1){
				sLCD_DotSet(0);
				dot = 0;
			}
			else{
				sLCD_DotClear(0);
				dot = 1;
			}	
		}
		PIT->CHANNEL[1].TFLG |= 1;
	}
}

void PORTC_PORTD_IRQHandler(void){
	if (PORTC->PCR[12] & PORT_PCR_ISF_MASK){
		switch(stage){
			case LCD_NORMAL:
				stage = LCD_CHANGE_MINUTE;
				break;
			case LCD_CHANGE_MINUTE:
				stage = LCD_CHANGE_HOUR;
				break;
			case LCD_CHANGE_HOUR:
				stage = LCD_NORMAL;
				break;
			case LCD_SYSTEM_OFF:
				break;
			case LCD_FALL:
				stage = LCD_NORMAL;
				PTE->PDOR |= (1<<29);
				break;
		}
		PORTC->PCR[12]|=PORT_PCR_ISF_MASK;
	}
	else if (PORTC->PCR[3] & PORT_PCR_ISF_MASK){
		switch (stage){
			case LCD_NORMAL:
				stage = LCD_SYSTEM_OFF;
				SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
				PTD->PDOR |= (1<<5);
				sLCD_Clear();
				break;
			case LCD_CHANGE_MINUTE:
				if (++minute == 60){
					if(++hour == 24){
						hour = 0;
					}
					minute = 0;
				}
				break;
			case LCD_CHANGE_HOUR:
				if(++hour == 24){
						hour = 0;
				}
				break;
			case LCD_SYSTEM_OFF:
				SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
				PTD->PDOR &= ~(1<<5);
				stage = LCD_NORMAL;
				break;
			case LCD_FALL:
				break;
		}
		PORTC->PCR[3] |= PORT_PCR_ISF_MASK;
	}
	else if (PORTC->PCR[5]&PORT_PCR_ISF_MASK){
		if (stage != LCD_SYSTEM_OFF){
			stage = LCD_FALL;
		}
		uint8_t temp;
		I2C_Receive(MMA8451_I2C_ADDRESS, 0x16, &temp);
		PORTC->PCR[5] |= PORT_PCR_ISF_MASK;
	}
}
void SysTick_Handler(void)
{
	if (stage!=LCD_FALL){
		greenLed++;
		if (greenLed == 1001)
		{
			PTD->PTOR |= 1<<5;
			greenLed = 0;
		}
	}
	if (stage == LCD_FALL)
	{
		redLed++;
		if (redLed == 501)
		{
			PTE->PTOR |= 1<<29;
			redLed = 0;
		}
	}
}

void accelerometer_init(){
	uint8_t tmp;
	I2C_Transmit(MMA8451_I2C_ADDRESS, 0x2a, 0x20);
	
	I2C_Transmit(MMA8451_I2C_ADDRESS, 0x15, 0xB8);
	I2C_Transmit(MMA8451_I2C_ADDRESS, 0x17, 0x03);
	I2C_Transmit(MMA8451_I2C_ADDRESS, 0x18, 0x06);
	I2C_Transmit(MMA8451_I2C_ADDRESS, 0x2D, 0x04);
	I2C_Transmit(MMA8451_I2C_ADDRESS, 0x2E, 0x04);
	
	I2C_Receive(MMA8451_I2C_ADDRESS, 0x2a, &tmp);
	I2C_Transmit(MMA8451_I2C_ADDRESS, 0x2a, tmp|0x01);
}

void show_current_time(){
	sLCD_Print(0,minute%10);
	sLCD_Print(1,minute/10);
	sLCD_Print(2,hour%10);
	sLCD_Print(3,hour/10);
}