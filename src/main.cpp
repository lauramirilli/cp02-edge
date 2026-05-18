#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTTYPE DHT22
// #define DHTTYPE DHT11 - retirar comentário para usar com o DHT11

// Pinos
#define DHTPIN 2
#define LDR_PIN A0
#define LED_GREEN 10
#define LED_YELLOW 9
#define LED_RED 8
#define BUZZER_PIN 1

// Faixas de lux
#define LUX_ESCURO 30000
#define LUX_MEIA_LUZ 60000

// Faixas de temperatura
#define TEMP_MIN 10.0
#define TEMP_MAX 15.0

// Faixas de umidade
#define UMID_MIN 50.0
#define UMID_MAX 70.0

// Constantes do LDR
#define RL10 50
#define GAMMA 0.7


DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte vinho[8] = {
  B00000,
  B10001,
  B10001,
  B01110,
  B00100,
  B00100,
  B01110,
};

byte gota[8] = {
  B00000,
  B00100,
  B01010,
  B10001,
  B10001,
  B10001,
  B01110,
};

byte lamp[8] = {
  B00000,
  B00000,
  B01100,
  B10010,
  B10010,
  B01100,
  B01100,
};

// Variáveis de controle de tela
// Tela 0 = Luz | Tela 1 = Temperatura | Tela 2 = Umidade
int telaAtual = 0;
unsigned long ultimaTroca = 0;
const unsigned long INTERVALO_TELA = 4000; // 5 segundos

// Função: ler lux médio (5 amostras)
float lerLux() {
  float soma = 0;
  for (int i = 0; i < 5; i++) {
    int leitura = analogRead(LDR_PIN);
    float voltage = leitura / 1024.0 * 5.0;
    float resistance = 2000.0 * voltage / (1.0 - voltage / 5.0);
    float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, 1.0 / GAMMA);
    soma += lux;
    delay(10);
  }
  return soma / 5.0;
}

// Função: ler temperatura média (5 amostras)
float lerTemp() {
  float soma = 0;
  for (int i = 0; i < 5; i++) {
    soma += dht.readTemperature();
    delay(10);
  }
  return soma / 5.0;
}

// Função: ler umidade média (5 amostras)
float lerUmid() {
  float soma = 0;
  for (int i = 0; i < 5; i++) {
    soma += dht.readHumidity();
    delay(10);
  }
  return soma / 5.0;
}

// Função: controlar LEDs
void atualizarLEDs(float lux, float temp, float umid) {
  bool alertaVermelho = false;
  bool alertaAmarelo  = false;

  if (umid < UMID_MIN || umid > UMID_MAX) {
    alertaVermelho = true;
  }

  if (temp < TEMP_MIN || temp > TEMP_MAX) {
    alertaAmarelo = true;
  }

  if (lux > LUX_MEIA_LUZ) {
    alertaVermelho = true;
  }

  // Aplicar prioridade
  if (alertaVermelho) {
    digitalWrite(LED_RED,    HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN,  LOW);
    tone(BUZZER_PIN, 1000); // buzzer ligado
  } else if (alertaAmarelo || (lux > LUX_ESCURO && lux <= LUX_MEIA_LUZ)) {
    digitalWrite(LED_RED,    LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_GREEN,  LOW);
    noTone(BUZZER_PIN);
  } else {
    // Tudo OK / ambiente escuro
    digitalWrite(LED_RED,    LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN,  HIGH);
    noTone(BUZZER_PIN);
  }
}

// Função: exibir tela de LUZ
void telaLuz(float lux) {
  lcd.clear();
  if (lux <= LUX_ESCURO) {
    lcd.setCursor(0, 0);
    lcd.print("Ambiente escuro");
  } else if (lux <= LUX_MEIA_LUZ) {
    lcd.setCursor(0, 0);
    lcd.print("Ambiente a meia");
    lcd.setCursor(0, 1);
    lcd.print("luz");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Ambiente muito");
    lcd.setCursor(0, 1);
    lcd.print("claro!");
  }
  lcd.setCursor(7, 1);
  lcd.write(byte(2));
}

// Função: exibir tela de TEMPERATURA
void telaTemp(float temp) {
  lcd.clear();
  if (temp >= TEMP_MIN && temp <= TEMP_MAX) {
    lcd.setCursor(0, 0);
    lcd.print("Temperatura OK");
  } else if (temp > TEMP_MAX) {
    lcd.setCursor(0, 0);
    lcd.print("Temp. ALTA");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Temp. BAIXA");
  }
  lcd.setCursor(0, 1);
  lcd.print("Temp. = ");
  lcd.print(temp, 1);
  lcd.print(" C");
}

// Função: exibir tela de UMIDADE
void telaUmid(float umid) {
  lcd.clear();
  if (umid >= UMID_MIN && umid <= UMID_MAX) {
    lcd.setCursor(0, 0);
    lcd.print("Umidade OK");
  } else if (umid > UMID_MAX) {
    lcd.setCursor(0, 0);
    lcd.print("Umidade ALTA");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Umidade BAIXA");
  }
  lcd.setCursor(0, 1);
  lcd.print(umid, 1);
  lcd.print(" %");
  lcd.setCursor(8, 1);
  lcd.write(byte(1));
}

void setup() {
  pinMode(LED_GREEN,  OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED,    OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  dht.begin();
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, vinho);
  lcd.createChar(1, gota);
  lcd.createChar(2, lamp);

  lcd.setCursor(0, 0);
  lcd.print("Vinheria");
  lcd.setCursor(0, 1);
  lcd.print("Agnello");
  lcd.setCursor(8, 1);
  lcd.write(byte(0));
  delay(2000);
  lcd.clear();
}

void loop() {
  unsigned long agora = millis();

  // Trocar tela a cada 5 segundos
  if (agora - ultimaTroca >= INTERVALO_TELA) {
    ultimaTroca = agora;

    float lux  = lerLux();
    float temp = lerTemp();
    float umid = lerUmid();

    atualizarLEDs(lux, temp, umid);

    switch (telaAtual) {
      case 0: telaLuz(lux);   break;
      case 1: telaTemp(temp); break;
      case 2: telaUmid(umid); break;
    }

    telaAtual = (telaAtual + 1) % 3;
  }
}
