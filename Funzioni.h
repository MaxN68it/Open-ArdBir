void CountDown(unsigned long Tempo, byte posX, byte posY, byte numH){
  byte Ore, Minuti, Secondi;
  
  Ore= byte(Tempo/3600);
  Minuti= byte((Tempo-(Ore*3600))/60);
  Secondi= byte(Tempo-((Ore*3600)+(Minuti*60)));
     
  lcd.setCursor(posX, posY);
  if (numH==2)lcd.print(F("0"));
  lcd.print(Ore);
      
  if(Minuti<10)lcd.print(F(":0"));
  else lcd.print(F(":"));
  lcd.print(Minuti);
      
  if(Secondi<10)lcd.print(F(":0"));
  else lcd.print(F(":"));
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
    delay(225);
    return 1;
  }
  return 0;
}

byte LeggiPulsante(byte& Verso, unsigned long& Timer ){	
  boolean f_btnUp,f_btnDn;
  
  if (digitalRead (Button_up)==0){	//Pressione specifica del pulsante UP
    if(Verso!=1)Timer=millis();		//Se non esiste pressione precedente parte il conteggio del tempo
    f_btnUp=true;
    Verso=1;		//Sentinella pulsante premuto
    //Set_1(Set, Up, Low, Step, Timer, Verso);	
    delay(25);
  }else f_btnUp=false;
  
  if (digitalRead (Button_dn)==0){	//Pressione specifica del pulsante UP
    if(Verso!=2)Timer=millis();		//Se non esiste pressione precedente parte il conteggio del tempo
    f_btnDn=true;
    Verso=2;		//Sentinella pulsante premuto
    //Set_1(Set, Up, Low, Step, Timer, Verso);	
    delay(25); 
  }else f_btnDn=false;
  
  //delay(50);
  
  if(digitalRead (Button_up)==0)f_btnUp=true;	//Legge lo stato del pulsante allâ€™uscita per aggiornare il flag
  else f_btnUp=false;
  
  if(digitalRead (Button_dn)==0)f_btnDn=true;	//Legge lo stato del pulsante allâ€™uscita per aggiornare il flag
  else f_btnDn=false;
  	
  if(f_btnUp==false && f_btnDn==false)Verso=0;	//Confronta lo stato dei pulsanti per assegnare la NON pressione
}



float Arrotonda025(float& Num){
  int Appoggio;  
  // Appoggio la parte intera
  Appoggio = (int)int(Num);           
  // Arrotondo il valore con peso 0.25
  Num=Appoggio+int((Num-Appoggio)*1000/225)*0.25;
}

float ConvertiCtoF(float& Num){        
  Num = Num/16;              // Recupero il valore
  Num = (Num*1.8)+32;          // Converto in Â°F
  Arrotonda025(Num);
  Num = Num*16;              // Preparo il valore per la registrazione
}
float ConvertiFtoC(float& Num){
  Num = Num/16;              // Recupero il valore
  Num = (Num-32)/1.8;            // Converto in Â°C
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
//    if(Set<Low)Set=Low;
//    if(Set>=Up)Set=Up;
    ControllaPulsante=digitalRead(Button_up);
  }
  if(Verso==2){
//    if(Set>Up)Set=Up;
//    if(Set<=Low)Set=Low;
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
//    if(Set<Low)Set=Low;
//    if(Set>=Up)Set=Up;
    ControllaPulsante=digitalRead(Button_up);
  }
  if(Verso==2){
//    if(Set>Up)Set=Up;
//    if(Set<=Low)Set=Low;
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
//    if(Set<Low)Set=Low;
//    if(Set>=Up)Set=Up;
    ControllaPulsante=digitalRead(Button_up);
  }
  if(Verso==2){
//    if(Set>Up)Set=Up;
//    if(Set<=Low)Set=Low;
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

