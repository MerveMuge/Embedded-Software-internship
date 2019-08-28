#include "Arduino.h"
#include "Temperature.h"

Temperature::Temperature(int temperature_pin){
  _pin = temperature_pin;
}

void Temperature::evaluateTemperature() {
  int val = analogRead(_pin);
  float mv = ( val / 1024.0) * 5000;
  float cel = mv / 10;
  float farh = (cel * 9) / 5 + 32;

  Serial.print("TEMPERATURE = ");
  Serial.print(cel);
  Serial.print("*C");
  Serial.println();
}
