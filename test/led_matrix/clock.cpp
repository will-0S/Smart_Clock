#include "clock.h"

myClock::myClock(){ 
  //Serial.begin(9600); 
  //delay(100); 
} 

void myClock::clockInitialize(void){
    // SETUP RTC MODULE
    if (! rtc.begin()) {
    Serial.println("RTC module is NOT found");
    Serial.flush();
    while (1);
    }
    // Check if the RTC is running
    if (!rtc.isrunning()) {
        Serial.println("RTC is not running, setting to compile time.");
        setTimeToCompileTime();
    } else {
        Serial.println("RTC is running.");
    }

}
void myClock::setTimeToCompileTime(void) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}
fullTime myClock::currentTime(void){
    DateTime now = rtc.now();
    fullTime currentTime;
    currentTime.hour = now.hour();
    currentTime.minute = now.minute();
    currentTime.second = now.second();

    return currentTime; 
}

fullDate myClock::currentDate(void){
    DateTime now = rtc.now();
    fullDate today;
    today.day = now.day();
    today.month = now.month();
    today.year = now.year()-2000; 
    return today;
}

const char* myClock::currentDay(void){
    DateTime now = rtc.now();
    char daysOfWeek[7][12] = {
        "Sun",
        "Mon",
        "Tue",
        "Wed",
        "Thu",
        "Fri",
        "Sat"
    };

    return daysOfWeek[now.dayOfTheWeek()];
}
