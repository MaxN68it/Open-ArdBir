char *PIDName[]   ={"Konstante  P", "Konstante  I", "Konstante  D", "Finestra  ms", "PWM         ", "Calibracion "};
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
  if (Temp<10.0)LCDSpace(2);
  if (Temp>=10.0 && Temp<100.0)LCDSpace(1);
  lcd.print(Temp);
  
  //Gradi();
  lcd.write((byte)0);
}

void Clear_2_3(){
  LCDClear(2);
  LCDClear(3);
}

void Version(byte locX, byte locY){
  lcd.setCursor(locX, locY);
  //lcd.print(Version20);
  lcd.print(F("2.6.63b"));
  lcd.write(7);
}

void Intestazione(){  
  lcd.setCursor(0,0);
  lcd.print(F("Open ArdBir "));
  Version(12,0);
}

void LCD_Procedo(){
  lcd.setCursor(1,3);
  lcd.print(F("Procedo-> Ok Salir"));
}

/*
void LCD_Conferma(){
  lcd.setCursor(1,3);
  lcd.print(F("Procedo->  Ok  ---"));
}
*/

void LCD_Default(float Temp){
  Intestazione();
  
  lcd.setCursor(6,1);
  PrintTemp(Temp);

  LCDClear(2);
  
  lcd.setCursor(1,3);
  lcd.print(F("---  MAN AUTO SETUP"));
}

void CntDwn(int Time){
  CountDown(Time,11,2,2);
}

void Watch(int Time){
  CountDown(Time,6,2,2);
}

void PauseScreen(){
  lcd.setCursor(0,0);
  lcd.print(F("------ "));
  lcd.setCursor(13,0);
  lcd.print(F(" ------"));

  lcd.setCursor(1,2);
  lcd.print(F("----"));
  lcd.setCursor(14,2);
  lcd.print(F(" ----"));
}

void LCD_QQSO(){
  lcd.setCursor(1,3);
  lcd.print(F("SU* *BAJ omite  ok"));
}
void LCD_QQxO(){
  lcd.setCursor(1,3);
  lcd.print(F("SU* *BAJ  ---   ok"));
}
void LCD_SGEO(){
  lcd.setCursor(1,3);
  lcd.print(F("su   baj salir  ok"));
}
void LCD_xGEO(){
  lcd.setCursor(1,3);
  lcd.print(F("---  baj salir  ok"));
}

void LCD_SxEO(){
  lcd.setCursor(1,3);
  lcd.print(F("su   --- salir  ok"));
}


void Menu_1(){
  lcd.clear();

  lcd.setCursor(4,0);
  lcd.print(F("MODO  MANUAL"));
}    

void Manuale(float Set, float Temp,float TempBoil){    
  lcd.setCursor(1,1);
  PrintTemp(Temp);

  lcd.setCursor(12,1);
  if (Set<100)LCDSpace(1);
  lcd.print(Set);
  lcd.write(2);
  
  lcd.setCursor(1,3);
  lcd.print(F("SU* *BAJ  Res  Bmb"));
}


void Menu_2(){
  lcd.clear();  

  lcd.setCursor(2,0);
  lcd.print(F("MODO  AUTOMATICO"));
}
void AddMalt(){
  lcd.setCursor(2,2);
  lcd.print(F(" Agregar Malta  "));
  
  LCD_Procedo();
  Buzzer(1, 1000);
}

void Stage(byte Stage, float Set, float Temp){
  lcd.setCursor(0,0);
  lcd.print(F(" AUTO --> "));
  lcd.print(stageName[Stage]);
  
  lcd.setCursor(1,1);
  PrintTemp(Temp);

  lcd.setCursor(8,1);
  LCDSpace(4);
  if (Set<100)LCDSpace(1);
  lcd.print(Set);
  lcd.write(2);
    
  lcd.setCursor(1,3);
  lcd.print(F("SU* *BAJ Pausa STP"));
}

void SaltoStep(){
  lcd.setCursor(2,2);
  lcd.print(F("Siguiente  Paso? "));
  LCD_Procedo();
}

void RemoveMalt(){
  lcd.setCursor(3,2);
  lcd.print(F("Eliminar Malta  "));
  LCD_Procedo();
  Buzzer(1,1500);
}

void Temp_Wait(float Temp){
  lcd.setCursor(1,1);
  PrintTemp(Temp);
}

void Boil(float Heat, float Temp, byte Tipo){
  if (Tipo==1){
    lcd.setCursor(0,0);
    lcd.print(F(" AUTO --> Ebullicion"));
  }
  
  lcd.setCursor(1,1);
  PrintTemp(Temp);
  
  lcd.setCursor(1,2);
  lcd.print(F("PWM="));    //Display output%
  if (Heat<100 && Heat>=10)LCDSpace(1); 
  if (Heat<10)LCDSpace(2); 
  lcd.print(Heat,0); //Display output%
  lcd.print(F("% ")); 
} 

void NoBoil(){
  lcd.setCursor(1,2);
  LCDSpace(9);
}

void HopAdd(byte HopAdd){
  lcd.setCursor(10,0);
  lcd.print(F("Lupulo  "));
  if(HopAdd<10)LCDSpace(1);
  lcd.print(HopAdd+1);
}



void Menu_3(){
  lcd.clear();

  lcd.setCursor(1,0);
  lcd.print(F("MENU CONFIGURACION"));
}
void Menu_3_1(){
  lcd.setCursor(1,1);
  lcd.print(F("  Parametros PID  "));

  LCDClear(2);

  LCD_xGEO();
}
void Menu_3_1_x(byte i){
  lcd.setCursor(1,2);
  lcd.print(PIDName[i]);
  LCD_QQxO();
}  

void PidSet(int pidSet, byte i){
  lcd.setCursor(14,2);
  if(i<4)LCDSpace(1);

  if (i<3||i==4||i==6){
    if (pidSet<=-10 && pidSet>-100)LCDSpace(1);
    if (pidSet<0 && pidSet>-10)LCDSpace(2);
    if (pidSet<10 && pidSet>=0)LCDSpace(3);
    if (pidSet>=10 && pidSet<100)LCDSpace(2);
    if (pidSet>=100)LCDSpace(1);
  }
  
  if(i==5){
    float OffSet=pidSet/10.0;
    if (OffSet>=0)LCDSpace(1);
    lcd.print(OffSet);
    return;
  }
  
  lcd.print(pidSet);

  if(i==4)lcd.print(F("%"));
}

void Menu_3_2(){
  lcd.setCursor(1,1);
  lcd.print(F("Parametros  Unidad"));
  
  LCDClear(2);
  
  LCD_SGEO();
}

void Menu_3_2_x(byte i){
  lcd.setCursor(1,2);
  lcd.print(unitName[i]);
  LCD_QQxO();
}

void UnitSet(byte unitSet, byte i){
  lcd.setCursor(12,2);
  switch(i){
   
    case(0):// Scala Temp
      //lcd.setCursor(17,2);
      LCDSpace(6);
      lcd.write((byte)0);  
      break;
      
    case(1)://Sensore
      //lcd.setCursor(11,2);
      if (unitSet==0)lcd.print(F("Interno"));
      else lcd.print(F("Externo"));
      break;
      
    default:// Temperatura di Ebollizione
      //lcd.setCursor(14,2);
      LCDSpace(3);
      if (unitSet<100)LCDSpace(1);
      lcd.print(unitSet);
      lcd.write((byte)0);
      break;
    /*
    case(3):// Temperatura di Ebollizione F
      //lcd.setCursor(14,2);
      LCDSpace(3);
      if (unitSet<100)LCDSpace(1);
      lcd.print(unitSet);
      lcd.write((byte)0);
      break;
    */  
    case(4):// Durata Ciclo Pompa
      //lcd.setCursor(15,2);
      LCDSpace(4);
      if (unitSet<10)LCDSpace(1);
      lcd.print(unitSet);
      lcd.print(F("'"));
      break;
    
    case(5)://Durata Pausa Pompa
      //lcd.setCursor(16,2);
      LCDSpace(5);
      lcd.print(unitSet);
      lcd.print(F("'"));
      break;
    
    case(6):
      //lcd.setCursor(15,2);
      LCDSpace(4);
      if (unitSet==0)lcd.print(F("OFF"));
      if (unitSet==1)lcd.print(F(" ON"));
      break;
      
    case(7):
      //lcd.setCursor(15,2);
      LCDSpace(3);
      if (unitSet<100)LCDSpace(1);
      lcd.print(unitSet);
      //Gradi();
      lcd.write((byte)0);
  }  
}


void Menu_3_3(){
  lcd.setCursor(1,1);
  lcd.print(F("Set Automatizacion"));
  LCD_SGEO();
}     
void Menu_3_3_x(byte Stage){
  lcd.setCursor(1,2);
  lcd.print(stageName[Stage]);
  if (Stage==0||Stage==6||Stage==7)LCD_QQxO();
  else LCD_QQSO();
}  

void StageSet(float Temp){
  lcd.setCursor(12,2);
  PrintTemp(Temp);
}

void TimeSet(int Time){
  lcd.setCursor(12,2);
  if (Time<10)LCDSpace(5);
  if (Time>=10 && Time<100)LCDSpace(4);
  if (Time>100)LCDSpace(3);
  lcd.print(Time);   
  lcd.print(F("'"));
  LCD_QQxO();
}


void Menu_3_3_8(){
  lcd.setCursor(1,2);
  lcd.print(F("Numero Lupulo  "));
  LCD_QQxO();
} 
void NumHops(byte SetNumHops){
  lcd.setCursor(17,2);
  if(SetNumHops<10)LCDSpace(1);
  lcd.print(SetNumHops);   
}

void Menu_3_3_9(){
  lcd.setCursor(1,2);
  lcd.print(F("Ebullicion    "));
  LCD_QQxO();
} 

void Menu_3_3_10(byte SetHop){
  lcd.setCursor(1,2);
  lcd.print(F("Lupulo  ("));
  if (SetHop<10)LCDSpace(1);
  lcd.print(SetHop);
  lcd.print(F(")  "));
  LCD_QQxO();
} 

void TimeHops(int Time){
  lcd.setCursor(15,2);
  if (Time<10)LCDSpace(2);
  if (Time>=10 && Time<100)LCDSpace(1);
  lcd.print(Time);   
  lcd.print(F("'"));
}



void Menu_3_4(){
  lcd.setCursor(1,1);
  lcd.print(F(" Gestion  Recetas "));
  
  LCDClear(2);

  LCD_SGEO();
}     
void Menu_3_4_1(){
  lcd.setCursor(2,2);
  lcd.print(F(" Cargar  Receta "));
  LCD_xGEO();
}
void Menu_3_4_2(){
  lcd.setCursor(2,2);
  lcd.print(F(" Salvar  Receta "));
  LCD_SGEO();
}
void Menu_3_4_3(){
  lcd.setCursor(2,2);
  lcd.print(F("Eliminar  Receta"));
  LCD_SGEO();
}
void Menu_3_4_4(){
  lcd.setCursor(2,2);
  lcd.print(F("Inicializacion  "));
  LCD_SxEO();
}

void NoRecipe(){
  LCDClear(2);  
  
  lcd.setCursor(2,3);
  lcd.print(F(" NINGUNA  RECETA "));
  
  Buzzer(3,50);
  delay(1500);
}

void Ricetta(byte numRicetta, byte Tipo){
  //Tipo 0 = Load
  //Tipo 1 = Save
  
  lcd.setCursor(2,2);
  if (numRicetta<10)lcd.print(F("0"));
  lcd.print(numRicetta);
  lcd.print(F(" > "));
  
  if (Tipo==0)LCD_Procedo();
  else{
    lcd.setCursor(1,3);
    lcd.print(F("SU* *BAJ  atras ok"));
  }
}

void LCD_NomeRicetta(byte pos, byte Lettera){
  lcd.setCursor(pos+7,2);
  lcd.print((char)Lettera);
}

void LeggoRicetta(byte Ricetta){
  LCDClear(2);
  
  lcd.setCursor(1,3);
  lcd.print(F(" Cargar Recetas... "));
  Buzzer(2,35);
  delay(1500);

  lcd.setCursor(1,3);
  lcd.print(F(" Rec. "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  lcd.print(F(" Cargada  "));
  delay(1500);
}


void SalvataggioRicetta(byte Ricetta){
  lcd.setCursor(1,2);
  lcd.print(F("Salvar Recetas "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  lcd.print(F("?"));
  
  LCD_Procedo();
}

void SalvaRicetta(){
  LCDClear(2);
  
  lcd.setCursor(1,3);
  lcd.print(F("   Rescatar...  "));
  Buzzer(5,35);
  delay(1500);

  lcd.setCursor(1,3);
  lcd.print(F(" Receta   Salvada  "));
  delay(1500);  
}

void CancelloRicetta(byte Ricetta){
  lcd.setCursor(0,2);
  lcd.print(F("Eliminar Receta  "));
  if (Ricetta<10)lcd.print(F("0"));
  lcd.print(Ricetta);
  LCDSpace(1);
  
  LCD_Procedo();
}

void Cancellazione(byte Ricetta){
  LCDClear(2);
  
  lcd.setCursor(1,3);
  lcd.print(F("Cancelacion...    "));
  Buzzer(2,35);
  delay(1500);
  
  lcd.setCursor(1,3);
  lcd.print(F("Receta  Cancelada "));
  delay(1500);
}

void Inizializzazione(){
  lcd.setCursor(2,2);
  lcd.print(F(" Iniciar "));
  
  LCD_Procedo();
}
void Inizializza(){
  LCDClear(2);
  
  lcd.setCursor(1,3);
  lcd.print(F("Iniciar EEPROM"));
  delay(1500);
  
  lcd.setCursor(1,3);
  lcd.print(F(" EEPROM preparada "));
  Buzzer(3,75);
  delay(1500);
}

void MemoriaPiena(){
  LCDClear(2);
  
  lcd.setCursor(1,3);
  lcd.print(F("     ATENCION     "));
  Buzzer(3,125);
  delay(1500);

  lcd.setCursor(1,3);
  lcd.print(F("  MEMORIA  LLENA  "));
  delay(2000);
}


void Menu_3_5(){
  lcd.setCursor(1,1);
  lcd.print(F("     Creditos     "));
  LCD_SxEO();
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
  
  viewCredits(3,2,"Idea Original:",750);
  viewCredits(2,3,"Stephen Mathison",2000);

  Clear_2_3();

  viewCredits(2,2,"Modificacion: FW",750);
  viewCredits(3,3," Mike Wilson ",1750);
  viewCredits(3,3,"Massimo  Nevi",1750);
  
  Clear_2_3();
  
  viewCredits(4,2,"PCB & Tests:",750);
  viewCredits(3,3,"  DanielXan ",999);
  viewCredits(3,3,"SavioThechnic",999);
  viewCredits(3,3,"   A. Tidei  ",999);
  viewCredits(3,3," D. Arzarello",999);
  viewCredits(3,3,"L. Di Michele",999);
  
  Clear_2_3();  
}



/*
void Menu_4(){
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print(F("TEST DELLA RAM"));
}
void Menu_4_1(){
  lcd.setCursor(3,1);
  lcd.print(F("Memoria Libera"));
  lcd.setCursor(6,2);
  //if (freeRam()<1000&&freeRam()>=100)LCDSpace(1);
  if (freeRam()<100&&freeRam()>=10)LCDSpace(1);
  if (freeRam()<10)LCDSpace(2);
  lcd.print(freeRam());
  LCDSpace(2);
  lcd.print(F("byte"));
  
  LCDClear(3);
  delay(3500);
  lcd.clear();
}
*/


void Pause_Stage(float Temp, int Time){
  PauseScreen();
  
  lcd.setCursor(1,1);
  lcd.print(F("     En Pausa     " ));
  
  lcd.setCursor(5,0);
  PrintTemp(Temp);
  
//  CountDown(Time,6,2,2);
  Watch (Time);
  
  lcd.setCursor(1,3);
  lcd.print(F("---  ---  Salir---"));
}

void prompt_for_water (){
  lcd.setCursor(2,1);
  lcd.print(F(" Agregar  Agua? "));
  LCD_Procedo();
  Buzzer(1,750);
}

void Resume(){
  lcd.setCursor(1,1);
  lcd.print(F("Iniciar   Coccion?"));
  LCD_Procedo();
  Buzzer(1,750);
}

void PausaPompa(float Temp, int Time){
//  if (Ciclo>=225){
//    Buzzer(2,35);
//    Ciclo=0;
//  }
  
  lcd.setCursor(1,1);
  PrintTemp(Temp);

//  CountDown(Time,11,2,2);
  CntDwn(Time);
  
  lcd.setCursor(1,3);
  lcd.print(F(" - Pausa  Bomba - "));
 
//  Ciclo++;
}

void Iodine(float Temp, int Time){
  PauseScreen();
  
    
  lcd.setCursor(4,1);
  lcd.print(F("PRUEBA  YODO"));
  
  lcd.setCursor(6,0);
  PrintTemp(Temp);
  
//  CountDown(Time,6,2,2);
  Watch (Time);
  
  lcd.setCursor(1,3);
  lcd.print(F("---  ---   OK  ---"));
}

void End(){
  lcd.clear();
  
  delay(500);
  //Buzzer(3,250);
  
  lcd.setCursor(5,1);
  lcd.print(F("Tu cerveza"));
  lcd.setCursor(4,2);
  lcd.print(F("esta  lista!"));
  
  Buzzer(1,3000);
  delay(2500);
}

void PumpPrime(){
  lcd.setCursor(2,1);
  lcd.print(F("Activacion Bomba"));  // priming the pump
}  

void ledHeatON(){
  lcd.setCursor(0,2);
  lcd.write(6);
}

void ledHeatStatus(boolean mheat){
  lcd.setCursor(0,2);
  if (mheat)lcd.write(5);
  else LCDSpace(1);
}

void ledPumpON(){
  lcd.setCursor(19,2);
  lcd.write(4);
}

void ledPumpStatus(boolean mpump){
  lcd.setCursor(19,2);
  if (mpump)lcd.write(3);
  else LCDSpace(1);
}

void ArdBir(){
  ArdBir1(6,1);
}

void PartenzaRitardata(){
  Clear_2_3();
  lcd.setCursor(2,2);
  lcd.print(F("Comenzar Ahora?"));
  lcd.setCursor(14,3);
  lcd.print(F("Si No")); 
}

void ImpostaTempo(unsigned long Time){
  lcd.setCursor(1,1);
  lcd.print(F("Configurar Retraso"));
 
  CountDown(Time*60,6,2,2);
  
  LCD_SGEO();
}

void StartDelay(unsigned long Tempo){
  lcd.setCursor(2,1);
  lcd.print(F("Comenzando en"));
  CountDown(Tempo,6,2,2);
}

void TemperaturaRaggiunta(){
  LCDClear(2);
  lcd.setCursor(3,2);
  lcd.print(F("Temp.Alcanzada"));
  lcd.setCursor(1,3);
  lcd.print(F("Procedo->  Ok  ---"));
  //LCD_Conferma();
}
