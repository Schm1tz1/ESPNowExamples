#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <espnow.h>
#include "user_interface.h"

void initEspNow() {
    if (esp_now_init()!=0) {
        Serial.println("*** ESP_Now init failed");
        ESP.restart();
    }

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

    esp_now_register_recv_cb([](uint8_t *mac, uint8_t *data, uint8_t len) {

        Serial.print("$$");
        Serial.write(mac, 6);
        Serial.write(len);
        Serial.write(data, len);

    });
}

void setup() {
    WiFi.mode(WIFI_STA);
    Serial.begin(115200); Serial.println();
    Serial.print("MAC: "); Serial.println(WiFi.macAddress());
    initEspNow();
}

int heartBeat;

void loop() {
    if (millis()-heartBeat > 3000) {
        Serial.println("Waiting for ESP-NOW messages...");
        heartBeat = millis();
    }
}
