/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-ds1307-rtc-module
 */

#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

RTC_DS1307 rtc;
//Create liquid crystal object called 'lcd'
const int lcdColumns = 20;
const int lcdRows = 4; 

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

char daysOfWeek[7][12] = {
  "Sun",
  "Mon",
  "Tue",
  "Wed",
  "Thu",
  "Fri",
  "Sat"
};

void setup () {
  Serial.begin(9600);
  lcd.init();
  lcd.begin(20, 4);                    
  lcd.backlight();       

  // SETUP RTC MODULE
  if (! rtc.begin()) {
    Serial.println("RTC module is NOT found");
    Serial.flush();
    while (1);
  }

  // automatically sets the RTC to the date & time on PC this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // manually sets the RTC with an explicit date & time, for example to set
  // January 21, 2021 at 3am you would call:
  // rtc.adjust(DateTime(2021, 1, 21, 3, 0, 0));
}

void loop () {
  DateTime now = rtc.now();
  Serial.print("ESP32 RTC Date Time: ");
  Serial.print(now.year()-2000 );
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);
  lcd.setCursor(0,1);
  lcd.print(daysOfWeek[now.dayOfTheWeek()]);  
  lcd.setCursor(5,1);
  lcd.print(now.month() ); 
  lcd.setCursor(7,1);
  lcd.print('/'); 
  lcd.setCursor(8,1);
  lcd.print(now.day() );  
  lcd.setCursor(10,1);
  lcd.print('/'); 
  lcd.setCursor(11,1);
  lcd.print(now.year()-2000); 
  if(now.hour()>12){
    lcd.setCursor(14,1);
    lcd.print(now.hour()-12 );     
  }
  lcd.setCursor(16,1);
  lcd.print(':'); 
  lcd.setCursor(17,1);
  lcd.print(now.minute()); 
  delay(1000); // delay 1 seconds
}
