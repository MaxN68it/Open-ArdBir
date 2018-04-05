void ArdBir1 (byte posX, byte posY){
   // Ard Symbol
  byte Ard1[8] = {B00011, B00111, B00111, B01110, B01110, B11100, B11100, B11100};
  byte Ard2[8] = {B11000, B11100, B11100, B01110, B01110, B00111, B00111, B00111};
  byte Ard3[8] = {B11000, B11111, B11111, B11111, B11100, B11100, B11100, B11100};
  byte Ard4[8] = {B00111, B11111, B11111, B11111, B00111, B00111, B00111, B00111};

  // Bir Symbol
  //byte Bir1[8] = {B01111, B11111, B11111, B11100, B11100, B11100, B11111, B11111};
  //byte Bir2[8] = {B11000, B11100, B11110, B01111, B00111, B01111, B11110, B11100};
  //byte Bir3[8] = {B11111, B11100, B11100, B11100, B11100, B11111, B11111, B01111};
  //byte Bir4[8] = {B11110, B00111, B00111, B00111, B01110, B11110, B11100, B11000};

  byte Bir1[8] = {B11111, B11111, B11100, B11100, B11100, B11100, B11111, B11111};
  byte Bir2[8] = {B11100, B11110, B00111, B00011, B00011, B00111, B11110, B11110};
  
  
  lcd.createChar(1, Ard1);
  lcd.createChar(2, Ard2);
  lcd.createChar(3, Ard3);
  lcd.createChar(4, Ard4);
  lcd.createChar(5, Bir1);
  lcd.createChar(6, Bir2);
  //lcd.createChar(7, Bir3);
  //lcd.createChar(8, Bir4);

  byte Count=1;

  lcd.clear();

  for(byte y=0; y<2; y++){
    for (byte x=0; x<2; x++){
      lcd.setCursor(posX+x,posY+y);
      lcd.write(Count);
      //lcd.setCursor(posX+x+4,posY+y);
      //lcd.write(4+Count);
      Count++;
    }
    lcd.setCursor(posX+4,posY+y);
    lcd.write(5);
    lcd.write(6);
  }
  display_lcd(posX+2,posY+1,"rd",0);
  display_lcd(posX+6,posY+1,"ir",4500);
}


