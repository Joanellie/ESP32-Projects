/***************************************************************************
*@brief FreeRTOS: BLINKING RGB LEDS USING TASKS
This project will make blink RGB LEDS at different frecuency using Tasks. 
***************************************************************************/
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define LEDR    33      /*LED Red connected to PIN 33 from MCU*/
#define LEDG    25      /*LED Green connected to PIN 25 from MCU*/
#define LEDB    26      /*LED Blue connected to PIN 26 from MCU*/

#define LEDR_DELAY  1000    /*Delay set to LED Red*/
#define LEDG_DELAY  2000    /*Delay set to LED Green*/
#define LEDB_DELAY  4000    /*Delay set to LED Blue*/

#define STACK_SIZE 1024*2    /*Number of bytes the stack will hold*/

static const char *TAG = "Main";    /*Tag for the LOGS mns in terminal*/

/**********************
* Function Prototypes
**********************/
void app_main(void);
void vTask_LEDR(void *pvParameters);    /*Task from LED RED created in here*/
void vTask_LEDG(void *pvParameters);    /*Task from LED GREEN created in here*/
void vTask_LEDB(void *pvParameters);    /*Task from LED BLUE created in here*/
esp_err_t init_led(void);               /*Setting LEDS directions and inital level*/
esp_err_t create_tasks(void);           /*Creating a Task for each LED*/

/*******************************
*   MAIN AND INIFINITE LOOP
*******************************/
void app_main(void)
{
    init_led();
    create_tasks();
    while (1)
    {
        /* code */
    }
    
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

    gpio_reset_pin( LEDB);
    gpio_set_direction( LEDB, GPIO_MODE_OUTPUT);

    /*Initialize each LED in LOW level*/
    gpio_set_level(LEDR, 0);
    gpio_set_level(LEDG, 0);
    gpio_set_level(LEDB, 0);

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

    /*Creating Task for LED BLUE*/
    xTaskCreate( vTask_LEDB, 
                "vTask_LEDB", 
                STACK_SIZE, 
                &ucParameterToPass, 
                1, &xHandle );

    return ESP_OK;
}

/*Actions to be executed once created Task LED Red*/
void vTask_LEDR(void *pvParameters)
{
    while (1)
    {
      gpio_set_level( LEDR, 0);
      vTaskDelay(pdMS_TO_TICKS(LEDR_DELAY)); 
      gpio_set_level( LEDR, 1);
      vTaskDelay(pdMS_TO_TICKS(LEDR_DELAY));  
    }    
}

/*Actions to be executed once created Task LED Green*/
void vTask_LEDG(void *pvParameters)
{
    while (1)
    {
      gpio_set_level( LEDG, 0);
      vTaskDelay(pdMS_TO_TICKS(LEDG_DELAY)); 
      gpio_set_level( LEDG, 1);
      vTaskDelay(pdMS_TO_TICKS(LEDG_DELAY));        
    }    
}

/*Actions to be executed once created Task LED Green*/
void vTask_LEDB(void *pvParameters)
{
    while (1)
    {
      gpio_set_level( LEDB, 0);
      vTaskDelay(pdMS_TO_TICKS(LEDB_DELAY)); 
      gpio_set_level( LEDB, 1);
      vTaskDelay(pdMS_TO_TICKS(LEDB_DELAY));        
    }    
}