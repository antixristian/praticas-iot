// Definindo os pinos do sensor HC-SR04
const int trigPin = D1; // Pino Trig do sensor conectado ao D1 no NodeMCU
const int echoPin = D2; // Pino Echo do sensor conectado ao D2 no NodeMCU

void setup() {
  // Iniciando a comunicação serial
  Serial.begin(115200);
  
  // Configurando os pinos do sensor ultrassônico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
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
  
  // Pausa por 500ms antes de realizar nova medição
  delay(500);
}