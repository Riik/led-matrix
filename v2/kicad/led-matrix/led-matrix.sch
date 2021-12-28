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
$Comp
L akn_misc:DISP_LED_MAT_8x8_COM_ANODE D?
U 1 1 61CD0FA8
P 4250 2050
F 0 "D?" H 4925 3137 60  0000 C CNN
F 1 "DISP_LED_MAT_8x8_COM_ANODE" H 4925 3031 60  0000 C CNN
F 2 "" H 4350 2000 60  0000 C CNN
F 3 "" H 4350 2000 60  0000 C CNN
	1    4250 2050
	1    0    0    -1  
$EndComp
$Comp
L my_connectors:Conn_02x03_Input J?
U 1 1 61D0C670
P 1500 3800
F 0 "J?" H 1500 4167 50  0000 C CNN
F 1 "Conn_02x03_Input" H 1500 4076 50  0000 C CNN
F 2 "" H 1200 3800 50  0001 C CNN
F 3 "~" H 1200 3800 50  0001 C CNN
	1    1500 3800
	1    0    0    -1  
$EndComp
$Comp
L my_connectors:Conn_02x03_Output J?
U 1 1 61D0DD4C
P 5850 3800
F 0 "J?" H 5850 4167 50  0000 C CNN
F 1 "Conn_02x03_Output" H 5850 4076 50  0000 C CNN
F 2 "" H 5550 3800 50  0001 C CNN
F 3 "~" H 5550 3800 50  0001 C CNN
	1    5850 3800
	1    0    0    -1  
$EndComp
Text GLabel 2250 3700 2    50   Input ~ 0
GND
Text GLabel 2050 3000 3    50   Input ~ 0
GND
Text GLabel 6650 3700 2    50   Input ~ 0
GND
Wire Wire Line
	6350 3700 6650 3700
Wire Wire Line
	2000 3700 2250 3700
$Comp
L Device:R R?
U 1 1 61D1432E
P 1650 950
F 0 "R?" V 1443 950 50  0000 C CNN
F 1 "28 K" V 1534 950 50  0000 C CNN
F 2 "" V 1580 950 50  0001 C CNN
F 3 "~" H 1650 950 50  0001 C CNN
	1    1650 950 
	0    1    1    0   
$EndComp
Wire Wire Line
	1800 950  2050 950 
Wire Wire Line
	2050 950  2050 1000
Wire Wire Line
	1500 950  1500 1200
Wire Wire Line
	1500 1200 1650 1200
$Comp
L Device:C C?
U 1 1 61D15CC5
P 2400 950
F 0 "C?" V 2148 950 50  0000 C CNN
F 1 "0.1 uF" V 2239 950 50  0000 C CNN
F 2 "" H 2438 800 50  0001 C CNN
F 3 "~" H 2400 950 50  0001 C CNN
	1    2400 950 
	0    1    1    0   
$EndComp
Text GLabel 2550 950  2    50   Input ~ 0
GND
Wire Wire Line
	2050 950  2250 950 
Connection ~ 2050 950 
Text GLabel 2050 800  1    50   Input ~ 0
Vcc
Wire Wire Line
	2050 800  2050 950 
$Comp
L Driver_LED:MAX7221xWG U?
U 1 1 61D1867B
P 2050 2000
F 0 "U?" H 1550 2500 50  0000 C CNN
F 1 "MAX7221xWG" H 1500 2400 50  0000 C CNN
F 2 "Package_SO:SOIC-24W_7.5x15.4mm_P1.27mm" H 2000 2050 50  0001 C CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf" H 2100 1850 50  0001 C CNN
	1    2050 2000
	1    0    0    -1  
$EndComp
$EndSCHEMATC
