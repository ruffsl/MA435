#include <Max3421e.h>
#include <Usb.h>
#include <AndroidAccessory.h>
#include <LiquidCrystal.h>
#include <RobotAsciiCom.h>
#include <WildThumperCom.h>
#include <GolfBallStand.h>

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
#define BATTERY _VOLTAGE_REPLY_LENGTH 27
byte txBuf[64];
LiquidCrystal lcd(14, 15, 16, 17, 18, 19, 20);
#define LINE_1 0
#define LINE_2 1

RobotAsciiCom robotCom;
WildThumperCom wildThumperCom(0);
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
#define FLAG_NEED_TO_SEND_BATTERY_VOLTAGE 0x08

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
#define NO_BALL_DETECTED 0
#define MOVING_BALL_TO_SENSOR 1
#define HANDLING_GOOD_BALL 2
#define HANDLING_BAD_BALL 3
int STAGE = NO_BALL_DETECTED;

void setup()  {
  Serial.begin(9600);
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
  // Register callbacks for commands you might receive from Android.
  robotCom.registerWheelSpeedCallback(wheelSpeedMessageFromAndroid);
  robotCom.registerPositionCallback(positionMessageFromAndroid);
  robotCom.registerJointAngleCallback(jointAngleMessageFromAndroid);
  robotCom.registerGripperCallback(gripperMessageFromAndroid);
  robotCom.registerBatteryVoltageRequestCallback(batteryVoltageRequestFromAndroid);
  // Note, not using the wheel current feature.
  // Register callbacks for commands you might receive from the Wild Thumper.
  wildThumperCom.registerBatteryVoltageReplyCallback(batteryVoltageReplyFromThumper);
  lcd.clear();
  lcd.print("Ready");
  delay(1500);
  acc.powerOn();
}

void wheelSpeedMessageFromAndroid(byte leftMode, byte rightMode, byte leftDutyCycle, byte rightDutyCycle) {
  wildThumperCom.sendWheelSpeed(leftMode, rightMode, leftDutyCycle, rightDutyCycle);  
  lcd.clear();
  lcd.print("Wheel speed:");
  lcd.setCursor(0, LINE_2);
  lcd.print("L");
  lcd.print(leftMode);
  lcd.print(" R");
  lcd.print(rightMode);
  lcd.print(" L");
  lcd.print(leftDutyCycle);
  lcd.print(" R");
  lcd.print(rightDutyCycle);
}

void positionMessageFromAndroid(int joint1Angle, int joint2Angle, int joint3Angle, int joint4Angle, int joint5Angle) {
  wildThumperCom.sendPosition(joint1Angle, joint2Angle, joint3Angle, joint4Angle, joint5Angle);  
  /*lcd.clear();
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
  lcd.print(joint5Angle);*/
}

void jointAngleMessageFromAndroid(byte jointNumber, int jointAngle) {
  wildThumperCom.sendJointAngle(jointNumber, jointAngle);
  /*lcd.clear();
  lcd.print("Joint angle:");
  lcd.setCursor(0, LINE_2);
  lcd.print("J");
  lcd.print(jointNumber);
  lcd.print(" move to ");
  lcd.print(jointAngle);*/
}

void gripperMessageFromAndroid(int gripperDistance) {
  if (gripperDistance < 10) {
    gripperDistance = 10;
  }
  wildThumperCom.sendGripperDistance(gripperDistance);
  /*lcd.clear();
  lcd.print("Gripper:");
  lcd.setCursor(0, LINE_2);
  lcd.print("Gripper to ");
  lcd.print(gripperDistance);*/
}   

void batteryVoltageRequestFromAndroid(void) {
  wildThumperCom.sendBatteryVoltageRequest();
}

void batteryVoltageReplyFromThumper(int batteryMillivolts) {
  // Send to Android from within the main event loop.
  mainEventFlags |= FLAG_NEED_TO_SEND_BATTERY_VOLTAGE;
  sprintf((char *) txBuf, "BATTERY VOLTAGE REPLY %d.%03d", batteryMillivolts / 1000, batteryMillivolts % 1000);  
  
  // Display battery voltage on LCD.
  lcd.clear();
  lcd.print("Battery voltage:");
  lcd.setCursor(0, LINE_2);
  lcd.print(batteryMillivolts / 1000);
  lcd.print(".");
  if (batteryMillivolts % 1000  < 100) {
    lcd.print("0");
  }
  if (batteryMillivolts % 1000 < 10) {
    lcd.print("0");
  }
  lcd.print(batteryMillivolts % 1000);
}

void loop(){
  // See if there is a new message from Android.
  if (acc.isConnected()) {
    int len = acc.read(rxBuf, sizeof(rxBuf), 1);
    if (len > 0) {
      robotCom.handleRxBytes(rxBuf, len);
    }
    if (mainEventFlags & FLAG_INTERRUPT_0) {
      delay(20);
      mainEventFlags &= ~FLAG_INTERRUPT_0;
      if (!digitalRead(PIN_RIGHT_BUTTON)) {
        acc.write(ball_present, sizeof(ball_present));
      }
    }
    if (mainEventFlags & FLAG_INTERRUPT_1) {
      delay(20);
      mainEventFlags &= ~FLAG_INTERRUPT_1;
      if (!digitalRead(PIN_LEFT_BUTTON)) {
        acc.write(good_ball, sizeof(good_ball));
      }
    }
    if (mainEventFlags & FLAG_INTERRUPT_2) {
      delay(20);
      mainEventFlags &= ~FLAG_INTERRUPT_2;
      if (!digitalRead(PIN_SELECT_BUTTON)) {
        acc.write(bad_ball, sizeof(bad_ball));
      }
    }
    if (mainEventFlags & FLAG_NEED_TO_SEND_BATTERY_VOLTAGE) {
      mainEventFlags &= ~FLAG_NEED_TO_SEND_BATTERY_VOLTAGE;
      acc.write(txBuf, BATTERY_VOLTAGE_REPLY_LENGTH);
    }
    if ((stand.getAnalogReading(LOCATION_EXTERNAL) > 950) && (STAGE == NO_BALL_DETECTED)) {
      acc.write(ball_present, sizeof(ball_present));
      STAGE = MOVING_BALL_TO_SENSOR;
      lcd.setCursor(0, LINE_2);
      lcd.print("BALL DETECTED");
    }
    if ((stand.getAnalogReading(LOCATION_2) > 950) && (STAGE == MOVING_BALL_TO_SENSOR)) {
      delay(2000);
      double errors[] = {0, 0, 0, 0, 0, 0};
      int val = stand.determineBallColor(LOCATION_2, errors);
      lcd.setCursor(0, LINE_1);
      lcd.print(errors[0]);
      lcd.print("   ");
      lcd.print(errors[1]);
      lcd.print("   ");
      lcd.print(errors[2]);
      lcd.setCursor(0, LINE_2);
      lcd.print(errors[3]);
      lcd.print("   ");
      lcd.print(errors[4]);
      lcd.print("   ");
      lcd.print(errors[5]);
      if (val >= 0) {
        acc.write(good_ball, sizeof(good_ball));
        lcd.setCursor(0, LINE_2);
        lcd.print("GOOD BALL");
        STAGE= HANDLING_GOOD_BALL;
      } else {
        acc.write(bad_ball, sizeof(bad_ball));
        lcd.setCursor(0, LINE_2);
        lcd.print("BAD BALL");
        STAGE= HANDLING_BAD_BALL;
      }
    }
    if (STAGE == HANDLING_GOOD_BALL) {
      STAGE = NO_BALL_DETECTED;
    } else if (STAGE == HANDLING_BAD_BALL) {
      STAGE = NO_BALL_DETECTED;
    }
  }
  
  // See if there is a new message from the Wild Thumper.
  if (Serial.available() > 0) {
    wildThumperCom.handleRxByte(Serial.read());
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
