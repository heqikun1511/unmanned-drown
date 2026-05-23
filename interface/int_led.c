#include "int_led.h"



void int_led_turn_on(led_struct* led)
{
    HAL_GPIO_WritePin(led->port, led->Pin, GPIO_PIN_RESET);
}

void int_led_turn_off(led_struct* led)
{
    HAL_GPIO_WritePin(led->port, led->Pin, GPIO_PIN_SET);
}
void int_led_toggle(led_struct* led){
    HAL_GPIO_TogglePin(led->port, led->Pin);
}