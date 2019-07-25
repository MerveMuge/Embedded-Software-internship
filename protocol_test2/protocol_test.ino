int incomingValue;
char incomingChar;
bool isStarted;
int index;
char write_buffer[8];

void print_buffer(char * write_buffer){
  for(int i = 0; i < 8 ; i++){
    Serial.println(write_buffer[i]);
  }
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  if(Serial.available() ){

  incomingValue = Serial.read();
  incomingChar = incomingValue;
  
  if (incomingChar == '&' ){ //stop byte
      isStarted = false;
      print_buffer(write_buffer);
  }
  else if(incomingChar == '*'){ //start byte
    isStarted = true;
    index = 0;
  }
  else if(isStarted == true){
    write_buffer[index] = incomingChar;
    index++;
  }

  //Serial.println(incomingValue);
    
  }
}
