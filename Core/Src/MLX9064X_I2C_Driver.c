#include "main.h"
#include "MLX9064X_I2C_Driver.h"
#include <stdio.h>

extern I2C_HandleTypeDef hi2c3;

void MLX9064x_I2CInit() {}

void MLX9064x_I2CFreqSet(int freq) {}

//Read a number of words from startAddress. Returns 0 if successful, -1 if error
int MLX9064x_I2CRead(uint8_t _deviceAddress, unsigned int startAddress, unsigned int nWordsRead, uint16_t* data)
{
    if (HAL_I2C_Mem_Read(&hi2c3, _deviceAddress << 1, startAddress, I2C_MEMADD_SIZE_16BIT,
                         (uint8_t *)data, nWordsRead * 2, 1000) != HAL_OK)
    {
        printf("I2C read fail: addr 0x%04X, %u words, HAL err 0x%lX\r\n",
               startAddress, nWordsRead, HAL_I2C_GetError(&hi2c3));
        return -1;
    }

    // sensor sends high byte first, STM32 stores low byte first -> swap
    for (unsigned int i = 0; i < nWordsRead; i++)
        data[i] = (data[i] >> 8) | (data[i] << 8);

    return 0;
}

//Write two bytes to a two byte address
int MLX9064x_I2CWrite(uint8_t _deviceAddress, unsigned int writeAddress, uint16_t data)
{
    uint8_t buf[2] = { data >> 8, data & 0xFF };
    if (HAL_I2C_Mem_Write(&hi2c3, _deviceAddress << 1, writeAddress, I2C_MEMADD_SIZE_16BIT,
                          buf, 2, 100) != HAL_OK)
    {
        printf("I2C write fail: addr 0x%04X, HAL err 0x%lX\r\n",
               writeAddress, HAL_I2C_GetError(&hi2c3));
        return -1;
    }

    uint16_t dataCheck;
    MLX9064x_I2CRead(_deviceAddress, writeAddress, 1, &dataCheck);
    if (dataCheck != data)
        return -2;

    return 0;
}