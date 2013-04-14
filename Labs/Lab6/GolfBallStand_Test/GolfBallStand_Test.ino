#include <GolfBallStand.h>

GolfBallStand stand;
int ballColor_1;
int ballColor_2;
int ballColor_3;

void setup()  {
  Serial.begin(115200);
  stand.setLedState(LED_GREEN, LOCATION_1 | LOCATION_2 | LOCATION_3, LED_UNDER_AND_FRONT);
}

void loop(){
  delay(1000);
  stand.setLedState(LED_RED, LOCATION_1 | LOCATION_2 | LOCATION_3, LED_UNDER_AND_FRONT);
  delay(1000);
  stand.setLedState(LED_WHITE, LOCATION_1 | LOCATION_2 | LOCATION_3, LED_UNDER_AND_FRONT);
  delay(1000);
  stand.setLedState(LED_BLUE, LOCATION_1 | LOCATION_2 | LOCATION_3, LED_UNDER_AND_FRONT);
  delay(1000);
  stand.setLedState(LED_GREEN, LOCATION_1 | LOCATION_2 | LOCATION_3, LED_UNDER_AND_FRONT);
  delay(1000);
  stand.setLedState(LED_OFF, LOCATION_1 | LOCATION_2 | LOCATION_3, LED_UNDER_AND_FRONT);
}

void printBallColor(int color) {
  
}
