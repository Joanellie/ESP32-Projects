/*****************************
  Author: A. Joanellie H.A
  Embedded Systems Engineer
 ****************************/
#define output26 26
/*****************************
 *        INCLUDES
 ****************************/
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "WiFi_comm.h"

/*****************************
 *    NETWORK CREDENTIALS
 ****************************/
const char* ssid     = "Automatic Feeder";
const char* password = NULL;

/*****************************
 *      TASK HANDLES
 ****************************/
TaskHandle_t BlinkTaskHandle = NULL;

/*****************************
 *      ESP32 SETUP
 ****************************/
void setup() {
  Serial.begin(115200);
  pinMode(output26, OUTPUT);
  digitalWrite(output26, LOW);

/******  BLINK TASK ******/ 
  WiFi_comm_setup(ssid, password);
  // Task creation for WiFi communication
  xTaskCreatePinnedToCore(
    BlinkTask,         // Task function
    "BlinkTask",       // Task name
    2048,             // Stack size (bytes)
    NULL,              // Parameters
    1,                 // Priority
    &BlinkTaskHandle,  // Task handle
    1                  // Core 1
  );  

}

void loop(){
  WiFi_comm_task();
}

/*****************************
 *    TASK FUNCTIONS
 ****************************/
void BlinkTask(void *parameter) {
  for (;;) { // Infinite loop
    Serial.println("PIN HIGH");
    digitalWrite(output26, HIGH);
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 10 ms
    Serial.println("PIN LOW");
    digitalWrite(output26, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 10 ms
  }
}