#include "com_filter.h"
#include "mpu6050.h"

#define  ALPHA 0.15



int16_t CommonFilter_lowpass(int16_t new_data, int16_t last_data)
{
    return (int16_t)(ALPHA * new_data + (1 - ALPHA) * last_data);
}

KalmanFiter_struct kfs[3]={

{0.02,0,0,0,0.001,0.543},
{0.02,0,0,0,0.001,0.543},
{0.02,0,0,0,0.001,0.543}


}


