#include <EEPROM.h>

int addr = 0;
int led = 9;
int button = 2;

int temp = 0;

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);

  temp = EEPROM.read(addr);
  
  switch (temp) {
    case 0:
      digitalWrite(led, LOW);
      break;
    case 1:
      digitalWrite(led, HIGH);
      break;
  }
}

void loop() {

  while (1) {
    if (digitalRead(button) == HIGH) {
      digitalWrite(led, HIGH);
      EEPROM.write(addr, 1);
      break;
    }
  }

  while (1) {
    if (digitalRead(button) == HIGH) {
      digitalWrite(led, LOW);
      EEPROM.write(addr, 0);
      break;
    }
  }
}
