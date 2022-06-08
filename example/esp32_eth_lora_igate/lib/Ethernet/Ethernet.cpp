/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Ethernet.cpp
 * Author: ale
 * 
 * Created on 25 mai 2022, 16:06
 */

#include "Ethernet.h"

Ethernet::Ethernet():
eth_connected(false)
{
  anchor = this;
  pinMode(ETH_POWER_PIN, OUTPUT);
  digitalWrite(ETH_POWER_PIN, 1);
  pinMode(NRST, OUTPUT);
  digitalWrite(NRST, LOW);
  delay(200);
  digitalWrite(NRST, HIGH);
  delay(200);
  digitalWrite(NRST, LOW);
  delay(200);
  digitalWrite(NRST, HIGH);
  WiFi.onEvent(Ethernet::WiFiEvent);
  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);  
}

Ethernet::Ethernet(const Ethernet& orig) {
}

Ethernet::~Ethernet() {
}

void Ethernet::WiFiEvent(WiFiEvent_t event)
{
  switch (event)
  {
  case SYSTEM_EVENT_ETH_START:
    Serial.println("ETH Started");
    // set eth hostname here
    ETH.setHostname("esp32-ethernet");
    break;
  case SYSTEM_EVENT_ETH_CONNECTED:
    Serial.println("ETH Connected");
    break;
  case SYSTEM_EVENT_ETH_GOT_IP:
    Serial.print("ETH MAC: ");
    Serial.print(ETH.macAddress());
    Serial.print(", IPv4: ");
    Serial.print(ETH.localIP());
    if (ETH.fullDuplex())
    {
      Serial.print(", FULL_DUPLEX");
    }
    Serial.print(", ");
    Serial.print(ETH.linkSpeed());
    Serial.println("Mbps");
    anchor->eth_connected = true;
    break;
  case SYSTEM_EVENT_ETH_DISCONNECTED:
    Serial.println("ETH Disconnected");
    anchor->eth_connected = false;
    break;
  case SYSTEM_EVENT_ETH_STOP:
    Serial.println("ETH Stopped");
    anchor->eth_connected = false;
    break;
  default:
    break;
  }
}

bool Ethernet::isConnected(){
    return eth_connected;    
}

Ethernet* Ethernet::anchor = NULL;