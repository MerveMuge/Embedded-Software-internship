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

void pwm() {
  analogWrite(led, 50);
  EEPROM.write(addr, 2);
}

void flash() {
  EEPROM.write(addr, 3);
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  delay(500);
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
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
        pwm();
        break;
      }
      else if (counter == 3) {
        counter++;
        flash();
        break;
      }
    }
  }

  while (1) {
    if (digitalRead(button) == LOW) {
      break;
    }
  }
}
