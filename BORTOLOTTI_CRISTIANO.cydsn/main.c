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


#define STATUS_REG 0x27
#define CTRL_REG4_SET_HR_AND_BDU 0x88 //set BDU e HR

#define CTRL_REG1 0x20
#define CTRL_REG4 0x23

#define A_X_L 0x28
#define A_X_H 0x29
#define A_Y_L 0x2A
#define A_Y_H 0x2B
#define A_Z_L 0x2C
#define A_Z_H 0x2D

//packet parameters
#define BYTE_TO_SEND 6
#define BUFFER_SIZE 1+BYTE_TO_SEND+1

#define M_digit_TO_MS2      2*9.81/2047

#define TRUE 1
#define FALSE 0

int main(void)
{
    UART_Start();
    PWM_Start();
    I2C_Start();
    EEPROM_Start();
    EEPROM_UpdateTemperature();
    CyDelay(5);
    
    //char message[50] = {'\0'};
    count = 0;
    Button_flag = 0;
    
    ErrorCode error;
    uint8_t DataBuffer [BUFFER_SIZE]; //Packet statement
    
     //packet header and tail setting
    DataBuffer [0] = 0xA0;
    DataBuffer [BUFFER_SIZE-1] = 0xC0;   
    
    uint8_t Acc_X[2];
    uint8_t Acc_Y[2];
    uint8_t Acc_Z[2];
    int16 A_X;
    int16 A_Y;
    int16 A_Z;
    float A_X_Conv;
    float A_Y_Conv;
    float A_Z_Conv;
    uint8_t status_reg;
    uint8_t freq;

    error = I2C_WriteRegister(DEVICE_ADDRESS, CTRL_REG4, CTRL_REG4_SET_HR_AND_BDU);
    // set frequency
    freq = EEPROM_ReadByte(FREQ_MEMORY_ADDRESS);
    switch(freq)
    {
        case FREQ_1:
            Set_PWM(PERIOD_FREQ_1);
            break;
        case FREQ_10:
            Set_PWM(PERIOD_FREQ_10);
            break;
        case FREQ_25:
            Set_PWM(PERIOD_FREQ_25);
            break;
        case FREQ_50:
            Set_PWM(PERIOD_FREQ_50);
            break;
        case FREQ_100:
            Set_PWM(PERIOD_FREQ_100);
            break;
        case FREQ_200:
            Set_PWM(PERIOD_FREQ_200);
            break;
        default:
            EEPROM_UpdateTemperature();
            EEPROM_WriteByte(FREQ_1,FREQ_MEMORY_ADDRESS);
            Set_PWM(PERIOD_FREQ_1); 
            freq = EEPROM_ReadByte(FREQ_MEMORY_ADDRESS);
            break;   
    }
   
    
    error = I2C_WriteRegister(DEVICE_ADDRESS, CTRL_REG1, freq); //set frequency from memory
   
    CyGlobalIntEnable; 
    isr_Button_StartEx(Custom_ISR_Button);
    
    
    
    
    while(1)
    {
        if (Button_flag == FALSE)
        {
            error = I2C_ReadRegister(DEVICE_ADDRESS, STATUS_REG, &status_reg);
            
            if ( status_reg & 0x08)
            {
                error = I2C_ReadRegister(DEVICE_ADDRESS, A_X_L, &Acc_X[0]);
                if (error == NO_ERROR)
                {
                    error = I2C_ReadRegister(DEVICE_ADDRESS, A_X_H, &Acc_X[1]);
                    if (error == NO_ERROR)
                    {
                        error = I2C_ReadRegister(DEVICE_ADDRESS, A_Y_L, &Acc_Y[0]);
                        if (error == NO_ERROR)
                        {
                            error = I2C_ReadRegister(DEVICE_ADDRESS, A_Y_H, &Acc_Y[1]);
                            if (error == NO_ERROR)
                            {
                                error = I2C_ReadRegister(DEVICE_ADDRESS, A_Z_L, &Acc_Z[0]);
                                if (error == NO_ERROR)
                                {
                                    error = I2C_ReadRegister(DEVICE_ADDRESS, A_Z_H, &Acc_Z[1]);
                                }
                            }
                        }
                    }
                }
                
                if (error == NO_ERROR)
                {
                        A_X = (int16)((Acc_X[0] | (Acc_X[1]<<8)))>>4;
                        A_Y = (int16)((Acc_Y[0] | (Acc_Y[1]<<8)))>>4;
                        A_Z = (int16)((Acc_Z[0] | (Acc_Z[1]<<8)))>>4;

                        A_X_Conv = (A_X*M_digit_TO_MS2)*1000;
                        A_X = (int16)(A_X_Conv);
                    
                        DataBuffer[1] = A_X & 0xFF;
                        DataBuffer[2] = A_X >> 8;

                        A_Y_Conv = (A_Y*M_digit_TO_MS2)*1000;
                        A_Y = (int16)(A_Y_Conv);

                        DataBuffer[3] = A_Y & 0xFF;
                        DataBuffer[4] = A_Y >> 8;
                    
                        A_Z_Conv = (A_Z*M_digit_TO_MS2)*1000;
                        A_Z = (int16)(A_Z_Conv);
                    
                        DataBuffer[5] = (uint8_t)(A_Z & 0xFF);
                        DataBuffer[6] = (uint8_t)(A_Z >> 8);
                    
                    
                        UART_PutArray(DataBuffer,BUFFER_SIZE);
                }
                else 
                {
                    UART_PutString("Error occurred during I2C comm to axis register 1\r\n"); 
                }
            }
        }
        if (Button_flag == TRUE)
        {
            Button_flag = FALSE;
            if (count < 6)
            {
                Set_PWM(PERIOD_FREQ_1 - 2500*count);
                freq = EEPROM_ReadByte(FREQ_MEMORY_ADDRESS) + 0x10;
                EEPROM_UpdateTemperature();
                EEPROM_WriteByte(freq, FREQ_MEMORY_ADDRESS);
                error = I2C_WriteRegister(DEVICE_ADDRESS, CTRL_REG1, freq);
                
                
            }
            else
            {
                count = 0;
                EEPROM_UpdateTemperature();
                EEPROM_WriteByte(FREQ_1, FREQ_MEMORY_ADDRESS);
                error = I2C_WriteRegister(DEVICE_ADDRESS, CTRL_REG1, FREQ_1);
                Set_PWM(PERIOD_FREQ_1);
            }
            
        }

    }  
}

/* [] END OF FILE */
