char incomingByte;
String charsIn = "";
char printout[20];  //max char to print: 20

void setup() {
  Serial.begin(9600);
}
 
void loop() {
  
  if (Serial.available() > 0) {   // something came across serial
      incomingByte = Serial.read();
      charsIn.concat(incomingByte);
    }
  if(charsIn != ""){
      Serial.println("How are you, " + charsIn);
      charsIn.toCharArray(printout, 21);
      charsIn = "";
  }
       
 }
