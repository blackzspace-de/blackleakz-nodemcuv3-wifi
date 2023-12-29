/*
 (c) by BlackLeakz | 12/29/23

 Name: blackleakz-nodemcuv3-wifi 
 Version: 0.1
 Github: https://github.com/blackzspace-de/blackleakz-nodemcuv3-wifi
 Homepage: https://blackzspace.de/
 Details: This program is made by learning by doing. 
 We use an Asynchrone WebServer, LittleFS, Adafruits OLED SSD1306 Display Libarys and other essential WiFi-Modules you need.


1x OLED SSD1306 128x64
1x Breadboard
*x Some Wires
2x LED 
1x resistor
 
*/


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <LittleFS.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>




// Adafruit Settings
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// WiFi Details
const char* ssid     = "BLACKLEAKZ-AP";
const char* password = "123456789";


// Starts Asynchrone WebServer
AsyncWebServer server(80);







// Initialize Messages on Screen
void oled_start() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Console >");
  display.display();
  delay(2000);
  display.clearDisplay();


  display.setCursor(0,0);
  display.println("Started---");
  display.display();
  delay(2000);
  display.clearDisplay();
}








// WebServer's 404 Page
void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}





// The Main-Part of this program 
void setup(){



  // Starts Serial-Communication || BAUDRATE: 115200
  Serial.begin(115200);
  Serial.println("Console > Blackz NodeMCUv3 ESP8266 App started...");


  // Declare BUILTIN LED 
  pinMode(LED_BUILTIN, OUTPUT);    

  
  // Mounting FileSystem
  if(!LittleFS.begin()){
    Serial.println("Console > An error has occured while mounting LittleFS");
    return;
  }



  // Starting Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Console > SSD1306 allocation failed!!! :::..:.:..---__---_-"));
    for(;;); 
  }



  // Starting Display-Text function
  display.clearDisplay();
  oled_start();



 


  // Setting up WiFi softAP with given variables
  Serial.print("Console > Setting AP (Access Point)…");
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Setting AP");
  display.display();
  display.setCursor(0,10);
  display.println(ssid);
  display.display();
  display.setCursor(0,20);
  display.println(password);
  display.display();
  delay(555);

  WiFi.softAP(ssid, password); // Starts WiFi 

  IPAddress IP = WiFi.softAPIP();  // output variable from function to string
  Serial.print("Console > AP IP address: ");
  Serial.println(IP);
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("AP IP >");
  display.display();
  display.setCursor(0,10);
  display.println(IP);
  display.display();

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());
  display.setCursor(0,20);
  display.println(WiFi.localIP());
  display.display();






  // AsyncWebServer's     ||   Route-Handler    || AsyncWebServer's      ||   Route-Handler   
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  { request->send(LittleFS, "/index.html", "text/html"); });

  server.on("/scan", HTTP_GET, [](AsyncWebServerRequest *request)
  { request->send(LittleFS, "/wifi_scan.html", "text/html"); });



  server.onNotFound(notFound);
  // END AsyncWebServer's     ||   Route-Handler    ||  END AsyncWebServer's      ||   Route-Handler 




  // Start server
  server.begin();


}
 








void loop(){  


  // LED BLINK LOOP
  digitalWrite(LED_BUILTIN, LOW); 
  delay(1000);                      
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(2000);

}