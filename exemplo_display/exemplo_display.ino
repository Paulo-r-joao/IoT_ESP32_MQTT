#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // Largura da tela
#define SCREEN_HEIGHT 64 // Altura da tela

#define I2C_SCK 6
#define I2C_SDA 5

Adafruit_SSD1306 tela(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA,I2C_SCK);
  tela.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  tela.clearDisplay();
  tela.setTextSize(2);
  tela.setTextColor(SSD1306_WHITE);
  tela.setCursor(0,0);
  tela.println("Hello World!");
  tela.display();
}

void loop() {
  tela.clearDisplay();
  tela.setCursor(40,20);
  tela.print(millis()/1000);
  tela.println("s");
  tela.display();
}
