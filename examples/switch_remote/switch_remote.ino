/**
 * @example switch_remote.ino
 *
 * Show how to use API of This Library. 
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/14
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include <Switch.h>

#define SWITCH_ID            "5580000003"
#define SWITCH_CHECK_CODE    "86f32f24-70bc-44e0-82f2-e95da6f8cde5"
#define SWITCH_APIKEY        "d0555f12-a67c-4c54-9ee0-8f5b7f4268fa"
#define WIFI_SSID           "ITEAD"
#define WIFI_PASS           "12345678"
#define IOT_SERVER          "172.16.7.6"

#define SWITCH_PIN           (13)

Switch sw(SWITCH_PIN);

void setup()
{
    const char *apikey;
    sw.setServer(IOT_SERVER);
    if (!sw.connectWiFi(WIFI_SSID, WIFI_PASS))
    {
        Serial.println("connectWiFI error and halt...");
        while(1);
    }

    Serial.println("Connecting device to server...");
    apikey = sw.init(SWITCH_ID, SWITCH_APIKEY);
    //apikey = sw.init(SWITCH_ID, SWITCH_APIKEY, DEVICE_PRODUCT);
    Serial.print("apikey = ");
    Serial.println(apikey);    
    
    Serial.println("setup done.");
}

void loop()
{
    static int32_t counter = 0;
    Serial.print("\ncounter = ");
    Serial.println(++counter);
    int32_t ret1;
    int32_t ret2;

    ret1 = sw.setState(SWITCH_STATE_ON);
    ret2 = sw.sync();
    if(!ret1 && !ret2)
    {
        Serial.println("on");
    }
    else
    {
        Serial.println("sync() err!");
    }
    
    delay(1000);
    
    ret1 = sw.setState(SWITCH_STATE_OFF);
    ret2 = sw.sync();
    if(!ret1 && !ret2)
    {
        Serial.println("off");
    }
    else
    {
        Serial.println("sync() err!");
    }
    
    delay(1000);
}