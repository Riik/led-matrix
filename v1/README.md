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

# On the subject of the LED matrix 3D model
For this model, we use two programs: OpenSCAD and FreeCAD. OpenSCAD is used to program the model. To make the model readable for KiCad:
1. Open FreeCAD
2. Select View->Workbench->OpenSCAD
3. Open the *.scad file (File->Open)
4. In Combo View->Model->Labels & Attributes->2088BS select union
5. Go to File->Export
6. Export the model as a STEP with colors (*.step *.stp)

# Selected components
 - 100 Ohm 1206 [3216 Metric]: Vishay CRCW1206100RFKEA (https://nl.farnell.com/vishay/crcw1206100rfkea/res-100r-1-0-25w-1206-thick-film/dp/1469977,
 - 10 kOhm 1206 [3216 Metric]: Vishay CRCW120610K0FKEA (https://nl.farnell.com/vishay/crcw120610k0fkea/res-10k-1-0-25w-1206-thick-film/dp/1469970RL,
 - 100 nF (0.1 uF) 1206 [3216 Metric] capacitor: Vishay VJ1206Y104KXQCW1BC (https://nl.farnell.com/vishay/vj1206y104kxqcw1bc/cap-0-1-f-10v-10-x7r-1206/dp/2896644RL,
 - Pin header male: https://nl.farnell.com/multicomp/mc34753/header-1-row-r-angle-8way/dp/1593431?ost=mc34753,
 - Pin header female: https://nl.farnell.com/samtec/ssw-108-02-g-s-ra/receptacle-2-54mm-r-a-single-8way/dp/1668368,
