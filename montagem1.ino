// Interface I2C //
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h> // Biblioteca para o BMP280

// Definindo o endereço padrão do BMP280 (I2C)
Adafruit_BMP280 bmp; // Inicializa o sensor BMP280 para interface I2C

void setup() {
  Serial.begin(115200); // Inicializa a comunicação serial

  // Inicializa o BMP280
  if (!bmp.begin(0x76)) { // Endereço I2C padrão do BMP280 (pode ser 0x76 ou 0x77)
    Serial.println("Erro ao inicializar o BMP280!");
    while (1); // Para a execução se houver erro
  }

  // Configurações do BMP280
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,   // Modo normal
                  Adafruit_BMP280::SAMPLING_X2,   // Oversampling de temperatura
                  Adafruit_BMP280::SAMPLING_X16,  // Oversampling de pressão
                  Adafruit_BMP280::FILTER_X16,    // Filtro
                  Adafruit_BMP280::STANDBY_MS_500); // Tempo de standby
}

void loop() {
  // Captura os dados do BMP280
  float temperatura = bmp.readTemperature();           // Temperatura em graus Celsius
  float pressao = bmp.readPressure() / 100.0F;         // Pressão em hPa (hectopascais)
  float altitude = bmp.readAltitude(1013.25);          // Altitude em metros (pressão ao nível do mar padrão)

  // Exibe os valores na console
  Serial.println("===== Leitura do BMP280 =====");
  Serial.print("Temperatura (C): ");
  Serial.println(temperatura);
  Serial.print("Pressao (hPa): ");
  Serial.println(pressao);
  Serial.print("Altitude (m): ");
  Serial.println(altitude);
  
  delay(2000); // Aguarda 2 segundos antes da próxima leitura
}


// Interface SPI //
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h> // Biblioteca para o BMP280

// Define os pinos SPI
#define BMP_SCK  D5  // GPIO 14
#define BMP_MISO D6  // GPIO 12
#define BMP_MOSI D7  // GPIO 13
#define BMP_CS   D8  // GPIO 15

// Inicializa o sensor BMP280 para interface SPI
Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);

void setup() {
  Serial.begin(115200); // Inicializa a comunicação serial

  // Inicializa o BMP280
  if (!bmp.begin()) { 
    Serial.println("Erro ao inicializar o BMP280!");
    while (1); // Para a execução se houver erro
  }

  // Configurações do BMP280
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,   // Modo normal
                  Adafruit_BMP280::SAMPLING_X2,   // Oversampling de temperatura
                  Adafruit_BMP280::SAMPLING_X16,  // Oversampling de pressão
                  Adafruit_BMP280::FILTER_X16,    // Filtro
                  Adafruit_BMP280::STANDBY_MS_500); // Tempo de standby
}

void loop() {
  // Captura os dados do BMP280
  float temperatura = bmp.readTemperature();           // Temperatura em graus Celsius
  float pressao = bmp.readPressure() / 100.0F;         // Pressão em hPa (hectopascais)
  float altitude = bmp.readAltitude(1013.25);          // Altitude em metros (pressão ao nível do mar padrão)

  // Exibe os valores na console
  Serial.println("===== Leitura do BMP280 =====");
  Serial.print("Temperatura (C): ");
  Serial.println(temperatura);
  Serial.print("Pressao (hPa): ");
  Serial.println(pressao);
  Serial.print("Altitude (m): ");
  Serial.println(altitude);
  
  delay(2000); // Aguarda 2 segundos antes da próxima leitura
}
