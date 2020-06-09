#include <Arduino.h>

#include <esp_now.h>
#include <WiFi.h>
#include <HardwareSerial.h>

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    Serial.print("$$");
    Serial.write(mac, 6);
    Serial.write(len);
    Serial.write(incomingData, len);
}

void initEspNow() {
    if (esp_now_init() != ESP_OK) {
        Serial.println("*** ESP_Now init failed");
        ESP.restart();
    }

    esp_now_register_recv_cb(OnDataRecv);
}

void setup() {
    WiFi.mode(WIFI_STA);
    Serial.begin(115200);
    Serial.println();
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
    initEspNow();
}

int heartBeat;

void loop() {
    if (millis() - heartBeat > 3000) {
        Serial.println("Waiting for ESP-NOW messages...");
        heartBeat = millis();
    }
}