#include <Max3421e.h>
#include <Usb.h>
#include <AndroidAccessory.h>
#include <LiquidCrystal.h>
#include <RobotAsciiCom.h>
#include <WildThumperCom.h>

#define TEAM_NUMBER 24  // Replace this with your team number.

char manufacturer[] = "Rose-Hulman";
char model[] = "one_good_gps_reading";
char versionStr[] = "1.0";
        
// Only Manufacturer, Model, and Version matter to Android
AndroidAccessory acc(manufacturer,
                     model,
                     "ME435 robot arm message relay station.",
                     versionStr,
                     "https://sites.google.com/site/me435spring2013/",
                     "12345");

byte rxBuf[255];
char txBuf[64];
int batteryVoltageReplyLength = 0;
int wheelCurrentReplyLength = 0;
// Note, when sending commands to Android I don't add the '\n'.
// Turned out to be easier to just assume the whole message arrives together.
// Seems to work fine.  Just separate into different write() commands.

// Just a random set of scripts that you might make for testing.
char ballPresentScript[] = "ball_present";
char placeBallPortScript[] = "place_ball_port";
char placeBallCenterScript[] = "place_center_port";
char placeBallStarboardScript[] = "place_starboard_port";
char portBallPickupScript[] = "port_ball_pickup";
char centerBallPickupScript[] = "center_ball_pickup";
char starboardBallSPickupcript[] = "starboard_ball_pickup";
char redScript[] = "red";
char whiteScript[] = "white";
char blueScript[] = "blue";
char yellowScript[] = "yellow";
char greenScript[] = "green";
char blackScript[] = "black";

/***  Pin I/O   ***/ 
#define PIN_RIGHT_BUTTON 2
#define PIN_LEFT_BUTTON 3
#define PIN_SELECT_BUTTON 21

/*** Interrupt flags ***/
volatile int mainEventFlags = 0;
#define FLAG_INTERRUPT_0                   0x0001
#define FLAG_INTERRUPT_1                   0x0002
#define FLAG_INTERRUPT_2                   0x0004
#define FLAG_NEED_TO_SEND_BATTERY_VOLTAGE  0x0008
#define FLAG_NEED_TO_SEND_WHEEL_CURRENT    0x0010

//LiquidCrystal lcd(14, 15, 16, 17, 18, 19, 20);
LiquidCrystal lcd(20, 19, 18, 17, 16, 15, 14);
#define LINE_1 0
#define LINE_2 1

RobotAsciiCom robotAsciiCom;
WildThumperCom wildThumperCom(TEAM_NUMBER);

void setup() {
  Serial.begin(9600);  // Change if you are using a different baudrate.
  pinMode(PIN_LEFT_BUTTON, INPUT_PULLUP);
  pinMode(PIN_RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(PIN_SELECT_BUTTON, INPUT_PULLUP);
  attachInterrupt(0, int0_isr, FALLING);
  attachInterrupt(1, int1_isr, FALLING);
  attachInterrupt(2, int2_isr, FALLING);
  lcd.begin(16, 2);
  
  // Register callbacks for commands you might receive from Android.
  robotAsciiCom.registerWheelSpeedCallback(wheelSpeedMessageFromAndroid);
  robotAsciiCom.registerPositionCallback(positionMessageFromAndroid);
  robotAsciiCom.registerJointAngleCallback(jointAngleMessageFromAndroid);
  robotAsciiCom.registerGripperCallback(gripperMessageFromAndroid);
  robotAsciiCom.registerBatteryVoltageRequestCallback(batteryVoltageRequestFromAndroid);
  robotAsciiCom.registerWheelCurrentRequestCallback(wheelCurrentRequestFromAndroid);
  
  // Register callbacks for commands you might receive from the Wild Thumper.
  wildThumperCom.registerBatteryVoltageReplyCallback(batteryVoltageReplyFromThumper);
  wildThumperCom.registerWheelCurrentReplyCallback(wheelCurrentReplyFromThumper);
  
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

void jointAngleMessageFromAndroid(byte jointNumber, int jointAngle) {
  wildThumperCom.sendJointAngle(jointNumber, jointAngle);
  lcd.clear();
  lcd.print("Joint angle:");
  lcd.setCursor(0, LINE_2);
  lcd.print("J");
  lcd.print(jointNumber);
  lcd.print(" move to ");
  lcd.print(jointAngle);
}

void gripperMessageFromAndroid(int gripperDistance) {
  if (gripperDistance < 10) {
    gripperDistance = 10;
  }
  wildThumperCom.sendGripperDistance(gripperDistance);
  lcd.clear();
  lcd.print("Gripper:");
  lcd.setCursor(0, LINE_2);
  lcd.print("Gripper to ");
  lcd.print(gripperDistance);
}   

void batteryVoltageRequestFromAndroid(void) {
  wildThumperCom.sendBatteryVoltageRequest();
}

void wheelCurrentRequestFromAndroid(void) {
  wildThumperCom.sendWheelCurrentRequest();
}

void batteryVoltageReplyFromThumper(int batteryMillivolts) {
  // Send to Android from within the main event loop.
  mainEventFlags |= FLAG_NEED_TO_SEND_BATTERY_VOLTAGE;
  batteryVoltageReplyLength = robotAsciiCom.prepareBatteryVoltageReply(
      batteryMillivolts, txBuf, sizeof(txBuf));
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

void wheelCurrentReplyFromThumper(int leftWheelMotorsMilliamps, int rightWheelMotorsMilliamps) {
  // Send to Android from within the main event loop.
  mainEventFlags |= FLAG_NEED_TO_SEND_WHEEL_CURRENT;
  wheelCurrentReplyLength = robotAsciiCom.prepareWheelCurrentReply(
      leftWheelMotorsMilliamps, rightWheelMotorsMilliamps, txBuf, sizeof(txBuf));

  // Display wheel currents on LCD.
  lcd.clear();
  lcd.print("Wheel current:");
  lcd.setCursor(0, LINE_2);
  lcd.print(leftWheelMotorsMilliamps / 1000);
  lcd.print(".");
  if (leftWheelMotorsMilliamps % 1000  < 100) {
    lcd.print("0");
  }
  if (leftWheelMotorsMilliamps % 1000 < 10) {
    lcd.print("0");
  }
  lcd.print(leftWheelMotorsMilliamps % 1000);
  lcd.print("  ");
  lcd.print(rightWheelMotorsMilliamps / 1000);
  lcd.print(".");
  if (rightWheelMotorsMilliamps % 1000  < 100) {
    lcd.print("0");
  }
  if (rightWheelMotorsMilliamps % 1000 < 10) {
    lcd.print("0");
  }
  lcd.print(rightWheelMotorsMilliamps % 1000);
}

void loop() {
  // See if there is a new message from Android.
  if (acc.isConnected()) {
    int len = acc.read(rxBuf, sizeof(rxBuf), 1);
    if (len > 0) {
      robotAsciiCom.handleRxBytes(rxBuf, len);
    }
    // Testing sending some scripts to Android on button press instead of a real sensor.
    if (mainEventFlags & FLAG_INTERRUPT_0) {
      delay(20);
      mainEventFlags &= ~FLAG_INTERRUPT_0;
      if (!digitalRead(PIN_RIGHT_BUTTON)) {
        acc.write(ballPresentScript, sizeof(ballPresentScript));
      }
    }
    if (mainEventFlags & FLAG_INTERRUPT_1) {
      delay(20);
      mainEventFlags &= ~FLAG_INTERRUPT_1;
      if (!digitalRead(PIN_LEFT_BUTTON)) {
        acc.write(placeBallCenterScript, sizeof(placeBallCenterScript));
      }
    }
    if (mainEventFlags & FLAG_INTERRUPT_2) {
      delay(20);
      mainEventFlags &= ~FLAG_INTERRUPT_2;
      if (!digitalRead(PIN_SELECT_BUTTON)) {
        acc.write(centerBallPickupScript, sizeof(centerBallPickupScript));
      }
    }
    // Passing commands from the Wild Thumper on up to Android.
    if (mainEventFlags & FLAG_NEED_TO_SEND_BATTERY_VOLTAGE) {
      mainEventFlags &= ~FLAG_NEED_TO_SEND_BATTERY_VOLTAGE;
      acc.write(txBuf, batteryVoltageReplyLength);
    }
    if (mainEventFlags & FLAG_NEED_TO_SEND_WHEEL_CURRENT) {
      mainEventFlags &= ~FLAG_NEED_TO_SEND_WHEEL_CURRENT;
      acc.write(txBuf, wheelCurrentReplyLength);
    }
  }
  
  // See if there is a new message from the Wild Thumper.
  if (Serial.available() > 0) {
    wildThumperCom.handleRxByte(Serial.read());
  }
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
