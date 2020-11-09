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

#ifndef DEFINES_H
    #define DEFINES_H
    
    #include "project.h"
    
    #define TRUE 1
    #define FALSE 0
    
    #define DEVICE_ADDRESS 0x18 
    
    //registers address
    #define CTRL_REG1 0x20
    #define CTRL_REG4 0x23
    #define STATUS_REG 0x27
    #define A_X_L 0x28
    #define A_X_H 0x29
    #define A_Y_L 0x2A
    #define A_Y_H 0x2B
    #define A_Z_L 0x2C
    #define A_Z_H 0x2D
    
    //setting mask for registers
    #define CTRL_REG4_SET_HR_AND_BDU 0x88 
    #define STATUS_REG_SET_ZYXDA 0x08 
    #define FREQ_1    0x17
    #define FREQ_10   0x27
    #define FREQ_25   0x37
    #define FREQ_50   0x47
    #define FREQ_100  0x57
    #define FREQ_200  0x67
    
    //packet parameters
    #define BYTE_TO_SEND 6
    #define BUFFER_SIZE 1+BYTE_TO_SEND+1

    //Calibration line coefficient
    #define M_digit_TO_MS2      2*9.81/2047
    
    //EEPROM address where the frequency is saved
    #define FREQ_MEMORY_ADDRESS 0x0000
    
    // set of define for PWM setting as described in the TopDesign
    #define PERIOD_FREQ_1 15000
    #define PERIOD_FREQ_10 12500
    #define PERIOD_FREQ_25 10000
    #define PERIOD_FREQ_50 7500
    #define PERIOD_FREQ_100 5000
    #define PERIOD_FREQ_200 2500
    
    typedef enum { // enum variable used to define erorrs
        NO_ERROR,           
        ERROR               
    } ErrorCode;

    typedef struct { // struct of array used to save data read from axis registers
        uint8 X[2];
        uint8_t Y[2];
        uint8_t Z[2];
    } A_array;

    typedef struct { // struct of int variable used for X,Y,Z data
        int16 X;
        int16 Y;
        int16 Z;
    } A_int;
    
    typedef struct { // struct of float variable used as support to perform elaboration
        float X;
        float Y;
        float Z;
    } A_float;
    
#endif



/* [] END OF FILE */
