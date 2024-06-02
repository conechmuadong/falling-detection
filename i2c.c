#include "i2c.h"

void I2C_Init(){
	SIM->CLKDIV1 |= (1<<17)|(1<<16);
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[24] |= PORT_PCR_MUX(5);
	PORTE->PCR[25] |= PORT_PCR_MUX(5);
	
	I2C0->F = 0x1F;
	
	I2C0->C1 = 0x80;
	//I2C0->C1 |= I2C_C1_MST_MASK;
}

error_code I2C_Transmit(uint8_t address, uint8_t reg, uint8_t data){
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->C1 |= I2C_C1_TX_MASK;
	I2C0->D = address << 1;
	while((I2C0->S & I2C_S_IICIF_MASK)==0);
	I2C0->S |= I2C_S_IICIF_MASK;
	if (!(I2C0->S & I2C_S_RXAK_MASK)){
	}
	I2C0->D = reg;
	while((I2C0->S & I2C_S_IICIF_MASK)==0);
	I2C0->S |= I2C_S_IICIF_MASK;
	if (!(I2C0->S & I2C_S_RXAK_MASK)){
	}
	I2C0->D = data;
	while((I2C0->S & I2C_S_IICIF_MASK)==0);
	I2C0->S |= I2C_S_IICIF_MASK;
	if (!(I2C0->S & I2C_S_RXAK_MASK)){
	}
	I2C0->C1 &= ~I2C_C1_MST_MASK;
	I2C0->C1 &= ~I2C_C1_TX_MASK;
	return I2C_OK;
}
error_code I2C_Receive(uint8_t address, uint8_t reg, uint8_t * data){
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->C1 |= I2C_C1_TX_MASK;
	I2C0->D = address << 1;
	while((I2C0->S & I2C_S_IICIF_MASK)==0);
	I2C0->S |= I2C_S_IICIF_MASK;
	if (!(I2C0->S & I2C_S_RXAK_MASK)){
	}
	I2C0->D = reg;
	while((I2C0->S & I2C_S_IICIF_MASK)==0);
	I2C0->S |= I2C_S_IICIF_MASK;
	if (!(I2C0->S & I2C_S_RXAK_MASK)){
	}
	I2C0->C1 |= 0x04;
	I2C0->D = address<<1|1;
	//wait
	while((I2C0->S & I2C_S_IICIF_MASK)==0);
	I2C0->S |= I2C_S_IICIF_MASK;
	
	if (!(I2C0->S & I2C_S_RXAK_MASK)){
	}
	
	I2C0->C1 &= ~I2C_C1_TX_MASK;
	I2C0->C1 |= I2C_C1_TXAK_MASK;
	*data = I2C0->D;
	while((I2C0->S & I2C_S_IICIF_MASK)==0);
	I2C0->S |= I2C_S_IICIF_MASK;
	if (!(I2C0->S & I2C_S_RXAK_MASK)){
	}
	I2C0->C1 &= ~I2C_C1_MST_MASK;
	I2C0->C1 &= ~I2C_C1_TX_MASK;
	*data = I2C0->D;
	
	return I2C_OK;
}
