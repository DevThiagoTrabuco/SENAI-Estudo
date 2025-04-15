// Bibliotecas necessárias para funcionamento do sistema
#include <Wire.h>                   // Comunicação I2C
#include <LiquidCrystal_I2C.h>      // Controle do display LCD via I2C
#include <Keypad.h>                 // Leitura de teclado matricial
#include <ESP32Servo.h>             // Controle de servo motor para ESP32
#include <Firebase_ESP_Client.h>    // Integração com Firebase
#include <WiFi.h>                   // Conexão Wi-Fi
#include "time.h"                   // Funções para data e hora via NTP

// Credenciais da rede Wi-Fi
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

// Credenciais do Firebase
#define API_KEY "AIzaSyBrnqehQ_h9MbIcYlDHK4BHbyvgztQZRqs"
#define DATABASE_URL "https://iot-senai-esp32-default-rtdb.firebaseio.com"
#define USER_EMAIL "conta.qualquer@email.com"
#define USER_PASSWORD "415263senha"

// Objetos do Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Inicializa o display LCD I2C com endereço 0x27 e 16 colunas x 2 linhas
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Mapeamento do teclado matricial 4x4
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Pinos conectados às linhas e colunas do teclado
byte rowPins[ROWS] = {15, 2, 4, 16};
byte colPins[COLS] = {17, 5, 18, 19};

// Inicializa o teclado com o mapeamento e os pinos definidos
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Definições dos pinos e objetos de hardware
const int ledVerde = 12;
const int ledVermelho = 13;
const int pinoServo = 14;
Servo servo;

// Senha correta e variável para armazenar entrada do usuário
String senhaCorreta = "1234";
String senhaDigitada = "";

// Configuração do servidor NTP para sincronizar horário
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800;         // Fuso horário GMT-3 (Brasil)
const int   daylightOffset_sec = 0;         // Sem horário de verão

void setup() {
  Serial.begin(115200);                     // Inicializa comunicação serial
  Wire.begin(21, 26);                       // Define pinos SDA e SCL para o LCD
  lcd.init();                               // Inicializa o LCD
  lcd.backlight();                          // Liga a luz de fundo do LCD

  pinMode(ledVerde, OUTPUT);                // Define LED verde como saída
  pinMode(ledVermelho, OUTPUT);             // Define LED vermelho como saída

  servo.attach(pinoServo);                  // Conecta o servo ao pino definido
  servo.write(0);                           // Garante que comece fechado
  lcd.setCursor(0, 0);
  lcd.print("Digite a senha:");            // Mensagem inicial

  // Conecta ao Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi conectado.");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);  // Configura fuso

  // Inicializa Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  char key = keypad.getKey();               // Lê tecla pressionada

  if (key) {                                // Se tecla foi pressionada
    Serial.print("Tecla: ");
    Serial.println(key);

    if (key == '#') {                       // Tecla de confirmação
      verificaSenha();
    } else if (key == '*') {               // Tecla para apagar senha
      senhaDigitada = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Senha apagada");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Digite a senha:");
    } else {                                // Tecla comum
      senhaDigitada += key;
      lcd.setCursor(0, 1);
      lcd.print(senhaDigitada);
    }
  }
}

// Função que verifica a senha digitada
void verificaSenha() {
  String resultado = (senhaDigitada == senhaCorreta) ? "Correta" : "Incorreta";

  if (resultado == "Correta") {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Senha correta!");
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);
    servo.write(90);                       // Abre porta
    delay(3000);                           // Aguarda 3 segundos
    servo.write(0);                        // Fecha porta
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

  registrarNoFirebase(resultado);         // Registra tentativa no Firebase

  senhaDigitada = "";                     // Limpa a senha
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Digite a senha:");
}

// Função que registra tentativa no Firebase com data e hora
void registrarNoFirebase(String resultado) {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {         // Tenta obter data/hora local
    Serial.println("Erro ao obter hora");
    return;
  }

  char timestamp[30];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo); // Formata horário

  String caminho = "/tentativas";        // Caminho no Firebase
  FirebaseJson json;
  json.set("timestamp", String(timestamp));
  json.set("resultado", resultado);

  // Envia os dados ao Firebase
  if (Firebase.RTDB.pushJSON(&fbdo, caminho.c_str(), &json)) {
    Serial.println("Tentativa registrada com sucesso.");
  } else {
    Serial.println("Erro ao registrar tentativa: " + fbdo.errorReason());
  }
}
