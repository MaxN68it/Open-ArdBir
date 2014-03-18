Open-ArdBir 2012-2014
===========

Version 2.6.54

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
 - Modified stage Wait in Active Pause (PID Controlo On)
 - Modified H e P indicator for ON-OFF and Active-Disactive Modulation
 - Added Some Control Parameter in Configuration Menu 
 - Added Temp Pump Stop in Manual Mode
 - Dual Scale Temp (°C-°F)
 - Reorganized EEPROM
 - Reorganized Configuration
 
 - Added Second Menu Configuration
   - Set Scale Temp (°C-°F)
   - Set Temp of Boil
   - Set Set Time Pump Cycle and Time Rest
   - Set Location of Temp Sensor
   - Set Pump ON-OFF at Boil 
   - Set Temp Pump Stop
   - Set Calibration Temp
 
 - Reorganized Automation
   - Removed Number of Stages Setting
   - Fixed Name of Stages (7)
   - Correct Mash In Stage
   - Auto Mash Design
   - Correct Time Reset of Pump
   - Added Iodine Test (Active Pause)
 
 - Stage Managing
   - Load Stage Set
   - Save Stage Set
   - Delete Stage Set
   - Initialize EEPROM
   
 - LCD 16x2 wiew
 - LCD 20x4 wiew
 - Italian Language
 - English Language
 
 compiled on Arduino V1.0.5
