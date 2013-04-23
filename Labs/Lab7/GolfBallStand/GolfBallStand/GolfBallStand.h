#ifndef GolfBallStand_h
#define GolfBallStand_h

#include "Arduino.h"

/**
 * IO pins on the Golf Ball Stand
 *	8 pin connector
 *	  blue   - 38 Led Location 1 Under
 *	  orange - 36 Led Location 1 Front
 *	  black  - 34 Led Location 3 Front
 *	  red    - 32 Blue
 *	  green  - 30 Led Location 3 Under
 *	  yellow - 28 Green
 *	  brown  - 26 Led Location 2 Under
 *	  white  - 24 Led Location 2 Front
 *	 
 *	6 pin connector
 *	  blue   - A2 56 Photo Location 1
 *	  yellow - A3 57 Photo External
 *	  green  - A4 58 Photo Location 3
 *	  orange - A5 59 Switch
 *	  black  - A6 60 Red
 *	  white  - A7 61 Photo Location 2
 */
 
#define PIN_LED_1_UNDER 		38
#define PIN_LED_1_FRONT 		36
#define PIN_LED_2_UNDER 		26
#define PIN_LED_2_FRONT 		24
#define PIN_LED_3_UNDER 		30
#define PIN_LED_3_FRONT 		34

// Analog pin numbers
#define PIN_PHOTO_1 			2
#define PIN_PHOTO_2 			7
#define PIN_PHOTO_3 			4
#define PIN_PHOTO_EXTERNAL 		3

#define PIN_RED 				60
#define PIN_GREEN 				28
#define PIN_BLUE 				32

#define PIN_GOLF_BALL_STAND_SWITCH	59

// PNP transistors (colors) are on when the base is LOW.
// NPN transistors (led number) are on when the base is HIGH.
#define COLOR_TRANSISTOR_ON		LOW
#define COLOR_TRANSISTOR_OFF	HIGH
#define LED_TRANSISTOR_ON		HIGH
#define LED_TRANSISTOR_OFF		LOW

// LED colors
#define LED_OFF			0x00
#define LED_BLUE		0x01
#define LED_GREEN		0x02
#define LED_CYAN		0x03
#define LED_RED			0x04
#define LED_PURPLE		0x05
#define LED_YELLOW		0x06
#define LED_WHITE		0x07

// Ball colors.
#define BALL_NONE		-1
#define BALL_BLACK		0
#define BALL_BLUE		1
#define BALL_GREEN 		2
#define BALL_RED 		3
#define BALL_YELLOW 	4
#define BALL_WHITE 		5

// Ball values.
const int ballRedVal[3][4] =	{{624,866,964,564},
								{684,880,975,622},
								{537,943,862,480}};
								
const int ballYellowVal[3][4] = {{430,398,900,276},
								{464,418,919,300},
								{321,692,733,270}};

const int ballGreenVal[3][4] = 	{{791,571,868,484},
								{907,655,954,607},
								{865,864,787,638}};

const int ballBlueVal[3][4] = 	{{875,674,831,572},
								{881,700,882,609},
								{767,865,609,484}};

const int ballWhiteVal[3][4] = 	{{408,327,621,225},
								{462,354,712,259},
								{323,641,418,213}};

const int ballBlackVal[3][4] = 	{{941,882,965,800},
								{901,837,954,741},
								{898,938,809,700}};

// Locations
#define LOCATION_EXTERNAL	0x00
#define LOCATION_1			0x01
#define LOCATION_2			0x02
#define LOCATION_3			0x04

// Front / Back location
#define LED_FRONT			0x01
#define LED_UNDER			0x02
#define LED_UNDER_AND_FRONT	0x03

#define TIME_DELAY			100


class GolfBallStand
{
  public:
    GolfBallStand();
	
    void setLedState(int ledColor, int location, int underOrFront);
    int getAnalogReading(int location);
    int determineBallColor(int location);

    // More public functions coming.
  protected:
    void _init(void);
};

#endif