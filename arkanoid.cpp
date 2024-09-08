#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

const char * ssid = "Arkanoid_ArkDark";
const char * password = "ArkDark13579";

unsigned int localPort = 8888;
WiFiUDP udp;

const int Motor1_1 = 1;
const int Motor1_2 = 2;
const int Motor2_1 = 3;
const int Motor2_2 = 4;

const int relay1 = 8;
const int relay2 = 9;

void setup() {
  Serial.begin(5760);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  pinMode(Motor1_1, OUTPUT);
  pinMode(Motor1_2, OUTPUT);
  pinMode(Motor2_1, OUTPUT);
  pinMode(Motor2_2, OUTPUT);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  udp.begin(localPort);
}

void loop() {
  int packet = udp.parsePacket();
  if (packet) {
    char Buffer[4];
    udp.read(Buffer, 4);

    int speed = (Buffer[0] - '0') * 100 + (Buffer[1] - '0') * 10 + (Buffer[2] - '0');
    int relay = Buffer[3] - '0';

    int speed_left = speed;
    int speed_right = speed;

    motors(speed_left, speed_right);

    digitalWrite(relay1, relay & 0x1);
    digitalWrite(relay2, (relay >> 1) & 0x1);
  }
}

void motors(int speed_left, int speed_right) {
  speed_left = constrain(speed_left, -255, 255);
  speed_right = constrain(speed_right, -255, 255);

  if (speed_left >= 0) {
    digitalWrite(Motor1_1, LOW);
    analogWrite(Motor1_2, speed_left);
  }
  else {
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
