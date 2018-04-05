void Buzzer(byte NumBeep, int Period){
  for (byte i = 0; i < NumBeep; i++){
    #if ToneOnBuzzer == false
      digitalWrite (Buzz, HIGH);
      delay (Period);
      digitalWrite(Buzz, LOW);
      delay(75);
    #elif ToneOnBuzzer == true
      tone(Buzz, 600, Period);
      delay(Period + 75);
      noTone(Buzz);
    #endif
  }
}

void LCDSpace (byte Num){
  for(byte i = 0; i < Num; i++){
    lcd.print(F(" "));
  }
}

void FormatNumeri(float Numero, int Offset) {
  byte Spazi = 0;
  if (Numero <= -100.0)                    Spazi = 0;
  if (Numero <=  -10.0 && Numero > -100.0) Spazi = 1;
  if (Numero <     0.0 && Numero >  -10.0) Spazi = 2;
  if (Numero <    10.0 && Numero >=   0.0) Spazi = 3;
  if (Numero <   100.0 && Numero >=  10.0) Spazi = 2;
  if (Numero <  1000.0 && Numero >= 100.0) Spazi = 1;
  if (Numero >= 1000.0)                    Spazi = 0;
  
  LCDSpace(Spazi + Offset);
}

#if TestMemoria == true
int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
#endif


byte r_set(int addr){
  #if ReadWrite   == true  
    Serial.print (F("R-> "));
    Serial.print (addr);
    Serial.print (F(" byte: "));
    Serial.println (EEPROM.read(addr)); 
  #endif

  return EEPROM.read(addr);
}

void s_set (int addr, byte data){
  #if ReadWrite   == true
    Serial.print (F("W-> "));
    Serial.print (addr);
    Serial.print (F(" byte: "));
    Serial.println (data);
  #endif
  
  EEPROM.write(addr,data);

}

float r_set_float(int addr){ 
  #if ReadWrite    == true
    Serial.print (F("R-> "));
    Serial.print (addr);
    Serial.print (F(" float: "));
    Serial.println (word(EEPROM.read(addr),EEPROM.read(addr+1))); 
  #endif
  
  return word(EEPROM.read(addr),EEPROM.read(addr+1));
}

double r_set_double(int addr){ 
  #if ReadWrite   == true
    Serial.print (F("R-> "));
    Serial.print (addr);
    Serial.print (F(" double: "));
    Serial.println (word(EEPROM.read(addr),EEPROM.read(addr+1)));
  #endif
  
  return word(EEPROM.read(addr),EEPROM.read(addr+1));
 
}

void save_set (int addr, int data){
  EEPROM.write(addr,highByte(data));
  EEPROM.write((addr+1),lowByte(data));
  
  #if ReadWrite   == true
    Serial.print (F("W-> "));
    Serial.print (addr);
    Serial.print (F(" Word: "));
    Serial.println (data); 
  #endif
}  

void CountDown(unsigned long Tempo, byte posX, byte posY, byte numH){
  //numH = 1 Ore a 1 cifra
  //numH = 2 Ore a 2 cifre
  
  byte Ore, Minuti, Secondi;
  Ore= (byte)(Tempo/3600);
  Minuti= (byte)((Tempo%3600)/60);
  Secondi= (byte)(Tempo%60);
  
  lcd.setCursor(posX, posY);
  if (numH==2){
    if(Ore<10)lcd.print("0");
  }
  lcd.print(Ore);
  
  lcd.print(F(":"));    
  if(Minuti<10)lcd.print("0");
  lcd.print(Minuti);
  
  lcd.print(F(":"));    
  if(Secondi<10)lcd.print("0");
  lcd.print(Secondi);
}

// some seconds button press
byte btn_Press (byte Button_press, int msTime){
  if (digitalRead(Button_press)==0){
    delay (msTime);
    if (digitalRead(Button_press)==0){
      while(digitalRead(Button_press)==0){
      }
      return 1;
    }
  }
  return 0;
}

// repeat button press
byte btn_Repeat (byte Button_press){
  if (digitalRead(Button_press)==0){
    delay(250);
    return 1;
  }
  return 0;
}

byte LeggiPulsante(byte& Verso, unsigned long& Timer ){	
  // Verso=1 UP
  // Verso=2 DWN
  
  boolean f_btnUp,f_btnDn;
  
  if (digitalRead (Button_up)==0){	//Pressione specifica del pulsante UP
    if(Verso!=1)Timer=millis();		//Se non esiste pressione precedente parte il conteggio del tempo
    f_btnUp=true;
    Verso=1;		//Sentinella pulsante premuto
    delay(35);
  }else f_btnUp=false;
  
  if (digitalRead (Button_dn)==0){	//Pressione specifica del pulsante UP
    if(Verso!=2)Timer=millis();		//Se non esiste pressione precedente parte il conteggio del tempo
    f_btnDn=true;
    Verso=2;		//Sentinella pulsante premuto
    delay(35); 
  }else f_btnDn=false;
  
  if(digitalRead (Button_up)==0)f_btnUp=true;	//Legge lo stato del pulsante allâ€™uscita per aggiornare il flag
  else f_btnUp=false;
  
  if(digitalRead (Button_dn)==0)f_btnDn=true;	//Legge lo stato del pulsante allâ€™uscita per aggiornare il flag
  else f_btnDn=false;
  	
  if(f_btnUp==false && f_btnDn==false)Verso=0;	//Confronta lo stato dei pulsanti per assegnare la NON pressione
}



float Arrotonda025(float& Num){
  // Appoggio la parte intera
  int Appoggio= (int)Num;
  
  // Arrotondo il valore con peso 0.25
  Num=Appoggio+int((Num-Appoggio)*1000/225)*0.25;
}

float ConvertiCtoF(float& Num){        
  Num = Num/16;              // Recupero il valore
  Num = (Num*1.8)+32;          // Converto in °F
  Arrotonda025(Num);
  Num = Num*16;              // Preparo il valore per la registrazione
}
float ConvertiFtoC(float& Num){
  Num = Num/16;              // Recupero il valore
  Num = (Num-32)/1.8;            // Converto in °C
  Arrotonda025(Num);
  Num = Num*16;              // Preparo il valore per la registrazione
}

int Set(int& Set, int Up, int Low, int Step, long Timer, byte Verso){
  int step_size;
  int ControllaPulsante;
  
  if(Set>Up)Set=Up;
  if(Set<Low)Set=Low;
  	
  delay(35);
  if(Verso==1){
    ControllaPulsante=digitalRead(Button_up);
  }
  if(Verso==2){
    ControllaPulsante=digitalRead(Button_dn);
  }
  
  if(ControllaPulsante==0 && Verso!=0){
    if(((millis()-Timer)/1000)>=4)step_size=(Step*10);
    else{
      if(((millis()-Timer)/1000)>=2)step_size=(Step*5);
      else step_size=Step;
    }
    if(Verso==1){    
      if (Set+step_size>Up)Set=Up;
      else Set+=step_size;
    }else if(Verso==2){
      if (Set-step_size<Low)Set=Low;
      else Set-=step_size;
    }		
  }   
}

float Set(float& Set, float Up, float Low, float Step, long Timer, byte Verso){
  float step_size;
  int ControllaPulsante;

  if(Set>Up)Set=Up;
  if(Set<Low)Set=Low;
  	
  delay(35);
  if(Verso==1){
    ControllaPulsante=digitalRead(Button_up);
  }
  if(Verso==2){
    ControllaPulsante=digitalRead(Button_dn);
  }
  
  if(ControllaPulsante==0 && Verso!=0){
    if(((millis()-Timer)/1000)>=4)step_size=(Step*20.0);
    else{
      if(((millis()-Timer)/1000)>=2)step_size=(Step*4.0);
      else step_size=Step;
    }
    if(Verso==1){    
      if (Set+step_size>Up)Set=Up;
      else Set+=step_size;
    }else if(Verso==2){
      if (Set-step_size<Low)Set=Low;
      else Set-=step_size;
    }		
  }   
}

byte Set(byte& Set, byte Up, byte Low, byte Step, long Timer, byte Verso){
  int step_size;
  int ControllaPulsante;

  if(Set>Up)Set=Up;
  if(Set<Low)Set=Low;
  
  delay(35);
  if(Verso==1){
    ControllaPulsante=digitalRead(Button_up);
  }
  if(Verso==2){
    ControllaPulsante=digitalRead(Button_dn);
  }
  
  if(ControllaPulsante==0 && Verso!=0){
    if(((millis()-Timer)/1000)>=4)step_size=(Step*10);
    else{
      if(((millis()-Timer)/1000)>=2)step_size=(Step*5);
      else step_size=Step;
    }
    if(Verso==1){    
      if (Set+step_size>Up)Set=Up;
      else Set+=step_size;
    }else if(Verso==2){
      if (Set-step_size<Low)Set=Low;
      else Set-=step_size;
    }		
  }   
}

void display_lcd (byte posX , byte posY ,const char* lable, int Pausa){
  lcd.setCursor(posX, posY);
  lcd.print(lable);
  delay(Pausa);
}


