int led = 11;
int led_2 = 10;
int led_3 = 9;

int unit_brightness = 255 / 10;


void clear_page()
{
  Serial.write(27);
  Serial.print("[2J"); // clear screen
  Serial.write(27); // ESC
  Serial.print("[H"); // cursor to home
}

void homePage() {
  clear_page();
  Serial.println("Main Menu");
  Serial.println("---------");
  Serial.println("Please give an input which is a number from the options available");
  Serial.println("1. PWM ");
  Serial.println("2. Flasher");
  Serial.println("3. PWM & Flasher - run together");
  Serial.println("4. Led Selection");
  String ch;
  while (1) {
    ch = Serial.readString();
    ch.trim();
    if (ch == "1") {
      pwm();
    }
    else if (ch == "2") {
      flash();
    }
    else if (ch == "4") {
      led = led_swithc();
    }

  }

}

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  homePage();
}


void write(int multi ) {
  analogWrite(led, unit_brightness * multi);
}

void printBrightnessRate(char ch) {
  Serial.println("r/R -> return Home Page");
  Serial.print("brightness rate : %");
  Serial.print(ch);
  Serial.print("0");
}

void checkQuit(String chInPWM) {
  chInPWM = Serial.readString();
  chInPWM.trim();
  if ((chInPWM == "q") || (chInPWM == "Q") ) {
    homePage();
  }
}

void pwm() {
  clear_page();
  Serial.println("r/R -> return Home Page");
  Serial.println("Your input should be in the 0-10 range.");
  Serial.println("This input could help adjust the Led' brightness rate.");
  Serial.println("ps. 10 for 100% rate and 0 for 0%. ");
  write(0);

  String chPWM;
  while (1) {
    chPWM = Serial.readString();
    chPWM.trim();

    if (chPWM == "0") {
      clear_page();
      printBrightnessRate('0');
      write(0);
    }
    else if (chPWM == "1") {
      clear_page();
      printBrightnessRate('1');
      write(1);
    }
    else if (chPWM == "2") {
      clear_page();
      printBrightnessRate('2');
      write(2);
    }
    else if (chPWM == "3") {
      clear_page();
      printBrightnessRate('3');
      write(3);
    }
    else if (chPWM == "4") {
      clear_page();
      printBrightnessRate('4');
      write(4);
    }
    else if (chPWM == "5") {
      clear_page();
      printBrightnessRate('5');
      write(5);
    }
    else if (chPWM == "6") {
      clear_page();
      printBrightnessRate('6');
      write(6);
    }
    else if (chPWM == "7") {
      clear_page();
      printBrightnessRate('7');
      write(7);
    }
    else if (chPWM == "8") {
      clear_page();
      printBrightnessRate('8');
      write(8);
    }
    else if (chPWM == "9") {
      clear_page();
      printBrightnessRate('9');
      write(9);
    }
    else if (chPWM == "10") {
      clear_page();
      printBrightnessRate('1');
      Serial.print("0");
      write(10);
    }
    else if ((chPWM == "r") || (chPWM == "R") ) {
      homePage();
    }
  }

}

void flash() {
  digitalWrite(led, LOW);
  clear_page();
  Serial.println("r/R -> return Home Page");
  Serial.println("Your input would be a flash loop time.");
  Serial.println("ps. if input is 2, calculation should be like : 60/2= 30 ");
  write(0);

  String chFlash;
  chFlash = Serial.readString();
  chFlash.trim();

  while ( (chFlash != "r") || (chFlash != "R") ) {
    String chFlashIn;
    chFlash = Serial.readString();
    chFlash.trim();
    if ( (chFlash != "") ) {
      Serial.print("Input: ");
      Serial.println(chFlash);
      chFlashIn = chFlash;
      int divider = chFlash.toInt();
      int period = 60 / divider;
      Serial.print("period ");
      Serial.println(period);
      /*unsigned long StartTime = millis();
        Serial.println(StartTime);
        unsigned long EndTime = period*1000 + StartTime;
        do{
        StartTime= millis();
        }while(StartTime >= EndTime);
        digitalWrite(led, HIGH);
      */
      if( (chFlashIn == "r") || (chFlashIn == "R") ){
          homePage();
      }
      else if((chFlashIn != "r") && (chFlashIn != "R") && (period <= 0) ){
        Serial.println("Invalid Input");
      }
      while (period >0) {
        delay( 1000 * period );
        digitalWrite(led, HIGH);
        delay(1000 * period);
        digitalWrite(led, LOW);
      }
      
    }
  }
 /* if ( (chFlash == "r") || (chFlash == "R") ) {
    homePage();
  }*/
}

int led_swithc() {
  clear_page();
  Serial.println("r/R -> return Home Page");
  Serial.println("There are 3 led which are numbers; 1,2,3. Select one of them.");
  String ch_led;
  ch_led = Serial.readString();
  ch_led.trim();

  while ( 1 ) {
    ch_led = Serial.readString();
    ch_led.trim();
    if (ch_led == "1") {
      digitalWrite(led, LOW);
      led = 11;
      digitalWrite(led, HIGH);
    } else if (ch_led == "2") {
      digitalWrite(led, LOW);
      led = led_2;
      digitalWrite(led, HIGH);
    } else if (ch_led == "3") {
      digitalWrite(led, LOW);
      led = led_3;
      digitalWrite(led, HIGH);
    } else if ( (ch_led != "1") && (ch_led != "2") && (ch_led != "3") && (ch_led != "") && (ch_led != "r") && (ch_led != "R") ) {
      Serial.println("Wrong input");
    } else if( (ch_led == "r") || (ch_led == "R") ){
          homePage();
      }
  }
  return led;

}

void loop() {
  if (Serial.available()) {
    String ch;
    ch = Serial.readString();
    ch.trim();
    
    if (ch == "1") {
      pwm();
    }
    else if (ch == "2") {
      flash();
    }
    else if (ch == "4") {
      led = led_swithc();
    }

  }
}
