#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <LiquidCrystal.h>

// LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(26, 25, 33, 32, 27, 14);

// Wi-Fi
#define WIFI_SSID "A21s"
#define WIFI_PASSWORD "12345678"

// Firebase
#define API_KEY "AIzaSyBrnqehQ_h9MbIcYlDHK4BHbyvgztQZRqs"
#define DATABASE_URL "https://iot-senai-esp32-default-rtdb.firebaseio.com"
#define USER_EMAIL "conta.qualquer@email.com"
#define USER_PASSWORD "415263senha"

// Firebase objetos
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Dispositivo
const String dispositivo = "ESP32-01";

// Pinos
const int pinoLed = 23;
const int sensorPin = 16;
const int releLCD = 22;
const int ledSecundarioPin = 21;

bool controleManualLED = false;
bool lcdLigado = false;

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConectado ao Wi-Fi");
}

void initFirebase() {
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void enviarEstado(String campo, bool valor) {
  String path = "testeCasa/" + dispositivo + "/" + campo;
  Firebase.RTDB.setBool(&fbdo, path.c_str(), valor);
}

bool lerComando(String campo) {
  String path = "testeCasa/" + dispositivo + "/" + campo;
  if (Firebase.RTDB.getBool(&fbdo, path.c_str())) {
    return fbdo.boolData();
  } else {
    Serial.println("Erro ao ler comando: " + fbdo.errorReason());
    return false;
  }
}

void atualizarModo() {
  String path = "testeCasa/" + dispositivo + "/modo";
  if (Firebase.RTDB.getString(&fbdo, path.c_str())) {
    String modo = fbdo.stringData();
    if (modo == "auto" && controleManualLED) {
      controleManualLED = false;
      Serial.println("Modo automático ativado.");
    } else if (modo == "manual" && !controleManualLED) {
      controleManualLED = true;
      Serial.println("Modo manual ativado.");
    }
  } else {
    Serial.println("Erro ao ler modo: " + fbdo.errorReason());
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(pinoLed, OUTPUT);
  pinMode(sensorPin, INPUT);
  pinMode(releLCD, OUTPUT);
  pinMode(ledSecundarioPin, OUTPUT);

  connectWiFi();
  initFirebase();

  // Verifica se o relé está ativando o VCC da tela
  digitalWrite(releLCD, LOW);  // Liga LCD (conecta VCC)
  delay(500);
  lcd.begin(16, 2);
  lcd.print("Iniciando...");
  lcdLigado = true;
  enviarEstado("lcd", true);
}

void loop() {
  atualizarModo();

  int movimento = digitalRead(sensorPin);

  if (!controleManualLED) {
    if (movimento == HIGH) {
      digitalWrite(pinoLed, HIGH);
      if (lcdLigado) {
        lcd.clear();
        lcd.print("Movimento!");
      }
      enviarEstado("led1", true);
      Serial.println("Movimento detectado!");
    } else {
      digitalWrite(pinoLed, LOW);
      if (lcdLigado) {
        lcd.clear();
        lcd.print("Sem movimento");
      }
      enviarEstado("led1", false);
      Serial.println("Nada detectado");
    }
  } else {
    if (lcdLigado) {
      lcd.clear();
      lcd.print("Sensor desligado.");
    }
  }

  // Comandos remotos
  if (lerComando("comando_ligar_led1")) {
    ligarLed();
    enviarEstado("comando_ligar_led1", false);
  }
  if (lerComando("comando_desligar_led1")) {
    desligarLed();
    enviarEstado("comando_desligar_led1", false);
  }
  if (lerComando("comando_ligar_lcd")) {
    ligarLCD();
    enviarEstado("comando_ligar_lcd", false);
  }
  if (lerComando("comando_desligar_lcd")) {
    desligarLCD();
    enviarEstado("comando_desligar_lcd", false);
  }
  if (lerComando("comando_ligar_led2")) {
    ligarLed2();
    enviarEstado("comando_ligar_led2", false);
  }
  if (lerComando("comando_desligar_led2")) {
    desligarLed2();
    enviarEstado("comando_desligar_led2", false);
  }

  // Comandos seriais
  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando == "ligar tela") {
      ligarLCD();
    } else if (comando == "desligar tela") {
      desligarLCD();
    } else if (comando == "ligar led1") {
      ligarLed();
    } else if (comando == "desligar led1") {
      desligarLed();
    } else if (comando == "ligar tudo") {
      ligarLCD();
      ligarLed();
    } else if (comando == "desligar tudo") {
      desligarLCD();
      desligarLed();
    } else if (comando == "ligar led2") {
      ligarLed2();
    } else if (comando == "desligar led2") {
      desligarLed2();
    } else if (comando == "auto") {
      controleManual();
    } else {
      Serial.println("Comando inválido.");
    }
  }

  delay(500);
}

// Funções auxiliares
void ligarLCD() {
  digitalWrite(releLCD, LOW);  // Alimenta VCC do LCD
  delay(500);
  lcd.begin(16, 2);
  lcdLigado = true;
  lcd.print("LCD ligado");
  enviarEstado("lcd", true);
  Serial.println("LCD ligado.");
}

void desligarLCD() {
  if (lcdLigado) {
    lcd.clear();
    lcdLigado = false;
  }
  digitalWrite(releLCD, HIGH);  // Corta alimentação do LCD
  enviarEstado("lcd", false);
  Serial.println("LCD desligado.");
}

void ligarLed() {
  digitalWrite(pinoLed, HIGH);
  controleManualLED = true;
  enviarEstado("led1", true);
  Serial.println("LED 1 ligado.");
}

void desligarLed() {
  digitalWrite(pinoLed, LOW);
  controleManualLED = true;
  enviarEstado("led1", false);
  Serial.println("LED 1 desligado.");
}

void ligarLed2() {
  digitalWrite(ledSecundarioPin, HIGH);
  enviarEstado("led2", true);
  Serial.println("LED 2 ligado.");
}

void desligarLed2() {
  digitalWrite(ledSecundarioPin, LOW);
  enviarEstado("led2", false);
  Serial.println("LED 2 desligado.");
}

void controleManual() {
  controleManualLED = !controleManualLED;
  if (digitalRead(pinoLed) == HIGH) {
    delay(1000);
    digitalWrite(pinoLed, LOW);
    enviarEstado("led1", false);
  }
  Serial.print("Sensor ");
  Serial.println(controleManualLED ? "desligado" : "ligado");
}