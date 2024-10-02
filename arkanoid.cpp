#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char *ssid = "Esp_ArkDark";
const char *password = "123456788";

WiFiServer server(80);

void setup() {
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
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    while (client.connected()) {
      if (client.available()) {
        uint8_t Buffer[4];
        client.read(Buffer, 4);

        int speed = (Buffer[0] * 100) + (Buffer[1] * 10) + Buffer[2];
        int relay = Buffer[3];

        Serial.print("Speed:");
        Serial.println(speed);
        Serial.print("Relay:");
        Serial.println(relay);
      }
    }
  }
}

