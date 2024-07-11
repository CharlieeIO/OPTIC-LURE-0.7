 char programName[] = "Optical Lure 0.7";
//Libraries
 #include <Wire.h>
 #include <SPI.h>
 #include <WiFiNINA.h>
 #include <ArduinoOTA.h>
 #include "Adafruit_TCS34725.h"

#include "RTC.h"
#include "LEDStruct.h"
#include "Runplan.h"

unsigned long previousMillis = 0;
unsigned long functionDuration = 5000;  // 15 seconds in milliseconds
int currentPattern = 0;

//Wifi Credentials 
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

// Example usage in void loop()
void loop() {
  unsigned long currentMillis = millis();

// Check the RTC every second
  if (currentMillis - lastRtcCheckMillis >= 1000) {
   lastRtcCheckMillis = currentMillis;  
   FetchRtcData();
   DisplayRtcData();}

 // Handle OTA
 ArduinoOTA.poll();
  
  // Check if it's time to move to the next pattern
  if (currentMillis - previousMillis >= functionDuration) {
    // Reset timer
    previousMillis = currentMillis;

    // Increment pattern index
    currentPattern = (currentPattern + 1) % 12;  // There are 7 patterns to cycle through

    // Clear NeoPixels
    strip.clear();
    strip.show();
  }

  // Call the current pattern
  switch (currentPattern) {
    case 0:
      applyPatternToNeoPixels(AllRings, STEADY, strip.Color(0, 0, 200));  // AllRings
      break;
    case 1:
      applyPatternToNeoPixels(Ring_4, STEADY, strip.Color(0, 0, 200));  // Ring_4
      break;
    case 2:
      applyPatternToNeoPixels(Ring_3, STEADY, strip.Color(0, 0, 200));  // Ring_3
      break;
    case 3:
      applyPatternToNeoPixels(Ring_2, STEADY, strip.Color(0, 0, 200));  // Ring_2
      break;
    case 4:
      applyPatternToNeoPixels(Ring_1, STEADY, strip.Color(0, 0, 200));  // Ring_1
      break;
    case 5:
      applyPatternToNeoPixels(StraightLines, STEADY, strip.Color(0, 0, 200));  // StraightLines
      break;
    case 6:
      applyPatternToNeoPixels(DiagonalLines, STEADY, strip.Color(0, 0, 200));  // DiagonalLines
      break;
    case 7:
     applyPatternToNeoPixels(AllRings, STEADY, strip.Color(0, 0, 200));  // DiagonalLines
     break;
    case 8:
    applyPatternToNeoPixels(AllRings, BLINK, strip.Color(0, 0, 200));  // DiagonalLines
    break;
    case 9:
    applyPatternToNeoPixels(AllRings, PULSE, strip.Color(0, 0, 200));  // DiagonalLines
    break;
    case 10:
    applyPatternToNeoPixels(AllRings, TWINKLE, strip.Color(0, 0, 200));  // DiagonalLines
    break;
    case 11:
    applyPatternToNeoPixels(AllRings, SPARKLE, strip.Color(0, 0, 200));  // DiagonalLines
    break;
    default:
      // Should not happen, but handle default case if necessary
      break;
  }
}