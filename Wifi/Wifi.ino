#include <WiFi.h>

void scanLocalNetworks();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(1152200);
  Serial.println("Iniciando Scan de redes Wi-Fi");

}

void loop() {
  // put your main code here, to run repeatedly:


void scanLocalNetworks(){
  int number = WiFi.scanNetworks();
  delay(500);
  if(number == -1){
    Serial.println("ERRO!. Não deu bom");
  }else{
    Serial.printf("Número de reders encontradas: %d\n", number);
    for(int net = 0; net < number; net++){
      Serial.printf("%d - %s | %d db\n", net, WiFi.SSID(number), WiFi.RSSI(net));
    }
  }
}
}