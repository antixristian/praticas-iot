#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <Arduino.h>

#define sendPin D8

const char* ssid = "IC-LCI";
const char* password = "aluno.dcc!";

const char* mqtt_server = "10.4.44.19";

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP280 bme;

unsigned long delayTime = 2000; 
unsigned long lastMsg = 0;

IRsend irsend(sendPin);

// Códigos IR (Você precisará adicionar os códigos para ligar e ajustar a temperatura)
uint16_t rawDataOff[199] = {4388, 4420, 526, 1628, 524, 552, 526, 1628, 526, 1630, 524, 552, 524, 554, 526, 1628, 524, 554, 524, 552, 524, 1632, 524, 552, 526, 552, 526, 1630, 524, 1632, 524, 552, 526, 1630, 526, 550, 524, 1632, 524, 1630, 550, 1604, 526, 1650, 504, 550, 526, 1652, 528, 1626, 528, 1628, 504, 550, 526, 550, 526, 552, 526, 546, 556, 1606, 524, 552, 524, 552, 524, 1652, 502, 1630, 524, 1632, 522, 554, 522, 552, 524, 552, 524, 552, 524, 576, 502, 550, 528, 550, 550, 526, 526, 1628, 526, 1626, 528, 1628, 524, 1630, 524, 1630, 524, 5232, 4380, 4420, 520, 1634, 520, 556, 520, 1654, 498, 1656, 498, 558, 518, 558, 518, 1656, 496, 560, 516, 558, 518, 1656, 496, 560, 516, 558, 516, 1658, 496, 1656, 498, 558, 516, 1658, 496, 560, 514, 1658, 494, 1660, 494, 1658, 492, 1662, 492, 584, 492, 1660, 492, 1662, 490, 586, 488, 588, 488, 586, 478, 598, 464, 1688, 464, 612, 462, 612, 462, 1692, 460, 1718, 434, 1718, 436, 616, 460, 616, 460, 616, 460, 616, 458, 618, 458, 616, 460, 616, 458, 618, 458, 1718, 434, 1718, 434, 1718, 434, 1718, 434, 1718, 434};
uint16_t rawDataOn[199] = { /* Insira o código IR para ligar aqui */ };
uint16_t* rawDataTemp[7]; // 18 a 24 graus Celsius

// Inicializa os códigos de temperatura
void initTempCodes() {
    rawDataTemp[0] = new uint16_t[199]; // Código para 18 graus
    rawDataTemp[1] = new uint16_t[199]; // Código para 19 graus
    rawDataTemp[2] = new uint16_t[199]; // Código para 20 graus
    rawDataTemp[3] = new uint16_t[199]; // Código para 21 graus
    rawDataTemp[4] = new uint16_t[199]; // Código para 22 graus
    rawDataTemp[5] = new uint16_t[199]; // Código para 23 graus
    rawDataTemp[6] = new uint16_t[199]; // Código para 24 graus

    // Substitua com os códigos IR corretos para cada temperatura
    // Exemplo para 18 graus:
    // rawDataTemp[0][0] = 4500;
    // rawDataTemp[0][1] = 4500;
    // ... preencha o restante do array ...
}

WiFiClient espClient;
PubSubClient client(espClient);

// Variáveis para controle do estado do ar-condicionado
bool acState = false; // false = desligado, true = ligado
int acTemp = 24;     // Temperatura inicial

void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("ar-condicionado/estado");
      client.subscribe("ar-condicionado/temperatura");
      client.subscribe("sensor/temperatura");
      client.subscribe("sensor/umidade");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void sendIRCommand(uint16_t* command, unsigned int commandLength) {
  irsend.sendRaw(command, commandLength, 38);
  delay(500);
  irsend.sendRaw(command, commandLength, 38); //Repete o comando para garantir que foi recebido
  delay(500); 
  irsend.sendRaw(command, commandLength, 38); //Repete o comando para garantir que foi recebido
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    messageTemp += (char)payload[i];
  }
  Serial.println(messageTemp);

  if (String(topic) == "ar-condicionado/estado") {
    if (messageTemp == "ON") {
      if (!acState) {
        sendIRCommand(rawDataOn, sizeof(rawDataOn) / sizeof(rawDataOn[0]));
        acState = true;
        Serial.println("Ar-condicionado Ligado");
      }
    } else if (messageTemp == "OFF") {
      if (acState) {
        sendIRCommand(rawDataOff, sizeof(rawDataOff) / sizeof(rawDataOff[0]));
        acState = false;
        Serial.println("Ar-condicionado Desligado");
      }
    }
  } else if (String(topic) == "ar-condicionado/temperatura") {
    int newTemp = messageTemp.toInt();
    if (newTemp >= 18 && newTemp <= 24) {
      acTemp = newTemp;
      if (acState) {
          sendIRCommand(rawDataTemp[acTemp - 18], sizeof(rawDataTemp[0]) / sizeof(rawDataTemp[0][0]));
          Serial.print("Temperatura ajustada para ");
          Serial.print(acTemp);
          Serial.println(" graus");
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println(F("BME280 test"));
  bool status;
  status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
  initTempCodes();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  irsend.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > delayTime) {
    lastMsg = now;

    // Publicar temperatura e umidade
    float temperature = bme.readTemperature();
    float humidity = bme.readHumidity();

    char tempString[8];
    char humString[8];

    dtostrf(temperature, 1, 2, tempString);
    dtostrf(humidity, 1, 2, humString);

    client.publish("sensor/temperatura", tempString);
    client.publish("sensor/umidade", humString);

    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.println(" *C");
    Serial.print("Umidade: ");
    Serial.print(humidity);
    Serial.println(" %");
  }
}