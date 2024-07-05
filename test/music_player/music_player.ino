#include "uart_player.h"

uart_player musicPlayer;

int count = 0;
void setup() {
    Serial.begin(9600); 
}

void loop() {
  // put your main code here, to run repeatedly:
  
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

  
//#include "uart_player.h"
// 
//uart_player myPlayer;
//
//void setup() {
//    Serial.begin(115200);  
//    delay(1000); // Allow some time for initialization 
//    
//    // Example: Command to play music
//    playMusic();
//    delay(1000); // Adjust delay as necessary to allow module to process command
//}
//
//void loop() { 
//}
//
//void playMusic() {
//  ////myPlayer.music_player__play();
//  //myPlayer.music_player__volume(1000);
//  myPlayer.music_player__song(7);
//  delay(2000); // Adjust delay as necessary to allow module to process command 
//}
