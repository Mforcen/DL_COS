EESchema Schematic File Version 4
LIBS:prockeckv2-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 10
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 3300 2450 0    50   BiDi ~ 0
IO[0..14]
$Comp
L power:GND #PWR04
U 1 1 5E5225D5
P 5700 3550
F 0 "#PWR04" H 5700 3300 50  0001 C CNN
F 1 "GND" H 5705 3377 50  0000 C CNN
F 2 "" H 5700 3550 50  0001 C CNN
F 3 "" H 5700 3550 50  0001 C CNN
	1    5700 3550
	1    0    0    -1  
$EndComp
Entry Wire Line
	3500 2450 3600 2550
Entry Wire Line
	3600 2450 3700 2550
Entry Wire Line
	3700 2450 3800 2550
Entry Wire Line
	3900 2450 4000 2550
Entry Wire Line
	4000 2450 4100 2550
Entry Wire Line
	4100 2450 4200 2550
Entry Wire Line
	4300 2450 4400 2550
Entry Wire Line
	4400 2450 4500 2550
Entry Wire Line
	4500 2450 4600 2550
Entry Wire Line
	4700 2450 4800 2550
Entry Wire Line
	4800 2450 4900 2550
Entry Wire Line
	4900 2450 5000 2550
Entry Wire Line
	5100 2450 5200 2550
Entry Wire Line
	5200 2450 5300 2550
Entry Wire Line
	5300 2450 5400 2550
Text Label 3600 2600 3    50   ~ 0
IO0
Text Label 3700 2600 3    50   ~ 0
IO1
Text Label 3800 2600 3    50   ~ 0
IO2
Text Label 4000 2600 3    50   ~ 0
IO3
Text Label 4100 2600 3    50   ~ 0
IO4
Text Label 4200 2600 3    50   ~ 0
IO5
Text Label 4400 2600 3    50   ~ 0
IO6
Text Label 4500 2600 3    50   ~ 0
IO7
Text Label 4600 2600 3    50   ~ 0
IO8
Text Label 4800 2600 3    50   ~ 0
IO9
Text Label 4900 2600 3    50   ~ 0
IO10
Text Label 5000 2600 3    50   ~ 0
IO11
Text Label 5200 2600 3    50   ~ 0
IO12
Text Label 5400 2600 3    50   ~ 0
IO14
Text Label 5300 2600 3    50   ~ 0
IO13
Text HLabel 2000 2100 0    50   Input ~ 0
DAC
Text HLabel 7000 2450 0    50   Input ~ 0
PWM[0..1]
$Comp
L power:GND #PWR06
U 1 1 5E57A347
P 7250 3550
F 0 "#PWR06" H 7250 3300 50  0001 C CNN
F 1 "GND" H 7255 3377 50  0000 C CNN
F 2 "" H 7250 3550 50  0001 C CNN
F 3 "" H 7250 3550 50  0001 C CNN
	1    7250 3550
	1    0    0    -1  
$EndComp
Entry Wire Line
	7500 2450 7600 2550
Entry Wire Line
	7800 2450 7900 2550
Text Label 7600 2600 3    50   ~ 0
PWM0
Text Label 7900 2600 3    50   ~ 0
PWM1
$Comp
L Connector:Screw_Terminal_01x06 J4
U 1 1 5E57511A
P 7800 3650
F 0 "J4" V 7672 3930 50  0000 L CNN
F 1 "Screw_Terminal_01x06" V 7763 3930 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 7800 3650 50  0001 C CNN
F 3 "~" H 7800 3650 50  0001 C CNN
	1    7800 3650
	0    1    1    0   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J2
U 1 1 5E07BAAA
P 2450 2000
F 0 "J2" H 2530 1992 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 2530 1901 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2450 2000 50  0001 C CNN
F 3 "~" H 2450 2000 50  0001 C CNN
	1    2450 2000
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR02
U 1 1 5E07BFC3
P 2150 1950
F 0 "#PWR02" H 2150 1800 50  0001 C CNN
F 1 "+12V" H 2165 2123 50  0000 C CNN
F 2 "" H 2150 1950 50  0001 C CNN
F 3 "" H 2150 1950 50  0001 C CNN
	1    2150 1950
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR03
U 1 1 5E0C9166
P 5700 3350
F 0 "#PWR03" H 5700 3200 50  0001 C CNN
F 1 "+12V" V 5715 3478 50  0000 L CNN
F 2 "" H 5700 3350 50  0001 C CNN
F 3 "" H 5700 3350 50  0001 C CNN
	1    5700 3350
	0    1    1    0   
$EndComp
$Comp
L power:+12V #PWR05
U 1 1 5E0D82DA
P 7200 3300
F 0 "#PWR05" H 7200 3150 50  0001 C CNN
F 1 "+12V" V 7215 3428 50  0000 L CNN
F 2 "" H 7200 3300 50  0001 C CNN
F 3 "" H 7200 3300 50  0001 C CNN
	1    7200 3300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3600 3450 3600 2550
Wire Wire Line
	3700 2550 3700 3450
Wire Wire Line
	3800 2550 3800 3450
Wire Wire Line
	4000 3450 4000 2550
Wire Wire Line
	4100 2550 4100 3450
Wire Wire Line
	4200 3450 4200 2550
Wire Wire Line
	4400 3450 4400 2550
Wire Wire Line
	4500 2550 4500 3450
Wire Wire Line
	4600 2550 4600 3450
Wire Wire Line
	4800 3450 4800 2550
Wire Wire Line
	4900 2550 4900 3450
Wire Wire Line
	5200 3450 5200 2550
Wire Wire Line
	5300 2550 5300 3450
Wire Wire Line
	5400 3450 5400 2550
Wire Wire Line
	5700 3550 5700 3400
Wire Wire Line
	4700 3400 4700 3450
Connection ~ 4700 3400
Wire Wire Line
	3900 3400 3900 3450
Wire Wire Line
	2000 2100 2250 2100
Wire Wire Line
	7250 3550 7250 3350
Wire Wire Line
	7600 3450 7600 2550
Wire Wire Line
	7900 3450 7900 2550
Wire Wire Line
	7700 3350 7700 3450
Wire Wire Line
	2150 1950 2150 2000
Wire Wire Line
	2150 2000 2250 2000
Wire Wire Line
	3900 3400 4700 3400
Wire Wire Line
	4700 3400 5700 3400
Wire Wire Line
	5700 3350 5100 3350
Wire Wire Line
	5100 3350 5100 3450
Wire Wire Line
	5000 3450 5000 2550
Wire Wire Line
	5100 3350 4300 3350
Wire Wire Line
	4300 3350 4300 3450
Connection ~ 5100 3350
Wire Wire Line
	7250 3350 7700 3350
Wire Wire Line
	7200 3300 7500 3300
Wire Wire Line
	7500 3300 7500 3450
Wire Wire Line
	7500 3300 7800 3300
Wire Wire Line
	7800 3300 7800 3450
Connection ~ 7500 3300
Wire Wire Line
	8000 3450 8000 3350
Wire Wire Line
	8000 3350 7700 3350
Connection ~ 7700 3350
$Comp
L Connector:Screw_Terminal_01x19 J3
U 1 1 5DF27906
P 4500 3650
F 0 "J3" V 4625 3646 50  0000 C CNN
F 1 "Screw_Terminal_01x19" V 4716 3646 50  0000 C CNN
F 2 "chinalandfoot:TerminalBlock_19_P5.08mm" H 4500 3650 50  0001 C CNN
F 3 "~" H 4500 3650 50  0001 C CNN
	1    4500 3650
	0    1    1    0   
$EndComp
Wire Bus Line
	7000 2450 7850 2450
Wire Bus Line
	3300 2450 5350 2450
$EndSCHEMATC
