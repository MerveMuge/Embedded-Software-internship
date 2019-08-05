#include <EEPROM.h>

int addr = 0;
int led = 9;
int button = 2;

void On() {
  digitalWrite(led, HIGH);
  //EEPROM.write(addr, 1);
}

void Off() {
  digitalWrite(led, LOW);
  //EEPROM.write(addr, 0);
}

bool is_button_not_high() {

  if (!(digitalRead(button) == HIGH) ) {
    return true;
  } else {
    return false;
  }
}

int pwm() {

  int brightness = 0;    // how bright the LED is
  int fadeAmount = 5;    // how many points to fade the LED by
  //EEPROM.write(addr, 2);
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

int flash() {
  delay(300);
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
      if ( m_opMode == ON ) {
        m_opMode = OFF;
        delay(1000);
      }
      else if ( m_opMode == OFF ) {
        m_opMode = PWM;
        delay(1000);
      }
      else if ( m_opMode == PWM ) {
        m_opMode = FLASH;
        delay(1000);
      }
      else if ( m_opMode == FLASH ) {
        m_opMode = ON;
        delay(1000);
      }

      return m_opMode;
    }

    opMode currentMode() {
      return m_opMode;
    }

} opManagerSt;

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

      if (next_item == 0 ) {
        Serial.println("HIGH");
        digitalWrite(led, HIGH);
        break;
      }
      else if ( next_item == 1 ) {
        Serial.println("LOW");
        digitalWrite(led, LOW);
        break;
      }
      else if (next_item == 2 ) {
        Serial.println("PWM");
        pwm();
        break;
      }
      else if ( next_item == 3 ) {
        Serial.println("FLASH");
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
