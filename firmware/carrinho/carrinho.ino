#include <WiFi.h>
#include <WebServer.h>
#include "web_assets.h"

// Configure apenas na sua cópia local antes de gravar.
const char* ssid = "iot2";
const char* senha = "SUA_SENHA_WIFI";

WebServer servidor(80);
const int IN1 = 25, IN2 = 26, IN3 = 27, IN4 = 32;
const int pino_led_r1 = 22, pino_led_g1 = 23, pino_led_b1 = 18;
const int pino_led_r2 = 19, pino_led_g2 = 21, pino_led_b2 = 33;
const int pinoLDR = 34;
const int limiteLuz = 300;
int valorLuz = 0;
unsigned long ultimaLeitura = 0;
unsigned long ultimoComandoWeb = 0;
const unsigned long timeoutWeb = 1200;
bool movimentoWeb = false;

void parar() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  movimentoWeb = false;
}
void frente() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}
void re() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}
void virarDireita() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}
void virarEsquerda() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}
void verificarLuz() {
  valorLuz = analogRead(pinoLDR);
  int nivel = valorLuz < limiteLuz ? HIGH : LOW;
  digitalWrite(pino_led_r1, nivel); digitalWrite(pino_led_g1, nivel);
  digitalWrite(pino_led_b1, nivel); digitalWrite(pino_led_r2, nivel);
  digitalWrite(pino_led_g2, nivel); digitalWrite(pino_led_b2, nivel);
}
void receberComando() {
  String acao = servidor.arg("acao");
  bool valido = acao == "frente" || acao == "re" || acao == "direita"
    || acao == "esquerda" || acao == "parar";
  if (!valido) {
    parar();
    servidor.send(400, "text/plain", "Acao invalida");
    return;
  }
  if (acao == "frente") frente();
  else if (acao == "re") re();
  else if (acao == "direita") virarDireita();
  else if (acao == "esquerda") virarEsquerda();
  else parar();

  // O controle físico original continua usando seus comandos de apertar/soltar.
  // A página envia origem=web e renova o movimento a cada 250 ms.
  movimentoWeb = acao != "parar" && servidor.arg("origem") == "web";
  ultimoComandoWeb = millis();
  servidor.sendHeader("Cache-Control", "no-store");
  servidor.send(200, "text/plain", "OK");
}
void setup() {
  Serial.begin(9600);
  const int saidas[] = {IN1, IN2, IN3, IN4, pino_led_r1, pino_led_g1,
    pino_led_b1, pino_led_r2, pino_led_g2, pino_led_b2};
  for (int pino : saidas) pinMode(pino, OUTPUT);
  pinMode(pinoLDR, INPUT);
  parar();
  verificarLuz();

  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, senha);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Abra o painel em http://");
  Serial.println(WiFi.localIP());

  servidor.on("/", HTTP_GET, []() {
    servidor.sendHeader("Cache-Control", "no-store");
    servidor.send_P(200, "text/html; charset=utf-8", INDEX_HTML);
  });
  servidor.on("/styles.css", HTTP_GET, []() {
    servidor.send_P(200, "text/css; charset=utf-8", STYLES_CSS);
  });
  servidor.on("/app.js", HTTP_GET, []() {
    servidor.sendHeader("Cache-Control", "no-store");
    servidor.send_P(200, "application/javascript; charset=utf-8", APP_JS);
  });
  servidor.on("/manifest.webmanifest", HTTP_GET, []() {
    servidor.send_P(200, "application/manifest+json", MANIFEST_JSON);
  });
  servidor.on("/status", HTTP_GET, []() {
    servidor.sendHeader("Cache-Control", "no-store");
    servidor.send(200, "application/json", "{\"conectado\":true}");
  });
  servidor.on("/comando", HTTP_GET, receberComando);
  servidor.onNotFound([]() { servidor.send(404, "text/plain", "Nao encontrado"); });
  servidor.begin();
  Serial.println("Servidor iniciado. Motores parados.");
}
void loop() {
  if (WiFi.status() != WL_CONNECTED
      || (movimentoWeb && millis() - ultimoComandoWeb >= timeoutWeb)) {
    parar();
  }
  servidor.handleClient();
  if (millis() - ultimaLeitura >= 250) {
    ultimaLeitura = millis();
    verificarLuz();
  }
}
