#include "SPI.h"
#include "font.h"
#include "lcd8x8rgb.h"
#include "clock.h"
#include "uart_player.h"


uart_player musicPlayer;
myClock thisClock;

unsigned long startTime;  // to store the start time
unsigned char timeString[5]; 
 
bool playAlarmSound = false;
bool isAlarmTurnedOn = false;

bool buttonStatus = false;
bool snoozeButtonStatus = false;
int alarmHour = 18;
int alarmMinute = 28;  
int tempAlarmHour = alarmHour;
int tempAlarmMinute = alarmMinute;

void buttonInterrupt() { //up
  buttonStatus = true;
}
void buttonSnooze(){
  snoozeButtonStatus = true;
}

void alarm(int setHour, int setMinute, bool setAlarm) {
  fullTime currentTime = thisClock.currentTime();
  
  if(setAlarm){
    if (setHour == currentTime.hour && setMinute == currentTime.minute && !isAlarmTurnedOn) {
      musicPlayer.music_player__song(2);
      isAlarmTurnedOn = true; 
      playAlarmSound = true; 
    }
    
    if(playAlarmSound){
      musicPlayer.music_player__song(2);
    }else{
      if(setHour != currentTime.hour && setMinute != currentTime.minute){
        isAlarmTurnedOn = false;  
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
  pinMode(15, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(15), buttonSnooze, FALLING);  
   
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV64);
  SPI.begin();
  pinMode(chipSelectPin, OUTPUT);
  vClear();   

  delay(1000);
}




void loop() { 
  fullTime currentTime = thisClock.currentTime();
  fullDate currentDate = thisClock.currentDate();
  
  if (buttonStatus && playAlarmSound) {
    playAlarmSound = false;  
    buttonStatus = false; 
    alarmHour = tempAlarmHour;
    alarmMinute = tempAlarmMinute; 
  } else if(snoozeButtonStatus && playAlarmSound){
    playAlarmSound = false;  
    snoozeButtonStatus = false; 
    isAlarmTurnedOn = false;  
    alarmMinute = currentTime.minute + 1;
    if(alarmMinute > 60){
      alarmMinute -= 60;
      alarmHour++; 
    } 
    alarmHour = alarmHour%24; 
  }else{
    Serial.print("set time ");
    Serial.print(alarmHour);
    Serial.print(':');
    Serial.println(alarmMinute);
    alarm(alarmHour, alarmMinute, true);  
  }

 
  
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



  // Move cursor to bottom left corner (x = 0, y = 1)
  cX = 0;
  cY = 1;

  // Convert numbers to characters
  timeString[0] = '0' + (currentTime.hour/ 10);  
  timeString[1] = '0' + (currentTime.hour % 10);  
  timeString[2] = '0' + (currentTime.minute / 10);  
  timeString[3] = '0' + (currentTime.minute % 10);  

  // Draw string on LED matrix
  drawString(timeString);
  Transfer();

  // Update every second
  delay(1000);
  vClear();
}
