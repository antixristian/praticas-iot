#include <DHT.h>

// Definições para o sensor DHT22
#define DHTPIN D4       // Pino digital conectado ao DHT22 (ex: D4 no NodeMCU)
#define DHTTYPE DHT22   // Define o tipo do sensor (DHT22)

// Inicializa o sensor DHT
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200); // Inicializa a comunicação serial
  dht.begin();          // Inicializa o sensor DHT22
}

void loop() {
  // Aguarda o sensor estabilizar
  delay(2000);

  // Captura os valores de umidade e temperatura
  float umidade = dht.readHumidity();       // Umidade em percentual (%)
  float temperatura = dht.readTemperature(); // Temperatura em graus Celsius

  // Verifica se houve falha na leitura
  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Falha ao ler do sensor DHT22!");
    return;
  }

  // Exibe os valores na console serial
  Serial.println("===== Leitura do DHT22 =====");
  Serial.print("Umidade (%): ");
  Serial.println(umidade);
  Serial.print("Temperatura (C): ");
  Serial.println(temperatura);
}
