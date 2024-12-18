#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <DHT.h>


const char* ssid = "IC-LCI";
const char* password = "aluno.dcc!";

const char* mqtt_server = "10.4.44.53";
const int mqtt_port = 1883;
const char* mqtt_user = "MQTT_USER"; 
const char* mqtt_pass = "MQTT_PASS"; 

// Tópicos MQTT
const char* topic_estado = "ar-condicionado/estado";
const char* topic_temperatura = "ar-condicionado/temperatura";
const char* topic_sensor_temp = "sensor/temperatura";
const char* topic_sensor_umid = "sensor/umidade";

// Configuração do sensor DHT
#define DHTPIN D2 // Pino conectado ao DHT
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Configuração do emissor infravermelho
#define IR_PIN D3 // Pino do emissor IR
IRsend irsend(IR_PIN);

// Código IR cru para DESLIGAR (extraído do código 2)
uint16_t offRawData[199] = {
  4388, 4420,  526, 1628, 524, 552,  526, 1628,  526, 1630,  524, 552, 524, 554, 526, 1628,
  524, 554, 524, 552,  524, 1632,  524, 552,  526, 552,  526, 1630,  524, 1632,  524, 552,
  526, 1630,  526, 550,  524, 1632,  524, 1630,  550, 1604,  526, 1650,  504, 550,  526, 1652,
  528, 1626,  528, 1628,  504, 550,  526, 550,  526, 552,  526, 546,  556, 1606,  524, 552,
  524, 552,  524, 1652,  502, 1630,  524, 1632,  522, 554,  522, 552,  524, 552,  524, 552,
  524, 576,  502, 550,  528, 550,  550, 526,  526, 1628,  526, 1626,  528, 1628,  524, 1630,
  524, 1630,  524, 5232,  4380, 4420,  520, 1634,  520, 556,  520, 1654,  498, 1656,  498, 558,
  518, 558,  518, 1656,  496, 560,  516, 558,  518, 1656,  496, 560,  516, 558,  516, 1658,
  496, 1656,  498, 558,  516, 1658,  496, 560,  514, 1658,  494, 1660,  494, 1658,  492, 1662,
  492, 584,  492, 1660,  492, 1662,  490, 1662,  490, 586,  488, 588,  488, 586,  478, 598,
  464, 1688,  464, 612,  462, 612,  462, 1692,  460, 1718,  434, 1718,  436, 616,  460, 616,
  460, 616,  460, 616,  458, 618,  458, 616,  460, 616,  458, 618,  458, 1718,  434, 1718,
  434, 1718,  434, 1718,  434, 1718,  434
};

uint16_t onRawData[199] = {4486, 4312,  592, 1584,  570, 484,  592, 1584,  570, 1584,  570, 508,  568, 508,  568, 1584,  570, 484,  590, 508,  568, 1584,  570, 508,  568, 508,  568, 1584,  568, 1584,  570, 508,  568, 1584,  570, 508,  568, 486,  590, 1582,  572, 1582,  570, 1582,  570, 1584,  570, 1582,  570, 1582,  572, 1582,  570, 1582,  570, 484,  590, 484,  590, 486,  590, 484,  590, 486,  592, 484,  590, 484,  590, 1564,  624, 1530,  622, 1532,  620, 454,  622, 454,  624, 452,  622, 454,  622, 1532,  622, 454,  620, 454,  622, 454,  592, 1562,  624, 1528,  624, 1530,  624, 1528,  624, 5130,  4452, 4346,  592, 1562,  592, 484,  592, 1562,  592, 1562,  592, 484,  590, 488,  590, 1582,  572, 484,  590, 488,  588, 1584,  570, 508,  568, 508,  568, 1586,  566, 1588,  566, 510,  566, 1588,  564, 512,  564, 512,  564, 1590,  562, 1592,  562, 1592,  560, 1594,  560, 1594,  558, 1598,  554, 1620,  516, 1638,  530, 546,  514, 562,  530, 546,  530, 546,  530, 544,  532, 546,  530, 546,  520, 1632,  530, 1622,  530, 1624,  530, 546,  530, 546,  506, 570,  530, 546,  530, 1624,  528, 548,  528, 548,  528, 546,  516, 1638,  528, 1624,  530, 1624, 528, 1626, 530};

// WiFi e MQTT
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT...");
    if (client.connect("NodeMCUClient", mqtt_user, mqtt_pass)) {
      Serial.println("Conectado!");
      client.subscribe(topic_estado);
      client.subscribe(topic_temperatura);
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

// Função para enviar código IR
void enviarIR(String comando) {
  if (comando == "ON") {
    irsend.sendRaw(onRawData, 199, 38); 
  } else if (comando == "OFF") {
    
    // for(int i=0;i<199;i++){
    //   Serial.println(offRawData[i]);
    // }
    irsend.sendRaw(offRawData, 199, 38); 
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String messageTemp;
  for (int i = 0; i < length; i++) {
    messageTemp += (char)payload[i];
  }

  Serial.print("Mensagem recebida: ");
  Serial.println(messageTemp);

  if (String(topic) == topic_estado) {
    enviarIR(messageTemp);
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  dht.begin();
  irsend.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Publica os dados de temperatura e umidade
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  if (!isnan(temperatura) && !isnan(umidade)) {
    client.publish(topic_sensor_temp, String(temperatura).c_str(), true);
    client.publish(topic_sensor_umid, String(umidade).c_str(), true);
  }

  delay(500); 
}