#ifndef RTC_H
#define RTC_H

 #include <Wire.h>
 #include <time.h>
 #include <WiFiUdp.h>
 #include <NTPClient.h>
 #include <RTClib.h>

// Global variables
extern RTC_DS3231 rtc;
// Global variables
extern int tm_year; // time.h: Years since 1900; RTC: Years since 2000
extern int tm_mon;  // time.h: Months numbered 0-11; RTC: Months numbered 1-12
extern struct tm timeinfo; // timeinfo structure (defined in time.h)
extern const uint8_t addr_DS3231;  // I2C address (FOR RTC)
extern uint8_t bcdToDec(uint8_t val);
extern uint8_t decToBcd(uint8_t val);
extern unsigned long lastRtcCheckMillis; // Variable to keep track of the last time the RTC was checked
extern unsigned long lastRtcUpdateMillis; // variable to keep track of the last time RTC was updated
// NTP time updates
extern WiFiUDP ntpUDP;
extern NTPClient timeClient;

// Function prototypes
void SetupRtc();
void setRtcTime(int year, int month, int day, int hour, int minute, int second); 
void FetchRtcData();
void DisplayRtcData();
void updateRTC();

#endif // RTC_H
