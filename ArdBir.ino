char* Version = "2.6.53"; 

/*
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
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

//libraries
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <PID_v1.h>
//#include <SD.h>
//#include <Serial.h>

// SETTING PCB*****
// Select your PCB Version
//#include "Pcb_05.h"
//#include "Pcb_11s.h"
//#include "Pcb_14.h"
//#include "Pcb_Brauduino_DanielXan.h"
#include "Pcb_Brauduino_Original.h"

// Porzioni di codice
#include "ArdBir1.h"
#include "Funzioni.h"

// SETTING LCD*****
// Select your LCD

// LANGUAGE ITA
//#include "LCD16x2_ITA.h"
//#include "LCD20x4_ITA.h"

// LANGUAGE ENG
#include "LCD16x2_ENG.h"
//#include "LCD20x4_ENG.h"

// *****

#include "Recipe.h"

// global variables
unsigned int WindowSize;
unsigned long w_StartTime;
unsigned long start;

double Setpoint, Input, Output, eepromKp, eepromKi, eepromKd;

boolean Conv_start   = false;
boolean mpump        = false;
boolean mheat        = false;
boolean autoEnter    = false;
boolean tempReached  = false;
boolean pumpRest     = false;
boolean resume       = false;
boolean IodineTest   = false;

float mset_temp;
float stageTemp;
float boil_output;  // boil output %
float boilStageTemp;
float Temp_Now;


byte x;
byte ScaleTemp       = EEPROM.read(15);
byte SensorType      = EEPROM.read(16);
byte stageTime;
byte hopTime;
unsigned long TimeLeft, TimeSpent;

byte mainMenu        = 0;
byte pumpTime;
byte data[9]; // Sensor Temp with crc version
byte second;
byte Busy            = 0;
byte StageAddr;
byte blhpAddr;
byte hopAdd;

 //Setting 7 Stages
float p_C[]    ={  75.00,20.00,0.25,   55.00,25.00,0.25,   50.00,35.00,0.25,   60.00, 45.00,0.25,   70.00, 50.00,0.25,   76.00, 60.00,0.25,  80.00, 75.00,0.25 }; 
float p_F[]    ={ 167.00,68.00,0.25,  131.00,77.00,0.25,  122.00,95.00,0.25,  140.00,113.00,0.25,  158.00,122.00,0.25,  168.75,140.00,0.25, 176.00,167.00,0.25 }; 
int   p_PID[]  ={ 100, -100, 1,       100, -100, 1,       100, -100, 1,      7500, 1000, 250,      100, 0, 1,            50, -50, 1 }; 
byte  p_Unit[] ={   1, 0, 1,             1, 0, 1,         105, 90, 1,         221, 194, 1,          15, 5, 1,             5, 0, 1,            1, 0, 1,           0, 0, 0  }; 

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,100,20,5, DIRECT);


byte degC[8]         = {B01000, B10100, B01000, B00111, B01000, B01000, B01000, B00111};  // [8] degree c sybmol 
byte degF[8]         = {B01000, B10100, B01000, B00111, B00100, B00110, B00100, B00100};  // [1] degree f symbol
byte SP_Symbol[8]    = {B11100, B10000, B11100, B00111, B11101, B00111, B00100, B00100};  // [2] SP Symbol

byte PumpONOFF[8]    = {B00000, B01110, B01010, B01110, B01000, B01000, B01000, B00000};  // [3] Pump Symbol 
byte RevPumpONOFF[8] = {B11111, B10001, B10101, B10001, B10111, B10111, B10111, B11111};  // [4] Reverse PUMP Symbol


// ****************************************
// ******** start of  the funtions ******** 
// ****************************************




int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


void display_lcd (byte posX, byte posY, const char* lable){
  lcd.setCursor(posX, posY);
  lcd.print(lable);
}

void Gradi(){
  if (ScaleTemp==0)lcd.write(8);
  else lcd.write(1);
}

void Buzzer(byte NumBeep, int Period){
  for (byte i=0; i < NumBeep; i++){
    digitalWrite (Buzz, HIGH);
    delay (Period);
    digitalWrite(Buzz, LOW);
    delay(100);
  }
}

void save_settings (int addr, int data){
  EEPROM.write(addr,highByte(data));
  EEPROM.write((addr+1),lowByte(data));
/*
  Serial.print (F("WRITE--> "));
  Serial.print (F("Addr("));
  Serial.print (addr);
  Serial.print (F(")"));
  Serial.print (F(" Data word("));
  Serial.print (data); 
  Serial.println (F(")"));
*/
}  

void save_settings (int addr, byte data){
  EEPROM.write(addr,data);
/*
  Serial.print (F("WRITE--> "));
  Serial.print (F("Addr("));
  Serial.print (addr);
  Serial.print (F(")"));  
  Serial.print (F(" Data byte("));
  Serial.print (data);
  Serial.println (F(")"));
*/
}


void pauseStage(){
  boolean stage_pause = false;
  if (btn_Press(Button_start, 250)){
    Buzzer(3,100);
    stage_pause = true;
    heat_off(mheat);
    pump_off(mpump);

    while (stage_pause){
      Temperature();
      Pause_Stage(Temp_Now,TimeLeft);
      delay(100);
      if (btn_Press(Button_start,250))stage_pause=false;
    }
    Buzzer(3,100);
    Menu_2();
  } 
}


void Temperature(){// reads the DS18B20 temerature probe 
  ds.reset();
  ds.skip();
  // start conversion and return
  if (!(Conv_start)){
    ds.write(0x44,0);
    Conv_start = true;
    return;
  }
  if (Conv_start){// check for conversion if it isn't complete return if it is then convert to decimal
    Busy = ds.read_bit();
    if (Busy == 0)return;

    ds.reset();
    ds.skip();
    ds.write(0xBE);  
    for ( byte i = 0; i < 9; i++) {           // with crc we need 9 bytes
      data[i] = ds.read();
    } 
    /* add this routine for crc version */
    if ( OneWire::crc8(data, 8) != data[8]) {  //if checksum fails start a new conversion right away
      ds.reset();
      ds.skip();
      ds.write(0x44,0);
      Conv_start = true;
      return;
      /*Fine Routine crc*/
    }

    unsigned int raw = (data[1] << 8) + data[0];
    if (ScaleTemp==0)Temp_Now = (raw & 0xFFFC) * 0.0625;
    else Temp_Now = ((raw & 0xFFFC) * 0.0625) *1.8 + 32.0;

    byte Correzione = word(EEPROM.read(9),EEPROM.read(10));
    Temp_Now = Temp_Now + (Correzione/10.0);

    Conv_start = false;
    return;
  } 
}

// PID_HEAT modified to use SetMode(MANUAL)
void PID_HEAT (boolean autoMode){
  if (autoMode){
    if ((Setpoint - Input)<11)myPID.Compute();//was 6, getting close, start feeding the PID -mdw
    if((Setpoint - Input)>9)Output=100;    // was 5, ignore PID and go full speed -mdw  // set the output to full on
  }

  // PWM the output
  unsigned long now = millis();
  if(now - w_StartTime>WindowSize)w_StartTime += WindowSize; //time to shift the Relay Window
  if((Output*(WindowSize/100)) > now - w_StartTime)heat_on();
  else heat_off(mheat);
}

void load_pid_settings (){
  eepromKp = word(EEPROM.read(0),EEPROM.read(1));// read the PID settings from the EEPROM
  eepromKi = word(EEPROM.read(2),EEPROM.read(3));
  eepromKd = word(EEPROM.read(4),EEPROM.read(5));
  eepromKi = eepromKi/100;

  myPID.SetTunings(eepromKp,eepromKi,eepromKd); // send the PID settings to the PID
  WindowSize = word(EEPROM.read(6),EEPROM.read(7));
  myPID.SetOutputLimits(0, 100);
  myPID.SetSampleTime(5000);
}  

boolean wait_for_confirm (boolean& test, byte Stato){ 
  boolean wtBtn = true;
  
  while (wtBtn){               // wait for comfirmation 
    
    if(Stato==1){ //Pausa ATTIVA
      Temperature();
      Input=Temp_Now;
    
      Temp_Wait(Temp_Now);
  
      if(SensorType==1)heat_off(mheat);
      else PID_HEAT(true);
    }else{  //Pausa PASSIVA
      heat_off(mheat);
    }
        
    if (btn_Press(Button_start,50)){
      test = true;
      wtBtn = false;
    }
    if (btn_Press(Button_enter,50)){
      test = false;
      wtBtn = false;
      lcd.clear();
    }
  }
}

void quit_mode (boolean& processLoop){
  if ((digitalRead(Button_dn)==0) && (digitalRead(Button_up)==0)){
    delay(500);
    if ((digitalRead(Button_dn)==0) && (digitalRead(Button_up)==0)){
      mheat = false;
      mpump=false;
      heat_off(mheat);
      pump_off(mpump);
      processLoop = false;
      Clear_2_3();
    }
  }
}

void heat_on(){
  digitalWrite (Heat,HIGH);
  ledHeatON();
}

void heat_off(boolean mheat){
  digitalWrite (Heat,LOW);
  //if (mheat)ledHeatOFF();
  //else ledHeatStatus();  
  ledHeatStatus(mheat);
}


void heat_control(){
  //turns heat on or off      
  if (btn_Press(Button_start,50)){
    if (mheat==false){
      mheat = true;
      w_StartTime = millis();
    }else{
      mheat = false;
      heat_off(mheat);
    }
  }
}

void pump_on(){
  digitalWrite(Pump,HIGH);
  ledPumpON(); 
}

void pump_off(boolean mpump){
  digitalWrite(Pump,LOW);
  //ledPumpOFF();
  ledPumpStatus(mpump);
}

void pump_prime(){
  PumpPrime();
  
  for(byte i=1;i<6;i++){
    pump_on();
    delay(750+i*250);
    pump_off(mpump);
    delay(250);
  }Menu_2(); 
}


void pump_control(){//ON-OFF Manuale
  //turns the pump on or off
  if (btn_Press(Button_enter,50)){
    if (mpump == false){
      mpump = true;
      pump_on();
    }else{
      mpump = false;
      pump_off(mpump);
    }
  }
}

void pump_rest (byte stage){
  //byte SensorType = EEPROM.read(16);
  byte TimePumpCycle = EEPROM.read(19);
  byte TimePumpRest = EEPROM.read(20);
  byte setPumpBoil = EEPROM.read(21);
  byte TempPumpRest;
 
  //Condizioni per il ripristino a POMPA ON
  float DeltaTemp;//Stabilisce il Delta in base al sensore
 
  if (SensorType==0){
   
    //Sensore INTERNO
    DeltaTemp= (0.40 * TimePumpRest);
    if (ScaleTemp==1)DeltaTemp= (0.75 * TimePumpRest);    
  }else{
    
    //Sensore ESTERNO
    DeltaTemp= (1.25 * TimePumpRest);
    if (ScaleTemp==1)DeltaTemp= (2.25 * TimePumpRest);    
  }
 
  if (ScaleTemp==0)TempPumpRest = EEPROM.read(22);
  else TempPumpRest = EEPROM.read(23);

  
  if (Temp_Now >= TempPumpRest){
    if (SensorType==0){ 
      //Sensore Interno
      pump_off(mpump);
      //pumpRest=true;
    
    }else{
      // Modifica per sensore sulla pompa
    
      //Determina il Delta in cui tenere spento
      if (tempReached){
        if (Temp_Now < (boilStageTemp-(DeltaTemp*2))){
          pump_on();
          pumpRest=false;
        }else{ 
          pump_off(mpump);
          //pumpRest=true;
        }
      }else{
        pump_on();
        pumpRest=false;
      }
    }
  }else{
    if (stage==7){
      //Siamo in fase di BOIL
      if (setPumpBoil==0){ //POMPA OFF
        pump_off(mpump); // Turn OFF the pump in BOIL stage
        //pumpRest = true;
      }else{
        pump_on();
        pumpRest=false;
      }
    }else{//Durante le fasi 
      //Se non viene raggiunto il limite di tempo POMPA ON
      if ((pumpTime < TimePumpCycle)){ // starts pumps and heat
        pump_on();
        pumpRest = false; 
      }else{//Se non viene raggiunto il limite di tempo POMPA OFF
        pump_off(mpump);
        heat_off(mheat);
        pumpRest = true;

        //Viene raggiunta la fine della PAUSA POMPA
        //Viene raggiunta la soglia di abbassamento Temperatura
        //Si azzera il PumpTime per cominciare un altro ciclo
        if(pumpTime>=(TimePumpCycle + TimePumpRest) || ((stageTemp - Temp_Now) > DeltaTemp))pumpTime = 0;
      } 
    }
  }
}


void start_time (){
  start = millis();
  second = 0;
}

void hop_add (){
  byte nmbrHops = EEPROM.read(70);//read the number of hop additions
  
  if(hopAdd < nmbrHops){
    
    if (stageTime == hopTime){
      Buzzer(4,250);
      HopAdd(Temp_Now,hopAdd);
      CntDwn(TimeLeft);

      if (TimeLeft<6)Buzzer(1,150);

      delay(2000);
      Buzzer(1,750);
      hopAdd++;
      EEPROM.write(85,hopAdd);
      blhpAddr++;
      hopTime = EEPROM.read(blhpAddr);
    }
  } 
}



void stage_loop (byte stage){  
  byte lastminute;

  byte Verso=0;
  unsigned long Timer=0;

  boolean tempBoilReached=false;
  
  while ((stageTime>0)&&(autoEnter)){
    lastminute=stageTime;

//    stage_timing(stage);
    if ((millis()-start)>1000){  // timing routine
      start = millis();
      second++;

      if(!(tempReached))second=0;// starts counting down when temp reached
      else TimeLeft--;

      if (second>59){
        second = 0;
        pumpTime++;
        stageTime--;
        if(stage == 0)pumpTime = 0;
        EEPROM.write(84,lowByte(stageTime));// saves stage time incase of interuption
      }
    }
  
    Temperature();// get temp
    Setpoint = stageTemp;

    Input = Temp_Now;

    pauseStage();
      
    LeggiPulsante(Verso, Timer);

    //Stage(stage, stageTemp, Temp_Now, tempReached);
    
    if (pumpRest)PausaPompa(Temp_Now, TimeLeft);
    else{
      if (stage != 0   ) CntDwn(TimeLeft);
      if (TimeLeft < 6 ) Buzzer(1, 150);
      if (TimeLeft == 0) Buzzer(1, 1000); 

      if (stage ==6 && IodineTest==false)Iodine_Test(); 


//      if (stage==7 && tempReached && tempBoilReached && Temp_Now >= boilStageTemp) {  //if temp reached during boil
      if (stage==7 && tempBoilReached && Temp_Now >= boilStageTemp) {  //if temp reached during boil
      
        Set(boil_output,100,0,1,Timer,Verso);
        //change_up_dwn(boil_output,100,0,1);  // allow manual control of Output
        Output = boil_output;
        
        Boil(boil_output,Temp_Now,1);
        PID_HEAT(false); //set heat in manual mode
      }else{ 
        
        float Max, Min;
        if (ScaleTemp==0){
          Max=p_C[stage*3];
          Min=p_C[stage*3 + 1];
        }else{
          Max=p_F[stage*3];
          Min=p_F[stage*3 + 1];
        }
        
        if (stage==7){
          if (ScaleTemp==0)Set(stageTemp,110,EEPROM.read(17),0.25,Timer,Verso);
          else Set(stageTemp,230,EEPROM.read(18),0.25,Timer,Verso);
          NoBoil();
          tempBoilReached=false;
        }else{
          Set(stageTemp, Max, Min, 0.25, Timer, Verso);
          //if (ScaleTemp==0)Set(stageTemp,80,20,0.25,Timer,Verso);
          //else Set(stageTemp,176,68,0.25,Timer,Verso);
        }
        Stage(stage,stageTemp,Temp_Now);
        //Stage(stage, stageTemp, Temp_Now, tempReached);

        if (pumpRest==false)PID_HEAT(true);
      }  
    }
    if (Temp_Now >= stageTemp){
      if(stage==7 && tempBoilReached == false)tempBoilReached=true;
      if(tempReached == false){
        tempReached = true; 
      
        //***** Aggiunta per salto tenuta Mash In
        if (stage==0)stageTime=0;
        //*****

        if (stage == 7)hop_add();  // check for hop add once temp reached
      }
    }
    
    pump_rest(stage);
    if (stage==7){
      //check for minute tick
      if(stageTime<lastminute)hop_add();  //check to add hops
    }
    quit_mode (autoEnter);
  }
}

void add_malt (){
  boolean malt;
  pump_off(mpump);
  
  AddMalt();

  wait_for_confirm(malt,1);
  if (malt==false){
    //heat_off(mheat);
    //pump_off(mpump);
    LCD_Default(Temp_Now);
    delay(50);
    mainMenu=0;
    autoEnter = false;
  }
}

void Iodine_Test (){
  boolean Test = true;
  
  if (ScaleTemp==0)Setpoint = word(EEPROM.read(55),EEPROM.read(56));
  else Setpoint = word(EEPROM.read(57),EEPROM.read(58));
  Setpoint = Setpoint/16.0;

  pump_on();
  pumpTime=0;

  while (Test){

//    iodine_timing();
    if ((millis()-start)>1000){  // timing routine
      start = millis();
      second++;

      if(IodineTest)second=0;// starts counting down when temp reached
      TimeSpent++; // ***** Add for COUNTDOWN

      if (second>59){
        second = 0;
        pumpTime++;  
      }
    }  
        
    Temperature();
    Input = Temp_Now;

    if (pumpRest==false)PID_HEAT(true);
    pump_rest(5);

    Iodine(Temp_Now,TimeSpent);
    //delay(50);
    
    if (TimeSpent % 45 == 0)Buzzer(1,65);
    delay (135);
    
    quit_mode(Test);  
    if (btn_Press(Button_start,50)){
      Test = false;
      IodineTest = true;
    }
  }
  pump_on();
  pumpRest=false;
  pumpTime=0;
  Menu_2();
}

void remove_malt (){
  boolean malt;
  x = 7;               // used add to stage count on the final stage for the resume 
  EEPROM.write(83,lowByte(x)); // stores the stage number for the resume
  pump_off(mpump);
  //heat_off(mheat);

  //***** Reset della pompa al cambio step
  pumpRest = false;
  pumpTime = 0;

  RemoveMalt();

  wait_for_confirm(malt,1);
  if (malt==false){
    stageTime = EEPROM.read(71);
    EEPROM.write(84,lowByte(stageTime));
    //heat_off(mheat);
    //pump_off(mpump);
    LCD_Default(Temp_Now);
    delay(50);
    mainMenu=0;
    autoEnter = false;
  }
}


void manual_mode (){
  boolean manualLoop = false;

  byte Verso=0;
  unsigned long Timer =0;

  load_pid_settings();
  boil_output = EEPROM.read(8); // get boil heat %
  prompt_for_water();
  wait_for_confirm(manualLoop,0);

  Menu_1();//Pulisce lo schermo

  tempReached = false;
  TimeSpent = 0;
  
  float DeltaSetPoint;
  if (ScaleTemp==0)DeltaSetPoint = 2.0;
  else DeltaSetPoint = 3.6;
  
  while (manualLoop){            // manual loop
    Temperature();
    Setpoint = mset_temp;

    Input = Temp_Now;

    if(tempReached==false){
      if(Input>=Setpoint)tempReached = true;
    }else{
      if ((Input + DeltaSetPoint) < Setpoint && Verso==1){
        tempReached = false;
        TimeSpent = 0;
      }
    }
//  manual_timing();
    if ((millis()-start)>1000){  // timing routine
      start = millis();
      second++;
      if(!(tempReached))second=0;// starts counting down when temp reached
      else TimeSpent++; // ***** Add for COUNTDOWN
    }

    quit_mode(manualLoop);
    heat_control();
    pump_control();
    
    LeggiPulsante(Verso, Timer);
    
    Manuale(mset_temp,Temp_Now,boilStageTemp);
    //NoBoil();
    
    if (Setpoint >= boilStageTemp && Input >= Setpoint){ 
      Set(boil_output,100,0,1,Timer,Verso);
      //change_up_dwn(boil_output,100,0,1);  // allow manual control of Output
      Output = boil_output;

      Boil(boil_output,Temp_Now,0);
      
      if (mheat)PID_HEAT(false); //set heat in manual mode
      
      if(mpump)pump_rest(7); //Forced Boil Stage for Pump Control
      
    }else{
      if (ScaleTemp==0)Set(mset_temp,110,20,0.25,Timer,Verso);
      else Set(mset_temp,230,68,0.25, Timer, Verso);
  
      //Manuale(mset_temp,Temp_Now,boilStageTemp);

      if (mheat)PID_HEAT(true); //set heat in auto mode
      
      if(mpump)pump_rest(5); //Forced Boil Stage for Pump Control
      
    }CntDwn(TimeSpent);
  }lcd.clear();
}

void auto_mode (){
  StageAddr=30;
  load_pid_settings();

//  check_for_resume();
  if(EEPROM.read(82)){ // FLAG Automode Started
    Resume();

    wait_for_confirm(resume,0);
    if(resume==true){
      StageAddr = (EEPROM.read(83)*5)+30;
      x = EEPROM.read(83);
      autoEnter = true;
    }
  } 
    
  Menu_2();

  if(!(resume)){  // if starting a new process prompt for water
    prompt_for_water();
    wait_for_confirm(autoEnter,0);

    Menu_2();//pulisce lo schermo

    if(!(autoEnter))return;
    hopAdd =0; //reset hop count at teh start of the processes
    EEPROM.write(85,hopAdd); 
    pumpTime =0;
    pumpRest =false;
    pump_prime();
    x = 0;
  }
  if (autoEnter){     // mash steps
    EEPROM.write(82,1);// auto mode started
    for (byte i = x; i < 7; i++){
      EEPROM.write(83, lowByte(x)); // stores the stage number for the resume
      x++;                         // used to count the stages for the resume 
      tempReached = false;

      //***** Reset della pompa al cambio step
      pumpRest = false;
      pumpTime = 0;
      //*****

//      get_stage_settings();
      if (ScaleTemp==0)stageTemp = word(EEPROM.read(StageAddr),EEPROM.read(StageAddr+1));
      else stageTemp = word(EEPROM.read(StageAddr+2),EEPROM.read(StageAddr+3));
      stageTemp = stageTemp/16.0;

      if (resume){                 // on the start of resume gets saved time
        stageTime=EEPROM.read(84);
        resume = false;            // clears resume for next loop
      }else{
        stageTime = EEPROM.read(StageAddr+4); // gets stage time
        EEPROM.write(84,lowByte(stageTime));// saves the intial stage time
      } 
      TimeLeft= stageTime * 60;
      
      start_time(); 
      stage_loop(i);
      if (!(autoEnter)) break;
      if( i==0 && autoEnter){    // at the end of the mashIn step pauses to add grain
        add_malt();
        if (!(autoEnter))break;

        Menu_2();
      }
      if(i==(6)&& autoEnter){   // at the end of the last step pauses to remove the malt pipe before the boil
        remove_malt();
        if (!(autoEnter))break;

        Menu_2();
      }          
      StageAddr +=5; // increase stage addresses  
    }
  }

  // start of the boil
  if(autoEnter){

    start_time(); 
    
    stageTemp= boilStageTemp; // was set the intital boil temp to 98 deg c -mdw
    tempReached = false;  

//    get_boil_settings();
    byte nmbrHops = EEPROM.read(70);
    if (resume){
      if(x!=7)stageTime = EEPROM.read(71);
      else stageTime= EEPROM.read(84);
    }else{
      stageTime = EEPROM.read(71);
      EEPROM.write(84,lowByte(stageTime));
    }
    hopAdd = EEPROM.read(85);
    blhpAddr = hopAdd+72;
    hopTime = EEPROM.read(blhpAddr);
    boil_output = EEPROM.read(8);
    TimeLeft = stageTime * 60;
    
    stage_loop(7);
//    if (ScaleTemp==0)stage_loop(7,120,90);
//    else stage_loop(7,248,194);

    if(autoEnter){    // finishes the brewing process
      End();

      EEPROM.write(82,0); // sets auto start byte to 0 for resume
      EEPROM.write(85,0); // sets hop count to 0
      mainMenu=0;
      autoEnter =false;
      resume =false;
    }
  }lcd.clear();
}

void set_PID (){
//  int paramPID[] ={
//    100,-100,1, 100,-100,1, 100,-100,1, 7500,1000,250, 100,0,1, 50,-50,1   }; 

  byte a = 0;
  boolean pidLoop = false;
  int pidSet;
  byte boil_pcnt;

  byte Verso=0;
  unsigned long Timer=0;

  byte setAddr = 0;

  for(byte i = 0; i < 6; i++){
    if (i<4||i==5)pidSet=word(EEPROM.read(setAddr),EEPROM.read((setAddr+1)));
    else boil_pcnt=EEPROM.read(setAddr);
    pidLoop= true;

    while (pidLoop){
      Menu_3_1_x(i);
      
      LeggiPulsante(Verso, Timer);
      
      if(i==4){
        PidSet(boil_pcnt,i);
        Set(boil_pcnt, p_PID[a], p_PID[a+1], p_PID[a+2], Timer, Verso);
      }else{ 
        PidSet(pidSet,i);

        if(i<3)Set(pidSet, p_PID[a], p_PID[a+1], p_PID[a+2], Timer, Verso);
        else   Set(pidSet, p_PID[a], p_PID[a+1], p_PID[a+2], Timer, Verso);
      }
      quit_mode(pidLoop);
      if (!(pidLoop))i=6;

      if(btn_Press(Button_enter,50)){     
        if(i==4)save_settings(setAddr,boil_pcnt);
        else save_settings(setAddr,pidSet);
        pidLoop = false;
      }
   }  
   if (i<4 || i==5)setAddr+=2;
   else setAddr+=1;
   a+=3;
 }Clear_2_3();
}

void set_Unit (){
//  byte paramUnit[] ={
//    1,0,1, 1,0,1, 105,90,1, 221,194,1, 15,5,1, 5,0,1, 1,0,1, 0,0,0  }; 

  byte a = 0;

//  int unitSet;
  byte unitSet;

  boolean unitLoop = false;
  //boolean TipoSensore = false;

  byte Verso=0;
  unsigned long Timer=0;
  
  byte setAddr = 15;

  for(byte i=0;i<8;i++){
    if ((i==2 && ScaleTemp==1) || (i==3 && ScaleTemp==0))unitLoop=false;
    else unitLoop= true;    
    
    if(i!=7)unitSet=EEPROM.read(setAddr);
    else{
      if (ScaleTemp==0) unitSet=EEPROM.read(22);
      else unitSet=EEPROM.read(23);
    }
    
    while (unitLoop){
      Menu_3_2_x(i);  
      UnitSet(unitSet,i);
  
      LeggiPulsante(Verso, Timer);
      
      if(i!=7){
        Set(unitSet, p_Unit[a], p_Unit[a+1], p_Unit[a+2], Timer, Verso);
      }else{
        if(ScaleTemp==0){
          Set(unitSet,EEPROM.read(17), 80, 1, Timer, Verso);
        }else{
          Set(unitSet,EEPROM.read(18), 176, 1, Timer, Verso);
        }
      }

      quit_mode(unitLoop);
      if (!(unitLoop))i=8;

      if(btn_Press(Button_enter,50)){
        if(i<2 || i>3){
          save_settings(setAddr,lowByte(unitSet));

          if (i==0)ScaleTemp=unitSet;
          if (i==1)SensorType=unitSet;    
      
          if (i==5 && SensorType==1){ //Il SENSORE Ã¨ ESTERNO
            save_settings(21,lowByte(1));//La pompa deve essere OBBLIGATORIAMENTE ON
            //Il Pump Rest viene settato a 0
            save_settings(22,lowByte(105));
            save_settings(23,lowByte(221));
            unitLoop = false;
            i=8;
          }
                    
          if(i==7){
            if (ScaleTemp==0){// Â°C
              save_settings(22,lowByte(unitSet));
              save_settings(23,lowByte((int)((unitSet*1.8)+32)));      
            }else{// Â°F
              save_settings(22,lowByte((int)((unitSet-32)/1.8)));
              save_settings(23,lowByte(unitSet));  
            }  
            i==8;
          }
        }else{
          if (i==2){
            save_settings(setAddr, lowByte(unitSet));
            save_settings(setAddr+1, lowByte(int((unitSet * 1.8) + 32)));
            boilStageTemp = unitSet;
          }
          
          if(i==3){
            save_settings(setAddr-1 ,lowByte( int((unitSet - 32) / 1.8)));
            save_settings(setAddr, lowByte(unitSet));
            boilStageTemp = unitSet;
          }
        }
        unitLoop = false;
      }         
    }
    setAddr+=1;
    a+=3;
  }Clear_2_3(); 
}

void set_Stages (){ 
 /*
  //Setting 7 Stages
  float paramC[] ={
    80,20,0.25, 55,25,0.25, 50,35,0.25, 60,45,0.25, 70,50,0.25, 76,60,0.25, 80,75,0.25    }; 

  float paramF[] ={
    176,68,0.25, 131,77,0.25, 122,95,0.25, 140,113,0.25, 158,122,0.25, 168.75,140,0.25, 176,167,0.25    }; 
*/
  byte a = 0;

  boolean TempTimeLoop = false;
  boolean autoLoop = false;
  boolean Control = true;
  boolean FlagStart = false;
  float stagetempSet;
  int w_stagetempSet;
  byte stagetimeSet;

  float Min, Max,Step;
  float temp_stageTemp=0.0;
  float DeltaTemp;
  boolean MashInFLAG = true;
  float MashInTemp=0;
  
  byte Verso=0;
  unsigned long Timer=0;

  StageAddr =30;
  
  for (byte i=0; i<7; i++){ // loops for the number of stages
    TempTimeLoop = true;
    autoLoop = true;

    if(ScaleTemp==0){
      stagetempSet = word(EEPROM.read(StageAddr),EEPROM.read(StageAddr+1));
      if(i>0)temp_stageTemp=word(EEPROM.read(StageAddr-5),EEPROM.read(StageAddr-4))/16.0;
      Max=p_C[a];
      Min=p_C[a+1];
      Step=p_C[a+2];
      DeltaTemp=3.5;
    }else{ 
      stagetempSet = word(EEPROM.read(StageAddr+2),EEPROM.read(StageAddr+3));
      if(i>0)temp_stageTemp=word(EEPROM.read(StageAddr-3),EEPROM.read(StageAddr-2))/16.0;
      Max=p_F[a];
      Min=p_F[a+1];
      Step=p_F[a+2];
      DeltaTemp=6.3;    
    }
    if(!(MashInFLAG))DeltaTemp=0;

    stagetempSet = stagetempSet/16.0;
    if(i>0){      
      if(Max<temp_stageTemp-DeltaTemp){//La temepratura precedente Ã¨ sopra i limiti dello stage   
        //Salta lo Stage
        if(i!=5 && i!=6){ 
          stagetempSet = temp_stageTemp*16;
          w_stagetempSet =word(stagetempSet);

          if (ScaleTemp==0){
            // Salva il settaggio in °C
            save_settings(StageAddr, w_stagetempSet);

            // Salva il settaggio in °F
            ConvertiCtoF(stagetempSet); 
            w_stagetempSet = word(stagetempSet);
            save_settings(StageAddr+2,w_stagetempSet);
          }else{
            // Salva il settaggio in °F
            save_settings(StageAddr+2,w_stagetempSet);

            // Salva il settaggio in °C
            ConvertiFtoC(stagetempSet);
            w_stagetempSet = word(stagetempSet);
            save_settings(StageAddr,w_stagetempSet);
          }          
          save_settings(StageAddr+4, lowByte(0));
          autoLoop = false;
          TempTimeLoop = false;
          FlagStart = false;
        }
      }else{
        
        if(MashInFLAG){
          if(MashInTemp-DeltaTemp>=Min)Min=MashInTemp-DeltaTemp;
        }else{
          if(Min<temp_stageTemp)Min=temp_stageTemp;
        }
      } 
    }
    while (TempTimeLoop){  // loops for temp adjust
      Menu_3_3_x(i);
      StageSet(stagetempSet);

      quit_mode(TempTimeLoop);
      if (TempTimeLoop == false){
        Control=false;
        return;
      }

      if (btn_Press(Button_start,50))FlagStart = true;

      LeggiPulsante(Verso, Timer);

      Set(stagetempSet, Max, Min, Step, Timer, Verso);
      
      // Vengono obbligati i tempi di: Mash In - A-Amilasi - Mash Out
      if (i==0 || i==5 || i==6 )FlagStart=false;      

      if (FlagStart||(btn_Press(Button_enter,50))){
        if(i==0)MashInTemp=stagetempSet;
    
        stagetempSet = stagetempSet*16;
        w_stagetempSet =word(stagetempSet);

        if (ScaleTemp==0){
          // Salva il settaggio in Â°C
          save_settings(StageAddr, w_stagetempSet);

          // Salva il settaggio in Â°F
                //stagetempSet = stagetempSet/16;              // Recupero il valore   
          ConvertiCtoF(stagetempSet); 
                //Arrotonda025(stagetempSet);
                //stagetempSet = stagetempSet*16;              // Preparo il valore per la registrazione
        
          w_stagetempSet = word(stagetempSet);
          save_settings(StageAddr+2,w_stagetempSet);
        }else{
          // Salva il settaggio in Â°F
          save_settings(StageAddr+2,w_stagetempSet);

          // Salva il settaggio in Â°C
                //stagetempSet = stagetempSet/16;              // Recupero il valore
          ConvertiFtoC(stagetempSet);
                //Arrotonda025(stagetempSet);
                //stagetempSet = stagetempSet*16;             // Preparo il valore per la registrazione
          w_stagetempSet = word(stagetempSet);
          save_settings(StageAddr,w_stagetempSet);
        }          
        if (FlagStart){//      Viene memorizzato il tempo a 0
          save_settings(StageAddr+4, lowByte(0));
          autoLoop = false;
        }
        TempTimeLoop = false;
        FlagStart = false;
      }
    }

    if (autoLoop)TempTimeLoop = true;
    stagetimeSet = EEPROM.read(StageAddr+4);
    
    while (TempTimeLoop){ // loops to adjust time setting
      if (i!=0){
        TimeSet(stagetimeSet);

        quit_mode(TempTimeLoop);
        if (TempTimeLoop == false){
          Control=false;
          return;
        }

        LeggiPulsante(Verso, Timer);   
        Set(stagetimeSet, 140, 1, 1, Timer, Verso);
        
        if (i!=5 && i!=6 && btn_Press(Button_start,50)){    // Viene memorizzato il tempo a 0
          save_settings(StageAddr+4,lowByte(0));
          TempTimeLoop = false;
        }

        if (btn_Press(Button_enter,50)){
          if (stagetimeSet>0){
            save_settings(StageAddr+4,lowByte(stagetimeSet));
            if(MashInFLAG)MashInFLAG=false;
            TempTimeLoop = false;
          }
        }
      }
      else{ //Se si tratta del Mash In setta il tempo a 1 (fittizio)
        save_settings(StageAddr+4,lowByte(1));
        TempTimeLoop = false;
      }
    } 
    StageAddr+=5;
    a+=3;
  }
  if(Control)set_hops();
  Clear_2_3();
}  

void set_hops (){
  boolean hopLoop;
  byte hopSet;
  byte nmbrHops = EEPROM.read(70);

  byte TimeUp;

  byte Verso=0;
  unsigned long Timer=0; 

  blhpAddr = 71;

  hopLoop=true;

  while (hopLoop){
    Menu_3_3_8();
    NumHops(nmbrHops);
    
    LeggiPulsante(Verso,Timer);
    
    Set(nmbrHops, 10, 0, 1, Timer, Verso);
    
    //change_set(nmbrHops,10,0,1);
    quit_mode(hopLoop);
    if(hopLoop == false)return;

    if (btn_Press(Button_enter,50)){
      save_settings(70,nmbrHops);
      hopLoop = false;
    }
  }

  nmbrHops+=1;

  for(byte i =0;i<nmbrHops;i++){
    hopLoop = true;
    hopSet = EEPROM.read(blhpAddr);
    while (hopLoop){
      if (i==0){
        Menu_3_3_9();
        TimeHops(int(hopSet));
      }
      else{
        Menu_3_3_10(i);
        TimeHops(int(hopSet));
      }
     
      quit_mode(hopLoop);
      if(hopLoop == false)return;

      if(i==0)TimeUp=180;
      else{
       if(i==1)TimeUp=EEPROM.read(blhpAddr-1);
       else TimeUp=EEPROM.read(blhpAddr-1)-1;
      
      }

      LeggiPulsante(Verso, Timer);
      Set(hopSet, TimeUp, 0, 1, Timer, Verso);
      
      if (btn_Press(Button_enter,50)){
        save_settings(blhpAddr,hopSet);
        hopLoop = false;
      }
    }blhpAddr+= 1;
  }Clear_2_3();
}



void TestRam(){  
  Menu_4_1();
}


void setup_mode (){
  boolean setupLoop = true;
  byte setupMenu = 0;

  while (setupLoop){
    switch (setupMenu){ 

      case(0):
      Menu_3_1();
      if (btn_Press(Button_start,50))setupLoop=false;
      if (btn_Press(Button_dn,50))setupMenu = 1;
      if (btn_Press(Button_enter,50))set_PID();  
      break;

      case(1):
      Menu_3_2();
      if (btn_Press(Button_start,50))setupLoop=false;
      if (btn_Press(Button_up,50))setupMenu = 0;
      if (btn_Press(Button_dn,50))setupMenu = 2;
      if (btn_Press(Button_enter,50))set_Unit();  
      break;

      case(2):
      Menu_3_3();
      if (btn_Press(Button_start,50))setupLoop=false;
      if (btn_Press(Button_up,50))setupMenu = 1;
      if (btn_Press(Button_dn,50))setupMenu = 3;
      if (btn_Press(Button_enter,50))set_Stages();
      break;

      case(3):
      Menu_3_4();
      if (btn_Press(Button_start,50))setupLoop=false;
      if (btn_Press(Button_up,50))setupMenu = 2;
      if (btn_Press(Button_dn,50))setupMenu = 4;
      if (btn_Press(Button_enter,50))RecipeMenu();
      break;
      
      case(4):
      Menu_3_5();
      if (btn_Press(Button_start,50))setupLoop=false;
      if (btn_Press(Button_up,50))setupMenu = 3;
      if (btn_Press(Button_enter,50))Credits();
      break;
    }
  }lcd.clear();
}   

void setup(){
  // Start up the library
  Serial.begin(9600);

  // SETTING LCD*****
  lcd.begin(16,2);
//  lcd.begin(20,4);

  pinMode (Button_up, INPUT_PULLUP);
  pinMode (Button_dn, INPUT_PULLUP);
  pinMode (Button_start, INPUT_PULLUP);
  pinMode (Button_enter, INPUT_PULLUP);
  pinMode (Heat, OUTPUT);
  pinMode (Pump, OUTPUT);
  pinMode (Buzz, OUTPUT);
  w_StartTime = millis();

  //tell the PID to range between 0 and the full window size
  myPID.SetMode(AUTOMATIC);

  heat_off(mheat);
  pump_off(mpump);

  if (ScaleTemp==0){
    boilStageTemp = EEPROM.read(17);
    mset_temp = 35;
  }else{
    boilStageTemp = EEPROM.read(18);
    mset_temp = 95;
  }

//  Sprite screen
  ArdBir();

  // write custom symbol to LCD
  lcd.createChar(8, degC);         // Celsius
  lcd.createChar(1, degF);         // Faherenheit
  lcd.createChar(2, SP_Symbol);    // Set Point
  lcd.createChar(3, PumpONOFF);    // Pump
  lcd.createChar(4, RevPumpONOFF);    // Pump
  lcd.createChar(5, HeatONOFF);    // Resistor ON-OFF
  lcd.createChar(6, RevHeatONOFF); // Heat On-OFF
  //lcd.createChar(6, CopyRight );   // CopyRight Symbol
  lcd.createChar(7, Language);     // Language Symbol 
//  lcd.createChar(7, Italy);        // IT Symbol
//  lcd.createChar(7, English);      // EN Symbol
}

void loop(){
  pumpTime=0;
  TimeSpent=0;
  TimeLeft=0;
  IodineTest=false;

  switch(mainMenu){

    case (1):
    Menu_1();
    manual_mode();
    mainMenu = 0;
    break;

    case (2):
    Menu_2();
    auto_mode();
    mainMenu = 0;  
    break;

    case (3):   
    Menu_3();
    setup_mode();
    mainMenu = 0;    
    break;

    case (4):
    Menu_4();
    TestRam();
    mainMenu = 0;    
    break;

  default: 
    mheat=false;
    mpump=false;  
    heat_off(mheat);
    pump_off(mpump);
    Temperature();
    LCD_Default(Temp_Now);

    if (btn_Press(Button_dn,500))mainMenu = 1;
    if (btn_Press(Button_start,500))mainMenu = 2;
    if (btn_Press(Button_enter,500))mainMenu = 3;
    if (btn_Press(Button_up,2500))mainMenu = 4;
    break;    
  }
}
