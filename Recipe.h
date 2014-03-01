void scriviRicetta(int Scrivi,int Leggi){
  EEPROM.write(Scrivi, EEPROM.read(Leggi));
}  

void SwapRecipe(byte NumRicetta, byte LoadSave){
  // 0 = Load
  // 1 = Save
  int Da;
  
  //Parametri Ricetta
  Da= 100 + ((NumRicetta-1)*47);
  for (byte j=30; j<65; j++){    
    if(LoadSave==0)scriviRicetta(j,Da);
    else scriviRicetta(Da,j);
    Da++;
  }
  
  for (byte j=70; j<82; j++){    
    if(LoadSave==0)scriviRicetta(j,Da);
    else scriviRicetta(Da,j);
    Da++;
  }
}


byte Congruita(byte& numRicetta, byte Verso){
  if (EEPROM.read(89+numRicetta)==0){
    boolean Controllo=true;
    byte numControllo=numRicetta; 
    
    while(Controllo){
      if(Verso==1)if(numControllo<10)numControllo++;
      else Controllo=false; 
        
      if(Verso==2)if(numControllo>1)numControllo--;
      else Controllo=false;
        
      if (EEPROM.read(89+numControllo)==1){
        numRicetta=numControllo;
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
  
  for(byte i=RicettaDwn+89; i<RicettaUp+89+1; i++){//Trova la prima ricetta libera
    numRicetta=i-89;
    
    while (ricettaLoop){
      CaricaRicetta(numRicetta);
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
        LeggoRicetta(numRicetta);
        SwapRecipe(numRicetta, 0);
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

  SalvataggioRicetta (numRicetta);
  wait_for_confirm(saverecipeLoop,0);
  
  if (saverecipeLoop==false){
    Menu_3();
    Menu_3_4();
    return;
  }else{
    SalvaRicetta();
    SwapRecipe(numRicetta, 1);
    //Byte di Controllo
    EEPROM.write(89+numRicetta,1);
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
  
  for(byte i=RicettaDwn+89; i<RicettaUp+89+1; i++){//Trova la prima ricetta libera
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
        EEPROM.write(89+numRicetta,0);
        ricettaLoop = false;  
        i=100;  
      }
    }
  }
}

void initializeRecipe(){
  boolean initialize;
  
  Inizializzazione();
  
  wait_for_confirm(initialize,0);
  
  if (initialize==false){
    Menu_3();
    Menu_3_4();
    return;
  }else{
    Inizializza();      	
    for(byte i=1; i<11; i++){
      EEPROM.write(89 + i,0);
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
