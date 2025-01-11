/***************************************************************************
*@brief Blink using Timers
This project enables the pin 2 of the MCU to make the LED within the PCB to
blink every 'interval' miliseconds.
'Interval' is the variable that stores the wished time in miliseconds of the
callback from Timer to be executed. Every time the callback executes, the 
status level of the LED will change to the opposite.
***************************************************************************/

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/timers.h"

#define led 2 // GPIO 2 from MCU

static const char *TAG = "Main";

uint8_t count = 0;
uint8_t led_status = 0; // Initial status led gpio 2 in LOW

esp_err_t init_led(void);
esp_err_t blink_led(void);
esp_err_t set_timer(void);

TimerHandle_t xTimers;
int interval = 1000;    //time in miliseconds
int timerId = 1;

void vTimerCallback( TimerHandle_t pxTimer ){
    ESP_LOGI(TAG, "Event was called from timer");
    blink_led();
}
void app_main(void)
{
    init_led();
    set_timer();
}

esp_err_t init_led(void)
{
    gpio_reset_pin(led);
    gpio_set_direction(led, GPIO_MODE_OUTPUT);
    return ESP_OK;
}

esp_err_t blink_led(void)
{
    led_status = !led_status;
    gpio_set_level(led, led_status);
    return ESP_OK;
}

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