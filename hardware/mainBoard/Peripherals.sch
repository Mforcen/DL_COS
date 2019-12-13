EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 10
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
L Chinaland:MicroSD J5
U 1 1 5DEE176A
P 4550 2650
F 0 "J5" H 4550 3367 50  0000 C CNN
F 1 "MicroSD" H 4550 3276 50  0000 C CNN
F 2 "chinalandfoot:MicroSD-KoreanHroparts" H 4550 2650 50  0001 C CNN
F 3 "" H 4550 2650 50  0001 C CNN
	1    4550 2650
	1    0    0    -1  
$EndComp
$Comp
L Chinaland:E19-868M30S U2
U 1 1 5DEE3C98
P 4250 5500
F 0 "U2" H 4275 6215 50  0000 C CNN
F 1 "E19-868M30S" H 4275 6124 50  0000 C CNN
F 2 "chinalandfoot:E19-868MS1W" H 4250 5450 50  0001 C CNN
F 3 "" H 4250 5450 50  0001 C CNN
	1    4250 5500
	1    0    0    -1  
$EndComp
Text HLabel 2050 4400 0    50   BiDi ~ 0
SPI1_[0..2]
Text HLabel 1600 6150 0    50   BiDi ~ 0
IO2[0..10]
Wire Bus Line
	2050 4400 2250 4400
Entry Wire Line
	2250 4300 2350 4400
Entry Wire Line
	2250 4200 2350 4300
Entry Wire Line
	2250 4100 2350 4200
Wire Wire Line
	2350 4300 3300 4300
Wire Wire Line
	3050 4750 3050 4400
Wire Wire Line
	3050 4400 2350 4400
Wire Wire Line
	2350 4200 3050 4200
Wire Wire Line
	3050 4200 3050 3850
Wire Wire Line
	3050 3850 4600 3850
Wire Wire Line
	4600 3850 4600 4400
Text Label 2400 4200 0    50   ~ 0
SPI1_0
Text Label 2400 4300 0    50   ~ 0
SPI1_1
Text Label 2400 4400 0    50   ~ 0
SPI1_2
$Comp
L power:GND #PWR07
U 1 1 5DF073AA
P 3600 4500
F 0 "#PWR07" H 3600 4250 50  0001 C CNN
F 1 "GND" V 3605 4372 50  0000 R CNN
F 2 "" H 3600 4500 50  0001 C CNN
F 3 "" H 3600 4500 50  0001 C CNN
	1    3600 4500
	0    1    1    0   
$EndComp
Wire Wire Line
	3600 4500 3650 4500
Wire Bus Line
	1600 6150 2200 6150
Entry Wire Line
	2200 4950 2300 5050
Wire Wire Line
	3650 4200 3100 4200
Wire Wire Line
	3100 5050 2300 5050
$Comp
L power:+3V3 #PWR013
U 1 1 5DF0A362
P 4450 4150
F 0 "#PWR013" H 4450 4000 50  0001 C CNN
F 1 "+3V3" H 4465 4323 50  0000 C CNN
F 2 "" H 4450 4150 50  0001 C CNN
F 3 "" H 4450 4150 50  0001 C CNN
	1    4450 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 4150 4450 4200
Wire Wire Line
	4450 4200 4350 4200
NoConn ~ 3650 4400
NoConn ~ 4350 4300
Wire Wire Line
	3100 4200 3100 5050
$Comp
L power:GND #PWR014
U 1 1 5DF6D39B
P 4850 5150
F 0 "#PWR014" H 4850 4900 50  0001 C CNN
F 1 "GND" V 4855 5022 50  0000 R CNN
F 2 "" H 4850 5150 50  0001 C CNN
F 3 "" H 4850 5150 50  0001 C CNN
	1    4850 5150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4850 5150 4800 5150
Wire Wire Line
	4750 5050 4800 5050
Wire Wire Line
	4800 5050 4800 5150
Connection ~ 4800 5150
Wire Wire Line
	4800 5150 4750 5150
Wire Wire Line
	4750 5250 4800 5250
Wire Wire Line
	4800 5250 4800 5150
$Comp
L power:GND #PWR011
U 1 1 5DF6FC7C
P 3700 5050
F 0 "#PWR011" H 3700 4800 50  0001 C CNN
F 1 "GND" V 3705 4922 50  0000 R CNN
F 2 "" H 3700 5050 50  0001 C CNN
F 3 "" H 3700 5050 50  0001 C CNN
	1    3700 5050
	0    1    1    0   
$EndComp
Wire Wire Line
	3700 5050 3800 5050
$Comp
L power:GND #PWR015
U 1 1 5DF70374
P 4850 5450
F 0 "#PWR015" H 4850 5200 50  0001 C CNN
F 1 "GND" V 4855 5322 50  0000 R CNN
F 2 "" H 4850 5450 50  0001 C CNN
F 3 "" H 4850 5450 50  0001 C CNN
	1    4850 5450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4850 5450 4750 5450
$Comp
L power:GND #PWR010
U 1 1 5DF70E78
P 3650 5950
F 0 "#PWR010" H 3650 5700 50  0001 C CNN
F 1 "GND" V 3655 5822 50  0000 R CNN
F 2 "" H 3650 5950 50  0001 C CNN
F 3 "" H 3650 5950 50  0001 C CNN
	1    3650 5950
	0    1    1    0   
$EndComp
Wire Wire Line
	3650 5950 3700 5950
Wire Wire Line
	3800 5850 3700 5850
Wire Wire Line
	3700 5850 3700 5950
Connection ~ 3700 5950
Wire Wire Line
	3700 5950 3800 5950
Entry Wire Line
	2200 5250 2300 5350
Entry Wire Line
	2200 5350 2300 5450
Entry Wire Line
	2200 5450 2300 5550
Entry Wire Line
	2200 5550 2300 5650
Entry Wire Line
	2200 5650 2300 5750
Text Label 2350 5750 0    50   ~ 0
IO21
Text Label 2350 5650 0    50   ~ 0
IO23
Text Label 2350 5550 0    50   ~ 0
IO24
Text Label 2350 5450 0    50   ~ 0
IO25
Text Label 2350 5350 0    50   ~ 0
IO26
$Comp
L power:+5V #PWR012
U 1 1 5DF79F10
P 3700 6050
F 0 "#PWR012" H 3700 5900 50  0001 C CNN
F 1 "+5V" V 3715 6178 50  0000 L CNN
F 2 "" H 3700 6050 50  0001 C CNN
F 3 "" H 3700 6050 50  0001 C CNN
	1    3700 6050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3700 6050 3800 6050
Entry Wire Line
	2200 5750 2300 5850
Entry Wire Line
	2200 5850 2300 5950
Entry Wire Line
	2200 5950 2300 6050
Wire Wire Line
	4750 6050 5050 6050
Wire Wire Line
	5350 6050 5350 4400
Wire Wire Line
	5350 4400 4600 4400
Wire Wire Line
	4750 5950 5300 5950
Wire Wire Line
	5300 5950 5300 4650
Wire Wire Line
	5300 4650 3300 4650
Wire Wire Line
	3300 4650 3300 4300
Connection ~ 3300 4300
Wire Wire Line
	3300 4300 3650 4300
Wire Wire Line
	4750 5850 5100 5850
Wire Wire Line
	5250 5850 5250 4750
Wire Wire Line
	4750 5750 4800 5750
Wire Wire Line
	4800 5750 4800 6250
Wire Wire Line
	4800 6250 3150 6250
Wire Wire Line
	3150 6250 3150 5850
Wire Wire Line
	3150 5850 2300 5850
Wire Wire Line
	2300 5950 3100 5950
Wire Wire Line
	3100 5950 3100 6300
Wire Wire Line
	3100 6300 4850 6300
Wire Wire Line
	4850 6300 4850 5650
Wire Wire Line
	4850 5650 4750 5650
Wire Wire Line
	4750 5550 4900 5550
Wire Wire Line
	4900 5550 4900 6350
Wire Wire Line
	4900 6350 3050 6350
Wire Wire Line
	3050 6350 3050 6050
Wire Wire Line
	3050 6050 2300 6050
Text Label 2350 5850 0    50   ~ 0
IO22
Text Label 2350 5950 0    50   ~ 0
IO27
Text Label 2350 6050 0    50   ~ 0
IO28
NoConn ~ 3800 5150
NoConn ~ 3800 5250
Wire Wire Line
	2300 5350 3800 5350
Wire Wire Line
	2300 5450 3800 5450
Wire Wire Line
	2300 5550 3800 5550
Wire Wire Line
	2300 5650 3800 5650
Wire Wire Line
	2300 5750 3800 5750
$Comp
L power:+3V3 #PWR017
U 1 1 5DFE0D8D
P 5500 6650
F 0 "#PWR017" H 5500 6500 50  0001 C CNN
F 1 "+3V3" V 5515 6778 50  0000 L CNN
F 2 "" H 5500 6650 50  0001 C CNN
F 3 "" H 5500 6650 50  0001 C CNN
	1    5500 6650
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR018
U 1 1 5DFE2A70
P 5500 6750
F 0 "#PWR018" H 5500 6500 50  0001 C CNN
F 1 "GND" V 5505 6622 50  0000 R CNN
F 2 "" H 5500 6750 50  0001 C CNN
F 3 "" H 5500 6750 50  0001 C CNN
	1    5500 6750
	0    1    1    0   
$EndComp
Entry Wire Line
	2200 6050 2300 6150
Entry Wire Line
	2200 6150 2300 6250
$Comp
L Connector:Conn_01x06_Male J6
U 1 1 5E01FA1B
P 5750 6850
F 0 "J6" H 5722 6824 50  0000 R CNN
F 1 "Conn_01x06_Male" H 5722 6733 50  0000 R CNN
F 2 "Connector_JST:JST_XH_B6B-XH-A_1x06_P2.50mm_Vertical" H 5750 6850 50  0001 C CNN
F 3 "~" H 5750 6850 50  0001 C CNN
	1    5750 6850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2300 6150 3000 6150
Wire Wire Line
	2300 6250 2950 6250
Text Label 2350 6150 0    50   ~ 0
IO29
Text Label 2350 6250 0    50   ~ 0
IO210
Wire Wire Line
	5500 6750 5550 6750
Wire Wire Line
	5500 6650 5550 6650
Wire Wire Line
	5550 6850 3000 6850
Wire Wire Line
	3000 6150 3000 6850
Wire Wire Line
	5550 7050 2950 7050
Wire Wire Line
	2950 6250 2950 7050
Wire Wire Line
	5100 5850 5100 6950
Wire Wire Line
	5100 6950 5550 6950
Connection ~ 5100 5850
Wire Wire Line
	5100 5850 5250 5850
Wire Wire Line
	5050 6050 5050 7150
Wire Wire Line
	5050 7150 5550 7150
Connection ~ 5050 6050
Wire Wire Line
	5050 6050 5350 6050
Text HLabel 2700 3200 0    50   BiDi ~ 0
SDIO[0..5]
Text Label 3100 2350 0    50   ~ 0
SDIO2
Text Label 3100 2450 0    50   ~ 0
SDIO3
Text Label 3100 2550 0    50   ~ 0
SDIO5
Text Label 3100 2750 0    50   ~ 0
SDIO4
Text Label 3100 2950 0    50   ~ 0
SDIO0
Text Label 3100 3050 0    50   ~ 0
SDIO1
NoConn ~ 3700 3150
Wire Wire Line
	3050 3050 3700 3050
Wire Wire Line
	3700 2950 3050 2950
Wire Wire Line
	3050 2750 3700 2750
Wire Wire Line
	3700 2550 3050 2550
Wire Wire Line
	3050 2450 3700 2450
Wire Wire Line
	3700 2350 3050 2350
Wire Bus Line
	2700 3200 2950 3200
Entry Wire Line
	2950 2950 3050 3050
Entry Wire Line
	2950 2850 3050 2950
Entry Wire Line
	2950 2650 3050 2750
Entry Wire Line
	2950 2450 3050 2550
Entry Wire Line
	2950 2350 3050 2450
Entry Wire Line
	2950 2250 3050 2350
Wire Wire Line
	3650 2850 3700 2850
$Comp
L power:GND #PWR09
U 1 1 5E08C40A
P 3650 2850
F 0 "#PWR09" H 3650 2600 50  0001 C CNN
F 1 "GND" V 3655 2722 50  0000 R CNN
F 2 "" H 3650 2850 50  0001 C CNN
F 3 "" H 3650 2850 50  0001 C CNN
	1    3650 2850
	0    1    1    0   
$EndComp
Wire Wire Line
	3650 2650 3700 2650
$Comp
L power:+3V3 #PWR08
U 1 1 5E0895DC
P 3650 2650
F 0 "#PWR08" H 3650 2500 50  0001 C CNN
F 1 "+3V3" V 3665 2778 50  0000 L CNN
F 2 "" H 3650 2650 50  0001 C CNN
F 3 "" H 3650 2650 50  0001 C CNN
	1    3650 2650
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR016
U 1 1 5E0B2235
P 5500 3250
F 0 "#PWR016" H 5500 3000 50  0001 C CNN
F 1 "GND" V 5505 3122 50  0000 R CNN
F 2 "" H 5500 3250 50  0001 C CNN
F 3 "" H 5500 3250 50  0001 C CNN
	1    5500 3250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5400 3250 5500 3250
Text Label 2350 5050 0    50   ~ 0
IO20
Text Notes 1400 4200 0    50   ~ 0
SPI_0 = SCK\nSPI_1 = MISO\nSPI_2 = MOSI
Text Notes 2450 2800 2    50   ~ 0
SDIO_0-3 =  0-3\nSDIO_CK = 4\nSDIO_CMD = 5
$Comp
L Connector:Conn_01x01_Male J25
U 1 1 5E360DF2
P 5000 5350
F 0 "J25" H 4972 5282 50  0000 R CNN
F 1 "Conn_01x01_Male" H 4972 5373 50  0000 R CNN
F 2 "Connector_Wire:SolderWirePad_1x01_SMD_5x10mm" H 5000 5350 50  0001 C CNN
F 3 "~" H 5000 5350 50  0001 C CNN
	1    5000 5350
	-1   0    0    1   
$EndComp
Wire Wire Line
	4800 5350 4750 5350
Wire Wire Line
	4600 4400 4350 4400
Connection ~ 4600 4400
Wire Wire Line
	4350 4500 4450 4500
Wire Wire Line
	4450 4750 5250 4750
Wire Wire Line
	3050 4750 4450 4750
Connection ~ 4450 4750
Wire Wire Line
	4450 4500 4450 4750
$Comp
L Chinaland:W25Q64JV U1
U 1 1 5E52ADCC
P 4000 4350
F 0 "U1" H 4000 4765 50  0000 C CNN
F 1 "W25Q64JV" H 4000 4674 50  0000 C CNN
F 2 "Package_SO:SOIC-8_5.23x5.23mm_P1.27mm" H 4000 4350 50  0001 C CNN
F 3 "" H 4000 4350 50  0001 C CNN
	1    4000 4350
	1    0    0    -1  
$EndComp
Wire Bus Line
	2250 4100 2250 4400
Wire Bus Line
	2950 2250 2950 3200
Wire Bus Line
	2200 4950 2200 6150
$EndSCHEMATC
