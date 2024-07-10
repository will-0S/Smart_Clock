#include "SPI.h"
#include "font.h"
#include "lcd8x8rgb.h"
#include "clock.h"
#include "uart_player.h"
 #include <BluetoothSerial.h>

 
//Create a BluetoothSerial object
BluetoothSerial SerialBT;
uart_player musicPlayer;
myClock thisClock;

unsigned long startTime;  // to store the start time
unsigned char timeString[5]; 
 
bool playAlarmSound = false;
bool isAlarmTurnedOn = false;

bool buttonStatus = false;
bool snoozeButtonStatus = false;
bool alarmState = false;
int alarmHour = 18;
int alarmMinute = 28;  
int tempAlarmHour = alarmHour;
int tempAlarmMinute = alarmMinute;
int musicPlayerPin = 4; 


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
      Serial.println("alarm activated");
      musicPlayer.music_player__song(2);
      isAlarmTurnedOn = true; 
      playAlarmSound = true; 
    }
    
    if(playAlarmSound){
      Serial.println("alarm playing");
      musicPlayer.music_player__song(2);
    }else{
      if(setHour != currentTime.hour && setMinute != currentTime.minute){
        isAlarmTurnedOn = false;  
      }
      
      musicPlayer.music_player__stop();
    }
  }
 
}
void parseTime(String timeStr, int &hour, int &minute) {
  // Find the index of the colon
  int colonIndex = timeStr.indexOf(':');
  
  // Extract the hour and minute substrings
  String hourStr = timeStr.substring(0, colonIndex);
  String minuteStr = timeStr.substring(colonIndex + 1);
  
  // Convert the substrings to integers
  hour = hourStr.toInt();
  minute = minuteStr.toInt();
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
    
//Start Bluetooth serial communication with the device name "ESP32"
  SerialBT.begin("SMART_COCK"); 
  Serial.println("The device started, now you can pair it with Bluetooth!");
  
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
    alarmMinute = currentTime.minute + 5;
    if(alarmMinute > 60){
      alarmMinute -= 60;
      alarmHour++; 
    } 
    alarmHour = alarmHour%24; 
  }else{ 
    Serial.print("set = ");
    Serial.print(alarmState);
    Serial.print(" set time ");
    Serial.print(alarmHour);
    Serial.print(':');
    Serial.println(alarmMinute);
    alarm(alarmHour, alarmMinute, alarmState);  
  }
   if (SerialBT.available()) {
     // Read the incoming message
     String incomingMessage = SerialBT.readString();
    playAlarmSound = false;  
    buttonStatus = false; 
    snoozeButtonStatus = false; 
    isAlarmTurnedOn = false;  
     // Print the incoming message to the serial monitor
     Serial.print("Received: ");
     Serial.println(incomingMessage);
 
    // Check if the incoming message is "hello"
    incomingMessage.trim(); // Remove any leading/trailing whitespace or newline characters
//do a if where the message can only be in the format of hour ":" minute
    parseTime(incomingMessage, alarmHour,alarmMinute );
    tempAlarmHour = alarmHour;//the new set time becames the original alarm time
    tempAlarmMinute = alarmMinute;
    //have another if, where it sends set off or set on, maybe we can do a function where it sets the days and if its not that day dont set the alarm
    alarmState = true;
    Serial.println(incomingMessage);
    
   }
 
  
  //for debuging process can erased later
//  Serial.print("ESP32 RTC Date Time: ");
//  Serial.print(currentDate.year + 2000); // Adjust year back to full year format
//  Serial.print('/');
//  Serial.print(currentDate.month);
//  Serial.print('/');
//  Serial.print(currentDate.day);
//  Serial.print(" (");
//  Serial.print(thisClock.currentDay());
//  Serial.print(") ");
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
