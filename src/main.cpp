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
#include <AsyncElegantOTA.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>


// Adafruit Settings
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 


#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// WiFi Details
const char* ssid     = "BLACKLEAKZ-AP";
const char* password = "123456789";


// mDNS Credentials
#define mDns = "blackleakz"



// Addr to read/write from 
const char* host = "http://blackzspace.de/projects/blackleakz-nodemcuv3-esp8266/config.json"; // JSON Config 

const char* message = "http://blackzspace.de/projects/blackleakz-nodemcuv3-esp8266/services/welcome.msg" // Connect Message



// Values for WiFi-Scanner
int numberOfNetworks;
unsigned long lastAusgabe;
const unsigned long intervall = 5000;


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





// Starting WiFi Network scan  
void wifi_networkscan() {
  Serial.println("Console > Starting Network Scan !! :::..::.");
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Console > ");
  display.display();
  display.setCursor(2,10);
  display.println("Scan for: ");
  display.display();
  display.setCursor(1,0);
  display.println("Networkz !!");
  display.display();

  int n = WiFi.scanNetworks();
   
  Serial.println("Console > Scan done");
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Scan done!!!");
  display.display();
  display.startscrollright(0x00, 0x00);
  
  if (n == 0)
  Serial.println("Console > No Networks found!!! :(( ");
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("No Networks forund");
  display.display();
  display.startscrollright(0x00, 0x00);
  else
  {
    Serial.print(n);
    Serial.println("Console > Networks found");
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Networks found:");
    display.display();
    display.startscrollright(0x00, 0x00);
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.println("Console >>> ");
      Serial.print(i + 1);  //Sr. No
      display.clearDisplay();
      display.setCursor(0,0);
      display.println(i + 1);
      display.display();
      Serial.print(": ");
      display.setCursor(0,20);
      display.println(WiFi.SSID(i));
      display.display();
      Serial.print(WiFi.SSID(i)); //SSID
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i)); //Signal Strength
      display.setCursor(0,30);
      display.println(WiFi.RSSI(i));
      display.setCursor(0,40);
      display.println("MAC:");
      display.display();
      Serial.print(") MAC:");
      Serial.print(WiFi.BSSIDstr(i));
      display.setCursor(0,50);
      display.println(WiFi.BSSIDstr(i));
      display.display();
      display.setCursor(0,60);
      display.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" Unsec" : "Sec");
      display.display();
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" Unsecured":" Secured");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before starting New scanning again
  delay(1500);
}




void scan_wifi()
{
  static unsigned long lastAusgabe;
  const unsigned long intervall = 5000;

  if ( (numberOfNetworks > 0) && (millis() - lastAusgabe > intervall) )
  {
    lastAusgabe = millis();
    numberOfNetworks--;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Network-name: ");
    display.setCursor(0, 10);
    display.print(WiFi.SSID(numberOfNetworks));
    display.setCursor(0, 20);
    display.print("Signal strength: ");
    display.setCursor(0, 30);
    display.print(WiFi.RSSI(numberOfNetworks));
    display.setCursor(0, 40);
    display.display();
  }
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



  // Starting mDNS 
  if (!MDNS.begin("blackleakz")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(356);
      }
    }
  Serial.println("mDNS responder started");







 


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


  // Starts WiFi-Network Scan
  wifi_networkscan();


  // AsyncWebServer's     ||   Route-Handler    || AsyncWebServer's      ||   Route-Handler   
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  { request->send(LittleFS, "/index.html", "text/html"); });

  server.on("/scan", HTTP_GET, [](AsyncWebServerRequest *request)
  { request->send(LittleFS, "/wifi_scan.html", "text/html"); });



  server.onNotFound(notFound);
  // END AsyncWebServer's     ||   Route-Handler    ||  END AsyncWebServer's      ||   Route-Handler 




  // Start AsyncElegantOTA
  AsyncElegantOTA.begin(&server);

  // Start server
  server.begin();


}
 








void loop(){  

  // mDNS update 
  MDNS.update();


  // WiFi-Scan Loop
  scan_wifi();


  // LED BLINK LOOP
  digitalWrite(LED_BUILTIN, LOW); 
  delay(1000);                      
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(2000);

}