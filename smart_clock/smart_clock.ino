
#include "clock.h"
#include "uart_player.h"

uart_player musicPlayer;
myClock thisClock;

bool alarmStatus = false;
bool buttonStatus = false;

void buttonInterrupt() { //up
  buttonStatus = true;
}

bool stopAlarm = false;
bool stopMusic = true;

void alarm(int setHour, int setMinute, bool setAlarm) {
  fullTime currentTime = thisClock.currentTime();
  
  if(setAlarm){
    if (setHour == currentTime.hour && setMinute == currentTime.minute && !stopAlarm) {
      musicPlayer.music_player__song(2);
      stopAlarm = true; 
      stopMusic = false;
    }
    
    if(!stopMusic){
      musicPlayer.music_player__song(2);
    }else{
      if(setHour != currentTime.hour && setMinute != currentTime.minute){
        stopAlarm = false;  
      }
      
      musicPlayer.music_player__stop();
    }
  }
 
}

void setup() {
  Serial.begin(9600); 
  thisClock.clockInitialize();
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), buttonInterrupt, FALLING); 
  delay(1000);
}

void loop() { 
  if (buttonStatus == true) {
    stopMusic = true;  
    buttonStatus = false;
  } else {
    alarm(21, 22, true);  
  }

  fullTime currentTime = thisClock.currentTime();
  fullDate currentDate = thisClock.currentDate();
  //for debuging process can erased later
  Serial.print("ESP32 RTC Date Time: ");
  Serial.print(currentDate.year + 2000); // Adjust year back to full year format
  Serial.print('/');
  Serial.print(currentDate.month);
  Serial.print('/');
  Serial.print(currentDate.day);
  Serial.print(" (");
  Serial.print(thisClock.currentDay());
  Serial.print(") ");
  Serial.print(currentTime.hour);
  Serial.print(':');
  Serial.print(currentTime.minute);
  Serial.print(':');
  Serial.println(currentTime.second);

  delay(2000);
}
