int incomingValue;
char incomingChar;
bool isStarted;

int index;
char write_buffer[8];
int unit_brightness = 255 / 10;

int led = 11;
int led_2 = 10;
int led_3 = 9;

int led_array[] = {11, 0, 0};
//led_array[0] = 11;

void write(int multi, int * led_array) {
  for (int i = 0 ; i < 3 ; i++) {
    analogWrite(led_array[i], unit_brightness * multi);
  }
}

void pwm(char write_buffer[], int * led_array) {
    int value = read_two_digit_data(write_buffer , 4, 5);
    write(value, led_array);
  
}

void turn_light_low(int * led_array){
  for (int i = 0; i < 3 ; i++) {
    digitalWrite(led_array[i], LOW);
  }
}

void turn_light_high(int * led_array){
  for (int i = 0; i < 3 ; i++) {
    digitalWrite(led_array[i], HIGH);
  }
}



void led_switch( char * write_buffer ,int * led_array ) {
  
  for (int i = 0; i < 3 ; i++) {
    digitalWrite(led_array[i], LOW);
    led_array[i] = 0 ;
  }
  
  int index = 0;
  if ( write_buffer[1] == '1' ) {
    led_array[index] = 11;
    index++;
  }
  if ( write_buffer[2] == '1' ) {
    led_array[index] = 10;
    index++;
  }
  if ( write_buffer[3] == '1' ) {
    led_array[index] = 9;
    index++;
  }

  turn_light_high(led_array);

}

bool break_checker(int period ) {
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

int read_two_digit_data( char * write_buffer , int higher_order , int lower_order ) {

  int higher_digit = write_buffer[higher_order] - '0';
  int lower_digit = write_buffer[lower_order] - '0';

  int value = ( higher_digit * 10 ) + lower_digit;

  return value;

}


void run_tg( char * write_buffer , int * led_array) {

  turn_light_low(led_array);

  int period = read_two_digit_data( write_buffer, 6, 7);
  int pwm = read_two_digit_data( write_buffer, 4, 5);

  while (period > 0) {

    if ( !(break_checker(period)) ) {
      break;
    }

    write(pwm, led_array);
    if ( !(break_checker(period)) ) {
      break;
    }

    turn_light_low(led_array);

  }

}

void flash(char * write_buffer , int * led_array) {

  turn_light_low(led_array);

  int period = read_two_digit_data( write_buffer, 6, 7);

  while (period > 0) {

    if ( !(break_checker(period)) ) {
      break;
    }

    turn_light_high(led_array);

    if ( !(break_checker(period)) ) {
      break;
    }

    turn_light_low(led_array);

  }
}

void print_buffer(char * write_buffer) {
  for (int i = 0; i < 8 ; i++) {
    Serial.println(write_buffer[i]);
  }
}

void guidance_to_start_menu(char * write_buffer, int * led_array) {

  if (write_buffer[0] == '1') {
    pwm(write_buffer , led_array );
  }
  else if (write_buffer[0] == '2') {
    flash(write_buffer , led_array );
  }
  else if (write_buffer[0] == '3') {
    run_tg( write_buffer, led_array );
  }
  else if (write_buffer[0] == '4') {
    led_switch( write_buffer , led_array );
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
      guidance_to_start_menu(write_buffer , led_array);
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
