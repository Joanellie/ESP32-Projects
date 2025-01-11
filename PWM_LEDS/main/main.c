#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/timers.h"
#include "driver/ledc.h"

static const char *TAG = "Main";    /*Tag for the LOGS mns in terminal*/

uint8_t led_status = 0; /*Initial status led GPIO 2 in LOW*/

esp_err_t set_timer(void);      /*Timers task created in here*/
esp_err_t set_pwm(void);        /*Cofigures every PWM setting of each LED*/
esp_err_t set_pwm_duty(void);   /*Sets new duty cycle and updates it for each LED*/

TimerHandle_t xTimers;  /*Variable for Timer task that stores the configuration to be set*/
int interval = 50;      /*Miliseconds before Timer callback executes*/
int timerId = 1;        /*ID for the Timer Task created*/

int dutyR = 0;     /*Initial Duty cycle for RED LED*/
int dutyG = 300;   /*Initial Duty cycle for GREEN LED*/
int dutyB = 600;   /*Initial Duty cycle for BLUE LED*/

/*Function Prototypes*/
void vTimerCallback( TimerHandle_t pxTimer );
void app_main(void);

/*******************************
*   CONFIGURATION SET SECTION
*******************************/
void app_main(void)
{
    set_pwm();
    set_timer();
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
    dutyR += 10;
    dutyG += 10;
    dutyB += 10;

    /* ADC for the PWM is set as 10 bit resolution, so equals to 1023 as max value*/
    if (dutyR > 1023){
        dutyR = 0;
    }
    if (dutyG > 1023){
        dutyG = 0;
    }
    if (dutyB > 1023){
        dutyB = 0;
    }

    set_pwm_duty();    
}

/*********************
*   PWM SECTION
*********************/

esp_err_t set_pwm(void){
    /*Settings for RED LED*/
    ledc_channel_config_t channelConfigR = {0};
    channelConfigR.gpio_num = 33;
    channelConfigR.speed_mode = LEDC_HIGH_SPEED_MODE;
    channelConfigR.channel = LEDC_CHANNEL_0;
    channelConfigR.intr_type = LEDC_INTR_DISABLE;
    channelConfigR.timer_sel = LEDC_TIMER_0;
    channelConfigR.duty = 0;

    /*Settings for GREEN LED*/
        ledc_channel_config_t channelConfigG = {0};
    channelConfigG.gpio_num = 25;
    channelConfigG.speed_mode = LEDC_HIGH_SPEED_MODE;
    channelConfigG.channel = LEDC_CHANNEL_1;
    channelConfigG.intr_type = LEDC_INTR_DISABLE;
    channelConfigG.timer_sel = LEDC_TIMER_0;
    channelConfigG.duty = 0;

    /*Settings for BLUE LED*/
        ledc_channel_config_t channelConfigB = {0};
    channelConfigB.gpio_num = 26;
    channelConfigB.speed_mode = LEDC_HIGH_SPEED_MODE;
    channelConfigB.channel = LEDC_CHANNEL_2;
    channelConfigB.intr_type = LEDC_INTR_DISABLE;
    channelConfigB.timer_sel = LEDC_TIMER_0;
    channelConfigB.duty = 0;

    ledc_channel_config(&channelConfigR);
    ledc_channel_config(&channelConfigG);
    ledc_channel_config(&channelConfigB);

    ledc_timer_config_t timerConfig = {0};
    timerConfig.speed_mode = LEDC_HIGH_SPEED_MODE;
    timerConfig.duty_resolution = LEDC_TIMER_10_BIT;
    timerConfig.timer_num = LEDC_TIMER_0;
    timerConfig.freq_hz = 20000; 

    ledc_timer_config(&timerConfig);

    return ESP_OK;
}

esp_err_t set_pwm_duty(void){

ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, dutyR);
ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, dutyG);
ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2, dutyB);

ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);
ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2);

    return ESP_OK;
}