#include <IRremoteESP8266.h>
#include <IRsend.h>

const int IR_SEND_PIN = D6;
IRsend irsend(IR_SEND_PIN);  

unsigned long codeToTurnOn = 0x1FE48B7; 
unsigned long codeToTurnOff = 0x1FE58A7; 

void setup() {
  Serial.begin(9600);
  irsend.begin();  
  Serial.println("Pronto para receber comandos 'on' ou 'off'.");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');  
    command.trim();  

    if (command == "on") {
      irsend.sendNEC(codeToTurnOn, 32); 
      Serial.println("Comando para ligar o ar condicionado enviado.");
    } else if (command == "off") {
      irsend.sendNEC(codeToTurnOff, 32); 
      Serial.println("Comando para desligar o ar condicionado enviado.");
    } else {
      Serial.println("Comando inválido. Use 'on' ou 'off'.");
    }
  }

  delay(200);  
}
