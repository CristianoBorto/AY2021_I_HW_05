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

#ifndef __ERRORCODES_H
    #define __ERRORCODES_H
    
    #include "MyISR.h"
    #include "Defines.h"
    
    typedef enum {
        NO_ERROR,           ///< No error generated
        ERROR               ///< Error generated
    } ErrorCode;

    typedef struct {
        uint8 X[2];
        uint8_t Y[2];
        uint8_t Z[2];
    } A_array;

    typedef struct {
        int16 X;
        int16 Y;
        int16 Z;
    } A_int;
    
    typedef struct {
        float X;
        float Y;
        float Z;
    } A_float;
    
    uint8 flag_error;
    uint8_t DataBuffer [BUFFER_SIZE]; //Packet statement
        
    ErrorCode I2C_ReadRegister(uint8_t device_address, 
                               uint8_t register_address,
                               uint8_t* data);
    ErrorCode I2C_WriteRegister(uint8_t device_address,
                                uint8_t register_address,
                                uint8_t data);
    void Set_PWM(uint16 period);
  
    void Start_Peripherals(void);
    
    void Set_Ctrl_Reg4(void);
    
    void Set_Ctrl_Reg1(uint8_t frequency);
    
    uint8_t freq_from_memory (void);
    
    void Start_Interrupt(void);
    
    A_array XYZ_Reading(void);
    
    A_int XYZ_Elaboration(A_array array);
    
    void XYZ_Sending(A_int value);
    
    void Freq_1_to_200(uint8_t frequency);
    
    void Freq_200_to_1(void);
    
#endif

/* [] END OF FILE */
