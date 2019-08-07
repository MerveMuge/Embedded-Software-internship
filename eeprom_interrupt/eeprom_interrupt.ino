#include <EEPROM.h>

#define addr 0
#define led 9
#define button 2

volatile int next_state = 0;

bool is_button_not_high() {

  if ( !(digitalRead(button) == HIGH) ) {
    return true;
  } else {
    return false;
  }
}

void On() {
  Serial.println("HIGH");
  digitalWrite(led, HIGH);
  EEPROM.write(addr, 0);
}

void Off() {
  Serial.println("LOW");
  digitalWrite(led, LOW);
  EEPROM.write(addr, 1);
}

int Pwm() {
  Serial.println("PWM");
  int brightness = 0;    // how bright the LED is
  int fade_amount = 5;    // how many points to fade the LED by
  EEPROM.write(addr, 2);
  bool pwm_temp = true;
  while ( pwm_temp != false ) {

    if ( is_button_not_high() )
      analogWrite(led, brightness);
    else {
      pwm_temp = false;
    }

    if ( is_button_not_high() )
      brightness = brightness + fade_amount;
    else {
      pwm_temp = false;
    }

    if ( is_button_not_high() ) {
      if (brightness <= 0 || brightness >= 255) {
        fade_amount = -fade_amount;
      }
    }
    else {
      pwm_temp = false;
    }
    delay(30);
  }

  return 1;
}

int Flash() {
  Serial.println("FLASH");
  EEPROM.write(addr, 3);
  bool flash_temp = true;
  while (flash_temp != false) {

    if ( is_button_not_high() ) {
      digitalWrite(led, HIGH);
    }
    else {
      flash_temp = false;
    }
    delay(100);

    if ( is_button_not_high() ) {
      digitalWrite(led, LOW);
    }
    else {
      flash_temp = false;;
    }
    delay(100);
  }

  return 1;
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

void Interrupt_f() {
  next_state = opManagerSt.next();
}

void Select_menu_item(int next_state) {

  switch (next_state) {
    case ON:
      On();
      break;
    case OFF:
      Off();
      break;
    case PWM:
      Pwm();
      break;
    case FLASH:
      Flash();
      break;
  }

}


void setup() {

  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
  attachInterrupt(digitalPinToInterrupt(button), Interrupt_f, FALLING);
}

void loop() {

  //Serial.println(state);

  Select_menu_item(next_state);
}
