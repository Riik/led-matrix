#include <SPI.h>
#include <avr/eeprom.h>.
#include "font.cpp"
#define FRAME_LENGTH_BYTES 8

#define S0    A0            // PORTC0
#define S1    A1            // PORTC1
#define S2    A2            // PORTC2
#define R_CLK A3            // PORTC3
#define R_CLK_SET (1 << 3)
#define N_PANELS 1
#define N_FRAMES (N_PANELS + 2) // margin both left and right
#define FRAME_REPEAT_COUNT 6

SPISettings settingsA(16000, LSBFIRST, SPI_MODE0);
uint8_t screen_buff[N_FRAMES*FRAME_LENGTH_BYTES];
char display_message[] = "Dag amice Dirks, hoe maakt u het?";

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

void load_font_character(const uint8_t chr_idx, uint8_t panel_offset) {
  const uint8_t* chr_offset = font + FRAME_LENGTH_BYTES * chr_idx;
  memcpy_P(&screen_buff[panel_offset*FRAME_LENGTH_BYTES], chr_offset, FRAME_LENGTH_BYTES);
}

inline void set_address_pins_and_toggle(uint8_t new_selection) {
  new_selection &= 0x07; // make sure only 3 LSB are set
  uint8_t cleared_port = PORTC & 0xF8; // keep all but 3 LSB
  PORTC = cleared_port | new_selection | R_CLK_SET; // set 3 address pins and R_CLK
  PORTC &= ~R_CLK_SET;
}

void shift_screen_buff_right(){
  for(int j = 0; j < FRAME_LENGTH_BYTES; j++){
    screen_buff[j] >>= 1;
    for(int i = 1; i < N_FRAMES; i++){
      uint8_t offset = i*FRAME_LENGTH_BYTES;
      screen_buff[offset+j] = (screen_buff[offset+j-FRAME_LENGTH_BYTES] << 7) | (screen_buff[offset+j] >> 1);
    }
  }
}

void shift_screen_buff_left(){
  for(int j = 0; j < FRAME_LENGTH_BYTES; j++){
    for(int i = 0; i < N_FRAMES-1; i++){
      uint8_t offset = i*FRAME_LENGTH_BYTES;
      screen_buff[offset+j] = (screen_buff[offset+j] << 1) | (screen_buff[offset+j+FRAME_LENGTH_BYTES] >> 7);
    }
    screen_buff[j+(N_FRAMES-1)*FRAME_LENGTH_BYTES] <<= 1;
  }
}

void loop() {
  for (int m = 0; m < sizeof(display_message); m++) {
    load_font_character(display_message[m], 2);
    for (int k = 0; k < 8; k++) {
      shift_screen_buff_left();
      for (int j = 0; j < FRAME_REPEAT_COUNT; j++) {
        for (int i = 0; i < 8; ++i) {
          SPI.transfer(screen_buff[i+FRAME_LENGTH_BYTES]);
          set_address_pins_and_toggle(i);
          delay(1);
        }
      }
    }
  }
}
