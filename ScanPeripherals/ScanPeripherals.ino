include <LBLE.h>
#include <LBLECentral.h>

#define MAX_ADV_LEN 32

void setup() 
{
    Serial.begin(115200);

    Serial.println("BLE begin");
    LBLE.begin();
    while (!LBLE.ready()) {
        delay(10);
    }

    LBLECentral.scan();
}

void loop() 
{
    LBLECentral.stopScan();

    for (int i = 0; i < LBLECentral.getPeripheralCount(); ++i) {
        printDeviceInfo(i);
        printManufactureData(i);
        Serial.println();
    }

    LBLECentral.clear();

    LBLECentral.scan();

    delay(1000);
}

void printDeviceInfo(int i) 
{
    Serial.print(LBLECentral.getAddress(i));
    Serial.print("\t");
    
    String name = LBLECentral.getName(i);
    Serial.print(name);
    if(name.length() == 0)
    {
        Serial.print("----");
    }
    Serial.print("\t");
    
    Serial.print(LBLECentral.getRSSI(i));
    Serial.print("\t");
}

void printManufactureData(int index)
{
    char buf[MAX_ADV_LEN];
    int  ret = 0;
    char hex[16];
    
    ret = LBLECentral.getRawManufacturerData(index, (uint8_t*) buf, MAX_ADV_LEN);
    for(int i=0; i<ret; i++) 
    {
        sprintf(hex, "%02X", buf[i]); 
        Serial.print(hex);
    } 
}

