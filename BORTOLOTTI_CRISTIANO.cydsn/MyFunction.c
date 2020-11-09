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
#include "stdio.h"

ErrorCode error_message;
char message[50] = {'\0'};
     
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
  
void Start_Peripherals(void)
{
    UART_Start();
    PWM_Start();
    I2C_Start();
    EEPROM_Start();
    EEPROM_UpdateTemperature();
    CyDelay(5);
}

void Set_Ctrl_Reg4(void)
{
    error_message = I2C_WriteRegister(DEVICE_ADDRESS, CTRL_REG4, CTRL_REG4_SET_HR_AND_BDU);
    if (error_message == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 4 successfully written as: 0x%02X\r\n", CTRL_REG4_SET_HR_AND_BDU);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during control register 4 setting\r\n");   
        }
}

void Set_Ctrl_Reg1(uint8_t frequency)
{
    error_message = I2C_WriteRegister(DEVICE_ADDRESS, CTRL_REG1, frequency);
    if (error_message == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", frequency);
            UART_PutString(message); 
        }
        else
        {
            UART_PutString("Error occurred during control register 1 setting\r\n");   
        }
}

uint8_t freq_from_memory (void)
{
    uint8_t freq_memory;
    freq_memory = EEPROM_ReadByte(FREQ_MEMORY_ADDRESS);
    switch(freq_memory)
    {
        case FREQ_1:
            Set_PWM(PERIOD_FREQ_1);
            count = 0;
            break;
        case FREQ_10:
            Set_PWM(PERIOD_FREQ_10);
            count = 1;
            break;
        case FREQ_25:
            Set_PWM(PERIOD_FREQ_25);
            count = 2;
            break;
        case FREQ_50:
            Set_PWM(PERIOD_FREQ_50);
            count = 3;
            break;
        case FREQ_100:
            Set_PWM(PERIOD_FREQ_100);
            count = 4;
            break;
        case FREQ_200:
            Set_PWM(PERIOD_FREQ_200);
            count = 5;
            break;
        default:
            EEPROM_UpdateTemperature();
            EEPROM_WriteByte(FREQ_1,FREQ_MEMORY_ADDRESS);
            Set_PWM(PERIOD_FREQ_1); 
            freq_memory = EEPROM_ReadByte(FREQ_MEMORY_ADDRESS);
            break;   
    }
    return freq_memory;
}

void Start_Interrupt(void)
{
    CyGlobalIntEnable; 
    isr_Button_StartEx(Custom_ISR_Button);
}

A_array XYZ_Reading (void)
{
    A_array array;
    //sequential reading of the registers of the X, Y, Z axis; this complex nesting (annidamento) allows to print a spefic error for each register
    // in order to easily identify where the error occurred
    error_message = I2C_ReadRegister(DEVICE_ADDRESS, A_X_L, &array.X[0]);
    if (error_message == NO_ERROR)
    {
        error_message = I2C_ReadRegister(DEVICE_ADDRESS, A_X_H, &array.X[1]);
        if (error_message == NO_ERROR)
        {
            error_message = I2C_ReadRegister(DEVICE_ADDRESS, A_Y_L, &array.Y[0]);
            if (error_message == NO_ERROR)
            {
                error_message = I2C_ReadRegister(DEVICE_ADDRESS, A_Y_H, &array.Y[1]);
                if (error_message == NO_ERROR)
                {
                    error_message = I2C_ReadRegister(DEVICE_ADDRESS, A_Z_L, &array.Z[0]);
                    if (error_message == NO_ERROR)
                    {
                        error_message = I2C_ReadRegister(DEVICE_ADDRESS, A_Z_H, &array.Z[1]);
                        if (error_message == NO_ERROR)
                        {
                             return array;
                        }
                    }
                }
            }
        }
    }
    flag_error = 1;
    return array;
}

A_int XYZ_Elaboration(A_array array)
{
    A_int value;
    value.X = (int16)((array.X[0] | (array.X[1]<<8)))>>4;
    value.Y = (int16)((array.Y[0] | (array.Y[1]<<8)))>>4;
    value.Z = (int16)((array.Z[0] | (array.Z[1]<<8)))>>4;
    
    A_float Conv;
    Conv.X = (value.X*M_digit_TO_MS2)*1000;
    Conv.Y = (value.Y*M_digit_TO_MS2)*1000;
    Conv.Z = (value.Z*M_digit_TO_MS2)*1000;
    
    value.X = (int16)(Conv.X);
    value.Y = (int16)(Conv.Y);
    value.Z = (int16)(Conv.Z);
    
    return value;
}

void XYZ_Sending(A_int value)
{
    DataBuffer[1] = (uint8_t)(value.X & 0xFF);
    DataBuffer[2] = (uint8_t)(value.X >> 8);
    DataBuffer[3] = (uint8_t)(value.Y & 0xFF);
    DataBuffer[4] = (uint8_t)(value.Y >> 8);
    DataBuffer[5] = (uint8_t)(value.Z & 0xFF);
    DataBuffer[6] = (uint8_t)(value.Z >> 8);
    
    UART_PutArray(DataBuffer,BUFFER_SIZE); 
}

void Freq_1_to_200(uint8_t frequency)
{
    Set_PWM(PERIOD_FREQ_1 - 2500*count);
    frequency = EEPROM_ReadByte(FREQ_MEMORY_ADDRESS) + 0x10;
    EEPROM_UpdateTemperature();
    EEPROM_WriteByte(frequency, FREQ_MEMORY_ADDRESS);
    Set_Ctrl_Reg1(frequency);
}

void Freq_200_to_1(void)
{
    count = 0;
    EEPROM_UpdateTemperature();
    EEPROM_WriteByte(FREQ_1, FREQ_MEMORY_ADDRESS);
    Set_Ctrl_Reg1(FREQ_1);
    Set_PWM(PERIOD_FREQ_1);
}

/* [] END OF FILE */
