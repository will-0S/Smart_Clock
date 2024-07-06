#include "SPI.h"
#include "font.h"
#include "lcd8x8rgb.h"

unsigned long startTime;  // to store the start time
char timeString[5]; 

void setup() {
   Serial.begin(9600);

   SPI.setDataMode(SPI_MODE1);
   SPI.setClockDivider(SPI_CLOCK_DIV64);
   SPI.begin();

   pinMode(chipSelectPin, OUTPUT);
   vClear();
}

void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsedMillis = currentTime - startTime;

  // Calculate time variables
  unsigned long elapsedSeconds = elapsedMillis / 1000;
  unsigned int minutes = elapsedSeconds / 60;
  unsigned int seconds = elapsedSeconds % 60;

  // Move cursor to bottom left corner (x = 0, y = 1)
  cX = 0;
  cY = 1;

  // Convert numbers to characters
  timeString[0] = '0' + (minutes / 10);  
  timeString[1] = '0' + (minutes % 10);  
  timeString[2] = '0' + (seconds / 10);  
  timeString[3] = '0' + (seconds % 10);  

  // Draw string on LED matrix
  drawString(timeString);
  Transfer();

  // Update every second
  delay(1000);
  vClear();
}
