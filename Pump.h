void pump_on(){
  digitalWrite(Pump,HIGH);
  ledPumpON(); 
}

void pump_off(){
  digitalWrite(Pump,LOW);
  ledPumpOFF();
}

void pump_prime(){
  PumpPrime();
  
  pump_on();
  delay (1000);
  pump_off();
  delay(250);
  pump_on();
  delay (1250);
  pump_off();
  delay(250);
  pump_on();
  delay (1500);
  pump_off();
  
  Menu_2(); 
}


