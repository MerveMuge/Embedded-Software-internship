int incomingValue;
char incomingChar;
bool isStarted;

int index;
char write_buffer[8];
int unit_brightness = 255 / 10;

int led = 11;
int led_2 = 10;
int led_3 = 9;

void write(int multi) {
  analogWrite(led, unit_brightness * multi);
}

void pwm(char write_buffer[]) {
  if (write_buffer[5] == '1') {
    write(1);
  } else if (write_buffer[5] == '2') {
    write(2);
  } else if (write_buffer[5] == '3') {
    write(3);
  } else if (write_buffer[5] == '4') {
    write(4);
  } else if (write_buffer[5] == '5') {
    write(5);
  } else if (write_buffer[5] == '6') {
    write(6);
  } else if (write_buffer[5] == '7') {
    write(7);
  } else if (write_buffer[5] == '8') {
    write(8);
  } else if (write_buffer[5] == '9') {
    write(9);
  } else if ((write_buffer[5] == '0') && (write_buffer[4] == '1')) {
    write(10);
  } else if ((write_buffer[5] == '0') && (write_buffer[4] == '0')) {
    write(0);
  }
}

bool break_checker(int period ){
    bool check = true;

    unsigned long time_now = millis();

    while ( (millis() < time_now + (period * 1000)) ) {
      if (Serial.available()) {
        check = false;
        break;
      }
    }
    if ( !check ) {
      return check;
    }
  
}

void flash(char * write_buffer) {
  digitalWrite(led, LOW);
  int check = 0;

  int sixth_index = write_buffer[6] - '0';
  int seventh_index = write_buffer[7] - '0';

  int period = ( sixth_index * 10 ) + seventh_index;

  while (period > 0) {

    if( !(break_checker(period)) ){
      break;
    }

    digitalWrite(led, HIGH);

    if( !(break_checker(period)) ){
      break;
    }
    digitalWrite(led, LOW);
    
  }

}

void print_buffer(char * write_buffer) {
  for (int i = 0; i < 8 ; i++) {
    Serial.println(write_buffer[i]);
  }
}

void guidance_to_start_menu(char * write_buffer) {

  if (write_buffer[0] == '1') {
    pwm(write_buffer);
  }
  else if (write_buffer[0] == '2') {
    //Serial.println("flash");
    flash(write_buffer);
  }
  else if (write_buffer[0] == '3') {
    Serial.println("run tg");
  }
  else if (write_buffer[0] == '4') {
    Serial.println("led switch");
  }

}

void setup() {
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() ) {

    incomingValue = Serial.read();
    incomingChar = incomingValue;

    if (incomingChar == '&' ) { //stop byte
      isStarted = false;
      //print_buffer(write_buffer);
      guidance_to_start_menu(write_buffer);
    }
    else if (incomingChar == '*') { //start byte
      isStarted = true;
      index = 0;
    }
    else if (isStarted == true) {
      write_buffer[index] = incomingChar;
      index++;
    }

  }
}
