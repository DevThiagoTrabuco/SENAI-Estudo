#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <ESP32Servo.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include "time.h"

// Firebase
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define API_KEY "AIzaSyBrnqehQ_h9MbIcYlDHK4BHbyvgztQZRqs"
#define DATABASE_URL "https://iot-senai-esp32-default-rtdb.firebaseio.com"
#define USER_EMAIL "conta.qualquer@email.com"
#define USER_PASSWORD "415263senha"
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Tela LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {15, 2, 4, 16};
byte colPins[COLS] = {17, 5, 18, 19};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Leds
const int ledVerde = 12;
const int ledVermelho = 13;

// Servo
const int pinoServo = 14;
Servo servo;

// NTP
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800;
const int   daylightOffset_sec = 0;

// Variáveis
String senhaDigitada = "";
bool modoCadastro = false;

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 26);
  lcd.init();
  lcd.backlight();

  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  servo.attach(pinoServo);
  servo.write(0);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    lcd.setCursor(0, 0);
    lcd.print("Conectando");
    lcd.setCursor(0, 1);
    lcd.print("ao WiFi...");
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi conectado.");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A - Criar senha");
  lcd.setCursor(0, 1);
  lcd.print("B - Checar senha");
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    processarTecla(key);
  }
}

void processarTecla(char key) {
  switch (key) {
    case 'A':
      iniciarCadastro();
      break;
    case 'B':
      iniciarLogin();
      break;
    case '*':
      limparSenha();
      break;
    case '#':
      confirmarSenha();
      break;
    default:
      adicionarDigito(key);
      break;
  }
}

void iniciarCadastro() {
  senhaDigitada = "";
  modoCadastro = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Nova senha:");
}

void iniciarLogin() {
  senhaDigitada = "";
  modoCadastro = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Digite a senha:");
}

void limparSenha() {
  senhaDigitada = "";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Senha apagada");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(modoCadastro ? "Nova senha:" : "Digite a senha:");
}

void confirmarSenha() {
  if (modoCadastro) {
    cadastrarNovaSenha();
  } else {
    verificaSenhaFirebase();
  }
}

void adicionarDigito(char key) {
  senhaDigitada += key;
  lcd.setCursor(0, 1);
  lcd.print(senhaDigitada);
}

void cadastrarNovaSenha() {
  if (Firebase.RTDB.setString(&fbdo, "/senha", senhaDigitada)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Senha salva!");
    delay(2000);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erro ao salvar");
    delay(2000);
  }
  iniciarLogin();
}

void verificaSenhaFirebase() {
  if (!Firebase.RTDB.getString(&fbdo, "/senha")) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erro ao buscar");
    delay(2000);
    return;
  }

  String senhaSalva = fbdo.stringData();
  String resultado = (senhaDigitada == senhaSalva) ? "Correta" : "Incorreta";

  if (resultado == "Correta") {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Senha correta!");
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);
    servo.write(90);
    delay(3000);
    servo.write(0);
    digitalWrite(ledVerde, LOW);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Senha errada!");
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledVerde, LOW);
    delay(2000);
    digitalWrite(ledVermelho, LOW);
  }

  registrarNoFirebase(resultado);
  iniciarLogin();
}

void registrarNoFirebase(String resultado) {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Erro ao obter hora");
    return;
  }

  char timestamp[30];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);

  String caminho = "/tentativas";
  FirebaseJson json;
  json.set("timestamp", String(timestamp));
  json.set("resultado", resultado);

  if (Firebase.RTDB.pushJSON(&fbdo, caminho.c_str(), &json)) {
    Serial.println("Tentativa registrada com sucesso.");
  } else {
    Serial.println("Erro ao registrar tentativa: " + fbdo.errorReason());
  }
}