#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>
#include <stdio.h> 
#include <RTClib.h>

 
struct fullTime{
    uint16_t hour;
    uint16_t minute;
    uint16_t second;
};
struct fullDate {
    uint16_t day;
    uint16_t month;
    uint16_t year;
};


class myClock
{ 
  private: 
    RTC_DS1307 rtc; 

  public: 
    myClock(); 
    void clockInitialize(void);
    fullTime currentTime(void);
    fullDate currentDate(void);
    const char* currentDay(void);
    void  setTimeToCompileTime(void);
    
};

#endif
