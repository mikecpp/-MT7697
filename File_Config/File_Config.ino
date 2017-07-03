#include "config.h"

void setup() 
{
    Serial.begin(115200);
    
    if(config_init() != 0) {
        Serial.println("config_init() fail !");
    }

    char msg[64];
    char ssid[32], pass[32];
    int  ssid_len, pass_len;

    if(config_read("ssid", (char*) ssid, &ssid_len) <= 0) {
        Serial.println("ssid not found !");
    }
    else {
        sprintf(msg, "SSID = %s (%d)", ssid, ssid_len);
        Serial.println(msg);
    }

    if(config_read("pass", (char*) pass, &pass_len) <= 0) {
        Serial.println("pass not found !");
    }
    else {
        sprintf(msg, "PASS = %s (%d)", pass, pass_len);
        Serial.println(msg);
    }
}

void loop() 
{

}
