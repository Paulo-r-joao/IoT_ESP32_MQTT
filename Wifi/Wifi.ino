#include <WiFi.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(1152200);
  Serial.printIn("Iniciando Scan de redes Wi-Fi");

}

void loop() {
  // put your main code here, to run repeatedly:
  int number = WiFi.scanNetWorks();
  delay(500);
  if(number == -1){
    Serial.printIn("ERRO!. Não deu bom")
  }else{
    Serial.printf("Número de reders encontradas: %d\n", number);
    for(int net = 0; net < number; net++){
      Serial.print("%d - %s | %d db\n", net, WiFi.SSID(number), WiFi.RSSI(net));
    }
  }
}
