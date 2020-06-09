#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <espnow.h>
#include "user_interface.h"

uint8_t broadcastAddress[] = {0xCC, 0x50, 0xE3, 0x9C, 0x61, 0xAC};

unsigned long countTotal=0;
unsigned long countSuccess=0;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
    countTotal++;

    Serial.print("Last Packet Send Status: ");
    if (sendStatus == 0){
        Serial.println("Delivery success");
        countSuccess++;
    }
    else{
        Serial.println("Delivery fail");
    }
}

void initEspNow() {
    if (esp_now_init()!=0) {
        Serial.println("*** ESP_Now init failed");
        ESP.restart();
    }

    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_register_send_cb(OnDataSent);

    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void setup() {
    WiFi.mode(WIFI_STA);
    Serial.begin(115200); Serial.println();
    Serial.print("MAC: "); Serial.println(WiFi.macAddress());
    initEspNow();
}

void loop() {
    char testData[32];
    sprintf(testData, "PING %ld of %ld!", countSuccess, countTotal);

    // Send message via ESP-NOW
    int result = esp_now_send(broadcastAddress, (uint8_t *) &testData, sizeof(testData));

    if (result == 0) {
        Serial.println("Sent with success");
    } else {
        Serial.println("Error sending the data");
    }
    delay(2000);
}
