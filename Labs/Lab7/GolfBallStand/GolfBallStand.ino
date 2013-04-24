#include <Max3421e.h>
#include <Usb.h>
#include <AndroidAccessory.h>
#include <GolfBallStand.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <ArmServos.h>
#include <ArmServosSpeedControlled.h>
#include <RobotAsciiCom.h>



char manufacturer[] = "Rose-Hulman";
char model[] = "My Sliders And Buttons";
char versionStr[] = "1.0";
        
// Only Manufacturer, Model, and Version matter to Android
AndroidAccessory acc(manufacturer,
                     model,
                     "Basic command set to carry out Sliders and Buttons",
                     versionStr,
                     "https://sites.google.com/site/me435spring2013/",
                     "12345");
byte rxBuf[255];

LiquidCrystal lcd(14, 15, 16, 17, 18, 19, 20);
#define LINE_1 0
#define LINE_2 1

ArmServosSpeedControlled armServos;
RobotAsciiCom robotCom;
char ball_present[] = "present_ball";
char good_ball[] = "good_ball";
char bad_ball[] = "bad_ball";

GolfBallStand stand;
int ballColor_1;
int ballColor_2;
int ballColor_3;

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

/*** Interrupt flags ***/
volatile int mainEventFlags = 0;
#define FLAG_INTERRUPT_0 0x01
#define FLAG_INTERRUPT_1 0x02
#define FLAG_INTERRUPT_2 0x04

int servoAngles[6];
#define RESET_JOINT_1_ANGLE 0
#define RESET_JOINT_2_ANGLE 90
#define RESET_JOINT_3_ANGLE 0
#define RESET_JOINT_4_ANGLE -90
#define RESET_JOINT_5_ANGLE 90
#define RESET_GRIPPER_DISTANCE 50

#define LOW_THRESHOLD 100
#define HIGH_THRESHOLD 900
#define VERT_MOVE_AMOUNT 4
#define HORZ_MOVE_AMOUNT 1

int currentJoint = 0;
#define MIN_CURRENT_JOINT 0
#define MAX_CURRENT_JOINT 5

void setup()  {
  Serial.begin(115200);
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
  servoAngles[1] = RESET_JOINT_1_ANGLE;
  servoAngles[2] = RESET_JOINT_2_ANGLE;
  servoAngles[3] = RESET_JOINT_3_ANGLE;
  servoAngles[4] = RESET_JOINT_4_ANGLE;
  servoAngles[5] = RESET_JOINT_5_ANGLE;
  servoAngles[0] = RESET_GRIPPER_DISTANCE;
  armServos.attach();
  robotCom.registerPositionCallback(positionCallback);
  robotCom.registerJointAngleCallback(jointAngleCallback);
  robotCom.registerGripperCallback(gripperCallback);
  lcd.clear();
  lcd.print("Ready");
  delay(1500);
  acc.powerOn();
}

void positionCallback(int joint1Angle, int joint2Angle, int joint3Angle, int joint4Angle, int joint5Angle) {
  //armServos.setPosition(joint1Angle, joint2Angle, joint3Angle, joint4Angle, joint5Angle);
  armServos.setJointAngle(1, joint1Angle);
  armServos.setJointAngle(2, joint2Angle);
  armServos.setJointAngle(3, joint3Angle);
  armServos.setJointAngle(4, joint4Angle);
  armServos.setJointAngle(5, joint5Angle);
  lcd.clear();
  lcd.print("Position:");
  lcd.setCursor(0, LINE_2);
  lcd.print(joint1Angle);
  lcd.print(" ");
  lcd.print(joint2Angle);
  lcd.print(" ");
  lcd.print(joint3Angle);
  lcd.print(" ");
  lcd.print(joint4Angle);
  lcd.print(" ");
  lcd.print(joint5Angle);
}

void jointAngleCallback(byte jointNumber, int jointAngle) {
  armServos.setJointAngle(jointNumber, jointAngle);
  lcd.clear();
  lcd.print("Joint angle:");
  lcd.setCursor(0, LINE_2);
  lcd.print("J");
  lcd.print(jointNumber);
  lcd.print(" move to ");
  lcd.print(jointAngle);
}

void gripperCallback(int gripperDistance) {
  if (gripperDistance < 10) {
    gripperDistance = 10;
  }
  armServos.setGripperDistance(gripperDistance);
  lcd.clear();
  lcd.print("Gripper:");
  lcd.setCursor(0, LINE_2);
  lcd.print("Gripper to ");
  lcd.print(gripperDistance);
}   

void loop(){
  if (acc.isConnected()) {
    int len = acc.read(rxBuf, sizeof(rxBuf), 1);
    if (len > 0) {
      robotCom.handleRxBytes(rxBuf, len);
    }
    if (stand.getAnalogReading(LOCATION_EXTERNAL) > 950) {
      acc.write(ball_present, sizeof(ball_present));
      delay(12000);
      int val = stand.determineBallColor(LOCATION_2);
      if (val >= 0) {
        acc.write(good_ball, sizeof(good_ball));
        delay(12000);
      } else {
        acc.write(bad_ball, sizeof(bad_ball));
        delay(12000);
      }
    }
  }
//  delay(1000);
//  stand.setLedState(LED_RED, LOCATION_1 | LOCATION_2 | LOCATION_3, LED_UNDER_AND_FRONT);
//  delay(1000);
//  stand.setLedState(LED_GREEN, LOCATION_1 | LOCATION_2 | LOCATION_3, LED_UNDER_AND_FRONT);
//  delay(1000);
//  stand.setLedState(LED_BLUE, LOCATION_1 | LOCATION_2 | LOCATION_3, LED_UNDER_AND_FRONT);
//  delay(1000);
//  stand.setLedState(LED_WHITE, LOCATION_1 | LOCATION_2 | LOCATION_3, LED_UNDER_AND_FRONT);
//  delay(1000);
//  stand.setLedState(LED_OFF, LOCATION_1 | LOCATION_2 | LOCATION_3, LED_UNDER_AND_FRONT);
  
//  int horzReading = analogRead(PIN_HORZ_ANALOG);
//  int vertReading = analogRead(PIN_VERT_ANALOG);
//  if (mainEventFlags & FLAG_INTERRUPT_0) {
//    delay(20);
//    mainEventFlags &= ~FLAG_INTERRUPT_0;
//    if (!digitalRead(PIN_RIGHT_BUTTON)) {
//      currentJoint++;
//      if (currentJoint > MAX_CURRENT_JOINT) {
//        currentJoint = MIN_CURRENT_JOINT;
//      }
//    }
//  }
//  if (mainEventFlags & FLAG_INTERRUPT_1) {
//    delay(20);
//    mainEventFlags &= ~FLAG_INTERRUPT_1;
//    if (!digitalRead(PIN_LEFT_BUTTON)) {
//      currentJoint--;
//      if (currentJoint < MIN_CURRENT_JOINT) {
//        currentJoint = MAX_CURRENT_JOINT;
//      }
//    }
//  }
//  if (mainEventFlags & FLAG_INTERRUPT_2) {
//    delay(20);
//    mainEventFlags &= ~FLAG_INTERRUPT_2;
//    if (!digitalRead(PIN_SELECT_BUTTON)) {
//      switch(currentJoint) {
//        case 0:
//          servoAngles[0] = RESET_GRIPPER_DISTANCE;
//          break;
//        case 1:
//          servoAngles[1] = RESET_JOINT_1_ANGLE;
//          break;
//        case 2:
//          servoAngles[2] = RESET_JOINT_2_ANGLE;
//          break;
//        case 3:
//          servoAngles[3] = RESET_JOINT_3_ANGLE;
//          break;
//        case 4:
//          servoAngles[4] = RESET_JOINT_4_ANGLE;
//          break;
//        case 5:
//          servoAngles[5] = RESET_JOINT_5_ANGLE;
//          break;
//      }
//    }
//  }
//  updateLeds();
//  
//  if (horzReading < LOW_THRESHOLD) {
//    servoAngles[currentJoint] -= HORZ_MOVE_AMOUNT;
//  } else if (horzReading > HIGH_THRESHOLD) {
//    servoAngles[currentJoint] += HORZ_MOVE_AMOUNT;
//  }
//  if (vertReading < LOW_THRESHOLD) {
//    servoAngles[currentJoint] -= VERT_MOVE_AMOUNT;
//  } else if (vertReading > HIGH_THRESHOLD) {
//    servoAngles[currentJoint] += VERT_MOVE_AMOUNT;
//  }
//  // Limit each joint using the DH angle limits.
//  servoAngles[1] = constrain(servoAngles[1], -90, 90);
//  servoAngles[2] = constrain(servoAngles[2], 0, 180);
//  servoAngles[3] = constrain(servoAngles[3], -90, 90);
//  servoAngles[4] = constrain(servoAngles[4], -180, 0);
//  servoAngles[5] = constrain(servoAngles[5], 0, 180);
//  servoAngles[0] = constrain(servoAngles[0], 0, 71);
//  
//  updateLcd();
//  updateServos();
//  delay(100);
}

void updateLeds() {
  digitalWrite(PIN_LED_1, LOW);
  digitalWrite(PIN_LED_2, LOW);
  digitalWrite(PIN_LED_3, LOW);
  digitalWrite(PIN_LED_4, LOW);
  digitalWrite(PIN_LED_5, LOW);
  digitalWrite(PIN_LED_6, LOW);
  switch(currentJoint) {
    case 0:
      digitalWrite(PIN_LED_6, HIGH);
      break;
    case 1:
      digitalWrite(PIN_LED_1, HIGH);
      break;
    case 2:
      digitalWrite(PIN_LED_2, HIGH);
      break;
    case 3:
      digitalWrite(PIN_LED_3, HIGH);
      break;
    case 4:
      digitalWrite(PIN_LED_4, HIGH);
      break;
    case 5:
      digitalWrite(PIN_LED_5, HIGH);
      break;
  }
}

void updateLcd() {
  lcd.setCursor(0, LINE_1);
  lcd.print(servoAngles[1]);
  lcd.print("   ");
  lcd.setCursor(5, LINE_1);
  lcd.print(servoAngles[2]);
  lcd.print("   ");
  lcd.setCursor(10, LINE_1);
  lcd.print(servoAngles[3]);
  lcd.print("   ");
  lcd.setCursor(0, LINE_2);
  lcd.print(servoAngles[4]);
  lcd.print("   ");
  lcd.setCursor(5, LINE_2);
  lcd.print(servoAngles[5]);
  lcd.print("   ");
  lcd.setCursor(10, LINE_2);
  lcd.print(servoAngles[0]);
  lcd.print("   ");
}
  
void int0_isr() {
  mainEventFlags |= FLAG_INTERRUPT_0;
}
void int1_isr() {
  mainEventFlags |= FLAG_INTERRUPT_1;
}
void int2_isr() {
  mainEventFlags |= FLAG_INTERRUPT_2;
}
