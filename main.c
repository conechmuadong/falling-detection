#include "MKL46Z4.h"
#include "i2c.h"
#include "timer.h"
#include "LCD.h"
#include "GPIO.h"
#include <stdio.h>
#include <string.h>

typedef enum {
	LCD_NORMAL,
	LCD_CHANGE_MINUTE,
	LCD_CHANGE_HOUR
} LCD_Stage;

LCD_Stage stage = LCD_NORMAL;
uint8_t second;
uint8_t minute;
uint8_t hour;
uint8_t dot = 0;
uint8_t isFell = 0;

void show_current_time(){
	sLCD_Print(0,minute%10);
	sLCD_Print(1,minute/10);
	sLCD_Print(2,hour%10);
	sLCD_Print(3,hour/10);
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

int main(){
	I2C_Init();
	
	Init_Button();
	Init_FreeFall_IRQ_Input();
	//InitUART();
	//for (int i = 0; i<10000; i++);
	
	accelerometer_init();
	
	//for (int i = 0; i<10000; i++);
	sLCD_Init();
	Timer_Init();
	show_current_time();
	
	while (1){
			show_current_time();
			while(isFell){
				sLCD_Fall_Message_Print();
				for (int i = 0; i<3000000; i++);
				sLCD_Clear();
				for (int i = 0; i<3000000; i++);
				//TODO: Add more Warning Function
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
	else if (PIT->CHANNEL[1].TFLG & (uint32_t)1u){
		if (dot == 1){
			sLCD_DotSet(2);
			dot = 0;
		}
		else{
			sLCD_DotClear(2);
			dot = 1;
		}
		PIT->CHANNEL[1].TFLG |= 1;
	}
}

void PORTC_PORTD_IRQHandler(void){
	if (PORTC->PCR[3] & PORT_PCR_ISF_MASK){
		if (isFell == 1){
			isFell = 0;
			return;
		}
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
		}
		PORTC->PCR[3]|=PORT_PCR_ISF_MASK;
	}
	else if (PORTC->PCR[12] & PORT_PCR_ISF_MASK){
		switch (stage){
			case LCD_NORMAL:
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
		}
		PORTC->PCR[12] |= PORT_PCR_ISF_MASK;
	}
	else if (PORTC->PCR[5]&PORT_PCR_ISF_MASK){
		isFell = 1;
		uint8_t temp;
		I2C_Receive(MMA8451_I2C_ADDRESS, 0x16, &temp);
		PORTC->PCR[5] |= PORT_PCR_ISF_MASK;
	}
}