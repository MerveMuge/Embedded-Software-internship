int led = 11;
int incomingValue = 0;
int index_counter =1;

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

void pwm(char data[]){
  if(data[6] == '1'){
     write(1);
  }else if(data[6] == '2'){
     write(2);
  }else if(data[6] == '3'){
     write(3);
  }else if(data[6] == '4'){
     write(4);
  }else if(data[6] == '5'){
     write(5);
  }else if(data[6] == '6'){
     write(6);
  }else if(data[6] == '7'){
     write(7);
  }else if(data[6] == '8'){
     write(8);
  }else if(data[6] == '9'){
     write(9);
  }else if((data[6] == '0') && (data[5] == '1')){
     write(10);
  }else if((data[6] == '0') && (data[5] == '0')){
     write(0);
  }
}

void flash(char data[]){
  write(0);
  int fifth_index = data[5] - '0';
  int sixth_index = data[6] - '0';
  int period = (fifth_index * 10) + sixth_index;
  while(period > 0){
    unsigned long time_now = millis();
    while (millis() < time_now + (period * 1000)) {
    }
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  }
 
}

void loop() {
  
  if (Serial.available()){
    incomingValue = Serial.read();
    c_temp = incomingValue;
    //Serial.print(c_temp);
    
    if(c_temp == '*'){ //start bit
      
      index_counter = 0;
      data[index_counter] = c_temp;
      index_counter++;
    }else{
      data[index_counter] = c_temp;
      
      if((data[1] == '1')){ // pwm
        pwm(data);
      }else if(data[2] == '1'){ //flash
        flash(data);
      }
      index_counter++;
    }
    
    
    
  }
  
}
