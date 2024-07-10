#include "uart_player.h"

uart_player musicPlayer;
int musicPlayerPin = 4; 


int count = 0;
void setup() {
    Serial.begin(9600); 
    pinMode(musicPlayerPin, OUTPUT);
}

void loop() { 
   musicPlayer.music_player__volume(255);
   musicPlayer.music_player__song(4);
 
 
  //digitalWrite(musicPlayerPin, LOW);
  delay(3000);
  //digitalWrite(musicPlayerPin, HIGH);
   musicPlayer.music_player__stop();
  delay(3000);
}
 
