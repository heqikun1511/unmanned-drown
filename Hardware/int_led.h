#ifndef INT_LED_H
#define INT_LED_H
#include "main.h"

typedef struct{
    GPIO_TypeDef *port;
    uint16_t Pin;

}led_struct;

void int_led_turn_on(led_struct* led);
void int_led_turn_off(led_struct* led);
void int_led_toggle(led_struct* led);
#endif // INT_LED_H     