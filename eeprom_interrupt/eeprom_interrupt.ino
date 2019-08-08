#include "Bounce2.h"
#include <EEPROM.h>

#define BUTTON_PIN 2
#define LED_PIN 9

// Instantiate a Bounce object
Bounce debouncer = Bounce();

volatile int buttonState;
volatile unsigned long buttonPressTimeStamp;

volatile int ledState;

volatile int status_counter = 0;

volatile int brightness = 0;    // how bright the LED is
volatile int fadeAmount = 5;    // how many points to fade the LED by

void setup() {

  Serial.begin(115200);

  // Setup the button
  pinMode(BUTTON_PIN, INPUT);
  // Activate internal pull-up
  digitalWrite(BUTTON_PIN, HIGH);

  // After setting up the button, setup debouncer
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(5);

  //Setup the LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);

}

void Flash() {
  if ( millis() - buttonPressTimeStamp >= 500 ) {
    buttonPressTimeStamp = millis();
    if ( ledState == HIGH ) ledState = LOW;
    else if ( ledState == LOW ) ledState = HIGH;
    digitalWrite(LED_PIN, ledState );
    Serial.println("Retriggering button");
  }
}

void High() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println("Retriggered high");
}

void Low() {
  digitalWrite(LED_PIN, LOW);
  Serial.println("Retriggered low");
}

void Pwm() {
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
}

void loop() {
  // Update the debouncer and get the changed state
  boolean changed = debouncer.update();

  if ( changed ) {
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

      //buttonState = 1;
      Serial.println("Button pressed (state 1)");
      buttonPressTimeStamp = millis();
      status_counter++;
      if (status_counter == 4) {
        status_counter = 0;
      }

    }
  }
  if  ( buttonState == 1 ) {
    Flash();
  }
  else if (buttonState == 2) {
    High();
  }
  else if (buttonState == 3) {
    Low();
  }
  else if (buttonState == 4) {
    Pwm();
  }


}
