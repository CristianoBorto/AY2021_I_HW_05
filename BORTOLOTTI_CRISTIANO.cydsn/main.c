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

int main(void)
{
    Start_Peripherals(); //UART, PWM, I2C and EEPROM start
    
    //flag reset
    Error_flag = FALSE;
    Button_flag = FALSE;
    Setting_flag = FALSE;
    
    //packet header and tail setting
    DataBuffer [0] = 0xA0;
    DataBuffer [BUFFER_SIZE-1] = 0xC0;   
    
    ErrorCode error;
    A_array Acc; //support variable to save data read from data registers
    A_int Output;
    uint8_t status_reg; //support variable to save the content of the status register
    uint8_t freq;

    //set ctrl_reg4 --> High Resolution + Block Data Update
    Set_Ctrl_Reg4();
    // set frequency
    freq = freq_from_memory(); //get frequency from memory
    Set_Ctrl_Reg1(freq);
    // interrupt setting
    Start_Interrupt();
    
    while(1)
    {
        if (Setting_flag == FALSE) //if true the setting is correctly occured
        {
            if (Button_flag == FALSE) //button not pressed
            {
                error = I2C_ReadRegister(DEVICE_ADDRESS, STATUS_REG, &status_reg);
                if ((status_reg & STATUS_REG_SET_ZYXDA) && error == NO_ERROR) //new data on the 3 axis are available
                {
                    Acc = XYZ_Reading();
                    if (Error_flag == FALSE) //no errors occured during reading
                    {
                        Output = XYZ_Elaboration(Acc);
                        XYZ_Sending(Output);
                    }
                    else 
                    { 
                        Error_flag = FALSE; //reset of flag whether the reading went wrong
                    }
                }
            }
            if (Button_flag == TRUE)
            {
                Button_flag = FALSE;
                if (count < 6)
                {
                    Freq_1_to_200(freq); //change frequency 1 --> 10 --> 25 --> 50 --> 100 --> 200
                }
                else
                {
                    Freq_200_to_1(); //change frequency 200 --> 1
                }
            }
        } //end if setting_flag
    } //end while(1)  
} //end main

/* [] END OF FILE */
