int led = 11;
int value = 0;
int unit_brightness = 255 / 10;
unsigned int integerValue=0;  // Max value is 65535
char incomingByte;

void write(int multi ){
  analogWrite(led, unit_brightness * multi);
  //Serial.println((int)unit_brightness * multi );
}

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}
 
void loop() {
  if (Serial.available() > 0) {   // something came across serial
    integerValue = 0;         // throw away previous integerValue
    while(1) {            // force into a loop until 'n' is received
      incomingByte = Serial.read();
      if (incomingByte == '\n') break;   // exit the while(1), we're done receiving
      if (incomingByte == -1) continue;  // if no characters are in the buffer read() returns -1
      integerValue *= 10;  // shift left 1 decimal place
      // convert ASCII to integer, add, and shift left 1 decimal place
      integerValue = ((incomingByte - 48) + integerValue);
    }
    Serial.println(integerValue);   // Do something with the value

    switch(integerValue){
       case 0:
        write(0);
        Serial.println("LED is 0- OFF");
        break;
       case 1:
        write(1);
        Serial.println("10%");
        break;
       case 2:
        write(2);
        Serial.println("20%");
        break;
       case 3:
        write(3);
        Serial.println("30%");
        break;
       case 4:
        write(4);
        Serial.println("40%");
        break;
       case 5:
        write(5);
        Serial.println("50%");
        break;
       case 6:
        write(6);
        Serial.println("60%");
        break;
       case 7:
        write(7);
        Serial.println("70%");
        break;
       case 8:
        write(8);
        Serial.println("80%");
        break;
       case 9:
        write(9);
        Serial.println("90%");
        break;
       case 10:
        write(10);
        Serial.println("%100");
        break;
        
    }
    
  }
}
