#include <SPI.h>
#include "font.cpp"
#include <avr/eeprom.h>

#define LOAD  8  // can be changed
#define CLK   13 // fixed by hardware SPI 
#define DATA  11 // fixed by hardware SPI

#define ADDR_INTENSITY  0x0A
#define ADDR_SCAN_LIMIT 0x0B
#define ADDR_SHUTDOWN   0x0C
#define ADDR_TESTMODE   0x0F

#define N_PANELS 8
#define BUFFER_LEN (N_PANELS+1)*8

#define TEXT_BUFF_SIZE 80

#define SPI_SPEED 10000

uint8_t frame_buffer[BUFFER_LEN];
SPISettings spiSettings(SPI_SPEED, MSBFIRST, SPI_MODE0);

char message_receive_buff[TEXT_BUFF_SIZE];
char message_display_buff[TEXT_BUFF_SIZE] = "Hello world!";
uint8_t input_char_count = 0;
uint8_t message_size = 15;
bool string_complete = false; 

void setRegister(uint8_t address, uint8_t value) {    
  digitalWrite(LOAD, LOW);
  for(uint8_t n = 0; n < N_PANELS; n++){
    SPI.transfer(address);
    SPI.transfer(value);
  }
  digitalWrite(LOAD, HIGH);
  delay(1);
}

void serialEvent() {
  if(input_char_count >= TEXT_BUFF_SIZE){
    string_complete = true;
    return;
  }
  while (Serial.available()) {
    input_char_count++;
    char in_char = (char)Serial.read();
    if (in_char == '\n') {
      string_complete = true;
      return;
    }
    message_receive_buff[input_char_count] = in_char;
  }
}

void sendBuffer() {
    for (uint8_t col = 0; col < 8; col++) {
      digitalWrite(LOAD, LOW);
      for(uint8_t n = 0; n < N_PANELS; n++){
        SPI.transfer(col+1);
        SPI.transfer(frame_buffer[col + (N_PANELS-1-n)*8]);
      }
      digitalWrite(LOAD, HIGH);
      delay(1);
    }
}

void moveData(){
  memmove(frame_buffer,frame_buffer+1, BUFFER_LEN-1);
}

void loadData(uint8_t char_idx){
  const uint8_t* offset = font+char_idx*8;
  memcpy_P(frame_buffer, offset, (size_t) BUFFER_LEN);
}

void loadNewCharacter(uint8_t char_idx){
  const uint8_t* offset = font+char_idx*8;
  memcpy_P(frame_buffer+N_PANELS*8, offset, (size_t) 8);
}

void setup() {
  pinMode(LOAD, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DATA, OUTPUT);

  SPI.begin();
  SPI.beginTransaction(spiSettings);

  setRegister(ADDR_TESTMODE, 0x00);
  setRegister(ADDR_INTENSITY, 0x01);
  setRegister(ADDR_SHUTDOWN, 0x01);
//  setRegister(ADDR_SCAN_LIMIT, 0x07);
  setRegister(ADDR_SCAN_LIMIT, 0x07);
  
  Serial.begin(115200);
  Serial.println("hi");

}

void loop() {
  for(uint8_t i = 0; i < message_size; i++){
    if(string_complete){
      memcpy(message_display_buff, message_receive_buff, TEXT_BUFF_SIZE);
      message_size = input_char_count;
      input_char_count = 0;
      string_complete = false;
      break;
    }
    loadNewCharacter(message_display_buff[i]);   
    for(uint8_t col = 0; col < 8; col++){
      moveData(); 
      sendBuffer();
      delay(35);
    }
    delay(100);
  }

}
