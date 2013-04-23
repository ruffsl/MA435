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

int GolfBallStand::determineBallColor(int location) {
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
		char redError;
		char greenError;
		char blueError;
		char whiteError;
		
		redError = (redReading-ballRedVal[location][0])^2;
		greenError = (greenReading-ballRedVal[location][1])^2;
		blueError = (blueReading-ballRedVal[location][2])^2;
		whiteError = (whiteReading-ballRedVal[location][3])^2;
		int redBallError = redError + greenError + blueError + whiteError;
		
		redError = (redReading-ballYellowVal[location][0])^2;
		greenError = (greenReading-ballYellowVal[location][1])^2;
		blueError = (blueReading-ballYellowVal[location][2])^2;
		whiteError = (whiteReading-ballYellowVal[location][3])^2;
		int yellowBallError = redError + greenError + blueError + whiteError;
		
		redError = (redReading-ballGreenVal[location][0])^2;
		greenError = (greenReading-ballGreenVal[location][1])^2;
		blueError = (blueReading-ballGreenVal[location][2])^2;
		whiteError = (whiteReading-ballGreenVal[location][3])^2;
		int greenBallError = redError + greenError + blueError + whiteError;
		
		redError = (redReading-ballBlueVal[location][0])^2;
		greenError = (greenReading-ballBlueVal[location][1])^2;
		blueError = (blueReading-ballBlueVal[location][2])^2;
		whiteError = (whiteReading-ballBlueVal[location][3])^2;
		int blueBallError = redError + greenError + blueError + whiteError;
		
		redError = (redReading-ballWhiteVal[location][0])^2;
		greenError = (greenReading-ballWhiteVal[location][1])^2;
		blueError = (blueReading-ballWhiteVal[location][2])^2;
		whiteError = (whiteReading-ballWhiteVal[location][3])^2;
		int whiteBallError = redError + greenError + blueError + whiteError;
		
		TODO;
		//TODO Chose the one with the smallest error to find the color
		
		// if(){
			// returnBallType = BALL_RED;
		// }
		// if(redPass&&greenPass&&bluePass&&whitePass){
			// returnBallType = BALL_YELLOW;
		// }
		// if(redPass&&greenPass&&bluePass&&whitePass){
			// returnBallType = BALL_GREEN;
		// }
		// if(redPass&&greenPass&&bluePass&&whitePass){
			// returnBallType = BALL_BLUE;
		// }
		// if(redPass&&greenPass&&bluePass&&whitePass){
			// returnBallType = BALL_WHITE;
		// }
		// if(redPass&&greenPass&&bluePass&&whitePass){
			// returnBallType = BALL_BLACK;
		// }
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
