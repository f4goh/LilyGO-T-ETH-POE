/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Aprsis.h
 * Author: ale
 *
 * Created on 8 avril 2022, 19:04
 */

#ifndef APRSIS_H
#define APRSIS_H

#include <Arduino.h>
#include <AsyncTCP.h>
#define TOOL_NAME "ESP32-APRS-IS"
#define VERSION "1.1"
#define MSG_MAX_LENGTH 100
//#define APRS_SERVER "euro.aprs2.net"
//#define APRS_PORT 14580
#define BEACON_DELAY 15*60*1000  //15 minutes
#define RECONNECT_DELAY 5*1000  //5 secondes
#define CONNECT_DELAY 2*1000  //2 secondes


class Aprsis {
public:
    Aprsis();
    Aprsis(const Aprsis& orig);
    virtual ~Aprsis();
    bool setup(const String _user,const String _latitude,const String _longitude,const String _message,const String _server,const int _port);

   
    bool isConnected();

    void setRssi(int _rssi);
    static QueueHandle_t queueMsg;
    
private:
    TaskHandle_t TaskHandle_Ait;
    static Aprsis* anchor;
    static void marshall(void *);
    void aprsIsTask();
    String passCode(String call);
    
    
    String user;
    String passcode;
    String tool_name;
    String version;
    String longitude;
    String latitude;
    String message;
    String serverAprs;
    int portAprs;
    int rssi;
    bool cnxState;
    char msg[MSG_MAX_LENGTH];
    
};

#endif /* APRSIS_H */
