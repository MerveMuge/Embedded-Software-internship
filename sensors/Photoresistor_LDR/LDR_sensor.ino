#define LED_PIN 13
#define LDR_PIN A0

#include "Photoresistor.h"

Photoresistor photoresistor(LED_PIN, LDR_PIN);

void setup() {

  Serial.begin(115200);
}


void loop() {

  photoresistor.ldr();
}
