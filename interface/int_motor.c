#include "int_motor.h"

void motor_setspeed(Motor_t *motor)
{
    if(motor->speed > 1000){
        printf("speed too large\n");
        return;
    }
__HAL_TIM_SET_COMPARE(motor->tim, motor->channel, motor->speed);

}
void int_motor_start(motor_struct *motor){
    HAL_TIM_PWM_Start(motor->tim, motor->channel);
}