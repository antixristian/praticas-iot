#include <TM1637Display.h>

// Definindo os pinos do sensor HC-SR04
const int trigPin = D4; // Pino Trig do sensor
const int echoPin = D5; // Pino Echo do sensor

// Definindo os pinos do display TM1637
const int CLK = D6;   // Pino CLK do display TM1637
const int DIO = D7;   // Pino DIO do display TM1637

// Inicializando o display
TM1637Display display(CLK, DIO);

void setup() {
  // Iniciando a comunicação serial
  Serial.begin(115200);

  // Configurando os pinos do sensor ultrassônico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Configurando o brilho do display
  display.setBrightness(0x0f);
}

void loop() {
  // Variáveis para armazenar o tempo e a distância
  long duration;
  float distance;

  // Gera um pulso no pino trig
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Calcula o tempo que o pulso levou para ir e voltar
  duration = pulseIn(echoPin, HIGH);

  // Calcula a distância em centímetros (velocidade do som = 34300 cm/s)
  distance = (duration * 0.034) / 2;

  // Exibe a distância no monitor serial
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Exibe a distância no display TM1637 (exibe até 9999 cm)
  int displayDistance = (int)distance;
  if (displayDistance > 9999) {
    displayDistance = 9999;  // Limita a exibição máxima no display
  }
  display.showNumberDec(displayDistance);

  // Pausa por 500ms antes de realizar nova medição
  delay(500);
}
