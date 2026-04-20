#include <WiFi.h>
#include "web_page.h"
#include <string.h>

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

void esp32WebPage(WiFiClient& client, String hour, String minute) {
    client.println(
        "<body>"
            "<script>"
                "function updateTime() {"
                    "var hour = document.querySelector('select[name=\"hour\"]').value;"
                    "var minute = document.querySelector('select[name=\"minute\"]').value;"
                "}"
            "</script>"        
            "<h1 id=\"currentTime\" class=\"setHr\">Pet will eat at " + hour + ":" + minute + " pm</h1>"
            "<p class=\"setHr\">Set new Hr</p>"
            "<form action=\"/saveHr\" method=\"GET\" onsubmit=\"updateTime()\">"
                "<select name=\"hour\" class=\"time-select\">"
                    "<option" + (hour == "01" ? " selected" : "") + ">01</option>"
                    "<option" + (hour == "02" ? " selected" : "") + ">02</option>"
                    "<option" + (hour == "03" ? " selected" : "") + ">03</option>"
                    "<option" + (hour == "04" ? " selected" : "") + ">04</option>"
                    "<option" + (hour == "05" ? " selected" : "") + ">05</option>"
                    "<option" + (hour == "06" ? " selected" : "") + ">06</option>"
                    "<option" + (hour == "07" ? " selected" : "") + ">07</option>"
                    "<option" + (hour == "08" ? " selected" : "") + ">08</option>"
                    "<option" + (hour == "09" ? " selected" : "") + ">09</option>"
                    "<option" + (hour == "10" ? " selected" : "") + ">10</option>"
                    "<option" + (hour == "11" ? " selected" : "") + ">11</option>"
                "</select>"
                
                ":"

                "<select name=\"minute\" class=\"time-select\">"
                    "<option" + (minute == "00" ? " selected" : "") + ">00</option>"
                    "<option" + (minute == "30" ? " selected" : "") + ">30</option>"
                "</select>"

                "<p>"
                    "<!-- GET /saveHr?hour=04&minute=30 HTTP/1.1 -->"
                    "<button type=\"submit\" class=\"buttonSave\">save</button>"
                "</p>"
            "</form>"
        "</body>");
    client.println("</html>");
}