#include "rtc.h"
#define addr_DS3231 0x68 // I2C address (FOR RTC)

  int tm_year;                  // time.h: Years since 1900
                                // RTC:    Years since 2000
                                
  int tm_mon;                   // time.h: Months numbered 0-11
                                // RTC:    Months numbered 1-12
 struct tm timeinfo; // timeinfo structure (defined in time.h)

//NTP time updates
 WiFiUDP ntpUDP;
 NTPClient timeClient(ntpUDP);
 
 RTC_DS3231 rtc;

 unsigned long lastRtcCheckMillis = 0; // Variable to keep track of the last time the RTC was checked
 unsigned long lastRtcUpdateMillis = 0; // variable to keep track of the last time RTC was updated

//Decimal converstions for RTC
 uint8_t bcdToDec(uint8_t val){ // Convert binary coded decimal to normal decimal numbers
  return ( (val / 16 * 10) + (val % 16) );
 }
 uint8_t decToBcd(uint8_t val){// Convert normal decimal numbers to binary coded decimal
  return ( (val / 10 * 16) + (val % 10) );
}

void setRtcTime(int year, int month, int day, int hour, int minute, int second){
  Wire.beginTransmission(addr_DS3231);
  Wire.write(0); // Start at register 0

  // Write the time data in BCD format
  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(0)); // Day of the week (not used here)
  Wire.write(decToBcd(day));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year - 2000)); // Year (assumes year >= 2000)
  
  Wire.endTransmission();
}
void SetupRtc(){
  // Get the clock into a known state
  // Turn off any Alarms that may be left over from a previous use
  // Disable interrupts from the clock
  // Make sure the clock is running
  Wire.beginTransmission(addr_DS3231); // Communicate with the RTC via I2C
  Wire.write(0x0E); // Control register
  Wire.write(B00000000); // Write register bitmap
  Wire.endTransmission();  

 // Request time from NTP server
 {
 timeClient.update();
 unsigned long epochTime = timeClient.getEpochTime();

 // Convert epoch time to struct tm
 struct tm *timeinfo;
 time_t epochTimeAsTimeT = (time_t)epochTime;  // Convert epochTime to time_t
 timeinfo = localtime(&epochTimeAsTimeT);

 //Set the RTC time using the fetched time
 setRtcTime(timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
 timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);}
}
void FetchRtcData(){
  // Request seven bytes of data from DS3231 starting with register 00h
  Wire.beginTransmission(addr_DS3231);
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(addr_DS3231, 7);  

  // Strip out extraneous bits used by the RTC chip and convert BCD to Decimal
  timeinfo.tm_sec  = bcdToDec(Wire.read() & 0x7F);
  timeinfo.tm_min  = bcdToDec(Wire.read() & 0x7F);
  timeinfo.tm_hour = bcdToDec(Wire.read() & 0x3F);
  Wire.read(); // Skip the day-of-week register
  timeinfo.tm_mday = bcdToDec(Wire.read() & 0x3F);
  timeinfo.tm_mon  = bcdToDec(Wire.read() & 0x1F) - 1; // Adjust month value to agree with time.h format
  timeinfo.tm_year = bcdToDec(Wire.read()) + 100; // Adjust year value to agree with time.h format
}
void DisplayRtcData(){
  // Display the date and time in this form --> 2020-03-21 12:34:56
  struct tm *timeinfoPtr = &timeinfo; // Pointer to timeinfo structure
  char strfBuf[30]; // Buffer used by strftime()
  
  Serial.print("\n[");  // (yy/mm/dd) (UTC clock)
  strftime(strfBuf, sizeof(strfBuf), "%F %T", timeinfoPtr);  
  Serial.print(strfBuf); 
  Serial.print("]");
}
void UpdateRtc() {
    unsigned long currentMillis = millis();

    // Update RTC every 30 minutes (1800000 milliseconds)
    if (currentMillis - lastRtcUpdateMillis >= 500000) {
        lastRtcUpdateMillis = currentMillis;

        // Request time from NTP server
        timeClient.update();
        unsigned long epochTime = timeClient.getEpochTime();

        // Convert epoch time to struct tm
        struct tm *timeinfo;
        time_t epochTimeAsTimeT = (time_t)epochTime;  // Convert epochTime to time_t
        timeinfo = localtime(&epochTimeAsTimeT);

        // Set the RTC time using the fetched time
        setRtcTime(timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
                   timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

        // Optionally, display the updated RTC time
       Serial.print("[NTP Time update success]");
    }
}
