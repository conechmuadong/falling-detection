# KL46Z Falling Detection Wearable Device
## Introduction
This repository contains source code for a project on FRDM KL46Z Board to make it a Wearable Device capable of detecting and warning if the person wearing it falls. It is our final assignment for the Introduction to Embedded Systems course at VNU-UET in the 2024 Spring Semester.

## Author
- Nguyen Duy Hung, 21020436, VNU-UET Computer Engineering Student, Class of 2025.
- Vu Thu Huyen, 21020441, VNU-UET Computer Engineering Student, Class of 2025.

## Requirements
- Keil uVision5 IDE
- ARM Compiler V5.06

## Functional Description
When the device is normally operating, it displays the time preset by the user on the Segment LCD. The device captures MMA8451 accelerometer data and decides whether the user is falling. If the user falls, the device displays a warning by showing the message: "FALL" on the sLCD and blinking the red and green LED. To dismiss the warning, the user needs to press the button on the device.
