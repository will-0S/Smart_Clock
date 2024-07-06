// 
//#include "clock.h"
//#include "uart_player.h"
//
//uart_player musicPlayer;
//myClock thisClock;
//
//bool alarmStatus = false;
//bool buttonStatus = false;
//
//void buttonInterrupt() { //up
//  buttonStatus = true;
//}
//
//void alarm(int setHour, int setMinute, bool status){
//  fullTime currentTime = thisClock.currentTime();  // Move these inside the function
////  Serial.print(currentTime.hour);
////  Serial.print(" ");
////  Serial.println(setHour);
////  Serial.print(currentTime.minute );
////  Serial.print( " " );
////  Serial.println( setMinute);
//  if(setHour == currentTime.hour && setMinute == currentTime.minute && status == true){ 
//    alarmStatus = true;
//  }
//
//  if(alarmStatus == false){
//    Serial.println( "turning of the alarm");
//    musicPlayer.music_player__stop();
//  }else{
//    musicPlayer.music_player__volume(255);
//    musicPlayer.music_player__song(2);
//    alarmStatus = true;
//  }
//}
//
//void setup() {
//  Serial.begin(9600); 
//  thisClock.clockInitialize();
//  pinMode(2, INPUT_PULLUP); // Enable internal pull-up resistor
//  attachInterrupt(digitalPinToInterrupt(2), buttonInterrupt, FALLING);
//  delay(1000);
//}
//
//void loop() { 
//  fullTime currentTime = thisClock.currentTime();  // Move these inside the function
//  fullDate currentDate = thisClock.currentDate();  // Move these inside the function
//  int min = currentTime.minute;
//  int hou = currentTime.hour;
//  int sec = currentTime.second;
//  int today = currentDate.day;
//  int mon = currentDate.month;
//  int yea = currentDate.year; 
//  
//  Serial.print("ESP32 RTC Date Time: ");
//  Serial.print(mon);
//  Serial.print('/');
//  Serial.print(today);
//  Serial.print('/');
//  Serial.print(yea);
//  Serial.print(" (");
//  Serial.print(thisClock.currentDay());
//  Serial.print(") ");
//  Serial.print(hou);
//  Serial.print(':');
//  Serial.print(min);
//  Serial.print(':');
//  Serial.println(sec);
//
//  
//  if(buttonStatus == true){
//    
//    alarmStatus = false;  //  
//    buttonStatus = false;
//  } else {
//     
//    alarm(14, 0, true); 
//     
//  }
//
//   delay(3000);
//}
#include "clock.h"
#include "uart_player.h"

uart_player musicPlayer;
myClock thisClock;

bool alarmStatus = false;
bool buttonStatus = false;

void buttonInterrupt() { //up
  buttonStatus = true;
}

void alarm(int setHour, int setMinute, bool status) {
  fullTime currentTime = thisClock.currentTime();
  if (setHour == currentTime.hour && setMinute == currentTime.minute && status == true) {
    musicPlayer.music_player__song(2);
    alarmStatus = true;
  }

  if (alarmStatus == false) {
    musicPlayer.music_player__stop();
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
    alarmStatus = false;  
    buttonStatus = false;
  } else {
    alarm(3, 0, true); 
    alarmStatus = false;
  }

  fullTime currentTime = thisClock.currentTime();
  fullDate currentDate = thisClock.currentDate();
  
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
