/**
 * Analog, Digital, LCD starting point template
 */
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

#define PIN_HORZ_ANALOG 0
#define PIN_VERT_ANALOG 1
#define PIN_SELECT_BUTTON 21
#define PIN_CONTRAST_ANALOG 8


/*** Interrupt flags ***/
volatile int mainEventFlags = 0;
#define FLAG_INTERRUPT_0 0x01
#define FLAG_INTERRUPT_1 0x02
#define FLAG_INTERRUPT_2 0x04


/***  LCD ***/
LiquidCrystal lcd(14, 15, 16, 17, 18, 19, 20);
#define LINE_1 0
#define LINE_2 1

unsigned char jointValues[] = {90,90,90,90,90,90};
unsigned char joint = 0;


void setup() {
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);
  pinMode(PIN_LED_4, OUTPUT);
  pinMode(PIN_LED_5, OUTPUT);
  pinMode(PIN_LED_6, OUTPUT);
  
  digitalWrite(PIN_LED_1, LOW);
  digitalWrite(PIN_LED_2, LOW);
  digitalWrite(PIN_LED_3, LOW);
  digitalWrite(PIN_LED_4, LOW);
  digitalWrite(PIN_LED_5, LOW);
  digitalWrite(PIN_LED_6, LOW);
  
  pinMode(PIN_LEFT_BUTTON, INPUT_PULLUP);
  pinMode(PIN_RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(PIN_SELECT_BUTTON, INPUT_PULLUP);
  
  attachInterrupt(0, int0_isr, FALLING);
  attachInterrupt(1, int1_isr, FALLING);
  attachInterrupt(2, int2_isr, FALLING);
  lcd.begin(16, 2);
}

void loop() {
  int horzReading = analogRead(PIN_HORZ_ANALOG);
  int vertReading = analogRead(PIN_VERT_ANALOG);
  
  if (mainEventFlags & FLAG_INTERRUPT_0) {
    delay(20);
    mainEventFlags &= ~FLAG_INTERRUPT_0;
    if (!digitalRead(PIN_RIGHT_BUTTON)) {
      joint++;
      if(joint>5){
        joint = 0;
      }
      digitalWrite(PIN_LED_1, LOW);
      digitalWrite(PIN_LED_2, LOW);
      digitalWrite(PIN_LED_3, LOW);
      digitalWrite(PIN_LED_4, LOW);
      digitalWrite(PIN_LED_5, LOW);
      digitalWrite(PIN_LED_6, LOW);
    }
  }
  
  if (mainEventFlags & FLAG_INTERRUPT_1) {
    delay(20);
    mainEventFlags &= ~FLAG_INTERRUPT_1;
    if (!digitalRead(PIN_LEFT_BUTTON)) {
      joint--;
      if(joint>5){
        joint = 5;
      }
      digitalWrite(PIN_LED_1, LOW);
      digitalWrite(PIN_LED_2, LOW);
      digitalWrite(PIN_LED_3, LOW);
      digitalWrite(PIN_LED_4, LOW);
      digitalWrite(PIN_LED_5, LOW);
      digitalWrite(PIN_LED_6, LOW);
    }
  }
  
  if (mainEventFlags & FLAG_INTERRUPT_2) {
    delay(20);
    mainEventFlags &= ~FLAG_INTERRUPT_2;
    if (!digitalRead(PIN_SELECT_BUTTON)) {
       jointValues[joint] = 90;
    }
  }
  
  switch(joint){
    case 0:
      digitalWrite(PIN_LED_1, HIGH);
      break;
    case 1:
      digitalWrite(PIN_LED_2, HIGH);
      break;
    case 2:
      digitalWrite(PIN_LED_3, HIGH);
      break;
    case 3:
      digitalWrite(PIN_LED_4, HIGH);
      break;
    case 4:
      digitalWrite(PIN_LED_5, HIGH);
      break;
    case 5:
      digitalWrite(PIN_LED_6, HIGH);
      break;
    default:
      break;
  }
  
  if(horzReading>=900){
    jointValues[joint] += 1;
    if(jointValues[joint]>180){
      jointValues[joint] = 180;
    }
    delay(100);
  }
  else if(horzReading<=100){
    jointValues[joint] -=1;
    if(jointValues[joint]>180){
      jointValues[joint] = 0;
    }
    delay(100);
  }
  
   if(vertReading>=900){
    jointValues[joint] += 4;
    if(jointValues[joint]>180){
      jointValues[joint] = 180;
    }
    delay(100);
  }
  else if(vertReading<=100){
    jointValues[joint] -=4;
    if(jointValues[joint]>180){
      jointValues[joint] = 0;
    }
    delay(100);
  }
  
  for(char i = 0; i < 3; i++){
    lcd.setCursor(i*5, LINE_1);
    lcd.print(jointValues[i]);
    lcd.print("  ");
  }
  
  for(char i = 0; i < 3; i++){
    lcd.setCursor(i*5, LINE_2);
    lcd.print(jointValues[i+3]);
    lcd.print("  ");
  }
  
  // do a bunch of stuff
}


// Simple ISRs that set flags only
void int0_isr() {
  mainEventFlags |= FLAG_INTERRUPT_0;
}

void int1_isr() {
  mainEventFlags |= FLAG_INTERRUPT_1;
}

void int2_isr() {
  mainEventFlags |= FLAG_INTERRUPT_2;
}
