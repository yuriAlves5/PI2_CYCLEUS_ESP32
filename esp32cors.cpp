#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

#define relay1 23
#define relay2 25

const char *ssid = "yuri";
const char *password = "123456788";

WebServer server(80);

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}

void handleAbrirTrava() {
  abrirTrava();
  server.send(200, "text/html", "A trava foi aberta com sucesso!");
}

void handleFecharTrava() {
  fecharTrava();
  server.send(200, "text/html", "A trava foi fechada com sucesso!");
}

void handleCORS() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET,POST,PUT,PATCH,DELETE,OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type, Access-Control-Allow-Headers, Authorization, X-Requested-With");
  server.send(204);
}

void abrirTrava() {
  digitalWrite(relay1, HIGH);
  delay(9000);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
}

void fecharTrava() {
  digitalWrite(relay2, HIGH);
  delay(9000);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  connectToWiFi();

  server.on("/abrir_trava", handleAbrirTrava);
  server.on("/fechar_trava", handleFecharTrava);
  server.on("/OPTIONS", handleCORS);
  
  server.begin();
}

void loop() {
  server.handleClient();
}
