#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char *ssid = "Esp_ArkDark";
const char *password = "123456788";

WiFiServer server(80);

unsigned long fastbreak = 0;
unsigned long timer = 0;

const int Motor1_1 = 17; // IN1
const int Motor1_2 = 18; // IN2
const int Motor1_3 = 16; // ENA

const int Motor2_1 = 21; // IN3
const int Motor2_2 = 22; // IN4
const int Motor2_3 = 23; // ENB

void setup() {
  pinMode(Motor1_1, OUTPUT);
  pinMode(Motor1_2, OUTPUT);
  pinMode(Motor1_3, OUTPUT);

  pinMode(Motor2_1, OUTPUT);
  pinMode(Motor2_2, OUTPUT);
  pinMode(Motor2_3, OUTPUT);

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

          int speed_left = motor;
          int speed_right = motor;

          Serial.print("Motor: ");
          Serial.println(motor);
          Serial.print("Relay: ");
          Serial.println(relay);

          motors(speed_left, speed_right);
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}

void motors(int speed_left, int speed_right) {
  speed_left = constrain(speed_left, -255, 255);
  speed_right = constrain(speed_right, -255, 255);

  // Управление первым мотором
  if (speed_left >= 0) {
    digitalWrite(Motor1_1, HIGH);
    digitalWrite(Motor1_2, LOW);
    analogWrite(Motor1_3, speed_left);
  } else {
    digitalWrite(Motor1_1, LOW);
    digitalWrite(Motor1_2, HIGH);
    analogWrite(Motor1_3, abs(speed_left));
  }

  // Управление вторым мотором
  if (speed_right >= 0) {
    digitalWrite(Motor2_1, HIGH);
    digitalWrite(Motor2_2, LOW);
    analogWrite(Motor2_3, speed_right);
  } else {
    digitalWrite(Motor2_1, LOW);
    digitalWrite(Motor2_2, HIGH);
    analogWrite(Motor2_3, abs(speed_right));
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
