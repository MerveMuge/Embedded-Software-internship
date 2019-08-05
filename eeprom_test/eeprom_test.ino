#include <EEPROM.h>

int addr = 0;
int led = 9;
int button = 2;

void On() {
  digitalWrite(led, HIGH);
  EEPROM.write(addr, 0);
}

void Off() {
  digitalWrite(led, LOW);
  EEPROM.write(addr, 1);
}

bool is_button_not_high() {

  if ( !(digitalRead(button) == HIGH) ) {
    return true;
  } else {
    return false;
  }
}

int Pwm() {

  int brightness = 0;    // how bright the LED is
  int fadeAmount = 5;    // how many points to fade the LED by
  EEPROM.write(addr, 2);
  delay(300);
  while (1) {

    if ( is_button_not_high() )
      analogWrite(led, brightness);
    else
      break;

    if ( is_button_not_high() )
      brightness = brightness + fadeAmount;
    else
      break;

    if ( is_button_not_high() ) {
      if (brightness <= 0 || brightness >= 255) {
        fadeAmount = -fadeAmount;
      }
    }
    else {
      break;
    }

    delay(30);

  }
}

int Flash() {
  delay(300);
  EEPROM.write(addr, 3);
  while (1) {

    if ( is_button_not_high() )
      digitalWrite(led, HIGH);
    else
      break;

    delay(100);

    if ( is_button_not_high() )
      digitalWrite(led, LOW);
    else
      break;
    delay(100);
  }
}

enum opMode {
  ON,
  OFF,
  PWM,
  FLASH
};

struct opManager {
  private:
    opMode m_opMode;

  public:
    /*opManager(opMode mode) {
      m_opMode = mode;
      }*/
    opMode next() {
      if ( m_opMode == FLASH )
        m_opMode = ON;
      else {
        int temp_opMode = m_opMode;
        temp_opMode++;
        m_opMode = (opMode) temp_opMode;
      }

      return m_opMode;
    }

    opMode currentMode() {
      return m_opMode;
    }

} opManagerSt;

int select_menu_item(int next_item) {

  switch (next_item) {
    case 0:
      Serial.println("HIGH");
      On(); 
      return 1;
    case 1:
      Serial.println("LOW");
      Off();
      return 1;
    case 2:
      Serial.println("PWM");
      Pwm();
      return 1;
    case 3:
      Serial.println("FLASH");
      Flash();
      return 1;

  }

}

void setup() {

  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);

  //opMode m_modem;
  //opManager(m_modem);

}

void loop() {

  while (1) {
    if (digitalRead(button) == HIGH) {

      int next_item = opManagerSt.next();

      if ( select_menu_item(next_item) == 1 ) {
        break;
      }

    }
  }


  while (1) {
    if ( digitalRead(button) == LOW ) {
      break;
    }
  }

}
