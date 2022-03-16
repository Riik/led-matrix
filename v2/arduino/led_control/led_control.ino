#include <SPI.h>

#define LOAD  8
#define CLK   13
#define DATA  11

#define ADDR_INTENSITY  0x0A
#define ADDR_SCAN_LIMIT 0x0B
#define ADDR_SHUTDOWN   0x0C

const uint8_t data[8] = {0xe1,0x1a,0x44,0x2a,0x12,0x29,0xc5,0x41};

uint8_t convert_data(uint8_t data){
  return (data >> 1) | (data << 7);
}

void sendWord(uint8_t address, uint8_t value) {    
  digitalWrite(LOAD, LOW);
  SPI.beginTransaction(SPISettings(100000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(address);
  SPI.transfer(value);
  SPI.endTransaction();
  digitalWrite(LOAD, HIGH);
}


void setup() {
  pinMode(LOAD, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  sendWord(ADDR_INTENSITY, 0x01);
  sendWord(ADDR_SHUTDOWN, 0x01);
  sendWord(ADDR_SCAN_LIMIT, 0x07);
}

void loop() {
  for (uint8_t col = 0; col < 8; col++) {
    sendWord(col+1, convert_data(data[col]));
  }
  delay(100);
}
