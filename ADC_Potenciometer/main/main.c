/***************************************************************************
*@brief ADC with Pternciometer and RGB LEDs
This project will let you to manipulate RGB LEDs level from a
potenciometer, which will be read using the ADC peripheral.
Expected behavior:  ADC from 0 to 999     -> All LEDS OFF
                    ADC from 1000 to 1999 -> LED Red ON
                    ADC from 2000 to 2999 -> LED Red-Green ON
                    ADC from 3000 to 3999 -> LED Red-Green-Blue ON
                    ADC from 4000 to 4095 -> All LEDS OFF
***************************************************************************/
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/timers.h"
#include "driver/ledc.h"
#include "driver/adc.h"

#define ledR    33      /*LED Red connected to PIN 33 from MCU*/
#define ledG    25      /*LED Green connected to PIN 25 from MCU*/
#define ledB    26      /*LED Blue connected to PIN 26 from MCU*/

static const char *TAG = "Main";    /*Tag for the LOGS mns in terminal*/

TimerHandle_t xTimers;  /*Variable for Timer task that stores the configuration to be set*/
int interval = 50;      /*Miliseconds before Timer callback executes*/
int timerId = 1;        /*ID for the Timer Task created*/
int adc_val = 0;        /*ADC value obtained from Potenciometer*/

/**********************
* Function Prototypes
**********************/
void vTimerCallback( TimerHandle_t pxTimer );
void app_main(void);
esp_err_t set_timer(void);      /*Timers task created in here*/
esp_err_t set_adc(void);        /*ADC configurations: Channel, Attenuation and Resolution*/ 
esp_err_t init_led(void);       /*Setting LEDS directions and inital level*/

/*******************************
*   CONFIGURATION SET SECTION
*******************************/
void app_main(void)
{
    init_led();
    set_timer();
    set_adc();
}

/*********************
*   TIMER SECTION
*********************/
esp_err_t set_timer(void)
{
    ESP_LOGI(TAG, "Timer init configuration");
    xTimers = xTimerCreate("Timer",         // Just a text name, not used by the kernel.
                              (pdMS_TO_TICKS(interval)), // The timer period in ticks.
                              pdTRUE,          // The timers will auto-reload themselves when they expire.
                              (void *)timerId,       // Assign each timer a unique id equal to its array index.
                              vTimerCallback   // Each timer calls the same callback when it expires.
    );

    if (xTimers == NULL)
    {
        ESP_LOGE(TAG, "Timer was not created");
    }
    else
    {
        if (xTimerStart(xTimers, 0) != pdPASS)
        {
            ESP_LOGE(TAG, "The timer could not be set into the Active state.");
        }
    }
    return ESP_OK;
}

void vTimerCallback( TimerHandle_t pxTimer ){
    adc_val = adc1_get_raw(ADC1_CHANNEL_4);         /*Remember: GPIO32 from MCU is ADC CH4*/
    ESP_LOGI(TAG, "ADC Raw Value: %d", adc_val);
    
    int adc_case = adc_val / 1000;  /*Raw ADC value divided by 1000 (4095/1000) to obtained 4 differents brightness cases*/

    switch (adc_case)
    {
    case 0:
        gpio_set_level(ledR, 0);
        gpio_set_level(ledG, 0);
        gpio_set_level(ledB, 0);
        break;
    case 1:
        gpio_set_level(ledR, 1);
        gpio_set_level(ledG, 0);
        gpio_set_level(ledB, 0);
        break;
    case 2:
        gpio_set_level(ledR, 1);
        gpio_set_level(ledG, 1);
        gpio_set_level(ledB, 0);
        break;
    case 3:
        gpio_set_level(ledR, 1);
        gpio_set_level(ledG, 1);
        gpio_set_level(ledB, 1);
        break;            
    default:
        gpio_set_level(ledR, 0);
        gpio_set_level(ledG, 0);
        gpio_set_level(ledB, 0);
        break;
    }

}

/*********************
*   ADC SECTION
*********************/
esp_err_t set_adc(void){

    adc2_config_channel_atten(ADC2_CHANNEL_4, ADC_ATTEN_DB_0);
    adc1_config_width(ADC_WIDTH_BIT_12);

    return ESP_OK;
}

/*********************
*   LEDS SECTION
*********************/
esp_err_t init_led(void)
{
    gpio_reset_pin(ledR);
    gpio_set_direction(ledR, GPIO_MODE_OUTPUT);

    gpio_reset_pin(ledG);
    gpio_set_direction(ledG, GPIO_MODE_OUTPUT);

    gpio_reset_pin(ledB);
    gpio_set_direction(ledB, GPIO_MODE_OUTPUT);

    /*Initialize each LED in LOW level*/
    gpio_set_level(ledR, 0);
    gpio_set_level(ledG, 0);
    gpio_set_level(ledB, 0);

    return ESP_OK;
}