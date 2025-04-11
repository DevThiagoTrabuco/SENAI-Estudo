#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <ESP32Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

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

const int ledVerde = 12;
const int ledVermelho = 13;
const int pinoServo = 14;
Servo servo;

String senhaCorreta = "1234";
String senhaDigitada = "";

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 26);
  lcd.init();
  lcd.backlight();

  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  servo.attach(pinoServo);
  servo.write(0);
  lcd.setCursor(0, 0);
  lcd.print("Digite a senha:");
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.print("Tecla: ");
    Serial.println(key);

    if (key == '#') {
      verificaSenha();
    } else if (key == '*') {
      senhaDigitada = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Senha apagada");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Digite a senha:");
    } else {
      senhaDigitada += key;
      lcd.setCursor(0, 1);
      lcd.print(senhaDigitada);
    }
  }
}

void verificaSenha() {
  if (senhaDigitada == senhaCorreta) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Senha correta!");
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);
    servo.write(90); // "Destrava"
    delay(3000);
    servo.write(0);  // Volta
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

  senhaDigitada = "";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Digite a senha:");
}
