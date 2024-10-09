#include <SoftwareSerial.h>

SoftwareSerial mySerial(D5, D6); // RX, TX

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);   
}

void loop() {
  if (mySerial.available()) { 
    int data = mySerial.read();
    Serial.print(data, HEX); 
  }

  if (Serial.available()) { 
    int data = Serial.read();
    mySerial.write(data); 
  }
}