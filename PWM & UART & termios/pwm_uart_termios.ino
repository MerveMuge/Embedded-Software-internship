int led = 11;
int unit_brightness = 255 / 10;

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void write(int multi ) {
  analogWrite(led, unit_brightness * multi);
}


void loop() {
  if (Serial.available()) {
    String ch;
    ch = Serial.readString();
    ch.trim();
    if (ch == "on" || ch == "ON") {
      write(10);
    }
    else if (ch == "off" || ch == "OFF") {
      write(0);
    }
    else if (ch == "1") {
      write(1);
    }
    else if (ch == "2") {
      write(2);
    }
    else if (ch == "3") {
      write(3);
    }
    else if (ch == "4") {
      write(4);
    }
    else if (ch == "5") {
      write(5);
    }
    else if (ch == "6") {
      write(6);
    }
    else if (ch == "7") {
      write(7);
    }    
    else if (ch == "8") {
      write(8);
    }
    else if (ch == "9") {
      write(9);
    }
    else if (ch == "10"){
      write(10);
    }
    
    
  }
}
