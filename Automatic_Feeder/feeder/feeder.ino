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
 *      HANDLERS
 ****************************/
TaskHandle_t TaskHandle = NULL;
TimerHandle_t TimerHandle = NULL;
SemaphoreHandle_t Semaphore = NULL;

/*****************************
 *      ESP32 SETUP
 ****************************/
void setup() {
  Serial.begin(115200);
  pinMode(output26, OUTPUT);
  digitalWrite(output26, LOW);

  /*****************************
  *        SEMAPHORE
  ****************************/
  Semaphore = xSemaphoreCreateBinary();
  if (Semaphore == NULL) {
    Serial.println("Failed to create semaphore!");
    while (1);
  }  
  /*****************************
  *           TASK
  ****************************/  
  WiFi_comm_setup(ssid, password);
  // Task creation for WiFi communication
  xTaskCreatePinnedToCore(
    Task,         // Task function
    "Task",       // Task name
    2048,         // Stack size (bytes)
    NULL,         // Parameters
    1,            // Priority
    &TaskHandle,  // Task handle
    1             // Core 1
  ); 
  if (TaskHandle == NULL) {
    Serial.println("Failed to create task!");
    while (1);
  }
  /*****************************
  *           TIMER
  ****************************/  
  TimerHandle = xTimerCreate(
    "Timer",               // Timer name
    pdMS_TO_TICKS(5000),   // Timer period 
    pdTRUE,                // Auto-reload
    NULL,                  // Timer ID (not used)
    Callback               // Callback function
  );
  if (TimerHandle == NULL) {
    Serial.println("Failed to create timer!");
    while (1);
  }
  xTimerStart(TimerHandle, 0); // Start the timer
}
/*****************************
 *      ESP32 LOOP
 ****************************/
void loop(){
  WiFi_comm_task();
}

/*****************************
 *    TASK FUNCTIONS
 ****************************/
void Task(void *parameter) {
  for (;;) { // Infinite loop
    if(xSemaphoreTake(Semaphore, portMAX_DELAY)) {
      Serial.println("Executing Task");
    }
  }
}
/*****************************
 *   CALLLBACK FUNCTIONS
 ****************************/
void Callback(TimerHandle_t xTimer) {
  Serial.println("Callback executed");
  xSemaphoreGive(Semaphore);
}