#ifndef COM_CONFIG_H
#define COM_CONFIG_H
#include "main.h"
typedef enum
{
    REMOTE_CONNECT = 0,
    REMOTE_DISCONNECT,
} Remotestate;

typedef enum
{
    IDLE = 0,
    NORMAL,
    FIX_HEIGHT,
    FAIL,
} flightstate;
typedef enum
{
    FREE = 0,
    MAX,
    LEAVE_MAX,
    MIN,
    UNLOCK
} thr_state;//油门遥感

typedef struct
{
    int16_t thr;
    int16_t yaw;
    int16_t roll;
    int16_t pitch;
    int16_t shutdown;
    int16_t fix_height;
} remotedata;

// 陀螺仪数据

// 角速度
typedef struct
{
    float gyro_x;
    float gyro_y;
    float gyro_z;
} gyrodata;

typedef struct
{
    float acc_x;
    float acc_y;
    float acc_z;
} accdata;

typedef struct
{
    gyrodata gyro;
    accdata acc;
} gyro_acc_data;

/* 解算欧拉角数据 */
typedef struct
{
    float roll;
    float pitch;
    float yaw;
} oula;

#endif