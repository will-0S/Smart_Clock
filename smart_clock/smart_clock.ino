#include "SPI.h"
#include "font.h"
#include "lcd8x8rgb.h"
#include "clock.h"
#include "uart_player.h"
#include <BluetoothSerial.h> 
#include <ArduinoJson.h>

 

BluetoothSerial SerialBT;
uart_player musicPlayer;
myClock thisClock;

//LED timming  
unsigned char timeString[5]; 

//starts the alarm sound and keeps playing until turned off
bool playAlarmSound = false;
bool isAlarmTurnedOn = false;

//turn off/motion sensor buttonstatus and snooze button
bool buttonStatus = false;
bool snoozeButtonStatus = false;

//activate alarm
bool alarmState = false;

//temp keeps time of original time after its hit snooze
int alarmHour = 0;
int alarmMinute = 0;  
int tempAlarmHour = alarmHour;
int tempAlarmMinute = alarmMinute; 

int count = 0;

String daysOfWeekJson = "{\"days\": [\"empty\"]}";

void buttonInterrupt() { //turn off interrupt
  buttonStatus = true;
}
void buttonSnooze(){ //snooze interrupt
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
  String hourStr = timeStr.substring(0, colonIndex);//maybe change to colonIndex - 2, colonIndex
  String minuteStr = timeStr.substring(colonIndex + 1);
  
  // Convert the substrings to integers
  hour = hourStr.toInt();
  minute = minuteStr.toInt();
}
/*
 * days of the week code logic
 * if there are no days with the time, there will be a calculation, if the time is later on the day, the day will be set to today, 
 * if the time is earlier then now then the day will be set to tommorow.
 * 
 * if the day that the alarm is suppose to sound is today then the setbool will be set to true and that should be the only place where is set
 * therefore this function should be call right before alarm all the  time
 * 
 * essentially it could be call setting alarm function
 * 
 * so there should be a for loop that goes through the json check if today day is on the json string if its not then set off, if its on then set true
 * 
 * so maybe we should have a global json for time and global json for day
 */
void updateDaysAndTime(const char* newJson) {
StaticJsonDocument<200> newDoc;
  DeserializationError error = deserializeJson(newDoc, newJson); 
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  JsonObject time = newDoc["time"];
  alarmHour = time["hours"];
  alarmMinute = time["minutes"];
  tempAlarmHour = alarmHour;
  tempAlarmMinute = alarmMinute; 

  JsonArray newDays = newDoc["day"];
  StaticJsonDocument<200> updatedDoc;
  JsonArray updatedDays = updatedDoc.createNestedArray("days");

  for (JsonVariant value : newDays) {
    updatedDays.add(value);
  }

  String updatedJson;
  serializeJson(updatedDoc, updatedJson);
  daysOfWeekJson = updatedJson;  // Use String to store the updated JSON

  Serial.println("Updated days of the week:");
  Serial.println(daysOfWeekJson);
}




bool isDayPresent(const char* dayToCheck) {
  // Deserialize the updated global JSON string
   
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, daysOfWeekJson);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return false;
  }
  JsonArray days = doc["days"];

  // Iterate through the days array to check for the specific day
  for (JsonVariant day : days) { 
    if (dayToCheck == day) {
      return true;
    }
  }
  return false;
}

void setup() {
  Serial.begin(9600); 

  thisClock.clockInitialize();

  //set up for motion sensor interrupt set up
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), buttonInterrupt, RISING);

  //turn of button interrupt set up
  //pinMode(2, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(2), buttonInterrupt, FALLING); 

  //turn of snooze interrupt set up
  pinMode(15, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(15), buttonSnooze, FALLING);  

  //LED matrix set up
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV64);
  SPI.begin();
  pinMode(chipSelectPin, OUTPUT);
  vClear(); 
    
  //Start Bluetooth serial communication with the device name "ESP32"
  SerialBT.begin("SMART_COCK"); 
  Serial.println("The device started, now you can pair it with Bluetooth!");
  
  delay(100);
}




void loop() { 
  //start the time and data objects
  fullTime currentTime = thisClock.currentTime();
  fullDate currentDate = thisClock.currentDate();


  //time
  static unsigned long lastSendTime = 0; // To keep track of the last time data was sent
  unsigned long currentSendTime = millis();  
  
  if (buttonStatus && playAlarmSound) {//turn off alarm
    playAlarmSound = false;  
    buttonStatus = false; 
    alarmHour = tempAlarmHour;
    alarmMinute = tempAlarmMinute; 
  } else if(snoozeButtonStatus && playAlarmSound){//activate snooze + 5 minutes
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
    //call alarm
    Serial.print("set = ");
    Serial.print(alarmState);
    Serial.print(" set time ");
    Serial.print(alarmHour);
    Serial.print(':');
    Serial.println(alarmMinute);

     
 
    if(isDayPresent(thisClock.currentDay())){
      Serial.println("inside");
      alarmState = true;   
    }else{
      Serial.println("outside");
      alarmState = false;  
    }
     
    alarm(alarmHour, alarmMinute, alarmState); 
  }


  if (SerialBT.available()) {
    // Read the incoming message
    String incomingMessage = SerialBT.readString();
     
    
    //update status
    playAlarmSound = false;  
    buttonStatus = false; 
    snoozeButtonStatus = false; 
    isAlarmTurnedOn = false;  

    
    
    // Print the incoming message to the serial monitor
    Serial.print("Received: ");
    Serial.println(incomingMessage);
    
    updateDaysAndTime(incomingMessage.c_str());
    //alarmState = true;  
      
//    incomingMessage.trim(); // Remove any leading/trailing whitespace or newline characters
//    parseTime(incomingMessage, alarmHour, alarmMinute);
//    tempAlarmHour = alarmHour;//the new set time becames the original alarm time
//    tempAlarmMinute = alarmMinute;
//    Serial.println(incomingMessage);
//    SerialBT.println("got the data");
  }

 
  // Send data at regular intervals (e.g., every 2 seconds)
    //String daysOfWeekJson = 
   if (currentSendTime - lastSendTime >= 1000) {
    StaticJsonDocument<200> temp;
    temp["temp"] = count;
    String dataToSend;
    serializeJson(temp, dataToSend); 
    SerialBT.println(dataToSend);
    Serial.print("Sent: ");
    Serial.println(dataToSend);    
     lastSendTime = currentSendTime; 
    count++;
    if(count > 100){
     count = 0;
    }
  }




 
  
 
  
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
