/*********
  A. Joanellie H.A
  Embedded Systems Engineer
*********/
#define output26 26
// Load FreeRTOS library
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "WiFi_comm.h"

// Network credentials
const char* ssid     = "Automatic Feeder";
const char* password = NULL;

void setup() {
  Serial.begin(115200);
  // Initialize the output variable as output
  pinMode(output26, OUTPUT);
  // Set output to LOW
  digitalWrite(output26, LOW);

  WiFi_comm_setup(ssid, password);
}

void loop(){
  WiFi_comm_task();
}