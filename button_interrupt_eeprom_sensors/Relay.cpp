#include "Arduino.h"
#include "Relay.h"

Relay::Relay(int pin) {
  
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void Relay::relay_process(int delay_amount) {
  
  digitalWrite(_pin, HIGH);// turn relay ON
  Serial.println("Relay ON");
  delay(delay_amount);// wait for 5 seconds


  digitalWrite(_pin, LOW);// turn relay OFF
  Serial.println("Relay OFF");
  delay(delay_amount);// wait for 3 secons
}
