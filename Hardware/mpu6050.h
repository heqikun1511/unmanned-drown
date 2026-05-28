#ifndef __MPU6050_H
#define __MPU6050_H
#include "i2c.h"
#include "com_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdlib.h"


#define MPU6050_ADDR 0x68;
#define MPU6050_ADDR_WRITE 0xD0
#define MPU6050_ADDR_READ 0xD1


void MPU6050_Init(void);
void MPU6050_calucate_offset(void);

void MPU6050_Get_GyroData(gyrodata* data);//算角速度
void MPU6050_Get_AccData(accdata* data);//算加速度



#endif /* __MPU6050_H */