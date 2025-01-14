/********************************************************************************************
*@brief FreeRTOS: Binary Semaphore between Task R and Task G
This project makes the Task R to give a Key to the Task G to executes whenever the
key is given to it. while Key is not given then Task G can not take any Key so it
will not execute.
Expected behavior:

1. Within Task R we will make Red LED blink 8 times before giving the Key. While this
is happening, a LOG Error (in red) will appear at the monitor device with the string "Task
R is giving the key". Then after giving the key, Task R will have a 10 sec delay
until it gives again the Key.

2. Once the Key is given, Task G will take the Key and execute Green LED blinking 8
times too. You will see at the monitor device a LOG Informative (in green) a mns saying:
"Task G is working" when Key have just taken.
"Task G is entering into sleep..." when Task G finished from executing.
"Task G is sleeping" when there was no Key available to be taken.
*******************************************************************************************/
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

#define LEDR    33      /*LED Red connected to PIN 33 from MCU*/
#define LEDG    25      /*LED Green connected to PIN 25 from MCU*/

/*Delay set in Tasks R before give again the Key*/
#define LEDR_DELAY  10000   /*10 sec*/

#define STACK_SIZE 1024*2    /*Number of bytes the stack will hold*/

static const char *TAG = "Main";    /*Tag for the LOGS mns in terminal*/

SemaphoreHandle_t GlobalKey = 0;    /*Key that will be given or taken in Binary Semaphore*/

/**********************
* Function Prototypes
**********************/
void app_main(void);
void vTask_LEDR(void *pvParameters);    /*Task from LED RED created in here*/
void vTask_LEDG(void *pvParameters);    /*Task from LED GREEN created in here*/
esp_err_t init_led(void);               /*Setting LEDS directions and inital level*/
esp_err_t create_tasks(void);           /*Creating a Task for each LED*/         
esp_err_t blink_indicator(int led);     /*Resource that makes blink a LED 8 times*/        

/*******************************
*   MAIN AND INIFINITE LOOP
*******************************/
void app_main(void)
{
    GlobalKey = xSemaphoreCreateBinary();   /*Creating the Binary Semaphore*/
    init_led();
    create_tasks();    
}

/*********************
*   LEDS SECTION
*********************/
esp_err_t init_led(void)
{
    gpio_reset_pin( LEDR);
    gpio_set_direction( LEDR, GPIO_MODE_OUTPUT);

    gpio_reset_pin( LEDG);
    gpio_set_direction( LEDG, GPIO_MODE_OUTPUT);

    /*Initialize each LED in LOW level*/
    gpio_set_level(LEDR, 0);
    gpio_set_level(LEDG, 0);

    return ESP_OK;
}

/*Make LED blink 8 times to indicate when Task R gives the Key to Task G 
    and for indicate Task G could took the Key*/
esp_err_t blink_indicator(int led){
    for (size_t i = 0; i < 8; i++)
    {
        gpio_set_level(led, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(led, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    gpio_set_level(led, 0);
    return ESP_OK;
}

/*********************
*   TASKS SECTION
*********************/
esp_err_t create_tasks(void){
    static uint8_t ucParameterToPass;
    TaskHandle_t xHandle = NULL;

    /*Creating Task for LED RED*/
    xTaskCreate( vTask_LEDR, 
                "vTask_LEDR", 
                STACK_SIZE, 
                &ucParameterToPass, 
                1, &xHandle );

    /*Creating Task for LED GREEN*/
    xTaskCreate( vTask_LEDG, 
                "vTask_LEDG", 
                STACK_SIZE, 
                &ucParameterToPass, 
                1, &xHandle );

    return ESP_OK;
}

/******************************
*   BINARY SEMAPHORE SECTION
******************************/

/*Task R will be the responsible to give the key to Task G*/
void vTask_LEDR(void *pvParameters)
{
    while (1)
    {
        blink_indicator(LEDR);                  /*Blink Red LED to see that Key was given*/
        ESP_LOGE(TAG, "Task R is giving the Key");
        xSemaphoreGive(GlobalKey);              /*Task R gives Key (Binary Semaphore in '1')...
                                                Once the Task gives the Key, it returns Binary Semmaphore to '0' inmediatly*/            
        vTaskDelay(pdMS_TO_TICKS(LEDR_DELAY));  /*Delay to wait 10 seconds to give again the Key*/
    }    
}

/*Actions to be executed once created Task LED Green*/
void vTask_LEDG(void *pvParameters)
{
    while (1)
    {
        /*Check if the key was given*/
        if(xSemaphoreTake(GlobalKey, portMAX_DELAY)){       /*Wait a maximum of 12hrs without consuming CPU resources until the Key is given*/
            ESP_LOGI(TAG, "Task G is working");      
            blink_indicator(LEDG);                          /*Indicator that Task G has taken the key*/
            ESP_LOGI(TAG, "Task G is entering into sleep...");
       }
        /*Task G is not working since Key is not given (Binary Semaphore in '0')*/
        ESP_LOGI(TAG, "Task G is sleeping");
        vTaskDelay(pdMS_TO_TICKS(100));   /*To avoid errors with Watchdog timer*/            
    }
}