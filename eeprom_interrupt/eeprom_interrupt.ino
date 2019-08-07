#include <EEPROM.h>

#define addr 0
#define led 9
#define button 2

volatile int state = 0;

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

void interrupt_f() {

  if( state == 0 ){
    state =2;
  }
  
  if ( state < 4 ) {
    state++;
  }
  else {
    state = 1;
  }
}

void setup() {

  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
  attachInterrupt(digitalPinToInterrupt(button), interrupt_f, FALLING);

}

void loop() {
  //Serial.print("here");
  Serial.println(state);
  if ( ( state == 1 ) || ( state == 0 ) ) {
    digitalWrite(led, HIGH);
  }
  else if (state == 2) {
    digitalWrite(led, LOW);
  }
  else if (state == 3) {
    digitalWrite(led, HIGH);
  }
  else if (state == 4) {
    digitalWrite(led, LOW);
  }
}
