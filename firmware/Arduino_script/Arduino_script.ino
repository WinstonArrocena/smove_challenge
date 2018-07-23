//Disclaimer: this code is not tested, may not work, just for the purposes of this exercise
#include <Wire.h>

#define SLAVE_ADDR 	0x48
#define BAUD_RATE	9600
#define RELAY1		3
#define RELAY2		5
#define RPI_POWER_CTRL	7
#define FUEL_SENSOR	A0

char flag = 0;
char counter = 0;

void setup() {
  pinMode(RELAY1, OUTPUT);          // sets the digital pin 3 as output
  pinMode(RELAY2, OUTPUT);          // sets the digital pin 5 as output
  pinMode(RPI_POWER_CTRL, OUTPUT);          // sets the digital pin 7 as output
  
  Wire.begin(SLAVE_ADDR);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(BAUD_RATE);           // start serial for output
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
      digitalWrite(RPI_POWER_CTRL, LOW);       // sets the RPI_POWER_CTRL off
      delay(100);
      digitalWrite(RPI_POWER_CTRL, HIGH);       // sets the RPI_POWER_CTRL on
      counter=0;
      flag=0;
    }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int num) {
  while (Wire.available()) { 
    flag = 1;
    char c = Wire.read(); // receive byte as a character
    Serial.println(c, HEX);         // print the char
    switch(c){
      case 0xFE: //read fuel sensor
      int rawValue;
      rawValue = analogRead(FUEL_SENSOR); 
      byte buffer[2];
      buffer[0] = rawValue >> 8;
      buffer[1] = rawValue & 0xFF;
      // ?? need to test here, not very sure how this works in arduino
      Wire.write(buffer, 2); // sends buffer
      break;
      case 0xA1: //on relay 1
        digitalWrite(RELAY1, HIGH);       // sets the digital pin 3 on
      break;
      case 0xB1: //on relay 2
        digitalWrite(RELAY2, HIGH);       // sets the digital pin 5 on
      break;
      case 0xA0: //off relay 1
        digitalWrite(RELAY1, LOW);       // sets the digital pin 3 off
      break;
      case 0xB0://off relay 2
        digitalWrite(RELAY2, LOW);       // sets the digital pin 5 off
      break;
      default:
      break;
      }
  }
}

