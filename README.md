Open ArdBir 2012-2014
===========

Version 2.8.3RC8


Source, PCB, Manual (English, Italian, Russian): 


brauduino semi automated single vessel RIMS
created by s.mathison
Copyright (C) 2012  Stephen Mathison

 Modified by Mike Wilson
 - Added INPUT_PULLUP to button inputs
 - swapped I/O ports for Buzz and Pump
 - changed to faherenheit
 - added heat and pump (H and P) indicators to the LCD
 - turned off pump during boil stage
 - added direct output% control and display during boil
 - saved boil heat % in EEPROM
 - 2v5 fixed hop addition when hoptime == boiltime
 - changed boil stage temp to 201 deg F.

 Modified by Massimo Nevi (2014)
 - Added Buzzer Modulation (NumBeep, Period)
 - Added Stage CountDown in Automatic Mode
 - Added Stage Watch in Manual Mode
 - Modified Max Number of Hops (10)
 - Modified stage Wait in Active Pause (PID Control On)
 - Modified H e P indicator for ON-OFF and Active-Disactive Modulation
 - Added Some Control Parameter in Configuration Menu 
 - Added Temp Pump Stop in Manual Mode
 - Dual Scale Temp (°C-°F)
 - Reorganized EEPROM
 - Reorganized Configuration
 - Added Delayed Start
 - Added Alert before Mash In
 - Jump Step whit Button Press in Automatic Mode
 - Added Cooling Procedure
 - Added Whirlpool Procedure
 
 Added more configuration on PID Menu
 - Added Use Gas section
 - Set Histeresy for Gas Use
 - Set Calibration Temp
 - Modified PID on BOIL
 
 - Added Second Menu Configuration
   - Set Scale Temp (°C-°F)
   - Set Location of Temp Sensor
   - Set Temp of Boil (°C-°F)
   - Set Set Time Pump Cycle and Time Rest
   - Set Pump ON-OFF before Mash In
   - Set Pump ON-OFF during Mash
   - Set Pump ON-OFF at Mash Out
   - Set Pump ON-OFF at Boil 
   - Set Temp Pump Stop (°C-°F)
   - Set Skip Pause at Add Malt
   - Set Skip Pause at Remove Malt
   - Set Skip Pause at Iodine Test
   - Set Time Max Iodine Test 
   - Set Manage Whirlpool
   
 - Reorganized Automation
   - Removed Number of Stages Setting
   - Fixed Name of Stages (8)
   - Dual A-Amylase Stage
   - Correct Mash In Stage
   - Auto Mash Design
   - Correct Time Reset of Pump
   - Added Iodine Test (Active Pause)
   - Added WHIRLPOOL control
   - Added COOLING control
   
 - Stage Managing
   - Load Stage Set
   - Save Stage Set
   - Delete Stage Set
   - Initialize EEPROM

 - LCD 16x2 wiew
 - LCD 20x4 wiew
 - Italian Language     (Both)
 - English Language     (Both)
 - Spanish Language     (Both)
 - Portuguese Language  (Both)
 - Russian Language     (20x4)
 - Norwegian Language   (20x4)
 - Romanian Language    (Both)
 
 compiled on Arduino V1.6.x

EEPROM MAP
  PID MENU
      0       Use Gas
      1       kP
      2       kI
      3       kD
      4       SampleTime
      5       WindowSize
      6       Boil Heat %
      7       Offset
      8       Hysteresi 
      9       [ SPACE ]

  UNIT MENU  
     10       Scale Temp
     11       Sensor Type
     12       Temp Boil °C
     13       Temp Boil °F
     14       Time Pump Cycle
     15       Time Pump Rest
     16       Pump PreMash
     17       Pump on Mash
     18       Pump on MashOut
     19       Pump on Boil
     20       Temp Pump Rest °C
     21       Temp Pump Rest °F
     22       PID Pipe
     23       Skip Add Malt
     24       Skip Remove Malt
     25       Skip Iodine Test
     26       Iodine Time
     27       Whirlpool
     28 -  31 [ SPACE ]

  RUN  (HTemp °C - LTemp °C - HTemp °F - LTemp °F - Time)
    32 -  36 MashIn
    37 -  41 Fitasi
    42 -  46 Glucanasi
    47 -  51 Proteasi
    52 -  55 B-Amilasi
    57 -  61 A-Amilasi 1
    62 -  66 A-Amilasi 2
    67 -  71 Mash Out

    72       Numbers of Hops
    73       Boil Time 
    74       Time Hop  1
    75       Time Hop  2
    76       Time Hop  3
    77       Time Hop  4
    78       Time Hop  5
    79       Time Hop  6
    80       Time Hop  7
    81       Time Hop  8
    82       Time Hop  9
    83       Time Hop 10

    84       FLAG Automode Started

  RESUME
    85       HANDLE Stage
    86       HANDLE Time Rest
    87       Hop Add

    88 -  89 [ SPACE ]

  RECIPE
    90 -  99 Index 1-10
   100 - 151 Recipe Data  1

   152 - 203 Recipe Data  2
   204 - 255 Recipe Data  3
   256 - 307 Recipe Data  4
   308 - 359 Recipe Data  5
   360 - 411 Recipe Data  6
   412 - 463 Recipe Data  7
   464 - 515 Recipe Data  8
   516 - 567 Recipe Data  9
   568 - 619 Recipe Data 10

   620 - 629 Recipe Name  1
   630 - 639 Recipe Name  2
   640 - 649 Recipe Name  3
   650 - 659 Recipe Name  4
   660 - 669 Recipe Name  5
   670 - 679 Recipe Name  6
   680 - 689 Recipe Name  7
   690 - 699 Recipe Name  8
   700 - 709 Recipe Name  9
   710 - 719 Recipe Name 10