/* 
 * File:   main.cpp
 * Author: ale
 * 
 * Created on 25 mai 2022, 16:06
 * Test Ethernet ESP32 class
 */

#include <Arduino.h>
#include <Ethernet.h>

bool eth_done = false;


Ethernet * ethernet;


void setup()
{
  Serial.begin(115200);
  ethernet = new Ethernet;
}

void testClient(const char *host, uint16_t port)
{
  Serial.print("\nconnecting to ");
  Serial.println(host);

  WiFiClient client;
  if (!client.connect(host, port))
  {
    Serial.println("connection failed");
    return;
  }
  client.printf("GET / HTTP/1.1\r\nHost: %s\r\n\r\n", host);
  while (client.connected() && !client.available());
  while (client.available())
  {
    Serial.write(client.read());
  }

  Serial.println("closing connection\n");
  client.stop();
}

void loop()
{

  if (ethernet->isConnected() && !eth_done)
  {
    testClient("touchardinforeseau.servehttp.com", 80);
    eth_done = true;
  }
 
}
