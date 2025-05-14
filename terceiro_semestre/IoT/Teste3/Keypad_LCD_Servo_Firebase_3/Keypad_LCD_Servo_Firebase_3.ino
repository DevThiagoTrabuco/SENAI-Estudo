#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <TFT_eSPI.h>
#include <Firebase_ESP_Client.h>
#include <SPI.h>       // Biblioteca para comunicação SPI
#include <Wire.h>      // Biblioteca para comunicação I2C
#include <Adafruit_FT6206.h> // Biblioteca para o controlador de touchscreen FT6206
#include <Adafruit_ILI9341.h>

Adafruit_FT6206 ctp = Adafruit_FT6206(); // Inicializa o controlador de touchscreen

// Firebase e WIFI
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define API_KEY "AIzaSyBrnqehQ_h9MbIcYlDHK4BHbyvgztQZRqs"
#define DATABASE_URL "https://iot-senai-esp32-default-rtdb.firebaseio.com"
#define USER_EMAIL "conta.qualquer@email.com"
#define USER_PASSWORD "415263senha"
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Definições de pinos e cores
#define GREEN_LED 5
#define RED_LED 13
#define TFT_CS 15
#define TFT_DC 2
#define TFT_MOSI 23
#define TFT_SCLK 18
#define BLACK 0x0000
#define BLUE 0x2015
#define RED 0xF800
#define GREEN 0x07E0
#define WHITE 0xFFFF
#define GRAY 0x31A6

//TFT_eSPI tft = TFT_eSPI(); // Inicializa o display TFT
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// Posições e teclas do teclado virtual
int vPositions[4] = {127, 180, 233, 283};
int hPositions[4] = {22, 83, 143, 204};
String keys[4][4] = {{"7", "8", "9", "A"}, 
                    {"4", "5", "6", "B"}, 
                    {"1", "2", "3", "C"}, 
                    {"<", "0", ">", "D"}};

// Variáveis para login e senha
String valorLogin = "";
String valorSenha = "";
boolean operado = false;
int op = 1;
boolean verificado;

void setup() {
  pinMode(GREEN_LED, OUTPUT); // Configura LED verde como saída
  pinMode(RED_LED, OUTPUT);   // Configura LED vermelho como saída
  Serial.begin(115200);       // Inicializa a comunicação serial
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 6); // Conecta ao WiFi
  tft.begin();                // Inicializa o display TFT
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Inicializa o touchscreen
  if (!ctp.begin(40)) {
    Serial.println("Couldn't start FT6206 touchscreen controller");
    while (1);
  }
  tft.setRotation(0);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    tft.print(".");
  }
  tft.print("\nOK! IP=");
  tft.println(WiFi.localIP());
  desenhaButtons(); // Desenha os botões na tela
  desenhaBox();     // Desenha o layout da tela
  mostraEntradas(); // Mostra os campos de login e senha

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (ctp.touched()) { // Verifica se a tela foi tocada
    TS_Point p = ctp.getPoint();
    p.x = map(p.x, 0, 240, 240, 0);
    p.y = map(p.y, 0, 320, 320, 0);
    selectedButton(p.x, p.y); // Identifica o botão pressionado
  }

  delay(200);
}

// Desenha o layout da tela
void desenhaBox() {
  tft.drawFastHLine(0, 1, 240, WHITE);
  tft.drawFastVLine(0, 0, 320, WHITE);
  tft.drawFastVLine(238, 0, 320, WHITE);
  tft.drawFastHLine(0, 319, 240, WHITE);
  for (int y = 112; y <= 268; y += 52) {
    tft.drawFastHLine(0, y, 240, WHITE);
    for (int x = 60; x <= 180; x += 60) {
      tft.drawFastVLine(x, y, 52, WHITE);
    }
  }
}

// Mostra os campos de login e senha
void mostraEntradas() {
  tft.setCursor(10, 15);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.print("LOGIN:");
  tft.setCursor(10, 65);
  tft.print("SENHA:");
}

// Identifica o botão pressionado
void selectedButton(int x, int y) {
  if (y >= 112 && y < 164) {
    if (x >= 0 && x < 60) adicionaDigito('7');
    else if (x >= 60 && x < 120) adicionaDigito('8');
    else if (x >= 120 && x < 180) adicionaDigito('9');
    else adicionaDigito('A');
  }
  if (y >= 164 && y < 216) {
    if (x >= 0 && x < 60) adicionaDigito('4');
    else if (x >= 60 && x < 120) adicionaDigito('5');
    else if (x >= 120 && x < 180) adicionaDigito('6');
    else adicionaDigito('B');
  }
  if (y >= 216 && y < 268) {
    if (x >= 0 && x < 60) adicionaDigito('1');
    else if (x >= 60 && x < 120) adicionaDigito('2');
    else if (x >= 120 && x < 180) adicionaDigito('3');
    else adicionaDigito('C');
  }
  if (y >= 268 && y < 320) {
    if (x >= 0 && x < 60) removeCaracter();
    else if (x >= 60 && x < 120) adicionaDigito('0');
    else if (x >= 120 && x < 180) adicionaEntrada();
    else adicionaDigito('D');
  }
}

// Desenha os botões na tela
void desenhaButtons() {
  tft.fillRect(0, 0, 240, 112, GRAY);
  for (int y = 112; y <= 268; y += 52) {
    tft.fillRect(60, y, 60, 52, BLACK);
    tft.fillRect(180, y, 60, 52, RED);
    if (y != 268) {
      tft.fillRect(0, y, 60, 52, BLACK);
      tft.fillRect(120, y, 60, 52, BLACK);
    } else {
      tft.fillRect(0, y, 60, 52, BLUE);
      tft.fillRect(120, y, 60, 52, GREEN);
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tft.setCursor(hPositions[j], vPositions[i]);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print(keys[i][j]);
    }
  }
}

// Adiciona um dígito ao campo atual
void adicionaDigito(char digito) {
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  if (op == 1) {
    valorLogin += digito;
    tft.setCursor(10, 40);
    tft.println(valorLogin);
  } else if (op == 2) {
    valorSenha += digito;
    tft.setCursor(10, 80);
    tft.println(valorSenha);
  }
}

// Remove o último caractere do campo atual
void removeCaracter() {
  if (op == 1 && valorLogin != "") {
    valorLogin = valorLogin.substring(0, valorLogin.length() - 1);
    tft.fillRect(10, 40, 240, 25, GRAY);
    tft.setCursor(10, 40);
    tft.println(valorLogin);
  } else if (op == 2 && valorSenha != "") {
    valorSenha = valorSenha.substring(0, valorSenha.length() - 1);
    tft.fillRect(10, 80, 240, 25, GRAY);
    tft.setCursor(10, 80);
    tft.println(valorSenha);
  }
}

// Limpa a tela e reinicia as variáveis
void limpaTela() {
  tft.fillRect(0, 0, 240, 112, GRAY);
  mostraEntradas();
  valorLogin = "";
  valorSenha = "";
  op = 1;
}

// Verifica o usuário e senha no Firebase
void verificaUsuario() {
  if (valorLogin == "" || valorSenha == "") {
    tft.setCursor(10, 100);
    tft.setTextColor(RED);
    tft.println("Campos vazios!");
    delay(2000);
    limpaTela();
    return;
  }

  String path = "/login/usuarios";
  if (Firebase.RTDB.getJSON(&fbdo, path)) {
    FirebaseJson &json = fbdo.jsonObject();
    FirebaseJsonData jsonData;

    // Percorre todos os usuários
    size_t len = json.iteratorBegin();
    bool usuarioEncontrado = false;
    String senhaFirebase = "";

    for (size_t i = 0; i < len; i++) {
      String key, value;
      int type;
      json.iteratorGet(i, type, key, value);

      // Verifica se o nome do usuário bate
      if (key == valorLogin) {
        FirebaseJson userJson;
        userJson.setJsonData(value);
        userJson.get(jsonData, "password");  // Busca a senha associada
        senhaFirebase = jsonData.stringValue;
        usuarioEncontrado = true;
        break;
      }
    }
    json.iteratorEnd();

    if (usuarioEncontrado) {
      if (senhaFirebase == valorSenha) {
        tft.setCursor(10, 100);
        tft.setTextColor(GREEN);
        tft.println("Acesso permitido!");
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(RED_LED, LOW);
      } else {
        tft.setCursor(10, 100);
        tft.setTextColor(RED);
        tft.println("Senha incorreta!");
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
      }
    } else {
      tft.setCursor(10, 100);
      tft.setTextColor(RED);
      tft.println("Usuario nao encontrado!");
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
    }
  } else {
    tft.setCursor(10, 100);
    tft.setTextColor(RED);
    tft.println("Erro ao acessar Firebase!");
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  }

  delay(2000);
  limpaTela();
}


void adicionaEntrada() {
  if (valorLogin != "" & op == 1) {
    op++;
    return;
  } else if (op == 2) {
    verificaUsuario();
    limpaTela();
    return;
  }
}
