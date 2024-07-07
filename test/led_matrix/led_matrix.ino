#include "SPI.h"
#include "font.h"
#include "lcd8x8rgb.h"
#include "clock.h"

unsigned long startTime;  // to store the start time
unsigned char timeString[5]; 
myClock thisClock;

void setup() {
   Serial.begin(9600);
  thisClock.clockInitialize();

   SPI.setDataMode(SPI_MODE1);
   SPI.setClockDivider(SPI_CLOCK_DIV64);
   SPI.begin();

   pinMode(chipSelectPin, OUTPUT);
   vClear();
}

void loop() {

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

  unsigned long currTime = millis();
  unsigned long elapsedMillis = currTime - startTime;

  // Calculate time variables
  unsigned long elapsedSeconds = elapsedMillis / 1000;
  unsigned int minutes = elapsedSeconds / 60;
  unsigned int seconds = elapsedSeconds % 60;

  // Move cursor to bottom left corner (x = 0, y = 1)
  cX = 0;
  cY = 1;

  // Convert numbers to characters
  timeString[0] = '0' + (currentTime.minute/ 10);  
  timeString[1] = '0' + (currentTime.minute % 10);  
  timeString[2] = '0' + (currentTime.second / 10);  
  timeString[3] = '0' + (currentTime.second % 10);  

  // Draw string on LED matrix
  drawString(timeString);
  Transfer();

  // Update every second
  delay(1000);
  vClear();
}
