#define TEMPERATURE_PIN A1

#include "Temperature.h"

Temperature temperature(TEMPERATURE_PIN);

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  temperature.evaluateTemperature();
}
