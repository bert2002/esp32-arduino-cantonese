#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "SSD1306.h"

SSD1306 display(0x3c, 5, 4);

char *ssid      = "ESSID";
char *password  = "PASSWORD";

void setup() {
  display.init();
  display.flipScreenVertically();
  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_24);
  Serial.begin(115200);
  Serial.println();

  display.display();
  display.clear();
  display.drawString(64, 20, String("Booted"));
  delay(2000);
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 20, String("Wi-Fi"));
  delay(2000);

  WiFi.mode(WIFI_STA);
  WiFi.begin (ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

}

void loop() {

  DynamicJsonDocument data(2048);
  HTTPClient http;
  http.begin("https://raw.githubusercontent.com/bert2002/esp32-arduino-cantonese/master/cantonese.json");
  int httpCode = http.GET();

  if (httpCode > 0) {
    String payload = http.getString();
    http.end();

    deserializeJson(data, payload);
    int Size = data.size() - 1;
  
    int RandomNumber = random(0, Size);

    // check string length
    String Cantonese = data[RandomNumber]["zh"];
    int CantoneseLength = Cantonese.length();
    //Serial.println(CantoneseLength);
    
    display.clear();
    if (CantoneseLength > 10) {
      display.setFont(ArialMT_Plain_16);
    } else if (CantoneseLength > 15) {
      display.setFont(ArialMT_Plain_10);
    } else {
      display.setFont(ArialMT_Plain_24);
    }
    //display.drawString(64, 5, Cantonese);
    display.drawStringMaxWidth(64, 5, 120,Cantonese);
    
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, data[RandomNumber]["en"]);

  } else {
    Serial.println("Error on HTTP request");
    ESP.restart();
  }
  
  display.display();
  delay(60000); // 1 minute
}
