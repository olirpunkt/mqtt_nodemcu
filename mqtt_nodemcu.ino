#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* SSID = "";
const char* PSK = "";
const char* MQTT_BROKER = "192.168.178.200";

const int pinStatusGarageDoor = D4;


WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  pinMode(pinStatusGarageDoor, INPUT);
  setup_wifi();
  client.setServer(MQTT_BROKER, 8883);

}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PSK);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Reconnecting...");
    if (!client.connect("Garage")) {
      Serial.print("Failed, rd=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //snprintf(msg, 50, "Alive since %ld milliseconds", millis());
  //Serial.print("Publish message: ");
  //Serial.println(msg);
  //client.publish("/home/garage", msg);

  if (digitalRead(pinStatusGarageDoor) == LOW) {
    Serial.println("Open");
    snprintf(msg, 50, "{\"status\":\open\"}");
    Serial.println(msg);
    client.publish("/home/garage/door", msg);
  }
  else {
    Serial.println("Closed");
    snprintf(msg, 50, "{\"status\":\closed\"}");
    client.publish("/home/garage/door", msg);
    Serial.println(msg);
  }
  
  delay(1000);
}
