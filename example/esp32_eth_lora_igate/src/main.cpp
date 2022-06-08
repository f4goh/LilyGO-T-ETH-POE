/* 
 * File:   main.cpp
 * Author: ale
 * 
 * Created on 25 mai 2022, 16:06
 * Test Ethernet ESP32 class
 * 
 * check ethernet connexion and reconnect automatically
 * todo
 * change afficheur class
 */

#include <Arduino.h>
#include <Ethernet.h>
#include <Aprsis.h>
#include <LoraAprs.h>
#include <Preferences.h>
#include <Menu.h>               // Classe commandes via la console
#include <DecodeAprs.h>
#include <Digipeater.h>
#include <ESP32Time.h>

#define BEACON_INTERVAL 15


#define LED 16 //green led 

Ethernet * ethernet;
Aprsis *is;
LoraAprs lora;
Preferences configuration;
Menu *leMenu;
//DecodeAprs *afficheur;
Digipeater *digi;

ESP32Time rtc;

void sendBeacon(){
     char * pdu;
     pdu=digi->getBeacon();
     Serial.println(pdu);
     lora.send(pdu,strlen(pdu));
}




void setup() {
    Serial.begin(115200);
    Serial.println(ESP.getFreeHeap());    
    pinMode(LED, OUTPUT);
    rtc.setTime(30, 59, 15, 17, 1, 2021);  // 17th Jan 2021 15:59:30 date bidon    
    lora.setup();

    //afficheur = new DecodeAprs;
    leMenu = new Menu;  // Menu de configuration
    leMenu->setup();
    
    configuration.begin("igate", false);        
    digi = new Digipeater(configuration.getString("call"),configuration.getString("latitude"),configuration.getString("longitude"),configuration.getString("comment"));
    

    if (configuration.getBool("internet")) {
        ethernet = new Ethernet;

        while (!ethernet->isConnected()) {
            Serial.println("Wait Ethernet connexion");
            delay(500);
        }

        bool ret;
        is = new Aprsis;
        ret = is->setup(configuration.getString("call"), configuration.getString("latitude"), 
                            configuration.getString("longitude"), configuration.getString("comment"),
                            configuration.getString("server"),configuration.getInt("port"));
        if (ret) {
            Serial.println("task APRS IS ok");
        } else Serial.println("Internet igate server error");
    }
    
    
}

void loop() {
    
    char * pdu;
    char msg[MSG_MAX_LENGTH];
    char car;
    if (Serial.available() > 0) {
        car = Serial.read();
        if (car == 'b') { //test beacon
            sendBeacon();
        }
    }

    /*
    if (configuration.getBool("internet")) {
        afficheur->setCnxState(ethernet->isConnected(), is->isConnected());
    }
    */
    
    if (xQueueReceive(lora.queueRx, msg, 0) == pdPASS) {
        Serial.println("msg queue");
        Serial.println(msg);
        Serial.print("with RSSI ");
        Serial.println(lora.packetRssi());
        //afficheur->setRssi(lora.packetRssi());
        //afficheur->displayRssi(true);
        //afficheur->parse(msg);
        
        if (configuration.getBool("internet")) {
            if (ethernet->isConnected() && is->isConnected()) {
                is->setRssi(lora.packetRssi());
                xQueueSend(is->queueMsg, msg, 0); //via internet
            }
        }
        if (configuration.getBool("digi")) { //digi
            if (digi->routing(msg)) {
                pdu = digi->getMsg();
                Serial.println(pdu);
                digitalWrite(LED, HIGH);
                lora.send(pdu, strlen(pdu));
                digitalWrite(LED, LOW);
            }
        }
    }

    
    //beaconing every 15 min
    if (configuration.getBool("digi")) {
        if (((rtc.getMinute() % BEACON_INTERVAL) == 0) && (rtc.getSecond() == 0)) {
            sendBeacon();
        }
    }
    //delay(1);
}
