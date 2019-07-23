int led = 11;
int incomingValue = 0;
int index_counter = 1;

char c_temp;
int unit_brightness = 255 / 10;
char data[9];

void write(int multi ) {
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
  write(0);

  int fourth_index = data[5] - '0';
  int fifth_index = data[6] - '0';

  int period = (fourth_index * 10) + fifth_index;
    Serial.println("----");
    Serial.print(fourth_index);
    Serial.print(" ");
    Serial.print(fifth_index);
    Serial.println("period is here : ");
    Serial.print(period);


  while (period > 0) {

    unsigned long time_now = millis();
    Serial.println("period check while ");
    Serial.print(period);

    while ( (millis() < time_now + (period * 1000)) && ( period > 0) ) {

      if (Serial.available()) {

        flash_temp = Serial.read();

        if (flash_temp == '1') {
          Serial.println("pwm from flash ! ");
          period =0;
          pwm(data);
          period =0;
          /*if(Serial.available()){
            char temp_c = Serial.read();
            Serial.println("readed");
            int temp= temp_c - '0';
            write(temp);
          }*/
          break;
       
        }
        /*else if(flash_temp = '2'){
          Serial.println("flash from flash");
          flash(data);
          break;
        }*/
        else if(flash_temp = '3'){
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
  Serial.print("here1 ");
  Serial.println(pwm_value);

  int seventh_index = data[7] - '0';
  int eighth_index = data[8] - '0';
  int period = (seventh_index * 10) + eighth_index;
  Serial.print("here2 ");
  Serial.println(period);


  while (period > 0) {

    unsigned long time_now = millis();

    while ( (millis() < time_now + (period * 1000)) && ( period > 0) ) {

      if (Serial.available()) {
        run_tg_temp = Serial.read();

        if (run_tg_temp == '1') { //call pwm
          
          Serial.println("here run tg!");
          pwm(data);
          Serial.println("after pwm");
          period = 0;
          Serial.println("after period ");
          Serial.print(period);
          break;
        }
        else if(run_tg_temp == '2'){
          Serial.println("called flash from run tg ");
          flash(data);
        }/*
        else if(run_tg_temp == '3'){
          Serial.println("run tg from run tg");
        }*/

      }

    }

    //digitalWrite(led, HIGH);
    write(pwm_value);
    delay(1000);
    digitalWrite(led, LOW);

  }

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

      index_counter++;
    }
  }
}
