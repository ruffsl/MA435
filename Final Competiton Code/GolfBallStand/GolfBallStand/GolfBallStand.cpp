#include "Arduino.h"
#include "GolfBallStand.h"

GolfBallStand::GolfBallStand() {
	_init();  
}
void GolfBallStand::_init() {
	pinMode(PIN_LED_1_UNDER, OUTPUT);
	pinMode(PIN_LED_1_FRONT, OUTPUT);
	pinMode(PIN_LED_2_UNDER, OUTPUT);
	pinMode(PIN_LED_2_FRONT, OUTPUT);
	pinMode(PIN_LED_3_UNDER, OUTPUT);
	pinMode(PIN_LED_3_FRONT, OUTPUT);
	pinMode(PIN_RED, OUTPUT);
	pinMode(PIN_GREEN, OUTPUT);
	pinMode(PIN_BLUE, OUTPUT);
	pinMode(PIN_GOLF_BALL_STAND_SWITCH, INPUT_PULLUP);
	digitalWrite(PIN_LED_1_UNDER, LED_TRANSISTOR_OFF);
	digitalWrite(PIN_LED_1_FRONT, LED_TRANSISTOR_OFF);
	digitalWrite(PIN_LED_2_UNDER, LED_TRANSISTOR_OFF);
	digitalWrite(PIN_LED_2_FRONT, LED_TRANSISTOR_OFF);
	digitalWrite(PIN_LED_3_UNDER, LED_TRANSISTOR_OFF);
	digitalWrite(PIN_LED_3_FRONT, LED_TRANSISTOR_OFF);
	digitalWrite(PIN_RED, COLOR_TRANSISTOR_OFF);
	digitalWrite(PIN_GREEN, COLOR_TRANSISTOR_OFF);
	digitalWrite(PIN_BLUE, COLOR_TRANSISTOR_OFF);
}

void GolfBallStand::setLedState(int ledColor, int location,
		int underOrFront) {
	// Start by clearing off all LEDs and colors.
	digitalWrite(PIN_RED, COLOR_TRANSISTOR_OFF);
	digitalWrite(PIN_GREEN, COLOR_TRANSISTOR_OFF);
	digitalWrite(PIN_BLUE, COLOR_TRANSISTOR_OFF);
	digitalWrite(PIN_LED_1_UNDER, LED_TRANSISTOR_OFF);
	digitalWrite(PIN_LED_2_UNDER, LED_TRANSISTOR_OFF);
	digitalWrite(PIN_LED_3_UNDER, LED_TRANSISTOR_OFF);
	digitalWrite(PIN_LED_1_FRONT, LED_TRANSISTOR_OFF);
	digitalWrite(PIN_LED_2_FRONT, LED_TRANSISTOR_OFF);
	digitalWrite(PIN_LED_3_FRONT, LED_TRANSISTOR_OFF);

	// Decide which of the six LEDs to turn on.
	if ((location & LOCATION_1) && (underOrFront & LED_UNDER)) {
		digitalWrite(PIN_LED_1_UNDER, LED_TRANSISTOR_ON);
	}
	if ((location & LOCATION_1) && (underOrFront & LED_FRONT)) {
		digitalWrite(PIN_LED_1_FRONT, LED_TRANSISTOR_ON);
	}
	if ((location & LOCATION_2) && (underOrFront & LED_UNDER)) {
		digitalWrite(PIN_LED_2_UNDER, LED_TRANSISTOR_ON);
	}
	if ((location & LOCATION_2) && (underOrFront & LED_FRONT)) {
		digitalWrite(PIN_LED_2_FRONT, LED_TRANSISTOR_ON);
	}
	if ((location & LOCATION_3) && (underOrFront & LED_UNDER)) {
		digitalWrite(PIN_LED_3_UNDER, LED_TRANSISTOR_ON);
	}
	if ((location & LOCATION_3) && (underOrFront & LED_FRONT)) {
		digitalWrite(PIN_LED_3_FRONT, LED_TRANSISTOR_ON);
	}

	// Set the color.
	if (ledColor & LED_BLUE) {
		digitalWrite(PIN_BLUE, COLOR_TRANSISTOR_ON);
	}
	if (ledColor & LED_GREEN) {
		digitalWrite(PIN_GREEN, COLOR_TRANSISTOR_ON);
	}
	if (ledColor & LED_RED) {
		digitalWrite(PIN_RED, COLOR_TRANSISTOR_ON);
	}
}

int GolfBallStand::getAnalogReading(int location) {
	int photoReading = -1;
	switch (location) {
	case LOCATION_1:
		photoReading = analogRead(PIN_PHOTO_1);
		break;
	case LOCATION_2:
		photoReading = analogRead(PIN_PHOTO_2);
		break;
	case LOCATION_3:
		photoReading = analogRead(PIN_PHOTO_3);
		break;
	case LOCATION_EXTERNAL:
		photoReading = analogRead(PIN_PHOTO_EXTERNAL);
		break;
	}
	return photoReading;
}

int GolfBallStand::determineBallColor(int location, double errors[6]) {
	int returnBallType = BALL_NONE;
	
	setLedState(LED_OFF, location, LED_UNDER_AND_FRONT);
	delay(TIME_DELAY);
	int offReading = getAnalogReading(location);

	setLedState(LED_RED, location, LED_UNDER_AND_FRONT);
	delay(TIME_DELAY);
	int redReading = getAnalogReading(location);
	
	setLedState(LED_GREEN, location, LED_UNDER_AND_FRONT);
	delay(TIME_DELAY);
	int greenReading = getAnalogReading(location);

	setLedState(LED_BLUE, location, LED_UNDER_AND_FRONT);
	delay(TIME_DELAY);
	int blueReading = getAnalogReading(location);

	setLedState(LED_WHITE, location, LED_UNDER_AND_FRONT);
	delay(TIME_DELAY);
	int whiteReading = getAnalogReading(location);
	
	setLedState(LED_OFF, location, LED_UNDER_AND_FRONT);
		
	location = (location == LOCATION_3 ? 3 : location)-1;

	if (offReading > 950) {	
		double redError;
		double greenError;
		double blueError;
		double whiteError;
		
		double colorErrors[6];
		
		redError = pow(redReading-ballRedVal[location][0], 2);
		greenError = pow(greenReading-ballRedVal[location][1],2);
		blueError = pow(blueReading-ballRedVal[location][2],2);
		whiteError = pow(whiteReading-ballRedVal[location][3],2);
		colorErrors[0] = redError + greenError + blueError + whiteError;
		
		redError = pow(redReading-ballYellowVal[location][0],2);
		greenError = pow(greenReading-ballYellowVal[location][1],2);
		blueError = pow(blueReading-ballYellowVal[location][2],2);
		whiteError = pow(whiteReading-ballYellowVal[location][3],2);
		colorErrors[1] = redError + greenError + blueError + whiteError;
		
		redError = pow(redReading-ballGreenVal[location][0],2);
		greenError = pow(greenReading-ballGreenVal[location][1],2);
		blueError = pow(blueReading-ballGreenVal[location][2],2);
		whiteError = pow(whiteReading-ballGreenVal[location][3],2);
		colorErrors[2] = redError + greenError + blueError + whiteError;
		
		redError = pow(redReading-ballBlueVal[location][0],2);
		greenError = pow(greenReading-ballBlueVal[location][1],2);
		blueError = pow(blueReading-ballBlueVal[location][2],2);
		whiteError = pow(whiteReading-ballBlueVal[location][3],2);
		colorErrors[3] = redError + greenError + blueError + whiteError;
		
		redError = pow(redReading-ballWhiteVal[location][0],2);
		greenError = pow(greenReading-ballWhiteVal[location][1],2);
		blueError = pow(blueReading-ballWhiteVal[location][2],2);
		whiteError = pow(whiteReading-ballWhiteVal[location][3],2);
		colorErrors[4] = redError + greenError + blueError + whiteError;
		
		redError = pow(redReading-ballBlackVal[location][0],2);
		greenError = pow(greenReading-ballBlackVal[location][1],2);
		blueError = pow(blueReading-ballBlackVal[location][2],2);
		whiteError = pow(whiteReading-ballBlackVal[location][3],2);
		colorErrors[5] = redError + greenError + blueError + whiteError;
		
		int i, j=0;
		Serial.println();
		Serial.print("Errors for colors\n");
		Serial.print("  LED red error     = ");
		Serial.println(colorErrors[0]);
		Serial.print("  LED yellow error  = ");
		Serial.println(colorErrors[1]);
		Serial.print("  LED green error   = ");
		Serial.println(colorErrors[2]);
		Serial.print("  LED blue error    = ");
		Serial.println(colorErrors[3]);
		Serial.print("  LED white error   = ");
		Serial.println(colorErrors[4]);
		Serial.print("  LED black error   = ");
		Serial.println(colorErrors[5]);
		errors[0] = colorErrors[0];
		errors[1] = colorErrors[1];
		errors[2] = colorErrors[2];
		errors[3] = colorErrors[3];
		errors[4] = colorErrors[4];
		errors[5] = colorErrors[5];
		for (i = 1; i < 6; i++) {
			if (colorErrors[j] > min(colorErrors[j], colorErrors[i])) {
				j=i;
			}
		}
		int minError = colorErrors[j];
		switch (j) {
			case 0:
				returnBallType = BALL_RED;
				break;
			case 1:
				returnBallType = BALL_YELLOW;
				break;
			case 2:
				returnBallType = BALL_GREEN;
				break;
			case 3:
				returnBallType = BALL_BLUE;
				break;
			case 4:
				returnBallType = BALL_WHITE;
				break;
			case 5:
				returnBallType = BALL_BLACK;
				break;
			default:
				returnBallType = BALL_NONE;
		}
	}
	
	Serial.println();
	Serial.print("Readings for location ");
	Serial.println(location+1);
	Serial.print("  LED off reading   = ");
	Serial.println(offReading);
	Serial.print("  LED red reading   = ");
	Serial.println(redReading);
	Serial.print("  LED green reading = ");
	Serial.println(greenReading);
	Serial.print("  LED blue reading  = ");
	Serial.println(blueReading);
	Serial.print("  LED white reading = ");
	Serial.println(whiteReading);
	Serial.print("  Ball Color = ");
	switch (returnBallType) {
		case BALL_RED:
			Serial.println("BALL_RED");
			break;
		case BALL_YELLOW:
			Serial.println("BALL_YELLOW");
			break;
		case BALL_GREEN:
			Serial.println("BALL_GREEN");
			break;
		case BALL_BLUE:
			Serial.println("BALL_BLUE");
			break;
		case BALL_WHITE:
			Serial.println("BALL_WHITE");
			break;
		case BALL_BLACK:
			Serial.println("BALL_BLACK");
			break;
		default:
			Serial.println("BALL_NONE");
			break;
	}
	
	return returnBallType;
}
