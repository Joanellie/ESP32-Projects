#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define led 2               //GPIO 2 from MCU

static const char* TAG = "Main";

uint8_t count = 0;
uint8_t led_status = 0;     //Initial status led gpio 2 in LOW

esp_err_t init_led(void);
esp_err_t blink_led(void);

void app_main(void)
{
    init_led();

    while (1)
    {
        vTaskDelay(1000/portTICK_PERIOD_MS);
        blink_led();
        printf("Led status: %s\n", led_status == 0 ? "LOW":"HIGH");

        count ++;
        if(count > 30)
        {
            count = 0;                              //Reset counter after 30 counts
        }
        if(count < 10)
        {
            ESP_LOGI(TAG, "Value: %u", count);      //Green Informative MNS in terminal console from 0 to 10 counts
        }
        else if (count >= 10 && count < 20)
        {
            ESP_LOGW(TAG, "Value: %u", count);      //Yellow Warning MNS in terminal console from 10 to 20 counts
        }
        else
        {
            ESP_LOGE(TAG, "Value: %u", count);      //Red Error MNS in terminal console from 20 to 30 counts
        }                      
    }
    
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