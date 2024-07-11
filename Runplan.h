#ifndef RUNPLAN_H
#define RUNPLAN_H

#include "RTC.h"
#include "LEDStruct.h"

extern unsigned long lastLEDUpdateMillis;
extern unsigned long lastFlashCheckMillis;
extern const unsigned long flashInterval;
extern bool flashingActive;



void LEDTrigger();


#endif // RUNPLAN_H