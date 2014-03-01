char *PIDName[]   ={" Konstant   P ", " Konstant   I ", " Konstant   D ", " Windowset ms ", "Heat in Boil  ", " Calibration  "};
char *stageName[] ={" Mash In   ", " Phytase   ", " Glucanase ", " Protease  ", " B-Amylase ", " A-Amylase ", " Mash Out  ", " Boil      "};
char *unitName[]  ={"  Set Degrees  ", "  Sensor   ", "Set Temp Boil ", "Set Temp Boil ", "  Pump  Cycle  ", " Time Pump Rest ", " Pump on Boil   ", "Temp Pump Stop "};

byte HeatONOFF[8]    = {B00000, B01010, B01010, B01110, B01110, B01010, B01010, B00000};  // [5] HEAT symbol
byte RevHeatONOFF[8] = {B11111, B10101, B10101, B10001, B10001, B10101, B10101, B11111};  // [6] reverse HEAT symbol
byte Language[8]     = {B11111, B00010, B01000, B11111, B00000, B10001, B10101, B11111};  // [7] EN symbol

byte Ciclo=0;

void LCDSpace (byte Num){
  for(byte i=0; i<Num; i++){
    lcd.print(F(" "));
  }
}

void LCDClear(byte Riga){
  lcd.setCursor(0,Riga);
  LCDSpace(20);
}

void Clear_2_3(){
  LCDClear(2);
  LCDClear(3);
}

void Versione(byte locX, byte locY){
  lcd.setCursor(locX, locY);
  lcd.print(Version);
  lcd.write(7);
}

void Intestazione(){  
  lcd.setCursor(0,0);
  lcd.print(F("Open ArdBir"));
  if (Ciclo<100)lcd.print(F(" 2012-14"));
  else{
    LCDSpace(2);
    Versione(13,0);
  }
  Ciclo++;
  if (Ciclo>250)Ciclo=0;
}

void PauseScreen(){
  lcd.setCursor(0,0);
  lcd.print(F("------"));
  lcd.setCursor(13,0);
  lcd.print(F(" ------"));

  lcd.setCursor(1,2);
  lcd.print(F("---- "));
  lcd.setCursor(14,2);
  lcd.print(F(" ----"));
}

void LCD_QQSO(){
  lcd.setCursor(0,3);
  lcd.print(F(" UP**DWN   skip  ok "));
}
void LCD_QQxO(){
  lcd.setCursor(0,3);
  lcd.print(F(" UP**DWN   ----  ok "));
}
void LCD_SGEO(){
  lcd.setCursor(0,3);
  lcd.print(F(" up  dwn   quit  ok "));
}
void LCD_xGEO(){
  lcd.setCursor(0,3);
  lcd.print(F(" --- dwn   quit  ok "));
}

void LCD_SxEO(){
  lcd.setCursor(0,3);
  lcd.print(F(" up   ---  quit  ok "));
}

void LCD_Procedo(){
  lcd.setCursor(0,3);
  lcd.print(F(" Sure-->   Ok  Quit "));
}

void LCD_Default(float Temp_Now){
  Intestazione();
  lcd.setCursor(5,1);
  LCDSpace(2);
  if (Temp_Now>=100)lcd.print(Temp_Now,1);
  else lcd.print(Temp_Now);
  Gradi();
  //Clear_2();
  LCDClear(2);
  lcd.setCursor(0,3);
  lcd.print(F(" ---  MAN AUTO SETUP"));
}

void Menu_1(){
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print(F("    MANUAL  MODE"));
 }    
 
void Manuale(float Set, float Temp,float TempBoil){    
  lcd.setCursor(1,1);
  if (Temp<100)LCDSpace(1);
  lcd.print(Temp);
  Gradi();
  
  lcd.setCursor(12,1);
  if (Set<100)LCDSpace(1);
  lcd.print(Set);
  lcd.write(2);
  
  //lcd.setCursor(1,2);
  //LCDSpace(8);
  
  lcd.setCursor(0,3);
  lcd.print(F(" UP**DWN  Heat  Pump"));
}    


void Menu_2(){
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print(F("     AUTO  MODE"));
}
void AddMalt(){
  lcd.setCursor(0,2);
  lcd.print(F("      Add Malt      "));
  //LCD_OkEsci();
  LCD_Procedo();
  Buzzer(1, 1000);
}
void Stage(byte Stage, float Set, float Temp){
  lcd.setCursor(0,0);
  lcd.print(F(" AUTO -->"));
  lcd.print(stageName[Stage]);
  
  lcd.setCursor(1,1);
  if (Temp<100)LCDSpace(1);
  lcd.print(Temp);
  Gradi();
  lcd.setCursor(8,1);
  LCDSpace(4);
  if (Set<100)LCDSpace(1);
  lcd.print(Set);
  lcd.write(2);
    
  lcd.setCursor(0,3);
  lcd.print(F(" UP  DWN  Pause --- "));
}
void RemoveMalt(){
  lcd.setCursor(0,2);
  lcd.print(F("    Remove  Malt    "));
  //LCD_OkEsci();
  LCD_Procedo();
  Buzzer(1,1500);
 }
 
void Temp_Wait(float Temp){
  lcd.setCursor(1,1);
  if (Temp<100)LCDSpace(1);
  lcd.print(Temp);
  Gradi();
}

void Boil(float Heat, float Temp, byte Tipo){
  if (Tipo==1){
    lcd.setCursor(0,0);
    lcd.print(F(" AUTO --> Boil      "));
  }
  
  lcd.setCursor(1,1);
  if(Temp<100)LCDSpace(1);
  lcd.print(Temp);
  Gradi();
  
  lcd.setCursor(1,2);
  lcd.print(F("PWM="));    //Display output%
  if (Heat<100)LCDSpace(1); //added space as place holder for <100
  lcd.print(Heat,0); //Display output%
  lcd.print(F("% ")); 
} 

void NoBoil(){
  lcd.setCursor(1,2);
  LCDSpace(9);
}

void HopAdd(float Temp, byte HopAdd){
  lcd.setCursor(9,0);
  lcd.print(F("   Hop   "));
  if(HopAdd<10)LCDSpace(1);
  lcd.print(HopAdd+1);
}

void Menu_3(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("    SETUP   MENU"));
}
void Menu_3_1(){
  lcd.setCursor(0,1);
  lcd.print(F(" P.I.D.  Parameters "));
  //Clear_2();
  LCDClear(2);
  //lcd.setCursor(0,3);
  LCD_xGEO();
}     
void Menu_3_1_x(byte i){
  lcd.setCursor(0,2);
  lcd.print(PIDName[i]);
  LCD_QQxO();
}  

void PidSet(int pidSet, byte i){
  if(i==5){
    lcd.setCursor(14,2);
    float OffSet=pidSet/10.0;
    if (OffSet>=0)LCDSpace(1);
    lcd.print(OffSet);
    LCDSpace(1);
  }
  else{ 
    lcd.setCursor(15,2);
    if (pidSet<=250){
      if (pidSet<=-10 && pidSet>-100 )LCDSpace(1);
      if (pidSet<0 && pidSet>-10)LCDSpace(2);
      if (pidSet<10 && pidSet>=0)LCDSpace(3);
      if (pidSet>=10 && pidSet<100)LCDSpace(2);
      if (pidSet>=100)LCDSpace(1);
    }
    else{ 
      if (pidSet<1000)LCDSpace(1);
    }
    lcd.print(pidSet);
    if(i==4)lcd.print(F("%"));
  }
}


void Menu_3_2(){
  lcd.setCursor(0,1);
  lcd.print(F("  Unit  Parameters  "));
  //Clear_2();
  LCDClear(2);
  //lcd.setCursor(0,3);
  LCD_SGEO();
}    

void Menu_3_2_x(byte i){
  lcd.setCursor(0,2);
  lcd.print(unitName[i]);
  LCD_QQxO();
}


void Menu_3_3(){
  lcd.setCursor(0,1);
  lcd.print(F("  Set   Automation  "));
  LCD_SGEO();
}     
void Menu_3_3_x(byte Stage){
  lcd.setCursor(1,2);
  lcd.print(stageName[Stage]);
  if (Stage==0||Stage==5||Stage==6)LCD_QQxO();
  else LCD_QQSO();
}  

void Menu_3_3_8(){
  lcd.setCursor(1,2);
  lcd.print(F("Number  of  Hops "));
  LCD_QQxO();
} 

void Menu_3_3_9(){
  lcd.setCursor(1,2);
  lcd.print(F("Boil         "));
  LCD_QQxO();
} 

void Menu_3_3_10(byte SetHop){
  lcd.setCursor(0,2);
  lcd.print(F("Hops nmbr("));
  if (SetHop<10)LCDSpace(1);
  lcd.print(SetHop);
  lcd.print(F(")  "));
  LCD_QQxO();
} 

void Menu_3_4(){
  lcd.setCursor(0,1);
  lcd.print(F("   Setting Manage   "));
  //Clear_2();
  LCDClear(2);
  LCD_SGEO();
}     
void Menu_3_4_1(){
  lcd.setCursor(0,2);
  lcd.print(F("    Load Setting    "));
  LCD_xGEO();
}
void Menu_3_4_2(){
  lcd.setCursor(0,2);
  lcd.print(F("    Save Setting    "));
  LCD_SGEO();
}
void Menu_3_4_3(){
  lcd.setCursor(0,2);
  lcd.print(F("   Delete etting   "));
  LCD_SGEO();
}
void Menu_3_4_4(){
  lcd.setCursor(0,2);
  lcd.print(F("   Initialization   "));
  LCD_SxEO();
}

void NoRecipe(){
  //Clear_2();
  LCDClear(2);  
  lcd.setCursor(0,3);
  lcd.print(F("    NO SETTINGS     "));
  Buzzer(3,50);
  delay(1500);
}

void CaricaRicetta(byte Ricetta){
  lcd.setCursor(0,2);
  lcd.print(F("   Load Setting  "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  LCDSpace(1);
  //LCD_OkEsci();
  LCD_Procedo();
}
void LeggoRicetta(byte Ricetta){
  //Clear_2();
  LCDClear(2);
  lcd.setCursor(0,3);
  lcd.print(F(" Loading Setting... "));
  delay(1500);
  
  lcd.setCursor(0,3);
  lcd.print(F("Setting "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  lcd.print(F("  Loaded  "));
  delay(1500);
}

void SalvataggioRicetta(byte Ricetta){
  lcd.setCursor(0,2);
  lcd.print(F("  Save Setting  "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  lcd.print(F("? "));
  //LCD_OkEsci();
  LCD_Procedo();
}


void SalvaRicetta(){
  //Clear_2();
  LCDClear(2);
  lcd.setCursor(0,3);
  lcd.print(F("  Saving Setting... "));
  Buzzer(5,35);
  delay(2000);
  
  lcd.setCursor(0,3);
  lcd.print(F("   Setting  Saved   "));
  delay(1500);  
}

void CancelloRicetta(byte Ricetta){
  lcd.setCursor(0,2);
  lcd.print(F(" Delete Setting "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  LCDSpace(1);
  //LCD_OkEsci();
  LCD_Procedo();
}

void Cancellazione(byte Ricetta){
  //Clear_2();
  LCDClear(2);
  lcd.setCursor(0,3);
  lcd.print(F("Deleting Setting... "));
  delay(1500);
  
  lcd.setCursor(0,3);
  lcd.print(F("  Setting  Deleted  "));
  delay(1500);
}

void Inizializzazione(){
  lcd.setCursor(0,2);
  lcd.print(F("    Initialization  "));
  //LCD_OkEsci();
  LCD_Procedo();
}
void Inizializza(){
  //Clear_2();
  LCDClear(2);
  lcd.setCursor(0,3);
  lcd.print(F(" Initialize  EEPROM "));
  Buzzer(2,35);
  delay(1500);
  
  lcd.setCursor(0,3);
  lcd.print(F("   EEPROM   ready   "));
  delay(1500);
}

void MemoriaPiena(){
  //Clear_2();
  LCDClear(2);
  lcd.setCursor(0,3);
  lcd.print(F("     ATTENTION!     "));

  Buzzer(5,35);
  delay(1000);
  
  lcd.setCursor(0,3);
  lcd.print(F("    FULL  MEMORY    "));
  delay(2500);
}

void Menu_3_5(){
  lcd.setCursor(0,1);
  lcd.print(F("      Credits       "));
  LCD_SxEO();
}     


void Menu_4(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("   TEST DELLA RAM   "));
}
void Menu_4_1(){
  lcd.setCursor(0,1);
  lcd.print(F("   Memoria Libera   "));
  lcd.setCursor(5,2);
  if (freeRam()<1000&&freeRam()>=100)LCDSpace(1);
  if (freeRam()<100&&freeRam()>=10)LCDSpace(2);
  if (freeRam()<10)LCDSpace(3);
  lcd.print(freeRam());
  LCDSpace(2);
  lcd.print(F("byte"));
  
  LCDClear(3);
  delay(3500);
  lcd.clear();
}


void Credits(){
  lcd.clear();
  Intestazione();
  Versione(7,1);
  delay(1500);

  lcd.clear();
  Intestazione();
  lcd.setCursor(0,2);
  lcd.print(F(" Original  Project:"));
  delay(950);
  lcd.setCursor(0,3);
  lcd.print(F("  Stephen Mathison "));
  delay(1500);
  
  Clear_2_3();
  
  lcd.setCursor(0,2);
  lcd.print(F("      Modified:  "));
  delay(950);
  lcd.setCursor(0,3);
  lcd.print(F("    Mike  Wilson"));
  delay(1250);
  lcd.setCursor(0,3);
  lcd.print(F("    Massimo Nevi"));
  delay(1250);
  
  /*
  lcd.clear();

  lcd.setCursor(0,1);
  lcd.print(F("    THE  ITALIAN"));
  lcd.setCursor(0,2);
  lcd.print(F("      REDISIGN  "));
  delay(1500);
  
  lcd.clear();
  
  Intestazione();
  Versione(7,1);
  */
  
  Clear_2_3();
  
  lcd.setCursor(0,2);
  lcd.print(F("    PCB  Design:"));
  delay(750);
  lcd.setCursor(0,3);
  lcd.print(F("     DanielXan  "));
  delay(850);

  Clear_2_3();

  lcd.setCursor(0,2);
  lcd.print(F("     PCB  Test: "));
  delay(750);
  lcd.setCursor(0,3);
  lcd.print(F("   SavioThechnic"));
  delay(850);

  Clear_2_3();

  lcd.setCursor(0,2);
  lcd.print(F("   Firmware  Test"));
  delay(750);
  lcd.setCursor(0,3);
  lcd.print(F("      A. Tidei   "));
  delay(850);
  lcd.setCursor(0,3);
  lcd.print(F("    D. Arzarello "));
  delay(850);
  lcd.setCursor(0,3);
  lcd.print(F("   L.  Di Michele"));
  delay(850);
  
  Clear_2_3();  
}

void UnitSet(int unitSet, byte i){
  
  switch(i){
     
    case(0):// Scala Temp
        lcd.setCursor(17,2);
        if (unitSet==0)lcd.write(8);
        else lcd.write(1);  
        break;
      
    case(1)://Sensore
      lcd.setCursor(11,2);
      if (unitSet==0)lcd.print(F("Inside   "));
      else lcd.print(F("Outside  "));
      break;
      
    case(2):// Temperatura di Ebollizione °C
      lcd.setCursor(14,2);
      if (unitSet<100)LCDSpace(1);
        lcd.print(unitSet);
        lcd.write(8);
      break;
    
    case(3):// Temperatura di Ebollizione °F
      lcd.setCursor(14,2);
      if (unitSet<100)LCDSpace(1);
        lcd.print(unitSet);
        lcd.write(1);
      break;
      
    case(4):// Durata Ciclo Pompa
      lcd.setCursor(15,2);
      if (unitSet<10)LCDSpace(1);
        lcd.print(unitSet);
        lcd.print(F("'"));
      break;
    
    case(5)://Durata Pausa Pompa
      lcd.setCursor(16,2);
      lcd.print(unitSet);
      lcd.print(F("'"));
      break;
    
    case(6):
      lcd.setCursor(16,2);
      if (unitSet==0)lcd.print(F("OFF"));
      if (unitSet==1)lcd.print(F(" ON"));
      break;
      
    case(7):
      lcd.setCursor(15,2);
      if (unitSet<100)LCDSpace(1);
      lcd.print(unitSet);
      Gradi();
  }  
}
 

void StageSet(float Temp){
  lcd.setCursor(11,2);
  if (Temp<10)LCDSpace(2);
  if (Temp<100)LCDSpace(1);
  lcd.print(Temp);  //** check -mdw
  Gradi();
}

void TimeSet(int Time){
  lcd.setCursor(11,2);
  if (Time<10)LCDSpace(5);
  if (Time>=10 && Time<100)LCDSpace(4);
  if (Time>=100)LCDSpace(3);
  lcd.print(Time);   
  lcd.print(F("'"));
}

void NumHops(byte SetNumHops){
  lcd.setCursor(16,2);
  if(SetNumHops<10)LCDSpace(1);
  lcd.print(SetNumHops);   
}

void TimeHops(int Time){
  lcd.setCursor(14,2);
  if (Time<10)LCDSpace(2);
  if (Time>=10 && Time<100)LCDSpace(1);
  lcd.print(Time);   
  lcd.print(F("'"));
}

void Pause_Stage(float Temp, int Time){
  PauseScreen();
  
  
  lcd.setCursor(0,1);
  if (Ciclo<100)LCDSpace(20);
  else lcd.print(F("      In Pause      " ));

  
  lcd.setCursor(6,0);
  if (Temp<100)LCDSpace(1);
  if (Temp<10)LCDSpace(2);
  lcd.print(Temp);
  Gradi();
  
  CountDown(Time,6,2,2);
  
  lcd.setCursor(0,3);
  lcd.print(F(" ---  ---  Exit --- "));

  Ciclo++;
  if (Ciclo>250)Ciclo=0;
}

void prompt_for_water (){
  lcd.setCursor(0,1);
  lcd.print(F("    Water  Added?   "));
  //LCD_OkEsci();
  LCD_Procedo();
  Buzzer(1,750);
}

void Resume(){
  lcd.setCursor(0,1);
  lcd.print(F("  Resume  Process?  "));
  //LCD_OkEsci();
  LCD_Procedo();
  Buzzer(1,750);
}

void CntDwn(int Time){
  CountDown(Time,11,2,2);
}

void PausaPompa(float Temp, int Time){
  if (Ciclo>=225){
    Buzzer(2,35);
    Ciclo=0;
  }
  
  lcd.setCursor(1,1);
  if (Temp<100)LCDSpace(1);
  lcd.print(Temp);
  Gradi();
  
  CountDown(Time,11,2,2);
  
  lcd.setCursor(0,3);
  lcd.print(F(" --- Pump  Rest --- "));
 
  Ciclo++;
}

void Iodine(float Temp, int Time){
  PauseScreen();
  
    
  lcd.setCursor(0,1);
  if (Ciclo<75)lcd.print(F("    IODINE  TEST    " ));
  else lcd.print(F("    Iodine  Test    " ));
  
  
  lcd.setCursor(6,0);
  if (Temp<100)LCDSpace(1);
  if (Temp<10)LCDSpace(2);
  lcd.print(Temp);
  Gradi();
  
  CountDown(Time,6,2,2);
  
  lcd.setCursor(0,3);
  lcd.print(F(" ---  ---   OK  --- "));

  Ciclo++;
  if (Ciclo>150)Ciclo=0;
}





void End(){
  lcd.clear();
  
  Buzzer(3,250);
  
  lcd.setCursor(0,1);
  lcd.print(F("       Brewing "));
  lcd.setCursor(0,2);
  lcd.print(F("      Finished!"));
  
  Buzzer(1,3000);
  delay(2500);
}

void PumpPrime(){
  lcd.setCursor(0,1);
  lcd.print(F("     Pump Prime     "));  // priming the pump
}  

void ledHeatON(){
  lcd.setCursor(0,2);
  lcd.write(6);
}
/*
void ledHeatOFF(){
  lcd.setCursor(0,2);
  lcd.write(5);
}
*/
void ledHeatStatus(boolean mheat){
  lcd.setCursor(0,2);
  if (mheat)lcd.write(5);
  else LCDSpace(1);
}

void ledPumpON(){
  lcd.setCursor(19,2);
  lcd.write(4);
}
/*  
void ledPumpOFF(){
  lcd.setCursor(19,2);
  LCDSpace(1);
}
*/
void ledPumpStatus(boolean mpump){
  lcd.setCursor(19,2);
  if (mpump)lcd.write(3);
  else LCDSpace(1);
}

/*
void Presentazione(){
  Presentazione(2,1);
}
*/
void ArdBir(){
  ArdBir1(2,1);
}
