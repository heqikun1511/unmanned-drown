#ifndef INT_MOTOR_H
#define INT_MOTOR_H
#include "main.h"
#include "tim.h"

typedef struct{
    TIM_HandleTypeDef *tim
    uint16_t channel;
    uint16_t speed;

}motor_struct

void motor_setspeed(Motor_t *motor);



#endif 