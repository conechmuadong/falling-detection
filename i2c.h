/**
 * @file i2c.h
 * @author Nguyen Duy Hung, Vu Thu Huyen
 * @date June 6th 2024
 * @brief I2C driver	
 * This file contains the declaration of functions for I2C communication.
*/

#include "MKL46Z4.h"


// I2C error codes
typedef enum {
	I2C_ERROR_TRANSMIT,
	I2C_ERROR_RECEIVE,
	I2C_TIMEOUT,
	I2C_OK
} error_code;

#define MMA8451_I2C_ADDRESS 0x1d

/**
 * @brief Initialize I2C module
 * This function initializes I2C module for communication with peripherals.
 * @param void
 * @return none
*/
void I2C_Init();

/**
 * @brief Transmit data via I2C
 * This function performs transmition data to a peripheral register via I2C.
 * @param address: address of the peripheral
 * @param reg: register address
 * @param data: data to be transmitted
 * @return error_code
*/
error_code I2C_Transmit(uint8_t address, uint8_t reg, uint8_t data);

/**
 * @brief Receive data via I2C
 * This function performs receiving data from a peripheral register via I2C.
 * @param address: address of the peripheral
 * @param reg: register address
 * @param data: pointer to the data to be received
 * @return error_code
*/
error_code I2C_Receive(uint8_t address, uint8_t reg, uint8_t * data);
