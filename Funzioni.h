float ConvertiCtoF(float& Num){
  Num = (Num*1.8)+32;          // Converto in Â°F
}
int ConvertiCtoF(int& Num){
  Num = (Num*1.8)+32;          // Converto in Â°F
}

float ConvertiFtoC(float& Num){
Num = (Num-32)/1.8;            // Converto in Â°C
}
int ConvertiFtoC(int& Num){
Num = (Num-32)/1.8;            // Converto in Â°C
}

float Arrotonda025(float& Num){
  int Appoggio;
  // Appoggio la parte intera
  Appoggio = (int)int(Num);           
  // Arrotondo il valore con peso 0.25
  Num=Appoggio+int((Num-Appoggio)*1000/225)*0.25;
}
/*
float change_set(float& set_change,int upper_limit, int lower_limit, float step_size){
  // Increase set
  if (btn_Repeat(Button_up)){
    if (set_change >= upper_limit) set_change = upper_limit;
    if (set_change < lower_limit) set_change = lower_limit;
    
    if (set_change < upper_limit)set_change += step_size;
  }
  // decrease set
  if (btn_Repeat(Button_dn)){
    if (set_change <= lower_limit) set_change = lower_limit;
    if (set_change > upper_limit) set_change = upper_limit; 
    
    if (set_change > lower_limit)set_change -= step_size;  
  }
}

int change_set(int& set_change, int upper_limit, int lower_limit, int step_size){
  // Increase set
  if (btn_Repeat(Button_up)){
    if (set_change >= upper_limit) set_change = upper_limit;
    if (set_change < lower_limit) set_change = lower_limit;
    
    if (set_change < upper_limit)set_change += step_size;
  }
  // decrease set
  if (btn_Repeat(Button_dn)){  
    if (set_change <= lower_limit) set_change = lower_limit;
    if (set_change > upper_limit) set_change = upper_limit; 
    
    if (set_change > lower_limit)set_change -= step_size;
  }
}

int change_set(byte& set_change, int upper_limit, int lower_limit, int step_size){
  // Increase set
  if (btn_Repeat(Button_up)){
    if (set_change >= upper_limit) set_change = upper_limit;
    if (set_change < lower_limit) set_change = lower_limit;
    
    if (set_change < upper_limit)set_change += step_size;
  }
  // decrease set
  if (btn_Repeat(Button_dn)){
    if (set_change <= lower_limit) set_change = lower_limit;
    if (set_change > upper_limit) set_change = upper_limit; 
    
    if (set_change > lower_limit)set_change -= step_size;  
  }
}

int change_up_dwn(int& change,int upper_limit,int lower_limit){
  // Increase set temp
  if (btn_Repeat(Button_up)){
    change++; 
    if (change > upper_limit)change = upper_limit;
  }
  // decrease temp
  if (btn_Repeat(Button_dn)){
    change--;
    if ( change < lower_limit) change = lower_limit;
  }
}

// change between upper and lower limit using up down buttons
float change_up_dwn(float& change, int upper_limit, int lower_limit, float weight){
  // Increase set
  if (btn_Repeat(Button_up)){
    change+=weight;
    
    if (change > upper_limit)change = upper_limit;
  }
  // decrease temp
  if (btn_Repeat(Button_dn)){
    change-=weight;
    if (change < lower_limit)change = lower_limit;
  }
}
*/
int Set(int& Set, int Up, int Low, int Step, long Timer, byte Verso){
  int step_size;
  int ControllaPulsante;
	
  delay(35);
  if(Verso==1){
    if(Set<Low)Set=Low;
    if(Set>=Up)Set=Up;
    ControllaPulsante=digitalRead(Button_up);
  }
  if(Verso==2){
    if(Set>Up)Set=Up;
    if(Set<=Low)Set=Low;
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

float Set(float& Set, int Up, int Low, float Step, long Timer, byte Verso){
  float step_size;
  int ControllaPulsante;
	
  delay(35);
  if(Verso==1){
    if(Set<Low)Set=Low;
    if(Set>=Up)Set=Up;
    ControllaPulsante=digitalRead(Button_up);
  }
  if(Verso==2){
    if(Set>Up)Set=Up;
    if(Set<=Low)Set=Low;
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

byte Set(byte& Set, int Up, int Low, int Step, long Timer, byte Verso){
  int step_size;
  int ControllaPulsante;
	
  delay(35);
  if(Verso==1){
    if(Set<Low)Set=Low;
    if(Set>=Up)Set=Up;
    ControllaPulsante=digitalRead(Button_up);
  }
  if(Verso==2){
    if(Set>Up)Set=Up;
    if(Set<=Low)Set=Low;
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
