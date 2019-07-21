int led = 11;
int incomingValue = 0;
int index_counter =1;

char c_temp;
int unit_brightness = 255 / 10;
char data[8];

void write(int multi ) {
  analogWrite(led, unit_brightness * multi);
}

void setup() {
  Serial.begin(115200);
  //myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  int incomingValue = 0;
  
}

int value;
char temp;

void loop() {
  
  if (Serial.available()){
    incomingValue = Serial.read();
    c_temp = incomingValue;
    Serial.print(c_temp);
    if(c_temp == '*'){
      index_counter = 0;
      data[index_counter] = c_temp;
      //Serial.print(c_temp);
      index_counter++;
    }else{
      data[index_counter] = c_temp;
      
      if((index_counter == 1) && (c_temp == '1')){
        digitalWrite(led, HIGH);
      }
      index_counter++;
    }
    
    
    
  }
  
}
