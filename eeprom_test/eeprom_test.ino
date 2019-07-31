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
      digitalWrite(led, LOW);
      break;
    case 1:
      counter = 1;
      digitalWrite(led, HIGH);
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

void loop() {

  while (1) {
    if (digitalRead(button) == HIGH) {
      Serial.println(counter);
      if (counter == 2) {
        counter = 0;
      }

      if (counter == 0 ) {
        delay(1000);
        counter++;
        on();
        break;
      }
      else if (counter == 1) {
        delay(1000);
        counter++;
        off();
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
