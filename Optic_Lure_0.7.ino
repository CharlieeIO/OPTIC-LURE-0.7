char programName[] = "Optical Lure 0.7";
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoOTA.h>
#include "Adafruit_TCS34725.h"
#include "RTC.h"
#include "LEDStruct.h"
#include "Runplan.h"
//wifi credentials  
 const char* ssid = "WHOIguest";
 const char* password = "interstitial5126";

void ConnectWiFi(){
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}
void SetupOTA(){
  IPAddress localIP = WiFi.localIP();
  ArduinoOTA.begin(localIP, "Nano33IoT", "password", InternalStorage);

  Serial.print("IP address: ");
  Serial.println(localIP);

}

void setup() {
 Wire.begin(); // Enable I2C communications for rtc
 strip.begin();
 strip.show(); // Initialize all pixels to 'off'
 ConnectWiFi();//Connect to WiFi network 
 timeClient.begin();
 SetupOTA();
 SetupRtc(); // Get the clock into a known state
 timeClient.update();
 applyPatternToNeoPixels(StraightLines, STEADY, strip.Color(0, 0, 0));  // Ring_3
 delay(1000);
}

void loop() {
 ArduinoOTA.poll();
}
