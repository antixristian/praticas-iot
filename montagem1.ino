#include <ESP8266WiFi.h>

const char* ssid = "IC-LCI";
const char* password = "aluno.dcc!";

void setup() {
    delay(1000);
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.println();
    Serial.print("Conectando...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(WiFi.status());
}

void loop() {}