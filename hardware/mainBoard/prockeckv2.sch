EESchema Schematic File Version 4
LIBS:prockeckv2-cache
LIBS:forcenOne-cache
EELAYER 26 0
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
$EndSheet
$Sheet
S 1050 5500 1500 850 
U 5DE32C3C
F0 "Peripherals" 50
F1 "Peripherals.sch" 50
$EndSheet
$Comp
L forcenOne-rescue:TP4056-chinaland U?
U 1 1 5C741D43
P 3400 1350
F 0 "U?" H 3400 1683 50  0000 C CNN
F 1 "TP4056" H 3400 1607 30  0000 C CNN
F 2 "Package_SO:HSOP-8-1EP_3.9x4.9mm_P1.27mm_EP2.41x3.1mm_ThermalVias" H 3400 1250 50  0001 C CNN
F 3 "" H 3400 1250 50  0001 C CNN
	1    3400 1350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5C742589
P 2900 1850
F 0 "#PWR?" H 2900 1600 50  0001 C CNN
F 1 "GND" H 2905 1677 50  0000 C CNN
F 2 "" H 2900 1850 50  0001 C CNN
F 3 "" H 2900 1850 50  0001 C CNN
	1    2900 1850
	1    0    0    -1  
$EndComp
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
Text GLabel 2900 850  0    50   Input ~ 0
Vusb
Wire Wire Line
	2900 1850 2900 1750
Wire Wire Line
	3100 1200 2900 1200
Wire Wire Line
	2900 1200 2900 1400
Wire Wire Line
	3100 1400 2900 1400
Connection ~ 2900 1400
Wire Wire Line
	2900 1400 2900 1750
$Comp
L Device:R_Small R?
U 1 1 5C749CA5
P 4550 1450
F 0 "R?" H 4609 1496 50  0000 L CNN
F 1 "1k" H 4609 1405 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 4550 1450 50  0001 C CNN
F 3 "~" H 4550 1450 50  0001 C CNN
	1    4550 1450
	1    0    0    -1  
$EndComp
$Comp
L forcenOne-rescue:DW01A-chinaland IC?
U 1 1 5C9233C6
P 4950 1250
F 0 "IC?" H 4950 1499 50  0000 C CNN
F 1 "DW01A" H 4950 1416 39  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6" H 4950 1400 50  0001 C CNN
F 3 "" H 4950 1400 50  0001 C CNN
	1    4950 1250
	1    0    0    -1  
$EndComp
$Comp
L forcenOne-rescue:FS8205A-chinaland M?
U 1 1 5C92345F
P 4950 1650
F 0 "M?" H 4950 1538 20  0000 C CNN
F 1 "FS8205A" H 4950 1501 12  0000 C CNN
F 2 "Package_SO:TSSOP-8_4.4x3mm_P0.65mm" H 4850 1550 50  0001 C CNN
F 3 "" H 4850 1550 50  0001 C CNN
	1    4950 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 1600 4750 1600
Wire Wire Line
	4750 1600 4750 1450
Wire Wire Line
	4750 1450 4900 1450
Wire Wire Line
	5100 1600 5150 1600
Wire Wire Line
	5150 1600 5150 1450
Wire Wire Line
	5150 1450 5000 1450
Wire Wire Line
	4800 1650 4750 1650
Wire Wire Line
	4750 1650 4750 1700
Wire Wire Line
	4750 1700 4800 1700
Wire Wire Line
	4750 1700 4750 1750
Connection ~ 4750 1700
Connection ~ 4750 1750
Wire Wire Line
	5100 1650 5100 1700
Wire Wire Line
	5100 1650 5350 1650
Wire Wire Line
	5350 1650 5350 1500
Wire Wire Line
	5350 1300 5250 1300
Connection ~ 5100 1650
NoConn ~ 4900 1550
NoConn ~ 5000 1550
Wire Wire Line
	4650 1300 4550 1300
Wire Wire Line
	4550 1300 4550 1350
Wire Wire Line
	4550 1550 4550 1750
Connection ~ 4550 1750
Wire Wire Line
	4550 1750 4750 1750
$Comp
L Device:R_Small R?
U 1 1 5C9296AC
P 5350 1000
F 0 "R?" H 5409 1046 50  0000 L CNN
F 1 "100" H 5409 955 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5350 1000 50  0001 C CNN
F 3 "~" H 5350 1000 50  0001 C CNN
	1    5350 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 850  3000 850 
Wire Wire Line
	5350 1200 5350 1100
Wire Wire Line
	5350 900  5350 850 
Connection ~ 5350 850 
$Comp
L Device:C_Small C?
U 1 1 5C92B90B
P 5500 1300
F 0 "C?" H 5592 1346 50  0000 L CNN
F 1 "0.1uF" H 5592 1255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5500 1300 50  0001 C CNN
F 3 "~" H 5500 1300 50  0001 C CNN
	1    5500 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 1200 5350 1200
Wire Wire Line
	5350 1200 5500 1200
Connection ~ 5350 1200
Wire Wire Line
	5350 1500 5500 1500
Wire Wire Line
	5500 1500 5500 1400
Connection ~ 5350 1500
Wire Wire Line
	5350 1500 5350 1300
Wire Wire Line
	5500 1500 5750 1500
Connection ~ 5500 1500
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 5C92EDA2
P 6300 1300
F 0 "J?" H 6380 1292 50  0000 L CNN
F 1 "BATT" H 6380 1201 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 6300 1300 50  0001 C CNN
F 3 "~" H 6300 1300 50  0001 C CNN
	1    6300 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 1300 5900 1300
Wire Wire Line
	5900 1300 5900 850 
Connection ~ 5900 850 
Wire Wire Line
	6100 1400 5750 1400
Wire Wire Line
	5750 1400 5750 1500
$Comp
L Device:C_Small C?
U 1 1 5C932B7C
P 5850 1600
F 0 "C?" H 5942 1646 50  0000 L CNN
F 1 "10uF" H 5942 1555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 5850 1600 50  0001 C CNN
F 3 "~" H 5850 1600 50  0001 C CNN
	1    5850 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 1500 5850 1300
Wire Wire Line
	5850 1300 5900 1300
Connection ~ 5900 1300
Wire Wire Line
	5850 1700 5850 1750
Wire Wire Line
	4750 1750 5850 1750
Wire Wire Line
	3700 1200 3850 1200
Wire Wire Line
	4450 1500 4450 1050
Wire Wire Line
	4450 1050 4600 1050
Wire Wire Line
	4600 1050 4600 850 
Wire Wire Line
	4600 850  5350 850 
Connection ~ 2900 1750
Wire Wire Line
	2900 1750 4550 1750
Wire Wire Line
	3850 1200 3850 850 
$Comp
L power:+5V #PWR?
U 1 1 5C93FAC4
P 4100 850
F 0 "#PWR?" H 4100 700 50  0001 C CNN
F 1 "+5V" H 4115 1023 50  0000 C CNN
F 2 "" H 4100 850 50  0001 C CNN
F 3 "" H 4100 850 50  0001 C CNN
	1    4100 850 
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 5C93FB8A
P 3950 1050
F 0 "R?" H 4009 1096 50  0000 L CNN
F 1 "10k" H 4009 1005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 3950 1050 50  0001 C CNN
F 3 "~" H 3950 1050 50  0001 C CNN
	1    3950 1050
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 5C93FCA8
P 4200 1050
F 0 "R?" H 4259 1096 50  0000 L CNN
F 1 "10k" H 4259 1005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 4200 1050 50  0001 C CNN
F 3 "~" H 4200 1050 50  0001 C CNN
	1    4200 1050
	1    0    0    -1  
$EndComp
Text GLabel 4150 1300 2    50   Output ~ 0
CHRG
Wire Wire Line
	4150 1300 3950 1300
Connection ~ 3950 1300
Wire Wire Line
	3950 1300 3700 1300
Wire Wire Line
	4200 1200 4050 1200
Wire Wire Line
	3950 1150 3950 1300
Wire Wire Line
	4200 1150 4200 1200
Wire Wire Line
	4200 950  4200 900 
Wire Wire Line
	4200 900  4100 900 
Wire Wire Line
	3950 900  3950 950 
Wire Wire Line
	4100 850  4100 900 
Connection ~ 4100 900 
Wire Wire Line
	4100 900  3950 900 
Wire Wire Line
	3700 1500 4450 1500
Wire Wire Line
	3700 1400 4050 1400
Wire Wire Line
	4050 1400 4050 1200
Text GLabel 4150 1400 2    50   Output ~ 0
STDBY
Wire Wire Line
	4050 1400 4150 1400
Connection ~ 4050 1400
Wire Wire Line
	3100 1500 3000 1500
Wire Wire Line
	3000 1500 3000 1050
Connection ~ 3000 850 
Wire Wire Line
	3000 850  3850 850 
$Comp
L Device:R_Small R?
U 1 1 5C960AD4
P 2650 1450
F 0 "R?" H 2709 1496 50  0000 L CNN
F 1 "1k2" H 2709 1405 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2650 1450 50  0001 C CNN
F 3 "~" H 2650 1450 50  0001 C CNN
	1    2650 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 1300 2650 1300
Wire Wire Line
	2650 1300 2650 1350
Wire Wire Line
	2650 1550 2650 1750
$Comp
L Device:C_Small C?
U 1 1 5C969A83
P 2300 1400
F 0 "C?" H 2392 1446 50  0000 L CNN
F 1 "0.1u" H 2392 1355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2300 1400 50  0001 C CNN
F 3 "~" H 2300 1400 50  0001 C CNN
	1    2300 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 1050 2300 1050
Wire Wire Line
	2300 1050 2300 1300
Connection ~ 3000 1050
Wire Wire Line
	3000 1050 3000 850 
Wire Wire Line
	2300 1500 2300 1750
Wire Wire Line
	2300 1750 2650 1750
Connection ~ 2650 1750
Wire Wire Line
	2650 1750 2900 1750
Wire Wire Line
	5900 850  6300 850 
Wire Wire Line
	5350 850  5900 850 
Text Notes 3500 2000 0    50   ~ 0
Cargador de batería con circuito protector
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
$Comp
L Device:Q_PMOS_GSD Q?
U 1 1 5DD4BC32
P 1650 2700
F 0 "Q?" V 1993 2700 50  0000 C CNN
F 1 "CJ2301" V 1902 2700 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23_Handsoldering" H 1850 2800 50  0001 C CNN
F 3 "~" H 1650 2700 50  0001 C CNN
	1    1650 2700
	0    -1   -1   0   
$EndComp
Text GLabel 6300 850  2    50   Output ~ 0
Vbatt
Text GLabel 1150 2600 0    50   Input ~ 0
Vbatt
Wire Wire Line
	1450 2600 1150 2600
Text GLabel 1100 3000 0    50   Input ~ 0
Vusb
Wire Wire Line
	1100 3000 1650 3000
Wire Wire Line
	1650 3000 1650 2900
$Comp
L Device:R_Small R?
U 1 1 5DFE1085
P 1650 3250
F 0 "R?" H 1709 3296 50  0000 L CNN
F 1 "100k" H 1709 3205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 1650 3250 50  0001 C CNN
F 3 "~" H 1650 3250 50  0001 C CNN
	1    1650 3250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E086355
P 1650 3450
F 0 "#PWR?" H 1650 3200 50  0001 C CNN
F 1 "GND" H 1655 3277 50  0000 C CNN
F 2 "" H 1650 3450 50  0001 C CNN
F 3 "" H 1650 3450 50  0001 C CNN
	1    1650 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 3450 1650 3350
Wire Wire Line
	1650 3150 1650 3000
Connection ~ 1650 3000
$Comp
L Device:D D?
U 1 1 5E12C416
P 1950 3000
F 0 "D?" H 1950 2784 50  0000 C CNN
F 1 "SS3150" H 1950 2875 50  0000 C CNN
F 2 "Diode_SMD:D_SMC_Handsoldering" H 1950 3000 50  0001 C CNN
F 3 "~" H 1950 3000 50  0001 C CNN
	1    1950 3000
	-1   0    0    1   
$EndComp
Wire Wire Line
	1800 3000 1650 3000
Wire Wire Line
	1850 2600 2450 2600
Wire Wire Line
	2450 2600 2450 3000
Wire Wire Line
	2450 3000 2100 3000
Text Notes 2050 3350 0    50   ~ 0
Cuando Vusb>Vbat no conduce\nVusb<Vbat-1
Text GLabel 3100 3000 2    50   Output ~ 0
VPow
Wire Wire Line
	3100 3000 2450 3000
Connection ~ 2450 3000
Wire Wire Line
	7600 800  7700 800 
Wire Wire Line
	7700 800  7700 950 
Text Notes 2150 2350 0    50   ~ 0
Conmutador de energía
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
F0 "DC_DCs" 50
F1 "DC_DCs.sch" 50
$EndSheet
Text Notes 4750 3250 0    118  ~ 0
+12V\n+5V\n-5V\n+3V3
$EndSCHEMATC
