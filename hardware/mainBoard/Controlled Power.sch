EESchema Schematic File Version 4
LIBS:prockeckv2-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 9 10
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 2200 2700 0    50   Input ~ 0
HS_POW[0..1]
Wire Bus Line
	2200 2700 2500 2700
Entry Wire Line
	2500 2800 2600 2900
$Comp
L Device:R R59
U 1 1 5E03794B
P 3200 2900
F 0 "R59" V 2993 2900 50  0000 C CNN
F 1 "100k" V 3084 2900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3130 2900 50  0001 C CNN
F 3 "~" H 3200 2900 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_100KR-1003-1_C14675.html" H 3200 2900 50  0001 C CNN "URL"
	1    3200 2900
	0    1    1    0   
$EndComp
Wire Wire Line
	3350 2900 3400 2900
Wire Wire Line
	3050 2900 2600 2900
$Comp
L power:GND #PWR0116
U 1 1 5E038042
P 3700 3150
F 0 "#PWR0116" H 3700 2900 50  0001 C CNN
F 1 "GND" H 3705 2977 50  0000 C CNN
F 2 "" H 3700 3150 50  0001 C CNN
F 3 "" H 3700 3150 50  0001 C CNN
	1    3700 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 3150 3700 3100
$Comp
L Device:R R61
U 1 1 5E03822A
P 3700 2450
F 0 "R61" H 3630 2404 50  0000 R CNN
F 1 "100k" H 3630 2495 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3630 2450 50  0001 C CNN
F 3 "~" H 3700 2450 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_100KR-1003-1_C14675.html" H 3700 2450 50  0001 C CNN "URL"
	1    3700 2450
	-1   0    0    1   
$EndComp
Wire Wire Line
	3700 2600 3700 2650
$Comp
L power:+12V #PWR0115
U 1 1 5E0385B3
P 3700 2200
F 0 "#PWR0115" H 3700 2050 50  0001 C CNN
F 1 "+12V" H 3715 2373 50  0000 C CNN
F 2 "" H 3700 2200 50  0001 C CNN
F 3 "" H 3700 2200 50  0001 C CNN
	1    3700 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 2200 3700 2300
$Comp
L Device:Q_PMOS_GSD Q6
U 1 1 5E038F5D
P 4600 2650
F 0 "Q6" H 4806 2604 50  0000 L CNN
F 1 "AO3415" H 4806 2695 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23_Handsoldering" H 4800 2750 50  0001 C CNN
F 3 "~" H 4600 2650 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/MOSFET_Alpha-Omega-Semicon_AO3415_Alpha-Omega-Semicon-AOS-AO3415_C14354.html" H 4600 2650 50  0001 C CNN "URL"
	1    4600 2650
	1    0    0    1   
$EndComp
Wire Wire Line
	4400 2650 3700 2650
Connection ~ 3700 2650
Wire Wire Line
	3700 2650 3700 2700
$Comp
L power:+12V #PWR0119
U 1 1 5E03A22A
P 4700 2150
F 0 "#PWR0119" H 4700 2000 50  0001 C CNN
F 1 "+12V" H 4715 2323 50  0000 C CNN
F 2 "" H 4700 2150 50  0001 C CNN
F 3 "" H 4700 2150 50  0001 C CNN
	1    4700 2150
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J21
U 1 1 5E03AF80
P 4950 3000
F 0 "J21" H 5030 2992 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 5030 2901 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4950 3000 50  0001 C CNN
F 3 "~" H 4950 3000 50  0001 C CNN
	1    4950 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 3000 4700 3000
Wire Wire Line
	4700 3000 4700 2850
$Comp
L power:GND #PWR0120
U 1 1 5E03B573
P 4700 3150
F 0 "#PWR0120" H 4700 2900 50  0001 C CNN
F 1 "GND" H 4705 2977 50  0000 C CNN
F 2 "" H 4700 3150 50  0001 C CNN
F 3 "" H 4700 3150 50  0001 C CNN
	1    4700 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 3100 4700 3100
Wire Wire Line
	4700 3100 4700 3150
$Comp
L Device:R R60
U 1 1 5E03F80A
P 3200 4400
F 0 "R60" V 2993 4400 50  0000 C CNN
F 1 "100k" V 3084 4400 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3130 4400 50  0001 C CNN
F 3 "~" H 3200 4400 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_100KR-1003-1_C14675.html" H 3200 4400 50  0001 C CNN "URL"
	1    3200 4400
	0    1    1    0   
$EndComp
Wire Wire Line
	3350 4400 3400 4400
$Comp
L power:GND #PWR0118
U 1 1 5E03F811
P 3700 4700
F 0 "#PWR0118" H 3700 4450 50  0001 C CNN
F 1 "GND" H 3705 4527 50  0000 C CNN
F 2 "" H 3700 4700 50  0001 C CNN
F 3 "" H 3700 4700 50  0001 C CNN
	1    3700 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 4700 3700 4600
$Comp
L Device:R R62
U 1 1 5E03F818
P 3700 3950
F 0 "R62" H 3630 3904 50  0000 R CNN
F 1 "100k" H 3630 3995 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3630 3950 50  0001 C CNN
F 3 "~" H 3700 3950 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_100KR-1003-1_C14675.html" H 3700 3950 50  0001 C CNN "URL"
	1    3700 3950
	-1   0    0    1   
$EndComp
Wire Wire Line
	3700 4100 3700 4150
$Comp
L power:+12V #PWR0117
U 1 1 5E03F81F
P 3700 3650
F 0 "#PWR0117" H 3700 3500 50  0001 C CNN
F 1 "+12V" H 3715 3823 50  0000 C CNN
F 2 "" H 3700 3650 50  0001 C CNN
F 3 "" H 3700 3650 50  0001 C CNN
	1    3700 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 3650 3700 3800
$Comp
L Device:Q_PMOS_GSD Q7
U 1 1 5E03F826
P 4600 4150
F 0 "Q7" H 4806 4104 50  0000 L CNN
F 1 "AO3415" H 4806 4195 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23_Handsoldering" H 4800 4250 50  0001 C CNN
F 3 "~" H 4600 4150 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/MOSFET_Alpha-Omega-Semicon_AO3415_Alpha-Omega-Semicon-AOS-AO3415_C14354.html" H 4600 4150 50  0001 C CNN "URL"
	1    4600 4150
	1    0    0    1   
$EndComp
Wire Wire Line
	4400 4150 3700 4150
Connection ~ 3700 4150
Wire Wire Line
	3700 4150 3700 4200
$Comp
L power:+12V #PWR0121
U 1 1 5E03F82F
P 4700 3650
F 0 "#PWR0121" H 4700 3500 50  0001 C CNN
F 1 "+12V" H 4715 3823 50  0000 C CNN
F 2 "" H 4700 3650 50  0001 C CNN
F 3 "" H 4700 3650 50  0001 C CNN
	1    4700 3650
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J22
U 1 1 5E03F836
P 4950 4500
F 0 "J22" H 5030 4492 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 5030 4401 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4950 4500 50  0001 C CNN
F 3 "~" H 4950 4500 50  0001 C CNN
	1    4950 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 4500 4700 4500
Wire Wire Line
	4700 4500 4700 4350
$Comp
L power:GND #PWR0122
U 1 1 5E03F83E
P 4700 4650
F 0 "#PWR0122" H 4700 4400 50  0001 C CNN
F 1 "GND" H 4705 4477 50  0000 C CNN
F 2 "" H 4700 4650 50  0001 C CNN
F 3 "" H 4700 4650 50  0001 C CNN
	1    4700 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 4600 4700 4600
Wire Wire Line
	4700 4600 4700 4650
Entry Wire Line
	2500 4300 2600 4400
Wire Wire Line
	2600 4400 3050 4400
Text Label 2650 4400 0    50   ~ 0
HS_POW1
Text Label 2650 2900 0    50   ~ 0
HS_POW0
Text HLabel 6400 2700 0    50   Input ~ 0
LS_POW[0..1]
Wire Bus Line
	6400 2700 6700 2700
$Comp
L Device:Q_NMOS_GSD Q8
U 1 1 5E045891
P 7900 3200
F 0 "Q8" H 8106 3154 50  0000 L CNN
F 1 "AP2300" H 8106 3245 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23_Handsoldering" H 8100 3300 50  0001 C CNN
F 3 "~" H 7900 3200 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/MOSFET_ALLPOWER-ShenZhen-Quan-Li-Semiconductor-AP2300_C360338.html" H 7900 3200 50  0001 C CNN "URL"
	1    7900 3200
	1    0    0    1   
$EndComp
Entry Wire Line
	6700 3100 6800 3200
$Comp
L Device:R R63
U 1 1 5E047A0B
P 7450 3200
F 0 "R63" V 7243 3200 50  0000 C CNN
F 1 "10k" V 7334 3200 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7380 3200 50  0001 C CNN
F 3 "~" H 7450 3200 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/New-Arrivals_SANYEAR-SY0603JN10KP_C380762.html" H 7450 3200 50  0001 C CNN "URL"
	1    7450 3200
	0    1    1    0   
$EndComp
$Comp
L Device:R R65
U 1 1 5E047DD7
P 7650 3450
F 0 "R65" H 7580 3404 50  0000 R CNN
F 1 "1M" H 7580 3495 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7580 3450 50  0001 C CNN
F 3 "~" H 7650 3450 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAJ0105T5E_C26098.html" H 7650 3450 50  0001 C CNN "URL"
	1    7650 3450
	-1   0    0    1   
$EndComp
Wire Wire Line
	7600 3200 7650 3200
Wire Wire Line
	7650 3300 7650 3200
Connection ~ 7650 3200
Wire Wire Line
	7650 3200 7700 3200
Wire Wire Line
	7300 3200 6800 3200
$Comp
L power:GND #PWR0123
U 1 1 5E049032
P 7650 3650
F 0 "#PWR0123" H 7650 3400 50  0001 C CNN
F 1 "GND" H 7655 3477 50  0000 C CNN
F 2 "" H 7650 3650 50  0001 C CNN
F 3 "" H 7650 3650 50  0001 C CNN
	1    7650 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 3650 7650 3600
$Comp
L power:GND #PWR0126
U 1 1 5E0495AC
P 8000 3650
F 0 "#PWR0126" H 8000 3400 50  0001 C CNN
F 1 "GND" H 8005 3477 50  0000 C CNN
F 2 "" H 8000 3650 50  0001 C CNN
F 3 "" H 8000 3650 50  0001 C CNN
	1    8000 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 3650 8000 3400
$Comp
L Connector:Screw_Terminal_01x02 J23
U 1 1 5E04A12D
P 8250 2350
F 0 "J23" H 8330 2342 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 8330 2251 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 8250 2350 50  0001 C CNN
F 3 "~" H 8250 2350 50  0001 C CNN
	1    8250 2350
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR0125
U 1 1 5E04A68B
P 8000 2200
F 0 "#PWR0125" H 8000 2050 50  0001 C CNN
F 1 "+12V" H 8015 2373 50  0000 C CNN
F 2 "" H 8000 2200 50  0001 C CNN
F 3 "" H 8000 2200 50  0001 C CNN
	1    8000 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 2200 8000 2350
Wire Wire Line
	8000 2350 8050 2350
Wire Wire Line
	8000 2450 8050 2450
$Comp
L Device:Q_NMOS_GSD Q9
U 1 1 5E0525D0
P 7900 4800
F 0 "Q9" H 8106 4754 50  0000 L CNN
F 1 "AP2300" H 8106 4845 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23_Handsoldering" H 8100 4900 50  0001 C CNN
F 3 "~" H 7900 4800 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/MOSFET_ALLPOWER-ShenZhen-Quan-Li-Semiconductor-AP2300_C360338.html" H 7900 4800 50  0001 C CNN "URL"
	1    7900 4800
	1    0    0    1   
$EndComp
Entry Wire Line
	6700 4700 6800 4800
$Comp
L Device:R R64
U 1 1 5E0525D7
P 7450 4800
F 0 "R64" V 7243 4800 50  0000 C CNN
F 1 "10k" V 7334 4800 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7380 4800 50  0001 C CNN
F 3 "~" H 7450 4800 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/New-Arrivals_SANYEAR-SY0603JN10KP_C380762.html" H 7450 4800 50  0001 C CNN "URL"
	1    7450 4800
	0    1    1    0   
$EndComp
$Comp
L Device:R R66
U 1 1 5E0525DD
P 7650 5050
F 0 "R66" H 7580 5004 50  0000 R CNN
F 1 "1M" H 7580 5095 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7580 5050 50  0001 C CNN
F 3 "~" H 7650 5050 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAJ0105T5E_C26098.html" H 7650 5050 50  0001 C CNN "URL"
	1    7650 5050
	-1   0    0    1   
$EndComp
Wire Wire Line
	7600 4800 7650 4800
Wire Wire Line
	7650 4900 7650 4800
Connection ~ 7650 4800
Wire Wire Line
	7650 4800 7700 4800
Wire Wire Line
	7300 4800 6800 4800
$Comp
L power:GND #PWR0124
U 1 1 5E0525E8
P 7650 5250
F 0 "#PWR0124" H 7650 5000 50  0001 C CNN
F 1 "GND" H 7655 5077 50  0000 C CNN
F 2 "" H 7650 5250 50  0001 C CNN
F 3 "" H 7650 5250 50  0001 C CNN
	1    7650 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 5250 7650 5200
$Comp
L power:GND #PWR0128
U 1 1 5E0525EF
P 8000 5250
F 0 "#PWR0128" H 8000 5000 50  0001 C CNN
F 1 "GND" H 8005 5077 50  0000 C CNN
F 2 "" H 8000 5250 50  0001 C CNN
F 3 "" H 8000 5250 50  0001 C CNN
	1    8000 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 5250 8000 5000
$Comp
L Connector:Screw_Terminal_01x02 J24
U 1 1 5E0525F6
P 8300 4250
F 0 "J24" H 8380 4242 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 8380 4151 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 8300 4250 50  0001 C CNN
F 3 "~" H 8300 4250 50  0001 C CNN
	1    8300 4250
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR0127
U 1 1 5E0525FC
P 8000 4250
F 0 "#PWR0127" H 8000 4100 50  0001 C CNN
F 1 "+12V" H 8015 4423 50  0000 C CNN
F 2 "" H 8000 4250 50  0001 C CNN
F 3 "" H 8000 4250 50  0001 C CNN
	1    8000 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 4250 8100 4250
Wire Wire Line
	8000 4350 8100 4350
Text Label 6850 3200 0    50   ~ 0
LS_POW0
Text Label 6850 4800 0    50   ~ 0
LS_POW1
$Comp
L Device:Polyfuse_Small F1
U 1 1 5DF34AF1
P 4700 2300
F 0 "F1" H 4768 2346 50  0000 L CNN
F 1 "SL1210400" H 4768 2255 50  0000 L CNN
F 2 "Resistor_SMD:R_1210_3225Metric" H 4750 2100 50  0001 L CNN
F 3 "~" H 4700 2300 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/PTC-Resettable-Fuses_TECHFUSE-SL1210400_C70186.html" H 4700 2300 50  0001 C CNN "URL"
	1    4700 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 2200 4700 2150
Wire Wire Line
	4700 2400 4700 2450
$Comp
L Device:Polyfuse_Small F2
U 1 1 5DF47C24
P 4700 3800
F 0 "F2" H 4768 3846 50  0000 L CNN
F 1 "SL1210400" H 4768 3755 50  0000 L CNN
F 2 "Resistor_SMD:R_1210_3225Metric" H 4750 3600 50  0001 L CNN
F 3 "~" H 4700 3800 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/PTC-Resettable-Fuses_TECHFUSE-SL1210400_C70186.html" H 4700 3800 50  0001 C CNN "URL"
	1    4700 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 3700 4700 3650
Wire Wire Line
	4700 3900 4700 3950
Wire Wire Line
	8000 2900 8000 3000
Wire Wire Line
	8000 2700 8000 2450
$Comp
L Device:Polyfuse_Small F4
U 1 1 5DF6F84A
P 8000 4450
F 0 "F4" H 8068 4496 50  0000 L CNN
F 1 "SL1210400" H 8068 4405 50  0000 L CNN
F 2 "Resistor_SMD:R_1210_3225Metric" H 8050 4250 50  0001 L CNN
F 3 "~" H 8000 4450 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/PTC-Resettable-Fuses_TECHFUSE-SL1210400_C70186.html" H 8000 4450 50  0001 C CNN "URL"
	1    8000 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 4550 8000 4600
$Comp
L Device:Polyfuse_Small F3
U 1 1 5DF65539
P 8000 2800
F 0 "F3" H 8068 2846 50  0000 L CNN
F 1 "SL1210400" H 8068 2755 50  0000 L CNN
F 2 "Resistor_SMD:R_1210_3225Metric" H 8050 2600 50  0001 L CNN
F 3 "~" H 8000 2800 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/PTC-Resettable-Fuses_TECHFUSE-SL1210400_C70186.html" H 8000 2800 50  0001 C CNN "URL"
	1    8000 2800
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_NPN_BEC Q4
U 1 1 5DFF9ED3
P 3600 2900
F 0 "Q4" H 3790 2946 50  0000 L CNN
F 1 "S8050" H 3790 2855 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23_Handsoldering" H 3800 3000 50  0001 C CNN
F 3 "~" H 3600 2900 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Transistors-NPN-PNP_S8050_C181158.html" H 3600 2900 50  0001 C CNN "URL"
	1    3600 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_NPN_BEC Q5
U 1 1 5DFFA34E
P 3600 4400
F 0 "Q5" H 3790 4446 50  0000 L CNN
F 1 "S8050" H 3790 4355 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23_Handsoldering" H 3800 4500 50  0001 C CNN
F 3 "~" H 3600 4400 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Transistors-NPN-PNP_S8050_C181158.html" H 3600 4400 50  0001 C CNN "URL"
	1    3600 4400
	1    0    0    -1  
$EndComp
Wire Bus Line
	6700 2700 6700 4700
Wire Bus Line
	2500 2700 2500 4300
$EndSCHEMATC