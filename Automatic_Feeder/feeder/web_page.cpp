#include <WiFi.h>
#include "web_page.h"
#include <string.h>

void webPageHeader(WiFiClient& client) {
    client.println("<!DOCTYPE html><html>");
    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<link rel=\"icon\" href=\"data:,\">");
    // CSS to style the on/off buttons 
    client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
    client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
    client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
    client.println(".button2 {background-color: #555555;}");
    client.println(".buttonSave { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
    client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
    // CSS to style the feeder selector
    client.println(".setHr { font-size: 24px; }");
    client.println("</style></head>");
}

void webPageBody(WiFiClient& client, const char *outputState) {
    // Web Page Heading
    client.println("<body><h1>ESP32 Web Server</h1>");
    
    // Display current state, and ON/OFF buttons for GPIO 26  
    client.print("<p>GPIO 26 - State ");
    client.print(outputState);
    client.println("</p>");
    // If the outputState is off, it displays the ON button       
    if (strcmp(outputState, "off") == 0) {
        client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
    } else {
        client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
    } 
    client.println("</body></html>");
    // The HTTP response ends with another blank line
    client.println();
}

void esp32WebPage(WiFiClient& client) {
    client.println(
        "<body>"
            "<h1 id=\"currentTime\" class=\"setHr\">Pet will eat at 04:30 pm</h1>"
            "<p class=\"setHr\">Set new Hr:</p>"
            "<form action=\"/saveHr\" method=\"GET\">"
                "<select name=\"hour\" class=\"time-select\">"
                    "<option>01</option>"
                    "<option>02</option>"
                    "<option>03</option>"
                    "<option selected>04</option>"
                    "<option>05</option>"
                    "<option>06</option>"
                    "<option>07</option>"
                    "<option>08</option>"
                    "<option>09</option>"
                    "<option>10</option>"
                    "<option>11</option>"
                "</select>"
                
                ":"

                "<select name=\"minute\" class=\"time-select\">"
                    "<option>00</option>"
                    "<option selected>30</option>"
                "</select>"

                "<p>"
                    "<!-- GET /saveHr?hour=04&minute=30 HTTP/1.1 -->"
                    "<button type=\"submit\" class=\"save-button\">save</button>"
                "</p>"
            "</form>"
            "<script>"
                "function updateTime() {"
                    "var hour = document.querySelector('select[name=\"hour\"]').value;"
                    "var minute = document.querySelector('select[name=\"minute\"]').value;"
                    "document.getElementById('currentTime').innerText = 'Pet will eat at ' + hour + ':' + minute + ' pm';"
                "}"
                "document.querySelector('select[name=\"hour\"]').addEventListener('change', updateTime);"
                "document.querySelector('select[name=\"minute\"]').addEventListener('change', updateTime);"
                "updateTime();"
            "</script>"
        "</body>");
    client.println("</html>");
}