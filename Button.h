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




