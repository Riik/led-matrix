#include <SPI.h>

uint8_t data = 0x01;
  
void setup() {
  SPI.begin();
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
}

void writeAndToggle(uint8_t data){
  SPI.transfer(data);
  digitalWrite(8, LOW);
  delay(10);
  digitalWrite(8, HIGH);
}

const uint8_t datas[] = {0x00, 0x01, 0x03, 0x06, 0x0C, 0x08, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00};

void loop() {
  for(int i =0; i < 12; i++){
    writeAndToggle(datas[i]);
    delay(10);
  }
  for(int i = 10; i >= 0; i--){
    writeAndToggle(datas[i]);
    delay(10);
  }
  delay(500);
  writeAndToggle(0xFF);
  delay(2000);a
}
