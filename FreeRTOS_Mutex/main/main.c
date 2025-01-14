/**********************************************************************************
*@brief FreeRTOS: Mutex between Tasks
This project contains a shared resource that is blink per 8 times. The shared
resource will be executed by the task which takes first the key from the Mutex.
Expected behavior:
If Task R takes the key, Red LED will blink.
If Task G takes the key, Green LED will blink.
Everytime a task finished to execute the shared resource, the key is given back
to the mutex, so the shared resource could be available again.
*********************************************************************************/
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

#define LEDR    33      /*LED Red connected to PIN 33 from MCU*/
#define LEDG    25      /*LED Green connected to PIN 25 from MCU*/

/*Delay in Tasks R and G before they want to take the key again */
#define LEDR_DELAY  1000    //Since Task R has a minor delay, this one will take the key more times than Task G 
#define LEDG_DELAY  2000    

#define STACK_SIZE 1024*2    /*Number of bytes the stack will hold*/

static const char *TAG = "Main";    /*Tag for the LOGS mns in terminal*/

SemaphoreHandle_t GlobalKey = 0;

/**********************
* Function Prototypes
**********************/
void app_main(void);
void vTask_LEDR(void *pvParameters);    /*Task from LED RED created in here*/
void vTask_LEDG(void *pvParameters);    /*Task from LED GREEN created in here*/
esp_err_t init_led(void);               /*Setting LEDS directions and inital level*/
esp_err_t create_tasks(void);           /*Creating a Task for each LED*/         
esp_err_t shared_resource(int led);     /*Resource that makes blink a LED 8 times*/        

/*******************************
*   MAIN AND INIFINITE LOOP
*******************************/
void app_main(void)
{
    GlobalKey = xSemaphoreCreateMutex();
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

/*********************
*   MUTEX SECTION
*********************/
esp_err_t shared_resource(int led){
    for (size_t i = 0; i < 8; i++)
    {
        gpio_set_level(led, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(led, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    return ESP_OK;
}

/*Actions to be executed once created Task LED Red*/
void vTask_LEDR(void *pvParameters)
{
    while (1)
    {
        /*Check if the key is available*/
       if(xSemaphoreTake(GlobalKey, pdMS_TO_TICKS(100))){
            ESP_LOGE(TAG, "Task R took the resource");      
            shared_resource(LEDR);                          /*If so, Task R takes the shared resource from mutex*/
            xSemaphoreGive(GlobalKey);                      /*After taken the shared resource, Task R returns the key to the mutex*/
       }       
       vTaskDelay(pdMS_TO_TICKS(LEDR_DELAY));   /*To avoid errors with Watchdog timer*/
    }    
}

/*Actions to be executed once created Task LED Green*/
void vTask_LEDG(void *pvParameters)
{

    while (1)
    {
        /*Check if the key is available*/
       if(xSemaphoreTake(GlobalKey, pdMS_TO_TICKS(100))){
            ESP_LOGI(TAG, "Task G took the resource");      
            shared_resource(LEDG);                          /*If so, Task R takes the shared resource from mutex*/
            xSemaphoreGive(GlobalKey);                      /*After taken the shared resource, Task R returns the key to the mutex*/
       }       
       vTaskDelay(pdMS_TO_TICKS(LEDG_DELAY));   /*To avoid errors with Watchdog timer*/
    }
}