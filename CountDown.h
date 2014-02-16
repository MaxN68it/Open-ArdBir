void CountDown(int Tempo, byte posX, byte posY, byte numH){
  long Ore, Minuti, Secondi;
  Ore= int(Tempo/3600);
  Minuti= int((Tempo-(Ore*3600))/60);
  Secondi= int(Tempo-((Ore*3600)+(Minuti*60)));
     
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
