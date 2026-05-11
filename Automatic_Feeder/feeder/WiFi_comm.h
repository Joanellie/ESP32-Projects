#ifndef WIFI_COMM_H   
#define WIFI_COMM_H   

#include <stdint.h>

/**
 * @brief Structure to hold the feeding time.
 *
 * This struct contains the hour and minute for the feeding time.
 * It is a global variable that can be accessed and modified by 
 * the WiFi communication task and the web page functions.
 */
struct feedTime {
    String hour;
    String minute;
};

/**
 * @brief Initializes the WiFi communication.
 *
 * This function sets up the ESP32 as a WiFi access point and starts the web server.
 *
 * @param ssid The name for the WiFi access point.
 * @param password The password for the WiFi access point.
 */
void WiFi_comm_setup(const char* ssid, const char* password);

/**
 * @brief Displays the main webpage.
 *
 * This function handles incoming client connections and serves the webpage that 
 * allows users to set the feeding time for the automatic feeder.
 * @param feed_time The feedTime struct containing hour and minute.
 */
void WiFi_comm_task(feedTime &feed_time);

#endif