#ifndef RUNPLAN_H
#define RUNPLAN_H

#include "RTC.h"
#include "LEDStruct.h"

extern unsigned long lastLEDUpdateMillis;
extern unsigned long lastFlashCheckMillis;
extern const unsigned long flashInterval;
extern bool flashingActive;
extern unsigned long previousMillis;
extern unsigned long functionDuration;
extern int currentPattern;


void LEDTrigger();
void Testingplan();


#endif // RUNPLAN_H
