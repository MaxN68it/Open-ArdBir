void save_settings (byte addr, int data){
  EEPROM.write(addr,highByte(data));
  EEPROM.write((addr+1),lowByte(data));
/*
  Serial.print (F("WRITE--> "));
  Serial.print (F("Addr("));
  Serial.print (addr);
  Serial.print (F(")"));
  Serial.print (F(" Data word("));
  Serial.print (data); 
  Serial.println (F(")"));
*/
}  

void save_settings (byte addr, byte data){
  EEPROM.write(addr,data);
/*
  Serial.print (F("WRITE--> "));
  Serial.print (F("Addr("));
  Serial.print (addr);
  Serial.print (F(")"));  
  Serial.print (F(" Data byte("));
  Serial.print (data);
  Serial.println (F(")"));
*/
}
