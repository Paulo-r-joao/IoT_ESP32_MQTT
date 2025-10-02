#include <WiFi.h>

const String SSID = "Paulo";
const String PSWD = "cxos9674";

void connectToWiFi();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(1152200);
  connectToWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("conexão perdida");
    connectToWiFi();
  }
}

void connectToWiFi(){
  Serial.println("Iniciando conexão con rede wifi");
  WiFi.begin(SSID,PSWD);
    while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(200);
    }

}

void scanLocalNetworks(){
  Serial.println("Iniciando Scan de redes Wi-Fi");

  int number = WiFi.scanNetworks();
  delay(500);

  if(number == -1){
    Serial.println("ERRO!. Não deu bom");
  }else{
    Serial.printf("Número de reders encontradas: %d\n", number);
    for(int net = 0; net < number; net++){
      Serial.printf("%d - %s | %d db\n", net, WiFi.SSID(net), WiFi.RSSI(net));
    }
  }
}