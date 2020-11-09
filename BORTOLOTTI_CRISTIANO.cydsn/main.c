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

#define TRUE 1
#define FALSE 0

int main(void)
{
    Start_Peripherals(); //UART, PWM, I2C and EEPROM start
    
    flag_error = FALSE;
    Button_flag = FALSE;
    
    //packet header and tail setting
    DataBuffer [0] = 0xA0;
    DataBuffer [BUFFER_SIZE-1] = 0xC0;   
    
    ErrorCode error;
    A_array Acc;
    A_int Output;
    uint8_t status_reg;
    uint8_t freq;

    //set ctrl_reg4 --> High Resolution + Block Data Update
    error = I2C_WriteRegister(DEVICE_ADDRESS, CTRL_REG4, CTRL_REG4_SET_HR_AND_BDU);
    // set frequency
    freq = freq_from_memory(); //get frequency from memory
    error= I2C_WriteRegister(DEVICE_ADDRESS, CTRL_REG1, freq);
    // interrupt setting
    Start_Interrupt();
    
    while(1)
    {
        if (Button_flag == FALSE)
        {
            error = I2C_ReadRegister(DEVICE_ADDRESS, STATUS_REG, &status_reg);
            if (status_reg & STATUS_REG_SET_ZYXDA)
            {
                Acc = XYZ_Reading();
                if (flag_error == FALSE)
                {
                    Output = XYZ_Elaboration(Acc);
                    XYZ_Sending(Output);
                }
                else 
                {
                    UART_PutString("Failed output readings\r\n"); 
                    flag_error = FALSE;
                }
            }
        }
        if (Button_flag == TRUE)
        {
            Button_flag = FALSE;
            if (count < 6)
            {
                Freq_1_to_200(freq);
            }
            else
            {
                Freq_200_to_1();
            }
        }
    } //end while(1)  
} //end main

/* [] END OF FILE */
