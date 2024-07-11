#include "Runplan.h"


unsigned long lastLEDUpdateMillis = 0; // Variable to keep track of the last time LEDs were updated
unsigned long lastFlashCheckMillis = 0; // Variable to keep track of the last time checkAndFlashLEDs() was called
const unsigned long flashInterval = 1000; // Interval for checking and flashing LEDs (1 seconds)
bool flashingActive = false; // Variable to keep track of the flashing 
unsigned long previousMillis = 0;
unsigned long functionDuration = 5000;  // 15 seconds in milliseconds
int currentPattern = 0;

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

void Testingplan(){
// Check if it's time to move to the next pattern
  unsigned long currentMillis = millis();

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
