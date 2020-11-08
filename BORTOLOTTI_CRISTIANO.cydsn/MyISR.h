/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef INTERRUPT_ROUTINES_H
    #define INTERRUPT_ROUTINES_H
    
    #include "project.h"
    
    #define DEVICE_ADDRESS 0x18
    #define CTRL_REG1 0x20
    #define FREQ_MEMORY_ADDRESS 0x0000
    #define FREQ_1    0x17
    #define FREQ_10   0x27
    #define FREQ_25   0x37
    #define FREQ_50   0x47
    #define FREQ_100  0x57
    #define FREQ_200  0x67
    
    // set of define for PWM setting
    #define PERIOD_FREQ_1 15000
    #define PERIOD_FREQ_10 12500
    #define PERIOD_FREQ_25 10000
    #define PERIOD_FREQ_50 7500
    #define PERIOD_FREQ_100 5000
    #define PERIOD_FREQ_200 2500
    
    #define STATUS_REG 0x27
    
    CY_ISR_PROTO (Custom_ISR_Button);
    
    uint8 count;
    uint8 Button_flag;
    
    #endif

/* [] END OF FILE */
