char *PIDName[]   ={"Konstant  P", "Konstant  I", "Konstant  D", "Windowset  ", "Use in Boil", "Calibration"};
char *stageName[] ={"Mash In   ", "Phytase   ", "Glucanase ", "Protease  ", "B-Amylase ", "A-Amylase ", "Mash Out  ", "Boil      "};
char *unitName[]  ={"Set Degrees  ", "Sensor   ", "Set Boil    ", "Set Boil    ", "Pump Cycle   ", "Time Pump Rest", "Pump on Boil ", "Pump Stop   "};

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
  LCDClear(1);
}

void Versione(byte locX, byte locY){
  lcd.setCursor(locX, locY);
  lcd.print(Version);
  lcd.write(7);
}

void Intestazione(){  
  lcd.setCursor(0,0);
  lcd.print(F("Open ArdBir 2014"));
}

void LCD_OkEsci(){
  lcd.setCursor(0,1);
  lcd.print(F(" --  --  Ok Quit"));
}

void LCD_Procedo(){
  lcd.setCursor(1,1);
  lcd.print(F("Sure?    Yes No"));
}
void LCD_Default(float Temp_Now){
  Intestazione();
  lcd.setCursor(0,1);
  LCDSpace(1);
  if (Temp_Now>=100)lcd.print(Temp_Now,1);
  else lcd.print(Temp_Now);
  Gradi();
  //Riga1;
  Versione(8,1);
}




void Menu_1(){
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print(F("  MANUAL  MODE  "));
  delay(750);
}    
void Manuale(float Set, float Temp, float TempBoil){    
  lcd.setCursor(0,0);
  lcd.print(F("MANUAL    "));
  
  if (Set>=TempBoil && Temp>=Set){ 
    if (Temp>=100)lcd.print(Temp,1);
    else lcd.print(Temp);
    Gradi();
  }else{
    if (Set>=100)lcd.print(Set,1);
    else lcd.print(Set);
    lcd.write(2);
    
    lcd.setCursor(1,1);
    if (Temp>=100)lcd.print(Temp,1);
    else lcd.print(Temp);
    Gradi();
  }
 
/*  
  if (Temp<TempBoil){
    lcd.setCursor(10,0);
    if (Set>=100)lcd.print(Set,1);
    else lcd.print(Set);
    lcd.write(2);
  }
  
  if (Temp<TempBoil)lcd.setCursor(1,1);
  else lcd.setCursor(10,0);
  if (Temp>=100)lcd.print(Temp,1);
  else lcd.print(Temp);
  Gradi();
*/
}    


void Menu_2(){
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print(F(" AUTOMATIC MODE "));
}
void AddMalt(){
  lcd.setCursor(0,0);
  lcd.print(F("   Add   Malt   "));
  LCD_OkEsci();
  Buzzer(1, 1000);
}
void Stage(byte Stage, float Set, float Temp){
  lcd.setCursor(0,0);
  lcd.print(stageName[Stage]);
  
  lcd.setCursor(10,0);
  if (Set>=100)lcd.print(Set,1);
  else lcd.print(Set);
  lcd.write(2);
  
  lcd.setCursor(1,1);
  if (Temp>=100)lcd.print(Temp,1);
  else lcd.print(Temp);
  Gradi();

}

void SaltoStep(){
  lcd.setCursor(0,0);
  lcd.print(F("Go to Next Step?"));
}

void RemoveMalt(){
  lcd.setCursor(0,0);
  lcd.print(F("  Remove  Malt  "));
  LCD_OkEsci();
  Buzzer(1, 1000);
}

void Temp_Wait(float Temp){
  lcd.setCursor(1,1);
  if (Temp>=100)lcd.print(Temp,1);
  else lcd.print(Temp);
  Gradi();
}

void Boil(float Heat, float Temp, byte Tipo){
  if (Tipo==1){
    lcd.setCursor(0,0);
    lcd.print(F("Boil      "));
  }
  
  lcd.setCursor(1,1);
  lcd.print(F("H="));    //Display output%
  if (Heat<100)LCDSpace(1); //added space as place holder for <100
  lcd.print(Heat,0); //Display output%
  lcd.print(F("% ")); 
  
  lcd.setCursor(10,0);
  if (Temp>=100)lcd.print(Temp,1);
  else lcd.print(Temp);
  Gradi();
} 
void NoBoil(){ 
}

void HopAdd(float Temp, byte HopAdd){
  lcd.setCursor(0,0);
  lcd.print(F("Hops   "));
  if(HopAdd<9)LCDSpace(1);
  lcd.print(HopAdd+1);
  lcd.setCursor(1,1);
  LCDSpace(6); 
}

void Menu_3(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F(" CONFIGURATION  "));
  delay(750);
}
void Menu_3_1(){
  lcd.setCursor(0,0);
  lcd.print(F("P.I.D.PARAMETERS"));
}     
void Menu_3_1_x(byte i){
  lcd.setCursor(0,1);
  lcd.print(PIDName[i]);
}  
void PidSet(int pidSet, byte i){
  if(i==4||i==5)lcd.setCursor(11,1);
  else lcd.setCursor(12,1);
   
  if (i<3||i==4){
    if (pidSet<=-10 && pidSet>-100 )LCDSpace(1);
    if (pidSet<0 && pidSet>-10)LCDSpace(2);
    if (pidSet<10 && pidSet>=0)LCDSpace(3);
    if (pidSet>=10 && pidSet<100)LCDSpace(2);
    if (pidSet>=100)LCDSpace(1);  
    lcd.print(pidSet);
    if(i==4)lcd.print(F("%"));
  }
  if(i==3){  
    if (pidSet<1000)LCDSpace(1);
    lcd.print(pidSet);
  }
  
  if(i==5){
    float OffSet=pidSet/10.0;
    if (OffSet>=0)LCDSpace(1);
    lcd.print(OffSet);
    LCDSpace(1);
  }
}


void Menu_3_2(){
  lcd.setCursor(0,0);
  lcd.print(F("UNIT  PARAMETERS"));
}     

void Menu_3_2_x(byte i){
  lcd.setCursor(0,1);
  lcd.print(unitName[i]);
}
void UnitSet(byte unitSet, byte i){
  
  switch(i){
     
    case(0):// Scala Temp
        lcd.setCursor(14,1);
        if (unitSet==0)lcd.write(8);
        else lcd.write(1);  
        break;
      
    case(1)://Sensore
      lcd.setCursor(9,1);
      if (unitSet==0)lcd.print(F("Inside "));
      else lcd.print(F("Outside"));
      break;
      
    case(2):// Temperatura di Ebollizione �C
      lcd.setCursor(12,1);
      if (unitSet<100)LCDSpace(1);
        lcd.print(unitSet);
        lcd.write(8);
      break;
    
    case(3):// Temperatura di Ebollizione �F
      lcd.setCursor(12,1);
      if (unitSet<100)LCDSpace(1);
        lcd.print(unitSet);
        lcd.write(1);
      break;
      
    case(4):// Durata Ciclo Pompa
      lcd.setCursor(13,1);
      if (unitSet<10)LCDSpace(1);
        lcd.print(unitSet);
        lcd.print(F("'"));
      break;
    
    case(5)://Durata Pausa Pompa
      lcd.setCursor(14,1);
      lcd.print(unitSet);
      lcd.print(F("'"));
      break;
    
    case(6):
      lcd.setCursor(13,1);
      if (unitSet==0)lcd.print(F("OFF"));
      if (unitSet==1)lcd.print(F(" ON"));
      break;
      
    case(7):
      lcd.setCursor(12,1);
      if (unitSet<100)LCDSpace(1);
      lcd.print(unitSet);
      Gradi();
  }  
}


void Menu_3_3(){
  lcd.setCursor(0,0);
  lcd.print(F(" SET AUTOMATION "));
}     
void Menu_3_3_x(byte Stage){
  lcd.setCursor(0,1);
  lcd.print(stageName[Stage]);
}  
void StageSet(float Temp){
  lcd.setCursor(9,1);
  if (Temp<10)LCDSpace(2);
  if (Temp<100)LCDSpace(1);
  lcd.print(Temp);
  Gradi();
}

void TimeSet(int Time){
  lcd.setCursor(9,1);
  if (Time<10)LCDSpace(5);
  if (Time>=10 && Time<100)LCDSpace(4);
  if (Time>=100)LCDSpace(3);
  lcd.print(Time);   
  lcd.print(F("'"));
}


void Menu_3_3_8(){
  lcd.setCursor(0,1);
  lcd.print(F("Number of Hops"));
} 
void NumHops(byte SetNumHops){
  lcd.setCursor(14,1);
  if(SetNumHops<10)LCDSpace(1);
  lcd.print(SetNumHops);   
}

void Menu_3_3_9(){
  lcd.setCursor(0,1);
  lcd.print(F("Boil        "));
} 

void Menu_3_3_10(byte SetHop){
  lcd.setCursor(0,1);
  lcd.print(F("Hops     "));
  if(SetHop<10)LCDSpace(1);
  lcd.print(SetHop);

  
} 
void TimeHops(int Time){
  lcd.setCursor(12,1);
  if (Time<10)LCDSpace(2);;
  if (Time>=10 && Time<100)LCDSpace(1);
  lcd.print(Time);   
  lcd.print(F("'"));
}

void Menu_3_4(){
  lcd.setCursor(0,0);
  lcd.print(F(" SETTING MANAGE "));
  LCDClear(1);
}     
void Menu_3_4_1(){
  lcd.setCursor(0,1);
  lcd.print(F("  Load Setting  "));
}
void Menu_3_4_2(byte Riga){
  lcd.setCursor(0,Riga);
  lcd.print(F("  Save Setting  "));
}
void Menu_3_4_3(){
  lcd.setCursor(0,1);
  lcd.print(F(" Delete Setting "));
}
void Menu_3_4_4(){
  lcd.setCursor(0,1);
  lcd.print(F(" Initialization "));
}

void CaricaRicetta(byte Ricetta){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("RECIPE  "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  LCD_Procedo();
}

void NoRecipe(){
  lcd.setCursor(0,1);
  lcd.print(F("- NO  SETTINGS -"));
  Buzzer(3,50);
  delay(2500);
}

void LeggoRicetta(byte Ricetta){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F(" Loading...     "));
  lcd.setCursor(0,1);
  lcd.print(F("  Setting  "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  delay(1500);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("  Setting  "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  lcd.setCursor(0,1);
  lcd.print(F(" Setting Loaded "));
  Menu_3_4();
}


void SalvataggioRicetta(byte Ricetta){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("  SETTING  "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  LCD_Procedo();
}

void SalvaRicetta(){
  Buzzer(5,35);
  lcd.setCursor(0,1);
  lcd.print(F("Saving...       "));
  delay(1500);
  //lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(F(" Setting  Saved "));
  delay(2000);
  Menu_3_4();
}

void CancelloRicetta(byte Ricetta){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("  SETTING  "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  LCD_Procedo();
}

void Cancellazione(byte Ricetta){
  lcd.setCursor(0,0);
  lcd.print(F(" Deleting...    "));
  lcd.setCursor(0,1);
  lcd.print(F("  Setting  "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  LCDSpace(3);
  delay(1500);
  lcd.setCursor(0,0);
  lcd.print(F("  SETTING  "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  LCDSpace(3);
  lcd.setCursor(0,1);
  lcd.print(F("     DELETED    "));
  delay(1500);
  Menu_3_4();
}


void Inizializzazione(){
  lcd.setCursor(0,0);
  lcd.print(F(" Initialization "));
  LCD_Procedo();
}
void Inizializza(){
lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("  WAIT FOR...   "));
  Buzzer(2,35);
  lcd.setCursor(1,1);
  lcd.print(F("Initialize  RAM"));
  delay(2500);
  Menu_3_4();
}

void MemoriaPiena(){
lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("   ATTENTION!   "));
  Buzzer(5,35);
  lcd.setCursor(1,1);
  lcd.print(F(" FULL  MEMORY  "));
  delay(3500);
}

void Menu_3_5(){
  lcd.setCursor(0,0);
  lcd.print(F("     CREDIT     "));
}     

void Credits(){
  lcd.clear();
  Intestazione();
  Versione(5,1);
  delay(2000);

  lcd.clear();
  Intestazione();
  lcd.setCursor(0,0);
  lcd.print(F("Original Project"));
  lcd.setCursor(0,1);
  delay(950);
  lcd.print(F("Stephen Mathison"));
  delay(1500);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Modified:       "));
  lcd.setCursor(0,1);
  delay(950);
  lcd.print(F("     Mike Wilson"));
  delay(1250);
  lcd.setCursor(0,1);
  lcd.print(F("    Massimo Nevi"));
  delay(1250);
  
  /*
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print(F("    ITALIAN     "));
  lcd.setCursor(0,1);
  lcd.print(F("    REDESIGN    "));
  delay(1500);
  
  lcd.clear();
  */
  
  lcd.setCursor(0,0);
  lcd.print(F("PCB Design:     "));
  lcd.setCursor(0,1);
  delay(750);
  lcd.print(F("       DanielXan"));
  delay(800);

  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("PCB Test:      "));
  lcd.setCursor(0,1);
  delay(750);
  lcd.print(F("   SavioThechnic"));
  delay(800);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Firmware Test:  "));
  delay(750);
  lcd.setCursor(0,1);
  lcd.print(F("        A. Tidei"));
  delay(800);
  lcd.setCursor(0,1);
  lcd.print(F("    D. Arzarello"));
  delay(800);
  lcd.setCursor(0,1);
  lcd.print(F("   L. Di Michele"));
  delay(800);
  lcd.clear();
}

/*
void Menu_4(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("    TEST RAM    "));
}
void Menu_4_1(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F(" Memoria Libera "));
  lcd.setCursor(3,1);
  if (freeRam()<1000&&freeRam()>=100)LCDSpace(1);
  if (freeRam()<100&&freeRam()>=10)LCDSpace(2);
  if (freeRam()<10)LCDSpace(3);
  lcd.print(freeRam());
  LCDSpace(1);
  lcd.print(F("byte"));
  
  delay(3500);
  lcd.clear();
}
*/
void Pause_Stage(float Temp, int Time){
  lcd.setCursor(0,0);
  lcd.print(F("--- In Pause ---" ));
  
  lcd.setCursor(0,1);
  if (Temp<100)LCDSpace(1);
  if (Temp<10)LCDSpace(2);
  lcd.print(Temp);
  Gradi();
  
  CountDown(Time,8,1,1);
}

void prompt_for_water (){
  lcd.setCursor(0,0);
  lcd.print(F("  Water Added?  "));
  LCD_OkEsci();
  Buzzer(1,750);
}

void Resume(){
  lcd.setCursor(0,0);
  lcd.print(F("Resume  Process?"));
  LCD_OkEsci();
  Buzzer(1,750);
}

void CntDwn(int Time){
  CountDown(Time,8,1,1);
}

void PausaPompa(float Temp, int Time){
  if (Ciclo>=225){
    Buzzer(2,35);
    Ciclo=0;
  }
  
  lcd.setCursor(1,1);
  if (Temp>=100)lcd.print(Temp,1);
  else lcd.print(Temp);
  Gradi();
  
  CountDown(Time,8,1,1);
  
  lcd.setCursor(0,0);
  lcd.print(F("-- Pump  Rest --"));
 
  Ciclo++;
}

void Iodine(float Temp, int Time){
  lcd.setCursor(0,0);
  LCDSpace(2);
  if (Temp>=100)lcd.print(Temp,1);
  else lcd.print(Temp);
  Gradi();
  LCDSpace(1);
  CountDown(Time,9,0,1);
  
  lcd.setCursor(1,1);
  if (Ciclo<50)lcd.print(F("IODINE"));
  else lcd.print(F(" TEST "));
  
  lcd.setCursor(7,1);
  lcd.print(F(" - OK --"));
  
  Ciclo++;
  if (Ciclo>100)Ciclo=0;
}

void End(){
  Buzzer(3,250);
  lcd.setCursor(0,0);
  lcd.print(F("     Brewing    "));
  lcd.setCursor(0,1);
  lcd.print(F("    Finished!   "));
  
  Buzzer(1,3000);
  delay(2500);
}

void PumpPrime(){
  lcd.setCursor(0,1);
  lcd.print(F("   Pump Prime   "));  // priming the pump
}  

void ledHeatON(){
  lcd.setCursor(0,1);
  lcd.write(6);
}
/*
void ledHeatOFF(){
  lcd.setCursor(0,1);
  lcd.write(5);
}
*/
void ledHeatStatus(boolean mheat){
  lcd.setCursor(0,1);
  if (mheat)lcd.write(5);
  else LCDSpace(1);
}

void ledPumpON(){
  lcd.setCursor(15,1);
  lcd.write(4);
}
/*  
void ledPumpOFF(){
  lcd.setCursor(15,1);
  LCDSpace(1);
}
*/
void ledPumpStatus(boolean mpump){
  lcd.setCursor(15,1);
  if (mpump)lcd.write(3);
  else LCDSpace(1);
}

/*
void Presentazione(){
  Presentazione(0,0);
}
*/
void ArdBir(){
  ArdBir1(0,0);
}
