#include <LFlash.h>

void setup() 
{
    uint8_t  data[32];
    uint32_t data_len;
    int ret;
    char msg[32];
    
    Serial.begin(115200);
    /*
    ret = LFlash.begin();
    if(ret != LFLASH_OK) {
        Serial.print("LFlash Error ");
        Serial.println(ret);
        return;
    }
    */
    ret = LFlash.read("WiFi", "SSID", data, &data_len);
    
    if(ret == LFLASH_ITEM_NOT_FOUND) {
        ret = LFlash.write("WiFi", "SSID", LFLASH_STRING_DATA, (uint8_t*) "MikeAP", 6);
        if(ret == LFLASH_OK) {
            Serial.println("LFlash Write OK");
        }
    }
    
    if(ret != LFLASH_OK) {
        Serial.print("LFlash Error ");
        Serial.println(ret);
        return;
    }
    
    Serial.println("LFlash Read OK");
    Serial.print("data = ");
    sprintf(msg, "%s", data);
    Serial.println(msg);
    Serial.print("data_len = ");
    Serial.println(data_len);
}

void loop() 
{

}
