#include "MKL46Z4.h"

typedef enum {
	I2C_ERROR_TRANSMIT,
	I2C_ERROR_RECEIVE,
	I2C_TIMEOUT,
	I2C_OK
} error_code;

#define MMA8451_I2C_ADDRESS 0x1d

void I2C_Init();
error_code I2C_Transmit(uint8_t address, uint8_t reg, uint8_t data);
error_code I2C_Receive(uint8_t address, uint8_t reg, uint8_t * data);
