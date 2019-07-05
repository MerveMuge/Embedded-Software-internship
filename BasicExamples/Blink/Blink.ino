
int led =13;
void setup() {
  pinMode(led, OUTPUT);  

}

void loop() {

  int i=1;
  do {                      // switch on quietly
  digitalWrite(led, HIGH);   // set the LED on
  delayMicroseconds(i);     // 
  digitalWrite(led, LOW);    // set the LED off
  delayMicroseconds(999-i); // 
  i++;                      // 
  } while (i < 999);        // 1 sn=1000milisec

  digitalWrite(led, HIGH); 

  //delay(500);              // left .5 sec bright 

  i=1;
  do {                      // switch off quietly
  digitalWrite(led, HIGH);   // set the LED on
  delayMicroseconds(999-i); 
  digitalWrite(led, LOW);    // set the LED off
  delayMicroseconds(i);     
  i++;
  } while (i < 999);  

  //delay(500);             // wait off for .5 sec 
}                        
