#include <EEPROM.h>

#define addr 0
#define led 9
#define button 2

volatile int state = 0;


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
  delay(300);
  while (1) {

    analogWrite(led, brightness);
    brightness = brightness + fade_amount;
    if (brightness <= 0 || brightness >= 255) {
      fade_amount = -fade_amount;
    }
    delay(30);
    /*if ( is_button_not_high() )
      analogWrite(led, brightness);
      else {
      break;
      }

      if ( is_button_not_high() )
      brightness = brightness + fade_amount;
      else {
      break;
      }

      if ( is_button_not_high() ) {
      if (brightness <= 0 || brightness >= 255) {
        fade_amount = -fade_amount;
      }
      }
      else {
      break;
      }
      delay(30);*/

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

  if ( state == 0 ) {
    state = 2;
  }

  if ( state < 4 ) {
    state++;
  }
  else {
    state = 1;
  }
}

void Select_menu_item(int next_item) {

  switch (next_item) {
    case 0:
      On();
      break;
    case 1:
      Off();
      break;
    case 2:
      /*if (Pwm() == 1) {
        int next = opManagerSt.next();
        Select_menu_item(next);
        }*/
      On();
      break;
    case 3:
      Off();
      break;
    case 4:
      On();
      break;
      /*case 3:
        if ( Flash() == 1 ) {
          int next = opManagerSt.next();
          Select_menu_item(next);
        }
        break;*/
  }

}


void setup() {

  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
  attachInterrupt(digitalPinToInterrupt(button), Interrupt_f, FALLING);

}

void loop() {
  //Serial.print("here");
  Serial.println(state);
  if ( state > 4 ) {
    state = 1;
  }

  Select_menu_item(state);
}
