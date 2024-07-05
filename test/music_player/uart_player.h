#ifndef UART_PLAYER_H
#define UART_PLAYER_H

#include <stdint.h>
#include <stdio.h>
#include <HardwareSerial.h>

class uart_player
{ 
  private:
    uint8_t music_player__checksum(uint8_t *uart_commad, uint8_t length);
    void music_player__uart__polled_put(uint8_t *uart_commad, uint8_t length);
     

  public: 
    uart_player(); 

    void music_player__loop_mode(uint8_t mode);

    void music_player__play(void);
    void music_player__song(uint16_t number);
    void music_player__stop(void);
    void music_player__volume(uint16_t volume); 
    
};

#endif
