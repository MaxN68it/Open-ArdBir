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
 - Added Delayed Start
 - Added Alert before Mash In
 - Jump Step whit Button Press in Automatic Mode
 
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
   - Fixed Name of Stages (8)
   - Dual A-Amylase Stage
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
 - Spanish Language
 
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

// Testing PCB
//#include "Pcb_05.h"
//#include "Pcb_11s.h"
//#include "Pcb_14.h"

// Supported PCB
//#include "Pcb_ArdBir_DanielXan.h"
//#include "Pcb_Brauduino_DanielXan.h"
#include "Pcb_Brauduino_Original.h"

// Porzioni di codice
#include "ArdBir1.h"
#include "Funzioni.h"

// SETTING LCD*****
// Select your LCD

// LANGUAGE ITA
//#include "LCD16x2_ITA.h"
#include "LCD20x4_ITA.h"

// LANGUAGE ENG
//#include "LCD16x2_ENG.h"
//#include "LCD20x4_ENG.h"

// LANGUAGE ESP
//#include "LCD16x2_ESP.h"
//#include "LCD20x4_ESP.h"

// *****

//#include "Recipe.h"

// global variables
unsigned long TimeLeft;
unsigned long TimeSpent;
unsigned long w_StartTime;
unsigned long start;

unsigned int WindowSize;

double Setpoint;
double Input;
double Output;
double eepromKp;
double eepromKi;
double eepromKd;

boolean Conv_start   = false;
boolean mpump        = false;
boolean mheat        = false;
boolean b_Enter      = false;
boolean tempReached  = false;
boolean pumpRest     = false;
boolean resume       = false;
boolean IodineTest  = false;
//boolean IodineTest1  = false;
//boolean IodineTest2  = false;
boolean DelayedMode  = false;


float mset_temp;
float stageTemp;
//float boil_output;  // boil output %
float boilStageTemp;
float Temp_Now;


byte x;
byte ScaleTemp       = EEPROM.read(15);
byte SensorType      = EEPROM.read(16);
int  stageTime;
byte hopTime;
byte mainMenu        = 0;
byte pumpTime           ;
byte data[9]            ; // Sensor Temp with crc version
byte second             ;
byte Busy            = 0;
byte StageAddr          ;
byte blhpAddr           ;
byte hopAdd             ;
byte nmbrHops           ;
byte boil_output        ;  // boil output %

 //Setting 8 Stages
float p_C[]    ={  75.00,20.00,0.25,   55.00,25.00,0.25,   50.00,35.00,0.25,   60.00, 45.00,0.25,   70.00, 50.00,0.25,   76.00, 60.00,0.25,  76.00, 60.00,0.25,  80.00, 75.00,0.25 }; 
float p_F[]    ={ 167.00,68.00,0.25,  131.00,77.00,0.25,  122.00,95.00,0.25,  140.00,113.00,0.25,  158.00,122.00,0.25,  168.75,140.00,0.25, 176.00,167.00,0.25, 176.00,167.00,0.25 }; 

int   p_PID[]  ={ 100, -100, 1,       100, -100, 1,       100, -100, 1,      7500, 1000, 250,      100, 0, 1,            50, -50, 1 }; 
byte  p_Unit[] ={   1, 0, 1,             1, 0, 1,         105, 90, 1,         221, 194, 1,          15, 5, 1,             5, 0, 1,            1, 0, 1,           0, 0, 0  }; 

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, 100, 20, 5, DIRECT);


//byte degC[8]         = {B01000, B10100, B01000, B00111, B01000, B01000, B01000, B00111};  // [8] degree c sybmol 
//byte degF[8]         = {B01000, B10100, B01000, B00111, B00100, B00110, B00100, B00100};  // [1] degree f symbol
byte SP_Symbol[8]    = {B11100, B10000, B11100, B00111, B11101, B00111, B00100, B00100};  // [2] SP Symbol

byte PumpONOFF[8]    = {B00000, B01110, B01010, B01110, B01000, B01000, B01000, B00000};  // [3] Pump Symbol 
byte RevPumpONOFF[8] = {B11111, B10001, B10101, B10001, B10111, B10111, B10111, B11111};  // [4] Reverse PUMP Symbol


// ****************************************
// ******** start of  the funtions ******** 
// ****************************************



/*
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
*/

/*
void display_lcd (byte posX, byte posY, const char* lable){
  lcd.setCursor(posX, posY);
  lcd.print(lable);
}
*/

void Gradi(){
  //if (ScaleTemp==0)lcd.write((byte)0);
  //else lcd.write(1);
  
  if (ScaleTemp==0){
    byte degC[8] = {B01000, B10100, B01000, B00111, B01000, B01000, B01000, B00111};  // [1] degree c sybmol 
    lcd.createChar(0, degC);// Celsius  
  }else{
    byte degF[8] = {B01000, B10100, B01000, B00111, B00100, B00110, B00100, B00100};  // [1] degree f symbol
    lcd.createChar(0, degF);// Faherenheit
  }
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
      delay(75);
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
    else Temp_Now = ((raw & 0xFFFC) * 0.0625) * 1.8 + 32.0;

    byte Correzione = word(EEPROM.read(9),EEPROM.read(10));
    Temp_Now = Temp_Now + (Correzione/10.0);

    Conv_start = false;
    return;
  } 
}

// PID_HEAT modified to use SetMode(MANUAL)
void PID_HEAT (boolean autoMode){
  //autoMode = TRUE PID Control
  //autoMode = TRUE PWM Control
  /*
  if (autoMode){
    if ((Setpoint - Input)<11)myPID.Compute();//was 6, getting close, start feeding the PID -mdw
    if((Setpoint - Input)>9)Output=100;    // was 5, ignore PID and go full speed -mdw  // set the output to full on
  }
  */
  if (autoMode){
    float DeltaPID=5.0;
    if(ScaleTemp==1)DeltaPID=9.0;
    
    if ((Setpoint - Input) < DeltaPID*1.2) myPID.Compute();//was 6, getting close, start feeding the PID -mdw
    if ((Setpoint - Input) > DeltaPID)     Output=100;    // was 5, ignore PID and go full speed -mdw  // set the output to full on
  }


  // PWM the output
  unsigned long now = millis();
  if(now - w_StartTime>WindowSize)w_StartTime += WindowSize; //time to shift the Relay Window
  if((Output*(WindowSize/100)) > now - w_StartTime)heat_on();
  else heat_off(mheat);
}

void load_pid_settings (){
  read_set(eepromKp,0);
  read_set(eepromKi,2);
  read_set(eepromKd,4);
  
  eepromKi = eepromKi/100;

  myPID.SetTunings(eepromKp,eepromKi,eepromKd); // send the PID settings to the PID

  read_set(WindowSize,6);
  myPID.SetOutputLimits(0.0, 100.0);
  myPID.SetSampleTime(3500);
}  

boolean wait_for_confirm (boolean& test, byte Stato, byte Tipo, byte Display){ 
  // Stato   == 1 Pause with PID
  // Stato   == 2 Pause without PID
  
  // Tipo    == 1 YES
  // Tipo    == 2 YES NO
  
  // Display == 1 Yes
  // Display == 2 No
  
  boolean wtBtn = true;
  
  byte Attesa = 0;
  
  while (wtBtn){               // wait for comfirmation 
    
    Temperature();
    Input=Temp_Now;
    if (Display==1)Temp_Wait(Temp_Now);

    if(Stato==1){                             //Pausa ATTIVA
      if(SensorType==1)heat_off(mheat);
      else PID_HEAT(true);
    }else{                                    //Pausa PASSIVA
      heat_off(mheat);
    }
        
    if (btn_Press(Button_start,50)){
      test = true;
      wtBtn = false;
    }
    
    if (Tipo==2){
      if (btn_Press(Button_enter,50)){
        test = false;
        wtBtn = false;
        lcd.clear();
      }
    }else{
      if (Attesa==255){
        Buzzer(2,25);
        Attesa=0;
      }
    }
    
    Attesa++;  
  }
}

void quit_mode (boolean& processLoop){
  if ((digitalRead(Button_dn)==0) && (digitalRead(Button_up)==0)){
    delay(350);
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
  ledPumpStatus(mpump);
}

void pump_prime(){
  PumpPrime();
  
  for(byte i=1;i<6;i++){
    pump_on();
    delay(750+i*250);
    pump_off(mpump);
    delay(350);
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
  byte TimePumpCycle = EEPROM.read(19);
  byte TimePumpRest = EEPROM.read(20);
  byte setPumpBoil = EEPROM.read(21);
  byte TempPumpRest;
  
  //Condizioni per il ripristino a POMPA ON
  float DeltaTemp;//Stabilisce il Delta in base al sensore
 
  if (SensorType==0){
   
    //Sensore INTERNO
    DeltaTemp= (0.45 * TimePumpRest);
    if (ScaleTemp==1)DeltaTemp= (0.80 * TimePumpRest);    
  }else{
    
    //Sensore ESTERNO
    DeltaTemp= (1.30 * TimePumpRest);
    if (ScaleTemp==1)DeltaTemp= (2.35 * TimePumpRest);    
  }
 
  if (ScaleTemp==0)r_set(TempPumpRest,22);
  else r_set(TempPumpRest,23);
  
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
    if (stage==8){
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

void Timing(byte stage, boolean Test, byte Type){
  //Type==0 AUTOMATIC
  //Type==1 MANUAL
  //Type==2 IODINE TEST
  //Type==3 START DELAYED

  if ((millis()-start)>1000){  // timing routine
    start = millis();
    second++;

    if(!(Test))second=0;// starts counting down when temp reached
    else{
      TimeLeft--;
      TimeSpent++;
    }
    
    if (Type==2){
      if(IodineTest)second=0;// starts counting down when temp reached
      //if(IodineTest1)second=0;// starts counting down when temp reached
      //if(IodineTest2)second=0;// starts counting down when temp reached
    }
    
    if (second>59){
      second = 0;
      if(stage==0)pumpTime = 0;
      else pumpTime++;
      stageTime--;
      if(Type==0)EEPROM.write(84,lowByte(stageTime));// saves stage time incase of interuption
    }
  }
}

void hop_add (){
  r_set(nmbrHops,70);
  
  if(hopAdd < nmbrHops){
    
    if (stageTime == hopTime){
      Buzzer(4,250);
      HopAdd(hopAdd);
      CntDwn(TimeLeft);

      if (TimeLeft<6)Buzzer(1,150);

      delay(2500);
      Buzzer(1,750);
      hopAdd++;
      EEPROM.write(85,hopAdd);
      blhpAddr++;
      r_set(hopTime,blhpAddr);
    }
  } 
}



void stage_loop (){  
  byte lastminute;

  byte Verso=0;
  unsigned long Timer=0;

  boolean tempBoilReached=false;
  
  while ((stageTime>0)&&(b_Enter)){
    lastminute=stageTime;

    Timing((x-1), tempReached, 0);

    Temperature();// get temp
    Setpoint = stageTemp;

    Input = Temp_Now;

    pauseStage();
      
    LeggiPulsante(Verso, Timer);

    
    if (pumpRest){
      PausaPompa(Temp_Now, TimeLeft);
      if (TimeSpent % 30 == 0)Buzzer(1,65);
      delay (135);
    }else{
      if ((x-1) != 0   ) CntDwn(TimeLeft);
      if (TimeLeft < 6 ) Buzzer(1, 150);
      if (TimeLeft == 0) Buzzer(1, 1000); 
      
      if ((x-1)==7 && IodineTest==false)Iodine_Test(); 
      //if ((x-1)==6 && IodineTest1==false)Iodine_Test(); 
      //if ((x-1)==7 && IodineTest2==false)Iodine_Test(); 

      if ((x-1)==8 && tempBoilReached && Temp_Now >= boilStageTemp) {  //if temp reached during boil
      
        Set(boil_output,100,0,1,Timer,Verso);
        Output = boil_output;
        
        Boil(boil_output,Temp_Now,1);
        PID_HEAT(false); //set heat in manual mode
      }else{ 
        
        float Max, Min;
        if (ScaleTemp==0){
          Max=p_C[(x-1)*3];
          Min=p_C[(x-1)*3 + 1];
        }else{
          Max=p_F[(x-1)*3];
          Min=p_F[(x-1)*3 + 1];
        }
        
        if ((x-1)==8){
          if (ScaleTemp==0)Set(stageTemp,110,EEPROM.read(17),0.25,Timer,Verso);
          else Set(stageTemp,230,EEPROM.read(18),0.25,Timer,Verso);
          NoBoil();
          tempBoilReached=false;
        }else{
          Set(stageTemp, Max, Min, 0.25, Timer, Verso);
        }
        Stage((x-1),stageTemp,Temp_Now);

        if (pumpRest==false)PID_HEAT(true);
      }  
    }
    if (Temp_Now >= stageTemp){
      if((x-1)==8 && tempBoilReached == false)tempBoilReached=true;
      if(tempReached == false){
        tempReached = true; 
      
        //***** Aggiunta per salto tenuta Mash In
        if ((x-1)==0)stageTime=0;
        //*****

        if ((x-1) == 8)hop_add();  // check for hop add once temp reached
      }
    }
    
    pump_rest((x-1));
    if ((x-1)==8){
      //check for minute tick
      if(stageTime<lastminute)hop_add();  //check to add hops
    }
    quit_mode (b_Enter);
    
    if (btn_Press(Button_enter,2500)){
      boolean flag_SaltoStep;
      //Buzzer(3,50);
      //delay(200);
      Buzzer(3,50);
      
      SaltoStep();
      //LCD_Procedo();
      
      if ((x-1) != 0)wait_for_confirm(flag_SaltoStep,1,2,1);
      if(flag_SaltoStep){
        Clear_2_3();
        return;
      }
    }  
  }
}

void add_malt (){
  boolean malt;
  pump_off(mpump);
  
  AddMalt();

  wait_for_confirm(malt,1,2,1);
  if (malt==false){
    LCD_Default(Temp_Now);
    delay(50);
    mainMenu=0;
    b_Enter = false;
  }
}

void Iodine_Test (){
  boolean Test = true;
  
  if (ScaleTemp==0)read_set(Setpoint,StageAddr-5);
  else read_set(Setpoint,StageAddr-3);
  Setpoint = Setpoint/16.0;

  pump_on();
  pumpTime=0;

  TimeSpent=0;
  
  while (Test){
    Timing(6, true, 2);
    //Timing(7, true, 2);

    Temperature();
    Input = Temp_Now;

    if (pumpRest==false)PID_HEAT(true);
    pump_rest(6);

    Iodine(Temp_Now,TimeSpent);
    
    if (TimeSpent % 45 == 0)Buzzer(1,65);
    delay (135);
    
    quit_mode(Test);  
    if (btn_Press(Button_start,50)){
      Test = false;
      
      if(x==8)IodineTest = true;
      //IodineTest1 = true;
      //if(x==8)IodineTest2 = true;
      TimeLeft= stageTime * 60;
    }
  }
  pump_on();
  pumpRest=false;
  pumpTime=0;
  Menu_2();
}

void remove_malt (){
  boolean malt;
  x = 8;               // used add to stage count on the final stage for the resume 
  EEPROM.write(83,lowByte(x)); // stores the stage number for the resume
  pump_off(mpump);
  //heat_off(mheat);

  //***** Reset della pompa al cambio step
  pumpRest = false;
  pumpTime = 0;

  RemoveMalt();

  // Pausa senza PID (gli enzimi ormai sono distrutti)
  wait_for_confirm(malt,2,2,1);
  if (malt==false){
    stageTime = EEPROM.read(71);
    EEPROM.write(84,lowByte(stageTime));
    LCD_Default(Temp_Now);
    delay(50);
    mainMenu=0;
    b_Enter = false;
  }
}


void manual_mode (){
  boolean manualLoop = false;

  byte Verso=0;
  unsigned long Timer = 0;
  
  boolean reachedBeep = false;
  
  load_pid_settings();
  r_set(boil_output,8);
  
  prompt_for_water();
  wait_for_confirm(manualLoop,2,2,2);

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
        reachedBeep=false;
        TimeSpent = 0;
      }
    }
    
    if(tempReached){
      if (reachedBeep==false){
        Buzzer(4,125);
        reachedBeep=true;
      }
    } 
    
    //Timing(7, tempReached, 1);
    Timing(8, tempReached, 1);

    quit_mode(manualLoop);
    heat_control();
    pump_control();
    
    LeggiPulsante(Verso, Timer);
    
    Manuale(mset_temp,Temp_Now,boilStageTemp);
    
    if (Setpoint >= boilStageTemp && Input >= Setpoint){ 
      Set(boil_output,100,0,1,Timer,Verso);
      Output = boil_output;

      Boil(boil_output,Temp_Now,0);
      
      if (mheat)PID_HEAT(false); //set heat in manual mode
      
      if(mpump)pump_rest(8); //Forced Boil Stage for Pump Control
      
    }else{
      if (ScaleTemp==0)Set(mset_temp,110,20,0.25,Timer,Verso);
      else Set(mset_temp,230,68,0.25, Timer, Verso);
  
      if (mheat)PID_HEAT(true); //set heat in auto mode
      
      if(mpump)pump_rest(8); //Forced Boil Stage for Pump Control
      
    }CntDwn(TimeSpent);
  }lcd.clear();
}

void WaitStart(){
  boolean Procedo=true;
  
  byte Verso;
  unsigned long Timer;
  
  TimeLeft=0;
  
  while (Procedo){
    ImpostaTempo(stageTime);
    LeggiPulsante(Verso,Timer);
    
    Set((stageTime),1440,15,15,Timer,Verso);
    
    if(btn_Press(Button_enter,50))Procedo=false;
    
    if(btn_Press(Button_start,50)){
      Procedo=false;
      mainMenu=0;
      b_Enter =false;
      return;
    }
  }
  
  LCD_Procedo();
  wait_for_confirm(Procedo,2,2,2);
  
  Clear_2_3();
  
  if(Procedo){

//  RIMUOVERE SERVE SOLO ALLA FASE DI TEST
//    stageTime=1;
//  --------------------------------------

    TimeLeft=(unsigned long)stageTime*60;
    
    start_time();
    
    while(stageTime>0){
      StartDelay(TimeLeft);
      Timing(0, true, 1);
      
      quit_mode(Procedo);    
      if(!Procedo){
        Procedo=false;
        mainMenu=0;
        b_Enter =false;
        return;
      }
  
    }
    Buzzer(5,250);
    Clear_2_3();
  
  }else{
  
    Procedo=false;
    mainMenu=0;
    b_Enter =false;
    return;
  
  }
}

void Temperatura_Raggiunta(){
  boolean TempRaggiunta = false;
  
  while (!TempRaggiunta){
    TemperaturaRaggiunta();
    pump_on();
    wait_for_confirm(TempRaggiunta,1,1,1);
  }
}

void auto_mode (){
  StageAddr=30;
  load_pid_settings();

//  check_for_resume();
  if(EEPROM.read(82)){ // FLAG Automode Started
    Resume();

    wait_for_confirm(resume,2,2,2);
    if(resume==true){
      StageAddr = (EEPROM.read(83)*5)+30;
      r_set(x,83);
      b_Enter = true;
    }
  } 
    
  Menu_2();

  if(!(resume)){  // if starting a new process prompt for water
    prompt_for_water();
    wait_for_confirm(b_Enter,2,2,2);

    Menu_2();//pulisce lo schermo

    if(!(b_Enter))return;
    hopAdd =0; //reset hop count at teh start of the processes
    EEPROM.write(85,hopAdd); 
    pumpTime =0;
    pumpRest =false;
    pump_prime();
    x = 0;
  }
 
  if (DelayedMode)WaitStart();
  
  if (b_Enter){     // mash steps
    EEPROM.write(82,1);// auto mode started
    for (byte i = x; i < 8; i++){
      EEPROM.write(83, lowByte(x)); // stores the stage number for the resume
      x++;                         // used to count the stages for the resume 
      tempReached = false;

      //***** Reset della pompa al cambio step
      pumpRest = false;
      pumpTime = 0;
      //*****

      if (ScaleTemp==0)read_set(stageTemp, StageAddr);
      else read_set(stageTemp, StageAddr+2);
      stageTemp = stageTemp/16.0;

      if (resume){                 // on the start of resume gets saved time
        stageTime=EEPROM.read(84);
        resume = false;            // clears resume for next loop
      }else{
        stageTime = EEPROM.read(StageAddr+4); // gets stage time
        EEPROM.write(84,lowByte(stageTime));// saves the intial stage time
       
        if(x==7 && stageTime==0)IodineTest=true;
        //if(x==6 && stageTime==0)IodineTest1=true;
        //if(x==7 && stageTime==0)IodineTest2=true;
        
      } 
      TimeLeft= stageTime * 60;
      
      start_time(); 
      stage_loop();
      
      if (!(b_Enter)) break;
      if( i==0 && b_Enter){    // at the end of the mashIn step pauses to add grain

//      INSERIMENTO PAUSA AGGIUNTIVA        
        Temperatura_Raggiunta();
                
        add_malt();
        if (!(b_Enter))break;

        Menu_2();
      }
      if(i==(7)&& b_Enter){   // at the end of the last step pauses to remove the malt pipe before the boil
        remove_malt();
        if (!(b_Enter))break;

        Menu_2();
      }          
      StageAddr +=5; // increase stage addresses  
    }
  }

  // start of the boil
  if(b_Enter){

    start_time(); 
    
    stageTemp= boilStageTemp; // was set the intital boil temp to 98 deg c -mdw
    tempReached = false;  

    r_set(nmbrHops,70);
    
    if (resume){
      if(x!=9)stageTime = EEPROM.read(71);
      else stageTime= EEPROM.read(84);
    }else{
      stageTime = EEPROM.read(71);
      EEPROM.write(84,lowByte(stageTime));
    }
    r_set(hopAdd,85);
    blhpAddr = hopAdd+72;
    r_set(hopTime,blhpAddr);
    r_set(boil_output,8);
    TimeLeft = stageTime * 60;
    
    x=9;
    stage_loop();

    if(b_Enter){    // finishes the brewing process
      End();

      EEPROM.write(82,0); // sets auto start byte to 0 for resume
      EEPROM.write(85,0); // sets hop count to 0
      
      mainMenu=0;
      b_Enter =false;
      resume =false;
    }
  }lcd.clear();
}

void set_PID (){
  byte a = 0;
  boolean pidLoop = false;
  int pidSet;

  byte Verso=0;
  unsigned long Timer=0;

  byte setAddr = 0;

  for(byte i = 0; i < 6; i++){
    
    if (i<4||i==5)read_set(pidSet,setAddr);
    else r_set(boil_output,setAddr);
    pidLoop= true;

    while (pidLoop){
      Menu_3_1_x(i);
      
      LeggiPulsante(Verso, Timer);
      
      if(i==4){
        PidSet(boil_output,i);
        Set(boil_output, p_PID[a], p_PID[a+1], p_PID[a+2], Timer, Verso);
      }else{ 
        PidSet(pidSet,i);

        if(i<3)Set(pidSet, p_PID[a], p_PID[a+1], p_PID[a+2], Timer, Verso);
        else   Set(pidSet, p_PID[a], p_PID[a+1], p_PID[a+2], Timer, Verso);
      }
      quit_mode(pidLoop);
      if (!pidLoop)i=6;

      if(btn_Press(Button_enter,50)){     
        if(i==4)save_set(setAddr,boil_output);
        else save_set(setAddr,pidSet);
        pidLoop = false;
      }
   }  
   if (i<4 || i==5)setAddr+=2;
   else setAddr+=1;
   a+=3;
 }Clear_2_3();
}

void set_Unit (){
  byte a = 0;

  byte unitSet;

  boolean unitLoop = false;

  byte Verso=0;
  unsigned long Timer=0;
  
  byte setAddr = 15;

  for(byte i=0;i<8;i++){
    if ((i==2 && ScaleTemp==1) || (i==3 && ScaleTemp==0))unitLoop=false;
    else unitLoop= true;    
    
    if(i!=7)r_set(unitSet,setAddr);
    else{
      if (ScaleTemp==0) r_set(unitSet,22);
      else r_set(unitSet,23);
    }
    
    while (unitLoop){
      Menu_3_2_x(i);  
      if (i==0){
        ScaleTemp=unitSet;
        Gradi();
      }
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
      if (!unitLoop)i=8;

      if(btn_Press(Button_enter,50)){
        if(i<2 || i>3){
          save_set(setAddr,lowByte(unitSet));

          if (i==0){
            ScaleTemp=unitSet;
            Gradi();
          }
          if (i==1)SensorType=unitSet;    
      
          if (i==5 && SensorType==1){ //Il SENSORE E' ESTERNO
            save_set(21,lowByte(1));//La pompa deve essere OBBLIGATORIAMENTE ON
            //Il Pump Rest viene settato a 0
            save_set(22,lowByte(105));
            save_set(23,lowByte(221));
            unitLoop = false;
            i=8;
          }
                    
          if(i==7){
            if (ScaleTemp==0){// °C
              save_set(22,lowByte(unitSet));
              save_set(23,lowByte((int)((unitSet*1.8)+32)));      
            }else{// °F
              save_set(22,lowByte((int)((unitSet-32)/1.8)));
              save_set(23,lowByte(unitSet));  
            }  
            i==8;
          }
        }else{
          if (i==2){
            save_set(setAddr, lowByte(unitSet));
            save_set(setAddr+1, lowByte(int((unitSet * 1.8) + 32)));
            boilStageTemp = unitSet;
          }
          
          if(i==3){
            save_set(setAddr-1 ,lowByte( int((unitSet - 32) / 1.8)));
            save_set(setAddr, lowByte(unitSet));
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
  
  for (byte i=0; i<8; i++){ // loops for the number of stages
    TempTimeLoop = true;
    autoLoop = true;

    if(ScaleTemp==0){
      
      read_set(stagetempSet,StageAddr);
     
      if(i>0)read_set(temp_stageTemp, StageAddr-5);
      temp_stageTemp=temp_stageTemp/16.0;
      Max=p_C[a];
      Min=p_C[a+1];
      Step=p_C[a+2];
      DeltaTemp=3.5;
    }else{ 
      read_set(stagetempSet, StageAddr+2);
      
      if(i>0)read_set(temp_stageTemp, StageAddr-3);
      temp_stageTemp=temp_stageTemp/16.0;
      Max=p_F[a];
      Min=p_F[a+1];
      Step=p_F[a+2];
      DeltaTemp=6.3;    
    }
    if(!(MashInFLAG))DeltaTemp=0;

    stagetempSet = stagetempSet/16.0;
    
    if(i>0){      
      if(Max<temp_stageTemp-DeltaTemp){//La temepratura precedente e' sopra i limiti dello stage   
        //Salta lo Stage
        if(i!=6 && i!=7){ 
          stagetempSet = temp_stageTemp*16;
          w_stagetempSet =word(stagetempSet);

          if (ScaleTemp==0){
            // Salva il settaggio in °C
            save_set(StageAddr, w_stagetempSet);

            // Salva il settaggio in °F
            ConvertiCtoF(stagetempSet); 
            w_stagetempSet = word(stagetempSet);
            save_set(StageAddr+2,w_stagetempSet);
          }else{
            // Salva il settaggio in °F
            save_set(StageAddr+2,w_stagetempSet);

            // Salva il settaggio in °C
            ConvertiFtoC(stagetempSet);
            w_stagetempSet = word(stagetempSet);
            save_set(StageAddr,w_stagetempSet);
          }          
          save_set(StageAddr+4, lowByte(0));
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
      if (!TempTimeLoop){
        Control=false;
        return;
      }

      if (btn_Press(Button_start,50)){
        
        if(ScaleTemp==0)stagetempSet=p_C[a+1];
        else stagetempSet=p_F[a+1];
        
        if (Min>stagetempSet)stagetempSet=Min;
        
        FlagStart = true;
      }
     
      LeggiPulsante(Verso, Timer);

      Set(stagetempSet, Max, Min, Step, Timer, Verso);
      
      // Vengono obbligati i tempi di: Mash In - A-Amilasi - Mash Out
      if (i==0 || i==6 || i==7 )FlagStart=false;      

      if (FlagStart||(btn_Press(Button_enter,50))){
        if(i==0)MashInTemp=stagetempSet;
    
        stagetempSet = stagetempSet*16;
        w_stagetempSet =word(stagetempSet);

        if (ScaleTemp==0){
          // Salva il settaggio in °C
          save_set(StageAddr, w_stagetempSet);

          // Salva il settaggio in °F
          ConvertiCtoF(stagetempSet); 
        
          w_stagetempSet = word(stagetempSet);
          save_set(StageAddr+2,w_stagetempSet);
        }else{
          // Salva il settaggio in °F
          save_set(StageAddr+2,w_stagetempSet);

          // Salva il settaggio in °C
          ConvertiFtoC(stagetempSet);
          w_stagetempSet = word(stagetempSet);
          save_set(StageAddr,w_stagetempSet);
        }          
        if (FlagStart){//      Viene memorizzato il tempo a 0
          
          save_set(StageAddr+4, lowByte(0));
          autoLoop = false;
        }
        TempTimeLoop = false;
        FlagStart = false;
      }
    }

    if (autoLoop)TempTimeLoop = true;

    r_set(stagetimeSet,StageAddr+4);
    
    while (TempTimeLoop){ // loops to adjust time setting
      if (i!=0){
        TimeSet(stagetimeSet);

        quit_mode(TempTimeLoop);
        if (!TempTimeLoop){
          Control=false;
          return;
        }

        LeggiPulsante(Verso, Timer);   
        Set(stagetimeSet, 140, 1, 1, Timer, Verso);
       
       /* 
        if (i!=6 && i!=7 && btn_Press(Button_start,50)){    // Viene memorizzato il tempo a 0
          save_set(StageAddr+4,lowByte(0));
          TempTimeLoop = false;
        }
      */
      
        if (btn_Press(Button_enter,50)){
          if (stagetimeSet>0){
            save_set(StageAddr+4,lowByte(stagetimeSet));
            if(MashInFLAG)MashInFLAG=false;
            TempTimeLoop = false;
          }
        }
      }
      else{ //Se si tratta del Mash In setta il tempo a 1 (fittizio)
        save_set(StageAddr+4,lowByte(1));
        TempTimeLoop = false;
      }
    } 
    StageAddr+=5;
    a+=3;
  }
  if(Control)set_hops();
  Clear_2_3();
}  


byte Congruita(byte& numRicetta, byte Verso){  
  if (EEPROM.read(89+numRicetta)==0){
    boolean Controllo=true;
    //byte numControllo=numRicetta; 
    
    while(Controllo){
      //if(Verso==1)if(numControllo<10)numControllo++;
      if(Verso==1)if(numRicetta<10)numRicetta++;
      else Controllo=false; 
        
      //if(Verso==2)if(numControllo>1)numControllo--;
      if(Verso==2)if(numRicetta>1)numRicetta--;
      else Controllo=false;
      
      //if (EEPROM.read(89+numControllo)==1){
      if (EEPROM.read(89+numRicetta)==1){ 
        //numRicetta=numControllo;
        Controllo=false;
      }
    }
  }
}

void loadRecipe(){
  byte numRicetta = 0;  
  boolean ricettaLoop = true;

  byte Verso=0;
  unsigned long Timer=0; 
 
  byte RicettaUp, RicettaDwn;
  RicettaUp=0;
  RicettaDwn=0;
  
  for(byte i=90; i<100; i++){//Assegna il limite di ricette registrate 
    if (EEPROM.read(i)==1){
      RicettaUp = (i-89);
      if(RicettaDwn==0)RicettaDwn = RicettaUp;
    }
  }
  if (RicettaUp==0){
    NoRecipe();
    return;
  }
  
  byte NomeRicetta[10];
  byte pos = 0; 
  
  for(byte i=RicettaDwn+89; i<RicettaUp+89+1; i++){//Trova la prima ricetta libera
    numRicetta=i-89;
    
    while (ricettaLoop){
      Ricetta(numRicetta,0);
      LeggiPulsante(Verso,Timer);
      Set(numRicetta, RicettaUp, RicettaDwn, 1, Timer, Verso);
      
      for (pos=0; pos<10; pos++){
        LCD_NomeRicetta(pos, EEPROM.read(620 + pos + ((numRicetta - 1)*10)));
      }
      
      Congruita(numRicetta, Verso);
      if (btn_Press(Button_enter,50)){
        Menu_3();
        Menu_3_4();
        ricettaLoop = false;  
        i=100;
      }
      if (btn_Press(Button_start,50)){
        Clear_2_3();
        LeggoRicetta(numRicetta);
        //SwapRecipe(numRicetta, 0);
        
        int Da;
  
        //Parametri Ricetta
        Da= 100 + ((numRicetta-1)*52);
        for (int j=30; j<82; j++){ 
          save_set (j, (byte)EEPROM.read(Da));
          //delay(75);
          Da++;
        }
        ricettaLoop = false;  
      }
    }
  }
}


void saveRecipe(){
  boolean saverecipeLoop;
  byte numRicetta = 0;
   
  for(byte i=90; i<100; i++){//Trova la prima ricetta libera
    if (EEPROM.read(i)==0){
      numRicetta = (i - 89);
      i=99;
    }
  }
  
  if (numRicetta == 0) MemoriaPiena();
  else {
    byte Verso=0;
    unsigned long Timer=0;
    // Spc 32
    // 0-9 da  48 a  57
    // A-Z da  65 a  90
    // a-z da  97 a 122
  
    byte NomeRicetta[10];
    byte pos = 0; 
    //NomeRicetta[pos] = 32;
    NomeRicetta[pos] = 97;
  
    Clear_2_3();
  
    Ricetta(numRicetta,1);

    while (pos<10){
      LCD_NomeRicetta(pos,NomeRicetta[pos] );
      lcd.blink();
    
      LeggiPulsante(Verso, Timer);
      Set(NomeRicetta[pos],122,32,1,Timer,Verso);
    
      if ((NomeRicetta[pos]> 32 && NomeRicetta[pos]< 48) && Verso==1)NomeRicetta[pos]=48;
      if ((NomeRicetta[pos]> 57 && NomeRicetta[pos]< 97) && Verso==1)NomeRicetta[pos]=97;
    
      if ((NomeRicetta[pos]< 97 && NomeRicetta[pos]> 57) && Verso==2)NomeRicetta[pos]=57;
      if ( NomeRicetta[pos]< 48 && Verso==2)NomeRicetta[pos]=32;
      
      if(btn_Press(Button_enter,50)){
        pos++;
        //NomeRicetta[pos]=32;
        NomeRicetta[pos]=97;
      }
    
      if ((digitalRead(Button_dn)==0) && (digitalRead(Button_up)==0)){
        delay(350);
        if ((digitalRead(Button_dn)==0) && (digitalRead(Button_up)==0)){
          for (byte j=pos; j<10; j++){
            NomeRicetta[pos]=32;
            pos++;
          }pos=9;
        }
      }    
      
      if(btn_Press(Button_start,50)){
        if(pos>0)pos--;
      }
      
    }
  
    lcd.noBlink();
  
    SalvataggioRicetta (numRicetta);
  
    wait_for_confirm(saverecipeLoop,2,2,2);
  
    if (saverecipeLoop==false){
      Menu_3();
      Menu_3_4();
      return;
    }else{
      SalvaRicetta();
      //SwapRecipe(numRicetta, 1);
      
      int Da;
  
      //Parametri Ricetta
      Da= 100 + ((numRicetta-1)*52);
      for (byte j=30; j<82; j++){ 
        save_set (Da, (byte)EEPROM.read(j));
        //delay(75);
        Da++;
      }
      
      
      //Nome Ricetta
      for (pos=0; pos<10; pos++){
        save_set(620 + pos + ((numRicetta - 1)*10),NomeRicetta[pos]);
        //delay(75);
        //EEPROM.write(620 + pos + ((numRicetta - 1)*10),NomeRicetta[pos]);
      }
      //Byte di Controllo
      save_set(89+numRicetta,(byte)1);
      //EEPROM.write(89+numRicetta,1);
    }
  }
}

void deleteRecipe(){
  byte numRicetta = 0;
  boolean ricettaLoop = true;
  
  byte Verso=0;
  unsigned long Timer=0; 
  
  byte RicettaUp, RicettaDwn;
  RicettaUp=0;
  RicettaDwn=0;
  
  for(byte i=90; i<100; i++){//Assegna il limite di ricette registrate 
    if (EEPROM.read(i)==1){
      RicettaUp = (i-89);
      if(RicettaDwn==0)RicettaDwn = RicettaUp;
    }
  }
  if (RicettaUp==0){
    NoRecipe();
    return;
  }
  
  for(byte i=RicettaDwn + 89; i<RicettaUp + 89 + 1; i++){//Trova la prima ricetta libera
    numRicetta=i-89;
    
    while (ricettaLoop){
      CancelloRicetta(numRicetta);
      LeggiPulsante(Verso,Timer);
      Set(numRicetta, RicettaUp, RicettaDwn, 1, Timer, Verso);
      
      Congruita(numRicetta, Verso);
      if (btn_Press(Button_enter,50)){
        Menu_3(); 
        Menu_3_4();
        ricettaLoop = false;  
        i=100;
      }
      if (btn_Press(Button_start,50)){
        Cancellazione(numRicetta);
        save_set(89+numRicetta,(byte)0);
        //EEPROM.write(89+numRicetta,0);
        ricettaLoop = false;  
        i=100;  
      }
    }
  }
}

void initializeRecipe(){
  boolean initialize;
  
  Inizializzazione();
  
  wait_for_confirm(initialize,2,2,2);
  
  if (initialize==false){
    Menu_3();
    Menu_3_4();
    return;
  }else{
    Inizializza();      	
    for(byte i=1; i<11; i++){
      save_set(89+i,(byte)0);
      //EEPROM.write(89+i,0);
    }	
  }
}

void RecipeMenu(){
  boolean recipeLoop = true;
  byte recipeMenu = 0;
  
  while (recipeLoop){
    switch (recipeMenu){ 
    
      case(0):
        Menu_3_4_1();
        if (btn_Press(Button_start,50))recipeLoop=false;
        if (btn_Press(Button_dn,50))recipeMenu = 1;
        if (btn_Press(Button_enter,50))loadRecipe(); 
        break;

      case(1):
        Menu_3_4_2();
        if (btn_Press(Button_start,50))recipeLoop=false;
        if (btn_Press(Button_up,50))recipeMenu = 0;
        if (btn_Press(Button_dn,50))recipeMenu = 2;
        if (btn_Press(Button_enter,50))saveRecipe(); 
        break;

      case(2):
        Menu_3_4_3();
        if (btn_Press(Button_start,50))recipeLoop=false;
        if (btn_Press(Button_up,50))recipeMenu = 1;
        if (btn_Press(Button_dn,50))recipeMenu = 3;
        if (btn_Press(Button_enter,50))deleteRecipe();
        break;

      case(3):
        Menu_3_4_4();
        if (btn_Press(Button_start,50))recipeLoop=false;
        if (btn_Press(Button_up,50))recipeMenu = 2;
        if (btn_Press(Button_enter,50))initializeRecipe();
        break;
    }        
  }Menu_3_4();
}


void set_hops (){
  boolean hopLoop;
  byte hopSet;
  r_set(nmbrHops,70);
  
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
    
    quit_mode(hopLoop);
    if(!hopLoop)return;

    if (btn_Press(Button_enter,50)){
      save_set(70,nmbrHops);
      hopLoop = false;
    }
  }

  nmbrHops+=1;

  for(byte i =0;i<nmbrHops;i++){
    hopLoop = true;
    r_set(hopSet,blhpAddr);
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
      if(!hopLoop)return;

      if(i==0)TimeUp=180;
      else{
       if(i==1)r_set(TimeUp,blhpAddr-1);
       else TimeUp=EEPROM.read(blhpAddr-1)-1;
      
      }

      LeggiPulsante(Verso, Timer);
      Set(hopSet, TimeUp, 0, 1, Timer, Verso);
      
      if (btn_Press(Button_enter,50)){
        save_set(blhpAddr,hopSet);
        hopLoop = false;
      }
    }blhpAddr+= 1;
  }
  
  for (byte i=nmbrHops; i<11; i++){
    save_set(blhpAddr,(byte)0);
    blhpAddr+= 1;
  }
  
  Clear_2_3();
}


/*
void TestRam(){  
  Menu_4_1();
}
*/

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
  //Serial.begin(9600);

  // SETTING LCD*****
  // Select your LCD
//  lcd.begin(16,2);
  lcd.begin(20,4);

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
  
  Gradi();
  // write custom symbol to LCD
  //lcd.createChar(0, degC);         // Celsius
  //lcd.createChar(1, degF);         // Faherenheit
  lcd.createChar(2, SP_Symbol);    // Set Point
  lcd.createChar(3, PumpONOFF);    // Pump
  lcd.createChar(4, RevPumpONOFF);    // Pump
  lcd.createChar(5, HeatONOFF);    // Resistor ON-OFF
  lcd.createChar(6, RevHeatONOFF); // Heat On-OFF
  lcd.createChar(7, Language);     // Language Symbol 
}

void loop(){
  boolean StartNow = false;
    
  pumpTime=0;
  TimeSpent=0;
  TimeLeft=0;
  
  IodineTest=false;
  //IodineTest1=false;
  //IodineTest2=false;

  switch(mainMenu){

    case (1):
    Menu_1();
    manual_mode();
    mainMenu = 0;
    break;

    case (2):
    Menu_2();
    
    PartenzaRitardata();
    wait_for_confirm(StartNow,2,2,2);
    
    if(!(StartNow))DelayedMode=true;
    
    Menu_2();
    auto_mode(); 
    mainMenu = 0;
    break;

    case (3):   
    Menu_3();
    setup_mode();
    mainMenu = 0;    
    break;
/*
    case (4):
    Menu_4();
    TestRam();
    mainMenu = 0;    
    break;
*/
  default: 
    DelayedMode=false;
    mheat=false;
    mpump=false;  
    heat_off(mheat);
    pump_off(mpump);
    Temperature();
    LCD_Default(Temp_Now);

    if (btn_Press(Button_dn,500))mainMenu = 1;
    if (btn_Press(Button_start,500))mainMenu = 2;
    if (btn_Press(Button_enter,500))mainMenu = 3;
    //if (btn_Press(Button_up,2500))mainMenu = 4;
    break;    
  }
}
