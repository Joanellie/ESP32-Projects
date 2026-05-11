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
#include <ESP32Time.h>
#include <stdint.h>
#include "WiFi_comm.h"

/*****************************
 *    NETWORK CREDENTIALS
 ****************************/
const char* ssid     = "Automatic Feeder";
const char* password = NULL;

/*****************************
 *    GLOBAL VARIABLES
 ****************************/
feedTime feed_time = {"4", "30"}; // Default feeding time: 04:30
ESP32Time rtc;
uint8_t HOUR = 9;
uint8_t MINUTE = 28;
uint8_t SECONDS = 0;
uint8_t DAY = 5;
uint8_t MONTH = 10;
uint16_t YEAR = 2026;
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

  /*****  RTC *****/
  rtc.setTime(SECONDS, MINUTE, HOUR, DAY, MONTH, YEAR);

  /*****  SEMAPHORE *****/
  Semaphore = xSemaphoreCreateBinary();
  if (Semaphore == NULL) {
    Serial.println("Failed to create semaphore!");
    while (1);
  } 
  /*******  TASK *******/ 
  // WiFi communication setup 
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
  /*******  TIMER *******/ 
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
  // Handle WiFi communication
  WiFi_comm_task(feed_time);
}

/*****************************
 *         FUNCTIONS
 ****************************/
// Task function to be executed when the semaphore is given
void Task(void *parameter) {
  for (;;) { // Infinite loop
    if(xSemaphoreTake(Semaphore, portMAX_DELAY)) {
      Serial.println("Task - Pet will eat now!");
    }
  }
}

// Callback executes when the timer expires
void Callback(TimerHandle_t xTimer) { 
  String now = rtc.getTime("%H:%M:%S");                 // Get current time in esp32
  Serial.print("Current time "); Serial.println(now);   

  uint8_t currentHour = rtc.getHour();                  // Get current hour
  uint8_t currentMinute = rtc.getMinute();              // Get current minute
  
  bool isFeedTime = false;                              // Flag to indicate if it's feeding time
  static bool isTriggered = false;                      // Flag to prevent multiple triggers within the same minute 

  // Check if current time matches the feeding time
  if (currentHour == feed_time.hour.toInt() && currentMinute == feed_time.minute.toInt()) {
    isFeedTime = true;
  } else{
    isTriggered = false; // Reset trigger if it's not feeding time
  }
  
  // Check if it's feeding time and the task hasn't been triggered yet
  if (isFeedTime && !isTriggered) {
    // Activate the feeder (gives semaphore to the task)
    Serial.println("Callback - Feeding time! Activating feeder...");
    xSemaphoreGive(Semaphore);
    isTriggered = true;
  }
}