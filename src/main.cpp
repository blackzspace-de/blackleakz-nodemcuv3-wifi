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


#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


const char* ssid     = "BLACKLEAKZ-AP";
const char* password = "123456789";


AsyncWebServer server(80);




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









void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}


void setup(){
  Serial.begin(115200);
  Serial.println("Console > Blackz NodeMCUv3 ESP8266 App started...");

  // Declare BUILTIN LED 
  pinMode(LED_BUILTIN, OUTPUT);    

  
  // Starting FileSystem
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



 


  
  Serial.print("Console > Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
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
  display.setCursor(0,30);
  display.println(WiFi.localIP());
  display.display();





  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.html", "text/html"); });

  server.onNotFound(notFound);

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