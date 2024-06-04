/**
 * @file timer.h
 * @date June 2th 2024
 * @brief PIT timer setup
 * This file contains the declaration of functions for PIT timer setup
 * for displaying the time on the LCD.
 * 
 */
#include "MKL46Z4.h"

/**
 * @brief Initialize PIT timer
 * This function initializes PIT timer for updating the time on the LCD.
 * @param void
 * @return none
 */
void Timer_Init(void);