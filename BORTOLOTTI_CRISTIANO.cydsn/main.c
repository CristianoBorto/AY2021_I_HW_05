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
#define CTRL_REG4_SET_HR_AND_BDU 0x88 //set BDU e HR

#define CTRL_REG1 0x20
#define CTRL_REG4 0x23

#define LIS3DH_OUT_X_L 0x28
#define LIS3DH_OUT_X_H 0x29
#define LIS3DH_OUT_Y_L 0x2A
#define LIS3DH_OUT_Y_H 0x2B
#define LIS3DH_OUT_Z_L 0x2C
#define LIS3DH_OUT_Z_H 0x2D

// ci sarà poi da fare il controllo con il bottone per la frequenza
#define FREQ_INIT 0x57

//packet parameters
#define BYTE_TO_SEND 6
#define BUFFER_SIZE 1+BYTE_TO_SEND+1

#define M_digit_TO_MS2 2000*9.81/2047
#define Q_digit_To_MS2 M_digit_TO_MS2*4095 

int main(void)
{
    CyGlobalIntEnable; 
//    isr_Button_StartEx(Custom_ISR_Button);
    UART_Start();
    PWM_Start();
    I2C_Start();
    
    char message[50] = {'\0'};
    ErrorCode error;
    uint8_t DataBuffer [BUFFER_SIZE]; //Packet statement
    
    uint8_t Acc_X[2];
    uint8_t Acc_Y[2];
    uint8_t Acc_Z[2];
    int16 A_X;
    int16 A_Y;
    int16 A_Z;
    float A_X_Conv;
    float A_Y_Conv;
    float A_Z_Conv;
    
    
    
    
    //packet header and tail setting
    DataBuffer [0] = 0xA0;
    DataBuffer [BUFFER_SIZE-1] = 0xC0;
    
    
    CyDelay(5);
     
    error = I2C_WriteRegister(DEVICE_ADDRESS, CTRL_REG4, CTRL_REG4_SET_HR_AND_BDU);
    // set frequency
    error = I2C_WriteRegister(DEVICE_ADDRESS, CTRL_REG1, FREQ_INIT);
      
    while(1)
    {
        error = I2C_ReadRegister(DEVICE_ADDRESS, LIS3DH_OUT_X_L, &Acc_X[0]);
        if (error == NO_ERROR)
        {
            error = I2C_ReadRegister(DEVICE_ADDRESS, LIS3DH_OUT_X_H, &Acc_X[1]);
            if (error == NO_ERROR)
            {
                error = I2C_ReadRegister(DEVICE_ADDRESS, LIS3DH_OUT_Y_L, &Acc_Y[0]);
                if (error == NO_ERROR)
                {
                    error = I2C_ReadRegister(DEVICE_ADDRESS, LIS3DH_OUT_Y_H, &Acc_Y[1]);
                    if (error == NO_ERROR)
                    {
                        error = I2C_ReadRegister(DEVICE_ADDRESS, LIS3DH_OUT_Z_L, &Acc_Z[0]);
                        if (error == NO_ERROR)
                        {
                            error = I2C_ReadRegister(DEVICE_ADDRESS, LIS3DH_OUT_Y_H, &Acc_Z[1]);
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
            
            if (A_X >= 0 && A_X <=2047) 
            {
                A_X_Conv = (A_X*M_digit_TO_MS2)*1000;     
            }
            else 
            {
                A_X_Conv = (A_X*M_digit_TO_MS2 - Q_digit_To_MS2)*1000;
            }
            
            A_X = (int16)(A_X_Conv);
            DataBuffer[1] = A_X & 0xFF;
            DataBuffer[2] = A_X >> 8;
            
            if (A_Y >= 0 && A_Y <=2047) 
            {
                A_Y_Conv = (A_Y*M_digit_TO_MS2)*1000;     
            }
            else 
            {
                A_Y_Conv = (A_Y*M_digit_TO_MS2 - Q_digit_To_MS2)*1000;
            }
            
            A_Y = (int16)(A_Y_Conv);
            DataBuffer[3] = A_Y & 0xFF;
            DataBuffer[4] = A_Y >> 8;
            
            if (A_Z >= 0 && A_Z <=2047) 
            {
                A_Z_Conv = (A_Z*M_digit_TO_MS2)*1000;     
            }
            else 
            {
                A_Z_Conv = (A_Z*M_digit_TO_MS2 - Q_digit_To_MS2)*1000;
            }
            
            A_Z = (int16)(A_Z_Conv);
            DataBuffer[5] = (uint8_t)(A_Z & 0xFF);
            DataBuffer[6] = (uint8_t)(A_Z >> 8);
            
            
            UART_PutArray(DataBuffer,BUFFER_SIZE);
        }
    }     
}

/* [] END OF FILE */
