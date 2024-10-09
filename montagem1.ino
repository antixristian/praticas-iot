#include <IRremote.h>

const int RECV_PIN = D5;  

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();  
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("Código recebido: ");
    Serial.println(results.value, HEX);  
    irrecv.resume();  
  }
}
