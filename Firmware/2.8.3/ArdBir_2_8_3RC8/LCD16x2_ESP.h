char *PIDName[]   ={"Uso ", "Constante P", "Constante I", "Constante D", "SampleTime ", "Finestra ms", "PWM        ", "Calibracion ", "Histeresis "};
char *stageName[] ={"Mash In   ", "Fitasa    ", "Glucanasa ", "Proteasa  ", "\xE2""Amilasa  ", "\xE0""Amilasa1 ", "\xE0""Amilasa2 ", "Mash Out  ", "Ebullicion", "Cooling   ", "Whirlpool "};
char *unitName[]  ={"Escala     ", "Sensor     ", "Ebullicion  ", "Ebullicion  ", "Ciclo Bomba", "Pausa Bomba   ","Bmb PreMash",  "Bmb on Mash", "Bmb MashOut", "Bmb en Ebul", "Bomba Parada", "Bomba Parada", "PID Pipe  " , "Skip Add      ", "Skip Remove ", "Skip Iodine ", "TiempoYodo", "Whirlpool" };

byte HeatONOFF[8]    = {B00000, B01110, B01010, B01010, B01100, B01010, B01010, B00000};  // [5] HEAT symbol
byte RevHeatONOFF[8] = {B11111, B10001, B10101, B10101, B10011, B10101, B10101, B11111};  // [6] reverse HEAT symbol
byte Language[8]     = {B00000, B10111, B10101, B11101, B00000, B10001, B10101, B11111};  // [7] ESP symbol


void LCDClear(byte Riga){
  lcd.setCursor(0,Riga);
  LCDSpace(20);
}

void PrintTemp(float Temp){
  if (Temp<10.0)LCDSpace(1);
  if (Temp>=100.0)lcd.print(Temp,1);
  else lcd.print(Temp);
  lcd.write((byte)0);
}

void Clear_2_3(){
  LCDClear(1);
}

void Version(byte locX, byte locY){
  lcd.setCursor(locX, locY);
  LCDSpace(1);
  lcd.print(F("2.8.3 "));
  lcd.write(7);
}

void Intestazione(){  
  lcd.setCursor(0,0);
  lcd.print(F("Open ArdBir"));
  lcd.write(1);
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
}

void Stage(byte Stage, float Set, float Temp){
  lcd.setCursor(0,0);
  if (Stage < 11) lcd.print(stageName[Stage]);
  else            lcd.print(stageName[10]);
  
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
  lcd.print(F("R="));       //Display output%
  if (Heat<100)LCDSpace(1); //added space as place holder for <100
  lcd.print(Heat,0);        //Display output%
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

void LCD_Add() {
  lcd.setCursor(0, 0);
  lcd.print(F("Irish Moss"));
}

void Raffreddamento() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F(" START  COOLING "));
  LCD_Procedo();
}

void LCDWhirlpool() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("START  WHIRLPOOL"));
  LCD_Procedo();
}void Menu_3(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F(" CONFIGURACION  "));
  delay(750);
}
void Menu_3_1(){
  lcd.setCursor(0,0);
  //lcd.print(F(" PARAMETROS PID "));
  lcd.print(F("   PID -- PWM   "));
}     
void Menu_3_1_x(byte i){
  lcd.setCursor(0,1);
  lcd.print(PIDName[i]);
}  
void PidSet(int pidSet, byte i){
  lcd.setCursor(9, 1);
  if (i > 0) LCDSpace(2);
  
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
  
  if (i == 0) {
    lcd.setCursor(7, 1);
    if (pidSet == 0) lcd.print(F("Electrico"));
    else             lcd.print(F("      Gas"));
  } else             lcd.print(pidSet);

  if (i == 6) lcd.print(F("%"));
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
        lcd.setCursor(15,1);
        lcd.write((byte)0);  
        break;
      
    case(1)://Sensore
      lcd.setCursor(9,1);
      if (unitSet==0)lcd.print(F("Interno"));
      else lcd.print(F("Externo"));
      break;
      
    case( 2):
    case( 3): // Temperatura di Ebollizione
    case(10):
    case(11): //Temperatura Fermo Pompa
      lcd.setCursor(12,1);
      if (unitSet<100)LCDSpace(1);
      lcd.print(unitSet);
      lcd.write((byte)0);
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
    
    default:
      lcd.setCursor(13,1);
      if (unitSet==0)lcd.print(F("Off"));
      if (unitSet==1)lcd.print(F(" On"));
      break;
      
    case(12)://Pipe
      lcd.setCursor(9,1);
      if (unitSet==0)lcd.print(F(" Pasivo"));
      else lcd.print(F(" Activo"));
      break;

    case(13):
    case(14):
    case(15):
      lcd.setCursor(14,1);
      if (unitSet==0)lcd.print(F("No"));
      if (unitSet==1)lcd.print(F("Si"));
      break;
    
    case(16): //Iodio
      if (unitSet==0){
        lcd.setCursor(9,1);
        lcd.print(F("    Off"));
      }else CountDown(unitSet*60,9,1,1);
      break;
       break;
      
    case(17):
      if (unitSet == 0) lcd.print(F("    Off"));
      if (unitSet == 1) lcd.print(F("   Cold"));
      if (unitSet == 2) lcd.print(F("    Hot"));
      break;
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

void Credits(){
  lcd.clear();
  
  Intestazione();
  delay(1500);
  
  display_lcd(1,0,"Idea Original:",750);
  display_lcd(0,1,"Stephen Mathison",2000);

  lcd.clear();

  display_lcd(0,0,"Modificacion FW:",750);
  display_lcd(2,1,"Mike  Wilson",1750);
  display_lcd(2,1,"Massimo Nevi",1750);
  
  lcd.clear();
  
  display_lcd(2,0,"PCB & Tests:",750);
  display_lcd(2,1,"  DanielXan ",999);
  display_lcd(2,1,"SavioThechnic",999);
  display_lcd(2,1,"   A. Tidei  ",999);
  display_lcd(2,1," D. Arzarello",999);
  display_lcd(2,1,"L. Di Michele",999);
  
  lcd.clear();  
  
  display_lcd(1,0,"Traducciones:",750);
  display_lcd(2,1," A. Moiseyev ",999);//Russo
  display_lcd(2,1," A. Mondejar ",999);//Spagnolo
  display_lcd(2,1," C.M. Macedo ",999);//Portoghese 20x4
  display_lcd(2,1,"F.A. Oliveira",999);//Portugues 16x2
  display_lcd(2,1,"G.  Santaella",999);//Portuguese 20x4 (new)
  display_lcd(2,1,"  E. Eduard  ",999);//Norvegese  20x4 (new)
  
  lcd.clear();
}

#if TestMemoria == true
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
  
    if (freeRam()<100&&freeRam()>=10)LCDSpace(1);
    if (freeRam()<10)LCDSpace(2);
    lcd.print(freeRam());
    LCDSpace(1);
    lcd.print(F("byte"));
  
    delay(3500);
    lcd.clear();
}
#endif

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
}

void Resume(){
  lcd.setCursor(0,0);
  lcd.print(F("Iniciar Coccion?"));
  LCD_OkEsci();
}

void CntDwn(int Time){
  CountDown(Time,8,1,1);
}

void PausaPompa(float Temp, int Time){
  lcd.setCursor(0,0);
  lcd.print(F("- Pausa  Bomba -"));

  lcd.setCursor(1,1);
  PrintTemp(Temp);
  
  CountDown(Time,8,1,1);
}

void Iodine(float Temp, int Time){
  lcd.setCursor(0,0);
  LCDSpace(1);
  PrintTemp(Temp);
  LCDSpace(2);
  CountDown(Time,9,0,1);
  
  lcd.setCursor(1,1);
  lcd.print(F(" YODO "));
  lcd.setCursor(7,1);
  lcd.print(F(" Ok  -- "));
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
  #if StartSprite == true
    Presentazione(0,0);
  #endif
  
  #if Sprite == true
    ArdBir1(4,0);
  #endif
}

void PartenzaRitardata(){
  lcd.setCursor(0,0);
  lcd.print(F("Retrasar Inicio?"));
  lcd.setCursor(0,1);
  LCDSpace(10);
  lcd.print(F("No Si ")); 
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

void ImpostaWhirlpool(unsigned long Time) {
  lcd.setCursor(0, 1);
  lcd.print(F("Timing "));
 
  CountDown(Time * 60, 7, 2, 2);
}

