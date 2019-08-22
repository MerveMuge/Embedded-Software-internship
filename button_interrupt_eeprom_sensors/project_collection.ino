#include <EEPROM.h>
#include <stdio.h>

#include "Bounce2.h"
#include "DS1302.h"

#define TEMPERATURE_PIN 1

#define LDR_LED_PIN 13
#define LDR_INPUT_PIN A0

#define RELAY_PIN 4

#define BUTTON_PIN 2
#define LED_PIN 9
#define ADDR 0

// Instantiate a Bounce object
Bounce debouncer = Bounce();

volatile int buttonState;
volatile unsigned long buttonPressTimeStamp;

const int debouncePeriod = 50; // <<< you may need more or less here
volatile int count = 0;
volatile int mod = 0;

volatile int ledState;

volatile int status_counter = 0;

volatile int brightness = 0;    // how bright the LED is
volatile int fadeAmount = 5;    // how many points to fade the LED by
volatile int eeprom_read_value;

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

class Temperature {
  public:
    virtual void temperature() {
      int val = analogRead(TEMPERATURE_PIN);
      float mv = ( val / 1024.0) * 5000;
      float cel = mv / 10;
      float farh = (cel * 9) / 5 + 32;

      Serial.print("TEMPRATURE = ");
      Serial.print(cel);
      Serial.print("*C");
      Serial.println();
      delay(1000);
    }

};
Temperature * temperature_obj;

class Photoresistor {
  public:
    virtual void photoresistor_LDR() {
      int ldrStatus = analogRead(LDR_INPUT_PIN);

      if (ldrStatus <= 200) {

        digitalWrite(LDR_LED_PIN, HIGH);
        Serial.print("Its DARK, Turn on the LED : ");
        Serial.println(ldrStatus);

      } else if (ldrStatus > 500) {

        digitalWrite(LDR_LED_PIN, LOW);
        Serial.print("Its BRIGHT, Turn off the LED : ");
        Serial.println(ldrStatus);

      }
    }

};
Photoresistor * ldr_obj;

class Relay {
  public:
    virtual void relay() {
      digitalWrite(RELAY_PIN, HIGH);// turn relay ON
      Serial.println("Relay ON");
      delay(300);// wait for 5 seconds

      digitalWrite(RELAY_PIN, LOW);// turn relay OFF
      Serial.println("Relay OFF");
      delay(1000);// wait for 3 secons

    }

};
Relay * relay_obj;

void setup() {

  Serial.begin(115200);

  // Setup the button
  pinMode(BUTTON_PIN, INPUT);
  // Activate internal pull-up

  pinMode(LDR_LED_PIN, OUTPUT);
  pinMode(LDR_INPUT_PIN, INPUT);

  digitalWrite(BUTTON_PIN, HIGH);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), blink , FALLING);

  // After setting up the button, setup debouncer
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(5);

  //Setup the LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);

  temperature_obj = new Temperature();
  ldr_obj = new Photoresistor();
  relay_obj = new Relay();

  rtc.writeProtect(false);
  rtc.halt(false);

  //define start time //year / month / day /-/hour /min / sec /-/ day
  Time t(2019, 8, 22, 14, 25, 50, Time::kThursday);

  // Set the time on the chip.
  rtc.time(t);

  eeprom_read_value = EEPROM.read(ADDR);

  if ( eeprom_read_value == 0 ) {
    buttonState = 1;
  }
  else if ( eeprom_read_value == 1 ) {
    buttonState = 2;
  }
  else if ( eeprom_read_value == 2 ) {
    buttonState = 3;
  }
  else if ( eeprom_read_value == 3 ) {
    buttonState = 4;
  }

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

    if (status_counter == 0) {
      buttonState = 2;

    }
    else if ( status_counter == 1 ) {
      buttonState = 1;
    }
    else if (status_counter == 2) {
      buttonState = 3;
    }
    else if (status_counter == 3) {
      buttonState = 4;
    }

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
  // Update the debouncer and get the changed state
  temperature_obj->temperature();
  ldr_obj->photoresistor_LDR();
  relay_obj->relay();
  printTime();

  boolean changed = debouncer.update();

  if ( changed ) {
    button_status_changed();
  }
  led_menu();

}
