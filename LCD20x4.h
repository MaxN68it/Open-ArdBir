char *PIDName[]   ={" Kostante   P ", " Kostante   I ", " Kostante   D ", " Finestra  ms ", "Uso Resistenza", " Calibrazione "};
char *stageName[] ={" Mash In   ", " Fitasi    ", " Glucanasi ", " Proteasi  ", " B-Amilasi ", " A-Amilasi ", " Mash Out  ", " Bollitura "};
char *unitName[]  ={"  Scala   Gradi", "  Sensore  ", "  Ebollizione ", "  Ebollizione ", "  Ciclo  Pompa ", "  Pausa  Pompa  ", " Pompa on Boil  ", " Fermo  Pompa  "};

byte Ciclo=0;

void Versione(byte locX, byte locY){
  lcd.setCursor(locX, locY);
  //lcd.print(F("Ver"));
  lcd.print(Version);
  lcd.write(7);
}

void Intestazione(){  
  lcd.setCursor(0,0);
  lcd.print(F("Open ArdBir"));
  lcd.setCursor(12,0);
  lcd.write(6);
  if (Ciclo<100){
    lcd.print(F("2012-14"));
  }else{
    Riga1();
    Versione(12,0);
  }
  Ciclo++;
  if (Ciclo>250)Ciclo=0;
}
void Clear_2(){
  lcd.setCursor(0,2);
  Riga20();
}

void Clear_2_3(){
  lcd.setCursor(0,2);
  Riga20();
  lcd.setCursor(0,3);
  Riga20();
}

void lcd_QQSO(){
  lcd.setCursor(0,3);
  lcd.print(F(" SU**GIU'  salta ok "));
}
void lcd_QQxO(){
  lcd.setCursor(0,3);
  lcd.print(F(" SU**GIU'   ---  ok "));
}
void lcd_SGEO(){
  lcd.setCursor(0,3);
  lcd.print(F(" su  giu'  esci  ok "));
}
void lcd_xGEO(){
  lcd.setCursor(0,3);
  lcd.print(F(" --- giu'  esci  ok "));
}

void lcd_SxEO(){
  lcd.setCursor(0,3);
  lcd.print(F(" su   ---  esci  ok "));
}

void lcd_OkEsci(){
  lcd.setCursor(0,3);
  lcd.print(F(" ---  ---  Ok  Esci "));
}

void LCD_Default(float Temp_Now){
  Intestazione();
  lcd.setCursor(5,1);
  Riga2();
  if (Temp_Now>=100)lcd.print(Temp_Now,1);
  else lcd.print(Temp_Now);
  Gradi();
  Clear_2();
  lcd.setCursor(0,3);
  lcd.print(F(" ---  MAN AUTO SETUP"));
}

void Menu_1(){
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print(F(" MODALITA'  MANUALE "));
 }    
 
void Manuale(float Set, float Temp,float TempBoil){    
  lcd.setCursor(1,1);
  if (Temp<100)Riga1();
  lcd.print(Temp);
  Gradi();
  
  lcd.setCursor(12,1);
  if (Set<100)Riga1();
  lcd.print(Set);
  lcd.write(2);
  
  lcd.setCursor(1,2);
  Riga8();
  lcd.setCursor(0,3);
  lcd.print(F(" SU  GIU'  Res  Pmp "));
}    


void Menu_2(){
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print(F("MODALITA' AUTOMATICA"));
}
void AddMalt(){
  lcd.setCursor(0,2);
  lcd.print(F("  Aggiungi  Malto   "));
  lcd.setCursor(0,3);
  lcd_OkEsci();
  Buzzer(1, 1000);
}
void Stage(byte Stage, float Set, float Temp){
  lcd.setCursor(0,0);
  lcd.print(F(" AUTO -->"));
  lcd.print(stageName[Stage]);
  
  lcd.setCursor(1,1);
  if (Temp<100)Riga1();
  lcd.print(Temp);
  Gradi();
  lcd.setCursor(8,1);
  Riga4();
  if (Set<100)Riga1();
  lcd.print(Set);
  lcd.write(2);
    
  lcd.setCursor(1,2);
  Riga8();
  Riga1();
  lcd.setCursor(18,2);
  Riga1();
  
  lcd.setCursor(0,3);
  lcd.print(F(" SU  GIU' Pausa --- "));
}
void RemoveMalt(){
  lcd.setCursor(0,2);
  lcd.print(F("   Rimuovi  Malto   "));
  //Clear2();
  lcd.setCursor(0,3);
  lcd_OkEsci();
  Buzzer(1,1500);
 }
 
void Temp_Wait(float Temp){
  lcd.setCursor(1,1);
  if (Temp<100)Riga1();
  lcd.print(Temp);
  Gradi();
}

void Boil(float Heat, float Temp, byte Tipo){
  if (Tipo==1){
    lcd.setCursor(0,0);
    lcd.print(F(" AUTO --> Bollitura "));
  }
  
  lcd.setCursor(1,1);
  if(Temp<100)Riga1();
  lcd.print(Temp);
  Gradi();
  Riga4();
  
  lcd.setCursor(1,2);
  lcd.print(F("Res="));    //Display output%
  if (Heat<100)Riga1(); //added space as place holder for <100
  lcd.print(Heat,0); //Display output%
  lcd.print(F("% ")); 
} 
void HopAdd(float Temp, byte HopAdd){
  lcd.setCursor(9,0);
  lcd.print(F(" Luppolo "));
  if(HopAdd<10)Riga1();
  lcd.print(HopAdd+1);
}

void Menu_3(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("MENU  CONFIGURAZIONE"));
}
void Menu_3_1(){
  lcd.setCursor(0,1);
  lcd.print(F("  Parametri P.I.D.  "));
  Clear_2();
  lcd.setCursor(0,3);
  lcd_xGEO();
}     
void Menu_3_1_x(byte i){
  lcd.setCursor(0,2);
  lcd.print(PIDName[i]);
  lcd_QQxO();
}  

void PidSet(int pidSet, byte i){
  if(i==5){
    lcd.setCursor(14,2);
    float OffSet=pidSet/10.0;
    if (OffSet>=0)Riga1();
    lcd.print(OffSet);
    Riga1();
  }
  else{ 
    lcd.setCursor(15,2);
    if (pidSet<=250){
      if (pidSet<=-10 && pidSet>-100 )Riga1();
      if (pidSet<0 && pidSet>-10)Riga2();
      if (pidSet<10 && pidSet>=0)Riga3();
      if (pidSet>=10 && pidSet<100)Riga2();
      if (pidSet>=100)Riga1();
    }
    else{ 
      if (pidSet<1000)Riga1();
    }
    lcd.print(pidSet);
    if(i==4)lcd.print(F("%"));
  }
}


void Menu_3_2(){
  lcd.setCursor(0,1);
  lcd.print(F("  Parametri Unita'  "));
  Clear_2();
  lcd.setCursor(0,3);
  lcd_SGEO();
}    

void Menu_3_2_x(byte i){
  lcd.setCursor(0,2);
  lcd.print(unitName[i]);
  lcd_QQxO();
}


void Menu_3_3(){
  lcd.setCursor(0,1);
  lcd.print(F(" Setta  Automazione "));
  lcd_SGEO();
}     
void Menu_3_3_x(byte Stage){
  lcd.setCursor(1,2);
  lcd.print(stageName[Stage]);
  if (Stage==0||Stage==5||Stage==6)lcd_QQxO();
  else lcd_QQSO();
}  

void Menu_3_3_8(){
  lcd.setCursor(1,2);
  lcd.print(F("Numero Luppoli   "));
  lcd_QQxO();
} 

void Menu_3_3_9(){
  lcd.setCursor(1,2);
  lcd.print(F("Bollitura    "));
  lcd_QQxO();
} 

void Menu_3_3_10(byte SetHop){
  lcd.setCursor(0,2);
  lcd.print(F(" Luppolo ("));
  if (SetHop<10);Riga1();
  lcd.print(SetHop);
  lcd.print(F(")  "));
  lcd_QQxO();
} 

void Menu_3_4(){
  lcd.setCursor(0,1);
  lcd.print(F("  Gestione Ricette  "));
  Clear_2();
  lcd_SGEO();
}     
void Menu_3_4_1(){
  lcd.setCursor(0,2);
  lcd.print(F("   Carica Ricetta   "));
  lcd_xGEO();
}
void Menu_3_4_2(){
  lcd.setCursor(0,2);
  lcd.print(F("   Salva  Ricetta   "));
  lcd_SGEO();
}
void Menu_3_4_3(){
  lcd.setCursor(0,2);
  lcd.print(F("  Cancella Ricetta  "));
  lcd_SGEO();
}
void Menu_3_4_4(){
  lcd.setCursor(0,2);
  lcd.print(F("  Inizializzazione  "));
  lcd_SxEO();
}

void NoRecipe(){
  Clear_2();  
  lcd.setCursor(0,3);
  lcd.print(F("  NESSUNA  RICETTA  "));
  Buzzer(3,50);
  delay(1500);
}

void CaricaRicetta(byte Ricetta){
  lcd.setCursor(0,2);
  lcd.print(F(" Carico  Ricetta "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  Riga1();
  lcd_OkEsci();
}
void LeggoRicetta(byte Ricetta){
  Clear_2();
  lcd.setCursor(0,3);
  lcd.print(F("Carico Ricetta...   "));
  delay(1500);
  
  lcd.setCursor(0,3);
  lcd.print(F("Ricetta "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  lcd.print(F("  Caricata"));
  delay(1500);
}

void SalvataggioRicetta(byte Ricetta){
  lcd.setCursor(0,2);
  lcd.print(F(" Salvo  Ricetta "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  lcd.print(F("? "));
  lcd_OkEsci();
}


void SalvaRicetta(){
  Clear_2();
  lcd.setCursor(0,3);
  lcd.print(F("   Salvataggio...   "));
  Buzzer(5,35);
  delay(2000);
  
  lcd.setCursor(0,3);
  lcd.print(F("  Ricetta  Salvata  "));
  delay(1500);  
}

void CancelloRicetta(byte Ricetta){
  lcd.setCursor(0,2);
  lcd.print(F("Cancello Ricetta "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  Riga1();
  lcd_OkEsci();
}

void Cancellazione(byte Ricetta){
  Clear_2();
  lcd.setCursor(0,3);
  lcd.print(F(" Cancellazione...   "));
  delay(1500);
  
  lcd.setCursor(0,3);
  lcd.print(F(" Ricetta Cancellata "));
  delay(1500);
}

void Inizializzazione(){
  lcd.setCursor(0,2);
  lcd.print(F("  Inizializzazione  "));
  lcd_OkEsci();
}
void Inizializza(){
  Clear_2();
  lcd.setCursor(0,3);
  lcd.print(F(" Inizializzo EEPROM "));
  Buzzer(2,35);
  delay(1500);
  
  lcd.setCursor(0,3);
  lcd.print(F("   EEPROM  pronta   "));
  delay(1500);
}

void MemoriaPiena(){
  Clear_2();
  lcd.setCursor(0,3);
  lcd.print(F("     ATTENZIONE     "));

  Buzzer(5,35);
  delay(1000);
  
  lcd.setCursor(0,3);
  lcd.print(F("   MEMORIA  PIENA   "));
  delay(2500);
}

void Menu_3_5(){
  lcd.setCursor(0,1);
  lcd.print(F("   Riconoscimenti   "));
  lcd_SxEO();
}     

/*
void Menu_4(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("   TEST DELLA RAM   "));
}
void Menu_4_1(){
  lcd.setCursor(0,1);
  lcd.print(F("   Memoria Libera   "));
  lcd.setCursor(5,2);
  if (freeRam()<1000&&freeRam()>=100)Riga1();
  if (freeRam()<100&&freeRam()>=10)Riga2();
  if (freeRam()<10)Riga3();
  lcd.print(freeRam());
  Riga2();
  lcd.print(F("byte"));
  
  lcd.setCursor(0,3);
  Riga20();
  delay(3500);
  lcd.clear();
}
*/

void Credits(){
  lcd.clear();
  Intestazione();
  Versione(4,1);
  delay(1500);

  lcd.clear();
  Intestazione();
  lcd.setCursor(0,2);
  lcd.print(F("  Idea  Originale:"));
  delay(1000);
  lcd.setCursor(0,3);
  lcd.print(F("  Stephen Mathison"));
  delay(1500);
  
  lcd.setCursor(0,2);
  lcd.print(F("    Modifica FW:  "));
  delay(1000);
  lcd.setCursor(0,3);
  lcd.print(F("    Mike  Wilson  "));
  delay(1250);
  lcd.setCursor(0,3);
  lcd.print(F("    Massimo Nevi  "));
  delay(1250);
  
  lcd.clear();

  lcd.setCursor(0,1);
  lcd.print(F("RIPROGETTAZIONE"));
  lcd.setCursor(0,2);
  lcd.print(F("          ITALIANA"));
  delay(1500);
  
  lcd.clear();
  
  Intestazione();
  
//  lcd.setCursor(0,2);
//  lcd.print(F("Disegno PCB: "));
//  delay(750);
  lcd.setCursor(0,3);
  lcd.print(F("DanielXan    "));
  delay(800);

//  lcd.setCursor(0,2);
//  lcd.print(F("Collaudo PCB:"));
//  delay(750);
  lcd.setCursor(0,3);
  lcd.print(F("SavioThechnic"));
  delay(800);

//  lcd.setCursor(0,2);
//  lcd.print(F("Collaudo FW: "));
//  delay(750);
  lcd.setCursor(0,3);
  lcd.print(F("A. Tidei     "));
  delay(800);
  lcd.setCursor(0,3);
  lcd.print(F("D. Arzarello "));
  delay(800);
  lcd.setCursor(0,3);
  lcd.print(F("L. Di Michele"));
  delay(800);
  
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
      if (unitSet==0)lcd.print(F("Interno  "));
      else lcd.print(F("Esterno  "));
      break;
      
    case(2):// Temperatura di Ebollizione Â°C
      lcd.setCursor(14,2);
      if (unitSet<100)Riga1();
        lcd.print(unitSet);
        lcd.write(8);
      break;
    
    case(3):// Temperatura di Ebollizione Â°F
      lcd.setCursor(14,2);
      if (unitSet<100)Riga1();
        lcd.print(unitSet);
        lcd.write(1);
      break;
      
    case(4):// Durata Ciclo Pompa
      lcd.setCursor(15,2);
      if (unitSet<10)Riga1();
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
      if (unitSet<100)Riga1();
      lcd.print(unitSet);
      Gradi();
  }  
}
 

void StageSet(float Temp){
  lcd.setCursor(11,2);
  if (Temp<10)Riga2();
  if (Temp<100)Riga1();
  lcd.print(Temp);  //** check -mdw
  Gradi();
}

void TimeSet(int Time){
  lcd.setCursor(11,2);
  if (Time<10)Riga5();
  if (Time>=10 && Time<100)Riga4();
  if (Time>=100)Riga3();
  lcd.print(Time);   
  lcd.print(F("'"));
}

void NumHops(byte SetNumHops){
  lcd.setCursor(16,2);
  if(SetNumHops<10)Riga1();
  lcd.print(SetNumHops);   
}

void TimeHops(int Time){
  lcd.setCursor(14,2);
  if (Time<10)Riga2();
  if (Time>=10 && Time<100)Riga1();
  lcd.print(Time);   
  lcd.print(F("'"));
}

void Pause_Stage(float Temp, int Time){
  lcd.setCursor(0,0);
  lcd.print(F("------        ------"));
  
  lcd.setCursor(0,1);
  if (Ciclo<100)Riga20();
  else lcd.print(F("      In Pausa      " ));
  
  lcd.setCursor(0,2);
  lcd.print(F("-----          -----"));
  
  lcd.setCursor(6,0);
  if (Temp<100)Riga1();
  if (Temp<10)Riga2();
  lcd.print(Temp);
  Gradi();
  
  CountDown(Time,6,2,2);
  
  lcd.setCursor(0,3);
  lcd.print(F(" ---  ---  Esci --- "));

  Ciclo++;
  if (Ciclo>250)Ciclo=0;
}

void prompt_for_water (){
  lcd.setCursor(0,1);
  lcd.print(F("  Acqua  aggiunta?  "));
  lcd_OkEsci();
  Buzzer(1,750);
}

void Resume(){
  lcd.setCursor(0,1);
  lcd.print(F("  Riprendi  Cotta?  "));
  lcd_OkEsci();
  Buzzer(1,750);
}

void CntDwn(int Time){
  CountDown(Time,11,2,2);
}

void PausaPompa(float Temp, int Time){
  if (Ciclo>=275){
    Buzzer(2,35);
    Ciclo=0;
  }
  
  lcd.setCursor(2,1);
  if (Temp<100)Riga1();
  lcd.print(Temp);
  Gradi();
  
  CountDown(Time,11,2,2);
  
  lcd.setCursor(0,3);
  lcd.print(F(" -- Pausa  Pompa -- "));
 
  Ciclo++;
}




void Iodine(float Temp, int Time){
  lcd.setCursor(0,0);
  lcd.print(F("------        ------"));
  
  lcd.setCursor(0,1);
  if (Ciclo<75)lcd.print(F("     TEST IODIO     " ));
  else lcd.print(F("     Test Iodio     " ));
  
  lcd.setCursor(1,2);
  lcd.print(F("----          ----"));
  
  lcd.setCursor(6,0);
  if (Temp<100)Riga1();
  if (Temp<10)Riga2();
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
  lcd.print(F("    La tua birra    "));
  lcd.setCursor(0,2);
  lcd.print(F("     e' pronta!     "));
  
  Buzzer(1,3000);
  delay(2500);
}

void PumpPrime(){
  lcd.setCursor(0,1);
  lcd.print(F("    Attivo Pompa    "));  // priming the pump
}  

void ledHeatON(){
  lcd.setCursor(0,2);
  lcd.write(5);
}

void ledHeatOFF(){
  lcd.setCursor(0,2);
  lcd.write(4);
}

void ledHeatStatus(){
  lcd.setCursor(0,2);
  Riga1();
}

void ledPumpON(){
  lcd.setCursor(19,2);
  lcd.write(3);
}
  
void ledPumpOFF(){
  lcd.setCursor(19,2);
  Riga1();
}

void Presentazione(){
  Presentazione(2,1);
}

void ArdBir(){
  ArdBir1(2,1);
}
