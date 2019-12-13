EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 10
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
S 7300 3400 1450 350 
U 5DCBE02B
F0 "IO" 50
F1 "IO.sch" 50
F2 "IO[0..14]" B L 7300 3550 50 
F3 "DAC" I L 7300 3450 50 
F4 "PWM[0..1]" I L 7300 3650 50 
$EndSheet
$Sheet
S 7150 2850 1500 350 
U 5DE32C3C
F0 "Peripherals" 50
F1 "Peripherals.sch" 50
F2 "SPI1_[0..2]" B L 7150 2900 50 
F3 "IO2[0..10]" B L 7150 3000 50 
F4 "SDIO[0..5]" B L 7150 3100 50 
$EndSheet
$Comp
L Connector:USB_B_Micro J1
U 1 1 5C742C3D
P 2850 3200
F 0 "J1" H 2905 3667 50  0000 C CNN
F 1 "USB_B_Micro" H 2905 3576 50  0000 C CNN
F 2 "Connector_USB:USB_Micro-B_Molex-105017-0001" H 3000 3150 50  0001 C CNN
F 3 "~" H 3000 3150 50  0001 C CNN
	1    2850 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 3000 3400 3000
$Comp
L power:GND #PWR01
U 1 1 5CA32110
P 2850 3700
F 0 "#PWR01" H 2850 3450 50  0001 C CNN
F 1 "GND" H 2855 3527 50  0000 C CNN
F 2 "" H 2850 3700 50  0001 C CNN
F 3 "" H 2850 3700 50  0001 C CNN
	1    2850 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 3600 2750 3650
Wire Wire Line
	2750 3650 2850 3650
Text Notes 2600 2550 0    50   ~ 0
Interfaz USB
Wire Wire Line
	2850 3600 2850 3650
Connection ~ 2850 3650
NoConn ~ 3150 3400
$Sheet
S 8600 650  2400 1100
U 5E851D3B
F0 "Power" 50
F1 "Power.sch" 50
$EndSheet
Text Notes 9500 1500 0    118  ~ 0
+12V\n+5V\n-5V\n+3V3
$Sheet
S 5350 2850 1250 2200
U 5DA8B375
F0 "MCU" 50
F1 "MCU.sch" 50
F2 "SPI1_[0..2]" B R 6600 2900 50 
F3 "IO2[0..10]" B R 6600 3000 50 
F4 "SDIO[0..5]" B R 6600 3100 50 
F5 "ADC_se[0..5]" I L 5350 4150 50 
F6 "ADC_diff[0..5]" I L 5350 4950 50 
F7 "IO[0..14]" B R 6600 3550 50 
F8 "DAC" O R 6600 3450 50 
F9 "USART1_[0..1]" B R 6600 4300 50 
F10 "SWD[0..1]" B R 6600 4600 50 
F11 "SPI3_[0..2]" B R 6600 5000 50 
F12 "I2C1_[0..1]" B R 6600 4700 50 
F13 "SPI2_[0..2]" B R 6600 4900 50 
F14 "I2C2_[0..1]" B R 6600 4800 50 
F15 "USART2_[0..1]" B R 6600 4400 50 
F16 "USART3_[0..1]" B R 6600 4500 50 
F17 "LS_POW[0..1]" O L 5350 3900 50 
F18 "SDI12[0..2]" B L 5350 4500 50 
F19 "HS_POW[0..1]" O L 5350 3800 50 
F20 "PWM[0..1]" O R 6600 3650 50 
$EndSheet
$Sheet
S 2050 4000 1050 300 
U 5E24374F
F0 "Analog Conditioning" 50
F1 "AnalogConditioning.sch" 50
F2 "ADC_se[0..5]" O R 3100 4150 50 
$EndSheet
Wire Bus Line
	3100 4150 5350 4150
$Sheet
S 2100 4800 1050 350 
U 5DF4C831
F0 "DifferentialConditioning.sch" 50
F1 "DifferentialConditioning.sch" 50
F2 "ADC_diff[0..5]" O R 3150 4950 50 
$EndSheet
Wire Bus Line
	6600 3550 7300 3550
Wire Wire Line
	6600 3450 7300 3450
Wire Bus Line
	3150 4950 5350 4950
Wire Bus Line
	6600 2900 7150 2900
Wire Bus Line
	6600 3000 7150 3000
Wire Bus Line
	6600 3100 7150 3100
Wire Bus Line
	6600 3650 7300 3650
$Sheet
S 4300 4400 550  200 
U 5DF01F26
F0 "SDI12" 50
F1 "SDI12.sch" 50
F2 "SDI12[0..2]" B R 4850 4500 50 
$EndSheet
Wire Bus Line
	4850 4500 5350 4500
$Sheet
S 3650 3650 700  350 
U 5E0359E0
F0 "Controlled Power" 50
F1 "Controlled Power.sch" 50
F2 "HS_POW[0..1]" I R 4350 3800 50 
F3 "LS_POW[0..1]" I R 4350 3900 50 
$EndSheet
Wire Bus Line
	4350 3800 5350 3800
Wire Bus Line
	4350 3900 5350 3900
Wire Bus Line
	6600 4600 7450 4600
Wire Bus Line
	6600 5000 7450 5000
Wire Bus Line
	6600 4900 7450 4900
Wire Bus Line
	6600 4800 7450 4800
Wire Bus Line
	6600 4700 7450 4700
Wire Bus Line
	6600 4500 7450 4500
Wire Bus Line
	6600 4400 7450 4400
Wire Bus Line
	6600 4300 7450 4300
$Sheet
S 7450 4250 1550 850 
U 5E56C3C6
F0 "External Connections" 50
F1 "ExternalConnections.sch" 50
F2 "USART1_[0..1]" B L 7450 4300 50 
F3 "USART2_[0..1]" B L 7450 4400 50 
F4 "USART3_[0..1]" B L 7450 4500 50 
F5 "I2C1_[0..1]" B L 7450 4700 50 
F6 "I2C2_[0..1]" B L 7450 4800 50 
F7 "SPI2_[0..2]" B L 7450 4900 50 
F8 "SPI3_[0..2]" B L 7450 5000 50 
F9 "SWD[0..1]" B L 7450 4600 50 
$EndSheet
Text Notes 3350 2800 0    50   ~ 0
USB0 = USB_D+\nUSB1 = USB_D-
NoConn ~ 3400 3000
Wire Wire Line
	2850 3700 2850 3650
Text GLabel 3250 3200 2    50   BiDi ~ 0
USB_D+
Text GLabel 3250 3300 2    50   BiDi ~ 0
USB_D-
Wire Wire Line
	3250 3300 3150 3300
Wire Wire Line
	3150 3200 3250 3200
$EndSCHEMATC
