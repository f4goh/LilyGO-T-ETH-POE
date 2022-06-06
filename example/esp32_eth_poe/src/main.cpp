/* 
 * File:   main.cpp
 * Author: ale
 * 
 * Created on 25 mai 2022, 16:06
 * Test Ethernet ESP32 class
 * 
 * check ethernet connexion and reconnect automatically
 * 
 */

#include <Arduino.h>
#include <Ethernet.h>
#include <Aprsis.h>



Ethernet * ethernet;
Aprsis *is;

void setup() {
    Serial.begin(115200);
    ethernet = new Ethernet;

    while (!ethernet->isConnected()) {

    }
    
    bool ret;
    is = new Aprsis;
    ret = is->setup("F4GOH-2", "4753.41N", "00016.61E", "LoRa iGATE : https://github.com/f4goh/lora-aprs-esp32", "euro.aprs2.net", 14580);
    if (ret) {
        Serial.println("task APRS IS ok");
    } else Serial.println("Internet igate server error");

}

void loop() {


}
