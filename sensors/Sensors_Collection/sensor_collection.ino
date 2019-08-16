#define TEMPRATURE_PIN 1

#define LDR_LED_PIN 13
#define LDR_INPUT_PIN A0

void setup()
{
  Serial.begin(115200);

  pinMode(LDR_LED_PIN, OUTPUT);
  pinMode(LDR_INPUT_PIN, INPUT);
}

void temprature() {
  int val = analogRead(TEMPRATURE_PIN);
  float mv = ( val / 1024.0) * 5000;
  float cel = mv / 10;
  float farh = (cel * 9) / 5 + 32;

  Serial.print("TEMPRATURE = ");
  Serial.print(cel);
  Serial.print("*C");
  Serial.println();
  delay(1000);
}

void photoresistor_LDR() {
  int ldrStatus = analogRead(LDR_INPUT_PIN);

  if (ldrStatus <= 200) {

    digitalWrite(LDR_LED_PIN, HIGH);
    Serial.print("Its DARK, Turn on the LED : ");
    Serial.println(ldrStatus);

  } else if (ldrStatus > 500) {

    digitalWrite(LDR_LED_PIN, LOW);
    Serial.print("Its BRIGHT, Turn off the LED : ");
    Serial.println(ldrStatus);

  }
}
void loop()
{
  temprature();

  photoresistor_LDR();


}
