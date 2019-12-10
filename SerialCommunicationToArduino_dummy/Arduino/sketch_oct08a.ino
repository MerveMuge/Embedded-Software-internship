#define PIN_LED 9

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop()
{
  if (stringComplete) {
    // print the string when a newline arrives:

    Serial.print(inputString);
    //Serial.print("-");
    inputString = "";
    stringComplete = false;

  }

}

void serialEvent() {

  if (Serial.available()) {
    while (Serial.available()) {
      // get the new byte:
      char inChar = (char)Serial.read();
      // add it to the inputString:
      inputString += inChar;
      // if the incoming character is a newline, set a flag
      // so the main loop can do something about it:
      if (inChar == '\n') {
        stringComplete = true;
      }
    }
  }
}
