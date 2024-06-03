#include "MKL46Z4.h"
#include "i2c.h"
#include "timer.h"
#include "LCD.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>

uint8_t second;
uint8_t minute;
uint8_t hour;
uint8_t dot = 0;
uint8_t isStartMeasure = 0;

int16_t accel_x, accel_y, accel_z;
int16_t previous_accel_y;
uint8_t isFell = 0;

int16_t _abs(int16_t number){
	if (number < 0){
		return 0 - number;
	}
	return number;
}

void show_current_time(){
	sLCD_Print(0,minute%10);
	sLCD_Print(1,minute/10);
	sLCD_Print(2,hour%10);
	sLCD_Print(3,hour/10);
}

void fall_check(){
	//TODO: Find more accurate algorithm to detect human falling
	if (_abs(accel_y)<=200){
		isFell = 1;			
	}
}

void accelerometer_init(){
	uint8_t tmp;
	I2C_Receive(MMA8451_I2C_ADDRESS, 0x2a, &tmp);
	I2C_Transmit(MMA8451_I2C_ADDRESS, 0x2a, tmp|0x01);
}

void accelerometer_read(){
	uint8_t data;
	I2C_Receive(MMA8451_I2C_ADDRESS, 0x00, &data);
	if ((data&0xf)!=0){
		I2C_Receive(MMA8451_I2C_ADDRESS, 0x01, &data);
		accel_x = data << 8;
		I2C_Receive(MMA8451_I2C_ADDRESS, 0x02, &data);
		accel_x |= data;
		accel_x >>=2;
		
		if(accel_x & (1u<<13)){
			accel_x |= (1u<<15)|(1u<<14);
		}
		
		I2C_Receive(MMA8451_I2C_ADDRESS, 0x03, &data);
		
		accel_y = data << 8;
		I2C_Receive(MMA8451_I2C_ADDRESS, 0x04, &data);
		
		accel_y |= data;
		accel_y >>=2;
	
		
		if(accel_y & (1u<<13)){
			accel_y |= (1u<<15)|(1u<<14);
		}
		
		I2C_Receive(MMA8451_I2C_ADDRESS, 0x05, &data);
		
		accel_z = data << 8;
		I2C_Receive(MMA8451_I2C_ADDRESS, 0x06, &data);
		
		accel_z |= data;
		accel_z >>=2;
		
		if(accel_z & (1u<<13)){
			accel_z |= (1u<<15)|(1u<<14);
		}
		
	}
}

void send_data(){
	char buffer[64];
	sprintf(buffer, "\r\n %d   %d   %d", accel_x, accel_y, accel_z);
	TransferMsg(buffer, strlen(buffer));
}


int main(){
	I2C_Init();
	
	InitUART();
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
				for (int i = 0; i<30000; i++);
				sLCD_Clear();
				for (int i = 0; i<30000; i++);
				//TODO: Add more Warning Function
			}
			accelerometer_read();
			fall_check();
			send_data();
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
		isFell = 0;
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
		isStartMeasure = 1;
		PIT->CHANNEL[1].TFLG |= 1;
	}
}