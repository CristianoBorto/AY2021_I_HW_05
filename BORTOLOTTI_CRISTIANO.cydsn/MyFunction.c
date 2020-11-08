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

#include "MyFunction.h"
     
ErrorCode I2C_ReadRegister(uint8_t device_address, uint8_t register_address, uint8_t* data)
    {
        // Send start condition
        uint8_t error = I2C_MasterSendStart(device_address,I2C_WRITE_XFER_MODE);
        if (error == I2C_MSTR_NO_ERROR)
        {
            // Write address of register to be read
            error = I2C_MasterWriteByte(register_address);
            if (error == I2C_MSTR_NO_ERROR)
            {
                // Send restart condition
                error = I2C_MasterSendRestart(device_address, I2C_READ_XFER_MODE);
                if (error == I2C_MSTR_NO_ERROR)
                {
                    // Read data without acknowledgement
                    *data = I2C_MasterReadByte(I2C_NAK_DATA);
                }
            }
        }
        // Send stop condition if something went wrong
        I2C_MasterSendStop();
        // Return error code
        return error ? ERROR : NO_ERROR;
    }

ErrorCode I2C_WriteRegister(uint8_t device_address,
                                       uint8_t register_address,
                                       uint8_t data)
    {
        // Start condition
        uint8_t error = I2C_MasterSendStart(device_address,I2C_WRITE_XFER_MODE);
        if (error == I2C_MSTR_NO_ERROR)
        {
            // write address of register to overwrite
            error = I2C_MasterWriteByte(register_address);
            if (error == I2C_MSTR_NO_ERROR)
            {
                // write byte
                error = I2C_MasterWriteByte(data);
            }
        }
        // close comm
        I2C_MasterSendStop();
        //
        return error ? ERROR : NO_ERROR;
    }

void Set_PWM(uint16 period)
{
    PWM_WritePeriod(period);
    PWM_WriteCompare(period/2);
    
}
    
    
    
    
    
/* [] END OF FILE */
