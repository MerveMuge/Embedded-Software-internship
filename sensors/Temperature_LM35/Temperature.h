#ifndef Photoresistor_h
#define Photoresistor_h

#include "Arduino.h"

class Temperature{
  public:
    Temperature(int pin);
    void evaluateTemperature();
  private:
    int _pin;
  
};

#endif
