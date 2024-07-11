#include "Runplan.h"


unsigned long lastLEDUpdateMillis = 0; // Variable to keep track of the last time LEDs were updated
unsigned long lastFlashCheckMillis = 0; // Variable to keep track of the last time checkAndFlashLEDs() was called
const unsigned long flashInterval = 1000; // Interval for checking and flashing LEDs (1 seconds)
bool flashingActive = false; // Variable to keep track of the flashing 


void LEDTrigger() {
  FetchRtcData();  // Ensure the timeinfo is updated

  unsigned long currentMillis = millis();

  // Check if current time is between 16:00 and 16:59
  if (timeinfo.tm_hour == 19 && timeinfo.tm_min >= 30 && timeinfo.tm_min < 59) {
    unsigned long elapsedMinutes = timeinfo.tm_min % 3;  // Minutes elapsed since last 3-minute interval
    Serial.print(elapsedMinutes);

    if (elapsedMinutes < 2) {
      // Check if it's time to update flashing status (every secondd
        applyPatternToNeoPixels(AllRings, PULSE, strip.Color(128, 0, 128));  // Pulse AllRings in blue
      }
      else if (currentMillis - lastFlashCheckMillis >= 1000) {
        lastFlashCheckMillis = currentMillis;
        Serial.println(" {Flashing Active}");
    } else {
      // If not within the flashing period, stop flashing
      if (currentMillis - lastFlashCheckMillis >= 1000) {
        lastFlashCheckMillis = currentMillis;
        Serial.println(" {Flashing Inactive}");
        applyPatternToNeoPixels(AllRings, STEADY, strip.Color(0, 0, 0));  // Turn off the LEDs
      }
    }
  } else {
    // If outside the 16:00-16:59 range, ensure flashing is stopped
    if (currentMillis - lastFlashCheckMillis >= 1000) {
      lastFlashCheckMillis = currentMillis;
      Serial.println(" {Outside of specified time range}");
      applyPatternToNeoPixels(AllRings, STEADY, strip.Color(0, 0, 0));  // Turn off the LEDs
    }
  }
}


