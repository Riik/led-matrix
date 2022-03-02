#include <SPI.h>

#define LOAD  8
#define CLK   13
#define DATA  11

const uint8_t data[8] = {0xe1,0x1a,0x44,0x2a,0x12,0x29,0xc5,0x41};

void setup() {
  pinMode(LOAD, OUTPUT);
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
}


void sendWord(uint8_t address, uint8_t value) {
  digitalWrite(LOAD, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(LOAD, HIGH);
}

void loop() {
  for (uint8_t row = 0; row < 8; row++) {
    for (uint8_t col = 0; col < 8; col++) {
      sendWord(col, data[row]);
    }
    delay(100);
  }
}


void setIntensity(uint8_t intensity) {
  const uint8_t address = 0x0A;
  intensity &= 0x0F;
  sendWord(address, intensity);
}

void setTestMode(uint8_t testMode) {
  const uint8_t address = 0x0F;
  testMode &= 0x01;
  sendWord(address, testMode);
}
