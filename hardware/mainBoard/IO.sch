EESchema Schematic File Version 4
LIBS:prockeckv2-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 8
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
L Connector:Screw_Terminal_01x20 J?
U 1 1 5E51841F
P 4500 3650
F 0 "J?" V 4625 3596 50  0000 C CNN
F 1 "Screw_Terminal_01x20" V 4716 3596 50  0000 C CNN
F 2 "" H 4500 3650 50  0001 C CNN
F 3 "~" H 4500 3650 50  0001 C CNN
	1    4500 3650
	0    1    1    0   
$EndComp
Text HLabel 2000 2450 0    50   BiDi ~ 0
IO1
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
	5000 3450 5000 2550
Wire Wire Line
	5200 3450 5200 2550
Wire Wire Line
	5300 2550 5300 3450
Wire Wire Line
	5400 3450 5400 2550
$Comp
L power:GND #PWR?
U 1 1 5E5225D5
P 5700 3550
F 0 "#PWR?" H 5700 3300 50  0001 C CNN
F 1 "GND" H 5705 3377 50  0000 C CNN
F 2 "" H 5700 3550 50  0001 C CNN
F 3 "" H 5700 3550 50  0001 C CNN
	1    5700 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 3550 5700 3400
Wire Wire Line
	5700 3400 5100 3400
Wire Wire Line
	5100 3400 5100 3450
Wire Wire Line
	5100 3400 4700 3400
Wire Wire Line
	4700 3400 4700 3450
Connection ~ 5100 3400
Wire Wire Line
	4700 3400 4300 3400
Wire Wire Line
	4300 3400 4300 3450
Connection ~ 4700 3400
Wire Wire Line
	4300 3400 3900 3400
Wire Wire Line
	3900 3400 3900 3450
Connection ~ 4300 3400
Wire Wire Line
	3900 3400 3500 3400
Wire Wire Line
	3500 3400 3500 3450
Connection ~ 3900 3400
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
IO1_1
Text Label 3700 2600 3    50   ~ 0
IO1_2
Text Label 3800 2600 3    50   ~ 0
IO1_3
Text Label 4000 2600 3    50   ~ 0
IO1_4
Text Label 4100 2600 3    50   ~ 0
IO1_5
Text Label 4200 2600 3    50   ~ 0
IO1_6
Text Label 4400 2600 3    50   ~ 0
IO1_7
Text Label 4500 2600 3    50   ~ 0
IO1_8
Text Label 4600 2600 3    50   ~ 0
IO1_9
Text Label 4800 2600 3    50   ~ 0
IO1_10
Text Label 4900 2600 3    50   ~ 0
IO1_11
Text Label 5000 2600 3    50   ~ 0
IO1_12
Text Label 5200 2600 3    50   ~ 0
IO1_13
Text Label 5400 2600 3    50   ~ 0
IO1_15
Text Label 5300 2600 3    50   ~ 0
IO1_14
$Comp
L Connector:Screw_Terminal_01x01 J?
U 1 1 5E56C93B
P 2450 2100
F 0 "J?" H 2530 2142 50  0000 L CNN
F 1 "Screw_Terminal_01x01" H 2530 2051 50  0000 L CNN
F 2 "" H 2450 2100 50  0001 C CNN
F 3 "~" H 2450 2100 50  0001 C CNN
	1    2450 2100
	1    0    0    -1  
$EndComp
Text HLabel 2000 2100 0    50   Input ~ 0
DAC
Wire Wire Line
	2000 2100 2250 2100
$Comp
L Connector:Screw_Terminal_01x06 J?
U 1 1 5E57511A
P 7900 3650
F 0 "J?" V 7772 3930 50  0000 L CNN
F 1 "Screw_Terminal_01x06" V 7863 3930 50  0000 L CNN
F 2 "" H 7900 3650 50  0001 C CNN
F 3 "~" H 7900 3650 50  0001 C CNN
	1    7900 3650
	0    1    1    0   
$EndComp
Text HLabel 7000 2450 0    50   Input ~ 0
PWM
Wire Bus Line
	7000 2450 8100 2450
$Comp
L power:GND #PWR?
U 1 1 5E57A347
P 7250 3550
F 0 "#PWR?" H 7250 3300 50  0001 C CNN
F 1 "GND" H 7255 3377 50  0000 C CNN
F 2 "" H 7250 3550 50  0001 C CNN
F 3 "" H 7250 3550 50  0001 C CNN
	1    7250 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 3550 7250 3350
Wire Wire Line
	7250 3350 7600 3350
Wire Wire Line
	7600 3350 7600 3450
Wire Wire Line
	7600 3350 7900 3350
Wire Wire Line
	7900 3350 7900 3450
Connection ~ 7600 3350
Wire Wire Line
	7700 3450 7700 2550
Wire Wire Line
	7800 3450 7800 2550
Wire Wire Line
	8000 3450 8000 2550
Wire Wire Line
	8100 3450 8100 2550
Wire Bus Line
	2000 2450 5350 2450
Entry Wire Line
	7600 2450 7700 2550
Entry Wire Line
	7700 2450 7800 2550
Entry Wire Line
	7900 2450 8000 2550
Entry Wire Line
	8000 2450 8100 2550
Text Label 7700 2600 3    50   ~ 0
PWM1
Text Label 7800 2600 3    50   ~ 0
PWM2
Text Label 8000 2600 3    50   ~ 0
PWM3
Text Label 8100 2600 3    50   ~ 0
PWM4
$EndSCHEMATC
