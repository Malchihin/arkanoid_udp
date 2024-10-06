#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char *ssid = "Esp_ArkDark";
const char *password = "123456788";

WiFiServer server(80);

unsigned long fastbreak = 0;
unsigned long timer = 0;

const int Motor1_1 = 1;
const int Motor1_2 = 2;
const int Motor2_1 = 3;
const int Motor2_2 = 4;

void setup() {
  pinMode(Motor1_1, OUTPUT);
  pinMode(Motor1_2, OUTPUT);
  pinMode(Motor2_1, OUTPUT);
  pinMode(Motor2_2, OUTPUT);
  
  //pinMode(relay1, OUTPUT);
  //pinMode(relay2, OUTPUT);

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
          int motor = data.substring(0, commaIndex).toInt();
          int relay = data.substring(commaIndex + 1).toInt();

          Serial.print("Motor: ");
          Serial.println(motor);
          Serial.print("Relay: ");
          Serial.println(relay);
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}

void motors(int motor) {
  int speed_left = constrain(motor, -255, 255);
  int speed_right = constrain(motor, -255, 255);

  if (speed_left >= 0) {
    digitalWrite(Motor1_1, LOW);
    analogWrite(Motor1_2, speed_left);
  } else {
    digitalWrite(Motor1_1, HIGH);
    analogWrite(Motor1_2, abs(speed_left));
  }

  if (speed_right >= 0) {
    digitalWrite(Motor2_1, LOW);
    analogWrite(Motor2_2, speed_right);
  } else {
    digitalWrite(Motor2_1, HIGH);
    analogWrite(Motor2_2, abs(speed_right));
  }
}

void punch() {
  digitalWrite(8, 0);
  delay(50);
  digitalWrite(9, 1);
  delay(50);
  digitalWrite(9, 0);
  timer = millis();
  fastbreak = millis();
}