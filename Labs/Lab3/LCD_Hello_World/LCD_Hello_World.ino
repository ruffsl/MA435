#include <LiquidCrystal.h>

#define PIN_RIGHT_BUTTON 2
#define PIN_LEFT_BUTTON 3

LiquidCrystal lcd(14, 15, 16, 17, 18, 19, 20);
int age = 22;

void setup() {
  pinMode(PIN_LEFT_BUTTON, INPUT_PULLUP);
  pinMode(PIN_RIGHT_BUTTON, INPUT_PULLUP);
  
  attachInterrupt(0, int0_isr, FALLING);
  attachInterrupt(1, int1_isr, FALLING);
  
  lcd.begin(16, 2);
  lcd.print("Spencer Carver");
}

void loop() {
  // lcd.setCursor(column, line);
  // Set the cursor to column 0, line 1
  // If line = 1 that’s the second row, since counting begins with 0
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print("is ");
  lcd.print(age);
  lcd.print("   ");
  
  
}

void int0_isr() {
  age++;
}
void int1_isr() {
  age--;
}
