#ifndef Photoresistor_h
#define Photoresistor_h

#include "Arduino.h"

class Photoresistor {
  public:
    Photoresistor(int pin, int ldrPin);
    void ldr();
  private:
    int _pin;
    int _ldrPin;
};

#endif
