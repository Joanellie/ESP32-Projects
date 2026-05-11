#include <WiFi.h>
#include "web_page.h"
#include <string.h>

// Webpage header with CSS styling for the feeder selector
void webPageHeader(WiFiClient& client) {
    client.println("<!DOCTYPE html><html>");
    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<link rel=\"icon\" href=\"data:,\">");
    // CSS to style the feeder selector
    client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
    client.println(".setHr { font-size: 24px; }");
    client.println(".time-select { font-size: 20px; padding: 5px; margin: 10px; }");
    client.println(".buttonSave { background-color: #4c90af; border: none; color: white; padding: 16px 40px;");
    client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
    client.println("</style></head>");
}

// Webpage body with the feeder selector form
void webPageBody(WiFiClient& client, feedTime &feed_time) {
    client.println(
        "<body>"
            "<h1 id=\"currentTime\" class=\"setHr\">Pet will eat at " + feed_time.hour + ":" + feed_time.minute + " pm</h1>"
            "<p class=\"setHr\">Set new Hr</p>"
            "<form action=\"/saveHr\" method=\"GET\">"
                "<select name=\"feed_time.hour\" class=\"time-select\">"
                    "<option" + (feed_time.hour.toInt() == 1 ? " selected" : "") + ">01</option>"
                    "<option" + (feed_time.hour.toInt() == 2 ? " selected" : "") + ">02</option>"
                    "<option" + (feed_time.hour.toInt() == 3 ? " selected" : "") + ">03</option>"
                    "<option" + (feed_time.hour.toInt() == 4 ? " selected" : "") + ">04</option>"
                    "<option" + (feed_time.hour.toInt() == 5 ? " selected" : "") + ">05</option>"
                    "<option" + (feed_time.hour.toInt() == 6 ? " selected" : "") + ">06</option>"
                    "<option" + (feed_time.hour.toInt() == 7 ? " selected" : "") + ">07</option>"
                    "<option" + (feed_time.hour.toInt() == 8 ? " selected" : "") + ">08</option>"
                    "<option" + (feed_time.hour.toInt() == 9 ? " selected" : "") + ">09</option>"
                    "<option" + (feed_time.hour.toInt() == 10 ? " selected" : "") + ">10</option>"
                    "<option" + (feed_time.hour.toInt() == 11 ? " selected" : "") + ">11</option>"
                "</select>"
                
                ":"

                "<select name=\"feed_time.minute\" class=\"time-select\">"
                    "<option" + (feed_time.minute.toInt() == 0 ? " selected" : "") + ">00</option>"
                    "<option" + (feed_time.minute.toInt() == 30 ? " selected" : "") + ">30</option>"
                "</select>"

                "<p>"
                    "<!-- GET /saveHr?feed_time.hour=04&feed_time.minute=30 HTTP/1.1 -->"
                    "<button type=\"submit\" class=\"buttonSave\">save</button>"
                "</p>"
            "</form>"
        "</body>");
    client.println("</html>");
}