#include <SoftwareSerial.h>

const int rxPin = D1;  // RX do YS-IRTM
const int txPin = D2;  // TX do YS-IRTM

SoftwareSerial ysIRTM(rxPin, txPin);  

void setup() {
  Serial.begin(9600);    
  ysIRTM.begin(9600);    
  Serial.println("Controle do Ar Condicionado com YS-IRTM iniciado.");
  Serial.println("Envie 'on' para ligar ou 'off' para desligar o ar-condicionado.");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "on") {
      sendIRCommand(0x1FE48B7);  
      Serial.println("Comando para ligar o ar-condicionado enviado.");
    } else if (command == "off") {
      sendIRCommand(0x1FE58A7); 
      Serial.println("Comando para desligar o ar-condicionado enviado.");
    } else {
      Serial.println("Comando inválido. Use 'on' ou 'off'.");
    }
  }

  
  if (ysIRTM.available()) {
    while (ysIRTM.available()) {
      int received = ysIRTM.read();
      Serial.print("Dados recebidos do YS-IRTM: ");
      Serial.println(received, HEX);  
    }
  }
  delay(200);  
}

void sendIRCommand(unsigned long irCode) {
  byte command[4]; 
  command[0] = (irCode >> 24) & 0xFF;  
  command[1] = (irCode >> 16) & 0xFF;  
  command[2] = (irCode >> 8) & 0xFF;  
  command[3] = irCode & 0xFF;          

  ysIRTM.write(0xA1);  // Comando para iniciar envio IR (ajustar conforme manual do YS-IRTM)
  for (int i = 0; i < 4; i++) {
    ysIRTM.write(command[i]);  
  }
  ysIRTM.write(0xAE);  // Comando para finalizar envio (ajustar conforme necessário)
}
