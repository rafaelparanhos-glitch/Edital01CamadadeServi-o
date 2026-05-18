Link para o wokwi:https://wokwi.com/projects/463209459083119617

📌 Objetivo do Projeto

O projeto consiste em um robô explorador espacial que:

Monitora temperatura, umidade, luminosidade e presença;
Calcula uma probabilidade de vida extraterrestre;
Aciona alertas visuais com LED RGB;
Envia notificações via WhatsApp usando o CallMeBot;
Controla movimentação através de joystick;
Utiliza servo motor para simular movimentação do robô.

Este projeto foi desenvolvido para a disciplina de Práticas Integradas – Camada de Percepção.

🛠️ Componentes Utilizados
| Componente          | Quantidade |
| ------------------- | ---------- |
| ESP32-S3            | 1          |
| Sensor DHT22        | 1          |
| Sensor PIR          | 1          |
| LDR (Fotorresistor) | 1          |
| LED RGB             | 1          |
| Joystick Analógico  | 1          |
| Servo Motor         | 1          |
| Protoboard          | 1          |
| Jumpers             | Vários     |

📚 Bibliotecas Utilizadas

Instale as seguintes bibliotecas na Wokwi:

WiFi.h
HTTPClient.h
DHT.h
UrlEncode.h
ESP32Servo.h

⚙️ Funcionalidades
🌡️ Monitoramento Ambiental

O robô realiza leituras de:

Temperatura
Umidade
Luminosidade
Presença de movimento

Os dados são exibidos no Monitor Serial.

🧠 Cálculo de Probabilidade de Vida

A probabilidade é calculada utilizando regras simples:
| Condição                      | Pontuação |
| ----------------------------- | --------- |
| Temperatura entre 15°C e 30°C | +25%      |
| Umidade entre 40% e 70%       | +25%      |
| Luminosidade acima do limite  | +20%      |
| Presença detectada            | +30%      |
Se a soma ultrapassar 75%, o sistema entra em modo de alerta.

🚨 Sistema de Alerta

Quando a probabilidade de vida é alta:

LED RGB fica vermelho;
Mensagem é enviada via WhatsApp;
Estado do robô muda para ALERTA.

Mensagem enviada:

ALERTA! Alta probabilidade de vida detectada.

🎮 Controle do Robô

O joystick controla a direção do servo motor:
| Movimento | Ação       |
| --------- | ---------- |
| Direita   | Servo 180° |
| Esquerda  | Servo 0°   |
| Frente    | Servo 135° |
| Ré        | Servo 45°  |
O botão do joystick desliga o robô.

📲 Configuração do CallMeBot
Adicione o número:
+34 644 78 33 97
Envie no WhatsApp:
I allow callmebot to send me messages
Você receberá sua API Key.
Configure no código:
String phoneNumber = "SEU_NUMERO";
String apiKey = "SUA_APIKEY";

🔌 Pinagem do Projeto
| Componente     | Pino    |
| -------------- | ------- |
| DHT22          | GPIO 13 |
| LDR            | GPIO 4  |
| PIR            | GPIO 5  |
| LED Vermelho   | GPIO 16 |
| LED Verde      | GPIO 17 |
| LED Azul       | GPIO 18 |
| Joystick X     | GPIO 6  |
| Joystick Y     | GPIO 7  |
| Botão Joystick | GPIO 8  |
| Servo Motor    | GPIO 10 |

▶️ Como Executar no Wokwi
1. Acesse o Simulador

Abra o site oficial do Wokwi Simulator

2. Faça Login ou Crie uma Conta
Clique em Sign In
Crie uma conta gratuita ou faça login
3. Crie um Novo Projeto
Clique em:
New Project
Escolha:
ESP32
4. Adicione os Componentes

No menu lateral do Wokwi, adicione:

ESP32-S3
DHT22
LDR
PIR Sensor
LED RGB
Servo Motor
Joystick
5. Faça as Conexões

Conecte os componentes conforme a tabela abaixo:
| Componente     | Pino ESP32 |
| -------------- | ---------- |
| DHT22          | GPIO 13    |
| LDR            | GPIO 4     |
| PIR            | GPIO 5     |
| LED Vermelho   | GPIO 16    |
| LED Verde      | GPIO 17    |
| LED Azul       | GPIO 18    |
| Joystick X     | GPIO 6     |
| Joystick Y     | GPIO 7     |
| Botão Joystick | GPIO 8     |
| Servo Motor    | GPIO 10    |

6. Cole o Código
Abra o arquivo:
sketch.ino
Apague o código padrão
Cole o código do projeto
7. Instale as Bibliotecas

No arquivo libraries.txt, adicione:

DHT sensor library
UrlEncode
ESP32Servo
8. Configure o Wi-Fi

O Wokwi utiliza automaticamente:

const char* ssid = "Wokwi-GUEST";
const char* password = "";

Não é necessário alterar.

9. Configure o CallMeBot

Substitua no código:

String phoneNumber = "SEU_NUMERO";
String apiKey = "SUA_APIKEY";

Pelos seus dados do CallMeBot.

10. Execute a Simulação

Clique no botão:

▶ Start Simulation
📟 O que Deve Acontecer

Durante a simulação:

O Monitor Serial mostrará:
temperatura
umidade
luminosidade
presença
probabilidade de vida
O LED RGB mudará:
Verde → exploração normal
Vermelho → alerta
O Servo Motor responderá ao joystick
O sistema enviará alerta via WhatsApp quando a probabilidade ultrapassar 75%
