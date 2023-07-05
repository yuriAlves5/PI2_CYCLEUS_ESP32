#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

Adafruit_SSD1306 display(128, 64, &Wire);

#define relay1 23
#define relay2 25

const char *ssid = "iPhone Alisson";
const char *password = "214bsb61afs";

WiFiServer server(80);

void relay_wifi();

void setup()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.display();
  display.clearDisplay();
  display.setCursor(0, 0);

  delay(500);
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setRotation(0);

  Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(741);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  display.println("WiFi connected");
  Serial.println("IP Address: ");
  display.println("IP Address:");
  Serial.println(WiFi.localIP());
  display.print(WiFi.localIP());

  server.begin();
  display.display();
}

void loop()
{
  while (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(ssid, password);
    Serial.print(".");
    delay(741);
  }

  relay_wifi();
}

void abrirTrava()
{
  digitalWrite(relay1, HIGH);
  delay(9000);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
}

void fecharTrava()
{
  digitalWrite(relay2, HIGH);
  delay(9000);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
}

void relay_wifi()
{
  WiFiClient client = server.available();

  if (client)
  {
    Serial.println("New Client Defined");
    String currentLine = "";

    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);

        if (c == '\n')
        {
          if (currentLine.length() == 0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<h2>Request Received!</h2>");
            client.println();
            break;
          }
          else
            currentLine = "";
        }
        else if (c != '\r')
          currentLine += c;

        if (currentLine.endsWith("GET /station1/abrir"))
          abrirTrava();
        if (currentLine.endsWith("GET /station1/fechar"))
          fecharTrava();
      }
    }

    client.stop();
    Serial.println("Client Disconnected.");
  }
}
