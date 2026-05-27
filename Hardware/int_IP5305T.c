#include "int_IP5305T.h"
#include "FreeRTOS.h"
#include "task.h"

void IP5305T_InitStart(void)
{
    HAL_GPIO_WritePin(POWERKEY_GPIO_Port, POWERKEY_Pin, GPIO_PIN_RESET);
    vTaskDelay(100);
    HAL_GPIO_WritePin(POWERKEY_GPIO_Port, POWERKEY_Pin, GPIO_PIN_SET);
}