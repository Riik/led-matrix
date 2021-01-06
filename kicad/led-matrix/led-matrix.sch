EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 1750 1800 0    50   ~ 0
a0
Text Label 1750 1900 0    50   ~ 0
a1
Text Label 1750 2000 0    50   ~ 0
a2
Text Label 1750 2100 0    50   ~ 0
a3
Text Label 1750 2200 0    50   ~ 0
a4
Text Label 1750 2300 0    50   ~ 0
a5
Text Label 1750 2400 0    50   ~ 0
a6
Text Label 1750 2500 0    50   ~ 0
a7
Text Label 2600 1800 0    50   ~ 0
a0
Text Label 2600 1900 0    50   ~ 0
a1
Text Label 2600 2000 0    50   ~ 0
a2
Text Label 2600 2200 0    50   ~ 0
a4
Text Label 2600 2300 0    50   ~ 0
a5
Text Label 2600 2400 0    50   ~ 0
a6
Text Label 2600 2500 0    50   ~ 0
a7
Text Label 4200 1800 0    50   ~ 0
b0
Text Label 4200 1900 0    50   ~ 0
b1
Text Label 4200 2000 0    50   ~ 0
b2
Text Label 4200 2100 0    50   ~ 0
b3
Text Label 4200 2200 0    50   ~ 0
b4
Text Label 4200 2300 0    50   ~ 0
b5
Text Label 4200 2400 0    50   ~ 0
b6
Text Label 4200 2500 0    50   ~ 0
b7
Text Label 2600 2100 0    50   ~ 0
a3
Text Label 4850 1800 0    50   ~ 0
b0
Text Label 4850 1900 0    50   ~ 0
b1
Text Label 4850 2000 0    50   ~ 0
b2
Text Label 4850 2100 0    50   ~ 0
b3
Text Label 4850 2200 0    50   ~ 0
b4
Text Label 4850 2300 0    50   ~ 0
b5
Text Label 4850 2400 0    50   ~ 0
b6
Text Label 4850 2500 0    50   ~ 0
b7
Entry Wire Line
	4750 1900 4850 1800
Entry Wire Line
	4750 2000 4850 1900
Entry Wire Line
	4750 2100 4850 2000
Entry Wire Line
	4750 2200 4850 2100
Entry Wire Line
	4750 2300 4850 2200
Entry Wire Line
	4750 2400 4850 2300
Entry Wire Line
	4750 2500 4850 2400
Entry Wire Line
	4750 2600 4850 2500
Wire Wire Line
	5050 1900 4850 1900
Wire Wire Line
	5050 2100 4850 2100
Wire Wire Line
	5050 2000 4850 2000
Wire Wire Line
	5050 2300 4850 2300
Wire Wire Line
	5050 2500 4850 2500
Wire Wire Line
	5050 2400 4850 2400
Wire Wire Line
	5050 2200 4850 2200
Wire Wire Line
	5050 1800 4850 1800
$Comp
L LED_MAT:DISP_LED_MAT_8x8_COM_ANODE D1
U 1 1 5FD8261F
P 5050 2600
F 0 "D1" H 5725 3687 60  0000 C CNN
F 1 "DISP_LED_MAT_8x8_COM_ANODE" H 5725 3581 60  0000 C CNN
F 2 "led_matrix:LED_8x8_7.62MM_60.2x60.2MM" H 5150 2550 60  0001 C CNN
F 3 "" H 5150 2550 60  0000 C CNN
	1    5050 2600
	1    0    0    -1  
$EndComp
Entry Wire Line
	4350 1800 4450 1900
Entry Wire Line
	4350 1900 4450 2000
Entry Wire Line
	4350 2000 4450 2100
Entry Wire Line
	4350 2100 4450 2200
Entry Wire Line
	4350 2200 4450 2300
Entry Wire Line
	4350 2300 4450 2400
Entry Wire Line
	4350 2400 4450 2500
Entry Wire Line
	4350 2500 4450 2600
Wire Bus Line
	4450 2600 4750 2600
Wire Wire Line
	4200 1800 4350 1800
Wire Wire Line
	4200 1900 4350 1900
Wire Wire Line
	4200 2000 4350 2000
Wire Wire Line
	4200 2100 4350 2100
Wire Wire Line
	4200 2200 4350 2200
Wire Wire Line
	4200 2300 4350 2300
Wire Wire Line
	4200 2400 4350 2400
Wire Wire Line
	4200 2500 4350 2500
$Comp
L MIC2981:MIC2981_82YN U2
U 1 1 5FD80D89
P 2600 1800
F 0 "U2" H 3400 2187 60  0000 C CNN
F 1 "MIC2981_82YN" H 3400 2081 60  0000 C CNN
F 2 "Package_DIP:DIP-18_W7.62mm" H 3400 2040 60  0001 C CNN
F 3 "" H 2600 1800 60  0000 C CNN
	1    2600 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 1800 2450 1800
Wire Wire Line
	2600 1900 2450 1900
Wire Wire Line
	2600 2000 2450 2000
Wire Wire Line
	2600 2100 2450 2100
Wire Wire Line
	2600 2200 2450 2200
Wire Wire Line
	2600 2300 2450 2300
Wire Wire Line
	2600 2400 2450 2400
Wire Wire Line
	2600 2500 2450 2500
Entry Wire Line
	2350 1900 2450 1800
Entry Wire Line
	2350 2000 2450 1900
Entry Wire Line
	2350 2100 2450 2000
Entry Wire Line
	2350 2200 2450 2100
Entry Wire Line
	2350 2300 2450 2200
Entry Wire Line
	2350 2400 2450 2300
Entry Wire Line
	2350 2500 2450 2400
Entry Wire Line
	2350 2600 2450 2500
Wire Bus Line
	2350 2600 2050 2600
Wire Wire Line
	1750 1800 1950 1800
Wire Wire Line
	1750 1900 1950 1900
Wire Wire Line
	1750 2000 1950 2000
Wire Wire Line
	1750 2100 1950 2100
Wire Wire Line
	1750 2200 1950 2200
Wire Wire Line
	1750 2300 1950 2300
Wire Wire Line
	1750 2400 1950 2400
Wire Wire Line
	1750 2500 1950 2500
Entry Wire Line
	1950 1800 2050 1900
Entry Wire Line
	1950 1900 2050 2000
Entry Wire Line
	1950 2000 2050 2100
Entry Wire Line
	1950 2100 2050 2200
Entry Wire Line
	1950 2200 2050 2300
Entry Wire Line
	1950 2300 2050 2400
Entry Wire Line
	1950 2400 2050 2500
Entry Wire Line
	1950 2500 2050 2600
Text Label 1200 1600 2    50   ~ 0
VCC
Text Label 2600 2600 2    50   ~ 0
VCC
$Comp
L TPIC6C595N:TPIC6C595N U3
U 1 1 5FE09D0E
P 9250 2150
F 0 "U3" H 9250 3088 60  0000 C CNN
F 1 "TPIC6C595N" H 9250 2982 60  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm" H 9250 2090 60  0001 C CNN
F 3 "" H 9250 2150 60  0000 C CNN
	1    9250 2150
	-1   0    0    -1  
$EndComp
Text Label 10450 1550 0    50   ~ 0
VCC
Text Label 10550 2150 0    50   ~ 0
VCC
Text Label 1350 2700 3    50   ~ 0
GND
Text Label 4200 2600 0    50   ~ 0
GND
Text Label 10250 1750 0    50   ~ 0
GND
$Comp
L Device:R R3
U 1 1 5FE118EA
P 7300 1750
F 0 "R3" V 7200 1750 50  0000 C CNN
F 1 "100" V 7400 1750 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 7230 1750 50  0001 C CNN
F 3 "~" H 7300 1750 50  0001 C CNN
	1    7300 1750
	0    1    1    0   
$EndComp
$Comp
L Device:R R4
U 1 1 5FE13757
P 7300 2050
F 0 "R4" V 7200 2050 50  0000 C CNN
F 1 "100" V 7400 2050 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 7230 2050 50  0001 C CNN
F 3 "~" H 7300 2050 50  0001 C CNN
	1    7300 2050
	0    1    1    0   
$EndComp
$Comp
L Device:R R5
U 1 1 5FE13C09
P 7300 2350
F 0 "R5" V 7200 2350 50  0000 C CNN
F 1 "100" V 7400 2350 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 7230 2350 50  0001 C CNN
F 3 "~" H 7300 2350 50  0001 C CNN
	1    7300 2350
	0    1    1    0   
$EndComp
$Comp
L Device:R R6
U 1 1 5FE14227
P 7300 2650
F 0 "R6" V 7200 2650 50  0000 C CNN
F 1 "100" V 7400 2650 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 7230 2650 50  0001 C CNN
F 3 "~" H 7300 2650 50  0001 C CNN
	1    7300 2650
	0    1    1    0   
$EndComp
$Comp
L Device:R R7
U 1 1 5FE146EF
P 7300 2950
F 0 "R7" V 7200 2950 50  0000 C CNN
F 1 "100" V 7400 2950 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 7230 2950 50  0001 C CNN
F 3 "~" H 7300 2950 50  0001 C CNN
	1    7300 2950
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 5FE14B19
P 7300 1450
F 0 "R2" V 7200 1450 50  0000 C CNN
F 1 "100" V 7400 1450 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 7230 1450 50  0001 C CNN
F 3 "~" H 7300 1450 50  0001 C CNN
	1    7300 1450
	0    1    1    0   
$EndComp
$Comp
L Device:R R1
U 1 1 5FE14FC7
P 7300 1150
F 0 "R1" V 7200 1150 50  0000 C CNN
F 1 "100" V 7400 1150 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 7230 1150 50  0001 C CNN
F 3 "~" H 7300 1150 50  0001 C CNN
	1    7300 1150
	0    1    1    0   
$EndComp
Text Label 1450 3600 0    50   ~ 0
GND
$Comp
L 74xx:74HCT4051 U1
U 1 1 5FD7E6DB
P 1350 2100
F 0 "U1" H 1550 2550 50  0000 C CNN
F 1 "74HCT4051" H 1300 1150 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm" H 1350 1700 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/cd74hct4051.pdf" H 1350 1700 50  0001 C CNN
	1    1350 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 1800 6550 1800
Wire Wire Line
	6400 1900 6550 1900
Wire Wire Line
	6400 2000 6550 2000
Wire Wire Line
	6400 2100 6550 2100
Wire Wire Line
	6400 2200 6550 2200
Wire Wire Line
	6400 2300 6550 2300
Wire Wire Line
	6400 2400 6550 2400
Wire Wire Line
	6400 2500 6550 2500
Text Label 6400 1800 0    50   ~ 0
c0
Text Label 6400 1900 0    50   ~ 0
c1
Text Label 6400 2000 0    50   ~ 0
c2
Text Label 6400 2100 0    50   ~ 0
c3
Text Label 6400 2200 0    50   ~ 0
c4
Text Label 6400 2300 0    50   ~ 0
c5
Text Label 6400 2400 0    50   ~ 0
c6
Text Label 6400 2500 0    50   ~ 0
c7
Wire Wire Line
	7150 1150 7000 1150
Wire Wire Line
	7150 1450 7000 1450
Wire Wire Line
	7150 1750 7000 1750
Wire Wire Line
	7150 2050 7000 2050
Wire Wire Line
	7150 2350 7000 2350
Wire Wire Line
	7150 2650 7000 2650
Wire Wire Line
	7150 2950 7000 2950
Text Label 7000 1150 0    50   ~ 0
c0
Text Label 7000 1450 0    50   ~ 0
c1
Text Label 7000 1750 0    50   ~ 0
c2
Text Label 7000 2050 0    50   ~ 0
c3
Text Label 7000 2350 0    50   ~ 0
c4
Text Label 7000 2650 0    50   ~ 0
c5
Text Label 7000 2950 0    50   ~ 0
c6
$Comp
L Device:R R8
U 1 1 5FE96CA4
P 7300 3250
F 0 "R8" V 7200 3250 50  0000 C CNN
F 1 "100" V 7400 3250 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 7230 3250 50  0001 C CNN
F 3 "~" H 7300 3250 50  0001 C CNN
	1    7300 3250
	0    1    1    0   
$EndComp
Wire Wire Line
	7150 3250 7000 3250
Text Label 7000 3250 0    50   ~ 0
c7
Entry Wire Line
	6550 1800 6650 1900
Entry Wire Line
	6550 1900 6650 2000
Entry Wire Line
	6550 2000 6650 2100
Entry Wire Line
	6550 2100 6650 2200
Entry Wire Line
	6550 2200 6650 2300
Entry Wire Line
	6550 2300 6650 2400
Entry Wire Line
	6550 2400 6650 2500
Entry Wire Line
	6550 2500 6650 2600
Entry Wire Line
	6900 1250 7000 1150
Entry Wire Line
	6900 1550 7000 1450
Entry Wire Line
	6900 1850 7000 1750
Entry Wire Line
	6900 2150 7000 2050
Entry Wire Line
	6900 2450 7000 2350
Entry Wire Line
	6900 2750 7000 2650
Entry Wire Line
	6900 3050 7000 2950
Entry Wire Line
	6900 3350 7000 3250
Wire Bus Line
	6900 3350 6650 3350
Wire Wire Line
	7450 1150 7600 1150
Wire Wire Line
	7450 1450 7600 1450
Wire Wire Line
	7450 1750 7600 1750
Wire Wire Line
	7450 2050 7600 2050
Wire Wire Line
	7450 2350 7600 2350
Wire Wire Line
	7450 2650 7600 2650
Wire Wire Line
	7450 2950 7600 2950
Wire Wire Line
	7450 3250 7600 3250
Text Label 7450 1150 0    50   ~ 0
d0
Text Label 7450 1450 0    50   ~ 0
d1
Text Label 7450 1750 0    50   ~ 0
d2
Text Label 7450 2050 0    50   ~ 0
d3
Text Label 7450 2350 0    50   ~ 0
d4
Text Label 7450 2650 0    50   ~ 0
d5
Text Label 7450 2950 0    50   ~ 0
d6
Text Label 7450 3250 0    50   ~ 0
d7
Wire Wire Line
	8250 1550 8100 1550
Wire Wire Line
	8250 1650 8100 1650
Wire Wire Line
	8250 1750 8100 1750
Wire Wire Line
	8250 1850 8100 1850
Wire Wire Line
	8250 1950 8100 1950
Wire Wire Line
	8250 2050 8100 2050
Wire Wire Line
	8250 2150 8100 2150
Wire Wire Line
	8250 2250 8100 2250
Text Label 8100 1550 0    50   ~ 0
d0
Text Label 8100 1650 0    50   ~ 0
d1
Text Label 8100 1750 0    50   ~ 0
d2
Text Label 8100 1850 0    50   ~ 0
d3
Text Label 8100 1950 0    50   ~ 0
d4
Text Label 8100 2050 0    50   ~ 0
d5
Text Label 8100 2150 0    50   ~ 0
d6
Text Label 8100 2250 0    50   ~ 0
d7
Entry Wire Line
	7600 1150 7700 1250
Entry Wire Line
	7600 1450 7700 1550
Entry Wire Line
	7600 1750 7700 1850
Entry Wire Line
	7600 2050 7700 2150
Entry Wire Line
	7600 2350 7700 2450
Entry Wire Line
	7600 2650 7700 2750
Entry Wire Line
	7600 2950 7700 3050
Entry Wire Line
	7600 3250 7700 3350
Entry Wire Line
	8000 1650 8100 1550
Entry Wire Line
	8000 1750 8100 1650
Entry Wire Line
	8000 1850 8100 1750
Entry Wire Line
	8000 1950 8100 1850
Entry Wire Line
	8000 2050 8100 1950
Entry Wire Line
	8000 2150 8100 2050
Entry Wire Line
	8000 2250 8100 2150
Entry Wire Line
	8000 2350 8100 2250
Wire Bus Line
	7700 3350 8000 3350
Text Label 1050 2400 2    50   ~ 0
GND
Text Label 1450 3750 0    50   ~ 0
s0
Text Label 1050 2000 2    50   ~ 0
s0
Text Label 1450 3900 0    50   ~ 0
s1
Text Label 1450 4050 0    50   ~ 0
s2
Text Label 1050 2100 2    50   ~ 0
s1
Text Label 1050 2200 2    50   ~ 0
s2
Text Label 1450 4500 0    50   ~ 0
r_clk
Text Label 10250 1950 0    50   ~ 0
r_clk
Text Label 1450 4200 0    50   ~ 0
serial_in
Text Label 1450 4350 0    50   ~ 0
serial_clk
Text Label 10250 2350 0    50   ~ 0
serial_clk
Text Label 8250 2550 2    50   ~ 0
serial_out
Text Label 1450 5800 0    50   ~ 0
serial_out
Text Label 1450 5050 0    50   ~ 0
VCC
Text Label 1450 5200 0    50   ~ 0
GND
Text Label 1450 5350 0    50   ~ 0
s0
Text Label 1450 5500 0    50   ~ 0
s1
Text Label 1450 5650 0    50   ~ 0
s2
Text Label 1450 5950 0    50   ~ 0
serial_clk
Text Label 1450 6100 0    50   ~ 0
r_clk
Text Label 10250 2550 0    50   ~ 0
serial_in
Text Label 8250 2750 2    50   ~ 0
GND
$Comp
L myConnectors:INPUT_CONN J1
U 1 1 5FE10ADF
P 1450 3300
F 0 "J1" H 1133 3415 50  0000 C CNN
F 1 "INPUT_CONN" H 1133 3324 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Horizontal" H 1450 3300 50  0001 C CNN
F 3 "" H 1450 3300 50  0001 C CNN
	1    1450 3300
	1    0    0    -1  
$EndComp
$Comp
L myConnectors:OUTPUT_CONN J2
U 1 1 5FE11788
P 1450 4900
F 0 "J2" H 1133 5015 50  0000 C CNN
F 1 "OUTPUT_CONN" H 1133 4924 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Horizontal" H 1450 4900 50  0001 C CNN
F 3 "" H 1450 4900 50  0001 C CNN
	1    1450 4900
	1    0    0    -1  
$EndComp
Text Label 1450 3450 0    50   ~ 0
VCC
$Comp
L Device:R R9
U 1 1 5FE2FB77
P 10400 2150
F 0 "R9" V 10300 2150 50  0000 C CNN
F 1 "10k" V 10500 2150 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 10330 2150 50  0001 C CNN
F 3 "~" H 10400 2150 50  0001 C CNN
	1    10400 2150
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C1
U 1 1 5FE506E1
P 1350 1500
F 0 "C1" H 1500 1500 50  0000 L CNN
F 1 "100 nF" H 1000 1550 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 1350 1500 50  0001 C CNN
F 3 "~" H 1350 1500 50  0001 C CNN
	1    1350 1500
	1    0    0    -1  
$EndComp
Text Label 1050 1800 2    50   ~ 0
VCC
Wire Wire Line
	1350 1600 1200 1600
Connection ~ 1350 1600
Text Label 1350 1400 2    50   ~ 0
GND
$Comp
L Device:C_Small C2
U 1 1 5FE5FD6A
P 2600 2700
F 0 "C2" H 2750 2700 50  0000 L CNN
F 1 "100 nF" H 2650 2600 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 2600 2700 50  0001 C CNN
F 3 "~" H 2600 2700 50  0001 C CNN
	1    2600 2700
	1    0    0    -1  
$EndComp
Text Label 2600 2800 3    50   ~ 0
GND
$Comp
L Device:C_Small C3
U 1 1 5FE60FA7
P 10250 1450
F 0 "C3" H 10400 1450 50  0000 L CNN
F 1 "100 nF" H 10450 1550 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 10250 1450 50  0001 C CNN
F 3 "~" H 10250 1450 50  0001 C CNN
	1    10250 1450
	1    0    0    -1  
$EndComp
Text Label 10250 1350 0    50   ~ 0
GND
Wire Wire Line
	10250 1550 10450 1550
Connection ~ 10250 1550
Text Label 1450 2700 3    50   ~ 0
GND
Wire Bus Line
	4750 1900 4750 2600
Wire Bus Line
	4450 1900 4450 2600
Wire Bus Line
	2350 1900 2350 2600
Wire Bus Line
	2050 1900 2050 2600
Wire Bus Line
	6900 1250 6900 3350
Wire Bus Line
	7700 1250 7700 3350
Wire Bus Line
	8000 1650 8000 3350
Wire Bus Line
	6650 1900 6650 3350
$EndSCHEMATC
