int led = 11;
int led_2 = 10;
int led_3 = 9;

int incomingValue = 0;
int index_counter = 1;

char c_temp;
int unit_brightness = 255 / 10;
char data[9];

void write(int multi) {
  analogWrite(led, unit_brightness * multi);
}

void setup() {
  Serial.begin(115200);
  int incomingValue = 0;
}

void pwm(char data[]) {
  if (data[6] == '1') {
    write(1);
  } else if (data[6] == '2') {
    write(2);
  } else if (data[6] == '3') {
    write(3);
  } else if (data[6] == '4') {
    write(4);
  } else if (data[6] == '5') {
    write(5);
  } else if (data[6] == '6') {
    write(6);
  } else if (data[6] == '7') {
    write(7);
  } else if (data[6] == '8') {
    write(8);
  } else if (data[6] == '9') {
    write(9);
  } else if ((data[6] == '0') && (data[5] == '1')) {
    write(10);
  } else if ((data[6] == '0') && (data[5] == '0')) {
    write(0);
  }
}

void flash(char data[]) {

  char flash_temp;
  digitalWrite(led, LOW);

  int fourth_index = data[5] - '0';
  int fifth_index = data[6] - '0';

  int period = (fourth_index * 10) + fifth_index;

  while (period > 0) {

    unsigned long time_now = millis();
    Serial.println("period check while ");
    Serial.print(period);

    while ( (millis() < time_now + (period * 1000)) && ( period > 0) ) {

      if (Serial.available()) {

        flash_temp = Serial.read();

        if (flash_temp == '1') {
          Serial.println("pwm from flash ! ");
          period = 0;
          pwm(data);
          period = 0;
          break;

        }

        else if (flash_temp = '3') {
          Serial.println("run tg from flash");
          run_tg(data);
          break;
        }
      }

    }
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);

  }
}

void run_tg(char data[]) {

  char run_tg_temp;
  write(0);
  int fifth_index = data[5] - '0';
  int sixth_index = data[6] - '0';
  int pwm_value = (fifth_index * 10) + sixth_index;
  Serial.print("pwm run tg: ");
  Serial.println(pwm_value);

  int seventh_index = data[7] - '0';
  int eighth_index = data[8] - '0';
  int period = (seventh_index * 10) + eighth_index;
  Serial.print("run tg period: ");
  Serial.println(period);


  while (period > 0) {

    unsigned long time_now = millis();

    while ( (millis() < time_now + (period * 1000)) && ( period > 0) ) {

      if (Serial.available()) {
        run_tg_temp = Serial.read();

        if (run_tg_temp == '1') { //call pwm

          Serial.println("pwm from run tg!");
          pwm(data);
          period = 0;
          break;
        }
        else if (run_tg_temp == '2') {
          Serial.println("called flash from run tg ");
          flash(data);
        }

      }

    }

    //digitalWrite(led, HIGH);
    //write(pwm_value);
    analogWrite(led, ( (255/10) * pwm_value) );
    delay(1000);
    digitalWrite(led, LOW);

  }

}

int led_switch(char data[]){
  Serial.println("led switch");

  if( data[5]  == '1'){
    digitalWrite(led, LOW);
    led = 11;
    digitalWrite(led, HIGH);
    Serial.println("led 5. index");
  }
  else if( data[6]  == '1' ){
    digitalWrite(led, LOW);
    led = led_2;
    digitalWrite(led, HIGH);
    Serial.println("led 6. index");
  }
  else if(data[7] == '1'){
    digitalWrite(led, LOW);
    led = led_3;
    digitalWrite(led, HIGH);
    Serial.println("led 7. index");
  }
  return led;
  
}

void loop() {

  if (Serial.available()) {

    incomingValue = Serial.read();
    c_temp = incomingValue;

    if (c_temp == '*') { //start bit

      index_counter = 0;
      data[index_counter] = c_temp;
      index_counter++;
    }
    else {
      data[index_counter] = c_temp;

      if ((data[1] == '1')) { // pwm
        pwm(data);
      }
      else if (data[2] == '1') { //flash
        flash(data);
      }
      else if (data[3] == '1') { // run tg
        run_tg(data);
      }
      else if( data[4] == '1' ){ //led switch
        led = led_switch(data);
      }

      index_counter++;
    }
  }
}
