#include <EEPROM.h>

int addr = 0;
int led = 9;
int button = 2;

int temp = 0;
int button_high = HIGH;
int button_low = LOW;

int counter = 0;

void check_in_setup(int temp) {

  switch (temp) {
    case 0:
      counter = 2;
      digitalWrite(led, LOW);
      break;
    case 1:
      counter = 1;
      digitalWrite(led, HIGH);
      break;
    case 2:
      counter = 3;
      pwm();
      break;
    case 3:
      counter = 0;
      flash();
      break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);

  temp = EEPROM.read(addr);
  check_in_setup(temp);
}

void on() {
  digitalWrite(led, HIGH);
  EEPROM.write(addr, 1);
}

void off() {
  digitalWrite(led, LOW);
  EEPROM.write(addr, 0);
}

int pwm() {

  int brightness = 0;    // how bright the LED is
  int fadeAmount = 5;    // how many points to fade the LED by
  EEPROM.write(addr, 2);
  delay(1000);
  while (1) {

    if (!(digitalRead(button) == HIGH) ) {
      analogWrite(led, brightness);
    }
    else {
      break;
    }

    if (!(digitalRead(button) == HIGH) ) {
      brightness = brightness + fadeAmount;
    }
    else {
      break;
    }

    if (!(digitalRead(button) == HIGH) ) {
      if (brightness <= 0 || brightness >= 255) {
        fadeAmount = -fadeAmount;
      }
    }
    else {
      break;
    }

    delay(30);

  }
  counter = 3;
  return counter;
}

int flash() {

  EEPROM.write(addr, 3);
  Serial.println("here flash");
  delay(1000);
  while (1) {
    Serial.println("here flash while");

    if (!(digitalRead(button) == HIGH) ) {
      Serial.println("high");
      digitalWrite(led, HIGH);
    } else {
      break;
    }

    //if (!(digitalRead(button) == HIGH) ) {
      Serial.println("delay");
      delay(100);
    //} else {
    //  break;
    //}

    if (!(digitalRead(button) == HIGH) ) {
      Serial.println("low");
      digitalWrite(led, LOW);
    } else {
      break;
    }

    //if (!(digitalRead(button) == HIGH) ) {
      Serial.println("delay");
      delay(100);
    //} else {
    //  break;
    //}

  }
  counter = 4;
  return counter;

}

void loop() {

  while (1) {
    if (digitalRead(button) == HIGH) {

      if (counter == 4) {
        counter = 0;
      }
      Serial.println(counter);
      if (counter == 0 ) {
        //delay(1000);
        counter++;
        on();
        break;
      }
      else if (counter == 1) {
        //delay(1000);
        counter++;
        off();
        break;
      }
      else if (counter == 2) {
        counter++;
        counter = pwm();
        if (counter != 3) {
          //flash();
          break;
        }
        //break;
      }
      else if (counter == 3) {
        counter++;
        counter = flash();
        if (counter != 4) {
          break;
        }
        
      }
    }
  }

  while (1) {
    if (digitalRead(button) == LOW) {
      break;
    }
  }
}
