// C++ code
//
#define SENSOR A0

#define LED_ROJO 3
#define LED_AMARILLO 5
#define LED_AZUL 6

void setup() {
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_AMARILLO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  int humedad = analogRead(SENSOR);

  digitalWrite(LED_ROJO, 0);
  digitalWrite(LED_AMARILLO, 0);
  digitalWrite(LED_AZUL, 0);

  if (humedad < 300) {
    digitalWrite(LED_ROJO, 255);
  }
  else if (humedad < 700) {
    digitalWrite(LED_AMARILLO, 255);
  }
  else {
    digitalWrite(LED_AZUL, 255);
  }
}