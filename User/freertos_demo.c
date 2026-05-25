#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "FreeRTOSConfig.h"
#include "int_IP5305T.h"
#include "int_motor.h"
#include "int_led.h"
#include "com_config.h"

motor_struct left0={.tim=&htim3,.channel=TIM_CHANNEL_1,.speed=200};
motor_struct right0={.tim=&htim2,.channel=TIM_CHANNEL_2,.speed=200};
motor_struct left1={.tim=&htim4,.channel=TIM_CHANNEL_4,.speed=200};
motor_struct right1={.tim=&htim1,.channel=TIM_CHANNEL_3,.speed=200};


led_struct left_top={.port=LED1_GPIO_Port,.Pin=LED1_Pin};
led_struct right_top={.port=LED2_GPIO_Port,.Pin=LED2_Pin};
led_struct left_bottom={.port=LED3_GPIO_Port,.Pin=LED3_Pin};
led_struct right_bottom={.port=LED4_GPIO_Port,.Pin=LED4_Pin};

Remotestate remotestate=REMOTE_CONNECT;
flightstate flight_state=FAIL;
/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 List_t                  TestList;           
 /* 任务优先级 */

#define START_TASK_PRIO 1                  
#define START_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */
void start_task(void *pvParameters);        /* 任务函数 */





#define POWER_TASK_PRIO 4
#define POWER_STK_SIZE 128
TaskHandle_t  power_task_handler;
void power_task(void *pvParameters);


#define FLIGHT_TASK_PRIO 3
#define FLIGHT_STK_SIZE 128
TaskHandle_t  flight_task_handler;
void flight_task(void *pvParameters);

#define POWER_TASK_PERIOD 10000
#define FLIGHT_TASK_PERIOD 6
#define LED_TASK_PERIOD 100

#define LED_TASK_PRIO 1
#define LED_STK_SIZE 128
TaskHandle_t  led_task_handler;
void led_task(void *pvParameters);





void freertos_demo(void)
{   

    
  
    
    xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */

     
    vTaskStartScheduler();
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* 进入临界区 */
    xTaskCreate((TaskFunction_t )power_task,            /* 任务函数 */
                (const char*    )"power_task",          /* 任务名称 */
                (uint16_t       )POWER_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )POWER_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&power_task_handler);   /* 任务句柄 */
    
    xTaskCreate((TaskFunction_t )flight_task,            /* 任务函数 */
                (const char*    )"flight_task",          /* 任务名称 */
                (uint16_t       )FLIGHT_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )FLIGHT_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&flight_task_handler);  /* 任务句柄 */
    
     xTaskCreate((TaskFunction_t )led_task,            /* 任务函数 */
                (const char*    )"led_task",          /* 任务名称 */
                (uint16_t       )LED_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )LED_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&led_task_handler);  /* 任务句柄 */            

    taskEXIT_CRITICAL();            /* 退出临界区 */
    vTaskDelete(NULL);              /* 删除自身，避免任务函数返回 */
}

void power_task(void *pvParameters){
    TickType_t xLastWakeTime=xTaskGetTickCount();//获取基准时间
    while(1){
        vTaskDelayUntil(&xLastWakeTime,10000);
        IP5305T_InitStart();
    }
}
void flight_task(void *pvParameters){

    TickType_t xLastWakeTime=xTaskGetTickCount();//获取基准时间

    while(1){

       
        vTaskDelayUntil(&xLastWakeTime,FLIGHT_TASK_PERIOD);
    }

}
void led_task(void *pvParameters){
    TickType_t xLastWakeTime=xTaskGetTickCount();

    while(1){
        if(remotestate==REMOTE_CONNECT){
            int_led_on(&left_top);

            int_led_on(&right_top);

            int_led_on(&left_bottom);

            int_led_on(&right_bottom);
        }
        else if(remotestate==REMOTE_DISCONNECT){
            int_led_off(&left_top);
            int_led_off(&right_top);
            int_led_off(&left_bottom);
            int_led_off(&right_bottom);
        }

        vTaskDelayUntil(&xLastWakeTime,LED_TASK_PERIOD);

    }
}