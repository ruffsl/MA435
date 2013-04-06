/**
 * Analog, Digital, LCD starting point template
 */
#include <LiquidCrystal.h>
#include <Servo.h>
#include <ArmServos.h>
#include <RobotAsciiCom.h>

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

int jointValues[] = {0,90,0,-90,90,71};
int homePosition[] = {0,90,0,-90,90,71};
char buffer[33];
unsigned char joint = 0;

/*** Servos ***/
//ArmServos robotArm(12, 11, 10, 9, 8, 6);
ArmServos robotArm;
RobotAsciiCom robotCom; // Construct an instance of the class

void setup() {
  Serial.begin(9600);
  delay(100);
  robotCom.registerPositionCallback(positionCallback);
  robotCom.registerJointAngleCallback(jointAngleCallback);
  robotCom.registerGripperCallback(gripperCallback);  
  robotArm.attach();
  
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

void positionCallback(int joint1Angle, int joint2Angle, int joint3Angle, int joint4Angle, int joint5Angle) {
  robotArm.setPosition(joint1Angle, joint2Angle, joint3Angle, joint4Angle, joint5Angle);
  jointValues[0] = joint1Angle;
  jointValues[1] = joint2Angle;
  jointValues[2] = joint3Angle;
  jointValues[3] = joint4Angle;
  jointValues[4] = joint5Angle;
}

void jointAngleCallback(byte jointNumber, int jointAngle) {
  robotArm.setJointAngle(jointNumber, jointAngle);
  jointValues[jointNumber-1] = jointAngle;
}

void gripperCallback(int gripperDistance) {
  robotArm.setGripperDistance(gripperDistance);
  jointValues[5] = gripperDistance;
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
       jointValues[joint] = homePosition[joint];
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
    delay(100);
  }
  else if(horzReading<=100){
    jointValues[joint] -=1;
    delay(100);
  }
  
   if(vertReading>=900){
    jointValues[joint] += 4;
    delay(100);
  }
  else if(vertReading<=100){
    jointValues[joint] -=4;
    delay(100);
  }
  // Limit each joint using the DH angle limits.
  jointValues[0] = constrain(jointValues[0], -90, 90);
  jointValues[1] = constrain(jointValues[1], 0, 180);
  jointValues[2] = constrain(jointValues[2], -90, 90);
  jointValues[3] = constrain(jointValues[3], -180, 0);
  jointValues[4] = constrain(jointValues[4], 0, 180);
  jointValues[5] = constrain(jointValues[5], 0, 71);
  for(char i = 0; i < 3; i++){
    lcd.setCursor(i*5, LINE_1);
    itoa(jointValues[i], buffer, 10);
    lcd.print(buffer);
    lcd.print("  ");
  }
  
  for(char i = 0; i < 3; i++){
    lcd.setCursor(i*5, LINE_2);
    itoa(jointValues[i+3], buffer, 10);
    lcd.print(buffer);
    lcd.print("  ");
  }
  
  // do a bunch of stuff
  robotArm.setJointAngle(1, jointValues[0]);
  robotArm.setJointAngle(2, jointValues[1]);
  robotArm.setJointAngle(3, jointValues[2]);
  robotArm.setJointAngle(4, jointValues[3]);
  robotArm.setJointAngle(5, jointValues[4]);
  robotArm.setGripperDistance(jointValues[5]);
}

void serialEvent() {
  while (Serial.available()) {
    robotCom.handleRxByte(Serial.read());
    Serial.write("RECIEVED\n");
  }
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
