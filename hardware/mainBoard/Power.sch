EESchema Schematic File Version 4
LIBS:prockeckv2-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 10
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
P 2250 1150
AR Path="/5E85D6AB" Ref="U?"  Part="1" 
AR Path="/5E851D3B/5E85D6AB" Ref="U6"  Part="1" 
F 0 "U6" H 2250 1565 50  0000 C CNN
F 1 "SGM3204" H 2250 1474 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6_Handsoldering" H 2250 1150 50  0001 C CNN
F 3 "" H 2250 1150 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/DC-DC-Converters_SGMICRO-SGM3204YN6G-TR_C194031.html" H 2250 1150 50  0001 C CNN "URL"
	1    2250 1150
	1    0    0    -1  
$EndComp
$Comp
L power:-5V #PWR?
U 1 1 5E85D6B2
P 950 1000
AR Path="/5E85D6B2" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E85D6B2" Ref="#PWR019"  Part="1" 
F 0 "#PWR019" H 950 1100 50  0001 C CNN
F 1 "-5V" V 965 1128 50  0000 L CNN
F 2 "" H 950 1000 50  0001 C CNN
F 3 "" H 950 1000 50  0001 C CNN
	1    950  1000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	950  1000 1050 1000
$Comp
L power:+5V #PWR?
U 1 1 5E85D6B9
P 1400 1150
AR Path="/5E85D6B9" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E85D6B9" Ref="#PWR021"  Part="1" 
F 0 "#PWR021" H 1400 1000 50  0001 C CNN
F 1 "+5V" V 1415 1278 50  0000 L CNN
F 2 "" H 1400 1150 50  0001 C CNN
F 3 "" H 1400 1150 50  0001 C CNN
	1    1400 1150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1400 1150 1550 1150
$Comp
L Device:C C?
U 1 1 5E85D6C0
P 2200 1700
AR Path="/5E85D6C0" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E85D6C0" Ref="C6"  Part="1" 
F 0 "C6" V 2452 1700 50  0000 C CNN
F 1 "3.3u" V 2361 1700 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2238 1550 50  0001 C CNN
F 3 "~" H 2200 1700 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10A335KP8NNNC_3-3uF-335-10-10V_C51412.html" H 2200 1700 50  0001 C CNN "URL"
	1    2200 1700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1800 1300 1700 1300
Wire Wire Line
	1700 1300 1700 1700
Wire Wire Line
	1700 1700 2050 1700
Wire Wire Line
	2350 1700 3150 1700
Wire Wire Line
	3150 1700 3150 1000
Wire Wire Line
	3150 1000 2700 1000
$Comp
L Device:R R?
U 1 1 5E85D6CD
P 2900 1150
AR Path="/5E85D6CD" Ref="R?"  Part="1" 
AR Path="/5E851D3B/5E85D6CD" Ref="R7"  Part="1" 
F 0 "R7" V 2693 1150 50  0000 C CNN
F 1 "10k" V 2784 1150 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2830 1150 50  0001 C CNN
F 3 "~" H 2900 1150 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/New-Arrivals_SANYEAR-SY0603JN10KP_C380762.html" H 2900 1150 50  0001 C CNN "URL"
	1    2900 1150
	0    1    1    0   
$EndComp
Wire Wire Line
	2750 1150 2700 1150
Wire Wire Line
	3250 1150 3100 1150
$Comp
L power:GND #PWR?
U 1 1 5E85D6D7
P 2750 1350
AR Path="/5E85D6D7" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E85D6D7" Ref="#PWR024"  Part="1" 
F 0 "#PWR024" H 2750 1100 50  0001 C CNN
F 1 "GND" H 2755 1177 50  0000 C CNN
F 2 "" H 2750 1350 50  0001 C CNN
F 3 "" H 2750 1350 50  0001 C CNN
	1    2750 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 1300 2750 1300
Wire Wire Line
	2750 1300 2750 1350
Text Notes 1650 650  0    50   ~ 0
Generador de Voltaje Negativo
$Comp
L Device:C C?
U 1 1 5E85D6E0
P 1550 1400
AR Path="/5E85D6E0" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E85D6E0" Ref="C4"  Part="1" 
F 0 "C4" V 1802 1400 50  0000 C CNN
F 1 "3.3u" V 1711 1400 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 1588 1250 50  0001 C CNN
F 3 "~" H 1550 1400 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10A335KP8NNNC_3-3uF-335-10-10V_C51412.html" H 1550 1400 50  0001 C CNN "URL"
	1    1550 1400
	-1   0    0    1   
$EndComp
$Comp
L Device:C C?
U 1 1 5E85D6E7
P 1050 1400
AR Path="/5E85D6E7" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E85D6E7" Ref="C3"  Part="1" 
F 0 "C3" V 1302 1400 50  0000 C CNN
F 1 "3.3u" V 1211 1400 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 1088 1250 50  0001 C CNN
F 3 "~" H 1050 1400 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10A335KP8NNNC_3-3uF-335-10-10V_C51412.html" H 1050 1400 50  0001 C CNN "URL"
	1    1050 1400
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E85D6EE
P 1550 1650
AR Path="/5E85D6EE" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E85D6EE" Ref="#PWR022"  Part="1" 
F 0 "#PWR022" H 1550 1400 50  0001 C CNN
F 1 "GND" H 1555 1477 50  0000 C CNN
F 2 "" H 1550 1650 50  0001 C CNN
F 3 "" H 1550 1650 50  0001 C CNN
	1    1550 1650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E85D6F4
P 1050 1650
AR Path="/5E85D6F4" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E85D6F4" Ref="#PWR020"  Part="1" 
F 0 "#PWR020" H 1050 1400 50  0001 C CNN
F 1 "GND" H 1055 1477 50  0000 C CNN
F 2 "" H 1050 1650 50  0001 C CNN
F 3 "" H 1050 1650 50  0001 C CNN
	1    1050 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1050 1650 1050 1550
Wire Wire Line
	1550 1650 1550 1550
Wire Wire Line
	1550 1250 1550 1150
Connection ~ 1550 1150
Wire Wire Line
	1550 1150 1800 1150
Wire Wire Line
	1050 1250 1050 1000
Connection ~ 1050 1000
Wire Wire Line
	1050 1000 1400 1000
$Comp
L Device:R R?
U 1 1 5E85D702
P 2900 1300
AR Path="/5E85D702" Ref="R?"  Part="1" 
AR Path="/5E851D3B/5E85D702" Ref="R8"  Part="1" 
F 0 "R8" V 3100 1300 50  0000 C CNN
F 1 "10k" V 3000 1300 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2830 1300 50  0001 C CNN
F 3 "~" H 2900 1300 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/New-Arrivals_SANYEAR-SY0603JN10KP_C380762.html" H 2900 1300 50  0001 C CNN "URL"
	1    2900 1300
	0    1    1    0   
$EndComp
Connection ~ 2750 1300
Wire Wire Line
	3050 1300 3100 1300
Wire Wire Line
	3100 1300 3100 1150
Connection ~ 3100 1150
Wire Wire Line
	3100 1150 3050 1150
$Comp
L Regulator_Linear:AMS1117-3.3 U?
U 1 1 5E871E9F
P 5100 900
AR Path="/5E871E9F" Ref="U?"  Part="1" 
AR Path="/5E851D3B/5E871E9F" Ref="U8"  Part="1" 
F 0 "U8" H 5100 1142 50  0000 C CNN
F 1 "AMS1117-3.3" H 5100 1051 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 5100 1100 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 5200 650 50  0001 C CNN
	1    5100 900 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E871EA6
P 5100 1400
AR Path="/5E871EA6" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E871EA6" Ref="#PWR029"  Part="1" 
F 0 "#PWR029" H 5100 1150 50  0001 C CNN
F 1 "GND" H 5105 1227 50  0000 C CNN
F 2 "" H 5100 1400 50  0001 C CNN
F 3 "" H 5100 1400 50  0001 C CNN
	1    5100 1400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5E871EAC
P 4550 1150
AR Path="/5E871EAC" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E871EAC" Ref="C8"  Part="1" 
F 0 "C8" H 4665 1196 50  0000 L CNN
F 1 "10u" H 4665 1105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 4588 1000 50  0001 C CNN
F 3 "~" H 4550 1150 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Others_Samsung-Electro-Mechanics_CL10A106MQ8NNNL_Samsung-Electro-Mechanics-CL10A106MQ8NNNL_C307461.html" H 4550 1150 50  0001 C CNN "URL"
	1    4550 1150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E871EB3
P 4550 1400
AR Path="/5E871EB3" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E871EB3" Ref="#PWR028"  Part="1" 
F 0 "#PWR028" H 4550 1150 50  0001 C CNN
F 1 "GND" H 4555 1227 50  0000 C CNN
F 2 "" H 4550 1400 50  0001 C CNN
F 3 "" H 4550 1400 50  0001 C CNN
	1    4550 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 1400 4550 1300
Wire Wire Line
	4550 1000 4550 900 
Wire Wire Line
	4550 900  4800 900 
Connection ~ 4550 900 
Wire Wire Line
	5100 1400 5100 1200
$Comp
L Device:C C?
U 1 1 5E871EBF
P 5600 1150
AR Path="/5E871EBF" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E871EBF" Ref="C10"  Part="1" 
F 0 "C10" H 5715 1196 50  0000 L CNN
F 1 "22u" H 5715 1105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5638 1000 50  0001 C CNN
F 3 "~" H 5600 1150 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10A226MQ8NRNE_22uF-226-20-6-3V_C159801.html" H 5600 1150 50  0001 C CNN "URL"
	1    5600 1150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E871EC6
P 5600 1400
AR Path="/5E871EC6" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E871EC6" Ref="#PWR031"  Part="1" 
F 0 "#PWR031" H 5600 1150 50  0001 C CNN
F 1 "GND" H 5605 1227 50  0000 C CNN
F 2 "" H 5600 1400 50  0001 C CNN
F 3 "" H 5600 1400 50  0001 C CNN
	1    5600 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 1400 5600 1300
Wire Wire Line
	5400 900  5600 900 
Wire Wire Line
	5600 900  5600 1000
$Comp
L Device:C C?
U 1 1 5E871ECF
P 6000 1150
AR Path="/5E871ECF" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E871ECF" Ref="C11"  Part="1" 
F 0 "C11" H 6115 1196 50  0000 L CNN
F 1 "100n" H 6115 1105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6038 1000 50  0001 C CNN
F 3 "~" H 6000 1150 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10F104ZB8NNNC_100nF-104-80-20-50V_C1688.html" H 6000 1150 50  0001 C CNN "URL"
	1    6000 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 900  6000 900 
Wire Wire Line
	6000 900  6000 1000
Connection ~ 5600 900 
$Comp
L power:GND #PWR?
U 1 1 5E871ED9
P 6000 1400
AR Path="/5E871ED9" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E871ED9" Ref="#PWR032"  Part="1" 
F 0 "#PWR032" H 6000 1150 50  0001 C CNN
F 1 "GND" H 6005 1227 50  0000 C CNN
F 2 "" H 6000 1400 50  0001 C CNN
F 3 "" H 6000 1400 50  0001 C CNN
	1    6000 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 1400 6000 1300
$Comp
L power:+3V3 #PWR?
U 1 1 5E871EE0
P 6200 900
AR Path="/5E871EE0" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E871EE0" Ref="#PWR034"  Part="1" 
F 0 "#PWR034" H 6200 750 50  0001 C CNN
F 1 "+3V3" V 6215 1028 50  0000 L CNN
F 2 "" H 6200 900 50  0001 C CNN
F 3 "" H 6200 900 50  0001 C CNN
	1    6200 900 
	0    1    1    0   
$EndComp
Connection ~ 6000 900 
Text Notes 5550 700  0    50   ~ 0
Regulador 3.3v
$Comp
L prockeckv2-rescue:HX3242-chinaland U?
U 1 1 5E87EE6B
P 9200 1300
AR Path="/5E87EE6B" Ref="U?"  Part="1" 
AR Path="/5E851D3B/5E87EE6B" Ref="U11"  Part="1" 
F 0 "U11" H 9200 1350 50  0000 C CNN
F 1 "HX3242" H 9200 900 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 9200 1150 50  0001 C CNN
F 3 "" H 9200 1150 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/DC-DC-Converters_HEXIN-HX3242_C172357.html" H 9200 1300 50  0001 C CNN "URL"
	1    9200 1300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E87EE72
P 8600 1900
AR Path="/5E87EE72" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E87EE72" Ref="#PWR045"  Part="1" 
F 0 "#PWR045" H 8600 1650 50  0001 C CNN
F 1 "GND" H 8750 1850 50  0000 C CNN
F 2 "" H 8600 1900 50  0001 C CNN
F 3 "" H 8600 1900 50  0001 C CNN
	1    8600 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:L L?
U 1 1 5E87EE78
P 9100 800
AR Path="/5E87EE78" Ref="L?"  Part="1" 
AR Path="/5E851D3B/5E87EE78" Ref="L2"  Part="1" 
F 0 "L2" V 9200 800 50  0000 C CNN
F 1 "4.7u" V 9050 800 50  0000 C CNN
F 2 "Inductor_SMD:L_Taiyo-Yuden_NR-60xx_HandSoldering" H 9100 800 50  0001 C CNN
F 3 "~" H 9100 800 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Power-Inductors_Taiyo-Yuden_NR6028T4R7M_Taiyo-Yuden-NR6028T4R7M_C77758.html" H 9100 800 50  0001 C CNN "URL"
	1    9100 800 
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9250 800  9650 800 
Wire Wire Line
	9650 800  9650 1050
Wire Wire Line
	9650 1050 9550 1050
$Comp
L Device:R R?
U 1 1 5E87EE82
P 8200 1200
AR Path="/5E87EE82" Ref="R?"  Part="1" 
AR Path="/5E851D3B/5E87EE82" Ref="R16"  Part="1" 
F 0 "R16" V 7993 1200 50  0000 C CNN
F 1 "10k" V 8084 1200 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8130 1200 50  0001 C CNN
F 3 "~" H 8200 1200 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/New-Arrivals_SANYEAR-SY0603JN10KP_C380762.html" H 8200 1200 50  0001 C CNN "URL"
	1    8200 1200
	0    1    1    0   
$EndComp
$Comp
L Device:C C?
U 1 1 5E87EE89
P 8400 1550
AR Path="/5E87EE89" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E87EE89" Ref="C15"  Part="1" 
F 0 "C15" H 8200 1600 50  0000 L CNN
F 1 "100n" H 8150 1450 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 8438 1400 50  0001 C CNN
F 3 "~" H 8400 1550 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10F104ZB8NNNC_100nF-104-80-20-50V_C1688.html" H 8400 1550 50  0001 C CNN "URL"
	1    8400 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 1200 8400 1200
Wire Wire Line
	8400 1200 8400 1400
Wire Wire Line
	8400 1700 8400 1850
Wire Wire Line
	8400 1850 8600 1850
Connection ~ 8600 1850
Wire Wire Line
	8600 1850 8600 1900
Wire Wire Line
	8400 1200 8850 1200
Connection ~ 8400 1200
Wire Wire Line
	9550 1200 9650 1200
Wire Wire Line
	9650 1200 9650 1050
Connection ~ 9650 1050
$Comp
L Device:D_Schottky D?
U 1 1 5E87EE9C
P 9900 800
AR Path="/5E87EE9C" Ref="D?"  Part="1" 
AR Path="/5E851D3B/5E87EE9C" Ref="D4"  Part="1" 
F 0 "D4" H 9900 700 50  0000 C CNN
F 1 "SS34A" H 9900 950 50  0000 C CNN
F 2 "Diode_SMD:D_SMA" H 9900 800 50  0001 C CNN
F 3 "~" H 9900 800 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Schottky-Barrier-Diodes-SBD_SS34_C183891.html" H 9900 800 50  0001 C CNN "URL"
	1    9900 800 
	-1   0    0    1   
$EndComp
Wire Wire Line
	9650 800  9750 800 
Connection ~ 9650 800 
$Comp
L Device:R R?
U 1 1 5E87EEA5
P 10200 1100
AR Path="/5E87EEA5" Ref="R?"  Part="1" 
AR Path="/5E851D3B/5E87EEA5" Ref="R18"  Part="1" 
F 0 "R18" H 10130 1054 50  0000 R CNN
F 1 "10k" H 10130 1145 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 10130 1100 50  0001 C CNN
F 3 "~" H 10200 1100 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/New-Arrivals_SANYEAR-SY0603JN10KP_C380762.html" H 10200 1100 50  0001 C CNN "URL"
	1    10200 1100
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5E87EEAC
P 10200 1550
AR Path="/5E87EEAC" Ref="R?"  Part="1" 
AR Path="/5E851D3B/5E87EEAC" Ref="R19"  Part="1" 
F 0 "R19" H 10130 1504 50  0000 R CNN
F 1 "10k" H 10130 1595 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 10130 1550 50  0001 C CNN
F 3 "~" H 10200 1550 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/New-Arrivals_SANYEAR-SY0603JN10KP_C380762.html" H 10200 1550 50  0001 C CNN "URL"
	1    10200 1550
	-1   0    0    1   
$EndComp
Wire Wire Line
	10050 800  10200 800 
Wire Wire Line
	10200 800  10200 950 
Wire Wire Line
	10200 1250 10200 1350
Wire Wire Line
	10200 1700 10200 1850
Wire Wire Line
	10200 1850 9650 1850
Wire Wire Line
	10200 1350 10550 1350
Wire Wire Line
	10550 1350 10550 600 
Wire Wire Line
	10550 600  8850 600 
Wire Wire Line
	8850 600  8850 1050
Connection ~ 10200 1350
Wire Wire Line
	10200 1350 10200 1400
Wire Wire Line
	9550 1350 9650 1350
Wire Wire Line
	9650 1350 9650 1500
Wire Wire Line
	9650 1500 9550 1500
$Comp
L Device:R R?
U 1 1 5E87EEC1
P 9650 1650
AR Path="/5E87EEC1" Ref="R?"  Part="1" 
AR Path="/5E851D3B/5E87EEC1" Ref="R17"  Part="1" 
F 0 "R17" H 9580 1604 50  0000 R CNN
F 1 "50m" H 9580 1695 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 9580 1650 50  0001 C CNN
F 3 "~" H 9650 1650 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_0-05R-R050-1_C158895.html" H 9650 1650 50  0001 C CNN "URL"
	1    9650 1650
	-1   0    0    1   
$EndComp
Connection ~ 9650 1500
Wire Wire Line
	9650 1800 9650 1850
Connection ~ 9650 1850
$Comp
L Device:C C?
U 1 1 5E87EECB
P 10550 1600
AR Path="/5E87EECB" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E87EECB" Ref="C20"  Part="1" 
F 0 "C20" H 10665 1646 50  0000 L CNN
F 1 "22p" H 10665 1555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 10588 1450 50  0001 C CNN
F 3 "~" H 10550 1600 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_Walsin-Tech-Corp-0603N220J500_C107451.html" H 10550 1600 50  0001 C CNN "URL"
	1    10550 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 1850 10550 1850
Wire Wire Line
	10550 1850 10550 1750
Connection ~ 10200 1850
Wire Wire Line
	10550 1450 10550 1350
Connection ~ 10550 1350
$Comp
L Device:C C?
U 1 1 5E87EED7
P 10850 1350
AR Path="/5E87EED7" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E87EED7" Ref="C22"  Part="1" 
F 0 "C22" H 10965 1396 50  0000 L CNN
F 1 "47u" H 10965 1305 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 10888 1200 50  0001 C CNN
F 3 "~" H 10850 1350 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_TDK_C3216X5R1E476MTJ00E_47uF-476-20-25V_C76659.html" H 10850 1350 50  0001 C CNN "URL"
	1    10850 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 800  10750 800 
Wire Wire Line
	10850 800  10850 1200
Connection ~ 10200 800 
Wire Wire Line
	10850 1500 10850 1850
Wire Wire Line
	10850 1850 10550 1850
Connection ~ 10550 1850
Wire Wire Line
	8850 1350 8600 1350
Wire Wire Line
	8600 1350 8600 800 
Connection ~ 8600 800 
Wire Wire Line
	8600 800  8950 800 
$Comp
L Device:C C?
U 1 1 5E87EEE9
P 8600 1550
AR Path="/5E87EEE9" Ref="C?"  Part="1" 
AR Path="/5E851D3B/5E87EEE9" Ref="C17"  Part="1" 
F 0 "C17" H 8650 1650 50  0000 L CNN
F 1 "100n" H 8650 1450 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 8638 1400 50  0001 C CNN
F 3 "~" H 8600 1550 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10F104ZB8NNNC_100nF-104-80-20-50V_C1688.html" H 8600 1550 50  0001 C CNN "URL"
	1    8600 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 1400 8600 1350
Connection ~ 8600 1350
Wire Wire Line
	8600 1700 8600 1850
Wire Wire Line
	8850 1500 8850 1850
Wire Wire Line
	8600 1850 8850 1850
Connection ~ 8850 1850
Wire Wire Line
	8850 1850 9650 1850
Text Notes 9300 2100 0    50   ~ 0
12V Boost
Wire Wire Line
	10900 800  10850 800 
Connection ~ 10850 800 
$Comp
L Device:R R?
U 1 1 5E87EEFB
P 7950 1550
AR Path="/5E87EEFB" Ref="R?"  Part="1" 
AR Path="/5E851D3B/5E87EEFB" Ref="R15"  Part="1" 
F 0 "R15" V 7743 1550 50  0000 C CNN
F 1 "10k" V 7834 1550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7880 1550 50  0001 C CNN
F 3 "~" H 7950 1550 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/New-Arrivals_SANYEAR-SY0603JN10KP_C380762.html" H 7950 1550 50  0001 C CNN "URL"
	1    7950 1550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E87EF02
P 7950 1900
AR Path="/5E87EF02" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E87EF02" Ref="#PWR042"  Part="1" 
F 0 "#PWR042" H 7950 1650 50  0001 C CNN
F 1 "GND" H 8100 1850 50  0000 C CNN
F 2 "" H 7950 1900 50  0001 C CNN
F 3 "" H 7950 1900 50  0001 C CNN
	1    7950 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 1700 7950 1900
Wire Wire Line
	7950 1400 7950 1200
Connection ~ 7950 1200
Wire Wire Line
	7950 1200 8050 1200
$Comp
L power:+12V #PWR?
U 1 1 5E87EF0C
P 10900 800
AR Path="/5E87EF0C" Ref="#PWR?"  Part="1" 
AR Path="/5E851D3B/5E87EF0C" Ref="#PWR052"  Part="1" 
F 0 "#PWR052" H 10900 650 50  0001 C CNN
F 1 "+12V" V 10800 800 50  0000 L CNN
F 2 "" H 10900 800 50  0001 C CNN
F 3 "" H 10900 800 50  0001 C CNN
	1    10900 800 
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x03_Male J7
U 1 1 5DECE079
P 2400 7200
F 0 "J7" H 2372 7132 50  0000 R CNN
F 1 "Conn_01x03_Male" H 2372 7223 50  0000 R CNN
F 2 "Connector_JST:JST_EH_B3B-EH-A_1x03_P2.50mm_Vertical" H 2400 7200 50  0001 C CNN
F 3 "~" H 2400 7200 50  0001 C CNN
	1    2400 7200
	-1   0    0    1   
$EndComp
$Comp
L Chinaland:HY2213 U3
U 1 1 5DF26F4D
P 1450 3050
F 0 "U3" H 1450 3515 50  0000 C CNN
F 1 "HY2213" H 1450 3424 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6_Handsoldering" H 1450 3050 50  0001 C CNN
F 3 "" H 1450 3050 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/PMIC-Battery-Management_HYCON-Tech-HY2213-BB3A_C113632.html" H 1450 3050 50  0001 C CNN "URL"
	1    1450 3050
	1    0    0    -1  
$EndComp
$Comp
L Chinaland:HY2213 U4
U 1 1 5DF28D47
P 1450 3950
F 0 "U4" H 1450 4415 50  0000 C CNN
F 1 "HY2213" H 1450 4324 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6_Handsoldering" H 1450 3950 50  0001 C CNN
F 3 "" H 1450 3950 50  0001 C CNN
F 4 "HY2213-BB3A" H 1450 3950 50  0001 C CNN "MPN"
F 5 "https://lcsc.com/product-detail/PMIC-Battery-Management_HYCON-Tech-HY2213-BB3A_C113632.html" H 1450 3950 50  0001 C CNN "URL"
	1    1450 3950
	1    0    0    -1  
$EndComp
$Comp
L Chinaland:HY2120 U5
U 1 1 5DF531DB
P 1700 5150
F 0 "U5" H 1700 5615 50  0000 C CNN
F 1 "HY2120" H 1700 5524 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6_Handsoldering" H 1700 5200 50  0001 C CNN
F 3 "" H 1700 5200 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Battery-Protection-ICs_HYCON-Tech-HY2120-AB_C168776.html" H 1700 5150 50  0001 C CNN "URL"
	1    1700 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 4950 750  4950
Wire Wire Line
	750  4950 750  6400
Wire Wire Line
	750  6400 1000 6400
Wire Wire Line
	1300 5150 1100 5150
Wire Wire Line
	1100 5150 1100 5500
Wire Wire Line
	1100 5500 2600 5500
Wire Wire Line
	2600 5500 2600 6400
Wire Wire Line
	2600 6400 2200 6400
NoConn ~ 1000 5950
NoConn ~ 2200 5950
Wire Wire Line
	2200 6250 2200 6100
Wire Wire Line
	1000 6100 1000 6250
$Comp
L power:GND #PWR023
U 1 1 5DF7CF57
P 2450 6150
F 0 "#PWR023" H 2450 5900 50  0001 C CNN
F 1 "GND" H 2455 5977 50  0000 C CNN
F 2 "" H 2450 6150 50  0001 C CNN
F 3 "" H 2450 6150 50  0001 C CNN
	1    2450 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 6150 2450 6100
Wire Wire Line
	2200 6100 2400 6100
$Comp
L Device:R R5
U 1 1 5DF8793B
P 2400 5900
F 0 "R5" H 2330 5854 50  0000 R CNN
F 1 "2k2" H 2330 5945 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2330 5900 50  0001 C CNN
F 3 "~" H 2400 5900 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_2KR-202-5_C105581.html" H 2400 5900 50  0001 C CNN "URL"
	1    2400 5900
	-1   0    0    1   
$EndComp
Wire Wire Line
	2400 6050 2400 6100
Connection ~ 2400 6100
Wire Wire Line
	2400 6100 2450 6100
Wire Wire Line
	2400 5750 2400 5600
Wire Wire Line
	2400 5600 1200 5600
Wire Wire Line
	1200 5600 1200 5350
Wire Wire Line
	1200 5350 1300 5350
Wire Wire Line
	1500 7300 2200 7300
Wire Wire Line
	2200 7200 2050 7200
Wire Wire Line
	2200 7100 1800 7100
Wire Wire Line
	2100 3200 2000 3200
Wire Wire Line
	2000 3200 2000 2850
Wire Wire Line
	2000 2850 1850 2850
Wire Wire Line
	2100 4100 2000 4100
Wire Wire Line
	2000 4100 2000 3750
Wire Wire Line
	2000 3750 1850 3750
$Comp
L Device:R R4
U 1 1 5E062DE5
P 2400 3700
F 0 "R4" H 2470 3746 50  0000 L CNN
F 1 "100" H 2470 3655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2330 3700 50  0001 C CNN
F 3 "~" H 2400 3700 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAJ0620T5E_C25252.html" H 2400 3700 50  0001 C CNN "URL"
	1    2400 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 5E063210
P 2400 2800
F 0 "R3" H 2470 2846 50  0000 L CNN
F 1 "100" H 2470 2755 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2330 2800 50  0001 C CNN
F 3 "~" H 2400 2800 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAJ0620T5E_C25252.html" H 2400 2800 50  0001 C CNN "URL"
	1    2400 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 3000 2400 2950
Wire Wire Line
	2400 3850 2400 3900
Wire Wire Line
	2600 4350 2400 4350
Wire Wire Line
	2400 4350 2400 4300
Wire Wire Line
	2400 3400 2400 3450
Wire Wire Line
	2600 3450 2400 3450
Connection ~ 2400 3450
Wire Wire Line
	2400 3450 2400 3550
Wire Wire Line
	2600 2500 2400 2500
Wire Wire Line
	2400 2500 2400 2650
Wire Wire Line
	2400 4350 800  4350
Wire Wire Line
	800  4350 800  4150
Wire Wire Line
	800  4150 1050 4150
Connection ~ 2400 4350
Wire Wire Line
	2400 3450 800  3450
Wire Wire Line
	800  3450 800  3250
Wire Wire Line
	800  3250 1050 3250
Wire Wire Line
	2400 2500 800  2500
Connection ~ 2400 2500
$Comp
L Device:R R1
U 1 1 5E0B1666
P 800 2750
F 0 "R1" H 870 2796 50  0000 L CNN
F 1 "100" H 870 2705 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 730 2750 50  0001 C CNN
F 3 "~" H 800 2750 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAJ0620T5E_C25252.html" H 800 2750 50  0001 C CNN "URL"
	1    800  2750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5E0B1DCA
P 800 3650
F 0 "R2" H 870 3696 50  0000 L CNN
F 1 "100" H 870 3605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 730 3650 50  0001 C CNN
F 3 "~" H 800 3650 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAJ0620T5E_C25252.html" H 800 3650 50  0001 C CNN "URL"
	1    800  3650
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C1
U 1 1 5E0C3BE5
P 800 3100
F 0 "C1" H 892 3146 50  0000 L CNN
F 1 "100n" H 892 3055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 800 3100 50  0001 C CNN
F 3 "~" H 800 3100 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10F104ZB8NNNC_100nF-104-80-20-50V_C1688.html" H 800 3100 50  0001 C CNN "URL"
	1    800  3100
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C2
U 1 1 5E0D1278
P 800 4000
F 0 "C2" H 892 4046 50  0000 L CNN
F 1 "100n" H 892 3955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 800 4000 50  0001 C CNN
F 3 "~" H 800 4000 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10F104ZB8NNNC_100nF-104-80-20-50V_C1688.html" H 800 4000 50  0001 C CNN "URL"
	1    800  4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	800  4100 800  4150
Connection ~ 800  4150
Wire Wire Line
	800  3850 800  3900
Wire Wire Line
	800  3800 800  3850
Connection ~ 800  3850
Wire Wire Line
	800  3500 800  3450
Connection ~ 800  3450
Wire Wire Line
	800  3250 800  3200
Connection ~ 800  3250
Wire Wire Line
	800  3000 800  2950
Wire Wire Line
	800  2500 800  2600
Wire Wire Line
	800  2950 1000 2950
Wire Wire Line
	1000 2950 1000 3050
Wire Wire Line
	1000 3050 1050 3050
Connection ~ 800  2950
Wire Wire Line
	800  2950 800  2900
Wire Wire Line
	1050 3950 1000 3950
Wire Wire Line
	1000 3950 1000 3850
Wire Wire Line
	1000 3850 800  3850
Wire Wire Line
	950  6100 1000 6100
Wire Wire Line
	3300 4950 3100 4950
$Comp
L Device:R R6
U 1 1 5E1CCC4A
P 2600 5150
F 0 "R6" V 2500 5150 50  0000 C CNN
F 1 "330" V 2700 5150 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2530 5150 50  0001 C CNN
F 3 "~" H 2600 5150 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_FH-Guangdong-Fenghua-Advanced-Tech-RS-03K331JT_C22104.html" H 2600 5150 50  0001 C CNN "URL"
	1    2600 5150
	0    1    1    0   
$EndComp
$Comp
L Device:R R9
U 1 1 5E1CD18B
P 3450 5350
F 0 "R9" V 3350 5350 50  0000 C CNN
F 1 "330" V 3550 5350 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3380 5350 50  0001 C CNN
F 3 "~" H 3450 5350 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_FH-Guangdong-Fenghua-Advanced-Tech-RS-03K331JT_C22104.html" H 3450 5350 50  0001 C CNN "URL"
	1    3450 5350
	0    1    1    0   
$EndComp
Wire Wire Line
	3300 5350 3100 5350
Wire Wire Line
	2450 5150 2150 5150
$Comp
L Device:C_Small C5
U 1 1 5E1E01D6
P 2150 5050
F 0 "C5" H 2242 5096 50  0000 L CNN
F 1 "100n" H 2242 5005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2150 5050 50  0001 C CNN
F 3 "~" H 2150 5050 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10F104ZB8NNNC_100nF-104-80-20-50V_C1688.html" H 2150 5050 50  0001 C CNN "URL"
	1    2150 5050
	1    0    0    -1  
$EndComp
Connection ~ 2150 4950
Wire Wire Line
	2150 4950 2100 4950
Connection ~ 2150 5150
Wire Wire Line
	2150 5150 2100 5150
$Comp
L Device:C_Small C7
U 1 1 5E20674A
P 3100 5150
F 0 "C7" H 3192 5196 50  0000 L CNN
F 1 "100n" H 3192 5105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 3100 5150 50  0001 C CNN
F 3 "~" H 3100 5150 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10F104ZB8NNNC_100nF-104-80-20-50V_C1688.html" H 3100 5150 50  0001 C CNN "URL"
	1    3100 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 5150 2750 5150
Wire Wire Line
	3100 5250 3100 5350
Connection ~ 3100 5350
Wire Wire Line
	3100 5350 2100 5350
Wire Wire Line
	3100 5050 3100 4950
Connection ~ 3100 4950
Wire Wire Line
	3100 4950 2150 4950
Wire Wire Line
	3650 5350 3600 5350
Text Notes 1150 2450 0    50   ~ 0
Balanceador
Text Notes 1500 4600 0    50   ~ 0
Protección
Text Notes 1350 7050 0    50   ~ 0
Conexión
Wire Wire Line
	7800 1200 7950 1200
Wire Wire Line
	7900 800  8600 800 
Wire Wire Line
	3750 2850 3850 2850
$Comp
L Device:L L1
U 1 1 5E30DFC6
P 5550 2400
F 0 "L1" V 5740 2400 50  0000 C CNN
F 1 "15u" V 5649 2400 50  0000 C CNN
F 2 "chinalandfoot:SLO0630h" H 5550 2400 50  0001 C CNN
F 3 "~" H 5550 2400 50  0001 C CNN
F 4 "SLO0630H150MTT" V 5550 2400 50  0001 C CNN "MPN"
F 5 "https://lcsc.com/product-detail/Power-Inductors_Sunltech-Tech-SLO0630H150MTT_C182182.html" H 5550 2400 50  0001 C CNN "URL"
	1    5550 2400
	0    -1   -1   0   
$EndComp
$Comp
L Device:D_Schottky D1
U 1 1 5E30E9AF
P 5300 2600
F 0 "D1" V 5254 2679 50  0000 L CNN
F 1 "SS34A" V 5345 2679 50  0000 L CNN
F 2 "Diode_SMD:D_SMA" H 5300 2600 50  0001 C CNN
F 3 "~" H 5300 2600 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Schottky-Barrier-Diodes-SBD_SS34_C183891.html" H 5300 2600 50  0001 C CNN "URL"
	1    5300 2600
	0    1    1    0   
$EndComp
$Comp
L Device:CP C12
U 1 1 5E30F8CE
P 6000 2600
F 0 "C12" H 6118 2646 50  0000 L CNN
F 1 "220u" H 6118 2555 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.4" H 6038 2450 50  0001 C CNN
F 3 "~" H 6000 2600 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Others_ROQANG-RT1A221M0605_C267466.html" H 6000 2600 50  0001 C CNN "URL"
	1    6000 2600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R12
U 1 1 5E30FCE3
P 6750 2600
F 0 "R12" H 6820 2646 50  0000 L CNN
F 1 "100k" H 6820 2555 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6680 2600 50  0001 C CNN
F 3 "~" H 6750 2600 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_100KR-1003-1_C14675.html" H 6750 2600 50  0001 C CNN "URL"
	1    6750 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 2400 5300 2400
Wire Wire Line
	5300 2450 5300 2400
Connection ~ 5300 2400
Wire Wire Line
	5300 2400 5400 2400
Wire Wire Line
	5700 2400 6000 2400
Wire Wire Line
	6000 2400 6000 2450
Wire Wire Line
	6000 2400 6400 2400
Wire Wire Line
	6750 2400 6750 2450
Connection ~ 6000 2400
Wire Wire Line
	6750 2750 6750 2800
$Comp
L power:GND #PWR030
U 1 1 5E33EC90
P 5300 2800
F 0 "#PWR030" H 5300 2550 50  0001 C CNN
F 1 "GND" H 5305 2627 50  0000 C CNN
F 2 "" H 5300 2800 50  0001 C CNN
F 3 "" H 5300 2800 50  0001 C CNN
	1    5300 2800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR033
U 1 1 5E33EF1C
P 6000 2800
F 0 "#PWR033" H 6000 2550 50  0001 C CNN
F 1 "GND" H 6005 2627 50  0000 C CNN
F 2 "" H 6000 2800 50  0001 C CNN
F 3 "" H 6000 2800 50  0001 C CNN
	1    6000 2800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR037
U 1 1 5E33F1F2
P 6750 3200
F 0 "#PWR037" H 6750 2950 50  0001 C CNN
F 1 "GND" H 6755 3027 50  0000 C CNN
F 2 "" H 6750 3200 50  0001 C CNN
F 3 "" H 6750 3200 50  0001 C CNN
	1    6750 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 3200 6750 3150
Wire Wire Line
	6000 2800 6000 2750
Wire Wire Line
	5300 2800 5300 2750
Wire Wire Line
	7550 2800 7100 2800
Connection ~ 6750 2800
Wire Wire Line
	6750 2800 6750 2850
$Comp
L power:+5V #PWR039
U 1 1 5E38F33A
P 7200 2400
F 0 "#PWR039" H 7200 2250 50  0001 C CNN
F 1 "+5V" V 7215 2528 50  0000 L CNN
F 2 "" H 7200 2400 50  0001 C CNN
F 3 "" H 7200 2400 50  0001 C CNN
	1    7200 2400
	0    1    1    0   
$EndComp
Wire Wire Line
	7200 2400 7100 2400
Connection ~ 6750 2400
$Comp
L power:+5V #PWR027
U 1 1 5E3982D9
P 4400 900
F 0 "#PWR027" H 4400 750 50  0001 C CNN
F 1 "+5V" V 4415 1028 50  0000 L CNN
F 2 "" H 4400 900 50  0001 C CNN
F 3 "" H 4400 900 50  0001 C CNN
	1    4400 900 
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4400 900  4550 900 
Text Notes 4800 2050 0    50   ~ 0
Regulador 5V
$Comp
L Chinaland:RT8279 U7
U 1 1 5E413619
P 4200 2800
F 0 "U7" H 4200 3173 50  0000 C CNN
F 1 "RT8279" H 4200 3264 50  0000 C CNN
F 2 "Package_SO:SOIC-8-1EP_3.9x4.9mm_P1.27mm_EP2.514x3.2mm_ThermalVias" H 4210 2730 50  0001 C CNN
F 3 "" H 4210 2730 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/DC-DC-Converters_RT8279GSP_C53194.html" H 4200 2800 50  0001 C CNN "URL"
	1    4200 2800
	-1   0    0    1   
$EndComp
NoConn ~ 3850 2550
$Comp
L power:GND #PWR025
U 1 1 5E44CBAF
P 3700 2700
F 0 "#PWR025" H 3700 2450 50  0001 C CNN
F 1 "GND" V 3705 2572 50  0000 R CNN
F 2 "" H 3700 2700 50  0001 C CNN
F 3 "" H 3700 2700 50  0001 C CNN
	1    3700 2700
	0    1    1    0   
$EndComp
Wire Wire Line
	3700 2700 3850 2700
Wire Wire Line
	3850 3000 3800 3000
Wire Wire Line
	3800 3000 3800 3350
Wire Wire Line
	3800 3350 5200 3350
Wire Wire Line
	5200 2400 5200 3000
Wire Wire Line
	7550 2800 7550 2150
Wire Wire Line
	7550 2150 4600 2150
Wire Wire Line
	4600 2150 4600 2550
Wire Wire Line
	4600 2550 4550 2550
$Comp
L Device:C_Small C9
U 1 1 5E473FFF
P 4800 3000
F 0 "C9" V 4571 3000 50  0000 C CNN
F 1 "10n" V 4662 3000 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 4800 3000 50  0001 C CNN
F 3 "~" H 4800 3000 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_FH-Guangdong-Fenghua-Advanced-Tech-0603B103K500NT_C57112.html" H 4800 3000 50  0001 C CNN "URL"
	1    4800 3000
	0    1    1    0   
$EndComp
Wire Wire Line
	4700 3000 4550 3000
Wire Wire Line
	4900 3000 5200 3000
Connection ~ 5200 3000
Wire Wire Line
	5200 3000 5200 3350
$Comp
L Device:C_Small C13
U 1 1 5E4913E3
P 6400 2600
F 0 "C13" H 6308 2554 50  0000 R CNN
F 1 "100n" H 6308 2645 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6400 2600 50  0001 C CNN
F 3 "~" H 6400 2600 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10F104ZB8NNNC_100nF-104-80-20-50V_C1688.html" H 6400 2600 50  0001 C CNN "URL"
	1    6400 2600
	-1   0    0    1   
$EndComp
Wire Wire Line
	6400 2500 6400 2400
Connection ~ 6400 2400
Wire Wire Line
	6400 2400 6750 2400
$Comp
L power:GND #PWR035
U 1 1 5E49AE4D
P 6400 2750
F 0 "#PWR035" H 6400 2500 50  0001 C CNN
F 1 "GND" H 6405 2577 50  0000 C CNN
F 2 "" H 6400 2750 50  0001 C CNN
F 3 "" H 6400 2750 50  0001 C CNN
	1    6400 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 2750 6400 2700
$Comp
L power:GND #PWR026
U 1 1 5E4B119E
P 4200 2300
F 0 "#PWR026" H 4200 2050 50  0001 C CNN
F 1 "GND" H 4205 2127 50  0000 C CNN
F 2 "" H 4200 2300 50  0001 C CNN
F 3 "" H 4200 2300 50  0001 C CNN
	1    4200 2300
	-1   0    0    1   
$EndComp
Wire Wire Line
	4200 2350 4200 2300
$Comp
L Device:R R13
U 1 1 5E30FF8F
P 6750 3000
F 0 "R13" H 6820 3046 50  0000 L CNN
F 1 "31k6" H 6820 2955 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6680 3000 50  0001 C CNN
F 3 "~" H 6750 3000 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF3162T5E_C25967.html" H 6750 3000 50  0001 C CNN "URL"
	1    6750 3000
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C14
U 1 1 5E4632AD
P 7100 2600
F 0 "C14" H 7008 2554 50  0000 R CNN
F 1 "82p" H 7008 2645 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 7100 2600 50  0001 C CNN
F 3 "~" H 7100 2600 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_FH-Guangdong-Fenghua-Advanced-Tech-0603CG820J500NT_C1683.html" H 7100 2600 50  0001 C CNN "URL"
	1    7100 2600
	-1   0    0    1   
$EndComp
Wire Wire Line
	7100 2500 7100 2400
Connection ~ 7100 2400
Wire Wire Line
	7100 2400 6850 2400
Wire Wire Line
	7100 2700 7100 2800
Connection ~ 7100 2800
Wire Wire Line
	7100 2800 6750 2800
Connection ~ 8200 3950
Wire Wire Line
	8200 4850 8200 3950
Wire Wire Line
	8900 4850 8200 4850
Wire Wire Line
	11100 3550 8200 3550
Wire Wire Line
	8900 5250 8900 5150
Wire Wire Line
	8850 5250 8900 5250
$Comp
L Device:D_Schottky D3
U 1 1 5E5EABA8
P 8900 5000
F 0 "D3" V 8854 5079 50  0000 L CNN
F 1 "SS34A" V 8945 5079 50  0000 L CNN
F 2 "Diode_SMD:D_SMA" H 8900 5000 50  0001 C CNN
F 3 "~" H 8900 5000 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Schottky-Barrier-Diodes-SBD_SS34_C183891.html" H 8900 5000 50  0001 C CNN "URL"
	1    8900 5000
	0    1    1    0   
$EndComp
Wire Wire Line
	8200 5350 8250 5350
Wire Wire Line
	8200 5850 8200 5350
Wire Wire Line
	8100 5850 8200 5850
Wire Wire Line
	6250 4250 6550 4250
$Comp
L power:GND #PWR044
U 1 1 5E4F0960
P 8050 6100
F 0 "#PWR044" H 8050 5850 50  0001 C CNN
F 1 "GND" H 8055 5927 50  0000 C CNN
F 2 "" H 8050 6100 50  0001 C CNN
F 3 "" H 8050 6100 50  0001 C CNN
	1    8050 6100
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_POT RV1
U 1 1 5E4DE4A3
P 7950 5850
F 0 "RV1" V 8150 6000 50  0000 C CNN
F 1 "3313J-1-503E" V 7800 5850 50  0000 C CNN
F 2 "chinalandfoot:Bourns 3313J" H 7950 5850 50  0001 C CNN
F 3 "~" H 7950 5850 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-Potentiometer_BOURNS_3313J-1-204E_200KR-10_C124622.html" H 7950 5850 50  0001 C CNN "URL"
	1    7950 5850
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR043
U 1 1 5E4CA2F2
P 7950 5600
F 0 "#PWR043" H 7950 5450 50  0001 C CNN
F 1 "+5V" V 7965 5728 50  0000 L CNN
F 2 "" H 7950 5600 50  0001 C CNN
F 3 "" H 7950 5600 50  0001 C CNN
	1    7950 5600
	1    0    0    -1  
$EndComp
Connection ~ 7700 5700
Wire Wire Line
	7700 5150 8250 5150
Wire Wire Line
	7700 5700 7700 5150
$Comp
L Amplifier_Operational:LM358 U9
U 2 1 5E49CE94
P 8550 5250
F 0 "U9" H 8550 5617 50  0000 C CNN
F 1 "LM358" H 8550 5526 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 8550 5250 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2904-n.pdf" H 8550 5250 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Others_IDCHIP-LM358_C329455.html" H 8550 5250 50  0001 C CNN "URL"
	2    8550 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	10700 4200 10350 4200
Connection ~ 10700 4200
Wire Wire Line
	10700 4100 10700 4200
Wire Wire Line
	10700 3800 10350 3800
Connection ~ 10700 3800
Wire Wire Line
	10700 3900 10700 3800
$Comp
L Device:C_Small C21
U 1 1 5E4217EF
P 10700 4000
F 0 "C21" H 10608 3954 50  0000 R CNN
F 1 "82p" H 10608 4045 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 10700 4000 50  0001 C CNN
F 3 "~" H 10700 4000 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_FH-Guangdong-Fenghua-Advanced-Tech-0603CG820J500NT_C1683.html" H 10700 4000 50  0001 C CNN "URL"
	1    10700 4000
	-1   0    0    1   
$EndComp
Wire Wire Line
	6450 5500 6450 5600
$Comp
L power:GND #PWR036
U 1 1 5E3C7934
P 6450 5500
F 0 "#PWR036" H 6450 5250 50  0001 C CNN
F 1 "GND" V 6455 5372 50  0000 R CNN
F 2 "" H 6450 5500 50  0001 C CNN
F 3 "" H 6450 5500 50  0001 C CNN
	1    6450 5500
	-1   0    0    1   
$EndComp
Wire Wire Line
	6250 5500 6250 4250
Wire Wire Line
	5800 5500 6250 5500
$Comp
L power:GND #PWR038
U 1 1 5E346CB4
P 6950 5600
F 0 "#PWR038" H 6950 5350 50  0001 C CNN
F 1 "GND" V 6955 5472 50  0000 R CNN
F 2 "" H 6950 5600 50  0001 C CNN
F 3 "" H 6950 5600 50  0001 C CNN
	1    6950 5600
	0    1    1    0   
$EndComp
Wire Wire Line
	6400 5600 6450 5600
Wire Wire Line
	6050 5600 6100 5600
Connection ~ 6050 5600
Wire Wire Line
	6050 5850 6050 5600
Wire Wire Line
	6400 5850 6050 5850
Wire Wire Line
	6400 5800 6400 5850
Wire Wire Line
	6550 5800 6400 5800
Connection ~ 6950 5800
Wire Wire Line
	7000 5800 6950 5800
Wire Wire Line
	6950 5800 6850 5800
Wire Wire Line
	6950 6050 6950 5800
Wire Wire Line
	7100 6050 6950 6050
Wire Wire Line
	7700 5700 7600 5700
Wire Wire Line
	7700 6050 7700 5700
Wire Wire Line
	7400 6050 7700 6050
$Comp
L Device:R R14
U 1 1 5E2E07B4
P 7250 6050
F 0 "R14" V 7350 6050 50  0000 C CNN
F 1 "100k" V 7450 6050 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7180 6050 50  0001 C CNN
F 3 "~" H 7250 6050 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_100KR-1003-1_C14675.html" H 7250 6050 50  0001 C CNN "URL"
	1    7250 6050
	0    1    1    0   
$EndComp
$Comp
L Device:R R11
U 1 1 5E2DFFB0
P 6700 5800
F 0 "R11" V 6800 5800 50  0000 C CNN
F 1 "10k" V 6900 5800 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6630 5800 50  0001 C CNN
F 3 "~" H 6700 5800 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/New-Arrivals_SANYEAR-SY0603JN10KP_C380762.html" H 6700 5800 50  0001 C CNN "URL"
	1    6700 5800
	0    1    1    0   
$EndComp
Wire Wire Line
	5800 5600 6050 5600
$Comp
L Device:R R10
U 1 1 5E27C8C8
P 6250 5600
F 0 "R10" V 6350 5600 50  0000 C CNN
F 1 "50m" V 6450 5600 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6180 5600 50  0001 C CNN
F 3 "~" H 6250 5600 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_0-05R-R050-1_C158895.html" H 6250 5600 50  0001 C CNN "URL"
	1    6250 5600
	0    1    1    0   
$EndComp
Wire Wire Line
	9850 5150 9950 5150
$Comp
L power:+5V #PWR048
U 1 1 5E25D2F3
P 9850 5150
F 0 "#PWR048" H 9850 5000 50  0001 C CNN
F 1 "+5V" V 9865 5278 50  0000 L CNN
F 2 "" H 9850 5150 50  0001 C CNN
F 3 "" H 9850 5150 50  0001 C CNN
	1    9850 5150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10650 5150 10550 5150
$Comp
L power:GND #PWR051
U 1 1 5E24A8FA
P 10650 5150
F 0 "#PWR051" H 10650 4900 50  0001 C CNN
F 1 "GND" V 10655 5022 50  0000 R CNN
F 2 "" H 10650 5150 50  0001 C CNN
F 3 "" H 10650 5150 50  0001 C CNN
	1    10650 5150
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J8
U 1 1 5E225D77
P 5600 5500
F 0 "J8" H 5708 5681 50  0000 C CNN
F 1 "Conn_01x02_Male" H 5708 5590 50  0000 C CNN
F 2 "Connector_JST:JST_EH_B2B-EH-A_1x02_P2.50mm_Vertical" H 5600 5500 50  0001 C CNN
F 3 "~" H 5600 5500 50  0001 C CNN
	1    5600 5500
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:LM358 U9
U 3 1 5E5161E5
P 10250 5050
F 0 "U9" H 10208 5096 50  0000 L CNN
F 1 "LM358" H 10208 5005 50  0000 L CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 10250 5050 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2904-n.pdf" H 10250 5050 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Others_IDCHIP-LM358_C329455.html" H 10250 5050 50  0001 C CNN "URL"
	3    10250 5050
	0    -1   -1   0   
$EndComp
$Comp
L Amplifier_Operational:LM358 U9
U 1 1 5E514410
P 7300 5700
F 0 "U9" V 7254 5930 50  0000 L CNN
F 1 "LM358" V 7345 5930 50  0000 L CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 7300 5700 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2904-n.pdf" H 7300 5700 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Others_IDCHIP-LM358_C329455.html" H 7300 5700 50  0001 C CNN "URL"
	1    7300 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 3750 7800 3700
$Comp
L power:GND #PWR041
U 1 1 5E4EADF0
P 7800 3700
F 0 "#PWR041" H 7800 3450 50  0001 C CNN
F 1 "GND" H 7805 3527 50  0000 C CNN
F 2 "" H 7800 3700 50  0001 C CNN
F 3 "" H 7800 3700 50  0001 C CNN
	1    7800 3700
	-1   0    0    1   
$EndComp
Wire Wire Line
	10000 4200 10000 4100
$Comp
L power:GND #PWR049
U 1 1 5E4EADE9
P 10000 4200
F 0 "#PWR049" H 10000 3950 50  0001 C CNN
F 1 "GND" H 10005 4027 50  0000 C CNN
F 2 "" H 10000 4200 50  0001 C CNN
F 3 "" H 10000 4200 50  0001 C CNN
	1    10000 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	10000 3800 10350 3800
Connection ~ 10000 3800
Wire Wire Line
	10000 3900 10000 3800
$Comp
L Device:C_Small C19
U 1 1 5E4EADE0
P 10000 4000
F 0 "C19" H 9908 3954 50  0000 R CNN
F 1 "100n" H 9908 4045 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 10000 4000 50  0001 C CNN
F 3 "~" H 10000 4000 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10F104ZB8NNNC_100nF-104-80-20-50V_C1688.html" H 10000 4000 50  0001 C CNN "URL"
	1    10000 4000
	-1   0    0    1   
$EndComp
Wire Wire Line
	8650 4400 8650 4700
Connection ~ 8650 4400
Wire Wire Line
	8500 4400 8650 4400
Wire Wire Line
	8300 4400 8150 4400
$Comp
L Device:C_Small C16
U 1 1 5E4EADD6
P 8400 4400
F 0 "C16" V 8171 4400 50  0000 C CNN
F 1 "10n" V 8262 4400 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 8400 4400 50  0001 C CNN
F 3 "~" H 8400 4400 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_FH-Guangdong-Fenghua-Advanced-Tech-0603B103K500NT_C57112.html" H 8400 4400 50  0001 C CNN "URL"
	1    8400 4400
	0    1    1    0   
$EndComp
Wire Wire Line
	8200 3950 8150 3950
Wire Wire Line
	8200 3550 8200 3950
Wire Wire Line
	11100 4200 11100 3550
Wire Wire Line
	8650 3800 8650 4400
Wire Wire Line
	7400 4700 8650 4700
Wire Wire Line
	7400 4400 7400 4700
Wire Wire Line
	7450 4400 7400 4400
Wire Wire Line
	7300 4100 7450 4100
$Comp
L power:GND #PWR040
U 1 1 5E4EADC7
P 7300 4100
F 0 "#PWR040" H 7300 3850 50  0001 C CNN
F 1 "GND" V 7305 3972 50  0000 R CNN
F 2 "" H 7300 4100 50  0001 C CNN
F 3 "" H 7300 4100 50  0001 C CNN
	1    7300 4100
	0    1    1    0   
$EndComp
NoConn ~ 7450 3950
$Comp
L Chinaland:RT8279 U10
U 1 1 5E4EADC0
P 7800 4200
F 0 "U10" H 7800 4573 50  0000 C CNN
F 1 "RT8279" H 7800 4664 50  0000 C CNN
F 2 "Package_SO:SOIC-8-1EP_3.9x4.9mm_P1.27mm_EP2.514x3.2mm_ThermalVias" H 7810 4130 50  0001 C CNN
F 3 "" H 7810 4130 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/DC-DC-Converters_RT8279GSP_C53194.html" H 7800 4200 50  0001 C CNN "URL"
	1    7800 4200
	-1   0    0    1   
$EndComp
Text Notes 8400 3450 0    50   ~ 0
Cargador batería
Connection ~ 10350 3800
Wire Wire Line
	10850 3800 10700 3800
Wire Wire Line
	10350 4200 10350 4250
Connection ~ 10350 4200
Wire Wire Line
	11100 4200 10700 4200
Wire Wire Line
	8900 4200 8900 4150
Wire Wire Line
	9600 4200 9600 4150
Wire Wire Line
	10350 4600 10350 4550
$Comp
L power:GND #PWR050
U 1 1 5E4EADAB
P 10350 4600
F 0 "#PWR050" H 10350 4350 50  0001 C CNN
F 1 "GND" H 10355 4427 50  0000 C CNN
F 2 "" H 10350 4600 50  0001 C CNN
F 3 "" H 10350 4600 50  0001 C CNN
	1    10350 4600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR047
U 1 1 5E4EADA5
P 9600 4200
F 0 "#PWR047" H 9600 3950 50  0001 C CNN
F 1 "GND" H 9605 4027 50  0000 C CNN
F 2 "" H 9600 4200 50  0001 C CNN
F 3 "" H 9600 4200 50  0001 C CNN
	1    9600 4200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR046
U 1 1 5E4EAD9F
P 8900 4200
F 0 "#PWR046" H 8900 3950 50  0001 C CNN
F 1 "GND" H 8905 4027 50  0000 C CNN
F 2 "" H 8900 4200 50  0001 C CNN
F 3 "" H 8900 4200 50  0001 C CNN
	1    8900 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	10350 4150 10350 4200
Connection ~ 9600 3800
Wire Wire Line
	10350 3800 10350 3850
Wire Wire Line
	9600 3800 10000 3800
Wire Wire Line
	9600 3800 9600 3850
Wire Wire Line
	9300 3800 9600 3800
Wire Wire Line
	8900 3800 9000 3800
Connection ~ 8900 3800
Wire Wire Line
	8900 3850 8900 3800
Wire Wire Line
	8650 3800 8900 3800
$Comp
L Device:R R21
U 1 1 5E4EAD8F
P 10350 4400
F 0 "R21" H 10420 4446 50  0000 L CNN
F 1 "16.9k" H 10420 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 10280 4400 50  0001 C CNN
F 3 "~" H 10350 4400 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_FH-Guangdong-Fenghua-Advanced-Tech-RS-03K1692FT_C304709.html" H 10350 4400 50  0001 C CNN "URL"
	1    10350 4400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R20
U 1 1 5E4EAD89
P 10350 4000
F 0 "R20" H 10420 4046 50  0000 L CNN
F 1 "100k" H 10420 3955 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 10280 4000 50  0001 C CNN
F 3 "~" H 10350 4000 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_100KR-1003-1_C14675.html" H 10350 4000 50  0001 C CNN "URL"
	1    10350 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C18
U 1 1 5E4EAD83
P 9600 4000
F 0 "C18" H 9718 4046 50  0000 L CNN
F 1 "220u" H 9718 3955 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.4" H 9638 3850 50  0001 C CNN
F 3 "~" H 9600 4000 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Others_ROQANG-RT1A221M0605_C267466.html" H 9600 4000 50  0001 C CNN "URL"
	1    9600 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Schottky D2
U 1 1 5E4EAD7D
P 8900 4000
F 0 "D2" V 8854 4079 50  0000 L CNN
F 1 "SS34A" V 8945 4079 50  0000 L CNN
F 2 "Diode_SMD:D_SMA" H 8900 4000 50  0001 C CNN
F 3 "~" H 8900 4000 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Schottky-Barrier-Diodes-SBD_SS34_C183891.html" H 8900 4000 50  0001 C CNN "URL"
	1    8900 4000
	0    1    1    0   
$EndComp
$Comp
L Device:L L3
U 1 1 5E4EAD77
P 9150 3800
F 0 "L3" V 9340 3800 50  0000 C CNN
F 1 "15u" V 9249 3800 50  0000 C CNN
F 2 "chinalandfoot:SLO0630h" H 9150 3800 50  0001 C CNN
F 3 "~" H 9150 3800 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Power-Inductors_Sunltech-Tech-SLO0630H150MTT_C182182.html" H 9150 3800 50  0001 C CNN "URL"
	1    9150 3800
	0    -1   -1   0   
$EndComp
Wire Notes Line
	450  2150 3700 2150
Wire Notes Line
	3700 2150 3700 450 
Wire Notes Line
	6800 1850 6800 450 
Wire Notes Line
	7650 2250 11250 2250
Wire Notes Line
	3700 1850 7650 1850
Wire Notes Line
	7650 1850 7650 3650
Wire Notes Line
	3950 6850 450  6850
Wire Notes Line
	450  4450 3950 4450
Wire Notes Line
	3950 3650 3950 7800
Wire Notes Line
	3350 3650 3350 2150
Wire Notes Line
	3350 3650 7650 3650
Wire Wire Line
	7950 5600 7950 5700
Wire Wire Line
	8050 6100 7950 6100
Wire Wire Line
	7950 6100 7950 6000
Wire Wire Line
	6950 5600 7000 5600
Text GLabel 7800 1200 0    50   Input ~ 0
PWR_CTL
Text GLabel 3250 1150 2    50   Input ~ 0
PWR_CTL
$Comp
L Chinaland:FS8205A Q1
U 1 1 5E3C1174
P 1600 6200
F 0 "Q1" H 1600 6715 50  0000 C CNN
F 1 "FS8205A" H 1600 6624 50  0000 C CNN
F 2 "Package_SO:TSSOP-8_4.4x3mm_P0.65mm" H 1550 6300 50  0001 C CNN
F 3 "" H 1550 6300 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/MOSFET_Fortune-Semicon-8205A_C14212.html" H 1600 6200 50  0001 C CNN "URL"
	1    1600 6200
	1    0    0    -1  
$EndComp
Connection ~ 1000 6100
Connection ~ 2200 6100
Text GLabel 1500 7100 0    50   BiDi ~ 0
V+
Text GLabel 1500 7200 0    50   BiDi ~ 0
VM
Text GLabel 1500 7300 0    50   BiDi ~ 0
V-
Text GLabel 2800 5150 2    50   BiDi ~ 0
V+
Text GLabel 950  6100 0    50   BiDi ~ 0
V-
Text GLabel 3300 4950 2    50   BiDi ~ 0
V-
Text GLabel 3650 5350 2    50   BiDi ~ 0
VM
Text GLabel 2600 4350 2    50   BiDi ~ 0
V-
Text GLabel 2600 3450 2    50   BiDi ~ 0
VM
Text GLabel 2600 2500 2    50   BiDi ~ 0
V+
Text GLabel 3750 2850 0    50   BiDi ~ 0
V+
Text GLabel 7900 800  0    50   BiDi ~ 0
V+
Text GLabel 10850 3800 2    50   BiDi ~ 0
V+
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5E4B76F6
P 10750 800
F 0 "#FLG0101" H 10750 875 50  0001 C CNN
F 1 "PWR_FLAG" H 10750 973 50  0000 C CNN
F 2 "" H 10750 800 50  0001 C CNN
F 3 "~" H 10750 800 50  0001 C CNN
	1    10750 800 
	1    0    0    -1  
$EndComp
Connection ~ 10750 800 
Wire Wire Line
	10750 800  10850 800 
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5E4E1236
P 1800 7100
F 0 "#FLG0102" H 1800 7175 50  0001 C CNN
F 1 "PWR_FLAG" H 1800 7273 50  0000 C CNN
F 2 "" H 1800 7100 50  0001 C CNN
F 3 "~" H 1800 7100 50  0001 C CNN
	1    1800 7100
	1    0    0    -1  
$EndComp
Connection ~ 1800 7100
Wire Wire Line
	1800 7100 1500 7100
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 5E4E1619
P 2050 7200
F 0 "#FLG0103" H 2050 7275 50  0001 C CNN
F 1 "PWR_FLAG" H 2050 7373 50  0000 C CNN
F 2 "" H 2050 7200 50  0001 C CNN
F 3 "~" H 2050 7200 50  0001 C CNN
	1    2050 7200
	1    0    0    -1  
$EndComp
Connection ~ 2050 7200
Wire Wire Line
	2050 7200 1500 7200
$Comp
L power:PWR_FLAG #FLG0104
U 1 1 5E4E193E
P 1400 1000
F 0 "#FLG0104" H 1400 1075 50  0001 C CNN
F 1 "PWR_FLAG" H 1400 1173 50  0000 C CNN
F 2 "" H 1400 1000 50  0001 C CNN
F 3 "~" H 1400 1000 50  0001 C CNN
	1    1400 1000
	1    0    0    -1  
$EndComp
Connection ~ 1400 1000
Wire Wire Line
	1400 1000 1800 1000
$Comp
L power:PWR_FLAG #FLG0105
U 1 1 5E4E34F4
P 6550 4250
F 0 "#FLG0105" H 6550 4325 50  0001 C CNN
F 1 "PWR_FLAG" H 6550 4423 50  0000 C CNN
F 2 "" H 6550 4250 50  0001 C CNN
F 3 "~" H 6550 4250 50  0001 C CNN
	1    6550 4250
	1    0    0    -1  
$EndComp
Connection ~ 6550 4250
Wire Wire Line
	6550 4250 7450 4250
$Comp
L power:PWR_FLAG #FLG0106
U 1 1 5E4E9318
P 6850 2400
F 0 "#FLG0106" H 6850 2475 50  0001 C CNN
F 1 "PWR_FLAG" H 6850 2573 50  0000 C CNN
F 2 "" H 6850 2400 50  0001 C CNN
F 3 "~" H 6850 2400 50  0001 C CNN
	1    6850 2400
	1    0    0    -1  
$EndComp
Connection ~ 6850 2400
Wire Wire Line
	6850 2400 6750 2400
Wire Wire Line
	6000 900  6200 900 
$Comp
L Device:Q_NMOS_GSD Q2
U 1 1 5DFD92F7
P 2300 3200
F 0 "Q2" H 2504 3246 50  0000 L CNN
F 1 "2N7002K" H 2504 3155 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23_Handsoldering" H 2500 3300 50  0001 C CNN
F 3 "~" H 2300 3200 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/MOSFET_Guangdong-Hottech-2N7002K_C181083.html" H 2300 3200 50  0001 C CNN "URL"
	1    2300 3200
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_NMOS_GSD Q3
U 1 1 5DFED561
P 2300 4100
F 0 "Q3" H 2504 4146 50  0000 L CNN
F 1 "2N7002K" H 2504 4055 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23_Handsoldering" H 2500 4200 50  0001 C CNN
F 3 "~" H 2300 4100 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/MOSFET_Guangdong-Hottech-2N7002K_C181083.html" H 2300 4100 50  0001 C CNN "URL"
	1    2300 4100
	1    0    0    -1  
$EndComp
$EndSCHEMATC
