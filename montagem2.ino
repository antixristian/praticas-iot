#include <IRremote.h>

const int IR_SEND_PIN = D6;
IRsend irsend;


unsigned long codeToTurnOn = 0x1FE48B7; 
unsigned long codeToTurnOff = 0x1FE58A7; 

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.readString() == "on") {
    irsend.sendNEC(codeToTurnOn, 32); 
    Serial.println("Ar condicionado ligado");
  } else if (Serial.readString() == "off") {
    irsend.sendNEC(codeToTurnOff, 32); 
    Serial.println("Ar condicionado desligado");
  }
  delay(5000); 
}