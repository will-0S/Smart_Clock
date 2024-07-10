#include "uart_player.h" 

HardwareSerial myUart(1);

uart_player::uart_player() 
{
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  myUart.begin(9600, SERIAL_8N1, 16, 17);
  delay(100); 
}

uint8_t uart_player::music_player__checksum(uint8_t *uart_commad, uint8_t length) {
    uint8_t sum = 0;
    for (uint8_t i = 0; i < length; i++) {
        sum = sum + uart_commad[i];
    }
    return sum;
}

void uart_player::music_player__uart__polled_put(uint8_t *uart_commad, uint8_t length) {
    uint8_t checksum = music_player__checksum(uart_commad, length); 
    myUart.write(uart_commad, length);   
    myUart.write(&checksum, 1);

    // Debug prints
    Serial.print("Sent Command: ");
    for (uint8_t i = 0; i < length; i++) {
        Serial.print(uart_commad[i], HEX);
        Serial.print(" ");
    }
    Serial.print("Checksum: ");
    Serial.println(checksum, HEX);
}

void uart_player::music_player__loop_mode(uint8_t mode){
    uint8_t uart_commad[4] = {0xaa, 0x18, 0x01, 0x00};
    uart_commad[3] = mode; 
    music_player__uart__polled_put(uart_commad, 4);
}

void uart_player::music_player__play(void){
    uint8_t uart_commad[4] = {0xaa, 0x02, 0x00, 0xac};

    for (int i = 0; i < 4; i++) { 
        myUart.write(uart_commad, sizeof(uart_commad));
    }

    // Debug prints
    Serial.print("Sent Play Command: ");
    for (uint8_t i = 0; i < sizeof(uart_commad); i++) {
        Serial.print(uart_commad[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

void uart_player::music_player__song(uint16_t number){
    uint8_t uart_commad[5] = {0xaa, 0x07, 0x02, 0x00, 0x00}; 
    uart_commad[4] = number & 0xFF;

    music_player__uart__polled_put(uart_commad, 5);
} 

void uart_player::music_player__stop(void){
    uint8_t uart_commad[4] = {0xaa, 0x04, 0x00, 0xae};

    music_player__uart__polled_put(uart_commad, 4);
}

void uart_player::music_player__volume(uint16_t volume){
    uint8_t uart_commad[4] = {0xaa, 0x13, 0x01, 0x00};
    uart_commad[3] = volume;

    music_player__uart__polled_put(uart_commad, 4);
}
