#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <UrlEncode.h>
#include <ESP32Servo.h>

// ===== WIFI =====
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ===== CALLMEBOT =====
String phoneNumber = "557583673663";
String apiKey = "9434444";

// ===== PINOS =====
#define DHT_PIN 13
#define DHT_TYPE DHT22

#define LDR_PIN 4
#define PIR_PIN 5

// ===== LED RGB =====
#define LED_R 16
#define LED_G 17
#define LED_B 18

// ===== JOYSTICK =====
#define JOY_X 6
#define JOY_Y 7
#define JOY_SW 8

// ===== SERVO =====
#define SERVO_PIN 10

// ===== OBJETOS =====
DHT dht(DHT_PIN, DHT_TYPE);

Servo servoMotor;

// ===== VARIÁVEIS =====
float temperatura = 0;
float umidade = 0;

int luz = 0;

bool presenca = false;

int probabilidadeVida = 0;

String estadoRobo = "Ligado";

bool alertaEnviado = false;

// ===== WHATSAPP =====
void enviarWhatsApp(String mensagem) {

  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    String mensagemCodificada = urlEncode(mensagem);

    String url = "https://api.callmebot.com/whatsapp.php?phone=" +
                 phoneNumber +
                 "&text=" +
                 mensagemCodificada +
                 "&apikey=" +
                 apiKey;

    Serial.println("Enviando mensagem...");
    Serial.println(url);

    http.begin(url);

    int httpResponseCode = http.GET();

    Serial.print("HTTP Response: ");
    Serial.println(httpResponseCode);

    http.end();

  } else {

    Serial.println("WiFi desconectado!");

  }
}

// ===== RGB =====
void setColor(int r, int g, int b) {

  analogWrite(LED_R, r);
  analogWrite(LED_G, g);
  analogWrite(LED_B, b);

}

// ===== SETUP =====
void setup() {

  Serial.begin(115200);

  // sensores
  pinMode(PIR_PIN, INPUT);

  // joystick
  pinMode(JOY_SW, INPUT_PULLUP);

  // rgb
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  // servo
  servoMotor.attach(SERVO_PIN);
  servoMotor.write(90); // posição central

  // dht
  dht.begin();

  // wifi
  WiFi.begin(ssid, password);

  Serial.print("Conectando ao WiFi");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");

  }

  Serial.println("\nWiFi conectado!");

  // teste whatsapp
  enviarWhatsApp("ESP32 conectado com sucesso!");

}

// ===== LOOP =====
void loop() {

  // ===== SENSORES =====
  temperatura = dht.readTemperature();

  umidade = dht.readHumidity();

  luz = analogRead(LDR_PIN);

  presenca = digitalRead(PIR_PIN);

  // ===== JOYSTICK =====
  int joyX = analogRead(JOY_X);

  int joyY = analogRead(JOY_Y);

  bool botao = !digitalRead(JOY_SW);

  // ===== VERIFICA DHT =====
  if (isnan(temperatura) || isnan(umidade)) {

    Serial.println("Erro ao ler DHT!");

    return;

  }

  // ===== PROBABILIDADE =====
  probabilidadeVida = 0;

  if (temperatura >= 15 && temperatura <= 30) {
    probabilidadeVida += 25;
  }

  if (umidade >= 40 && umidade <= 70) {
    probabilidadeVida += 25;
  }

  if (luz > 2000) {
    probabilidadeVida += 20;
  }

  if (presenca) {
    probabilidadeVida += 30;
  }

  // ===== ALERTA =====
  if (probabilidadeVida > 75) {

    setColor(255, 0, 0);

    estadoRobo = "ALERTA";

    if (!alertaEnviado) {

      enviarWhatsApp("ALERTA! Alta probabilidade de vida detectada.");

      alertaEnviado = true;

      Serial.println("Mensagem enviada!");

    }

  } else {

    setColor(0, 255, 0);

    estadoRobo = "Explorando";

    alertaEnviado = false;

  }

  // ===== CONTROLE DO SERVO =====
  String direcao = "Parado";

  // CENTRO
  servoMotor.write(90);

  // DIREITA
  if (joyX > 3000) {

    servoMotor.write(180);

    direcao = "Direita";

  }

  // ESQUERDA
  else if (joyX < 1000) {

    servoMotor.write(0);

    direcao = "Esquerda";

  }

  // FRENTE
  else if (joyY > 3000) {

    servoMotor.write(135);

    direcao = "Frente";

  }

  // RÉ
  else if (joyY < 1000) {

    servoMotor.write(45);

    direcao = "Ré";

  }

  // ===== BOTÃO JOYSTICK =====
  if (botao) {

    estadoRobo = "Desligado";

    setColor(255, 0, 0);

    servoMotor.write(90);

  }

  // ===== SERIAL =====
  Serial.println("------ DADOS DO SISTEMA ------");

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");

  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.println(" %");

  Serial.print("Luz: ");
  Serial.println(luz);

  Serial.print("Presença: ");
  Serial.println(presenca ? "Detectada" : "Sem presença");

  Serial.print("Joystick X: ");
  Serial.println(joyX);

  Serial.print("Joystick Y: ");
  Serial.println(joyY);

  Serial.print("Direção: ");
  Serial.println(direcao);

  Serial.print("Estado do Robô: ");
  Serial.println(estadoRobo);

  Serial.print("Probabilidade de Vida: ");
  Serial.print(probabilidadeVida);
  Serial.println("%");

  Serial.println("--------------------------------\n");

  delay(2000);

}
