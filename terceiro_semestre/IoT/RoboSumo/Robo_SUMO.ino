#include <BluetoothSerial.h>

#define pinMotorA1 33
#define pinMotorA2 32
#define pinMotorB1 25
#define pinMotorB2 26

BluetoothSerial BT;

// Variáveis para controle de tempo sem delay
unsigned long tempoAnterior = 0;
unsigned long tempoDelay = 500; // Defina o tempo de atraso desejado em milissegundos
bool motorEmMovimento = false;

void setup() {
  Serial.begin(115200);
  if(!BT.begin("ESP32-BarrilDobrado")) {
    Serial.println("Erro ao iniciar Bluetooth!");
    while(1) {
      delay(1000); // Erro de Bluetooth - mantém no loop
    }
  }
  Serial.println("Bluetooth iniciado.");

  pinMode(pinMotorA1, OUTPUT);
  pinMode(pinMotorA2, OUTPUT);
  pinMode(pinMotorB1, OUTPUT);
  pinMode(pinMotorB2, OUTPUT);
}

void loop() {
  
  if (BT.available()) {
    char leitura = BT.read();
    Serial.println(leitura);

    switch (leitura) {
      case '1': frente(); break;
      case '2': atras(); break;
      case '3': esquerda(); break;
      case '4': direita(); break;
      default:
        Serial.println("Comando desconhecido.");
        break;
    }
  }

  // Verificar se o motor está em movimento e aguardar o tempo de delay
  if (motorEmMovimento && (millis() - tempoAnterior >= tempoDelay)) {
    stop();  // Parar o motor após o tempo definido
    motorEmMovimento = false;  // Resetar a flag de movimento
  }
}

void stop() {
  digitalWrite(pinMotorA1, LOW);
  digitalWrite(pinMotorA2, LOW);
  digitalWrite(pinMotorB1, LOW);
  digitalWrite(pinMotorB2, LOW);
  Serial.println("parado");
}

void frente() {
  Serial.println("frente");
  digitalWrite(pinMotorA1, HIGH);
  digitalWrite(pinMotorA2, LOW);
  digitalWrite(pinMotorB1, LOW);
  digitalWrite(pinMotorB2, HIGH);
  
  // Armazenar o tempo em que o movimento começou
  tempoAnterior = millis();
  motorEmMovimento = true;
}

void atras() {
  Serial.println("atras");
  digitalWrite(pinMotorA1, LOW);
  digitalWrite(pinMotorA2, HIGH);
  digitalWrite(pinMotorB1, HIGH);
  digitalWrite(pinMotorB2, LOW);
  
  // Armazenar o tempo em que o movimento começou
  tempoAnterior = millis();
  motorEmMovimento = true;
}

void esquerda() {
  Serial.println("direita");
  digitalWrite(pinMotorA1, LOW);
  digitalWrite(pinMotorA2, HIGH);
  digitalWrite(pinMotorB1, LOW);
  digitalWrite(pinMotorB2, HIGH);
  // Armazenar o tempo em que o movimento começou
  tempoAnterior = millis();
  motorEmMovimento = true;
}

void direita() {
  
  Serial.println("esquerda");
  digitalWrite(pinMotorA1, HIGH);
  digitalWrite(pinMotorA2, LOW);
  digitalWrite(pinMotorB1, HIGH);
  digitalWrite(pinMotorB2, LOW);
  
  // Armazenar o tempo em que o movimento começou
  tempoAnterior = millis();
  motorEmMovimento = true;
}