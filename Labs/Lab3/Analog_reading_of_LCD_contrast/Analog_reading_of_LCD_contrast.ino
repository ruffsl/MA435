#include <LiquidCrystal.h>

#define PIN_CONTRAST_ANALOG 8

LiquidCrystal lcd(14, 15, 16, 17, 18, 19, 20);
int analogReading = 0;

void setup() {
  pinMode(PIN_CONTRAST_ANALOG, INPUT);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
}

void loop() {
  analogReading = analogRead(PIN_CONTRAST_ANALOG);
  
  lcd.setCursor(1, 0);
  lcd.print("Reading = ");
  lcd.print(analogReading);
  lcd.print("  ");
  lcd.setCursor(0, 1);
  lcd.print("Voltage = ");
  lcd.print( (float) analogReading / 1023.0 * 5.0);
  lcd.print("  ");
}
