/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <WiFi.h>
#include "web_page.h"

// Network credentials
const char* ssid     = "Automatic Feeder";
const char* password = NULL;

// Web server port 
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variable to store the current output state
struct pin26State
{
  const char* ON = "on";
  const char* OFF = "off";
};

pin26State D26;
const char *ledState = D26.OFF;

// Output variable to GPIO pin
const int output26 = 26;

// Variables to store the feeding time
String feedHour = "04";
String feedMinute = "30";

void setup() {
  Serial.begin(115200);
  // Initialize the output variable as output
  pinMode(output26, OUTPUT);
  // Set output to LOW
  digitalWrite(output26, LOW);

  // Configure ESP32 as an Access Point network with its SSID and password
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);

  // Obtain the IP address of the ESP32 Access Point
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /saveHr") >= 0) {
              // Extract hour and minute from the header
              int hourIndex = header.indexOf("hour=");
              int minuteIndex = header.indexOf("minute=");
              if (hourIndex >= 0) {
                feedHour = header.substring(hourIndex + 5, hourIndex + 7);
              }
              if (minuteIndex >= 0) {
                feedMinute = header.substring(minuteIndex + 7, minuteIndex + 9);
              }
              Serial.print("New feed time: ");
              Serial.print(feedHour);
              Serial.print(":");
              Serial.println(feedMinute);
              digitalWrite(output26, HIGH);
              delay(800);
              digitalWrite(output26, LOW);
            }

            webPageHeader(client);
            esp32WebPage(client, feedHour, feedMinute);

            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}