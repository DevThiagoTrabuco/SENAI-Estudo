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

bool controleManualLED = false;
bool lcdLigado = true;

void connectWiFi() {
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

  connectWiFi();
  initFirebase();

  digitalWrite(releLCD, LOW);
  delay(1000);
  lcd.begin(16, 2);
}

void loop() {
  atualizarModo();

  int movimento = digitalRead(sensorPin);

  if (!controleManualLED) {
    if (movimento == HIGH) {
      digitalWrite(pinoLed, HIGH);
      lcd.clear();
      lcd.print("Movimento!");
      enviarEstado("luz", true);
      Serial.println("Movimento detectado!");
    } else {
      digitalWrite(pinoLed, LOW);
      lcd.clear();
      lcd.print("Sem movimento");
      enviarEstado("luz", false);
      Serial.println("Nada detectado");
    }
  } else {
    lcd.clear();
    lcd.print("Sensor desligado.");
  }

  // Comandos remotos do Firebase
  if (lerComando("comando_ligar_luz")) {
    ligarLed();
    enviarEstado("comando_ligar_luz", false);
  }
  if (lerComando("comando_desligar_luz")) {
    desligarLed();
    enviarEstado("comando_desligar_luz", false);
  }
  if (lerComando("comando_ligar_lcd")) {
    ligarLCD();
    enviarEstado("comando_ligar_lcd", false);
  }
  if (lerComando("comando_desligar_lcd")) {
    desligarLCD();
    enviarEstado("comando_desligar_lcd", false);
  }

  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando == "ligar tela") {
      ligarLCD();
    } else if (comando == "desligar tela") {
      desligarLCD();
    } else if (comando == "ligar luz") {
      ligarLed();
    } else if (comando == "desligar luz") {
      desligarLed();
    } else if (comando == "ligar tudo") {
      ligarLCD();
      ligarLed();
    } else if (comando == "desligar tudo") {
      desligarLCD();
      desligarLed();
    } else if (comando == "auto") {
      controleManual();
    } else {
      Serial.println("Comando inválido.");
    }
  }

  delay(500);
}

void ligarLCD() {
  digitalWrite(releLCD, LOW);
  delay(500);
  lcd.begin(16, 2);
  lcdLigado = true;
  enviarEstado("lcd", true);
  Serial.println("LCD ligado.");
}

void desligarLCD() {
  lcd.clear();
  digitalWrite(releLCD, HIGH);
  lcdLigado = false;
  enviarEstado("lcd", false);
  Serial.println("LCD desligado.");
}

void ligarLed() {
  digitalWrite(pinoLed, HIGH);
  controleManualLED = true;
  enviarEstado("luz", true);
  Serial.println("Luz ligada.");
}

void desligarLed() {
  digitalWrite(pinoLed, LOW);
  controleManualLED = true;
  enviarEstado("luz", false);
  Serial.println("Luz desligada.");
}

void controleManual() {
  controleManualLED = !controleManualLED;
  if (digitalRead(pinoLed) == HIGH) {
    delay(1000);
    digitalWrite(pinoLed, LOW);
    enviarEstado("luz", false);
  }
  Serial.print("Sensor ");
  Serial.println(controleManualLED ? "desligado" : "ligado");
}
