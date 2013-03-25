#include <LiquidCrystal.h>

/***  Pin I/O   ***/ 
#define PIN_LED_1 64
#define PIN_LED_2 65
#define PIN_LED_3 66
#define PIN_LED_4 67
#define PIN_LED_5 68
#define PIN_LED_6 69
#define PIN_RIGHT_BUTTON 2
#define PIN_LEFT_BUTTON 3
#define PIN_SELECT_BUTTON 21
#define PIN_CONTRAST_ANALOG 8
#define PIN_HORZ_ANALOG 0
#define PIN_VERT_ANALOG 1

LiquidCrystal lcd(14, 15, 16, 17, 18, 19, 20);
int Horz = 0;
int Verz = 0;

void setup() {
  pinMode(PIN_HORZ_ANALOG, INPUT);
  pinMode(PIN_VERT_ANALOG, INPUT);
  pinMode(PIN_SELECT_BUTTON, INPUT);
  pinMode(PIN_RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(PIN_LEFT_BUTTON, INPUT_PULLUP);
  
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);
  pinMode(PIN_LED_4, OUTPUT);
  pinMode(PIN_LED_5, OUTPUT);
  pinMode(PIN_LED_6, OUTPUT);
    
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
}

void loop() {
  Horz = analogRead(PIN_HORZ_ANALOG);
  Verz = analogRead(PIN_VERT_ANALOG);
  
  lcd.setCursor(1, 0);
  lcd.print("Horz = ");
  lcd.print( Horz );
  lcd.print("  ");
  lcd.setCursor(0, 1);
  lcd.print("Verz = ");
  lcd.print( Verz);
  lcd.print("  ");
  
  if(!digitalRead(PIN_SELECT_BUTTON)){
    digitalWrite(PIN_LED_1, HIGH);
  }
  else{
    digitalWrite(PIN_LED_1, LOW);  
  }
  
  if(!digitalRead(PIN_LEFT_BUTTON)){
    digitalWrite(PIN_LED_4, HIGH);
  }
  else{
    digitalWrite(PIN_LED_4, LOW);  
  }
  
  if(!digitalRead(PIN_RIGHT_BUTTON)){
    digitalWrite(PIN_LED_6, HIGH);
  }
  else{
    digitalWrite(PIN_LED_6, LOW);  
  }
  
}
