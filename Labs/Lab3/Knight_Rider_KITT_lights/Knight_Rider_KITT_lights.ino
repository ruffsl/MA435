/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
#define PIN_LED_1 64
#define PIN_LED_2 65
#define PIN_LED_3 66
#define PIN_LED_4 67
#define PIN_LED_5 68
#define PIN_LED_6 69


// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);  
  pinMode(PIN_LED_3, OUTPUT);  
  pinMode(PIN_LED_4, OUTPUT);  
  pinMode(PIN_LED_5, OUTPUT);  
  pinMode(PIN_LED_6, OUTPUT);    
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(PIN_LED_1, HIGH);   
  delay(100);          
  digitalWrite(PIN_LED_1, LOW);  
  digitalWrite(PIN_LED_2, HIGH);
  delay(100);
  digitalWrite(PIN_LED_2, LOW);
  digitalWrite(PIN_LED_3, HIGH);
  delay(100);
  digitalWrite(PIN_LED_3, LOW);
  digitalWrite(PIN_LED_4, HIGH);
  delay(100);
  digitalWrite(PIN_LED_4, LOW);
  digitalWrite(PIN_LED_5, HIGH);
  delay(100);
  digitalWrite(PIN_LED_5, LOW);
  digitalWrite(PIN_LED_6, HIGH);
  delay(100);
  digitalWrite(PIN_LED_6, LOW);
  digitalWrite(PIN_LED_5, HIGH);
  delay(100);
  digitalWrite(PIN_LED_5, LOW);
  digitalWrite(PIN_LED_4, HIGH);
  delay(100);
  digitalWrite(PIN_LED_4, LOW);
  digitalWrite(PIN_LED_3, HIGH);
  delay(100);
  digitalWrite(PIN_LED_3, LOW);
  digitalWrite(PIN_LED_2, HIGH);
  delay(100);
  digitalWrite(PIN_LED_2, LOW);
}
