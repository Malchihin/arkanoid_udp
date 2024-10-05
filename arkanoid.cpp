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
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");
  
   pinMode(Motor1_1, OUTPUT);
  pinMode(Motor1_2, OUTPUT);
  pinMode(Motor2_1, OUTPUT);
  pinMode(Motor2_2, OUTPUT);
  
  //pinMode(relay1, OUTPUT);
  //pinMode(relay2, OUTPUT);

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

        int motor = (Buffer[0] * 100) + (Buffer[1] * 10) + Buffer[2];
        int relay = Buffer[3];

        Serial.print("motor:");
        Serial.println(motor);
        Serial.print("Relay:");
        Serial.println(relay);

        int speed_left = motor;
        int speed_right = motor;

        motors(motor);

        if (relay == 1 && millis() - fastbreak >= 3000) {
          if (millis() - timer >= 950) punch();
          else digitalWrite(8, 1);
        } else {
          timer = millis();
        }
        delay(4);
      }
    }
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