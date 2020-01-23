EESchema Schematic File Version 4
LIBS:prockeckv2-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 10
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
L Device:C_Small C29
U 1 1 5DA8BA1D
P 5950 1000
F 0 "C29" H 6042 1046 50  0000 L CNN
F 1 "1u" H 6042 955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5950 1000 50  0001 C CNN
F 3 "~" H 5950 1000 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10A105KB8NNNC_1uF-105-10-50V_C15849.html" H 5950 1000 50  0001 C CNN "URL"
	1    5950 1000
	1    0    0    -1  
$EndComp
$Comp
L MCU_ST_STM32F1:STM32F103VETx U12
U 1 1 5DA8BF00
P 4300 3900
F 0 "U12" H 4300 1014 50  0000 C CNN
F 1 "STM32F103VETx" H 4300 923 50  0000 C CNN
F 2 "Package_QFP:LQFP-100_14x14mm_P0.5mm" H 3500 1300 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/CD00191185.pdf" H 4300 3900 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/ST-Microelectronics_STMicroelectronics_STM32F103VET6_STM32F103VET6_C8308.html" H 4300 3900 50  0001 C CNN "URL"
	1    4300 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 6700 4300 6700
Connection ~ 4300 6700
Wire Wire Line
	4300 6700 4400 6700
Connection ~ 4400 6700
Wire Wire Line
	4400 6700 4500 6700
Connection ~ 4500 6700
Wire Wire Line
	4500 6700 4600 6700
Connection ~ 4600 6700
Wire Wire Line
	4600 6700 4700 6700
$Comp
L power:GND #PWR064
U 1 1 5DCE4321
P 4700 6800
F 0 "#PWR064" H 4700 6550 50  0001 C CNN
F 1 "GND" H 4705 6627 50  0000 C CNN
F 2 "" H 4700 6800 50  0001 C CNN
F 3 "" H 4700 6800 50  0001 C CNN
	1    4700 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 6800 4700 6700
Connection ~ 4700 6700
$Comp
L Device:C_Small C30
U 1 1 5DD13546
P 6250 1000
F 0 "C30" H 6342 1046 50  0000 L CNN
F 1 "10n" H 6342 955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6250 1000 50  0001 C CNN
F 3 "~" H 6250 1000 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_FH-Guangdong-Fenghua-Advanced-Tech-0603B103K500NT_C57112.html" H 6250 1000 50  0001 C CNN "URL"
	1    6250 1000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR066
U 1 1 5DD17A58
P 6250 1150
F 0 "#PWR066" H 6250 900 50  0001 C CNN
F 1 "GND" H 6255 977 50  0000 C CNN
F 2 "" H 6250 1150 50  0001 C CNN
F 3 "" H 6250 1150 50  0001 C CNN
	1    6250 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 1150 6250 1100
Wire Wire Line
	5950 1100 6250 1100
Connection ~ 6250 1100
Wire Wire Line
	6250 900  5950 900 
$Comp
L Device:C_Small C27
U 1 1 5DD23F6F
P 5250 1000
F 0 "C27" H 5342 1046 50  0000 L CNN
F 1 "1u" H 5342 955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5250 1000 50  0001 C CNN
F 3 "~" H 5250 1000 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10A105KB8NNNC_1uF-105-10-50V_C15849.html" H 5250 1000 50  0001 C CNN "URL"
	1    5250 1000
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C28
U 1 1 5DD23F75
P 5550 1000
F 0 "C28" H 5642 1046 50  0000 L CNN
F 1 "10n" H 5642 955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5550 1000 50  0001 C CNN
F 3 "~" H 5550 1000 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_FH-Guangdong-Fenghua-Advanced-Tech-0603B103K500NT_C57112.html" H 5550 1000 50  0001 C CNN "URL"
	1    5550 1000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR065
U 1 1 5DD23F7B
P 5550 1150
F 0 "#PWR065" H 5550 900 50  0001 C CNN
F 1 "GND" H 5555 977 50  0000 C CNN
F 2 "" H 5550 1150 50  0001 C CNN
F 3 "" H 5550 1150 50  0001 C CNN
	1    5550 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 1150 5550 1100
Wire Wire Line
	5250 1100 5550 1100
Connection ~ 5550 1100
$Comp
L Device:C_Small C31
U 1 1 5DD2AD47
P 6650 1000
F 0 "C31" H 6742 1046 50  0000 L CNN
F 1 "1u" H 6742 955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6650 1000 50  0001 C CNN
F 3 "~" H 6650 1000 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10A105KB8NNNC_1uF-105-10-50V_C15849.html" H 6650 1000 50  0001 C CNN "URL"
	1    6650 1000
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C32
U 1 1 5DD2AD4D
P 6950 1000
F 0 "C32" H 7042 1046 50  0000 L CNN
F 1 "10n" H 7042 955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6950 1000 50  0001 C CNN
F 3 "~" H 6950 1000 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_FH-Guangdong-Fenghua-Advanced-Tech-0603B103K500NT_C57112.html" H 6950 1000 50  0001 C CNN "URL"
	1    6950 1000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR067
U 1 1 5DD2AD53
P 6950 1150
F 0 "#PWR067" H 6950 900 50  0001 C CNN
F 1 "GND" H 6955 977 50  0000 C CNN
F 2 "" H 6950 1150 50  0001 C CNN
F 3 "" H 6950 1150 50  0001 C CNN
	1    6950 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 1150 6950 1100
Wire Wire Line
	6650 1100 6950 1100
Connection ~ 6950 1100
Wire Wire Line
	6950 900  6650 900 
Wire Wire Line
	6250 900  6650 900 
Connection ~ 6250 900 
Connection ~ 6650 900 
Wire Wire Line
	6650 1100 6250 1100
Connection ~ 6650 1100
Wire Wire Line
	5550 1100 5950 1100
Connection ~ 5950 1100
Wire Wire Line
	5950 900  5550 900 
Connection ~ 5950 900 
Connection ~ 5550 900 
Wire Wire Line
	4300 1200 4400 1200
Connection ~ 4400 1200
Wire Wire Line
	4400 1200 4500 1200
Connection ~ 4500 1200
Wire Wire Line
	4500 1200 4600 1200
Connection ~ 4600 1200
Wire Wire Line
	4600 1200 4700 1200
Connection ~ 4700 1200
Wire Wire Line
	4700 1200 4800 1200
Wire Wire Line
	4700 900  4700 1200
Wire Wire Line
	4700 900  5250 900 
Connection ~ 5250 900 
Wire Wire Line
	5250 900  5550 900 
$Comp
L power:+3.3V #PWR063
U 1 1 5DD4C535
P 4700 750
F 0 "#PWR063" H 4700 600 50  0001 C CNN
F 1 "+3.3V" H 4715 923 50  0000 C CNN
F 2 "" H 4700 750 50  0001 C CNN
F 3 "" H 4700 750 50  0001 C CNN
	1    4700 750 
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 900  4700 750 
Connection ~ 4700 900 
$Comp
L power:+3.3V #PWR062
U 1 1 5DD50144
P 3200 1900
F 0 "#PWR062" H 3200 1750 50  0001 C CNN
F 1 "+3.3V" H 3215 2073 50  0000 C CNN
F 2 "" H 3200 1900 50  0001 C CNN
F 3 "" H 3200 1900 50  0001 C CNN
	1    3200 1900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3200 1900 3300 1900
$Comp
L power:GND #PWR060
U 1 1 5DD525CE
P 2950 2000
F 0 "#PWR060" H 2950 1750 50  0001 C CNN
F 1 "GND" H 2955 1827 50  0000 C CNN
F 2 "" H 2950 2000 50  0001 C CNN
F 3 "" H 2950 2000 50  0001 C CNN
	1    2950 2000
	0    1    1    0   
$EndComp
Wire Wire Line
	2950 2000 3300 2000
Text GLabel 4200 1150 1    50   Input ~ 0
VBAT
Wire Wire Line
	4200 1150 4200 1200
Text GLabel 2800 1500 0    50   Input ~ 0
~RST
Wire Wire Line
	2800 1500 2850 1500
Text GLabel 3250 1700 0    50   Input ~ 0
BOOT0
Wire Wire Line
	3250 1700 3300 1700
Wire Wire Line
	2500 2450 2500 2500
Wire Wire Line
	2500 2500 3300 2500
Wire Wire Line
	3300 2500 3300 2300
Wire Wire Line
	3300 2200 3300 2100
$Comp
L Device:C_Small C25
U 1 1 5DD6AE7A
P 2500 2650
F 0 "C25" H 2592 2696 50  0000 L CNN
F 1 "20p" H 2592 2605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2500 2650 50  0001 C CNN
F 3 "~" H 2500 2650 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10C200JB8NNNC_20pF-200-5-50V_C1648.html" H 2500 2650 50  0001 C CNN "URL"
	1    2500 2650
	1    0    0    -1  
$EndComp
$Comp
L Device:Crystal_GND24 Y1
U 1 1 5DD5F9B1
P 2500 2300
F 0 "Y1" V 2600 2100 50  0000 R CNN
F 1 "Crystal_GND24" V 2650 2950 50  0000 R CNN
F 2 "Crystal:Crystal_SMD_7050-4Pin_7.0x5.0mm" H 2500 2300 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811081329_Yangxing-Tech-X50328MSB4SI_C157341.pdf" H 2500 2300 50  0001 C CNN
F 4 "https://kdelectronics.blogspot.com/2013/03/using-crystal-resonator-with.html" V 2500 2300 50  0001 C CNN "Calculo"
F 5 "Yangxing Tech X50328MSB4SI" V 2500 2300 50  0001 C CNN "Nombre"
F 6 "https://www.st.com/content/ccc/resource/technical/document/application_note/c6/eb/5e/11/e3/69/43/eb/CD00221665.pdf/files/CD00221665.pdf/jcr:content/translations/en.CD00221665.pdf" V 2500 2300 50  0001 C CNN "Calculo2"
F 7 "https://lcsc.com/product-detail/SMD-Crystal-Resonators_Yangxing-Tech-X50328MSB4SI_C157341.html" H 2500 2300 50  0001 C CNN "URL"
	1    2500 2300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2750 2300 2700 2300
$Comp
L power:GND #PWR058
U 1 1 5DD72FF6
P 2500 2800
F 0 "#PWR058" H 2500 2550 50  0001 C CNN
F 1 "GND" H 2505 2627 50  0000 C CNN
F 2 "" H 2500 2800 50  0001 C CNN
F 3 "" H 2500 2800 50  0001 C CNN
	1    2500 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 2800 2500 2750
$Comp
L power:GND #PWR057
U 1 1 5DD74202
P 2500 1800
F 0 "#PWR057" H 2500 1550 50  0001 C CNN
F 1 "GND" H 2505 1627 50  0000 C CNN
F 2 "" H 2500 1800 50  0001 C CNN
F 3 "" H 2500 1800 50  0001 C CNN
	1    2500 1800
	-1   0    0    1   
$EndComp
Wire Wire Line
	5300 4900 6100 4900
Wire Wire Line
	5300 5000 6100 5000
Wire Wire Line
	5300 5100 6100 5100
Wire Wire Line
	5300 5200 6100 5200
Wire Wire Line
	5300 5300 6100 5300
Wire Wire Line
	5300 5400 6100 5400
Text Label 5350 4900 0    50   ~ 0
ADC_diff0
Text Label 5350 5000 0    50   ~ 0
ADC_diff1
Text Label 5350 5100 0    50   ~ 0
ADC_diff2
Text Label 5350 5200 0    50   ~ 0
ADC_diff3
Text Label 5350 5300 0    50   ~ 0
ADC_diff4
Text Label 5350 5400 0    50   ~ 0
ADC_diff5
Entry Wire Line
	6100 5400 6200 5500
Entry Wire Line
	6100 5300 6200 5400
Entry Wire Line
	6100 5200 6200 5300
Entry Wire Line
	6100 5100 6200 5200
Entry Wire Line
	6100 5000 6200 5100
Entry Wire Line
	6100 4900 6200 5000
Wire Bus Line
	6200 4850 6550 4850
Text HLabel 6550 4850 2    50   Input ~ 0
ADC_diff[0..5]
Wire Wire Line
	5300 1500 6650 1500
Wire Wire Line
	5300 1600 6650 1600
Wire Wire Line
	5300 1700 6650 1700
Wire Wire Line
	5300 1800 6650 1800
Text Label 5350 1500 0    50   ~ 0
ADC_se0
Text Label 5350 1600 0    50   ~ 0
ADC_se1
Text Label 5350 1700 0    50   ~ 0
ADC_se2
Text Label 5350 1800 0    50   ~ 0
ADC_se3
Wire Wire Line
	5300 3200 6650 3200
Wire Wire Line
	5300 3300 6650 3300
Text Label 5350 3200 0    50   ~ 0
ADC_se4
Text Label 5350 3300 0    50   ~ 0
ADC_se5
Entry Wire Line
	6650 1500 6750 1600
Entry Wire Line
	6650 1600 6750 1700
Entry Wire Line
	6650 1700 6750 1800
Entry Wire Line
	6650 1800 6750 1900
Entry Wire Line
	6650 3200 6750 3300
Entry Wire Line
	6650 3300 6750 3400
Wire Bus Line
	6750 1500 6850 1500
Text HLabel 6850 1500 2    50   Input ~ 0
ADC_se[0..5]
Text HLabel 6150 1900 2    50   Output ~ 0
DAC
Wire Wire Line
	6150 1900 5300 1900
Wire Wire Line
	5300 2000 5950 2000
Wire Wire Line
	5300 2100 5950 2100
Wire Wire Line
	5300 2200 5950 2200
Text Label 5350 2000 0    50   ~ 0
SPI1_0
Text Label 5350 2100 0    50   ~ 0
SPI1_1
Text Label 5350 2200 0    50   ~ 0
SPI1_2
Entry Wire Line
	5950 2000 6050 2100
Entry Wire Line
	5950 2100 6050 2200
Entry Wire Line
	5950 2200 6050 2300
Wire Bus Line
	6050 2000 6150 2000
Text HLabel 6150 2000 2    50   BiDi ~ 0
SPI1_[0..2]
Wire Wire Line
	5300 2400 5950 2400
Wire Wire Line
	5300 2500 5950 2500
Entry Wire Line
	5950 2400 6050 2500
Entry Wire Line
	5950 2500 6050 2600
Text Label 5350 2400 0    50   ~ 0
USART1_0
Text Label 5350 2500 0    50   ~ 0
USART1_1
Wire Bus Line
	6050 2400 6150 2400
Text HLabel 6150 2400 2    50   BiDi ~ 0
USART1_[0..1]
Wire Wire Line
	5300 2800 5950 2800
Wire Wire Line
	5300 2900 5950 2900
Text Label 5350 2800 0    50   ~ 0
SWD0
Text Label 5350 2900 0    50   ~ 0
SWD1
Entry Wire Line
	5950 2800 6050 2900
Entry Wire Line
	5950 2900 6050 3000
Wire Bus Line
	6050 2850 6150 2850
Text HLabel 6150 2850 2    50   BiDi ~ 0
SWD[0..1]
Wire Wire Line
	5300 3500 5900 3500
Wire Wire Line
	5300 3600 5900 3600
Wire Wire Line
	5300 3700 5900 3700
Text Label 5350 3500 0    50   ~ 0
SPI3_0
Text Label 5350 3600 0    50   ~ 0
SPI3_1
Text Label 5350 3700 0    50   ~ 0
SPI3_2
Entry Wire Line
	5900 3500 6000 3600
Entry Wire Line
	5900 3600 6000 3700
Entry Wire Line
	5900 3700 6000 3800
Wire Bus Line
	6000 3500 6050 3500
Text HLabel 6050 3500 2    50   BiDi ~ 0
SPI3_[0..2]
Wire Wire Line
	5300 3800 5900 3800
Wire Wire Line
	5300 3900 5900 3900
Text Label 5350 3800 0    50   ~ 0
I2C1_0
Text Label 5350 3900 0    50   ~ 0
I2C1_1
Entry Wire Line
	5900 3800 6000 3900
Entry Wire Line
	5900 3900 6000 4000
Wire Bus Line
	6000 3850 6050 3850
Text HLabel 6050 3850 2    50   BiDi ~ 0
I2C1_[0..1]
Wire Wire Line
	5300 4500 5900 4500
Wire Wire Line
	5300 4600 5900 4600
Wire Wire Line
	5300 4700 5900 4700
Text Label 5350 4500 0    50   ~ 0
SPI2_0
Text Label 5350 4600 0    50   ~ 0
SPI2_1
Text Label 5350 4700 0    50   ~ 0
SPI2_2
Entry Wire Line
	5900 4500 6000 4600
Entry Wire Line
	5900 4600 6000 4700
Wire Bus Line
	6000 4500 6050 4500
Text HLabel 6050 4500 2    50   BiDi ~ 0
SPI2_[0..2]
Wire Wire Line
	5300 4300 5900 4300
Text Label 5350 4200 0    50   ~ 0
I2C2_0
Text Label 5350 4300 0    50   ~ 0
I2C2_1
Entry Wire Line
	5900 4300 6000 4400
Text HLabel 6050 4250 2    50   BiDi ~ 0
I2C2_[0..1]
Wire Wire Line
	5300 5700 5900 5700
Wire Wire Line
	5300 5800 5900 5800
Wire Wire Line
	5300 5900 5900 5900
Wire Wire Line
	5300 6000 5900 6000
Wire Wire Line
	5300 6100 5900 6100
Text Label 5350 5700 0    50   ~ 0
SDIO0
Text Label 5350 5800 0    50   ~ 0
SDIO1
Text Label 5350 5900 0    50   ~ 0
SDIO2
Text Label 5350 6000 0    50   ~ 0
SDIO3
Text Label 5350 6100 0    50   ~ 0
SDIO4
Wire Wire Line
	2850 5100 3300 5100
Text Label 3250 5100 2    50   ~ 0
SDIO5
Entry Wire Line
	5900 5700 6000 5800
Entry Wire Line
	5900 5800 6000 5900
Entry Wire Line
	5900 5900 6000 6000
Entry Wire Line
	5900 6000 6000 6100
Entry Wire Line
	5900 6100 6000 6200
Text HLabel 6000 5700 2    50   BiDi ~ 0
SDIO[0..5]
Text HLabel 2850 5100 0    50   Output ~ 0
SDIO5
Text HLabel 6150 6100 2    50   Input ~ 0
SDIO5
Entry Wire Line
	6100 6100 6000 6200
Wire Wire Line
	3300 5400 2850 5400
Wire Wire Line
	3300 5500 2850 5500
Entry Wire Line
	2850 5400 2750 5500
Entry Wire Line
	2850 5500 2750 5600
Text Label 3250 5400 2    50   ~ 0
USART2_0
Text Label 3250 5500 2    50   ~ 0
USART2_1
Wire Bus Line
	2750 5400 2700 5400
Text HLabel 2700 5400 0    50   BiDi ~ 0
USART2_[0..1]
Wire Wire Line
	3300 5700 2850 5700
Wire Wire Line
	3300 5800 2850 5800
Entry Wire Line
	2850 5700 2750 5800
Entry Wire Line
	2850 5800 2750 5900
Text Label 3250 5700 2    50   ~ 0
USART3_0
Text Label 3250 5800 2    50   ~ 0
USART3_1
Text HLabel 2750 5700 0    50   BiDi ~ 0
USART3_[0..1]
Wire Wire Line
	3300 3200 2650 3200
Wire Wire Line
	3300 3300 2650 3300
Wire Wire Line
	3300 3400 2650 3400
Wire Wire Line
	3300 3500 2650 3500
Wire Wire Line
	3300 3600 2650 3600
Wire Wire Line
	3300 3700 2650 3700
Wire Wire Line
	3300 3800 2650 3800
Wire Wire Line
	3300 3900 2650 3900
Wire Wire Line
	3300 4000 2650 4000
Wire Wire Line
	3300 4200 2650 4200
Wire Wire Line
	3300 4300 2650 4300
Wire Wire Line
	3300 4400 2650 4400
Wire Wire Line
	3300 4500 2650 4500
Wire Wire Line
	3300 4600 2650 4600
Wire Wire Line
	3300 4700 2650 4700
Text Label 3250 3200 2    50   ~ 0
IO0
Text Label 3250 3300 2    50   ~ 0
IO1
Text Label 3250 3400 2    50   ~ 0
IO2
Text Label 3250 3500 2    50   ~ 0
IO3
Text Label 3250 3600 2    50   ~ 0
IO4
Text Label 3250 3700 2    50   ~ 0
IO5
Text Label 3250 3800 2    50   ~ 0
IO6
Text Label 3250 3900 2    50   ~ 0
IO7
Text Label 3250 4000 2    50   ~ 0
IO8
Text Label 3250 4200 2    50   ~ 0
IO9
Text Label 3250 4300 2    50   ~ 0
IO10
Text Label 3250 4400 2    50   ~ 0
IO11
Text Label 3250 4500 2    50   ~ 0
IO12
Text Label 3250 4600 2    50   ~ 0
IO13
Text Label 3250 4700 2    50   ~ 0
IO14
Entry Wire Line
	2650 3200 2550 3300
Entry Wire Line
	2650 3300 2550 3400
Entry Wire Line
	2650 3400 2550 3500
Entry Wire Line
	2650 3500 2550 3600
Entry Wire Line
	2650 3600 2550 3700
Entry Wire Line
	2650 3700 2550 3800
Entry Wire Line
	2650 3800 2550 3900
Entry Wire Line
	2650 3900 2550 4000
Entry Wire Line
	2650 4000 2550 4100
Entry Wire Line
	2650 4200 2550 4300
Entry Wire Line
	2650 4300 2550 4400
Entry Wire Line
	2650 4400 2550 4500
Entry Wire Line
	2650 4500 2550 4600
Entry Wire Line
	2650 4600 2550 4700
Entry Wire Line
	2650 4700 2550 4800
Wire Bus Line
	2550 3200 2450 3200
Text HLabel 2450 3200 0    50   BiDi ~ 0
IO[0..14]
Wire Wire Line
	3300 4900 2150 4900
Wire Wire Line
	3300 5000 2150 5000
Wire Wire Line
	3300 5200 2150 5200
Wire Wire Line
	3300 5300 2150 5300
Wire Wire Line
	3300 5600 2800 5600
Wire Wire Line
	2800 5600 2800 5650
Wire Wire Line
	2800 5650 2700 5650
Wire Wire Line
	2700 5650 2700 5600
Wire Wire Line
	2700 5600 2150 5600
Wire Wire Line
	2800 5900 2800 5950
Wire Wire Line
	2700 5950 2700 5900
Wire Wire Line
	2700 5900 2150 5900
Wire Wire Line
	2700 5950 2800 5950
Wire Wire Line
	2800 5900 3300 5900
Wire Wire Line
	3300 6000 2150 6000
Wire Wire Line
	3300 6200 2150 6200
Wire Wire Line
	2150 6300 3300 6300
Wire Wire Line
	3300 6400 2150 6400
Text Label 3250 4900 2    50   ~ 0
IO20
Text Label 3250 5000 2    50   ~ 0
IO21
Text Label 3250 5200 2    50   ~ 0
IO22
Text Label 3250 5300 2    50   ~ 0
IO23
Text Label 3250 5600 2    50   ~ 0
IO24
Text Label 3250 5900 2    50   ~ 0
IO25
Text Label 3250 6000 2    50   ~ 0
IO26
Text Label 3250 6200 2    50   ~ 0
IO27
Text Label 3250 6300 2    50   ~ 0
IO28
Text Label 3250 6400 2    50   ~ 0
IO29
Wire Wire Line
	5300 6400 5850 6400
Wire Wire Line
	5300 6300 5850 6300
Text Label 5350 6300 0    50   ~ 0
SDI120
Text Label 5350 6400 0    50   ~ 0
SDI121
Text Label 5350 4400 0    50   ~ 0
IO210
Wire Wire Line
	3300 4100 2900 4100
Text Label 3250 4100 2    50   ~ 0
PWM0
Wire Wire Line
	3300 6100 2900 6100
Text Label 3250 6100 2    50   ~ 0
PWM1
Wire Wire Line
	5300 5500 5750 5500
Wire Wire Line
	5300 3000 5700 3000
Wire Wire Line
	5300 2300 6000 2300
Wire Wire Line
	6000 2300 6000 2350
Wire Wire Line
	6000 2350 6100 2350
Wire Wire Line
	6100 2350 6100 2300
Wire Wire Line
	6100 2300 6250 2300
Text Label 5350 2300 0    50   ~ 0
HS_POW0
Wire Wire Line
	5300 3400 6250 3400
Text Label 5350 3400 0    50   ~ 0
HS_POW1
Text Label 5350 4000 0    50   ~ 0
LS_POW0
Text Label 5350 4100 0    50   ~ 0
LS_POW1
Entry Wire Line
	2150 4900 2050 5000
Entry Wire Line
	2150 5000 2050 5100
Entry Wire Line
	2150 5200 2050 5300
Entry Wire Line
	2150 5300 2050 5400
Entry Wire Line
	2150 5600 2050 5700
Entry Wire Line
	2150 5900 2050 6000
Entry Wire Line
	2150 6000 2050 6100
Entry Wire Line
	2150 6200 2050 6300
Entry Wire Line
	2150 6300 2050 6400
Entry Wire Line
	2150 6400 2050 6500
Wire Bus Line
	2050 4900 2000 4900
Text HLabel 2000 4900 0    50   BiDi ~ 0
IO2[0..10]
Text HLabel 2900 4100 0    50   Output ~ 0
PWM0
Text HLabel 2900 6100 0    50   Output ~ 0
PWM1
Wire Wire Line
	5300 4200 5900 4200
Wire Bus Line
	6000 4250 6050 4250
Entry Wire Line
	5900 4200 6000 4300
Wire Wire Line
	5950 4000 5950 4050
Wire Wire Line
	5950 4050 6050 4050
Wire Wire Line
	6050 4050 6050 4000
Wire Wire Line
	6050 4000 6300 4000
Wire Wire Line
	5300 4000 5950 4000
Wire Wire Line
	5300 4100 6300 4100
Wire Wire Line
	5950 4400 5950 4450
Wire Wire Line
	5950 4450 6050 4450
Wire Wire Line
	6050 4450 6050 4400
Wire Wire Line
	6050 4400 6300 4400
Wire Wire Line
	5300 4400 5950 4400
Entry Wire Line
	6300 4000 6400 4100
Entry Wire Line
	6300 4100 6400 4200
Wire Bus Line
	6400 4000 7400 4000
Text HLabel 7400 4000 2    50   Output ~ 0
LS_POW[0..1]
Entry Wire Line
	5850 6300 5950 6400
Entry Wire Line
	5850 6400 5950 6500
Wire Bus Line
	5950 6350 6700 6350
Text HLabel 6700 6350 2    50   BiDi ~ 0
SDI12[0..2]
Text HLabel 6250 2300 2    50   Output ~ 0
HS_POW0
Text HLabel 6250 3400 2    50   Output ~ 0
HS_POW1
Text HLabel 7500 2550 0    50   Input ~ 0
HS_POW0
Text HLabel 7500 2650 0    50   Input ~ 0
HS_POW1
Entry Wire Line
	7550 2550 7650 2650
Entry Wire Line
	7550 2650 7650 2750
Wire Wire Line
	7500 2550 7550 2550
Wire Wire Line
	7500 2650 7550 2650
Text HLabel 8300 2550 0    50   Input ~ 0
PWM0
Text HLabel 8300 2650 0    50   Input ~ 0
PWM1
Text HLabel 6300 4400 2    50   BiDi ~ 0
IO210
Text HLabel 2550 6500 2    50   BiDi ~ 0
IO210
Entry Wire Line
	2050 6600 2150 6500
Wire Wire Line
	2150 6500 2550 6500
$Comp
L Connector:Conn_01x03_Male J9
U 1 1 5E5A0FC8
P 850 1600
F 0 "J9" H 958 1881 50  0000 C CNN
F 1 "Conn_01x03_Male" H 850 1800 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x03_P2.54mm_Vertical" H 850 1600 50  0001 C CNN
F 3 "~" H 850 1600 50  0001 C CNN
	1    850  1600
	1    0    0    -1  
$EndComp
Text GLabel 2050 1600 2    50   Output ~ 0
BOOT0
Wire Wire Line
	2050 1600 1800 1600
$Comp
L power:GND #PWR054
U 1 1 5E5AD45A
P 1150 1700
F 0 "#PWR054" H 1150 1450 50  0001 C CNN
F 1 "GND" H 1100 1550 50  0000 C CNN
F 2 "" H 1150 1700 50  0001 C CNN
F 3 "" H 1150 1700 50  0001 C CNN
	1    1150 1700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1150 1700 1050 1700
$Comp
L power:+3.3V #PWR053
U 1 1 5E5B996B
P 1150 1500
F 0 "#PWR053" H 1150 1350 50  0001 C CNN
F 1 "+3.3V" H 1100 1650 50  0000 C CNN
F 2 "" H 1150 1500 50  0001 C CNN
F 3 "" H 1150 1500 50  0001 C CNN
	1    1150 1500
	0    1    1    0   
$EndComp
Wire Wire Line
	1150 1500 1050 1500
$Comp
L Device:R R22
U 1 1 5E60071F
P 1800 1750
F 0 "R22" H 1870 1796 50  0000 L CNN
F 1 "100k" H 1870 1705 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 1730 1750 50  0001 C CNN
F 3 "~" H 1800 1750 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_100KR-1003-1_C14675.html" H 1800 1750 50  0001 C CNN "URL"
	1    1800 1750
	1    0    0    -1  
$EndComp
Connection ~ 1800 1600
Wire Wire Line
	1800 1600 1450 1600
$Comp
L Device:C_Small C23
U 1 1 5E600966
P 1450 1700
F 0 "C23" H 1542 1746 50  0000 L CNN
F 1 "100n" H 1542 1655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 1450 1700 50  0001 C CNN
F 3 "~" H 1450 1700 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10F104ZB8NNNC_100nF-104-80-20-50V_C1688.html" H 1450 1700 50  0001 C CNN "URL"
	1    1450 1700
	1    0    0    -1  
$EndComp
Connection ~ 1450 1600
Wire Wire Line
	1450 1600 1050 1600
$Comp
L power:GND #PWR056
U 1 1 5E61B44A
P 1800 1950
F 0 "#PWR056" H 1800 1700 50  0001 C CNN
F 1 "GND" H 1750 1800 50  0000 C CNN
F 2 "" H 1800 1950 50  0001 C CNN
F 3 "" H 1800 1950 50  0001 C CNN
	1    1800 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 1950 1800 1900
$Comp
L power:GND #PWR055
U 1 1 5E62822B
P 1450 1950
F 0 "#PWR055" H 1450 1700 50  0001 C CNN
F 1 "GND" H 1400 1800 50  0000 C CNN
F 2 "" H 1450 1950 50  0001 C CNN
F 3 "" H 1450 1950 50  0001 C CNN
	1    1450 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 1950 1450 1800
$Comp
L Device:C_Small C26
U 1 1 5E6431BC
P 3150 1300
F 0 "C26" H 3242 1346 50  0000 L CNN
F 1 "100n" H 3242 1255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 3150 1300 50  0001 C CNN
F 3 "~" H 3150 1300 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10F104ZB8NNNC_100nF-104-80-20-50V_C1688.html" H 3150 1300 50  0001 C CNN "URL"
	1    3150 1300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR061
U 1 1 5E6435A8
P 3150 1100
F 0 "#PWR061" H 3150 850 50  0001 C CNN
F 1 "GND" H 3100 950 50  0000 C CNN
F 2 "" H 3150 1100 50  0001 C CNN
F 3 "" H 3150 1100 50  0001 C CNN
	1    3150 1100
	-1   0    0    1   
$EndComp
Wire Wire Line
	3150 1100 3150 1200
Wire Wire Line
	3150 1400 3150 1500
Connection ~ 3150 1500
Wire Wire Line
	3150 1500 3300 1500
$Comp
L Switch:SW_Push SW1
U 1 1 5E66DA1F
P 2850 1250
F 0 "SW1" V 2750 1350 50  0000 L CNN
F 1 "SW_Push" V 2850 850 50  0000 L CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H4.3mm" H 2850 1450 50  0001 C CNN
F 3 "~" H 2850 1450 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Tactile-Switches_Korean-Hroparts-Elec-K2-1102DP-C4SW-04_C110153.html" H 2850 1250 50  0001 C CNN "URL"
	1    2850 1250
	0    1    1    0   
$EndComp
Wire Wire Line
	2850 1450 2850 1500
Connection ~ 2850 1500
Wire Wire Line
	2850 1500 3150 1500
$Comp
L power:GND #PWR059
U 1 1 5E67BCD0
P 2850 1000
F 0 "#PWR059" H 2850 750 50  0001 C CNN
F 1 "GND" H 2800 850 50  0000 C CNN
F 2 "" H 2850 1000 50  0001 C CNN
F 3 "" H 2850 1000 50  0001 C CNN
	1    2850 1000
	-1   0    0    1   
$EndComp
Wire Wire Line
	2850 1000 2850 1050
Wire Wire Line
	5600 6200 5300 6200
Text Label 5350 3000 0    50   ~ 0
SDI122
Text HLabel 5800 3050 2    50   Output ~ 0
SDI122
Wire Wire Line
	5700 3000 5700 3050
Wire Wire Line
	5700 3050 5800 3050
Text HLabel 6250 6450 2    50   Input ~ 0
SDI122
Entry Wire Line
	6050 6400 5950 6500
Wire Wire Line
	6050 6400 6200 6400
Wire Wire Line
	6200 6400 6200 6450
Wire Wire Line
	6200 6450 6250 6450
Text GLabel 5750 5500 2    50   Output ~ 0
PWR_CTL
Wire Wire Line
	8300 2650 8350 2650
Entry Wire Line
	8350 2650 8450 2750
Wire Wire Line
	8350 2550 8300 2550
Wire Bus Line
	8450 1800 8500 1800
Entry Wire Line
	8350 2550 8450 2650
Text Notes 1250 5700 0    50   ~ 0
USART_0 = TX\nUSART_1 = RX
Wire Bus Line
	7700 1800 7650 1800
Text HLabel 7700 1800 2    50   Output ~ 0
HS_POW[0..1]
Text HLabel 8500 1800 2    50   Output ~ 0
PWM[0..1]
Text Notes 7150 3800 0    50   ~ 0
I2C_0 = SCL\nI2C_1 = SDA
Text GLabel 5600 6200 2    50   Input ~ 0
I2C1_IT
Wire Wire Line
	6100 6100 6150 6100
Text GLabel 6300 5600 2    50   Input ~ 0
SPI2_IT
Wire Wire Line
	5300 5600 6300 5600
Entry Wire Line
	5900 4700 6000 4800
Text Notes 6900 4550 0    50   ~ 0
SPI_0 = SCK\nSPI_1 = MISO\nSPI_2 = MOSI
Text Notes 7150 6000 2    50   ~ 0
SDIO_0-3 =  0-3\nSDIO_CK = 4\nSDIO_CMD = 5
Text Notes 7500 3000 2    50   ~ 0
SWD0 = SWDIO\nSWD1 = SWCLK
Text GLabel 5550 2600 2    50   BiDi ~ 0
USB_D+
Wire Wire Line
	5550 2700 5300 2700
Wire Wire Line
	5300 2600 5550 2600
Text GLabel 5550 2700 2    50   BiDi ~ 0
USB_D-
$Comp
L Device:Battery_Cell BT1
U 1 1 5E48B000
P 1100 2800
F 0 "BT1" H 1218 2896 50  0000 L CNN
F 1 "Battery_Cell" H 1218 2805 50  0000 L CNN
F 2 "chinalandfoot:BT-CR2032" V 1100 2860 50  0001 C CNN
F 3 "~" V 1100 2860 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Battery-Holders-Clips-Contacts_Button-battery-holder-CR2032_C70377.html" H 1100 2800 50  0001 C CNN "URL"
	1    1100 2800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0129
U 1 1 5E48B315
P 1100 2950
F 0 "#PWR0129" H 1100 2700 50  0001 C CNN
F 1 "GND" H 1105 2777 50  0000 C CNN
F 2 "" H 1100 2950 50  0001 C CNN
F 3 "" H 1100 2950 50  0001 C CNN
	1    1100 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 2950 1100 2900
Text GLabel 1100 2500 1    50   Input ~ 0
VBAT
Wire Wire Line
	1100 2500 1100 2550
$Comp
L power:PWR_FLAG #FLG0108
U 1 1 5E5122D5
P 1000 2550
F 0 "#FLG0108" H 1000 2625 50  0001 C CNN
F 1 "PWR_FLAG" V 1000 2677 50  0000 L CNN
F 2 "" H 1000 2550 50  0001 C CNN
F 3 "~" H 1000 2550 50  0001 C CNN
	1    1000 2550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1000 2550 1100 2550
Connection ~ 1100 2550
Wire Wire Line
	1100 2550 1100 2600
Wire Wire Line
	2500 1800 2500 1850
Wire Wire Line
	2500 2100 2500 2150
Wire Wire Line
	3300 2100 2500 2100
$Comp
L Device:C_Small C24
U 1 1 5DD6A3FD
P 2500 1950
F 0 "C24" H 2592 1996 50  0000 L CNN
F 1 "20p" H 2592 1905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2500 1950 50  0001 C CNN
F 3 "~" H 2500 1950 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10C200JB8NNNC_20pF-200-5-50V_C1648.html" H 2500 1950 50  0001 C CNN "URL"
	1    2500 1950
	-1   0    0    1   
$EndComp
Wire Wire Line
	2500 2050 2500 2100
Connection ~ 2500 2100
Wire Wire Line
	2500 2550 2500 2500
Connection ~ 2500 2500
$Comp
L power:GND #PWR0130
U 1 1 5E168D3A
P 2250 2300
F 0 "#PWR0130" H 2250 2050 50  0001 C CNN
F 1 "GND" H 2255 2127 50  0000 C CNN
F 2 "" H 2250 2300 50  0001 C CNN
F 3 "" H 2250 2300 50  0001 C CNN
	1    2250 2300
	0    1    1    0   
$EndComp
Wire Wire Line
	2250 2300 2300 2300
$Comp
L power:GND #PWR0131
U 1 1 5E1765D4
P 2750 2300
F 0 "#PWR0131" H 2750 2050 50  0001 C CNN
F 1 "GND" H 2755 2127 50  0000 C CNN
F 2 "" H 2750 2300 50  0001 C CNN
F 3 "" H 2750 2300 50  0001 C CNN
	1    2750 2300
	0    -1   -1   0   
$EndComp
Wire Bus Line
	8450 1800 8450 2750
Wire Bus Line
	7650 1800 7650 2750
Wire Bus Line
	5950 6350 5950 6500
Wire Bus Line
	6400 4000 6400 4200
Wire Bus Line
	6000 4250 6000 4400
Wire Bus Line
	2750 5700 2750 5900
Wire Bus Line
	2750 5400 2750 5600
Wire Bus Line
	6000 3850 6000 4000
Wire Bus Line
	6050 2850 6050 3000
Wire Bus Line
	6050 2400 6050 2600
Wire Bus Line
	6050 2000 6050 2300
Wire Bus Line
	6000 3500 6000 3800
Wire Bus Line
	6000 5700 6000 6200
Wire Bus Line
	6000 4500 6000 4850
Wire Bus Line
	6200 4850 6200 5500
Wire Bus Line
	6750 1500 6750 3400
Wire Bus Line
	2050 4900 2050 6600
Wire Bus Line
	2550 3200 2550 4800
$EndSCHEMATC
