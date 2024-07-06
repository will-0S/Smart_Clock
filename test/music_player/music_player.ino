#include "uart_player.h"

uart_player musicPlayer;

int count = 0;
void setup() {
    Serial.begin(9600); 
}

void loop() { 
  musicPlayer.music_player__volume(255);
  musicPlayer.music_player__song(count);
  count++;

  if(count>9){
    count = 0;
  }

  delay(2000);
  musicPlayer.music_player__stop();
  delay(1000);
}

 