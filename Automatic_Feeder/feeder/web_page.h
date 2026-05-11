#ifndef WEB_PAGE_H   
#define WEB_PAGE_H   

#include <WiFi.h>
#include "WiFi_comm.h"

/**
 * @brief Displays the header for the webpage.
 *
 * This function generates the HTML header content for the webpage.
 *
 * @param client The WiFi client object.
 */
void webPageHeader(WiFiClient& client);

/**
 * @brief Displays the main webpage.
 *
 * This function generates the HTML content for the webpage that allows users to set
 * the feeding time for the automatic feeder. 
 * It includes a form with dropdowns for selecting the hour and minute
 * and a save button to submit the new feeding time.
 *
 * @param client The WiFi client object.
 * @param feed_time The feedTime struct containing hour and minute.
 */
void webPageBody(WiFiClient& client, feedTime &feed_time);

#endif
