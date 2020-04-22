EESchema Schematic File Version 4
LIBS:prockeckv2-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 10
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
L Chinaland:TLC227AIDR U?
U 4 1 5DF51701
P 3650 3450
AR Path="/5E24374F/5DF51701" Ref="U?"  Part="4" 
AR Path="/5DF4C831/5DF51701" Ref="U15"  Part="4" 
F 0 "U15" V 3678 3222 60  0000 R CNN
F 1 "TLC227AIDR" V 3572 3222 60  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 3750 3500 60  0001 C CNN
F 3 "" H 3750 3500 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 3650 3450 50  0001 C CNN "URL"
	4    3650 3450
	0    -1   -1   0   
$EndComp
$Comp
L Chinaland:TLC227AIDR U?
U 3 1 5DF51707
P 3050 4100
AR Path="/5E24374F/5DF51707" Ref="U?"  Part="3" 
AR Path="/5DF4C831/5DF51707" Ref="U15"  Part="3" 
F 0 "U15" V 3078 3872 60  0000 R CNN
F 1 "TLC227AIDR" V 2972 3872 60  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 3150 4150 60  0001 C CNN
F 3 "" H 3150 4150 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 3050 4100 50  0001 C CNN "URL"
	3    3050 4100
	0    -1   -1   0   
$EndComp
$Comp
L Chinaland:TLC227AIDR U?
U 2 1 5DF5170D
P 2250 3550
AR Path="/5E24374F/5DF5170D" Ref="U?"  Part="2" 
AR Path="/5DF4C831/5DF5170D" Ref="U15"  Part="2" 
F 0 "U15" V 2278 3322 60  0000 R CNN
F 1 "TLC227AIDR" V 2172 3322 60  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 2350 3600 60  0001 C CNN
F 3 "" H 2350 3600 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 2250 3550 50  0001 C CNN "URL"
	2    2250 3550
	0    -1   -1   0   
$EndComp
$Comp
L Chinaland:TLC227AIDR U15
U 1 1 5DF51713
P 1650 4100
F 0 "U15" V 1678 3872 60  0000 R CNN
F 1 "TLC227AIDR" V 1572 3872 60  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 1750 4150 60  0001 C CNN
F 3 "" H 1750 4150 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 1650 4100 50  0001 C CNN "URL"
	1    1650 4100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4750 5600 4750 5700
Wire Wire Line
	5350 5600 5350 5700
Connection ~ 5350 5600
Wire Wire Line
	5950 5600 5950 5700
Connection ~ 5950 5600
Wire Wire Line
	6300 5600 5950 5600
Wire Wire Line
	6300 5700 6300 5600
$Comp
L power:GND #PWR?
U 1 1 5DF5172B
P 6300 5700
AR Path="/5E24374F/5DF5172B" Ref="#PWR?"  Part="1" 
AR Path="/5DF4C831/5DF5172B" Ref="#PWR087"  Part="1" 
F 0 "#PWR087" H 6300 5450 50  0001 C CNN
F 1 "GND" H 6305 5527 50  0000 C CNN
F 2 "" H 6300 5700 50  0001 C CNN
F 3 "" H 6300 5700 50  0001 C CNN
	1    6300 5700
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x18 J11
U 1 1 5DF51731
P 5150 5900
F 0 "J11" V 5275 5846 50  0000 C CNN
F 1 "Screw_Terminal_01x18" V 5366 5846 50  0000 C CNN
F 2 "chinalandfoot:TerminalBlock_18_P5.08mm" H 5150 5900 50  0001 C CNN
F 3 "~" H 5150 5900 50  0001 C CNN
	1    5150 5900
	0    1    1    0   
$EndComp
Wire Wire Line
	1750 4500 1850 4500
Wire Wire Line
	1850 4500 1850 3750
Wire Wire Line
	1850 3750 1700 3750
Wire Wire Line
	2450 3200 2450 3950
Wire Wire Line
	2450 3950 2350 3950
Wire Wire Line
	2250 3200 2450 3200
Wire Wire Line
	3150 4500 3250 4500
Wire Wire Line
	3250 4500 3250 3750
Wire Wire Line
	3250 3750 3100 3750
Wire Wire Line
	3650 3100 3850 3100
$Comp
L Chinaland:TLC227AIDR U17
U 4 1 5DF77450
P 6400 3450
F 0 "U17" V 6428 3222 60  0000 R CNN
F 1 "TLC227AIDR" V 6322 3222 60  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 6500 3500 60  0001 C CNN
F 3 "" H 6500 3500 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 6400 3450 50  0001 C CNN "URL"
	4    6400 3450
	0    -1   -1   0   
$EndComp
$Comp
L Chinaland:TLC227AIDR U17
U 3 1 5DF77456
P 5800 4100
F 0 "U17" V 5828 3872 60  0000 R CNN
F 1 "TLC227AIDR" V 5722 3872 60  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 5900 4150 60  0001 C CNN
F 3 "" H 5900 4150 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 5800 4100 50  0001 C CNN "URL"
	3    5800 4100
	0    -1   -1   0   
$EndComp
$Comp
L Chinaland:TLC227AIDR U17
U 2 1 5DF7745C
P 5000 3450
F 0 "U17" V 5028 3222 60  0000 R CNN
F 1 "TLC227AIDR" V 4922 3222 60  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 5100 3500 60  0001 C CNN
F 3 "" H 5100 3500 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 5000 3450 50  0001 C CNN "URL"
	2    5000 3450
	0    -1   -1   0   
$EndComp
$Comp
L Chinaland:TLC227AIDR U17
U 1 1 5DF77462
P 4400 4100
F 0 "U17" V 4428 3872 60  0000 R CNN
F 1 "TLC227AIDR" V 4322 3872 60  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 4500 4150 60  0001 C CNN
F 3 "" H 4500 4150 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 4400 4100 50  0001 C CNN "URL"
	1    4400 4100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4500 4500 4600 4500
Wire Wire Line
	4600 4500 4600 3750
Wire Wire Line
	4600 3750 4450 3750
Wire Wire Line
	5200 3100 5200 3850
Wire Wire Line
	5200 3850 5100 3850
Wire Wire Line
	5000 3100 5200 3100
Wire Wire Line
	5900 4500 6000 4500
Wire Wire Line
	6000 4500 6000 3750
Wire Wire Line
	6000 3750 5900 3750
Wire Wire Line
	6400 3100 6600 3100
Wire Wire Line
	6600 3100 6600 3850
Wire Wire Line
	6600 3850 6500 3850
$Comp
L Chinaland:TLC227AIDR U?
U 4 1 5DF7907D
P 9000 3550
AR Path="/5E24374F/5DF7907D" Ref="U?"  Part="4" 
AR Path="/5DF4C831/5DF7907D" Ref="U18"  Part="4" 
F 0 "U18" V 9028 3322 60  0000 R CNN
F 1 "TLC227AIDR" V 8922 3322 60  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 9100 3600 60  0001 C CNN
F 3 "" H 9100 3600 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 9000 3550 50  0001 C CNN "URL"
	4    9000 3550
	0    -1   -1   0   
$EndComp
$Comp
L Chinaland:TLC227AIDR U?
U 3 1 5DF79083
P 8400 4200
AR Path="/5E24374F/5DF79083" Ref="U?"  Part="3" 
AR Path="/5DF4C831/5DF79083" Ref="U18"  Part="3" 
F 0 "U18" V 8428 3972 60  0000 R CNN
F 1 "TLC227AIDR" V 8322 3972 60  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 8500 4250 60  0001 C CNN
F 3 "" H 8500 4250 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 8400 4200 50  0001 C CNN "URL"
	3    8400 4200
	0    -1   -1   0   
$EndComp
$Comp
L Chinaland:TLC227AIDR U?
U 2 1 5DF79089
P 7600 3500
AR Path="/5E24374F/5DF79089" Ref="U?"  Part="2" 
AR Path="/5DF4C831/5DF79089" Ref="U18"  Part="2" 
F 0 "U18" V 7628 3272 60  0000 R CNN
F 1 "TLC227AIDR" V 7522 3272 60  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 7700 3550 60  0001 C CNN
F 3 "" H 7700 3550 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 7600 3500 50  0001 C CNN "URL"
	2    7600 3500
	0    -1   -1   0   
$EndComp
$Comp
L Chinaland:TLC227AIDR U?
U 1 1 5DF7908F
P 7000 4200
AR Path="/5E24374F/5DF7908F" Ref="U?"  Part="1" 
AR Path="/5DF4C831/5DF7908F" Ref="U18"  Part="1" 
F 0 "U18" V 7028 3972 60  0000 R CNN
F 1 "TLC227AIDR" V 6922 3972 60  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 7100 4250 60  0001 C CNN
F 3 "" H 7100 4250 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 7000 4200 50  0001 C CNN "URL"
	1    7000 4200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7100 4600 7200 4600
Wire Wire Line
	7200 4600 7200 3850
Wire Wire Line
	7200 3850 7100 3850
Wire Wire Line
	7800 3150 7800 3900
Wire Wire Line
	7800 3900 7700 3900
Wire Wire Line
	7600 3150 7800 3150
Wire Wire Line
	8500 4600 8600 4600
Wire Wire Line
	8600 4600 8600 3850
Wire Wire Line
	8600 3850 8550 3850
Wire Wire Line
	9000 3200 9200 3200
Wire Wire Line
	9200 3200 9200 3950
Wire Wire Line
	9200 3950 9100 3950
Wire Wire Line
	4250 5400 1550 5400
Wire Wire Line
	1550 5400 1550 4500
Wire Wire Line
	4250 5700 4250 5400
Wire Wire Line
	2150 3950 2150 5350
Wire Wire Line
	2150 5350 4350 5350
Wire Wire Line
	4350 5350 4350 5700
Wire Wire Line
	4550 5700 4550 5300
Wire Wire Line
	4550 5300 2950 5300
Wire Wire Line
	2950 5300 2950 4500
Wire Wire Line
	3550 5250 4650 5250
Wire Wire Line
	4650 5250 4650 5700
Wire Wire Line
	4850 5700 4850 5200
Wire Wire Line
	4850 5200 4300 5200
Wire Wire Line
	4300 5200 4300 4500
Wire Wire Line
	4900 3850 4900 5200
Wire Wire Line
	4900 5200 4950 5200
Wire Wire Line
	4950 5200 4950 5700
Wire Wire Line
	5150 5700 5150 5200
Wire Wire Line
	5150 5200 5700 5200
Wire Wire Line
	5700 5200 5700 4500
Wire Wire Line
	5250 5700 5250 5250
Wire Wire Line
	5250 5250 6300 5250
Wire Wire Line
	6900 4600 6900 5300
Wire Wire Line
	6900 5300 5450 5300
Wire Wire Line
	5450 5300 5450 5700
Wire Wire Line
	5550 5700 5550 5350
Wire Wire Line
	5550 5350 7500 5350
Wire Wire Line
	7500 5350 7500 3900
Wire Wire Line
	8300 4600 8300 5400
Wire Wire Line
	5750 5400 5750 5700
Wire Wire Line
	5850 5700 5850 5450
Wire Wire Line
	5850 5450 8900 5450
$Comp
L Chinaland:TLC227AIDR U15
U 5 1 5DF9A4C7
P 10500 3000
F 0 "U15" H 10678 3053 60  0000 L CNN
F 1 "TLC227AIDR" H 10678 2947 60  0000 L CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 10600 3050 60  0001 C CNN
F 3 "" H 10600 3050 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 10500 3000 50  0001 C CNN "URL"
	5    10500 3000
	1    0    0    -1  
$EndComp
$Comp
L Chinaland:TLC227AIDR U16
U 5 1 5DF9B82A
P 10500 3350
F 0 "U16" H 10678 3403 60  0000 L CNN
F 1 "TLC227AIDR" H 10678 3297 60  0000 L CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 10600 3400 60  0001 C CNN
F 3 "" H 10600 3400 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 10500 3350 50  0001 C CNN "URL"
	5    10500 3350
	1    0    0    -1  
$EndComp
$Comp
L Chinaland:TLC227AIDR U17
U 5 1 5DF9BF27
P 10500 3700
F 0 "U17" H 10678 3753 60  0000 L CNN
F 1 "TLC227AIDR" H 10678 3647 60  0000 L CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 10600 3750 60  0001 C CNN
F 3 "" H 10600 3750 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 10500 3700 50  0001 C CNN "URL"
	5    10500 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 4500 10050 4450
Wire Wire Line
	10050 3750 10150 3750
Wire Wire Line
	10050 3750 10050 3400
Wire Wire Line
	10050 3400 10150 3400
Connection ~ 10050 3750
Wire Wire Line
	10050 3400 10050 3050
Wire Wire Line
	10050 3050 10150 3050
Connection ~ 10050 3400
$Comp
L power:+5V #PWR092
U 1 1 5DFA3649
P 10000 2850
F 0 "#PWR092" H 10000 2700 50  0001 C CNN
F 1 "+5V" H 10015 3023 50  0000 C CNN
F 2 "" H 10000 2850 50  0001 C CNN
F 3 "" H 10000 2850 50  0001 C CNN
	1    10000 2850
	1    0    0    -1  
$EndComp
$Comp
L power:-5V #PWR093
U 1 1 5DFA3B1A
P 10050 4500
F 0 "#PWR093" H 10050 4600 50  0001 C CNN
F 1 "-5V" H 10065 4673 50  0000 C CNN
F 2 "" H 10050 4500 50  0001 C CNN
F 3 "" H 10050 4500 50  0001 C CNN
	1    10050 4500
	-1   0    0    1   
$EndComp
Wire Wire Line
	10150 3650 10000 3650
Wire Wire Line
	10000 3650 10000 3600
Wire Wire Line
	10150 3300 10000 3300
Connection ~ 10000 3300
Wire Wire Line
	10000 3300 10000 2950
Wire Wire Line
	10150 2950 10000 2950
Connection ~ 10000 2950
Wire Wire Line
	10000 2950 10000 2850
$Comp
L Chinaland:TLC227AIDR U16
U 1 1 5DFAE69D
P 1850 2250
F 0 "U16" V 1878 2022 60  0000 R CNN
F 1 "TLC227AIDR" V 1772 2022 60  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 1950 2300 60  0001 C CNN
F 3 "" H 1950 2300 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 1850 2250 50  0001 C CNN "URL"
	1    1850 2250
	0    -1   -1   0   
$EndComp
$Comp
L Chinaland:TLC227AIDR U?
U 2 1 5DFAEEA0
P 3250 2250
AR Path="/5E24374F/5DFAEEA0" Ref="U?"  Part="2" 
AR Path="/5DF4C831/5DFAEEA0" Ref="U16"  Part="2" 
F 0 "U16" V 3278 2022 60  0000 R CNN
F 1 "TLC227AIDR" V 3172 2022 60  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 3350 2300 60  0001 C CNN
F 3 "" H 3350 2300 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 3250 2250 50  0001 C CNN "URL"
	2    3250 2250
	0    -1   -1   0   
$EndComp
$Comp
L Chinaland:TLC227AIDR U?
U 3 1 5DFAFA34
P 4600 2250
AR Path="/5E24374F/5DFAFA34" Ref="U?"  Part="3" 
AR Path="/5DF4C831/5DFAFA34" Ref="U16"  Part="3" 
F 0 "U16" V 4628 2022 60  0000 R CNN
F 1 "TLC227AIDR" V 4522 2022 60  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 4700 2300 60  0001 C CNN
F 3 "" H 4700 2300 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 4600 2250 50  0001 C CNN "URL"
	3    4600 2250
	0    -1   -1   0   
$EndComp
$Comp
L Chinaland:TLC227AIDR U?
U 4 1 5DFB0A8F
P 6050 2250
AR Path="/5E24374F/5DFB0A8F" Ref="U?"  Part="4" 
AR Path="/5DF4C831/5DFB0A8F" Ref="U16"  Part="4" 
F 0 "U16" V 6078 2022 60  0000 R CNN
F 1 "TLC227AIDR" V 5972 2022 60  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 6150 2300 60  0001 C CNN
F 3 "" H 6150 2300 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 6050 2250 50  0001 C CNN "URL"
	4    6050 2250
	0    -1   -1   0   
$EndComp
$Comp
L Chinaland:TLC227AIDR U?
U 1 1 5DFB1F4E
P 7250 2300
AR Path="/5E24374F/5DFB1F4E" Ref="U?"  Part="1" 
AR Path="/5DF4C831/5DFB1F4E" Ref="U19"  Part="1" 
F 0 "U19" V 7278 2072 60  0000 R CNN
F 1 "TLC227AIDR" V 7172 2072 60  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 7350 2350 60  0001 C CNN
F 3 "" H 7350 2350 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 7250 2300 50  0001 C CNN "URL"
	1    7250 2300
	0    -1   -1   0   
$EndComp
$Comp
L Chinaland:TLC227AIDR U?
U 2 1 5DFB2942
P 8700 2300
AR Path="/5E24374F/5DFB2942" Ref="U?"  Part="2" 
AR Path="/5DF4C831/5DFB2942" Ref="U19"  Part="2" 
F 0 "U19" V 8728 2072 60  0000 R CNN
F 1 "TLC227AIDR" V 8622 2072 60  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 8800 2350 60  0001 C CNN
F 3 "" H 8800 2350 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 8700 2300 50  0001 C CNN "URL"
	2    8700 2300
	0    -1   -1   0   
$EndComp
$Comp
L Chinaland:TLC227AIDR U?
U 3 1 5DFB4292
P 10100 5350
AR Path="/5E24374F/5DFB4292" Ref="U?"  Part="3" 
AR Path="/5DF4C831/5DFB4292" Ref="U19"  Part="3" 
F 0 "U19" H 10075 5737 60  0000 C CNN
F 1 "TLC227AIDR" H 10075 5631 60  0000 C CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 10200 5400 60  0001 C CNN
F 3 "" H 10200 5400 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 10100 5350 50  0001 C CNN "URL"
	3    10100 5350
	1    0    0    -1  
$EndComp
$Comp
L Chinaland:TLC227AIDR U?
U 4 1 5DFB4829
P 10100 6050
AR Path="/5E24374F/5DFB4829" Ref="U?"  Part="4" 
AR Path="/5DF4C831/5DFB4829" Ref="U19"  Part="4" 
F 0 "U19" H 10075 6437 60  0000 C CNN
F 1 "TLC227AIDR" H 10075 6331 60  0000 C CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 10200 6100 60  0001 C CNN
F 3 "" H 10200 6100 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 10100 6050 50  0001 C CNN "URL"
	4    10100 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 5450 9700 5250
Wire Wire Line
	9700 5950 9700 6150
NoConn ~ 10450 5350
NoConn ~ 10450 6050
$Comp
L Device:R R31
U 1 1 5DFD0284
P 1700 2950
F 0 "R31" H 1770 2996 50  0000 L CNN
F 1 "200k" H 1770 2905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 1630 2950 50  0001 C CNN
F 3 "~" H 1700 2950 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html" H 1700 2950 50  0001 C CNN "URL"
	1    1700 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R30
U 1 1 5DFD0B03
P 1450 2700
F 0 "R30" H 1520 2746 50  0000 L CNN
F 1 "200k" H 1520 2655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 1380 2700 50  0001 C CNN
F 3 "~" H 1450 2700 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html" H 1450 2700 50  0001 C CNN "URL"
	1    1450 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R32
U 1 1 5DFD16F4
P 2000 2950
F 0 "R32" H 2070 2996 50  0000 L CNN
F 1 "200k" H 2070 2905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 1930 2950 50  0001 C CNN
F 3 "~" H 2000 2950 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html" H 2000 2950 50  0001 C CNN "URL"
	1    2000 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R33
U 1 1 5DFD4F22
P 2150 2550
F 0 "R33" H 2220 2596 50  0000 L CNN
F 1 "63k4" H 2220 2505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2080 2550 50  0001 C CNN
F 3 "~" H 2150 2550 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF6342T5E_C23093.html" H 2150 2550 50  0001 C CNN "URL"
	1    2150 2550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R35
U 1 1 5DFD52C9
P 2850 2700
F 0 "R35" H 2920 2746 50  0000 L CNN
F 1 "200k" H 2920 2655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2780 2700 50  0001 C CNN
F 3 "~" H 2850 2700 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html" H 2850 2700 50  0001 C CNN "URL"
	1    2850 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R36
U 1 1 5DFD58B0
P 3100 2900
F 0 "R36" H 3170 2946 50  0000 L CNN
F 1 "200k" H 3170 2855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3030 2900 50  0001 C CNN
F 3 "~" H 3100 2900 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html" H 3100 2900 50  0001 C CNN "URL"
	1    3100 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R37
U 1 1 5DFD6783
P 3400 2900
F 0 "R37" H 3470 2946 50  0000 L CNN
F 1 "200k" H 3470 2855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3330 2900 50  0001 C CNN
F 3 "~" H 3400 2900 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html" H 3400 2900 50  0001 C CNN "URL"
	1    3400 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R38
U 1 1 5DFDA1DA
P 3600 2550
F 0 "R38" H 3670 2596 50  0000 L CNN
F 1 "63k4" H 3670 2505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3530 2550 50  0001 C CNN
F 3 "~" H 3600 2550 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF6342T5E_C23093.html" H 3600 2550 50  0001 C CNN "URL"
	1    3600 2550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R40
U 1 1 5DFEBCD4
P 4150 2700
F 0 "R40" H 4220 2746 50  0000 L CNN
F 1 "200k" H 4220 2655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4080 2700 50  0001 C CNN
F 3 "~" H 4150 2700 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html" H 4150 2700 50  0001 C CNN "URL"
	1    4150 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R43
U 1 1 5DFEC0B1
P 4950 2550
F 0 "R43" H 5020 2596 50  0000 L CNN
F 1 "63k4" H 5020 2505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4880 2550 50  0001 C CNN
F 3 "~" H 4950 2550 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF6342T5E_C23093.html" H 4950 2550 50  0001 C CNN "URL"
	1    4950 2550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R41
U 1 1 5DFEF75C
P 4450 2850
F 0 "R41" H 4520 2896 50  0000 L CNN
F 1 "200k" H 4520 2805 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4380 2850 50  0001 C CNN
F 3 "~" H 4450 2850 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html" H 4450 2850 50  0001 C CNN "URL"
	1    4450 2850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R42
U 1 1 5DFEFB9C
P 4750 2850
F 0 "R42" H 4820 2896 50  0000 L CNN
F 1 "200k" H 4820 2805 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4680 2850 50  0001 C CNN
F 3 "~" H 4750 2850 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html" H 4750 2850 50  0001 C CNN "URL"
	1    4750 2850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R45
U 1 1 5DFF655C
P 5600 2700
F 0 "R45" H 5670 2746 50  0000 L CNN
F 1 "200k" H 5670 2655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 5530 2700 50  0001 C CNN
F 3 "~" H 5600 2700 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html" H 5600 2700 50  0001 C CNN "URL"
	1    5600 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R46
U 1 1 5DFFD98B
P 5900 2850
F 0 "R46" H 5970 2896 50  0000 L CNN
F 1 "200k" H 5970 2805 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 5830 2850 50  0001 C CNN
F 3 "~" H 5900 2850 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html" H 5900 2850 50  0001 C CNN "URL"
	1    5900 2850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R47
U 1 1 5DFFDD2F
P 6200 2850
F 0 "R47" H 6270 2896 50  0000 L CNN
F 1 "200k" H 6270 2805 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6130 2850 50  0001 C CNN
F 3 "~" H 6200 2850 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html" H 6200 2850 50  0001 C CNN "URL"
	1    6200 2850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R48
U 1 1 5DFFDFE5
P 6400 2550
F 0 "R48" H 6470 2596 50  0000 L CNN
F 1 "63k4" H 6470 2505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6330 2550 50  0001 C CNN
F 3 "~" H 6400 2550 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF6342T5E_C23093.html" H 6400 2550 50  0001 C CNN "URL"
	1    6400 2550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R50
U 1 1 5DFFED27
P 6850 2700
F 0 "R50" H 6920 2746 50  0000 L CNN
F 1 "200k" H 6920 2655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6780 2700 50  0001 C CNN
F 3 "~" H 6850 2700 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html" H 6850 2700 50  0001 C CNN "URL"
	1    6850 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R51
U 1 1 5DFFF081
P 7100 2900
F 0 "R51" H 7170 2946 50  0000 L CNN
F 1 "200k" H 7170 2855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7030 2900 50  0001 C CNN
F 3 "~" H 7100 2900 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html" H 7100 2900 50  0001 C CNN "URL"
	1    7100 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R52
U 1 1 5DFFF464
P 7400 2900
F 0 "R52" H 7470 2946 50  0000 L CNN
F 1 "200k" H 7470 2855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7330 2900 50  0001 C CNN
F 3 "~" H 7400 2900 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html" H 7400 2900 50  0001 C CNN "URL"
	1    7400 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R53
U 1 1 5DFFF77D
P 7600 2600
F 0 "R53" H 7670 2646 50  0000 L CNN
F 1 "63k4" H 7670 2555 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7530 2600 50  0001 C CNN
F 3 "~" H 7600 2600 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF6342T5E_C23093.html" H 7600 2600 50  0001 C CNN "URL"
	1    7600 2600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R55
U 1 1 5E0011AD
P 8200 2700
F 0 "R55" H 8270 2746 50  0000 L CNN
F 1 "200k" H 8270 2655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8130 2700 50  0001 C CNN
F 3 "~" H 8200 2700 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html" H 8200 2700 50  0001 C CNN "URL"
	1    8200 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R56
U 1 1 5E0014F3
P 8550 2900
F 0 "R56" H 8620 2946 50  0000 L CNN
F 1 "200k" H 8620 2855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8480 2900 50  0001 C CNN
F 3 "~" H 8550 2900 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html" H 8550 2900 50  0001 C CNN "URL"
	1    8550 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R57
U 1 1 5E0018D1
P 8850 2900
F 0 "R57" H 8920 2946 50  0000 L CNN
F 1 "200k" H 8920 2855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8780 2900 50  0001 C CNN
F 3 "~" H 8850 2900 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF2003T5E_C25811.html" H 8850 2900 50  0001 C CNN "URL"
	1    8850 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R58
U 1 1 5E0024EF
P 9050 2600
F 0 "R58" H 9120 2646 50  0000 L CNN
F 1 "63k4" H 9120 2555 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8980 2600 50  0001 C CNN
F 3 "~" H 9050 2600 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_UNI-ROYAL-Uniroyal-Elec-0603WAF6342T5E_C23093.html" H 9050 2600 50  0001 C CNN "URL"
	1    9050 2600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R29
U 1 1 5E07BA35
P 1450 2200
F 0 "R29" H 1520 2246 50  0000 L CNN
F 1 "100k" H 1520 2155 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 1380 2200 50  0001 C CNN
F 3 "~" H 1450 2200 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_100KR-1003-1_C14675.html" H 1450 2200 50  0001 C CNN "URL"
	1    1450 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R34
U 1 1 5E07BE87
P 2850 2250
F 0 "R34" H 2920 2296 50  0000 L CNN
F 1 "100k" H 2920 2205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2780 2250 50  0001 C CNN
F 3 "~" H 2850 2250 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_100KR-1003-1_C14675.html" H 2850 2250 50  0001 C CNN "URL"
	1    2850 2250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R39
U 1 1 5E07C29B
P 4150 2250
F 0 "R39" H 4220 2296 50  0000 L CNN
F 1 "100k" H 4220 2205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4080 2250 50  0001 C CNN
F 3 "~" H 4150 2250 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_100KR-1003-1_C14675.html" H 4150 2250 50  0001 C CNN "URL"
	1    4150 2250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R44
U 1 1 5E07F047
P 5600 2250
F 0 "R44" H 5670 2296 50  0000 L CNN
F 1 "100k" H 5670 2205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 5530 2250 50  0001 C CNN
F 3 "~" H 5600 2250 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_100KR-1003-1_C14675.html" H 5600 2250 50  0001 C CNN "URL"
	1    5600 2250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R49
U 1 1 5E07F750
P 6850 2250
F 0 "R49" H 6920 2296 50  0000 L CNN
F 1 "100k" H 6920 2205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6780 2250 50  0001 C CNN
F 3 "~" H 6850 2250 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_100KR-1003-1_C14675.html" H 6850 2250 50  0001 C CNN "URL"
	1    6850 2250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R54
U 1 1 5E07FE21
P 8200 2250
F 0 "R54" H 8270 2296 50  0000 L CNN
F 1 "100k" H 8270 2205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8130 2250 50  0001 C CNN
F 3 "~" H 8200 2250 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Chip-Resistor-Surface-Mount_100KR-1003-1_C14675.html" H 8200 2250 50  0001 C CNN "URL"
	1    8200 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 2550 1450 2500
Wire Wire Line
	1700 2800 1700 2650
Wire Wire Line
	1700 2650 1750 2650
Wire Wire Line
	1700 2650 1650 2650
Wire Wire Line
	1650 2650 1650 2500
Wire Wire Line
	1650 2500 1450 2500
Connection ~ 1700 2650
Connection ~ 1450 2500
Wire Wire Line
	1450 2500 1450 2350
Wire Wire Line
	1950 2800 2000 2800
Wire Wire Line
	2150 2700 1950 2700
Wire Wire Line
	1950 2650 1950 2700
Connection ~ 1950 2700
Wire Wire Line
	1950 2700 1950 2800
Wire Wire Line
	2150 2400 2050 2400
Wire Wire Line
	2050 2400 2050 1900
Wire Wire Line
	2050 1900 1850 1900
Wire Wire Line
	2250 3200 2000 3200
Wire Wire Line
	2000 3200 2000 3100
Connection ~ 2250 3200
Wire Wire Line
	1700 3100 1700 3750
Connection ~ 1700 3750
Wire Wire Line
	1700 3750 1650 3750
$Comp
L power:GND #PWR?
U 1 1 5E0B8BC8
P 1450 1950
AR Path="/5E24374F/5E0B8BC8" Ref="#PWR?"  Part="1" 
AR Path="/5DF4C831/5E0B8BC8" Ref="#PWR078"  Part="1" 
F 0 "#PWR078" H 1450 1700 50  0001 C CNN
F 1 "GND" H 1455 1777 50  0000 C CNN
F 2 "" H 1450 1950 50  0001 C CNN
F 3 "" H 1450 1950 50  0001 C CNN
	1    1450 1950
	-1   0    0    1   
$EndComp
Wire Wire Line
	1450 1950 1450 2050
$Comp
L power:+5V #PWR079
U 1 1 5E0BDDC3
P 1450 2950
F 0 "#PWR079" H 1450 2800 50  0001 C CNN
F 1 "+5V" H 1465 3123 50  0000 C CNN
F 2 "" H 1450 2950 50  0001 C CNN
F 3 "" H 1450 2950 50  0001 C CNN
	1    1450 2950
	-1   0    0    1   
$EndComp
Wire Wire Line
	1450 2950 1450 2850
Wire Wire Line
	3550 3850 3550 5250
Wire Wire Line
	3850 3100 3850 3850
Wire Wire Line
	3750 3850 3850 3850
Wire Wire Line
	3100 3050 3100 3750
Connection ~ 3100 3750
Wire Wire Line
	3100 3750 3050 3750
Wire Wire Line
	3400 3050 3650 3050
Wire Wire Line
	3650 3050 3650 3100
Connection ~ 3650 3100
Wire Wire Line
	3600 2700 3400 2700
Wire Wire Line
	3400 2700 3400 2750
Wire Wire Line
	3400 2700 3400 2650
Wire Wire Line
	3400 2650 3350 2650
Connection ~ 3400 2700
Wire Wire Line
	3600 2400 3450 2400
Wire Wire Line
	3450 2400 3450 1900
Wire Wire Line
	3450 1900 3250 1900
Wire Wire Line
	3100 2750 3100 2650
Wire Wire Line
	3100 2650 3150 2650
Wire Wire Line
	3100 2650 3100 2600
Wire Wire Line
	3100 2600 3000 2600
Wire Wire Line
	3000 2600 3000 2500
Wire Wire Line
	3000 2500 2850 2500
Wire Wire Line
	2850 2500 2850 2550
Connection ~ 3100 2650
Wire Wire Line
	2850 2500 2850 2400
Connection ~ 2850 2500
$Comp
L power:+5V #PWR081
U 1 1 5E1013F1
P 2850 2900
F 0 "#PWR081" H 2850 2750 50  0001 C CNN
F 1 "+5V" H 2865 3073 50  0000 C CNN
F 2 "" H 2850 2900 50  0001 C CNN
F 3 "" H 2850 2900 50  0001 C CNN
	1    2850 2900
	-1   0    0    1   
$EndComp
Wire Wire Line
	2850 2900 2850 2850
$Comp
L power:GND #PWR?
U 1 1 5E107557
P 2850 1950
AR Path="/5E24374F/5E107557" Ref="#PWR?"  Part="1" 
AR Path="/5DF4C831/5E107557" Ref="#PWR080"  Part="1" 
F 0 "#PWR080" H 2850 1700 50  0001 C CNN
F 1 "GND" H 2855 1777 50  0000 C CNN
F 2 "" H 2850 1950 50  0001 C CNN
F 3 "" H 2850 1950 50  0001 C CNN
	1    2850 1950
	-1   0    0    1   
$EndComp
Wire Wire Line
	2850 1950 2850 2100
Wire Wire Line
	4750 3000 5000 3000
Wire Wire Line
	5000 3000 5000 3100
Connection ~ 5000 3100
Wire Wire Line
	4750 2700 4700 2700
Wire Wire Line
	4700 2700 4700 2650
Wire Wire Line
	4950 2700 4750 2700
Connection ~ 4750 2700
Wire Wire Line
	4950 2400 4800 2400
Wire Wire Line
	4800 2400 4800 1900
Wire Wire Line
	4800 1900 4600 1900
Wire Wire Line
	4450 3000 4450 3750
Connection ~ 4450 3750
Wire Wire Line
	4450 3750 4400 3750
Wire Wire Line
	4450 2700 4500 2700
Wire Wire Line
	4500 2700 4500 2650
Wire Wire Line
	4450 2700 4450 2600
Wire Wire Line
	4450 2600 4350 2600
Wire Wire Line
	4350 2600 4350 2500
Wire Wire Line
	4350 2500 4150 2500
Wire Wire Line
	4150 2500 4150 2550
Connection ~ 4450 2700
Wire Wire Line
	4150 2500 4150 2400
Connection ~ 4150 2500
$Comp
L power:+5V #PWR083
U 1 1 5E18AFE3
P 4150 2900
F 0 "#PWR083" H 4150 2750 50  0001 C CNN
F 1 "+5V" H 4165 3073 50  0000 C CNN
F 2 "" H 4150 2900 50  0001 C CNN
F 3 "" H 4150 2900 50  0001 C CNN
	1    4150 2900
	-1   0    0    1   
$EndComp
Wire Wire Line
	4150 2900 4150 2850
$Comp
L power:GND #PWR?
U 1 1 5E193368
P 4150 2000
AR Path="/5E24374F/5E193368" Ref="#PWR?"  Part="1" 
AR Path="/5DF4C831/5E193368" Ref="#PWR082"  Part="1" 
F 0 "#PWR082" H 4150 1750 50  0001 C CNN
F 1 "GND" H 4155 1827 50  0000 C CNN
F 2 "" H 4150 2000 50  0001 C CNN
F 3 "" H 4150 2000 50  0001 C CNN
	1    4150 2000
	-1   0    0    1   
$EndComp
Wire Wire Line
	4150 2000 4150 2100
Wire Wire Line
	6300 5250 6300 3850
Wire Wire Line
	5750 5400 8300 5400
Wire Wire Line
	5900 3000 5900 3750
Connection ~ 5900 3750
Wire Wire Line
	5900 3750 5800 3750
Wire Wire Line
	6200 3000 6400 3000
Wire Wire Line
	6400 3000 6400 3100
Connection ~ 6400 3100
Wire Wire Line
	6200 2700 6150 2700
Wire Wire Line
	6150 2700 6150 2650
Wire Wire Line
	6400 2700 6200 2700
Connection ~ 6200 2700
Wire Wire Line
	6400 2400 6250 2400
Wire Wire Line
	6250 2400 6250 1900
Wire Wire Line
	6250 1900 6050 1900
Wire Wire Line
	5900 2700 5900 2650
Wire Wire Line
	5900 2650 5950 2650
Wire Wire Line
	5900 2650 5800 2650
Wire Wire Line
	5800 2650 5800 2500
Wire Wire Line
	5800 2500 5600 2500
Wire Wire Line
	5600 2500 5600 2550
Connection ~ 5900 2650
Wire Wire Line
	5600 2400 5600 2500
Connection ~ 5600 2500
$Comp
L power:+5V #PWR085
U 1 1 5E25B622
P 5600 2900
F 0 "#PWR085" H 5600 2750 50  0001 C CNN
F 1 "+5V" H 5615 3073 50  0000 C CNN
F 2 "" H 5600 2900 50  0001 C CNN
F 3 "" H 5600 2900 50  0001 C CNN
	1    5600 2900
	-1   0    0    1   
$EndComp
Wire Wire Line
	5600 2900 5600 2850
$Comp
L power:GND #PWR?
U 1 1 5E264B07
P 5600 2000
AR Path="/5E24374F/5E264B07" Ref="#PWR?"  Part="1" 
AR Path="/5DF4C831/5E264B07" Ref="#PWR084"  Part="1" 
F 0 "#PWR084" H 5600 1750 50  0001 C CNN
F 1 "GND" H 5605 1827 50  0000 C CNN
F 2 "" H 5600 2000 50  0001 C CNN
F 3 "" H 5600 2000 50  0001 C CNN
	1    5600 2000
	-1   0    0    1   
$EndComp
Wire Wire Line
	5600 2000 5600 2100
Wire Wire Line
	7100 2750 7150 2750
Wire Wire Line
	7150 2750 7150 2700
Wire Wire Line
	6850 2550 6850 2400
Wire Wire Line
	7150 2700 7050 2700
Wire Wire Line
	7050 2700 7050 2550
Wire Wire Line
	7050 2550 6850 2550
Connection ~ 7150 2700
Connection ~ 6850 2550
Connection ~ 7100 3850
Wire Wire Line
	7100 3850 7000 3850
Wire Wire Line
	7100 3050 7100 3850
Wire Wire Line
	7400 3050 7600 3050
Wire Wire Line
	7600 3050 7600 3150
Connection ~ 7600 3150
Wire Wire Line
	7400 2750 7350 2750
Wire Wire Line
	7350 2750 7350 2700
Wire Wire Line
	7600 2750 7400 2750
Connection ~ 7400 2750
Wire Wire Line
	7600 2450 7450 2450
Wire Wire Line
	7450 2450 7450 1950
Wire Wire Line
	7450 1950 7250 1950
$Comp
L power:+5V #PWR089
U 1 1 5E3002E5
P 6850 2900
F 0 "#PWR089" H 6850 2750 50  0001 C CNN
F 1 "+5V" H 6865 3073 50  0000 C CNN
F 2 "" H 6850 2900 50  0001 C CNN
F 3 "" H 6850 2900 50  0001 C CNN
	1    6850 2900
	-1   0    0    1   
$EndComp
Wire Wire Line
	6850 2900 6850 2850
$Comp
L power:GND #PWR?
U 1 1 5E30AE53
P 6850 1950
AR Path="/5E24374F/5E30AE53" Ref="#PWR?"  Part="1" 
AR Path="/5DF4C831/5E30AE53" Ref="#PWR088"  Part="1" 
F 0 "#PWR088" H 6850 1700 50  0001 C CNN
F 1 "GND" H 6855 1777 50  0000 C CNN
F 2 "" H 6850 1950 50  0001 C CNN
F 3 "" H 6850 1950 50  0001 C CNN
	1    6850 1950
	-1   0    0    1   
$EndComp
Wire Wire Line
	6850 1950 6850 2100
Wire Wire Line
	8900 5450 8900 3950
Wire Wire Line
	8550 2750 8600 2750
Wire Wire Line
	8600 2750 8600 2700
Wire Wire Line
	8550 2750 8500 2750
Wire Wire Line
	8500 2750 8500 2550
Wire Wire Line
	8500 2550 8200 2550
Connection ~ 8550 2750
Wire Wire Line
	8200 2550 8200 2400
Connection ~ 8200 2550
Wire Wire Line
	8550 3050 8550 3850
Connection ~ 8550 3850
Wire Wire Line
	8550 3850 8400 3850
Wire Wire Line
	8850 3050 9000 3050
Wire Wire Line
	9000 3050 9000 3200
Connection ~ 9000 3200
Wire Wire Line
	9050 2750 8850 2750
Wire Wire Line
	8800 2700 8850 2700
Wire Wire Line
	8850 2700 8850 2750
Connection ~ 8850 2750
Wire Wire Line
	9050 2450 8900 2450
Wire Wire Line
	8900 2450 8900 1950
Wire Wire Line
	8900 1950 8700 1950
$Comp
L power:+5V #PWR091
U 1 1 5E3BC215
P 8200 2900
F 0 "#PWR091" H 8200 2750 50  0001 C CNN
F 1 "+5V" H 8215 3073 50  0000 C CNN
F 2 "" H 8200 2900 50  0001 C CNN
F 3 "" H 8200 2900 50  0001 C CNN
	1    8200 2900
	-1   0    0    1   
$EndComp
Wire Wire Line
	8200 2900 8200 2850
$Comp
L power:GND #PWR?
U 1 1 5E3C8543
P 8200 1950
AR Path="/5E24374F/5E3C8543" Ref="#PWR?"  Part="1" 
AR Path="/5DF4C831/5E3C8543" Ref="#PWR090"  Part="1" 
F 0 "#PWR090" H 8200 1700 50  0001 C CNN
F 1 "GND" H 8205 1777 50  0000 C CNN
F 2 "" H 8200 1950 50  0001 C CNN
F 3 "" H 8200 1950 50  0001 C CNN
	1    8200 1950
	-1   0    0    1   
$EndComp
Wire Wire Line
	8200 1950 8200 2100
$Comp
L Chinaland:TLC227AIDR U18
U 5 1 5E424001
P 10500 4050
F 0 "U18" H 10678 4103 60  0000 L CNN
F 1 "TLC227AIDR" H 10678 3997 60  0000 L CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 10600 4100 60  0001 C CNN
F 3 "" H 10600 4100 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 10500 4050 50  0001 C CNN "URL"
	5    10500 4050
	1    0    0    -1  
$EndComp
$Comp
L Chinaland:TLC227AIDR U19
U 5 1 5E431184
P 10500 4400
F 0 "U19" H 10678 4453 60  0000 L CNN
F 1 "TLC227AIDR" H 10678 4347 60  0000 L CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 10600 4450 60  0001 C CNN
F 3 "" H 10600 4450 60  0001 C CNN
F 4 "https://lcsc.com/product-detail/Precision-OpAmps_Texas-Instruments-TLC2274AIDR_C93026.html" H 10500 4400 50  0001 C CNN "URL"
	5    10500 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	10000 4000 10150 4000
Wire Wire Line
	10000 3650 10000 4000
Connection ~ 10000 3650
Wire Wire Line
	10000 4000 10000 4350
Wire Wire Line
	10000 4350 10150 4350
Connection ~ 10000 4000
Wire Wire Line
	10150 4450 10050 4450
Connection ~ 10050 4450
Wire Wire Line
	10050 4450 10050 4300
Wire Wire Line
	10150 4100 10050 4100
Connection ~ 10050 4100
Wire Wire Line
	1850 1900 1850 1050
Connection ~ 1850 1900
Wire Wire Line
	3250 1050 3250 1900
Connection ~ 3250 1900
Wire Wire Line
	4600 1900 4600 1050
Connection ~ 4600 1900
Wire Wire Line
	6050 1900 6050 1050
Connection ~ 6050 1900
Wire Wire Line
	7250 1950 7250 1050
Connection ~ 7250 1950
Wire Wire Line
	8700 1950 8700 1050
Connection ~ 8700 1950
Entry Wire Line
	1750 950  1850 1050
Entry Wire Line
	3150 950  3250 1050
Entry Wire Line
	4500 950  4600 1050
Entry Wire Line
	5950 950  6050 1050
Entry Wire Line
	7150 950  7250 1050
Entry Wire Line
	8600 950  8700 1050
Text HLabel 10400 950  2    50   Output ~ 0
ADC_diff[0..5]
Text Label 1850 1100 3    50   ~ 0
ADC_diff0
Text Label 3250 1100 3    50   ~ 0
ADC_diff1
Text Label 4600 1100 3    50   ~ 0
ADC_diff2
Text Label 6050 1100 3    50   ~ 0
ADC_diff3
Text Label 7250 1100 3    50   ~ 0
ADC_diff4
Text Label 8700 1100 3    50   ~ 0
ADC_diff5
$Comp
L Device:C C34
U 1 1 5E52D18B
P 9800 4150
F 0 "C34" H 9850 4250 50  0000 L CNN
F 1 "100n" H 9600 4050 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 9838 4000 50  0001 C CNN
F 3 "~" H 9800 4150 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10F104ZB8NNNC_100nF-104-80-20-50V_C1688.html" H 9800 4150 50  0001 C CNN "URL"
	1    9800 4150
	1    0    0    -1  
$EndComp
$Comp
L Device:C C33
U 1 1 5E52F094
P 9800 3750
F 0 "C33" H 9850 3850 50  0000 L CNN
F 1 "100n" H 9600 3650 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 9838 3600 50  0001 C CNN
F 3 "~" H 9800 3750 50  0001 C CNN
F 4 "https://lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_SAMSUNG_CL10F104ZB8NNNC_100nF-104-80-20-50V_C1688.html" H 9800 3750 50  0001 C CNN "URL"
	1    9800 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9800 4300 10050 4300
Connection ~ 10050 4300
Wire Wire Line
	10050 4300 10050 4100
Wire Wire Line
	10000 4000 9800 4000
Wire Wire Line
	9800 3900 10050 3900
Wire Wire Line
	10050 3750 10050 3900
Connection ~ 10050 3900
Wire Wire Line
	10050 3900 10050 4100
Wire Wire Line
	9800 3600 10000 3600
Connection ~ 10000 3600
Wire Wire Line
	10000 3600 10000 3300
$Comp
L power:+12V #PWR086
U 1 1 5E09E244
P 6300 5550
F 0 "#PWR086" H 6300 5400 50  0001 C CNN
F 1 "+12V" V 6315 5678 50  0000 L CNN
F 2 "" H 6300 5550 50  0001 C CNN
F 3 "" H 6300 5550 50  0001 C CNN
	1    6300 5550
	0    1    1    0   
$EndComp
Wire Wire Line
	4750 5600 5350 5600
Wire Wire Line
	5350 5600 5950 5600
Wire Wire Line
	6300 5550 5650 5550
Wire Wire Line
	5650 5550 5650 5700
Wire Wire Line
	5650 5550 5050 5550
Wire Wire Line
	5050 5550 5050 5700
Connection ~ 5650 5550
Wire Wire Line
	5050 5550 4450 5550
Wire Wire Line
	4450 5550 4450 5700
Connection ~ 5050 5550
Wire Bus Line
	1600 950  10400 950 
$EndSCHEMATC