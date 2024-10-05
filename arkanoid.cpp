#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char *ssid = "Esp_ArkDark";
const char *password = "123456788";

WiFiServer server(80);

void setup()  {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  while (!WiFi.softAP(ssid, password)) {
    Serial.println("Soft AP creation failed.");
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available(); // Use available() to check for a client
  if (client) {
    Serial.println("New client connected");
    while (client.connected()) {
      if (client.available()) {
        String data = client.readStringUntil('\n');
        int commaIndex = data.indexOf(',');
        if (commaIndex != -1) {
          int speed = data.substring(0, commaIndex).toInt();
          int relay = data.substring(commaIndex + 1).toInt();

          Serial.print("Speed: ");
          Serial.println(speed);
          Serial.print("Relay: ");
          Serial.println(relay);
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}