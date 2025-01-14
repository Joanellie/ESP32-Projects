/**********************************************************************************
*@brief FreeRTOS: BLINKING RGB LEDS USING TASKS
This project will make blink LEDS Red and Green when a number (0 to 7) is sent or
received from a Queue respectively between Tasks R (the emitter to Queue) and 
Task G (the receptor from Queue)
Expected Behavior:
Numbers from 0 to 7 willl be stored at the Queue.
A Log in Green can be seen when a data is sent into the Queue
A Log in Yellow can be seen when a data is received from the Queue
A Log in Red can be seen when data could not be sent because of a full Queue or
either received because of an empty Queue.
*********************************************************************************/
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#define LEDR    33      /*LED Red connected to PIN 33 from MCU*/
#define LEDG    25      /*LED Green connected to PIN 25 from MCU*/

#define LEDR_DELAY  400     /*Delay set to Task from LED Red*/
#define LEDG_DELAY  2000    /*Delay set to Task from LED Green*/

#define STACK_SIZE 1024*2    /*Number of bytes the stack will hold*/

static const char *TAG = "Main";    /*Tag for the LOGS mns in terminal*/

QueueHandle_t GlobalQueue = 0;

/**********************
* Function Prototypes
**********************/
void app_main(void);
void vTask_LEDR(void *pvParameters);    /*Task from LED RED created in here*/
void vTask_LEDG(void *pvParameters);    /*Task from LED GREEN created in here*/
esp_err_t init_led(void);               /*Setting LEDS directions and inital level*/
esp_err_t create_tasks(void);           /*Creating a Task for each LED*/         

/*******************************
*   MAIN AND INIFINITE LOOP
*******************************/
void app_main(void)
{
    init_led();
    GlobalQueue = xQueueCreate(20, sizeof(uint32_t));   /*Creating a Queue to coomunicate TaskR and TaskG*/
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

/*Actions to be executed once created Task LED Red*/
void vTask_LEDR(void *pvParameters)
{
    while (1)
    {
        /*Sending through the GlobalQueue the numbers from 0 to 7*/
        for (size_t i = 0; i < 8; i++)
        {
            
            /*Check if a value could not be sent to the Queue*/
            if (!xQueueSend(GlobalQueue, &i, pdMS_TO_TICKS(100)))           //Wait 100ms to send another value if the GlobalQueue is full
            {
                ESP_LOGE(TAG, "Error sending %d to Queue", i);
            }
            else
            {
                /*Simulating task is taking 400ms to send the value to the Queue*/
                vTaskDelay(pdMS_TO_TICKS(LEDR_DELAY/2));    
                gpio_set_level( LEDR, 1);                   //LED Red in HIGH when starting sending a data to the Queue
                ESP_LOGI(TAG, "Sending: %d to Queue", i);
                vTaskDelay(pdMS_TO_TICKS(LEDR_DELAY/2));
                gpio_set_level( LEDR, 0);                   //LED Red in LOW when finished to send a data to the Queue
            }

        } 
        vTaskDelay(pdMS_TO_TICKS(7000));
    }    
}

/*Actions to be executed once created Task LED Green*/
void vTask_LEDG(void *pvParameters)
{
    int valueFromQueue = 0;

    while (1)
    {
        /*Check if the value was received correctly*/
        if (!xQueueReceive(GlobalQueue, &valueFromQueue, pdMS_TO_TICKS(100)))   //Wait 100ms if there's no elements in the Queue
        {
            ESP_LOGE(TAG, "Error receiving data from Queue");
        }
        else
        {
            /*Simulating receiving data takes 1s to be received*/
            vTaskDelay(pdMS_TO_TICKS(LEDG_DELAY/2));    
            gpio_set_level( LEDG, 1);          
            ESP_LOGW(TAG, "Receiving: %d to Queue", valueFromQueue);         
            vTaskDelay(pdMS_TO_TICKS(LEDG_DELAY/2));
            gpio_set_level( LEDG, 0);                 
        }
        /*Adding a small delay just to not make sure the watchdog will not be activated*/
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}