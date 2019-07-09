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
      Serial.println("led is on");
      write(10);
    }
    else if (ch == "off" || ch == "OFF") {
      Serial.println("led is off");
      write(0);
    }
    else if (ch == "0") {
      Serial.println("brightness rate : %0");
      write(0);
    }
    else if (ch == "1") {
      Serial.println("brightness rate : %10");
      write(1);
    }
    else if (ch == "2") {
      Serial.println("brightness rate : %20");
      write(2);
    }
    else if (ch == "3") {
      Serial.println("brightness rate : %30");
      write(3);
    }
    else if (ch == "4") {
      Serial.println("brightness rate : %40");
      write(4);
    }
    else if (ch == "5") {
      Serial.println("brightness rate : %50");
      write(5);
    }
    else if (ch == "6") {
      Serial.println("brightness rate : %60");
      write(6);
    }
    else if (ch == "7") {
      Serial.println("brightness rate : %70");
      write(7);
    }    
    else if (ch == "8") {
      Serial.println("brightness rate : %80");
      write(8);
    }
    else if (ch == "9") {
      Serial.println("brightness rate : %90");
      write(9);
    }
    else if (ch == "10"){
      Serial.println("brightness rate : %100");
      write(10);
    }
    
    
  }
}
