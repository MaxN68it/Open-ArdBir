
byte Congruita(byte& numRicetta, byte Verso){  
  if (EEPROM.read(89+numRicetta)==0){
    boolean Controllo=true;
    //byte numControllo=numRicetta; 
    
    while(Controllo){
      //if(Verso==1)if(numControllo<10)numControllo++;
      if(Verso==1)if(numRicetta<10)numRicetta++;
      else Controllo=false; 
        
      //if(Verso==2)if(numControllo>1)numControllo--;
      if(Verso==2)if(numRicetta>1)numRicetta--;
      else Controllo=false;
      
      //if (EEPROM.read(89+numControllo)==1){
      if (EEPROM.read(89+numRicetta)==1){ 
        //numRicetta=numControllo;
        Controllo=false;
      }
    }
  }
}

void loadRecipe(){
  byte numRicetta = 0;  
  boolean ricettaLoop = true;

  byte Verso=0;
  unsigned long Timer=0; 
 
  byte RicettaUp, RicettaDwn;
  RicettaUp=0;
  RicettaDwn=0;
  
  for(byte i=90; i<100; i++){//Assegna il limite di ricette registrate 
    if (EEPROM.read(i)==1){
      RicettaUp = (i-89);
      if(RicettaDwn==0)RicettaDwn = RicettaUp;
    }
  }
  if (RicettaUp==0){
    NoRecipe();
    return;
  }
  
  byte NomeRicetta[10];
  byte pos = 0; 
  
  for(byte i=RicettaDwn+89; i<RicettaUp+89+1; i++){//Trova la prima ricetta libera
    numRicetta=i-89;
    
    while (ricettaLoop){
      Ricetta(numRicetta,0);
      LeggiPulsante(Verso,Timer);
      Set(numRicetta, RicettaUp, RicettaDwn, 1, Timer, Verso);
      
      for (pos=0; pos<10; pos++){
        LCD_NomeRicetta(pos, EEPROM.read(620 + pos + ((numRicetta - 1)*10)));
      }
      
      Congruita(numRicetta, Verso);
      if (btn_Press(Button_enter,50)){
        Menu_3();
        Menu_3_4();
        ricettaLoop = false;  
        i=100;
      }
      if (btn_Press(Button_start,50)){
        Clear_2_3();
        LeggoRicetta(numRicetta);
        //SwapRecipe(numRicetta, 0);
        
        int Da;
  
        //Parametri Ricetta
        Da= 100 + ((numRicetta-1)*52);
        for (int j=30; j<82; j++){ 
          save_set (j, (byte)EEPROM.read(Da));
          //delay(75);
          Da++;
        }
        ricettaLoop = false;  
      }
    }
  }
}


void saveRecipe(){
  boolean saverecipeLoop;
  byte numRicetta = 0;
   
  for(byte i=90; i<100; i++){//Trova la prima ricetta libera
    if (EEPROM.read(i)==0){
      numRicetta = (i - 89);
      i=99;
    }
  }
  
  if (numRicetta == 0) MemoriaPiena();
  else {
    byte Verso=0;
    unsigned long Timer=0;
    // Spc 32
    // 0-9 da  48 a  57
    // A-Z da  65 a  90
    // a-z da  97 a 122
  
    byte NomeRicetta[10];
    byte pos = 0; 
    //NomeRicetta[pos] = 32;
    NomeRicetta[pos] = 97;
  
    Clear_2_3();
  
    Ricetta(numRicetta,1);

    while (pos<10){
      LCD_NomeRicetta(pos,NomeRicetta[pos] );
      lcd.blink();
    
      LeggiPulsante(Verso, Timer);
      Set(NomeRicetta[pos],122,32,1,Timer,Verso);
    
      if ((NomeRicetta[pos]> 32 && NomeRicetta[pos]< 48) && Verso==1)NomeRicetta[pos]=48;
      if ((NomeRicetta[pos]> 57 && NomeRicetta[pos]< 97) && Verso==1)NomeRicetta[pos]=97;
    
      if ((NomeRicetta[pos]< 97 && NomeRicetta[pos]> 57) && Verso==2)NomeRicetta[pos]=57;
      if ( NomeRicetta[pos]< 48 && Verso==2)NomeRicetta[pos]=32;
      
      if(btn_Press(Button_enter,50)){
        pos++;
        //NomeRicetta[pos]=32;
        NomeRicetta[pos]=97;
      }
    
      if ((digitalRead(Button_dn)==0) && (digitalRead(Button_up)==0)){
        delay(350);
        if ((digitalRead(Button_dn)==0) && (digitalRead(Button_up)==0)){
          for (byte j=pos; j<10; j++){
            NomeRicetta[pos]=32;
            pos++;
          }pos=9;
        }
      }    
      
      if(btn_Press(Button_start,50)){
        if(pos>0)pos--;
      }
      
    }
  
    lcd.noBlink();
  
    SalvataggioRicetta (numRicetta);
  
    wait_for_confirm(saverecipeLoop,0,2);
  
    if (saverecipeLoop==false){
      Menu_3();
      Menu_3_4();
      return;
    }else{
      SalvaRicetta();
      //SwapRecipe(numRicetta, 1);
      
      int Da;
  
      //Parametri Ricetta
      Da= 100 + ((numRicetta-1)*52);
      for (byte j=30; j<82; j++){ 
        save_set (Da, (byte)EEPROM.read(j));
        //delay(75);
        Da++;
      }
      
      
      //Nome Ricetta
      for (pos=0; pos<10; pos++){
        save_set(620 + pos + ((numRicetta - 1)*10),NomeRicetta[pos]);
        //delay(75);
        //EEPROM.write(620 + pos + ((numRicetta - 1)*10),NomeRicetta[pos]);
      }
      //Byte di Controllo
      save_set(89+numRicetta,(byte)1);
      //EEPROM.write(89+numRicetta,1);
    }
  }
}

void deleteRecipe(){
  byte numRicetta = 0;
  boolean ricettaLoop = true;
  
  byte Verso=0;
  unsigned long Timer=0; 
  
  byte RicettaUp, RicettaDwn;
  RicettaUp=0;
  RicettaDwn=0;
  
  for(byte i=90; i<100; i++){//Assegna il limite di ricette registrate 
    if (EEPROM.read(i)==1){
      RicettaUp = (i-89);
      if(RicettaDwn==0)RicettaDwn = RicettaUp;
    }
  }
  if (RicettaUp==0){
    NoRecipe();
    return;
  }
  
  for(byte i=RicettaDwn + 89; i<RicettaUp + 89 + 1; i++){//Trova la prima ricetta libera
    numRicetta=i-89;
    
    while (ricettaLoop){
      CancelloRicetta(numRicetta);
      LeggiPulsante(Verso,Timer);
      Set(numRicetta, RicettaUp, RicettaDwn, 1, Timer, Verso);
      
      Congruita(numRicetta, Verso);
      if (btn_Press(Button_enter,50)){
        Menu_3(); 
        Menu_3_4();
        ricettaLoop = false;  
        i=100;
      }
      if (btn_Press(Button_start,50)){
        Cancellazione(numRicetta);
        save_set(89+numRicetta,(byte)0);
        //EEPROM.write(89+numRicetta,0);
        ricettaLoop = false;  
        i=100;  
      }
    }
  }
}

void initializeRecipe(){
  boolean initialize;
  
  Inizializzazione();
  
  wait_for_confirm(initialize,0,2);
  
  if (initialize==false){
    Menu_3();
    Menu_3_4();
    return;
  }else{
    Inizializza();      	
    for(byte i=1; i<11; i++){
      save_set(89+i,(byte)0);
      //EEPROM.write(89+i,0);
    }	
  }
}

void RecipeMenu(){
  boolean recipeLoop = true;
  byte recipeMenu = 0;
  
  while (recipeLoop){
    switch (recipeMenu){ 
    
      case(0):
        Menu_3_4_1();
        if (btn_Press(Button_start,50))recipeLoop=false;
        if (btn_Press(Button_dn,50))recipeMenu = 1;
        if (btn_Press(Button_enter,50))loadRecipe(); 
        break;

      case(1):
        Menu_3_4_2();
        if (btn_Press(Button_start,50))recipeLoop=false;
        if (btn_Press(Button_up,50))recipeMenu = 0;
        if (btn_Press(Button_dn,50))recipeMenu = 2;
        if (btn_Press(Button_enter,50))saveRecipe(); 
        break;

      case(2):
        Menu_3_4_3();
        if (btn_Press(Button_start,50))recipeLoop=false;
        if (btn_Press(Button_up,50))recipeMenu = 1;
        if (btn_Press(Button_dn,50))recipeMenu = 3;
        if (btn_Press(Button_enter,50))deleteRecipe();
        break;

      case(3):
        Menu_3_4_4();
        if (btn_Press(Button_start,50))recipeLoop=false;
        if (btn_Press(Button_up,50))recipeMenu = 2;
        if (btn_Press(Button_enter,50))initializeRecipe();
        break;
    }        
  }Menu_3_4();
}
