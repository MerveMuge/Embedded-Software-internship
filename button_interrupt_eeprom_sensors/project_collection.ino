#include <EEPROM.h>
#include <stdio.h>

#include "Bounce2.h"
#include "DS1302.h"
#include "Photoresistor.h"
#include "Temperature.h"

#define TEMPERATURE_PIN A1
#define RELAY_PIN 4

#define LDR_LED_PIN 13
#define LDR_INPUT_PIN A0

#define BUTTON_PIN 2
#define LED_PIN 9
#define ADDR 0

// Instantiate a Bounce object
Bounce debouncer = Bounce();
Temperature temperature(TEMPERATURE_PIN);
Photoresistor photoresistor(LDR_LED_PIN, LDR_INPUT_PIN);

volatile int buttonState;
volatile unsigned long buttonPressTimeStamp;

const int debouncePeriod = 50; // << may need more or less here
volatile int count = 0;
volatile int mod = 0;

volatile int ledState;
volatile int status_counter = 0;

volatile int brightness = 0;    // how bright the LED is
volatile int fadeAmount = 5;    // how many points to fade the LED by

namespace {

const int kCePin   = 5;  // Chip Enable
const int kIoPin   = 6;  // Input/Output
const int kSclkPin = 7;  // Serial Clock

//DS1302 obj
DS1302 rtc(kCePin, kIoPin, kSclkPin);

String dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday: return "Sunday";
    case Time::kMonday: return "Monday";
    case Time::kTuesday: return "Tuesday";
    case Time::kWednesday: return "Wednesday";
    case Time::kThursday: return "Thursday";
    case Time::kFriday: return "Friday";
    case Time::kSaturday: return "Saturday";
  }
  return "(unknown day)";
}

void printTime() {
  // Get the current time and date from the chip.
  Time t = rtc.time();

  // Name the day of the week.
  const String day = dayAsString(t.day);

  char buf[50];
  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
           day.c_str(),
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);

  Serial.println(buf);
}

}

class Relay {
  public:
    virtual void relay() {
      Serial.println("Relay..");
      digitalWrite(RELAY_PIN, HIGH);// turn relay ON
      //Serial.println("Relay ON");
      delay(300);// wait for 5 seconds

      digitalWrite(RELAY_PIN, LOW);// turn relay OFF
      //Serial.println("Relay OFF");
    }

};
Relay * relay_obj;

void setup() {

  Serial.begin(115200);

  // Setup the button
  pinMode(BUTTON_PIN, INPUT);
  //pinMode(LDR_INPUT_PIN, INPUT);
  // Activate internal pull-up

  // pinMode(LDR_LED_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(BUTTON_PIN, HIGH);
  digitalWrite(LED_PIN, ledState);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), blink , FALLING);

  // After setting up the button, setup debouncer
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(5);

  rtc.writeProtect(false);
  rtc.halt(false);

  //define start time //year / month / day /-/hour /min / sec /-/ day
  Time t(2019, 8, 27, 17, 06, 50, Time::kTuesday);

  // Set the time on the chip.
  rtc.time(t);

  int eeprom_read_value = EEPROM.read(ADDR);
  Serial.print("eeprom read value ");
  Serial.println( eeprom_read_value);

  /*if ( eeprom_read_value == 0 ) {
    mod = 1;
    count = 1;
    Serial.println("mod 1");
    }
    else if ( eeprom_read_value == 1 ) {
    mod = 2;
    count = 2;
    Serial.println("mod 2");
    }
    else if ( eeprom_read_value == 2 ) {
    mod = 3;
    count = 3;
    Serial.println("mod 3");
    }
    else if ( eeprom_read_value == 3 ) {
    mod = 0;
    count = 0;
    Serial.println("mod 0");
    }*/

}

void blink() {
  static volatile int lastInterruptMillis = 0;
  if (millis() - lastInterruptMillis > debouncePeriod)
  {
    count++;
    mod = count % 4;
    lastInterruptMillis = millis();
  }
}

void flash() {
  EEPROM.write(ADDR, 0);
  if ( millis() - buttonPressTimeStamp >= 500 ) {
    buttonPressTimeStamp = millis();
    if ( ledState == HIGH ) ledState = LOW;
    else if ( ledState == LOW ) ledState = HIGH;
    digitalWrite(LED_PIN, ledState );
    Serial.println("Retriggering button");
  }
}

void high() {
  EEPROM.write(ADDR, 1);
  digitalWrite(LED_PIN, HIGH);
  Serial.println("Retriggered high");
}

void low() {
  EEPROM.write(ADDR, 2);
  digitalWrite(LED_PIN, LOW);
  Serial.println("Retriggered low");
}

void pwm() {
  EEPROM.write(ADDR, 3);
  // set the brightness of pin 9:
  analogWrite(LED_PIN, brightness);

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  // wait for 30 milliseconds to see the dimming effect
  delay(30);
  Serial.println("pwm");
}

void button_status_changed() {
  // Get the update value
  int value = debouncer.read();
  if ( value == HIGH) {
    ledState = LOW;
    digitalWrite(LED_PIN, ledState );

    //buttonState = 0;
    Serial.println("Button released (state 0)");

  } else {
    ledState = HIGH;
    digitalWrite(LED_PIN, ledState );

    Serial.println("Button pressed (state 1)");
    buttonPressTimeStamp = millis();
    status_counter++;
    if (status_counter == 4) {
      status_counter = 0;
    }
  }
}

void led_menu() {
  if  ( mod == 1 ) {
    flash();
  }
  else if (mod == 2) {
    high();
  }
  else if (mod == 3) {
    low();
  }
  else if (mod == 0) {
    pwm();
  }
}


void loop() {

  temperature.evaluateTemperature();
  photoresistor.ldr();

  printTime();

  // Update the debouncer and get the changed state
  boolean changed = debouncer.update();

  if ( changed ) {
    button_status_changed();
  }
  led_menu();

}
