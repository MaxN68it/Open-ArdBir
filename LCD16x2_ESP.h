char *PIDName[]   ={"Konstante P", "Konstante I", "Konstante D", "Finestra ms", "PWM        ", "Calibracion "};
char *stageName[] ={"Mash In   ", "Fitasa    ", "Glucanasa ", "Proteasa  ", "bAmilasa  ", "aAmilasa1 ", "aAmilasa2 ", "Mash Out  ", "Ebullicion"};
char *unitName[]  ={"Escala     ", "Sensor     ", "Ebullicion ", "Ebullicion ", "Ciclo Bomba", "Pausa Bomba", "Bmb en Ebul", "Bomba Parada"};

byte HeatONOFF[8]    = {B00000, B01110, B01010, B01010, B01100, B01010, B01010, B00000};  // [5] HEAT symbol
byte RevHeatONOFF[8] = {B11111, B10001, B10101, B10101, B10011, B10101, B10101, B11111};  // [6] reverse HEAT symbol
byte Language[8]     = {B00000, B10111, B10101, B11101, B00000, B10001, B10101, B11111};  // [7] ESP symbol

//byte Ciclo=0;

void LCDSpace (byte Num){
  for(byte i=0; i<Num; i++){
    lcd.print(F(" "));
  }
}

void LCDClear(byte Riga){
  lcd.setCursor(0,Riga);
  LCDSpace(20);
}

void PrintTemp(float Temp){
  //LCDSpace(1);
  if (Temp<10.0)LCDSpace(1);
  //if (Temp>=10.0 && Temp<100.0)LCDSpace(1);
  if (Temp>=100.0)lcd.print(Temp,1);
  else lcd.print(Temp);
  //Gradi();
  lcd.write((byte)0);
}

void Clear_2_3(){
  LCDClear(1);
}

void Version(byte locX, byte locY){
  lcd.setCursor(locX, locY);
  LCDSpace(1);
  //lcd.print(Version16);
  lcd.print(F("2.6.63"));
  lcd.write(7);
}

void Intestazione(){  
  lcd.setCursor(0,0);
  lcd.print(F("Open ArdBir 2014"));
}

void LCD_OkEsci(){
  lcd.setCursor(8,1);
  lcd.print(F("Ok  No "));
}

void LCD_Procedo(){
  lcd.setCursor(1,1);
  lcd.print(F("Procedo? Ok No "));
}

/*
void LCD_Conferma(){
  lcd.setCursor(1,1);
  lcd.print(F("Procedo? Ok -- "));
}
*/

void LCD_Default(float Temp){
  Intestazione();
  lcd.setCursor(1,1);
  PrintTemp(Temp);
  Version(7,1);
}

void Menu_1(){
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print(F("  MODO  MANUAL  "));
  delay(750);
}    

void Manuale(float Set, float Temp, float TempBoil){    
  lcd.setCursor(0,0);
  lcd.print(F("MANUAL    "));
  
  if (Set>=TempBoil && Temp>=Set){ 
    PrintTemp(Temp);
  }else{
    if (Set>=100)lcd.print(Set,1);
    else lcd.print(Set);
    lcd.write(2);
    
    lcd.setCursor(1,1);
    PrintTemp(Temp);
  }
}    


void Menu_2(){
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print(F("MODO  AUTOMATICO"));
}
void AddMalt(){
  lcd.setCursor(0,0);
  lcd.print(F(" Agregar  Malta "));
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
  PrintTemp(Temp);
}

void SaltoStep(){
  lcd.setCursor(0,0);
  lcd.print(F("Siguiente  Paso?"));
  lcd.setCursor(8,1);
  lcd.print(F("   Si No "));
}

void RemoveMalt(){
  lcd.setCursor(0,0);
  lcd.print(F(" Eliminar Malta "));
  LCD_OkEsci();
  Buzzer(1, 1000);
}

void Temp_Wait(float Temp){
  lcd.setCursor(1,1);
  PrintTemp(Temp);
}

void Boil(float Heat, float Temp, byte Tipo){
  if (Tipo==1){
    lcd.setCursor(0,0);
    lcd.print(F("Ebullicion"));
  }
  
  lcd.setCursor(1,1);
  lcd.print(F("R="));    //Display output%
  if (Heat<100)LCDSpace(1); //added space as place holder for <100
  lcd.print(Heat,0); //Display output%
  lcd.print(F("% ")); 
  
  lcd.setCursor(10,0);
  PrintTemp(Temp);
} 

void NoBoil(){ 
}

void HopAdd(byte HopAdd){
  lcd.setCursor(0,0);
  lcd.print(F("Lupulo "));
  if(HopAdd<9)LCDSpace(1);
  lcd.print(HopAdd+1);
  lcd.setCursor(1,1);
  LCDSpace(6); 
}

void Menu_3(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F(" CONFIGURACION  "));
  delay(750);
}
void Menu_3_1(){
  lcd.setCursor(0,0);
  lcd.print(F(" PARAMETROS PID "));
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
  lcd.print(F(" PARAM.  UNIDAD "));
}     

void Menu_3_2_x(byte i){
  lcd.setCursor(0,1);
  lcd.print(unitName[i]);
}
void UnitSet(byte unitSet, byte i){
  
  switch(i){
     
    case(0):// Scala Temp
        lcd.setCursor(14,1);
        lcd.write((byte)0);  
        break;
      
    case(1)://Sensore
      lcd.setCursor(9,1);
      if (unitSet==0)lcd.print(F("Interno"));
      else lcd.print(F("Externo"));
      break;
      
    default: // Temperatura di Ebollizione 
      lcd.setCursor(12,1);
      if (unitSet<100)LCDSpace(1);
      lcd.print(unitSet);
      lcd.write((byte)0);
      break;
    /*
    case(3):// Temperatura di Ebollizione F
      lcd.setCursor(12,1);
      if (unitSet<100)LCDSpace(1);
      lcd.print(unitSet);
      lcd.write((byte)0);
      break;
    */  
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
      //Gradi();
      lcd.write((byte)0);
  }  
}


void Menu_3_3(){
  lcd.setCursor(0,0);
  lcd.print(F("SET AUTOMATIZAC."));
}     
void Menu_3_3_x(byte Stage){
  lcd.setCursor(0,1);
  lcd.print(stageName[Stage]);
}  
void StageSet(float Temp){
  lcd.setCursor(10,1);
  PrintTemp(Temp);
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
  lcd.print(F("Numero Lupulo "));
} 
void NumHops(byte SetNumHops){
  lcd.setCursor(14,1);
  if(SetNumHops<10)LCDSpace(1);
  lcd.print(SetNumHops);   
}

void Menu_3_3_9(){
  lcd.setCursor(0,1);
  lcd.print(F("Ebullicion  "));
} 

void Menu_3_3_10(byte SetHop){
  lcd.setCursor(0,1);
  lcd.print(F("Lupulo   "));
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
  lcd.print(F("GESTION  RECETAS"));
  LCDClear(1);
}     
void Menu_3_4_1(){
  lcd.setCursor(0,1);
  lcd.print(F(" Cargar  Receta "));
}
void Menu_3_4_2(){
  lcd.setCursor(0,1);
  lcd.print(F(" Salvar  Receta "));
}
void Menu_3_4_3(){
  lcd.setCursor(0,1);
  lcd.print(F("Eliminar  Receta"));
}
void Menu_3_4_4(){
  lcd.setCursor(0,1);
  lcd.print(F("Inizializacion  "));
}
/*
void CaricaRicetta(byte Ricetta){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F(" RECETA "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  LCD_Procedo();
}
*/
void NoRecipe(){
  lcd.setCursor(0,1);
  lcd.print(F(" NINGUNA RECETA "));
  Buzzer(3,50);
  delay(2500);
}

void Ricetta(byte numRicetta, byte Tipo){
  //Tipo 0 = Load
  //Tipo 1 = Save
  
  lcd.setCursor(1,1);
  if (numRicetta<10)lcd.print(F("0"));
  lcd.print(numRicetta);
  lcd.print(F("> "));
}

void LCD_NomeRicetta(byte pos, byte Lettera){
  lcd.setCursor(pos+5,1);
  lcd.print((char)Lettera);
}


void LeggoRicetta(byte Ricetta){
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print(F("   Receta  "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  lcd.setCursor(4,1);
  lcd.print(F("Cargata "));
  Menu_3_4();
}


void SalvataggioRicetta(byte Ricetta){
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print(F("Salvar Receta "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  LCD_Procedo();
}

void SalvaRicetta(){
  Buzzer(5,35);
  
  lcd.setCursor(0,1);
  lcd.print(F(" Receta Salvada "));
  delay(2000);
  Menu_3_4();
}

void CancelloRicetta(byte Ricetta){
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print(F(" Elim. Receta "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  LCD_Procedo();
}

void Cancellazione(byte Ricetta){
  lcd.clear();
  
  lcd.setCursor(3,0);
  lcd.print(F(" Receta "));

  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  LCDSpace(3);
  lcd.setCursor(3,1);
  lcd.print(F("CANCELADA "));
  delay(1500);
  Menu_3_4();
}


void Inizializzazione(){
  lcd.setCursor(0,0);
  lcd.print(F("     Iniciar    "));
  LCD_Procedo();
}
void Inizializza(){
  lcd.clear();
  
  lcd.setCursor(0,1);
  lcd.print(F("EEPROM preparada"));
  Buzzer(3,75);
  delay(1500);

  Menu_3_4();
}

void MemoriaPiena(){
  lcd.clear();
  
  lcd.setCursor(1,1);
  lcd.print(F("MEMORIA  LLENA"));
  Buzzer(3,125);
  delay(2500);
}

void Menu_3_5(){
  lcd.setCursor(0,0);
  lcd.print(F("    CREDITOS    "));
}     

void viewCredits(byte X, byte Y, const char* Testo, int Pausa){
  lcd.setCursor(X,Y);
  lcd.print (Testo);
  delay(Pausa);
}

void Credits(){
  lcd.clear();
  
  Intestazione();
  delay(1500);
  
  viewCredits(1,0,"Idea Original:",750);
  viewCredits(0,1,"Stephen Mathison",2000);

  lcd.clear();

  viewCredits(0,0,"Modificacion FW:",750);
  viewCredits(2,1,"Mike  Wilson",1750);
  viewCredits(2,1,"Massimo Nevi",1750);
  
  lcd.clear();
  
  viewCredits(2,0,"PCB & Tests:",750);
  viewCredits(2,1,"  DanielXan ",999);
  viewCredits(2,1,"SavioThechnic",999);
  viewCredits(2,1,"   A. Tidei  ",999);
  viewCredits(2,1," D. Arzarello",999);
  viewCredits(2,1,"L. Di Michele",999);
  
  lcd.clear();  
}

/*
void Menu_4(){
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print(F("TEST RAM"));
}
void Menu_4_1(){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print(F("Memoria Libera"));
  lcd.setCursor(4,1);
  //if (freeRam()<1000&&freeRam()>=100)LCDSpace(1);
  if (freeRam()<100&&freeRam()>=10)LCDSpace(1);
  if (freeRam()<10)LCDSpace(2);
  lcd.print(freeRam());
  LCDSpace(1);
  lcd.print(F("byte"));
  
  delay(3500);
  lcd.clear();
}
*/

void Pause_Stage(float Temp, int Time){
  lcd.setCursor(0,0);
  lcd.print(F("--- En Pausa ---" ));
  
  lcd.setCursor(1,1);
  PrintTemp(Temp);
  
  CountDown(Time,8,1,1);
}

void prompt_for_water (){
  lcd.setCursor(0,0);
  lcd.print(F(" Agregar  Agua? "));
  LCD_OkEsci();
  Buzzer(1,750);
}

void Resume(){
  lcd.setCursor(0,0);
  lcd.print(F("Iniciar Coccion?"));
  LCD_OkEsci();
  Buzzer(1,750);
}

void CntDwn(int Time){
  CountDown(Time,8,1,1);
}

void PausaPompa(float Temp, int Time){
//  if (Ciclo>=225){
//    Buzzer(2,35);
//    Ciclo=0;
//  }
  
  lcd.setCursor(0,0);
  lcd.print(F("- Pausa  Bomba -"));

  lcd.setCursor(1,1);
  PrintTemp(Temp);
  
  CountDown(Time,8,1,1);

//  Ciclo++;
}

void Iodine(float Temp, int Time){
  lcd.setCursor(0,0);
  LCDSpace(1);
  PrintTemp(Temp);
  LCDSpace(2);
  CountDown(Time,9,0,1);
  
  lcd.setCursor(1,1);
  lcd.print(F(" YODO "));
  //if (Ciclo<50)lcd.print(F(" YODO "));
  //else lcd.print(F("PRUEBA"));
  
  lcd.setCursor(7,1);
  lcd.print(F(" OK  -- "));
  
  //Ciclo++;
  //if (Ciclo>100)Ciclo=0;
}

void End(){
  Buzzer(3,250);
  lcd.setCursor(0,0);
  lcd.print(F("   Tu cerveza   "));
  lcd.setCursor(0,1);
  lcd.print(F("   esta lista   "));
  
  Buzzer(1,3000);
  delay(2500);
}

void PumpPrime(){
  lcd.setCursor(0,1);
  lcd.print(F("Activation Bomba"));  // priming the pump
}  

void ledHeatON(){
  lcd.setCursor(0,1);
  lcd.write(6);
}

void ledHeatStatus(boolean mheat){
  lcd.setCursor(0,1);
  if (mheat)lcd.write(5);
  else LCDSpace(1);
}

void ledPumpON(){
  lcd.setCursor(15,1);
  lcd.write(4);
}

void ledPumpStatus(boolean mpump){
  lcd.setCursor(15,1);
  if (mpump)lcd.write(3);
  else LCDSpace(1);
}

void ArdBir(){
  ArdBir1(4,0);
}

void PartenzaRitardata(){
  lcd.setCursor(0,0);
  lcd.print(F("Comenzar  Ahora?"));
  lcd.setCursor(0,1);
  LCDSpace(10);
  lcd.print(F("Si No ")); 
}

void ImpostaTempo(unsigned long Time){
  lcd.setCursor(0,0);
  lcd.print(F("Retraso "));
  
  CountDown(Time*60,8,0,2);
  
  lcd.setCursor(0,1);
  lcd.print(F(" --  -- Salir Ok"));
}

void StartDelay(unsigned long Tempo){
  lcd.setCursor(0,0);
  lcd.print(F(" Comenzando  en "));
  CountDown(Tempo,4,1,2);
}

void TemperaturaRaggiunta(){
  LCDClear(0);
  lcd.setCursor(0,0);
  lcd.print(F("Temp.  Alcanzada"));
  lcd.setCursor(9,1);
  lcd.print(F("Ok -- "));
}
