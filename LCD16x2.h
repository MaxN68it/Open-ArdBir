char *PIDName[]   ={"Kostante  P", "Kostante  I", "Kostante  D", "Finestra ms", "Uso in Boll", "Calibraz.  "};
char *stageName[] ={"Mash In   ", "Fitasi    ", "Glucanasi ", "Proteasi  ", "B-Amilasi ", "A-Amilasi ", "Mash Out  ", "Bollitura "};
char *unitName[]  ={"Scala Gradi  ", "Sensore  ", "Ebollizione ", "Ebollizione ", "Ciclo Pompa  ", "Pausa Pompa   ", "In Bollitura ", "Fermo Pompa "};

byte Ciclo=0;

void Versione(byte locX, byte locY){
  lcd.setCursor(locX, locY);
  lcd.print(Version);
  lcd.write(7);
}

void Clear_2_3(){
  lcd.setCursor(0,1);
  Riga16();
}

void Intestazione(){  
  lcd.setCursor(0,0);
  lcd.print(F("Open ArdBir"));
  lcd.write(6);
  lcd.print(F("2014"));
}

void lcd_OkEsci(){
  lcd.setCursor(0,1);
  lcd.print(F(" --  --  Ok Esci"));
}


void LCD_Default(float Temp_Now){
  Intestazione();
  lcd.setCursor(0,1);
  Riga1();
  if (Temp_Now>=100)lcd.print(Temp_Now,1);
  else lcd.print(Temp_Now);
  Gradi();
  Riga1;
  Versione(7,1);
}

void Menu_1(){
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print(F("  MODO MANUALE  "));
  delay(750);
}    
void Manuale(float Set, float Temp, float TempBoil){    
  lcd.setCursor(0,0);
  lcd.print(F("MANUALE   "));
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
  }    


void Menu_2(){
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print(F("MODO  AUTOMATICO"));
}
void AddMalt(){
  lcd.setCursor(0,0);
  lcd.print(F(" Aggiungi Malto "));
  lcd_OkEsci();
//  lcd.setCursor(0,1);
//  lcd.print(F(" --  --  Ok Esci"));
  Buzzer(1, 1000);
}
void Stage(byte Stage, float Set, float Temp){
  lcd.setCursor(0,0);
  lcd.print(stageName[Stage]);
  
  lcd.setCursor(1,1);
  if (Temp>=100)lcd.print(Temp,1);
  else lcd.print(Temp);
  Gradi();
  lcd.setCursor(10,0);
  if (Set>=100)lcd.print(Set,1);
  else lcd.print(Set);
  lcd.write(2);
}
void RemoveMalt(){
  lcd.setCursor(0,0);
  lcd.print(F(" Rimuovi  Malto "));
  lcd_OkEsci();
//  lcd.setCursor(0,1);
//  lcd.print(F(" --  --  Ok Esci"));
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
    lcd.print(F("Bollitura "));
  }
  
  lcd.setCursor(1,1);
  lcd.print(F("A="));    //Display output%
  if (Heat<100)Riga1(); //added space as place holder for <100
  lcd.print(Heat,0); //Display output%
  lcd.print(F("% ")); 
  lcd.setCursor(10,0);
  if (Temp>=100)lcd.print(Temp,1);
  else lcd.print(Temp);
  Gradi();
} 
void HopAdd(float Temp, byte HopAdd){
  lcd.setCursor(0,0);
  lcd.print(F("Luppolo"));
  if(HopAdd<9)Riga1();
  lcd.print(HopAdd+1);
  //Riga1(); non so se ci va nella precedente nn c'ï¿½
  lcd.setCursor(1,1);
  Riga5();
  Riga2();  
}

void Menu_3(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F(" CONFIGURAZIONE "));
  delay(750);
}
void Menu_3_1(){
  lcd.setCursor(0,0);
  lcd.print(F("PARAMETRI P.I.D."));
}     
void Menu_3_1_x(byte i){
  lcd.setCursor(0,1);
  lcd.print(PIDName[i]);
}  
void PidSet(int pidSet, byte i){
  if(i==4||i==5)lcd.setCursor(11,1);
  else lcd.setCursor(12,1);
   
  if (i<3||i==4){
    if (pidSet<=-10 && pidSet>-100 )Riga1();
    if (pidSet<0 && pidSet>-10)Riga2();
    if (pidSet<10 && pidSet>=0)Riga3();
    if (pidSet>=10 && pidSet<100)Riga2();
    if (pidSet>=100)Riga1();  
    lcd.print(pidSet);
    if(i==4)lcd.print(F("%"));
  }
  if(i==3){  
    if (pidSet<1000)Riga1();
    lcd.print(pidSet);
  }
  
  if(i==5){
    float OffSet=pidSet/10.0;
    if (OffSet>=0)Riga1();
    lcd.print(OffSet);
    Riga1();
  }
}


void Menu_3_2(){
  lcd.setCursor(0,0);
  lcd.print(F("PARAMETRI UNITA'"));
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
      if (unitSet==0)lcd.print(F("Interno"));
      else lcd.print(F("Esterno"));
      break;
      
    case(2):// Temperatura di Ebollizione Â°C
      lcd.setCursor(12,1);
      if (unitSet<100)Riga1();
        lcd.print(unitSet);
        lcd.write(8);
      break;
    
    case(3):// Temperatura di Ebollizione Â°F
      lcd.setCursor(12,1);
      if (unitSet<100)Riga1();
        lcd.print(unitSet);
        lcd.write(1);
      break;
      
    case(4):// Durata Ciclo Pompa
      lcd.setCursor(13,1);
      if (unitSet<10)Riga1();
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
      if (unitSet<100)Riga1();
      lcd.print(unitSet);
      Gradi();
  }  
}


void Menu_3_3(){
  lcd.setCursor(0,0);
  lcd.print(F("SETT.AUTOMAZIONE"));
}     
void Menu_3_3_x(byte Stage){
  lcd.setCursor(0,1);
  lcd.print(stageName[Stage]);
}  
void StageSet(float Temp){
  lcd.setCursor(9,1);
  if (Temp<10)Riga2();
  if (Temp<100)Riga1();
  lcd.print(Temp);
  Gradi();
}

void TimeSet(int Time){
  lcd.setCursor(9,1);
  if (Time<10)Riga5();
  if (Time>=10 && Time<100)Riga4();
  if (Time>=100)Riga3();
  lcd.print(Time);   
  lcd.print(F("'"));
}


void Menu_3_3_8(){
  lcd.setCursor(0,1);
  lcd.print(F("Num. Luppoli  "));
} 
void NumHops(byte SetNumHops){
  lcd.setCursor(14,1);
  if(SetNumHops<10)Riga1();
  lcd.print(SetNumHops);   
}

void Menu_3_3_9(){
  lcd.setCursor(0,1);
  lcd.print(F("Bollitura   "));
} 

void Menu_3_3_10(byte SetHop){
  lcd.setCursor(0,1);
  lcd.print(F("Luppolo  "));
  if(SetHop<10)Riga1();
  lcd.print(SetHop);

  
} 
void TimeHops(int Time){
  lcd.setCursor(12,1);
  if (Time<10)Riga2();
  if (Time>=10 && Time<100)Riga1();
  lcd.print(Time);   
  lcd.print(F("'"));
}

void Menu_3_4(){
  lcd.setCursor(0,0);
  lcd.print(F("GESTIONE RICETTE"));
  Clear_2_3();
}     
void Menu_3_4_1(){
  lcd.setCursor(0,1);
  lcd.print(F(" Carica Ricetta "));
}
void Menu_3_4_2(){
  lcd.setCursor(0,1);
  lcd.print(F(" Salva  Ricetta "));
}
void Menu_3_4_3(){
  lcd.setCursor(0,1);
  lcd.print(F("Cancella Ricetta"));
}
void Menu_3_4_4(){
  lcd.setCursor(0,1);
  lcd.print(F("Inizializzazione"));
}

void CaricaRicetta(byte Ricetta){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("RICETTA "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  lcd.setCursor(0,1);
  lcd.print(F("Carico?->OK Esci"));
}

void NoRecipe(){
  lcd.setCursor(0,1);
  lcd.print(F("NESSUNA  RICETTA"));
  Buzzer(3,50);
  delay(2500);
}

void LeggoRicetta(byte Ricetta){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F(" In caricamento "));
  lcd.setCursor(0,1);
  lcd.print(F("   Ricetta "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  delay(1500);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("   Ricetta "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  lcd.setCursor(0,1);
  lcd.print(F("Ricetta caricata"));
  Menu_3_4();
}


void SalvataggioRicetta(byte Ricetta){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("   RICETTA "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);lcd.setCursor(1,1);
  lcd.print(F("Salvo?->Ok Esci"));
}

void SalvaRicetta(){
  Buzzer(5,35);
  lcd.setCursor(0,1);
  lcd.print(F("Salvataggio...  "));
  delay(1500);
  //lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(F("Ricetta  Salvata"));
  delay(2000);
  Menu_3_4();
}

void CancelloRicetta(byte Ricetta){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("   RICETTA "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  lcd.setCursor(0,1);
  lcd.print(F("Elimino? OK ESCI"));
}

void Cancellazione(byte Ricetta){
  lcd.setCursor(0,0);
  lcd.print(F(" Cancellazione  "));
  lcd.setCursor(0,1);
  lcd.print(F("   Ricetta "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  Riga3();
  delay(1500);
  lcd.setCursor(0,0);
  lcd.print(F("   RICETTA "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  Riga3();
  lcd.setCursor(0,1);
  lcd.print(F("   CANCELLATA   "));
  delay(1500);
  Menu_3_4();
}


void Inizializzazione(){
  lcd.setCursor(0,0);
  lcd.print(F("Inizializzazione"));
  lcd.setCursor(0,1);
  lcd.print(F("Procedo?->Si  No"));
}
void Inizializza(){
lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("  ATTENDERE...  "));
  Buzzer(2,35);
  lcd.setCursor(0,1);
  lcd.print(F("Inizializzo  RAM"));
  delay(2500);
  Menu_3_4();
}

void MemoriaPiena(){
lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("   ATTENZIONE   "));
  Buzzer(5,35);
  lcd.setCursor(0,1);
  lcd.print(F(" MEMORIA  PIENA "));
  delay(3500);
}

void Menu_3_5(){
  lcd.setCursor(0,0);
  lcd.print(F(" RICONOSCIMENTI "));
}     

void Credits(){
  lcd.clear();
  Intestazione();
  Versione(4,1);
  delay(2000);

  lcd.clear();
  Intestazione();
  lcd.setCursor(0,0);
  lcd.print(F("Idea Originale: "));
  lcd.setCursor(0,1);
  delay(1000);
  lcd.print(F("Stephen Mathison"));
  delay(1500);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Modifica FW:    "));
  lcd.setCursor(0,1);
  delay(1000);
  lcd.print(F("     Mike Wilson"));
  delay(1250);
  lcd.setCursor(0,1);
  lcd.print(F("    Massimo Nevi"));
  delay(1250);
  
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print(F("RIPROGETTAZIONE "));
  lcd.setCursor(0,1);
  lcd.print(F("    ITALIANA    "));
  delay(1500);
  
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print(F("Disegno PCB:    "));
  lcd.setCursor(0,1);
  delay(750);
  lcd.print(F("       DanielXan"));
  delay(800);

  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Collaudo PCB:   "));
  lcd.setCursor(0,1);
  delay(750);
  lcd.print(F("   SavioThechnic"));
  delay(800);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Collaudo FW:    "));
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
  if (freeRam()<1000&&freeRam()>=100)Riga1();
  if (freeRam()<100&&freeRam()>=10)Riga2();
  if (freeRam()<10)Riga3();
  lcd.print(freeRam());
  Riga1();
  lcd.print(F("byte"));
  
  delay(3500);
  lcd.clear();
}
*/
void Pause_Stage(float Temp, int Time){
  lcd.setCursor(0,0);
  lcd.print(F("--- In Pausa ---" ));
  
  lcd.setCursor(0,1);
  if (Temp<100)Riga1();
  if (Temp<10)Riga2();
  lcd.print(Temp);
  Gradi();
  
  CountDown(Time,8,1,1);
}

void prompt_for_water (){
  lcd.setCursor(0,0);
  lcd.print(F("Acqua  aggiunta?"));
  lcd_OkEsci();
//  lcd.setCursor(0,1);
//  lcd.print(F(" --  --  Ok Esci"));
  Buzzer(1,750);
}

void Resume(){
  lcd.setCursor(0,0);
  lcd.print(F("Riprendi  Cotta?"));
  lcd_OkEsci();
//  lcd.setCursor(0,1);
//  lcd.print(F(" --  --  Si  No "));
  Buzzer(1,750);
}

void CntDwn(int Time){
  CountDown(Time,8,1,1);
}

void PausaPompa(float Temp, int Time){
  if (Ciclo>=275){
    Buzzer(2,35);
    Ciclo=0;
  }
  
  lcd.setCursor(1,1);
  if (Temp>=100)lcd.print(Temp,1);
  else lcd.print(Temp);
  Gradi();
  
  CountDown(Time,8,1,1);
  
  lcd.setCursor(0,0);
  lcd.print(F("- Pausa  Pompa -"));
 
  Ciclo++;
}

void Iodine(float Temp, int Time){
  lcd.setCursor(0,0);
  Riga2();
  if (Temp>=100)lcd.print(Temp,1);
  else lcd.print(Temp);
  Gradi();
  Riga1();
  CountDown(Time,9,0,1);
  
  lcd.setCursor(1,1);
  if (Ciclo<50)lcd.print(F(" IODIO"));
  else lcd.print(F(" TEST "));
  
  lcd.setCursor(7,1);
  lcd.print(F(" - OK --"));
  
  Ciclo++;
  if (Ciclo>100)Ciclo=0;
}

void End(){
  Buzzer(3,250);
  lcd.setCursor(0,0);
  lcd.print(F("  La tua birra  "));
  lcd.setCursor(0,1);
  lcd.print(F("   e' pronta!   "));
  
  Buzzer(1,3000);
  delay(2500);
}

void PumpPrime(){
  lcd.setCursor(0,1);
  lcd.print(F("  Attivo Pompa  "));  // priming the pump
}  

void ledHeatON(){
  lcd.setCursor(0,1);
  lcd.write(5);
}

void ledHeatOFF(){
  lcd.setCursor(0,1);
  lcd.write(4);
}

void ledHeatStatus(){
  lcd.setCursor(0,1);
  Riga1();
}

void ledPumpON(){
  lcd.setCursor(15,1);
  lcd.write(3);
}
  
void ledPumpOFF(){
  lcd.setCursor(15,1);
  Riga1();
}

void Presentazione(){
  Presentazione(0,0);
}

void ArdBir(){
  ArdBir1(0,0);
}
