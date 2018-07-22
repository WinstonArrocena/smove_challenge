//Disclaimer: this code is not tested, may not work, just for the purposes of this exercise
#include <Wire.h>

char flag = 0;
char counter = 0;

void setup() {
  pinMode(3, OUTPUT);          // sets the digital pin 3 as output
  pinMode(5, OUTPUT);          // sets the digital pin 5 as output
  pinMode(7, OUTPUT);          // sets the digital pin 7 as output
  
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop() {
  delay(100);
  //check here if no i2c communication in 10 seconds
  if(!flag){
    counter++;
  } else {
    counter=0;
    flag=0;
  }
  if(counter>100){ // > 10 seconds?
    // we will reset the Pi
      digitalWrite(7, LOW);       // sets the digital pin 7 off
      delay(100);
      digitalWrite(7, HIGH);       // sets the digital pin 7 on
      counter=0;
      flag=0;
    }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    flag = 1;
    char c = Wire.read(); // receive byte as a character
    Serial.println(c, HEX);         // print the char
    switch(c){
      case 0xFE: //read fuel sensor
      int rawValue;
      rawValue = analogRead(A0); 
      byte buffer[2];
      buffer[0] = rawValue >> 8;
      buffer[1] = rawValue & 0xFF;
      // ?? need to test here, not very sure how this i2c works in arduino
      Wire.beginTransmission(9); // transmit to device
      Wire.write(buffer, 2); // sends buffer
      Wire.endTransmission(); // stop transmitting
      break;
      case 0xA1: //on relay 1
        digitalWrite(3, HIGH);       // sets the digital pin 3 on
      break;
      case 0xB1: //on relay 2
        digitalWrite(5, HIGH);       // sets the digital pin 5 on
      break;
      case 0xA0: //off relay 1
        digitalWrite(3, LOW);       // sets the digital pin 3 off
      break;
      case 0xB0://off relay 2
        digitalWrite(5, LOW);       // sets the digital pin 5 off
      break;
      default:
      break;
      }
  }
}

