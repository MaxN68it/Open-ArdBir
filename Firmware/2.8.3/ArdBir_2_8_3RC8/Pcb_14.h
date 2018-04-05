// sensor and lcd
OneWire ds(8);
#if Dallas == true
  DallasTemperature sensors(&ds);
#endif

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// push buttons
const char Button_up    = A3;
const char Button_dn    = A2;
const char Button_start = A1;
const char Button_enter = A0;

// outputs
const byte Heat = 11;
const byte Pump = 9;
const byte Buzz = 10;


