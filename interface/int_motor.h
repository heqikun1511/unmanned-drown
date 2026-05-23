#ifndef INT_MOTOR_H
#define INT_MOTOR_H
#include "main.h"
#include "tim.h"

typedef struct{
    TIM_HandleTypeDef *tim;
    uint16_t channel;
    uint16_t speed;
} motor_struct;

void motor_setspeed(motor_struct *motor);
void int_motor_start(motor_struct *motor);



#endif 