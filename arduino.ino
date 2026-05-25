#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <UrlEncode.h>
#include <ESP32Servo.h>

// ===== WIFI =====
const char* ssid = "CIMATEC-VISITANTE";
const char* password = "";

// ===== CALLMEBOT =====
String phoneNumber = "557583673663";
String apiKey = "9434444";

// ===== SUPABASE =====
// Pegue em: Project Settings -> API
const char* SUPABASE_URL = "https://enclssgyqctwxhppdnol.supabase.co";
const char* SUPABASE_KEY = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImVuY2xzc2d5cWN0d3hocHBkbm9sIiwicm9sZSI6ImFub24iLCJpYXQiOjE3Nzk3MjAxNzMsImV4cCI6MjA5NTI5NjE3M30.hnMQTgtHuHZnec9YuAZycS3DjaH0SBBxyh35vtzbf5U";
const char* SUPABASE_TABELA = "leituras";

// Intervalo de envio ao Supabase (ms)
const unsigned long INTERVALO_SUPABASE = 2000;
unsigned long ultimoEnvioSupabase = 0;

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
#define SERVO_PIN 15   // <-- alterado de 10 para 15 (pino seguro)

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
// ===== SUPABASE =====
void enviarLeituraSupabase() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[SUPABASE] WiFi desconectado, pulando envio.");
    return;
  }
  // Monta o JSON com as leituras atuais
  StaticJsonDocument<256> doc;


  doc["temperatura_c"]      = temperatura;
  doc["umidade_pct"]        = umidade;
  doc["luminosidade"]       = luz;
  doc["presenca"]           = presenca ? 1 : 0;
  doc["probabilidade_vida"] = probabilidadeVida;
  // 'timestamp' é preenchido automaticamente pelo Supabase (default now())


  String corpo;
  serializeJson(doc, corpo);




  // Cliente HTTPS (Supabase é sempre HTTPS)
  WiFiClientSecure client;
  client.setInsecure();   // suficiente para projeto acadêmico


  HTTPClient https;
  String url = String(SUPABASE_URL) + "/rest/v1/" + SUPABASE_TABELA;




  if (!https.begin(client, url)) {


    Serial.println("[SUPABASE] Falha ao iniciar HTTPS.");
    return;


  }




  // Headers obrigatórios do Supabase
  https.addHeader("apikey",        SUPABASE_KEY);
  https.addHeader("Authorization", String("Bearer ") + SUPABASE_KEY);
  https.addHeader("Content-Type",  "application/json");
  https.addHeader("Prefer",        "return=minimal");




  int codigo = https.POST(corpo);




  if (codigo == 201) {


    Serial.println("[SUPABASE] Leitura gravada com sucesso.");


  } else {


    Serial.print("[SUPABASE] Erro HTTP ");
    Serial.print(codigo);
    Serial.print(" - Resposta: ");
    Serial.println(https.getString());


  }




  https.end();
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
  // ===== CONTROLE DO SERVO (CORRIGIDO) =====
  String direcao = "Parado";
  int posicaoServo = 90; // padrão: centro
  // DIREITA
  if (joyX > 3000) {
    posicaoServo = 180;
    direcao = "Direita";
  }
  // ESQUERDA
  else if (joyX < 1000) {
    posicaoServo = 0;
    direcao = "Esquerda";
  }
  // FRENTE
  else if (joyY > 3000) {
    posicaoServo = 135;
    direcao = "Frente";
  }
  // RÉ
  else if (joyY < 1000) {
    posicaoServo = 45;
    direcao = "Ré";
  }
  // ===== BOTÃO JOYSTICK =====
  if (botao) {
    estadoRobo = "Desligado";
    setColor(255, 0, 0);
    posicaoServo = 90; // força o servo de volta ao centro
  }
  // escreve no servo UMA única vez por loop
  servoMotor.write(posicaoServo);
  // ===== ENVIO AO SUPABASE (não bloqueante) =====
  // Dispara a cada 2 s sem travar o loop, mantendo o servo responsivo.
  if (millis() - ultimoEnvioSupabase >= INTERVALO_SUPABASE) {
    ultimoEnvioSupabase = millis();
    enviarLeituraSupabase();
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
  delay(200); // <-- reduzido de 2000ms para 200ms (servo responsivo)
}
