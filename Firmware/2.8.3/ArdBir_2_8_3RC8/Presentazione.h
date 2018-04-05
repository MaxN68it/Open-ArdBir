void scriviSprite(byte posX, byte posY){
  byte Count=1;
  
  lcd.clear();
  
  for(byte y=0; y<2; y++){
    for (byte x=0; x<3; x++){
      lcd.setCursor(posX+x,posY+y);
      lcd.write(Count);
      Count++;
    }
  }
 
  //lcd.setCursor(posX+5,posY+0);
}  

void Presentazione(byte posX, byte posY){   
  /*
  // Arduino Symbol
  byte Arduino1[8] = {B00000, B00001, B00011, B00111, B01111, B01100, B11011, B10111};
  byte Arduino2[8] = {B00000, B11111, B11111, B11111, B11111, B01110, B10101, B11011};
  byte Arduino3[8] = {B00000, B10000, B11000, B11100, B11110, B00110, B11011, B11101};
  byte Arduino4[8] = {B10111, B11011, B01100, B01111, B00111, B00011, B00001, B00000};
  byte Arduino5[8] = {B11011, B10101, B01110, B11111, B11111, B11111, B11111, B00000};
  byte Arduino6[8] = {B11101, B11011, B00110, B11110, B11100, B11000, B10000, B00000};
  */
  
  
  // AHB symbol
  byte AHB1[8] = {B00000, B00011, B01111, B11011, B10101, B10001, B10101, B10101};
  byte AHB2[8] = {B11111, B11111, B11111, B10101, B10101, B10001, B10101, B10101};
  byte AHB3[8] = {B00000, B11000, B11110, B10011, B10101, B10011, B10101, B10011};
  byte AHB4[8] = {B11111, B11101, B10110, B01001, B00100, B00011, B00000, B00000};
  byte AHB5[8] = {B11011, B10001, B10101, B11011, B10001, B10001, B11011, B01110};
  byte AHB6[8] = {B11111, B10111, B01101, B10010, B00100, B11000, B00000, B00000};
  
  
  // AREA BIRRA symbol
  byte AreaBirra1[8] = {B00000, B01111, B10000, B10000, B10000, B10000, B10000, B10000};
  byte AreaBirra2[8] = {B00000, B11111, B00100, B00100, B00111, B00111, B00100, B00100};
  byte AreaBirra3[8] = {B00000, B11110, B11001, B11001, B00111, B00111, B11001, B11001};
  byte AreaBirra4[8] = {B11010, B10101, B11010, B10101, B11010, B10101, B01111, B00000};
  byte AreaBirra5[8] = {B10111, B01111, B10111, B01111, B10111, B01111, B11111, B00000};
  byte AreaBirra6[8] = {B11111, B11111, B11111, B11111, B11111, B11111, B11110, B00000};


  /*
  lcd.createChar(1, Arduino1);
  lcd.createChar(2, Arduino2);
  lcd.createChar(3, Arduino3);
  lcd.createChar(4, Arduino4);
  lcd.createChar(5, Arduino5);
  lcd.createChar(6, Arduino6);
    
  scriviSprite(posX, posY);
 
  display_lcd(posX+5,posY+0,"ARDUINO",0);
  display_lcd(posX+5,posY+1,"Open Source",2000);
  
  lcd.setCursor(posX+5,posY+0);
  lcd.print(F("ARDUINO"));
  lcd.setCursor(posX+5,posY+1);
  lcd.print(F("Open Source"));
  delay(2000);
  */

  
  lcd.createChar(1, AHB1);
  lcd.createChar(2, AHB2);
  lcd.createChar(3, AHB3);
  lcd.createChar(4, AHB4);
  lcd.createChar(5, AHB5);
  lcd.createChar(6, AHB6);
  
  scriviSprite(posX, posY);
  
  display_lcd(posX+5,posY+0,"AUSSIE",0);
  display_lcd(posX+5,posY+1,"Home Brewer",2500);
  
  
  
  
  lcd.createChar(1, AreaBirra1);
  lcd.createChar(2, AreaBirra2);
  lcd.createChar(3, AreaBirra3);
  lcd.createChar(4, AreaBirra4);
  lcd.createChar(5, AreaBirra5);
  lcd.createChar(6, AreaBirra6);
    
  scriviSprite(posX, posY);

  display_lcd(posX+5,posY+0,"AREA BIRRA",0);
  display_lcd(posX+5,posY+1,"Forum",2500);
  
}




