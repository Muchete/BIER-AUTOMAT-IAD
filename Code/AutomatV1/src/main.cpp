
#include "MQTTClient.h"
#include "ESP8266WiFi.h"
MQTTClient client;
WiFiClient net;

unsigned long lastMillis = 0;
const char* ssid     = "Muchete";
const char* password = "helmspecht";
String ip;
int pinNo = 2;

void connectToWifi() {
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  ip = WiFi.localIP().toString();
  Serial.println(WiFi.localIP());
}

void connectToShiftr() {
  Serial.print("connecting...");
  while (!client.connect("nodeMCUinBeeromat", "espController", "bc905f6f41006cda")) {
    Serial.print(".");
  }

  Serial.println("\nconnected!");
  client.publish("/myIP", ip);

  client.subscribe("/example");
  // client.unsubscribe("/example");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  digitalWrite(pinNo, LOW);
}

void setup() {
  Serial.begin(115200);
  delay(10);
  connectToWifi();

  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);
  connectToShiftr();

  pinMode(pinNo, OUTPUT);
  digitalWrite(pinNo, HIGH);
}

void loop() {
  client.loop();

  if(!client.connected()) {
    connectToShiftr();
  }

  // publish a message roughly every second.
  if(millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("/hello", "Pimmel");
  }

  delay(5000);
  digitalWrite(pinNo, HIGH);

  // digitalWrite(pinNo, HIGH);
  // Serial.println("HIGH");
  // delay(1000);
  // digitalWrite(pinNo, LOW);
  // Serial.println("LOW");
  // delay(1000);
  // 172.20.10.11
}
