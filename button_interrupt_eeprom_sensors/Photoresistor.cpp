#include "Arduino.h"
#include "Photoresistor.h"

Photoresistor::Photoresistor(int pin, int ldr_pin) {
  
  pinMode(pin, OUTPUT);
  pinMode(ldr_pin, INPUT);
  _pin = pin;
  _ldrPin = ldr_pin;
}


void Photoresistor::ldr() {

  int ldrStatus = analogRead(_ldrPin);

  if (ldrStatus <= 200) {

    digitalWrite(_pin, HIGH);
    Serial.print("Its DARK, Turn on the LED : ");
    Serial.println(ldrStatus);
  } else if (ldrStatus > 500) {

    digitalWrite(_pin, LOW);
    Serial.print("Its BRIGHT, Turn off the LED : ");
    Serial.println(ldrStatus);
  }

}
