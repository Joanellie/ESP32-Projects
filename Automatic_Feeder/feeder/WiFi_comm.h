#ifndef WIFI_COMM_H   
#define WIFI_COMM_H   

#include <stdint.h>

struct feedTime {
    String hour;
    String minute;
};

void WiFi_comm_setup(const char* ssid, const char* password);
void WiFi_comm_task(feedTime &feed_time);

#endif