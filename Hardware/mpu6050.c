#include "mpu6050.h"

void MPU6050_WriteReg(uint8_t reg, uint8_t data)
{
    // Code to write 'data' to the register 'reg' of MPU6050 using I2C
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR_WRITE, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000); // 写寄存器
}

void MPU6050_ReadReg(uint8_t reg, uint8_t *data)
{
    // Code to read data from the register 'reg' of MPU6050 using I2C
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR_READ, reg, I2C_MEMADD_SIZE_8BIT, data, 1, 1000); // 读寄存器
}

void MPU6050_Init(void)
{ // Initialization code for MPU6050

    MPU6050_ReadReg(0x6B, 0x80); // 重置MPU6050,重置之后0x6B寄存器的值为0x40,为低功耗
    uint8_t data = 0;
    while (data != 0x40)
    {
        MPU6050_ReadReg(0x6B, &data);
    }
    MPU6050_WriteReg(0x6B, 0x00);   // 唤醒MPU6050
    MPU6050_WriteReg(0x1B, 3 << 3); // 设置陀螺仪量程为±2000°/s
    MPU6050_WriteReg(0x1C, 0x00);   // 设置加速度计量程为±2g
    MPU6050_WriteReg(0x38, 0x00);   // 关闭中断
    MPU6050_WriteReg(0x6A, 0x00);   // 关闭FIFO
    MPU6050_WriteReg(0x19, 0x00);   // 设置采样率为1kHz,蔡琰率大于使用评率，否则失真
    MPU6050_WriteReg(0x1A, 1);      // 设置低通滤波器为188Hz
    MPU6050_WriteReg(0x6B, 0x01);   // 设置时钟源为PLL with X axis gyroscope reference
    MPU6050_WriteReg(0x6c, 0x00);   // 使能加速度角速度传感器
}

void MPU6050_Get_GyroData(gyrodata *data)
{
    uint16_t high = 0;
    uint16_t low = 0;
    MPU6050_ReadReg(0x43, &high); // 读取陀螺仪X轴高8位
    MPU6050_ReadReg(0x44, &low);  // 读取�陀螺仪X轴低8位
    // x
    data->gyro_x = (float)((int16_t)((high << 8) | low)) / 32768.0f * 2000.0; // 转换为°/s
    // y
    MPU6050_ReadReg(0x45, &high);                                             // 读取陀螺仪Y轴高8位
    MPU6050_ReadReg(0x46, &low);                                              // 读取陀螺仪Y轴低8位
    data->gyro_y = (float)((int16_t)((high << 8) | low)) / 32768.0f * 2000.0; // 转换为°/s
    // z
    MPU6050_ReadReg(0x47, &high);                                             // 读取陀螺仪Z轴高8位
    MPU6050_ReadReg(0x48, &low);                                              // 读取陀螺仪Z轴低8位
    data->gyro_z = (float)((int16_t)((high << 8) | low)) / 32768.0f * 2000.0; // 转换为°/s
}

void MPU6050_Get_AccData(accdata *data)
{
    uint16_t high = 0;
    uint16_t low = 0;
    MPU6050_ReadReg(0x3B, &high); // 读取加速度计X轴高8位
    MPU6050_ReadReg(0x3C, &low);  // 读取加速度计X轴低8位
    // x
    data->acc_x = (float)((int16_t)((high << 8) | low)) / 32768.0f * 2.0; // 转换为g
    // y
    MPU6050_ReadReg(0x3D, &high);                                         // 读取加速度计Y轴高8位
    MPU6050_ReadReg(0x3E, &low);                                          // 读取加速度计Y轴低8位
    data->acc_y = (float)((int16_t)((high << 8) | low)) / 32768.0f * 2.0; // 转换为g
    // z
    MPU6050_ReadReg(0x3F, &high);                                         // 读取加速度计Z轴高8位
    MPU6050_ReadReg(0x40, &low);                                          // 读取加速度计Z轴低8位
    data->acc_z = (float)((int16_t)((high << 8) | low)) / 32768.0f * 2.0; // 转换为g
}