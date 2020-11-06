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

#include "MyISR.h"
#include "MyFunction.h"
#include "stdio.h"

#define DEVICE_ADDRESS 0x18
#define STATUS_REG 0x27
#define CTRL_REG4_SET_HR 0x08

#define CTRL_REG1 0x20
#define CTRL_REG4 0x23

#define LIS3DH_OUT_X_L 0x28
#define LIS3DH_OUT_X_H 0x29
#define LIS3DH_OUT_Y_L 0x2A
#define LIS3DH_OUT_Y_H 0x2B
#define LIS3DH_OUT_Z_L 0x2C
#define LIS3DH_OUT_Z_H 0x2D

#define FREQ_INIT 0x57


int main(void)
{
//    CyGlobalIntEnable; 
//    isr_Button_StartEx(Custom_ISR_Button);
    UART_Start();
    //PWM_Start();
    I2C_Start();
    char message[50] = {'\0'};
    
    CyDelay(5);

    uint8_t ctrl_reg4=0; 
    ErrorCode error = I2C_ReadRegister(DEVICE_ADDRESS,
                                       CTRL_REG4,
                                       &ctrl_reg4);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4: 0x%02X\r\n", ctrl_reg4);
        UART_PutString(message); 
    }
    else
    {
        UART_PutString("Error occurred during I2C comm to read control register 1\r\n");   
    }
    
    if (ctrl_reg4 != CTRL_REG4_SET_HR)
    {
        UART_PutString("\r\nWriting new values..\r\n");
        ctrl_reg4 = CTRL_REG4_SET_HR;
    
        error = I2C_WriteRegister(DEVICE_ADDRESS,
                                  CTRL_REG4,
                                  ctrl_reg4);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 4 successfully written as: 0x%02X\r\n", ctrl_reg4);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    }
    
    // set frequency
     error = I2C_WriteRegister(DEVICE_ADDRESS,
                               CTRL_REG1,
                               FREQ_INIT);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", FREQ_INIT);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    
    
    
    
    while(1)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
