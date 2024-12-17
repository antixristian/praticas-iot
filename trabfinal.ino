#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <Arduino.h>
#include "DHT.h"

#define DHTPIN D2    // Pino do sensor DHT (altere se necessário)
#define DHTTYPE DHT11   // Tipo do sensor (DHT11 ou DHT22)
#define SEND_PIN D8 //Pino de envio do IR

DHT dht(DHTPIN, DHTTYPE);

// Variaveis Globais
int currentTemp = 22;
bool currentStatus = false;

/****** WiFi Connection Details *******/
const char* ssid = "IC-LCI";
const char* password = "aluno.dcc!";

/******* MQTT Broker Connection Details *******/
const char* mqtt_server = "10.4.44.19";

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP280 bme; // I2C

unsigned long delayTime;

IRsend irsend(SEND_PIN);

// Códigos IR (você precisa coletar os seus e preencher aqui)
//Exemplo:
// Comando para temperatura 17
uint16_t ir_temp_17[199] = {4400, 4400,  500, 1650, 500, 550,  500, 1650,  500, 1650,  500, 550, 500, 550, 500, 1650, 500, 550, 500, 550,  500, 1650,  500, 550,  500, 550,  500, 1650,  500, 1650,  500, 550,  500, 1650,  500, 550,  500, 1650,  500, 1650,  500, 1650,  500, 1650,  500, 550,  500, 1650,  500, 1650,  500, 1650,  500, 550,  500, 550,  500, 550,  500, 550,  500, 1650,  500, 550,  500, 550,  500, 1650,  500, 1650,  500, 1650,  500, 550,  500, 550,  500, 550,  500, 550,  500, 550,  500, 550,  500, 550,  500, 1650,  500, 1650,  500, 1650,  500, 1650,  500, 1650,  500, 5000,  500, 500,  500, 1650,  500, 550,  500, 1650,  500, 1650,  500, 550, 500, 550, 500, 1650, 500, 550, 500, 550, 500, 1650, 500, 550, 500, 550, 500, 1650, 500, 1650, 500, 550, 500, 1650, 500, 550, 500, 1650, 500, 1650, 500, 1650, 500, 1650, 500, 550, 500, 1650, 500, 1650, 500, 1650, 500, 550, 500, 550, 500, 550, 500, 550, 500, 1650, 500, 550, 500, 550, 500, 1650, 500, 1650, 500, 1650, 500, 550, 500, 550, 500, 550, 500, 550, 500, 550, 500, 550, 500, 550, 500, 1650, 500, 1650, 500, 1650, 500, 1650, 500, 1650, 500};
// Comando para temperatura 18
uint16_t ir_temp_18[199] = {4300, 4300,  550, 1600, 550, 500,  550, 1600,  550, 1600,  550, 500, 550, 500, 550, 1600, 550, 500, 550, 500,  550, 1600,  550, 500,  550, 500,  550, 1600,  550, 1600,  550, 500,  550, 1600,  550, 500,  550, 1600,  550, 1600,  550, 1600,  550, 1600,  550, 500,  550, 1600,  550, 1600,  550, 1600,  550, 500,  550, 500,  550, 500,  550, 500,  550, 1600,  550, 500,  550, 500,  550, 1600,  550, 1600,  550, 1600,  550, 500,  550, 500,  550, 500,  550, 500,  550, 500,  550, 500,  550, 500,  550, 1600,  550, 1600,  550, 1600,  550, 1600,  550, 1600,  550, 5000,  550, 500,  550, 1600,  550, 500,  550, 1600,  550, 1600,  550, 500, 550, 500, 550, 1600, 550, 500, 550, 500, 550, 1600, 550, 500, 550, 500, 550, 1600, 550, 1600, 550, 500, 550, 1600, 550, 500, 550, 1600, 550, 1600, 550, 1600, 550, 1600, 550, 500, 550, 1600, 550, 1600, 550, 1600, 550, 500, 550, 500, 550, 500, 550, 500, 550, 1600, 550, 500, 550, 500, 550, 1600, 550, 1600, 550, 1600, 550, 500, 550, 500, 550, 500, 550, 500, 550, 500, 550, 500, 550, 500, 550, 1600, 550, 1600, 550, 1600, 550, 1600, 550, 1600, 550};
// Comando para OFF
uint16_t ir_off[199] = {4388, 4420,  526, 1628, 524, 552,  526, 1628,  526, 1630,  524, 552, 524, 554, 526, 1628, 524, 554, 524, 552,  524, 1632,  524, 552,  526, 552,  526, 1630,  524, 1632,  524, 552,  526, 1630,  526, 550,  524, 1632,  524, 1630,  550, 1604,  526, 1650,  504, 550,  526, 1652,  528, 1626,  528, 1628,  504, 550,  526, 550,  526, 552,  526, 546,  556, 1606,  524, 552,  524, 552,  524, 1652,  502, 1630,  524, 1632,  522, 554,  522, 552,  524, 552,  524, 552,  524, 576,  502, 550,  528, 550,  550, 526,  526, 1628,  526, 1626,  528, 1628,  524, 1630,  524, 1630,  524, 5232,  4380, 4420,  520, 1634,  520, 556,  520, 1654,  498, 1656,  498, 558,  518, 558,  518, 1656,  496, 560,  516, 558,  518, 1656,  496, 560,  516, 558,  516, 1658,  496, 1656,  498, 558,  516, 1658,  496, 560,  514, 1658,  494, 1660,  494, 1658,  492, 1662,  492, 584,  492, 1660,  492, 1662,  490, 1662,  490, 586,  488, 588,  488, 586,  478, 598,  464, 1688,  464, 612,  462, 612,  462, 1692,  460, 1718,  434, 1718,  436, 616,  460, 616,  460, 616,  460, 616,  458, 618,  458, 616,  460, 616,  458, 618,  458, 1718,  434, 1718,  434, 1718,  434, 1718,  434, 1718,  434};
// Comando para ON
uint16_t ir_on[199] = {4388, 4420,  526, 1628, 524, 552,  526, 1628,  526, 1630,  524, 552, 524, 554, 526, 1628, 524, 554, 524, 552,  524, 1632,  524, 552,  526, 552,  526, 1630,  524, 1632,  524, 552,  526, 1630,  526, 550,  524, 1632,  524, 1630,  550, 1604,  526, 1650,  504, 550,  526, 1652,  528, 1626,  528, 1628,  504, 550,  526, 550,  526, 552,  526, 546,  556, 1606,  524, 552,  524, 552,  524, 1652,  502, 1630,  524, 1632,  522, 554,  522, 552,  524, 552,  524, 552,  524, 576,  502, 550,  528, 550,  550, 526,  526, 1628,  526, 1626,  528, 1628,  524, 1630,  524, 1630,  524, 5232,  4380, 4420,  520, 1634,  520, 556,  520, 1654,  498, 1656,  498, 558,  518, 558,  518, 1656,  496, 560,  516, 558,  518, 1656,  496, 560,  516, 558,  516, 1658,  496, 1656,  498, 558,  516, 1658,  496, 560,  514, 1658,  494, 1660,  494, 1658,  492, 1662,  492, 584,  492, 1660,  492, 1662,  490, 1662,  490, 586,  488, 588,  488, 586,  478, 598,  464, 1688,  464, 612,  462, 612,  462, 1692,  460, 1718,  434, 1718,  436, 616,  460, 616,  460, 616,  460, 616,  458, 618,  458, 616,  460, 616,  458, 618,  458, 1718,  434, 1718,  434, 1718,  434, 1718,  434, 1718,  434};

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void sendIRCommand(bool status, int temp) {
    if(status){
        irsend.sendRaw(ir_on, 199, 38);
    } else {
      irsend.sendRaw(ir_off, 199, 38);
    }

    if(temp == 17){
        irsend.sendRaw(ir_temp_17, 199, 38);
    } else if(temp == 18){
        irsend.sendRaw(ir_temp_18, 199, 38);
    } 
    //Adicione o resto das temperaturas se necessário.

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
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Mensagem recebida no topico: ");
    Serial.println(topic);

    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    if (strcmp(topic, "ar-condicionado/estado") == 0) {
      if(message == "ON"){
          currentStatus = true;
        
      } else if (message == "OFF"){
          currentStatus = false;
      }
    }else if (strcmp(topic, "ar-condicionado/temperatura") == 0) {
      
      int temp = message.toInt();
       if (temp >= 17 && temp <= 25) {
           currentTemp = temp;
       }
    }
    sendIRCommand(currentStatus, currentTemp);
    
    Serial.print("Estado: ");
    Serial.println(currentStatus);
    Serial.print("Temperatura: ");
    Serial.println(currentTemp);
}

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

void setup() {
  Serial.begin(9600);
  Serial.println(F("BME280 test"));
  bool status;
  status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
  dht.begin();

  Serial.println("-- Default Test --");
  delayTime = 1000;

  setup_wifi();

  pinMode(BUILTIN_LED, OUTPUT);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  irsend.begin();

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  float h = dht.readHumidity();
  float t = bme.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    
    dtostrf(t, 2, 2, msg);
    client.publish("sensor/temperatura", msg);
    
    dtostrf(h, 2, 2, msg);
    client.publish("sensor/umidade", msg);
    
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.print(" *C, Umidade: ");
    Serial.print(h);
    Serial.println(" %");
  }

  delay(2000);
}