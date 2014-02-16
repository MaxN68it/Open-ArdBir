void display_lcd (byte pos , byte line ,const char* lable){
  lcd.setCursor(pos,line);
  lcd.print(lable);
}

