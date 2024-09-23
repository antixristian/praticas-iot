#include <TM1637Display.h>

const int trigPin = D6; 
const int echoPin = D5; 

const int CLK = D6;   
const int DIO = D5;   

TM1637Display display(CLK, DIO);

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  display.setBrightness(0x0f);
}

void loop() {
  long duration;
  float distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = (duration * 0.034) / 2;

  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  int displayDistance = (int)distance;
  if (displayDistance > 9999) {
    displayDistance = 9999;  
  }
  display.showNumberDec(displayDistance);

  delay(500);
}
