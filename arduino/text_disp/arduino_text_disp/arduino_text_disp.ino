#include <SPI.h>
#include <avr/eeprom.h>.
#include "font.cpp"
#define CHAR_LENGTH_BYTES 8

#define S0 A0    // PORTC0
#define S1 A1    // PORTC1
#define S2 A2    // PORTC2
#define R_CLK A3 // PORTC3
#define R_CLK_SET (1 << 3)
#define N_PANELS 1

SPISettings settingsA(16000, LSBFIRST, SPI_MODE0);
uint8_t screen_buff[8];
static int pinout = 9;
void setup() {
  SPI.begin();
  SPI.beginTransaction(settingsA);
  pinMode(R_CLK, OUTPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  digitalWrite(R_CLK, LOW);
  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);
}

void load_font_character(const uint8_t chr_idx){
  const uint8_t* chr_offset = font+CHAR_LENGTH_BYTES*chr_idx;
  memcpy_P(screen_buff, chr_offset, CHAR_LENGTH_BYTES);
}

inline void set_address_pins_and_toggle(uint8_t new_selection){
  new_selection &= 0x07; // make sure only 3 LSB are set
  uint8_t cleared_port = PORTC & 0xF8; // keep all but 3 LSB
  PORTC = cleared_port | new_selection | R_CLK_SET; // set 3 address pins and R_CLK
  PORTC &= ~R_CLK_SET;
}

void loop() {
  for(int k = 64; k < 256; k++){
    load_font_character(k);
    for(int j = 0; j < 200; j++){
      for (int i = 0; i < 8; ++i) {
        SPI.transfer(screen_buff[i]);
        set_address_pins_and_toggle(i);
        delay(1);
      }
    }
  }
}
