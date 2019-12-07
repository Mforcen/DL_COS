EESchema Schematic File Version 4
LIBS:prockeckv2-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 5
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
L prockeckv2-rescue:SGM3204-chinaland U?
U 1 1 5E85D6AB
P 2500 1300
AR Path="/5E85D6AB" Ref="U?"  Part="1" 
AR Path="/5E851D3B/5E85D6AB" Ref="U?"  Part="1" 
F 0 "U?" H 2500 1715 50  0000 C CNN
F 1 "SGM3204" H 2500 1624 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6_Handsoldering" H 2500 1300 50  0001 C CNN
F 3 "" H 2500 1300 50  0001 C CNN
	1    2500 1300
	1    0    0    -1  
$EndComp
$Comp
L power:-5V #PWR?
U 1 1 5E85D6B2
P 1200 1150
AR Path="/5E85D6B2" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E85D6B2" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1200 1250 50  0001 C CNN
F 1 "-5V" V 1215 1278 50  0000 L CNN
F 2 "" H 1200 1150 50  0001 C CNN
F 3 "" H 1200 1150 50  0001 C CNN
	1    1200 1150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1200 1150 1300 1150
$Comp
L power:+5V #PWR?
U 1 1 5E85D6B9
P 1650 1300
AR Path="/5E85D6B9" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E85D6B9" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1650 1150 50  0001 C CNN
F 1 "+5V" V 1665 1428 50  0000 L CNN
F 2 "" H 1650 1300 50  0001 C CNN
F 3 "" H 1650 1300 50  0001 C CNN
	1    1650 1300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1650 1300 1800 1300
$Comp
L Device:C C?
U 1 1 5E85D6C0
P 2450 1850
AR Path="/5E85D6C0" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E85D6C0" Ref="C?"  Part="1" 
F 0 "C?" V 2702 1850 50  0000 C CNN
F 1 "3.3u" V 2611 1850 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2488 1700 50  0001 C CNN
F 3 "~" H 2450 1850 50  0001 C CNN
	1    2450 1850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2050 1450 1950 1450
Wire Wire Line
	1950 1450 1950 1850
Wire Wire Line
	1950 1850 2300 1850
Wire Wire Line
	2600 1850 3400 1850
Wire Wire Line
	3400 1850 3400 1150
Wire Wire Line
	3400 1150 2950 1150
$Comp
L Device:R R?
U 1 1 5E85D6CD
P 3150 1300
AR Path="/5E85D6CD" Ref="R?"  Part="1" 
AR Path="/5E851D3B/5E85D6CD" Ref="R?"  Part="1" 
F 0 "R?" V 2943 1300 50  0000 C CNN
F 1 "10k" V 3034 1300 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3080 1300 50  0001 C CNN
F 3 "~" H 3150 1300 50  0001 C CNN
	1    3150 1300
	0    1    1    0   
$EndComp
Wire Wire Line
	3000 1300 2950 1300
Text GLabel 3500 1300 2    50   Input ~ 0
-5VEN
Wire Wire Line
	3500 1300 3350 1300
$Comp
L power:GND #PWR?
U 1 1 5E85D6D7
P 3000 1500
AR Path="/5E85D6D7" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E85D6D7" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3000 1250 50  0001 C CNN
F 1 "GND" H 3005 1327 50  0000 C CNN
F 2 "" H 3000 1500 50  0001 C CNN
F 3 "" H 3000 1500 50  0001 C CNN
	1    3000 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 1450 3000 1450
Wire Wire Line
	3000 1450 3000 1500
Text Notes 1900 800  0    50   ~ 0
Generador de Voltaje Negativo
$Comp
L Device:C C?
U 1 1 5E85D6E0
P 1800 1550
AR Path="/5E85D6E0" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E85D6E0" Ref="C?"  Part="1" 
F 0 "C?" V 2052 1550 50  0000 C CNN
F 1 "3.3u" V 1961 1550 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 1838 1400 50  0001 C CNN
F 3 "~" H 1800 1550 50  0001 C CNN
	1    1800 1550
	-1   0    0    1   
$EndComp
$Comp
L Device:C C?
U 1 1 5E85D6E7
P 1300 1550
AR Path="/5E85D6E7" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E85D6E7" Ref="C?"  Part="1" 
F 0 "C?" V 1552 1550 50  0000 C CNN
F 1 "3.3u" V 1461 1550 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 1338 1400 50  0001 C CNN
F 3 "~" H 1300 1550 50  0001 C CNN
	1    1300 1550
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E85D6EE
P 1800 1800
AR Path="/5E85D6EE" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E85D6EE" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1800 1550 50  0001 C CNN
F 1 "GND" H 1805 1627 50  0000 C CNN
F 2 "" H 1800 1800 50  0001 C CNN
F 3 "" H 1800 1800 50  0001 C CNN
	1    1800 1800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E85D6F4
P 1300 1800
AR Path="/5E85D6F4" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E85D6F4" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1300 1550 50  0001 C CNN
F 1 "GND" H 1305 1627 50  0000 C CNN
F 2 "" H 1300 1800 50  0001 C CNN
F 3 "" H 1300 1800 50  0001 C CNN
	1    1300 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 1800 1300 1700
Wire Wire Line
	1800 1800 1800 1700
Wire Wire Line
	1800 1400 1800 1300
Connection ~ 1800 1300
Wire Wire Line
	1800 1300 2050 1300
Wire Wire Line
	1300 1400 1300 1150
Connection ~ 1300 1150
Wire Wire Line
	1300 1150 2050 1150
$Comp
L Device:R R?
U 1 1 5E85D702
P 3150 1450
AR Path="/5E85D702" Ref="R?"  Part="1" 
AR Path="/5E851D3B/5E85D702" Ref="R?"  Part="1" 
F 0 "R?" V 3350 1450 50  0000 C CNN
F 1 "10k" V 3250 1450 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3080 1450 50  0001 C CNN
F 3 "~" H 3150 1450 50  0001 C CNN
	1    3150 1450
	0    1    1    0   
$EndComp
Connection ~ 3000 1450
Wire Wire Line
	3300 1450 3350 1450
Wire Wire Line
	3350 1450 3350 1300
Connection ~ 3350 1300
Wire Wire Line
	3350 1300 3300 1300
$Comp
L Regulator_Linear:AMS1117-3.3 U?
U 1 1 5E871E9F
P 2150 2650
AR Path="/5E871E9F" Ref="U?"  Part="1" 
AR Path="/5E851D3B/5E871E9F" Ref="U?"  Part="1" 
F 0 "U?" H 2150 2892 50  0000 C CNN
F 1 "AMS1117-3.3" H 2150 2801 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 2150 2850 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 2250 2400 50  0001 C CNN
	1    2150 2650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E871EA6
P 2150 3150
AR Path="/5E871EA6" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E871EA6" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2150 2900 50  0001 C CNN
F 1 "GND" H 2155 2977 50  0000 C CNN
F 2 "" H 2150 3150 50  0001 C CNN
F 3 "" H 2150 3150 50  0001 C CNN
	1    2150 3150
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5E871EAC
P 1600 2900
AR Path="/5E871EAC" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E871EAC" Ref="C?"  Part="1" 
F 0 "C?" H 1715 2946 50  0000 L CNN
F 1 "10u" H 1715 2855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 1638 2750 50  0001 C CNN
F 3 "~" H 1600 2900 50  0001 C CNN
	1    1600 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E871EB3
P 1600 3150
AR Path="/5E871EB3" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E871EB3" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1600 2900 50  0001 C CNN
F 1 "GND" H 1605 2977 50  0000 C CNN
F 2 "" H 1600 3150 50  0001 C CNN
F 3 "" H 1600 3150 50  0001 C CNN
	1    1600 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 3150 1600 3050
Wire Wire Line
	1600 2750 1600 2650
Wire Wire Line
	1600 2650 1850 2650
Wire Wire Line
	1400 2650 1600 2650
Connection ~ 1600 2650
Wire Wire Line
	2150 3150 2150 2950
$Comp
L Device:C C?
U 1 1 5E871EBF
P 2650 2900
AR Path="/5E871EBF" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E871EBF" Ref="C?"  Part="1" 
F 0 "C?" H 2765 2946 50  0000 L CNN
F 1 "22u" H 2765 2855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2688 2750 50  0001 C CNN
F 3 "~" H 2650 2900 50  0001 C CNN
	1    2650 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E871EC6
P 2650 3150
AR Path="/5E871EC6" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E871EC6" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2650 2900 50  0001 C CNN
F 1 "GND" H 2655 2977 50  0000 C CNN
F 2 "" H 2650 3150 50  0001 C CNN
F 3 "" H 2650 3150 50  0001 C CNN
	1    2650 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 3150 2650 3050
Wire Wire Line
	2450 2650 2650 2650
Wire Wire Line
	2650 2650 2650 2750
$Comp
L Device:C C?
U 1 1 5E871ECF
P 3050 2900
AR Path="/5E871ECF" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E871ECF" Ref="C?"  Part="1" 
F 0 "C?" H 3165 2946 50  0000 L CNN
F 1 "100n" H 3165 2855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 3088 2750 50  0001 C CNN
F 3 "~" H 3050 2900 50  0001 C CNN
	1    3050 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 2650 3050 2650
Wire Wire Line
	3050 2650 3050 2750
Connection ~ 2650 2650
$Comp
L power:GND #PWR?
U 1 1 5E871ED9
P 3050 3150
AR Path="/5E871ED9" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E871ED9" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3050 2900 50  0001 C CNN
F 1 "GND" H 3055 2977 50  0000 C CNN
F 2 "" H 3050 3150 50  0001 C CNN
F 3 "" H 3050 3150 50  0001 C CNN
	1    3050 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 3150 3050 3050
$Comp
L power:+3V3 #PWR?
U 1 1 5E871EE0
P 3400 2650
AR Path="/5E871EE0" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E871EE0" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3400 2500 50  0001 C CNN
F 1 "+3V3" V 3415 2778 50  0000 L CNN
F 2 "" H 3400 2650 50  0001 C CNN
F 3 "" H 3400 2650 50  0001 C CNN
	1    3400 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	3400 2650 3050 2650
Connection ~ 3050 2650
Text Notes 2800 2400 0    50   ~ 0
Regulador 3.3v
Text GLabel 1400 2650 0    50   Output ~ 0
Vbatt
$Comp
L prockeckv2-rescue:HX3242-chinaland U?
U 1 1 5E87EE6B
P 9000 1450
AR Path="/5E87EE6B" Ref="U?"  Part="1" 
AR Path="/5E851D3B/5E87EE6B" Ref="U?"  Part="1" 
F 0 "U?" H 9000 1500 50  0000 C CNN
F 1 "HX3242" H 9000 1050 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 9000 1300 50  0001 C CNN
F 3 "" H 9000 1300 50  0001 C CNN
	1    9000 1450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E87EE72
P 8400 2050
AR Path="/5E87EE72" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E87EE72" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8400 1800 50  0001 C CNN
F 1 "GND" H 8550 2000 50  0000 C CNN
F 2 "" H 8400 2050 50  0001 C CNN
F 3 "" H 8400 2050 50  0001 C CNN
	1    8400 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:L L?
U 1 1 5E87EE78
P 8900 950
AR Path="/5E87EE78" Ref="L?"  Part="1" 
AR Path="/5E851D3B/5E87EE78" Ref="L?"  Part="1" 
F 0 "L?" V 9000 950 50  0000 C CNN
F 1 "4.7u" V 8850 950 50  0000 C CNN
F 2 "Inductor_SMD:L_Taiyo-Yuden_NR-60xx_HandSoldering" H 8900 950 50  0001 C CNN
F 3 "~" H 8900 950 50  0001 C CNN
	1    8900 950 
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9050 950  9450 950 
Wire Wire Line
	9450 950  9450 1200
Wire Wire Line
	9450 1200 9350 1200
$Comp
L Device:R R?
U 1 1 5E87EE82
P 8000 1350
AR Path="/5E87EE82" Ref="R?"  Part="1" 
AR Path="/5E851D3B/5E87EE82" Ref="R?"  Part="1" 
F 0 "R?" V 7793 1350 50  0000 C CNN
F 1 "10k" V 7884 1350 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7930 1350 50  0001 C CNN
F 3 "~" H 8000 1350 50  0001 C CNN
	1    8000 1350
	0    1    1    0   
$EndComp
$Comp
L Device:C C?
U 1 1 5E87EE89
P 8200 1700
AR Path="/5E87EE89" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E87EE89" Ref="C?"  Part="1" 
F 0 "C?" H 8000 1750 50  0000 L CNN
F 1 "0.1u" H 7950 1600 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 8238 1550 50  0001 C CNN
F 3 "~" H 8200 1700 50  0001 C CNN
	1    8200 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 1350 7750 1350
Wire Wire Line
	8150 1350 8200 1350
Wire Wire Line
	8200 1350 8200 1550
Wire Wire Line
	8200 1850 8200 2000
Wire Wire Line
	8200 2000 8400 2000
Connection ~ 8400 2000
Wire Wire Line
	8400 2000 8400 2050
Wire Wire Line
	8200 1350 8650 1350
Connection ~ 8200 1350
Wire Wire Line
	9350 1350 9450 1350
Wire Wire Line
	9450 1350 9450 1200
Connection ~ 9450 1200
$Comp
L Device:D_Schottky D?
U 1 1 5E87EE9C
P 9700 950
AR Path="/5E87EE9C" Ref="D?"  Part="1" 
AR Path="/5E851D3B/5E87EE9C" Ref="D?"  Part="1" 
F 0 "D?" H 9700 850 50  0000 C CNN
F 1 "D_Schottky" H 9700 1100 50  0000 C CNN
F 2 "Diode_SMD:D_SMA" H 9700 950 50  0001 C CNN
F 3 "~" H 9700 950 50  0001 C CNN
	1    9700 950 
	-1   0    0    1   
$EndComp
Wire Wire Line
	9450 950  9550 950 
Connection ~ 9450 950 
$Comp
L Device:R R?
U 1 1 5E87EEA5
P 10000 1250
AR Path="/5E87EEA5" Ref="R?"  Part="1" 
AR Path="/5E851D3B/5E87EEA5" Ref="R?"  Part="1" 
F 0 "R?" H 9930 1204 50  0000 R CNN
F 1 "10k" H 9930 1295 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 9930 1250 50  0001 C CNN
F 3 "~" H 10000 1250 50  0001 C CNN
	1    10000 1250
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5E87EEAC
P 10000 1700
AR Path="/5E87EEAC" Ref="R?"  Part="1" 
AR Path="/5E851D3B/5E87EEAC" Ref="R?"  Part="1" 
F 0 "R?" H 9930 1654 50  0000 R CNN
F 1 "10k" H 9930 1745 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 9930 1700 50  0001 C CNN
F 3 "~" H 10000 1700 50  0001 C CNN
	1    10000 1700
	-1   0    0    1   
$EndComp
Wire Wire Line
	9850 950  10000 950 
Wire Wire Line
	10000 950  10000 1100
Wire Wire Line
	10000 1400 10000 1500
Wire Wire Line
	10000 1850 10000 2000
Wire Wire Line
	10000 2000 9450 2000
Wire Wire Line
	10000 1500 10350 1500
Wire Wire Line
	10350 1500 10350 750 
Wire Wire Line
	10350 750  8650 750 
Wire Wire Line
	8650 750  8650 1200
Connection ~ 10000 1500
Wire Wire Line
	10000 1500 10000 1550
Wire Wire Line
	9350 1500 9450 1500
Wire Wire Line
	9450 1500 9450 1650
Wire Wire Line
	9450 1650 9350 1650
$Comp
L Device:R R?
U 1 1 5E87EEC1
P 9450 1800
AR Path="/5E87EEC1" Ref="R?"  Part="1" 
AR Path="/5E851D3B/5E87EEC1" Ref="R?"  Part="1" 
F 0 "R?" H 9380 1754 50  0000 R CNN
F 1 "50m" H 9380 1845 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 9380 1800 50  0001 C CNN
F 3 "~" H 9450 1800 50  0001 C CNN
	1    9450 1800
	-1   0    0    1   
$EndComp
Connection ~ 9450 1650
Wire Wire Line
	9450 1950 9450 2000
Connection ~ 9450 2000
$Comp
L Device:C C?
U 1 1 5E87EECB
P 10350 1750
AR Path="/5E87EECB" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E87EECB" Ref="C?"  Part="1" 
F 0 "C?" H 10465 1796 50  0000 L CNN
F 1 "22p" H 10465 1705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 10388 1600 50  0001 C CNN
F 3 "~" H 10350 1750 50  0001 C CNN
	1    10350 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	10000 2000 10350 2000
Wire Wire Line
	10350 2000 10350 1900
Connection ~ 10000 2000
Wire Wire Line
	10350 1600 10350 1500
Connection ~ 10350 1500
$Comp
L Device:C C?
U 1 1 5E87EED7
P 10650 1500
AR Path="/5E87EED7" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E87EED7" Ref="C?"  Part="1" 
F 0 "C?" H 10765 1546 50  0000 L CNN
F 1 "47u" H 10765 1455 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 10688 1350 50  0001 C CNN
F 3 "~" H 10650 1500 50  0001 C CNN
	1    10650 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	10000 950  10650 950 
Wire Wire Line
	10650 950  10650 1350
Connection ~ 10000 950 
Wire Wire Line
	10650 1650 10650 2000
Wire Wire Line
	10650 2000 10350 2000
Connection ~ 10350 2000
Wire Wire Line
	7750 950  8400 950 
Wire Wire Line
	8650 1500 8400 1500
Wire Wire Line
	8400 1500 8400 950 
Connection ~ 8400 950 
Wire Wire Line
	8400 950  8750 950 
$Comp
L Device:C C?
U 1 1 5E87EEE9
P 8400 1700
AR Path="/5E87EEE9" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E87EEE9" Ref="C?"  Part="1" 
F 0 "C?" H 8450 1800 50  0000 L CNN
F 1 "0.1u" H 8450 1600 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 8438 1550 50  0001 C CNN
F 3 "~" H 8400 1700 50  0001 C CNN
	1    8400 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8400 1550 8400 1500
Connection ~ 8400 1500
Wire Wire Line
	8400 1850 8400 2000
Wire Wire Line
	8650 1650 8650 2000
Wire Wire Line
	8400 2000 8650 2000
Connection ~ 8650 2000
Wire Wire Line
	8650 2000 9450 2000
Text Notes 9100 2250 0    50   ~ 0
12V Boost
Wire Wire Line
	10700 950  10650 950 
Connection ~ 10650 950 
Text GLabel 7750 950  0    50   Input ~ 0
VPow
$Comp
L Device:R R?
U 1 1 5E87EEFB
P 7750 1700
AR Path="/5E87EEFB" Ref="R?"  Part="1" 
AR Path="/5E851D3B/5E87EEFB" Ref="R?"  Part="1" 
F 0 "R?" V 7543 1700 50  0000 C CNN
F 1 "10k" V 7634 1700 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7680 1700 50  0001 C CNN
F 3 "~" H 7750 1700 50  0001 C CNN
	1    7750 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E87EF02
P 7750 2050
AR Path="/5E87EF02" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E87EF02" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7750 1800 50  0001 C CNN
F 1 "GND" H 7900 2000 50  0000 C CNN
F 2 "" H 7750 2050 50  0001 C CNN
F 3 "" H 7750 2050 50  0001 C CNN
	1    7750 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 1850 7750 2050
Wire Wire Line
	7750 1550 7750 1350
Connection ~ 7750 1350
Wire Wire Line
	7750 1350 7850 1350
$Comp
L power:+12V #PWR?
U 1 1 5E87EF0C
P 10700 950
AR Path="/5E87EF0C" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E87EF0C" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10700 800 50  0001 C CNN
F 1 "+12V" V 10600 950 50  0000 L CNN
F 2 "" H 10700 950 50  0001 C CNN
F 3 "" H 10700 950 50  0001 C CNN
	1    10700 950 
	0    1    1    0   
$EndComp
Text GLabel 7700 1350 0    50   Input ~ 0
SENSOR_EN
$Comp
L prockeckv2-rescue:HX3001-chinaland U?
U 1 1 5E886D9C
P 5250 1450
AR Path="/5E886D9C" Ref="U?"  Part="1" 
AR Path="/5E851D3B/5E886D9C" Ref="U?"  Part="1" 
F 0 "U?" H 5400 1750 50  0000 C CNN
F 1 "HX3001" H 5400 1850 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6_Handsoldering" H 5250 1450 50  0001 C CNN
F 3 "" H 5250 1450 50  0001 C CNN
	1    5250 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 1400 4900 1400
$Comp
L Device:L L?
U 1 1 5E886DA4
P 4900 1000
AR Path="/5E886DA4" Ref="L?"  Part="1" 
AR Path="/5E851D3B/5E886DA4" Ref="L?"  Part="1" 
F 0 "L?" V 5090 1000 50  0000 C CNN
F 1 "4.7u" V 4999 1000 50  0000 C CNN
F 2 "Inductor_SMD:L_Taiyo-Yuden_NR-60xx_HandSoldering" H 4900 1000 50  0001 C CNN
F 3 "~" H 4900 1000 50  0001 C CNN
	1    4900 1000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4750 1000 4700 1000
Connection ~ 4700 1000
Wire Wire Line
	4700 1000 4700 1400
Wire Wire Line
	5050 1000 5250 1000
Wire Wire Line
	5250 1000 5250 1100
Wire Wire Line
	4300 1000 4400 1000
$Comp
L power:GND #PWR?
U 1 1 5E886DB1
P 5250 1950
AR Path="/5E886DB1" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E886DB1" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5250 1700 50  0001 C CNN
F 1 "GND" H 5255 1777 50  0000 C CNN
F 2 "" H 5250 1950 50  0001 C CNN
F 3 "" H 5250 1950 50  0001 C CNN
	1    5250 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 1950 5250 1900
$Comp
L Device:C C?
U 1 1 5E886DB8
P 4400 1400
AR Path="/5E886DB8" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E886DB8" Ref="C?"  Part="1" 
F 0 "C?" H 4515 1446 50  0000 L CNN
F 1 "4.7u" H 4515 1355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 4438 1250 50  0001 C CNN
F 3 "~" H 4400 1400 50  0001 C CNN
	1    4400 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 1250 4400 1000
Connection ~ 4400 1000
Wire Wire Line
	4400 1000 4700 1000
Wire Wire Line
	4400 1550 4400 1900
Wire Wire Line
	4400 1900 5250 1900
Connection ~ 5250 1900
Wire Wire Line
	5250 1900 5250 1850
$Comp
L Device:C C?
U 1 1 5E886DC6
P 6250 1650
AR Path="/5E886DC6" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E886DC6" Ref="C?"  Part="1" 
F 0 "C?" H 6365 1696 50  0000 L CNN
F 1 "4.7u" H 6365 1605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6288 1500 50  0001 C CNN
F 3 "~" H 6250 1650 50  0001 C CNN
	1    6250 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 1400 6250 1500
Wire Wire Line
	6250 1800 6250 1900
Wire Wire Line
	6250 1900 5450 1900
$Comp
L Device:R R?
U 1 1 5E886DD0
P 5950 1600
AR Path="/5E886DD0" Ref="R?"  Part="1" 
AR Path="/5E851D3B/5E886DD0" Ref="R?"  Part="1" 
F 0 "R?" H 6020 1646 50  0000 L CNN
F 1 "1M" H 6020 1555 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 5880 1600 50  0001 C CNN
F 3 "~" H 5950 1600 50  0001 C CNN
	1    5950 1600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5E886DD7
P 5650 1800
AR Path="/5E886DD7" Ref="R?"  Part="1" 
AR Path="/5E851D3B/5E886DD7" Ref="R?"  Part="1" 
F 0 "R?" V 5550 1800 50  0000 C CNN
F 1 "332k" V 5700 2000 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 5580 1800 50  0001 C CNN
F 3 "~" H 5650 1800 50  0001 C CNN
	1    5650 1800
	0    1    1    0   
$EndComp
Wire Wire Line
	5500 1800 5450 1800
Wire Wire Line
	5450 1800 5450 1900
Connection ~ 5450 1900
Wire Wire Line
	5450 1900 5250 1900
Wire Wire Line
	5800 1800 5850 1800
Wire Wire Line
	5950 1800 5950 1750
Wire Wire Line
	5850 1600 5600 1600
Connection ~ 5850 1800
Wire Wire Line
	5850 1800 5950 1800
Wire Wire Line
	5600 1400 5950 1400
Wire Wire Line
	5950 1400 5950 1450
Wire Wire Line
	5950 1400 6250 1400
Connection ~ 5950 1400
Wire Wire Line
	5850 1600 5850 1800
Connection ~ 6250 1400
$Comp
L Device:D_Schottky D?
U 1 1 5E886DED
P 5650 1000
AR Path="/5E886DED" Ref="D?"  Part="1" 
AR Path="/5E851D3B/5E886DED" Ref="D?"  Part="1" 
F 0 "D?" H 5650 784 50  0000 C CNN
F 1 "D_Schottky" H 5650 875 50  0000 C CNN
F 2 "Diode_SMD:D_SMA" H 5650 1000 50  0001 C CNN
F 3 "~" H 5650 1000 50  0001 C CNN
	1    5650 1000
	-1   0    0    1   
$EndComp
Wire Wire Line
	5250 1000 5500 1000
Connection ~ 5250 1000
Wire Wire Line
	5950 1000 5950 1400
Wire Wire Line
	5800 1000 5950 1000
Text Notes 5050 2300 0    50   ~ 0
5V Boost
$Comp
L power:+5V #PWR?
U 1 1 5E886DF9
P 6700 1200
AR Path="/5E886DF9" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E886DF9" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6700 1050 50  0001 C CNN
F 1 "+5V" V 6715 1328 50  0000 L CNN
F 2 "" H 6700 1200 50  0001 C CNN
F 3 "" H 6700 1200 50  0001 C CNN
	1    6700 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 1400 6700 1200
Wire Wire Line
	6250 1400 6650 1400
$Comp
L Device:C_Small C?
U 1 1 5E886E01
P 6650 1650
AR Path="/5E886E01" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E886E01" Ref="C?"  Part="1" 
F 0 "C?" H 6500 1650 50  0000 C CNN
F 1 "100n" H 6450 1750 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6650 1650 50  0001 C CNN
F 3 "~" H 6650 1650 50  0001 C CNN
	1    6650 1650
	-1   0    0    1   
$EndComp
Wire Wire Line
	6650 1750 6650 1900
Wire Wire Line
	6650 1900 6250 1900
Connection ~ 6250 1900
Wire Wire Line
	6650 1550 6650 1400
Connection ~ 6650 1400
Wire Wire Line
	6650 1400 6700 1400
Text GLabel 4300 1000 0    50   Input ~ 0
VPow
Wire Wire Line
	4700 1600 4700 1400
Wire Wire Line
	4700 1600 4900 1600
Connection ~ 4700 1400
$EndSCHEMATC
