#ifndef WEB_PAGE_H   
#define WEB_PAGE_H   

#include <WiFi.h>

void esp32WebPage(WiFiClient& client);
void webPageHeader(WiFiClient& client);
void webPageBody(WiFiClient& client, const char *outputState);

#endif              