#include <WiFi.h>
#include <PubSubClient.h>

const String SSID = "Paulo";
const String PSWD = "cxos9674";

const String brokerUrl = "test.mosquito.org";
const int port = 1883;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void connectToWiFi();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(1152200);
  connectToWiFi();
  mqttClient.setServer(brokerUrl.c_str(),port);
  String userId = "ESP-teste09";
  mqttClient.connect(userId.c_str());
  while(!mqttClient.connected()){
    Serial.println("Erro de conexão");
    delay(500);
  }
  Serial.println("conectado com sucesso");

}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("conexão perdida");
    connectToWiFi();
  }
  mqttClient.loop();
}

void connectToWiFi(){
  Serial.println("Iniciando conexão con rede wifi");
  WiFi.begin(SSID,PSWD);
    while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(200);
    }
}

