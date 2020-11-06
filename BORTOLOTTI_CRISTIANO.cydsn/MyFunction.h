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
    
    #include "project.h"
    
    typedef enum {
        NO_ERROR,           ///< No error generated
        ERROR               ///< Error generated
    } ErrorCode;

    ErrorCode I2C_ReadRegister(uint8_t device_address, 
                               uint8_t register_address,
                               uint8_t* data);
    ErrorCode I2C_WriteRegister(uint8_t device_address,
                                uint8_t register_address,
                                uint8_t data);
  
#endif

/* [] END OF FILE */
