ARDUINO_PATH='/Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino'
VARIANTS_PATH='/Applications/Arduino.app/Contents/Java/hardware/arduino/avr/variants/standard'
SPI_LIB_PATH='/Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/SPI/src/'
AVRDUDE_CONF='/Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf'
MCU='atmega328p'
F_CPU=16000000
PORT='/dev/cu.usbmodem14101'
UPLOAD_RATE=115200

CORE_SOURCES=\
			 $(ARDUINO_PATH)/hooks.c \
	     $(ARDUINO_PATH)/wiring.c\
	     $(ARDUINO_PATH)/wiring_analog.c\
	     $(ARDUINO_PATH)/wiring_digital.c\
			 $(ARDUINO_PATH)/wiring_pulse.c\
	     $(ARDUINO_PATH)/wiring_pulse.S\
	     $(ARDUINO_PATH)/wiring_shift.c\
	     $(ARDUINO_PATH)/WInterrupts.c\
	     $(ARDUINO_PATH)/HardwareSerial.cpp\
	     $(ARDUINO_PATH)/WMath.cpp\
	     $(ARDUINO_PATH)/WString.cpp\
	     $(ARDUINO_PATH)/Print.cpp\
			 $(SPI_LIB_PATH)/SPI.cpp

SOURCES=main.cpp

all: compile

compile: build/main.hex build/main.eep

upload: build/main.hex build/main.eep
	avrdude -p$(MCU) -P$(PORT) -carduino -C$(AVRDUDE_CONF) -b$(UPLOAD_RATE) -U flash:w:build/main.hex:i

build/main.elf: build *.cpp
	avr-gcc -o build/main.elf -mmcu=$(MCU) -Os -DF_CPU=$(F_CPU) $(CORE_SOURCES) $(SOURCES) -I$(ARDUINO_PATH) -I$(VARIANTS_PATH) -I$(SPI_LIB_PATH)

build/main.hex: build/main.elf
	avr-objcopy -O ihex -R .eeprom build/main.elf build/main.hex

build/main.eep:
	avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc \
	--change-section-lma .eeprom=0 --no-change-warnings build/main.elf build/main.eep

build:
	mkdir build
