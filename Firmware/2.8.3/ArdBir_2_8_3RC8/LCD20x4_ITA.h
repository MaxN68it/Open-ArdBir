char *PIDName[]   ={"Uso ", "Costante  kP", "Costante  kI", "Costante  kD", "SampleTime  ", "Finestra ms ", "PWM Bollit.  ", "Calibrazione", "Isteresi    "};
char *stageName[] ={"Mash In   ", "Fitasi    ", "Glucanasi ", "Proteasi  ", "\xE2""Amilasi  ", "\xE0""Amilasi1 ", "\xE0""Amilasi2 ", "Mash Out  ", "Bollitura ", "Raffreddo ", "Whirlpool "};
char *unitName[]  ={"Scala Gradi", "Sensore    ", "Ebollizione", "Ebollizione", "Ciclo Pompa", "Pausa Pompa", "Pmp PreMash", "Pmp on Mash", "Pmp MashOut", "Pmp on Boil", "Fermo Pompa", "Fermo Pompa", "PID Pipe   ", "Skip Add   ", "Skip Remove", "Skip Iodine", "TempoIodio ", "Whirlpool  "};

byte HeatONOFF[8]    = {B00000, B01110, B01010, B01010, B01100, B01010, B01010, B00000};  // [5] HEAT symbol
byte RevHeatONOFF[8] = {B11111, B10001, B10101, B10101, B10011, B10101, B10101, B11111};  // [6] reverse HEAT symbol
byte Language[8]     = {B00000, B10000, B11111, B10000, B00000, B11111, B00000, B00000};  // [7] ITA symbol


void LCDClear(byte Riga) {
  lcd.setCursor(0, Riga);
  LCDSpace(20);
}

void PrintTemp(byte PosX, byte PosY, float Temp, byte dec) {
  if (PosY < 4) lcd.setCursor(PosX, PosY);
  
  FormatNumeri(Temp, -1);
  //if (Temp<10.0)LCDSpace(2);
  //if (Temp>=10.0 && Temp<100.0)LCDSpace(1);
  
  lcd.print(Temp, dec);
  lcd.write((byte)0);
}

void Clear_2_3() {
  LCDClear(2);
  LCDClear(3);
}

void Version(byte locX, byte locY) {
  lcd.setCursor(locX, locY);
  lcd.print(F("2.8.3RC"));
  lcd.write(7);
}

void Intestazione() { 
  lcd.setCursor(0, 0);
  lcd.print(F("Open ArdBir"));
  lcd.write(1);
  Version(12, 0);
}

void LCD_Procedo() {
  lcd.setCursor(1, 3);
  lcd.print(F("Procedo->  Ok Esci"));
}

void LCD_Default(float Temp) {
  Intestazione();

  //lcd.setCursor(6,1);
  PrintTemp(6, 1, Temp, 2);

  LCDClear(2);
  
  lcd.setCursor(1, 3);
  lcd.print(F("---  MAN AUTO SETUP"));
}

void CntDwn(int Time) {
  CountDown(Time, 11, 2, 2);
}

void Watch(int Time) {
  CountDown(Time, 6, 2, 2);
}

void PauseScreen() {
  lcd.setCursor(0, 0);
  lcd.print(F("-----  "));
  lcd.setCursor(14, 0);
  lcd.print(F(" -----"));

  lcd.setCursor(1, 2);
  lcd.print(F("----"));
  lcd.setCursor(14, 2);
  lcd.print(F(" ----"));
}

void LCD_QQSO() {
  lcd.setCursor(1, 3);
  lcd.print(F("SU* *GIU Salta  Ok"));
}
void LCD_QQxO() {
  lcd.setCursor(1, 3);
  lcd.print(F("SU* *GIU  ---   Ok"));
}
void LCD_SGEO() {
  lcd.setCursor(1, 3);
  lcd.print(F("Su   Giu  Esci  Ok"));
}
void LCD_xGEO() {
  lcd.setCursor(1, 3);
  lcd.print(F("---  Giu  Esci  Ok"));
}

void LCD_SxEO() {
  lcd.setCursor(1, 3);
  lcd.print(F("Su   ---  Esci  Ok"));
}


void Menu_1() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("MODALITA'  MANUALE"));
}    

void Manuale(float Set, float Temp,float TempBoil) {    
  //lcd.setCursor(1,1);
  PrintTemp(1, 1, Temp, 2);

  lcd.setCursor(12, 1);
  if (Set < 100) LCDSpace(1);
  lcd.print(Set);
  lcd.write(2);
  
  lcd.setCursor(1, 3);
  lcd.print(F("SU* *GIU  Res  Pmp"));
}

void Menu_2() {
  lcd.clear();  

  lcd.setCursor(0, 0);
  lcd.print(F("MODALITA' AUTOMATICA"));
}
void AddMalt() {
  lcd.setCursor(2, 2);
  lcd.print(F(" Aggiungi Malto "));
  
  LCD_Procedo();
}

void Stage(byte Stage, float Set, float Temp) {
  lcd.setCursor(0, 0);
  lcd.print(F(" AUTO --> "));
  if (Stage < 11) lcd.print(stageName[Stage]);
  else            lcd.print(stageName[10]);

  PrintTemp(1, 1, Temp, 2);

  lcd.setCursor(8, 1);
  LCDSpace(4);
  if (Set < 100)LCDSpace(1);
  lcd.print(Set);
  lcd.write(2);
  
  lcd.setCursor(1, 3);
  if (Stage ==  0)               lcd.print(F("SU* *GIU Pausa ---"));
  if (Stage  >  0 && Stage  < 8) lcd.print(F("SU* *GIU Pausa STP"));
  if (Stage ==  8)               lcd.print(F("SU* *GIU  ---  ---"));
  if (Stage ==  9)               lcd.print(F("SU* *GIU  ---  Pmp"));
  if (Stage == 10)               lcd.print(F("--* *---  ---  Pmp"));
  if (Stage == 11)               lcd.print(F("--* *--- Tempo Pmp")); 
}

void SaltoStep() {
  lcd.setCursor(2, 2);
  lcd.print(F("Step Successivo? "));
  LCD_Procedo();
}

void RemoveMalt() {
  lcd.setCursor(3, 2);
  lcd.print(F("Rimuovi  Malto  "));
  LCD_Procedo();
}

void Temp_Wait(float Temp) {
  //lcd.setCursor(1,1);
  PrintTemp(1, 1, Temp, 2);
}

void Boil(float Heat, float Temp, byte Tipo) {
  if (Tipo == 1) {
    lcd.setCursor(0, 0);
    lcd.print(F(" AUTO --> Bollitura "));
  }
  
  //lcd.setCursor(1,1);
  PrintTemp(1, 1, Temp, 2);
  
  lcd.setCursor(1, 2);
  lcd.print(F("PWM="));    //Display output%
  FormatNumeri(Heat, -1);
  //if (Heat<100 && Heat>=10)LCDSpace(1); 
  //if (Heat<10)LCDSpace(2); 
  lcd.print(Heat,0);       //Display output%
  lcd.print(F("% ")); 
} 

void NoBoil() {
  lcd.setCursor(1, 2);
  LCDSpace(9);
}

void HopAdd(byte HopAdd) {
  lcd.setCursor(10, 0);
  lcd.print(F("Luppolo "));
  if(HopAdd < 10) LCDSpace(1);
  lcd.print(HopAdd + 1);
}

void LCD_Add() {
  lcd.setCursor(10, 0);
  lcd.print(F("Irish Moss"));
}

void Raffreddamento() {
  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print(F("RAFFREDDAMENTO"));
  LCD_Procedo();
}

void LCDWhirlpool() {
  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.print(F("Inizio WHIRLPOOL"));
  LCD_Procedo();
}

void Menu_3() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(F("MENU  CONFIGURAZIONE"));
}
void Menu_3_1() {
  lcd.setCursor(2, 1);
  //lcd.print(F("Parametri P.I.D."));
  lcd.print(F("   PID -- PWM   "));

  LCDClear(2);

  LCD_xGEO();
}
void Menu_3_1_x(byte i) {
  display_lcd (1, 2, PIDName[i], 0);
  LCD_QQxO();
}  

void PidSet(int pidSet, byte i) {
  if (i == 0) {
    lcd.setCursor(10, 2);
    
    if (pidSet == 0) lcd.print(F("Elettrico"));
    else             lcd.print(F("      Gas"));
  } else { 
    lcd.setCursor(13, 2);
    LCDSpace(1);
  
    if (i > 0 && i <= 6) {
      if (i < 6) LCDSpace(1);
    
      if (i  < 4)           pidSet = pidSet - 100;
      if (i == 4 || i == 5) pidSet = pidSet * 250;
    
      FormatNumeri(pidSet, 0);
    }
  
    if(i >= 7){
      float OffSet = pidSet;
      if (i == 7) OffSet = (OffSet - 50.0) / 10.0;
      if (i == 8) OffSet =  OffSet / 10.0;
    
      FormatNumeri(OffSet, -2);
      lcd.print(OffSet);
      return;
    }
  
    lcd.print(pidSet);

    if (i == 6) lcd.print(F("%"));
  }
}

void Menu_3_2() {
  lcd.setCursor(2, 1);
  lcd.print(F("Parametri Unita'"));
  
  LCDClear(2);
  
  LCD_SGEO();
}

void Menu_3_2_x(byte i) {
  display_lcd(1, 2, unitName[i], 0);
  LCD_QQxO();
}

void UnitSet(byte unitSet, byte i) {
  lcd.setCursor(12, 2);
  
  switch(i) {
   
    case(0):// Scala Temp
      LCDSpace(6);
      lcd.write((byte)0);  
      break;
      
    case(1)://Sensore
      if (unitSet == 0) lcd.print(F("Interno"));
      else              lcd.print(F("Esterno"));
      break;
      
    case( 2):
    case( 3): // Temperatura di Ebollizione
    case(10):
    case(11): //Temperatura Fermo Pompa
      LCDSpace(3);
      PrintTemp(9, 9, unitSet, 0);
      break;

    case(4):// Durata Ciclo Pompa
      LCDSpace(4);
      FormatNumeri(unitSet, -2);
      //if (unitSet < 10) LCDSpace(1);
      lcd.print(unitSet);
      lcd.print(F("'"));
      break;
    
    case(5)://Durata Pausa Pompa
      LCDSpace(5);
      lcd.print(unitSet);
      lcd.print(F("'"));
      break;
    
 default:
     LCDSpace(4);
      if (unitSet == 0) lcd.print(F("Off"));
      if (unitSet == 1) lcd.print(F(" On"));
      break;
 
/* 
    case(10):
    case(11):
      LCDSpace(3);
      PrintTemp(9, 9, unitSet, 0);
      break;
*/      
    case(12)://Pipe
      if (unitSet == 0) lcd.print(F("Passivo"));
      else              lcd.print(F(" Attivo"));
      break;
 
    case(13):
    case(14):
    case(15):
      LCDSpace(5);
      if (unitSet == 0) lcd.print(F("No"));
      if (unitSet == 1) lcd.print(F("Si"));
      break;
      
    case(16): //Iodio
      if (unitSet == 0) {
        lcd.setCursor(12, 2);
        lcd.print(F("    Off"));
      } else CountDown(unitSet * 60, 12, 2, 1);
      break;   
      
    case(17):
      if (unitSet == 0) lcd.print(F("    Off"));
      if (unitSet == 1) lcd.print(F(" Freddo"));
      if (unitSet == 2) lcd.print(F("  Caldo"));
      break;
  }  
}


void Menu_3_3() {
  lcd.setCursor(2, 1);
  lcd.print(F("Set  Automazione"));
  LCD_SGEO();
}     
void Menu_3_3_x(byte Stage) {
  display_lcd(1, 2, stageName[Stage], 0);
  if (Stage==0 || Stage==6 || Stage==7) LCD_QQxO();
  else                                  LCD_QQSO();
}  

void StageSet(float Temp) {
  //lcd.setCursor(12,2);
  PrintTemp(12, 2, Temp, 2);
}

void TimeSet(int Time){
  lcd.setCursor(12,2);
  FormatNumeri(Time, 2);
  //if (Time<10)LCDSpace(5);
  //if (Time>=10 && Time<100)LCDSpace(4);
  //if (Time>=100)LCDSpace(3);
  lcd.print(Time);   
  lcd.print(F("'"));
  LCD_QQxO();
}


void Menu_3_3_8() {
  lcd.setCursor(1, 2);
  lcd.print(F("Numero Luppoli  "));
  LCD_QQxO();
} 
void NumHops(byte SetNumHops) {
  lcd.setCursor(17, 2);
  if (SetNumHops < 10) LCDSpace(1);
  lcd.print(SetNumHops);   
}

void Menu_3_3_9() {
  lcd.setCursor(1, 2);
  lcd.print(F("Bollitura     "));
  LCD_QQxO();
} 

void Menu_3_3_10(byte SetHop){
  lcd.setCursor(1,2);
  lcd.print(F("Luppolo ("));
  FormatNumeri(SetHop, -2);
  //if (SetHop<10)LCDSpace(1);
  lcd.print(SetHop);
  lcd.print(F(")  "));
  LCD_QQxO();
} 

void TimeHops(int Time){
  lcd.setCursor(15,2);
  FormatNumeri(Time, -1);
  //if (Time<10)LCDSpace(2);
  //if (Time>=10 && Time<100)LCDSpace(1);
  lcd.print(Time);   
  lcd.print(F("'"));
}



void Menu_3_4() {
  lcd.setCursor(2, 1);
  lcd.print(F("Gestione Ricette"));
  
  LCDClear(2);

  LCD_SGEO();
}     
void Menu_3_4_1() {
  lcd.setCursor(2, 2);
  lcd.print(F(" Carica Ricetta "));
  LCD_xGEO();
}
void Menu_3_4_2() {
  lcd.setCursor(2, 2);
  lcd.print(F(" Salva  Ricetta "));
  LCD_SGEO();
}
void Menu_3_4_3() {
  lcd.setCursor(2, 2);
  lcd.print(F("Cancella Ricetta"));
  LCD_SGEO();
}
void Menu_3_4_4() {
  lcd.setCursor(2, 2);
  lcd.print(F("Inizializzazione"));
  LCD_SxEO();
}

void NoRecipe() {
  LCDClear(2);  
  
  Buzzer(3, 50);

  lcd.setCursor(1, 3);
  lcd.print(F(" NESSUNA  RICETTA  "));

  delay(1500);
}

void Ricetta(byte numRicetta, byte Tipo){
  //Tipo 0 = Load
  //Tipo 1 = Save
  
  lcd.setCursor(2, 2);
  if (numRicetta < 10) lcd.print(F("0"));
  lcd.print(numRicetta);
  lcd.print(F(" > "));
  
  if (Tipo == 0) LCD_Procedo();
  else{
    lcd.setCursor(1, 3);
    lcd.print(F("SU* *GIU  Ind.  Ok"));
  }
}

void LCD_NomeRicetta(byte pos, byte Lettera) {
  lcd.setCursor(pos + 7, 2);
  lcd.print((char)Lettera);
}

void LeggoRicetta(byte Ricetta) {
  LCDClear(2);

  Buzzer(2, 35);
  
  lcd.setCursor(1,3);
  lcd.print(F(" Carico Ricetta... "));
  delay(1500);

  lcd.setCursor(1, 3);
  lcd.print(F(" Ric. "));
  if (Ricetta < 10) lcd.print(F("0"));
  lcd.print(Ricetta);
  lcd.print(F(" Caricata "));
  delay(1500);
}


void SalvataggioRicetta(byte Ricetta) {
  lcd.setCursor(1, 2);
  lcd.print(F("Salvo  Ricetta "));
  if (Ricetta < 10) lcd.print(F("0"));
  lcd.print(Ricetta);
  lcd.print(F("?"));
  
  LCD_Procedo();
}

void SalvaRicetta() {
  LCDClear(2);

  Buzzer(5, 35);

  lcd.setCursor(1, 3);
  lcd.print(F("  Salvataggio...  "));
  delay(1500);

  lcd.setCursor(1,3);
  lcd.print(F(" Ricetta  Salvata  "));
  delay(1500);  
}

void CancelloRicetta(byte Ricetta) {
  lcd.setCursor(0, 2);
  lcd.print(F("Cancello Ricetta "));
  if (Ricetta < 10) lcd.print(F("0"));
  lcd.print(Ricetta);
  LCDSpace(1);
  
  LCD_Procedo();
}

void Cancellazione(byte Ricetta) {
  LCDClear(2);

  Buzzer(2, 35);

  lcd.setCursor(1, 3);
  lcd.print(F("Cancellazione...  "));
  delay(1500);
  
  lcd.setCursor(1, 3);
  lcd.print(F("Ricetta Cancellata"));
  delay(1500);
}

void Inizializzazione() {
  lcd.setCursor(2, 2);
  lcd.print(F("Inizializzazione"));
  
  LCD_Procedo();
}
void Inizializza() {
  LCDClear(2);
  
  lcd.setCursor(1, 3);
  lcd.print(F("Inizializzo EEPROM"));
  delay(1500);
  
  Buzzer(3, 75);

  lcd.setCursor(1, 3);
  lcd.print(F("  EEPROM  pronta  "));
  delay(1500);
}

void MemoriaPiena() {
  LCDClear(2);

  Buzzer(3, 125);

  lcd.setCursor(1, 3);
  lcd.print(F("    ATTENZIONE    "));
  delay(1500);

  lcd.setCursor(1, 3);
  lcd.print(F("  MEMORIA  PIENA  "));
  delay(2000);
}


void Menu_3_5() {
  lcd.setCursor(2,1);
  lcd.print(F(" Riconoscimenti "));
  LCD_SxEO();
}     

void Credits() {
  lcd.clear();
  
  Intestazione();
  delay(1500);
  
  display_lcd(6, 2,  "Codice :",  750);
  display_lcd(5, 3, "S.Mathison", 2000);
  display_lcd(5, 3, " M.Wilson ", 1750);
  display_lcd(6, 3, " M.Nevi ", 1750);
  
  Clear_2_3();
  
  display_lcd(3, 2, "PCB  &  Tests:", 750);
  display_lcd(3, 3, "  Daniel Xan  ", 999);
  display_lcd(3, 3, "Savio Thechnic", 999);
  display_lcd(3, 3, "   A. Tidei   ", 999);
  display_lcd(4, 3, "D. Arzarello", 999);
  display_lcd(4, 3, "L. DiMichele", 999);
  
  Clear_2_3();    
  
  display_lcd(4, 2, "Traduzioni:",    750);
  display_lcd(4, 3, "A. Moiseyev",    999);//Russo
  display_lcd(4, 3, "A. Mondejar",    999);//Spagnolo
  display_lcd(4, 3, "C.M. Macedo",    999);//Portoghese 20x4
  display_lcd(3, 3, "F.A. Oliveira",  999);//Portugues 16x2
  display_lcd(3,3,"G.  Santaella",999);//Portuguese 20x4 (new)
  display_lcd(3,3,"  E. Eduard  ",999);//Norvegese  20x4 (new)
  
  Clear_2_3(); 
}



#if TestMemoria == true
void Menu_4() {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print(F("TEST DELLA RAM"));
}
void Menu_4_1() {
    lcd.setCursor(3, 1);
    lcd.print(F("Memoria Libera"));
    lcd.setCursor(6, 2);

    if (freeRam() < 100 && freeRam() >= 10) LCDSpace(1);
    if (freeRam() <  10)                    LCDSpace(2);
    lcd.print(freeRam());
    LCDSpace(2);
    lcd.print(F("byte"));
  
    LCDClear(3);
    delay(3500);
    lcd.clear();
}
#endif

void Pause_Stage(float Temp, int Time) {
  PauseScreen();
  
  lcd.setCursor(1, 1);
  lcd.print(F("     In Pausa     " ));
  
  //lcd.setCursor(7,0);
  PrintTemp(7, 0, Temp, 2);
  
  Watch (Time);
  
  lcd.setCursor(1, 3);
  lcd.print(F("---  ---  Esci ---"));
}

void prompt_for_water () {
  lcd.setCursor(2, 1);
  lcd.print(F("Acqua  Aggiunta?"));
  LCD_Procedo();
}

void Resume() {
  lcd.setCursor(2, 1);
  lcd.print(F("Riprendi  Cotta?"));
  LCD_Procedo();
}

void PausaPompa(float Temp, int Time) {
  //lcd.setCursor(1,1);
  PrintTemp(1, 1, Temp, 2);

  CntDwn(Time);
  
  lcd.setCursor(1, 3);
  lcd.print(F(" - Pausa  Pompa - "));
}

void Iodine(float Temp, int Time) {
  PauseScreen();
  
  lcd.setCursor(1,1);
  lcd.print(F("    TEST IODIO    "));
  
  //lcd.setCursor(7,0);
  PrintTemp(7, 0, Temp, 2);
  
  Watch (Time);
  
  lcd.setCursor(1, 3);
  lcd.print(F("---  ---   Ok  ---"));
}

void End() {
  lcd.clear();

  Buzzer(1, 3000);
  
  lcd.setCursor(4, 1);
  lcd.print(F("La tua birra"));
  lcd.setCursor(5, 2);
  lcd.print(F("e' pronta!"));
  
  delay(2500);
}

void PumpPrime() {
  lcd.setCursor(4, 1);
  lcd.print(F("Attivo Pompa"));  // priming the pump
}  

void ledHeatON() {
  lcd.setCursor(0, 2);
  lcd.write(6);
}

void ledHeatStatus(boolean mheat) {
  lcd.setCursor(0, 2);
  if (mheat) lcd.write(5);
  else       LCDSpace(1);
}

void ledPumpON() {
  lcd.setCursor(19, 2);
  lcd.write(4);
}

void ledPumpStatus(boolean mpump) {
  lcd.setCursor(19, 2);
  if (mpump)lcd.write(3);
  else      LCDSpace(1);
}

void ArdBir() {
  #if StartSprite == true
    Presentazione(2, 1);
  #endif
  
  #if Sprite == true
    ArdBir1(6, 1);
  #endif
}

void PartenzaRitardata() {
  Clear_2_3();
  lcd.setCursor(2, 1);
  lcd.print(F("Ritardo  Inizio?"));
  lcd.setCursor(14, 3);
  lcd.print(F("No Si"));
}

void ImpostaTempo(unsigned long Time) {
  lcd.setCursor(2, 1);
  lcd.print(F("Imposta  Ritardo"));
 
  CountDown(Time * 60, 6, 2, 2);
  
  LCD_SGEO();
}

void StartDelay(unsigned long Tempo) {
  lcd.setCursor(2, 1);
  lcd.print(F("   Inizio tra   "));
  CountDown(Tempo, 6, 2, 2);
}

void TemperaturaRaggiunta() {
  LCDClear(2);
  lcd.setCursor(3, 2);
  lcd.print(F("Temp.Raggiunta"));

  lcd.setCursor(1, 3);
  lcd.print(F("Procedo->  Ok  ---"));
}

void ImpostaWhirlpool(unsigned long Time) {
  lcd.setCursor(2, 1);
  lcd.print(F("Durata Whirlpool"));
 
  CountDown(Time * 60, 6, 2, 2);
  
  LCD_SGEO();
}

