#include <WiFi.h>
#include <PubSubClient.h>

const String SSID = "Paulo";
const String PSWD = "cxos9674";

const String brokerUrl = "test.mosquitto.org";
const int port = 1883;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void connectToWiFi();
void connectbroker();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(1152200);
  connectToWiFi();
  connectbroker();
}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("conexão perdida");
    connectToWiFi();
  }
  if(!mqttClient.connected()){
    Serial.println("conexão MQTT perdida");
    connectbroker();
  }
  mqttClient.loop();
}

void connectToWiFi(){
  Serial.println("Iniciando conexão con rede wifi");
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(SSID,PSWD);
      Serial.print(".");
      delay(2000);
    }
    Serial.println("conectado")
}

void connectbroker(){
  Serial.println("\n conectando ao broker...");
  mqttClient.setServer(brokerUrl.c_str(),port);
// user
  String userId = "ESP-PAULO";
  userId += String(random(0xffff), HEX);
//
  while(!mqttClient.connected()){
    mqttClient.connect(userId.c_str());
    Serial.print(".");
    delay(2000);
  }
  Serial.println("conectado com sucesso");
}
