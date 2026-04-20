#ifndef WEB_PAGE_H   
#define WEB_PAGE_H   

#include <WiFi.h>

void esp32WebPage(WiFiClient& client, String hour = "04", String minute = "30");
void webPageHeader(WiFiClient& client);

#endif              