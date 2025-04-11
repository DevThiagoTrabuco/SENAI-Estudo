#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#define WIFI_SSID //"Wokwi-GUEST"
#define WIFI_PASSWORD //""

#define API_KEY "AIzaSyBrnqehQ_h9MbIcYlDHK4BHbyvgztQZRqs"
#define DATABASE_URL "https://iot-senai-esp32-default-rtdb.firebaseio.com" 
#define USER_EMAIL "conta.qualquer@email.com"
#define USER_PASSWORD "415263senha"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

const int ledPin = 1;

void setup()
{
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);
  Firebase.setDoubleDigits(5);
  config.timeout.serverResponse = 10 * 1000;
}

void loop()
{
  if (Firebase.RTDB.getInt(&fbdo, "/statusLed/estado")) {
    int ledState = fbdo.intData();
    digitalWrite(ledPin, ledState ? HIGH : LOW);
    Serial.printf("Estado do LED: %d\n", ledState);
  } else {
    Serial.print("Erro ao ler do Firebase: ");
    Serial.println(fbdo.errorReason());
  }

  delay(1000); // atualiza a cada 1s
}