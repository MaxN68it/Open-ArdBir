// sensor and lcd
OneWire ds(7);
#if Dallas == true
  DallasTemperature sensors(&ds);
#endif

LiquidCrystal lcd(A4, A5, 2, 3, 4, 5);

// push buttons
const char Button_dn    = A3;
const char Button_up    = A2;
const char Button_enter = A1;
const char Button_start = A0;

// outputs
const byte Pump = 6;
const byte Buzz = 8;
const byte Heat = 9;


