/**
 * @file LCD.h
 * @date May 31st 2024
 * @brief LCD driver
 * This file contains the declaration of functions for LCD display.
*/
#include "MKL46Z4.h"

/**
 * @brief Initialize LCD module
 * This function initializes LCD module.
 * @param void
 * @return uint32_t - error code
*/
uint32_t sLCD_Init(void);

/**
 * @brief Print a number on LCD
 * This function prints a number on specific digit on the sLCD.
 * @param number: number to be printed
 * @param digit: digit to be printed on (0 to 3)
 * @return uint8_t - error code
*/
uint8_t sLCD_Print(uint8_t digit, uint8_t number); // single digit print (0 to F) digits are numerated 3 down to 0

/**
 * @brief Set dots on LCD
 * This function sets dot segment of specific digit on the sLCD.
 * @param dot: the digit to be set the dot (0 to 3)
 * @return uint8_t - error code
*/
uint8_t sLCD_DotSet(uint8_t dot); // set dots (0 is colon, 1 is dot on 1st dec point etc)

/**
 * @brief Clear dots on LCD
 * This function clears dot segment of specific digit on the sLCD.
 * @param dot: the digit to be cleared the dot (0 to 3)
 * @return uint8_t - error code
*/
uint8_t sLCD_DotClear(uint8_t dot); //clear dots and colon

/**
 * @brief Clear LCD
 * This function clears all LCD segments.
 * @param void
 * @return void
*/
void sLCD_Clear(void); //clear all LCD (also dots and colon)

/**
 * @brief Print a message on LCD
 * This function prints "FALL" message on the sLCD. This provide a warning message if the user falls.
 * @param void
 * @return void
*/
void sLCD_Fall_Message_Print();