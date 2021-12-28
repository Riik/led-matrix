#include <SPI.h>
#include <avr/pgmspace.h>
#include "font.cpp"
#define CHAR_LENGTH_BYTES 8
#define S0 8
#define S1 9
#define S2 10
#define R_CLK 7

extern "C" void __cxa_pure_virtual() { abort(); }

SPISettings settingsA(16000, MSBFIRST, SPI_MODE0);
uint8_t screen_buff[8];
static int pinout = 9;

void load_font_character(const uint8_t chr_idx){
  const uint8_t* chr_offset = font+CHAR_LENGTH_BYTES*chr_idx;
  memcpy_P(screen_buff, chr_offset, CHAR_LENGTH_BYTES);
}

inline void set_address_pins(uint8_t i){
  digitalWrite(S0, i & 0x1 ? HIGH : LOW);
  digitalWrite(S1, i & 0x2 ? HIGH : LOW);
  digitalWrite(S2, i & 0x4 ? HIGH : LOW);
}

inline void toggle_shift_registers(){
  digitalWrite(R_CLK, HIGH);
  digitalWrite(R_CLK, LOW);
}

void setup() {
  SPI.begin();
  SPI.beginTransaction(settingsA);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
}


void loop() {
  for(int k = 64; k < 256; k++){
    load_font_character(k);
    for(int j = 0; j < 100; j++){
      for (int i = 0; i < 8; ++i) {
        SPI.transfer(screen_buff[i]);
        set_address_pins(i);
        toggle_shift_registers();
        delay(1);
      }
    }
  }
}

int main(void){
  setup();
  for(;;) loop();
}
