# rpi-led-matrix
Python / C control for a LED matrix

# Goal
Create the software for a hardware project which can drive a LED matrix with a extendable number of rows and columns

# Hardware
To drive the rows, the MIC2981 driver IC is used. It can source up to 500mA per pin, which mean 25 columns @ 20mA / column.
To sink the columns, the TPIC6C595 is used. I needs to be able to sink one column (20mA) per pin, so for an 8 bit shift register the total IC must be able to sink 8*20mA. The TPIC6C595 is able to sink up to 100mA per pin.
The rows should multiplexed, i.e. they should not be one at the same time.

# Software
Suitable candidates:
  - PIGpio Python library using DMA control http://abyz.me.uk/rpi/pigpio/
  - Directly talking to SPI hardware in C using IOCTL

# Notes to selfs
  - If multiple rows are active, current is split / LEDS are in parallel with respect to the sinking column.
  - We probably need a resistor per sink / column 
