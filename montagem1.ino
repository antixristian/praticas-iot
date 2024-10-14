#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRutils.h>

const char* ssid = "IC-LCI";  
const char* password = "aluno.dcc!"; 

const uint16_t kIrLed = D6;  
IRsend irsend(kIrLed); 

ESP8266WebServer server(80); 

unsigned long codeToTurnOn = 0x1FE48B7;  
unsigned long codeToTurnOff = 0x1FE58A7; 

void setup() {
  Serial.begin(115200);
  irsend.begin();  

  Serial.print("Conectando-se a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("Conectado ao WiFi");

  server.on("/", handleRoot);
  server.on("/on", handleTurnOn);
  server.on("/off", handleTurnOff);

  server.begin();  
  Serial.println("Servidor HTTP iniciado");
  Serial.print("IP do NodeMCU: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();  
}

void handleRoot() {
  String htmlPage = "<html><body>";
  htmlPage += "<h1>Controle do Ar Condicionado</h1>";
  htmlPage += "<p><a href=\"/on\"><button style=\"font-size:20px;\">Ligar</button></a></p>";
  htmlPage += "<p><a href=\"/off\"><button style=\"font-size:20px;\">Desligar</button></a></p>";
  htmlPage += "</body></html>";

  server.send(200, "text/html", htmlPage);
}

void handleTurnOn() {
  irsend.sendNEC(codeToTurnOn, 32);  
  server.send(200, "text/html", "<html><body><h1>Comando Ligar Enviado</h1><a href=\"/\">Voltar</a></body></html>");
  Serial.println("Comando para ligar enviado.");
}

void handleTurnOff() {
  irsend.sendNEC(codeToTurnOff, 32);  
  server.send(200, "text/html", "<html><body><h1>Comando Desligar Enviado</h1><a href=\"/\">Voltar</a></body></html>");
  Serial.println("Comando para desligar enviado.");
}
