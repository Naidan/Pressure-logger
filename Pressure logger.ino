/*
 Name:		Pressure_logger.ino
 Created:	9/21/2020 4:15:36 PM
 Author:	lucas
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//Oled Display settings
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     BUILTIN_LED // Reset pin #
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/*Spectrum Van / Home*/
#define ssid      "ROV"       
#define password  "12345678"

const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0

ESP8266WebServer server(80);

String data = "";

int sensorValue = 0;  // value read from the pot
int outputValue = 0;  // value to output to a PWM pin

String getPage() {
    //Send the data to the page variable
    String page = data;
    // Clean the data string   
    data = "";
    display.clearDisplay();
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setCursor(0, 0);
    display.println(page);
    display.display();

    return page;
}

void handleRoot() {
    // Create the web page
    server.send(200, "text/plain", getPage());


}

void setup() {


    // Start serial and wifi connections
    Serial.begin(115200);

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay(); // Clear the buffer
    display.setTextSize(2);      // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.cp437(true);         // Use full 256 char 'Code Page 437' font

    WiFi.begin(ssid, password);
    //  Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500); Serial.print("*");
        display.print("*");
        display.display();
    }
    //  WiFi connexion is OK
    Serial.println("");
    Serial.print("Connected to "); Serial.println(ssid);
    display.clearDisplay();
    display.setCursor(0, 0);     // Start at top-left corner
    display.println(ssid);
    Serial.print("IP address: "); Serial.println(WiFi.localIP());
    display.println(WiFi.localIP());
    display.display();

    // link to the function that manage launch page 
    server.on("/", handleRoot);
    //Initiate the server
    server.begin();
    Serial.println("HTTP server started");
    delay(10000);
}


void loop() {
    // read the analog in value

    sensorValue = analogRead(analogInPin);
    delay(2);
    sensorValue = analogRead(analogInPin);
    // sensorValue = analogRead(analogInPin);

     // map it to the range of the PWM out
    outputValue = map(sensorValue, 0, 1023, 0, 500);

    // print the readings in the Serial Monitor

    Serial.println(outputValue);
    data = outputValue;
    display.clearDisplay();
    display.setTextSize(4);      // Normal 1:1 pixel scale
    display.setCursor(0, 0);
    display.println(data);
    display.display();


    delay(1000);
}