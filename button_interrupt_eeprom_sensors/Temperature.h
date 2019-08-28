#ifndef Temperature_h
#define Temperature_h

#include "Arduino.h"

class Temperature{
  public:
    Temperature(int pin);
    void evaluateTemperature();
  private:
    int _pin;
  
};

#endif
