// FIRMWARE: programa que roda no ESP32.
// WiFi conecta à rede; WebServer recebe HTTP; web_assets guarda a página na flash.
#include <WiFi.h>
#include <WebServer.h>
#include "web_assets.h"

// Configure apenas na sua cópia local antes de gravar.
const char* ssid = "iot2";
const char* senha = "SUA_SENHA_WIFI";

// Porta 80 é a porta HTTP. O celular e o ESP32 devem estar na mesma rede.
WebServer servidor(80);
// IN1/IN2 e IN3/IN4 controlam os dois canais da ponte H.
const int IN1 = 25;
const int IN2 = 26;
const int IN3 = 27;
const int IN4 = 32;
// Componentes vermelho, verde e azul do LED direito (1) e esquerdo (2).
const int pino_led_r1 = 22;
const int pino_led_g1 = 23;
const int pino_led_b1 = 18;
const int pino_led_r2 = 19;
const int pino_led_g2 = 21;
const int pino_led_b2 = 33;
const int pinoLDR = 34;
// Limite de leitura analógica, não uma medida em lux.
const int limiteLuz = 300;
int valorLuz = 0;

// Motores: HIGH e LOW definem o sentido conforme a ligação da ponte H.
// Não há PWM: estas funções não ajustam a velocidade.
// Em parar, todas as entradas vão para LOW; nas curvas, só um canal é acionado.
void parar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void frente() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void re() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void virarDireita() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void virarEsquerda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
// No circuito utilizado, os três componentes acesos formam branco.
void ligarFarol() {
  digitalWrite(pino_led_r1, HIGH);
  digitalWrite(pino_led_g1, HIGH);
  digitalWrite(pino_led_b1, HIGH);
  digitalWrite(pino_led_r2, HIGH);
  digitalWrite(pino_led_g2, HIGH);
  digitalWrite(pino_led_b2, HIGH);
}

void desligarFarol() {
  digitalWrite(pino_led_r1, LOW);
  digitalWrite(pino_led_g1, LOW);
  digitalWrite(pino_led_b1, LOW);
  digitalWrite(pino_led_r2, LOW);
  digitalWrite(pino_led_g2, LOW);
  digitalWrite(pino_led_b2, LOW);
}

// Lê o LDR: abaixo do limite, liga os faróis; acima, desliga.
void verificarLuz() {
  valorLuz = analogRead(pinoLDR);
  if (valorLuz < limiteLuz) {
    ligarFarol();
  } else {
    desligarFarol();
  }
}

// Exemplo recebido: /comando?acao=frente. arg lê o parâmetro após o ?.
// O if/else escolhe a função. parar ou qualquer ação desconhecida para os motores.
void receberComando() {
  String acao = servidor.arg("acao");

  if (acao == "frente") {
    frente();
  } else if (acao == "re") {
    re();
  } else if (acao == "direita") {
    virarDireita();
  } else if (acao == "esquerda") {
    virarEsquerda();
  } else {
    parar();
  }

  // Responde ao navegador: 200 significa sucesso; OK é o texto da resposta.
  servidor.sendHeader("Cache-Control", "no-store");
  servidor.send(200, "text/plain", "OK");
}

// Estas funções entregam HTML, CSS e JS ao navegador.
// send_P lê os textos embutidos na memória flash pelo arquivo web_assets.h.
void abrirPagina() {
  servidor.sendHeader("Cache-Control", "no-store");
  servidor.send_P(200, "text/html; charset=utf-8", INDEX_HTML);
}

void enviarEstilo() {
  servidor.send_P(200, "text/css; charset=utf-8", STYLES_CSS);
}

void enviarJavascript() {
  servidor.sendHeader("Cache-Control", "no-store");
  servidor.send_P(200, "application/javascript; charset=utf-8", APP_JS);
}

void enviarManifesto() {
  servidor.send_P(200, "application/manifest+json", MANIFEST_JSON);
}

// Resposta mínima em JSON: confirma que o servidor está acessível.
// Não é telemetria do LDR nem confirmação de que o motor se moveu.
void verificarConexao() {
  servidor.sendHeader("Cache-Control", "no-store");
  servidor.send(200, "application/json", "{\"conectado\":true}");
}

// Executa uma vez ao ligar: configura pinos, conecta o Wi-Fi e inicia o servidor.
void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(pino_led_r1, OUTPUT);
  pinMode(pino_led_g1, OUTPUT);
  pinMode(pino_led_b1, OUTPUT);
  pinMode(pino_led_r2, OUTPUT);
  pinMode(pino_led_g2, OUTPUT);
  pinMode(pino_led_b2, OUTPUT);

  pinMode(pinoLDR, INPUT);
  parar();
  verificarLuz();

  // Aguarda a conexão; o endereço atribuído aparece no Monitor Serial.
  WiFi.begin(ssid, senha);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Wi-Fi conectado!");
  Serial.print("Abra no navegador: http://");
  Serial.println(WiFi.localIP());

  // Cada rota associa um endereço a uma função, sem executá-la agora.
  servidor.on("/", HTTP_GET, abrirPagina);
  servidor.on("/styles.css", HTTP_GET, enviarEstilo);
  servidor.on("/app.js", HTTP_GET, enviarJavascript);
  servidor.on("/manifest.webmanifest", HTTP_GET, enviarManifesto);
  servidor.on("/status", HTTP_GET, verificarConexao);
  servidor.on("/comando", HTTP_GET, receberComando);
  servidor.begin();
  Serial.println("Servidor HTTP iniciado.");
  Serial.println("Carrinho pronto!");
}
// Repete continuamente. handleClient atende as requisições recebidas.
// delay(50) pausa 50 ms entre ciclos.
// Não há watchdog: perder só a conexão do celular não garante parada automática.
void loop() {
  // PARA SE O ESP32 PERDER O WIFI
  if (WiFi.status() != WL_CONNECTED) {
    parar();
  }

  // RECEBE OS COMANDOS
  servidor.handleClient();

  // CONTROLA OS FAROIS
  verificarLuz();

  delay(50);
}
