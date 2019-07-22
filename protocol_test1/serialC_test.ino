int led = 11;
int incomingValue = 0;
int index_counter =1;

char c_temp;
int unit_brightness = 255 / 10;
char data[9];

void write(int multi ) {
  analogWrite(led, unit_brightness * multi);
}

void setup() {
  Serial.begin(115200);
  int incomingValue = 0; 
}

void pwm(char data[]){
  if(data[6] == '1'){
     write(1);
  }else if(data[6] == '2'){
     write(2);
  }else if(data[6] == '3'){
     write(3);
  }else if(data[6] == '4'){
     write(4);
  }else if(data[6] == '5'){
     write(5);
  }else if(data[6] == '6'){
     write(6);
  }else if(data[6] == '7'){
     write(7);
  }else if(data[6] == '8'){
     write(8);
  }else if(data[6] == '9'){
     write(9);
  }else if((data[6] == '0') && (data[5] == '1')){
     write(10);
  }else if((data[6] == '0') && (data[5] == '0')){
     write(0);
  }
}

int flash_break( char flash_temp ){
    
    if ( (flash_temp == '1') || (flash_temp == '2') || (flash_temp == '3') || (flash_temp == '4') ) {
      Serial.print("flash term is here: ");
      Serial.println(flash_temp);
      return 1;
    }else{
      return 0;
    }
}

int listener( char flash_temp ){
  
       if( flash_break(flash_temp) == 1 ){
         //set period = -1;
         Serial.println("first");
         //data[2] = '0';
         return -1;
       }else{
        return 1;
       }
}

void flash(char data[]){
  
  // flash info in 4-5 indexs
  char flash_temp;
  write(0); 
  
  int fourth_index = data[5]- '0';
  int fifth_index = data[6] - '0';

  int period = (fourth_index * 10) + fifth_index;
  Serial.print("here ");
  Serial.print("period ");
  Serial.println(period);
   
  while(period > 0){

     /*if( flash_break(flash_temp) == 1 ){
         period = -1;
         Serial.println("first");
         //data[2] = '0';
     }*/
     /*if(listener(flash_temp) == -1){
      period = -1;
     }*/
    
    flash_temp = Serial.read();
    if(flash_temp == '1'){
      data[2] = '0';
      //period = -1;
      pwm(data);
      period = -1;
    }
    
    unsigned long time_now = millis();
    while ( (millis() < time_now + (period * 1000)) && ( period > 0) ) {

     /*if(listener(flash_temp) == -1){
      period = -1;
     }*/
    if(flash_temp == '1'){
      data[2] = '0';
      pwm(data);
      period =-1;
    }     
     
    }
    
    /* if( flash_break(flash_temp) == 1 ){
         period = -1;
         Serial.println("third");
         //data[2] = '0';
     }*/
    /* if(listener(flash_temp) == -1){
      period = -1;
     }*/
    if(flash_temp == '1'){
      data[2] = '0';
      pwm(data);
      period = -1;
    } 
  digitalWrite(led, HIGH);
  
     /*if( flash_break(flash_temp) == 1 ){
         period = -1;
         Serial.println("fourth");
         //data[2] = '0';
     }*/
     /*if(listener(flash_temp) == -1){
      period = -1;
     }*/
    if(flash_temp == '1'){
      data[2] = '0';
      pwm(data);
      period = -1;
    }    
  unsigned long time_now_high = millis();
    if(flash_temp == '1'){
      data[2] = '0';
      pwm(data);
      period = -1;
    } 
  while ( (millis() < time_now_high + 1000) ) {
    
         /*if( flash_break(flash_temp) == 1 ){
         period = -1;
         Serial.println("fifth");
         //data[2] = '0';
         }*/
    /* if(listener(flash_temp) == -1){
      period = -1;
     }*/
    if(flash_temp == '1'){
      data[2] = '0';
      pwm(data);
      period = -1;
    } 
     
  }

  //delay(1000);
  
    /* if( flash_break(flash_temp) == 1 ){
         period = -1;
         Serial.println("third");
         //data[2] = '0';
     }*/
     /*if(listener(flash_temp) == -1){
      period = -1;
     }*/
    if(flash_temp == '1'){
      data[2] = '0';
      pwm(data);
      period = -1;
    }   
  digitalWrite(led, LOW);
  
     /*if( flash_break(flash_temp) == 1 ){
         period = -1;
         Serial.println("sixth");
         //data[2] = '0';
     }*/
     /*if(listener(flash_temp) == -1){
      period = -1;
     } */
    if(flash_temp == '1'){
      data[2] = '0';
      pwm(data);
      period = -1;
    }      
  } 
  
}

void run_tg(char data[]){
  
  write(0);
  int fifth_index = data[5] - '0';
  int sixth_index = data[6] - '0';
  int pwm = (fifth_index * 10) + sixth_index;
  Serial.print("here1 ");
  Serial.println(pwm);
  
  int seventh_index = data[7] - '0';
  int eighth_index = data[8] - '0';
  int period = (seventh_index * 10) + eighth_index;
  Serial.print("here2 ");
  Serial.println(period);
  
  while(period > 0){
    
    unsigned long time_now = millis();
    while (millis() < time_now + (period * 1000)) {
      
  }
  write(pwm);
  delay(1000);
  digitalWrite(led, LOW);
  }
 
}

void loop() {
  
  if (Serial.available()){
    incomingValue = Serial.read();
    c_temp = incomingValue;
    //Serial.print(c_temp);
    
    if(c_temp == '*'){ //start bit
      
      index_counter = 0;
      data[index_counter] = c_temp;
      index_counter++;
    }else{
      data[index_counter] = c_temp;
      
      if((data[1] == '1')){ // pwm
        pwm(data);
      }else if(data[2] == '1'){ //flash
        flash(data);
        //data[2] = flash(data);
      }else if(data[3] == '1'){ // run tg 
        run_tg(data);
      }
      index_counter++;
    }
  }
}
