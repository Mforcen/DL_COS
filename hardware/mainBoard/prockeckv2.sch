EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 5
Title "Procheck V2"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 3850 4700 1250 2200
U 5DA8B375
F0 "MCU" 50
F1 "MCU.sch" 50
$EndSheet
$Sheet
S 1050 4300 1500 850 
U 5DCBE02B
F0 "IO" 50
F1 "IO.sch" 50
F2 "D_OUTPUTS" I R 2550 4800 50 
F3 "D_INPUTS" O R 2550 4600 50 
$EndSheet
$Sheet
S 1050 5500 1500 850 
U 5DE32C3C
F0 "Peripherals" 50
F1 "Peripherals.sch" 50
$EndSheet
$Comp
L Connector:USB_B_Micro J?
U 1 1 5C742C3D
P 850 1300
F 0 "J?" H 905 1767 50  0000 C CNN
F 1 "USB_B_Micro" H 905 1676 50  0000 C CNN
F 2 "Connector_USB:USB_Micro-B_Molex-105017-0001" H 1000 1250 50  0001 C CNN
F 3 "~" H 1000 1250 50  0001 C CNN
	1    850  1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1150 1100 1400 1100
Text GLabel 1400 1100 2    50   Output ~ 0
Vusb
$Comp
L power:GND #PWR?
U 1 1 5CA32110
P 850 1750
F 0 "#PWR?" H 850 1500 50  0001 C CNN
F 1 "GND" H 855 1577 50  0000 C CNN
F 2 "" H 850 1750 50  0001 C CNN
F 3 "" H 850 1750 50  0001 C CNN
	1    850  1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	750  1700 750  1750
Wire Wire Line
	750  1750 850  1750
Text Notes 600  650  0    50   ~ 0
Interfaz USB
Text GLabel 1250 1300 2    50   BiDi ~ 0
UD+
Text GLabel 1250 1400 2    50   BiDi ~ 0
UD-
Wire Wire Line
	1150 1400 1250 1400
Wire Wire Line
	1150 1300 1250 1300
$Comp
L Device:R R?
U 1 1 5DB2771D
P 8100 1100
F 0 "R?" H 8170 1146 50  0000 L CNN
F 1 "10k" H 8170 1055 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8030 1100 50  0001 C CNN
F 3 "~" H 8100 1100 50  0001 C CNN
	1    8100 1100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5DB27D33
P 8100 1550
F 0 "SW?" H 8100 1700 50  0000 C CNN
F 1 "SW_Push" H 8100 1500 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 8100 1750 50  0001 C CNN
F 3 "" H 8100 1750 50  0001 C CNN
	1    8100 1550
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5DB27E98
P 8100 1800
F 0 "#PWR?" H 8100 1550 50  0001 C CNN
F 1 "GND" H 8105 1627 50  0000 C CNN
F 2 "" H 8100 1800 50  0001 C CNN
F 3 "" H 8100 1800 50  0001 C CNN
	1    8100 1800
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5DB2819B
P 8100 850
F 0 "#PWR?" H 8100 700 50  0001 C CNN
F 1 "+5V" H 8115 1023 50  0000 C CNN
F 2 "" H 8100 850 50  0001 C CNN
F 3 "" H 8100 850 50  0001 C CNN
	1    8100 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 850  8100 950 
Wire Wire Line
	8100 1250 8100 1300
Wire Wire Line
	8100 1750 8100 1800
Text GLabel 8250 1300 2    50   UnSpc ~ 0
RST
Wire Wire Line
	8250 1300 8100 1300
Connection ~ 8100 1300
Wire Wire Line
	8100 1300 8100 1350
Text GLabel 7600 800  0    50   Input ~ 0
LED
$Comp
L Device:R R?
U 1 1 5DD47556
P 7700 1100
F 0 "R?" H 7770 1146 50  0000 L CNN
F 1 "220" H 7770 1055 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7630 1100 50  0001 C CNN
F 3 "~" H 7700 1100 50  0001 C CNN
	1    7700 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 800  7700 800 
Wire Wire Line
	7700 800  7700 950 
$Comp
L Device:LED D?
U 1 1 5DBDD71E
P 7700 1550
F 0 "D?" V 7738 1433 50  0000 R CNN
F 1 "LED" V 7647 1433 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 7700 1550 50  0001 C CNN
F 3 "~" H 7700 1550 50  0001 C CNN
	1    7700 1550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7700 1400 7700 1250
$Comp
L power:GND #PWR?
U 1 1 5DC1662F
P 7700 1800
F 0 "#PWR?" H 7700 1550 50  0001 C CNN
F 1 "GND" H 7705 1627 50  0000 C CNN
F 2 "" H 7700 1800 50  0001 C CNN
F 3 "" H 7700 1800 50  0001 C CNN
	1    7700 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 1800 7700 1700
Wire Wire Line
	850  1700 850  1750
Connection ~ 850  1750
NoConn ~ 1150 1500
NoConn ~ -1050 3850
Wire Notes Line width 14 style solid
	500  4150 11200 4150
Wire Notes Line width 14 style solid
	6950 6550 6950 4150
Text Notes 5850 7700 0    197  ~ 39
Digital
Text Notes 10200 6500 0    197  ~ 39
Others
Text Notes 9150 4000 0    197  ~ 39
Power Supply
$Sheet
S 3850 2400 2400 1100
U 5E851D3B
F0 "Power" 50
F1 "Power.sch" 50
$EndSheet
Text Notes 4750 3250 0    118  ~ 0
+12V\n+5V\n-5V\n+3V3
$EndSCHEMATC
