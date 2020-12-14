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
L Interface_Expansion:TPIC6595 U?
U 1 1 5FD7B849
P 3600 3800
F 0 "U?" H 3600 4581 50  0000 C CNN
F 1 "TPIC6595" H 3600 4490 50  0000 C CNN
F 2 "" H 4250 3150 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tpic6595.pdf" H 3600 3750 50  0001 C CNN
	1    3600 3800
	1    0    0    -1  
$EndComp
$Comp
L akn:DISP_LED_MAT_8x8_COM_ANODE D?
U 1 1 5FD8956E
P 4350 2450
F 0 "D?" H 5025 3537 60  0000 C CNN
F 1 "DISP_LED_MAT_8x8_COM_ANODE" H 5025 3431 60  0000 C CNN
F 2 "" H 4450 2400 60  0000 C CNN
F 3 "" H 4450 2400 60  0000 C CNN
	1    4350 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 3400 5950 3400
Wire Wire Line
	5950 3400 5950 1650
Wire Wire Line
	5950 1650 5700 1650
$EndSCHEMATC
