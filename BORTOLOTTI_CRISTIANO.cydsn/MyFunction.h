/* ========================================
 *
 * Author: Cristiano Bortolotti
 * File: MyFunction.h
 * Date: 04/11/20 - 14/11/20
 *
 * ========================================
*/

#ifndef __ERRORCODES_H
    #define __ERRORCODES_H
    
    #include "Defines.h"
    CY_ISR_PROTO (Custom_ISR_Button);
    
    //global variable statement
    uint8 count;
    uint8 Button_flag;
    uint8 Error_flag;
    uint8 Setting_flag;
    
    //Packet statement
    uint8_t DataBuffer [BUFFER_SIZE]; 
    
    //functions declaration
    ErrorCode I2C_ReadRegister(uint8_t device_address, uint8_t register_address, uint8_t* data);
    
    ErrorCode I2C_WriteRegister(uint8_t device_address, uint8_t register_address, uint8_t data);
    
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
