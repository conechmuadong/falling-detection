/**
 * @file GPIO.h
 * @brief GPIO Header File
 * This file contains the declaration of functions for GPIO peripherals including switch button, 
 * LED and Freefall INT1 signal from MMA8451Q.
*/
#include "MKL46Z4.h"

/**
 * @brief Initialize switch button
 * This function initializes switch button SW1 at PTC3 and SW2 at PTC12 as GPIO interrupt input.
 * @param void
 * @return none
*/
void Init_Button(void);

/**
 * @brief Initialize LED
 * This function initializes LED1 at PTD5 and LED2 at PTE29 as GPIO output.
 * @param void
 * @return none
*/
void Init_LED(void);

/**
 * @brief Initialize Freefall INT1 signal
 * This function initializes Freefall INT1 signal at PTC5 as GPIO interrupt input.
 * @param void
 * @return none
*/
void Init_FreeFall_IRQ(void);