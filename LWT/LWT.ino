#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// tempo
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000);

const String SSID = "";
const String PSWD = "";

const String brokerUrl = "test.mosquitto.org";  //URL do broker (servidor)
const int port = 1883;                          //Porta do broker (servidor)

// const char* Topic_LWT = "Aula/Boards/Paulo/Status";
const int QoS_LWT = 1;
const bool Retain_LWT = true;

const char* Msg_LWT = "Offline";

const char* Topic_LWT1 = "TDESI1v1/Placa1/vini_paulo/status";
const char* Topic_LWT2 = "TDESI1v1/Placa2/bruno/status";

JsonDocument doc;

// Status placas
String status1 = "";
String status2 = "";


//OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 //Largura da tela em px
#define SCREEN_HEIGHT 64 //Altura da tela em px

#define I2C_SCK 22
#define I2C_SDA 21
Adafruit_SSD1306 tela(SCREEN_WIDTH,SCREEN_HEIGHT, &Wire, -1);

WiFiClient espClient;                //Criando Cliente WiFi
PubSubClient mqttClient(espClient);  //Criando Cliente MQTT

void scanLocalworks();
void connectLocalworks();
void connectBroker();

void setup() {
  Serial.begin(115200);
  scanLocalworks();
  connectLocalworks();
  connectBroker();
  
  Wire.begin(I2C_SDA,I2C_SCK); //Inicia comunicação I2C
  tela.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  tela.clearDisplay();
  tela.setTextSize(1.0);
  tela.setTextColor(SSD1306_WHITE);
  tela.clearDisplay();
  delay(100);

  timeClient.begin();
  timeClient.setTimeOffset(-10000);
}


void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Conexão Perdida\n");
    connectLocalworks();
    
  }
  if (!mqttClient.connected()) {
    Serial.println("Erro de Conexão no Broker");
    connectBroker();
  }
  delay(1000);
  Serial.println(status1);
  lcd();

  mqttClient.loop();
}

void lcd(){
  tela.clearDisplay();
  timeClient.update();
  tela.setCursor(0,0);
  tela.println("Sistema Monitoramento");

  tela.setCursor(0,10);
  tela.println("---------------------");

  tela.setCursor(0,20);
  tela.println("Placa 1: ");
  tela.setCursor(50,20);
  tela.println(status1);

  tela.setCursor(0,30);
  tela.println("Placa 2: ");
  tela.setCursor(50,30);
  tela.println(status2);

  tela.setCursor(0,40);
  tela.println("---------------------");

  tela.setCursor(0,50);
  tela.println("Atualizado: ");
  tela.setCursor(70,50);
  tela.print(timeClient.getFormattedTime());

  tela.display();
}

void connectLocalworks() {
  Serial.println("Iniciando conexão com rede WiFi");
    WiFi.begin(SSID, PSWD);
    
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConectado!");
  status1 = "Online";
}

void scanLocalworks() {
  Serial.println("Iniciando Scan de redes Wi- Fi");
  status1 = "Offline";
  int number = WiFi.scanNetworks();
  delay(500);
  if (number == -1) {
    Serial.println("ERRO!. Não deu boa");
  } else {
    for (int net = 0; net < number; net++) {
      Serial.printf("cont: %d", net);
      Serial.printf("---- %d - %s | %d db\n", net, WiFi.SSID(net), WiFi.RSSI(net));
    }
  }
}

void connectBroker() {
  Serial.println("Conectando ao broker");
  mqttClient.setServer(brokerUrl.c_str(), port);
  String userId = "ESP-alves9";
  userId += String(random(0xffff), HEX);

  // json
  StaticJsonDocument<200>doc;
    doc["status"] = Msg_LWT;
    char buffer[200];
    serializeJson(doc,buffer);

    
  while (!mqttClient.connected()) {
    mqttClient.connect(
      userId.c_str(),
      "",
      "",
      Topic_LWT1,
      QoS_LWT,
      Retain_LWT,
      buffer
    );
    Serial.println(".");
    
  delay(5000);
  }

  doc["status"] = "Online";
  serializeJson(doc, buffer);

  mqttClient.publish(Topic_LWT1, buffer, Retain_LWT);
  mqttClient.subscribe(Topic_LWT2,QoS_LWT);
  mqttClient.setCallback(callback);
  Serial.print("Conectado com sucesso!");

  
}

void callback(char* Topic_LWT2, byte* payload, unsigned long length){
  String resposta = "";
  for(int i = 0; i < length;i++){
    // ele vai pegar um byte e  e transformar em letra
    resposta += (char) payload[i];
  }
  Serial.println(resposta);
  DeserializationError error = deserializeJson(doc, resposta);

  if(!error){
    String placa2 = doc["status"].as<String>();
    status2 = placa2;
  }else if(error){
    Serial.println("erro 👍");
  }
}



