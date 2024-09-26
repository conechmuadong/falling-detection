/**
 * @file i2c.c
 * @author Nguyen Duy Hung, Vu Thu Huyen
 * @date June 6th 2024
 * @brief I2C driver
 * This file contains the definition of functions for I2C communication.
*/
#include "i2c.h"

void I2C_Init(){
	// Choose Clock divider for I2C, 24MHz/3 = 8MHz
	SIM->CLKDIV1 |= (1<<17)|(1<<16);
	// Enable I2C0 clock
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	// Enable PORT E clock
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	// Set alternate function for I2C0 SCL and SDA
	PORTE->PCR[24] |= PORT_PCR_MUX(5);
	PORTE->PCR[25] |= PORT_PCR_MUX(5);
	
	// Set I2C baud rate, I2C baud rate = bus clock / (mul * SCL divider)
	I2C0->F = 0x1F;
	
	// Enable I2C module
	I2C0->C1 = 0x80;
}

error_code I2C_Transmit(uint8_t address, uint8_t reg, uint8_t data){
	// Generate start signal
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->C1 |= I2C_C1_TX_MASK;
	// Send address
	I2C0->D = address << 1;
	// Wait for interrupt flag
	while((I2C0->S & I2C_S_IICIF_MASK)==0);
	I2C0->S |= I2C_S_IICIF_MASK;
	// Check if slave acknowledge
	if (!(I2C0->S & I2C_S_RXAK_MASK)){
	}
	// Send register address
	I2C0->D = reg;
	// Wait for interrupt flag
	while((I2C0->S & I2C_S_IICIF_MASK)==0);
	I2C0->S |= I2C_S_IICIF_MASK;
	// Check if slave acknowledge
	if (!(I2C0->S & I2C_S_RXAK_MASK)){
	}
	// Send data
	I2C0->D = data;
	// Wait for interrupt flag
	while((I2C0->S & I2C_S_IICIF_MASK)==0);
	I2C0->S |= I2C_S_IICIF_MASK;
	// Check if slave acknowledge
	if (!(I2C0->S & I2C_S_RXAK_MASK)){
	}
	// Generate stop signal
	I2C0->C1 &= ~I2C_C1_MST_MASK;
	I2C0->C1 &= ~I2C_C1_TX_MASK;
	
	// Pause for a while, to make sure the slave has enough time to process the data
	for(int i=0; i<1000; i++);
	
	return I2C_OK;
}
error_code I2C_Receive(uint8_t address, uint8_t reg, uint8_t * data){
	// Generate start signal
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->C1 |= I2C_C1_TX_MASK;

	// Send address
	I2C0->D = address << 1;
	
	while((I2C0->S & I2C_S_IICIF_MASK)==0); //Wait for interrupt flag
	I2C0->S |= I2C_S_IICIF_MASK;
	
	if (!(I2C0->S & I2C_S_RXAK_MASK)){
	}
	// Send register address
	I2C0->D = reg;

	while((I2C0->S & I2C_S_IICIF_MASK)==0); //Wait for interrupt flag
	I2C0->S |= I2C_S_IICIF_MASK;  

	// Check if slave acknowledge
	if (!(I2C0->S & I2C_S_RXAK_MASK)){
	}

	// Generate repeated start signal
	I2C0->C1 |= 0x04;
	// Send address with read bit
	I2C0->D = address<<1|1;
	
	// Wait for interrupt flag
	while((I2C0->S & I2C_S_IICIF_MASK)==0);
	I2C0->S |= I2C_S_IICIF_MASK;
	
	// Check if slave acknowledge
	if (!(I2C0->S & I2C_S_RXAK_MASK)){
	}
	
	// Change to receive mode
	I2C0->C1 &= ~I2C_C1_TX_MASK;
	I2C0->C1 |= I2C_C1_TXAK_MASK;

	// Dummy read
	*data = I2C0->D;

	// Wait for interrupt flag
	while((I2C0->S & I2C_S_IICIF_MASK)==0);
	I2C0->S |= I2C_S_IICIF_MASK;

	// Check if slave acknowledge
	if (!(I2C0->S & I2C_S_RXAK_MASK)){
	}

	// Generate stop signal
	I2C0->C1 &= ~I2C_C1_MST_MASK;
	I2C0->C1 &= ~I2C_C1_TX_MASK;
	// Read data
	*data = I2C0->D;
	
	for(int i=0; i<1000; i++); // Pause for a while, to make sure the slave has enough time to process the data
	
	return I2C_OK;
}
