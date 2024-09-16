#define MODO CHANGE;
const byte interruptPin = D1;
int pin = D2;
volatile int state = LOW;

void ICACHE_RAM_ATTR blink() {
    state = !state;
    digitalWrite(pin, state);
    delay(100);
}

void setup() {
    pinMode(pin, OUTPUT);
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), blink, MODO);
}

void loop() {}